/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include <iostream>
#include <fstream>
#include <memory>
#include <limits>
#include <cmath>
#include <cpuid.h>

#include "external/argparse.h"

#include "disasm.hpp"

#define STACK_SIZE 0x2000
#define PAGE_MASK 0xFFFFFFFFFFFFF000

static const long NSEC_PER_SEC = (1000ULL * 1000ULL * 1000ULL);

static const char *gSpaces[16] =
{
    "",
    "  ",
    "    ",
    "      ",
    "        ",
    "          ",
    "            ",
    "              ",
    "                ",
    "                  ",
    "                    ",
    "                      ",
    "                        ",
    "                          ",
    "                            ",
    "                              ",
};


struct options {
    size_t rip;
    size_t offset;
    size_t size;
    size_t count;

    uint8_t bits;

    uint8_t vendor;
    bool no_color;
    bool verbose;
    bool dump_stats;
    bool interactive;
    bool comm;
    bool json_output;
    bool extended;

    bool shemu;
    std::string shctx;
    bool kernel;

    std::string in_file;
    std::string hex_string;
    std::string hex_file;
    std::string shctx_file;
    std::string shdec_file;

    // From here on, these are set internally
    std::unique_ptr<uint8_t[]> bytes;
    size_t actual_size;
    int address_size;

    bool output_redirected;
};


extern "C"
{
#if !defined(BDDISASM_HAS_VSNPRINTF)
    int nd_vsnprintf_s(char *buffer, size_t sizeOfBuffer, [[maybe_unused]] size_t count, const char *format, va_list argptr)
    {
        return vsnprintf(buffer, sizeOfBuffer, format, argptr);
    }
#endif // !defined(BDDISASM_HAS_VSNPRINTF)

#if !defined(BDDISASM_HAS_MEMSET)
    void *
    nd_memset(void *s, int c, size_t n)
    {
        return memset(s, c, n);
    }
#endif // !defined(BDDISASM_HAS_MEMSET)
}

static bool _hexstring_to_bytes(options &opts)
{
    if (!opts.hex_file.empty()) {
        auto f = std::ifstream(opts.hex_file, std::ios::in);

        if (!f.is_open()) {
            std::cerr << "Failed to open file " << opts.hex_file << std::endl;
            return false;
        }

        f.seekg(0, std::ios::end);
        opts.hex_string.reserve(f.tellg());
        f.seekg(0, std::ios::beg);

        opts.hex_string.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    }

    if (opts.hex_string.empty())
        return false;

    opts.actual_size = 0;
    opts.hex_string.erase(std::remove_if(opts.hex_string.begin(), opts.hex_string.end(), isspace), opts.hex_string.end());

    // This is the maximum size, not the actual size
    auto initial_size = opts.hex_string.length() / 2 + 1;

    opts.bytes = std::make_unique<uint8_t[]>(initial_size);
    auto bytes = opts.bytes.get();

    for (size_t i = 0; i < opts.hex_string.length(); i += 2) {
        auto pair = opts.hex_string.substr(i, 2);
        char *end_ptr;

        if (pair == "0x" || pair == "0X" || pair == "\\x")
            continue;

        auto b = static_cast<uint8_t>(std::strtoul(pair.c_str(), &end_ptr, 16));

        size_t conv_size = static_cast<size_t>(end_ptr - pair.c_str());

        if (conv_size != pair.length()) {
            std::cerr << "Trying to convert invalid hex number: " << pair << std::endl;
            return false;
        }

        bytes[opts.actual_size++] = b;
    }

    return true;
}

static bool _load_shctx(options &opts)
{
    if (opts.shctx_file.empty())
        return false;

    auto f = std::ifstream(opts.shctx_file, std::ios::in);
    if (!f.is_open()) {
        std::cout << "Failed to open file " << opts.shctx_file << std::endl;
        return false;
    }

    f.seekg(0, std::ios::end);
    opts.shctx.reserve(f.tellg());
    f.seekg(0, std::ios::beg);

    opts.shctx.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    return true;
}

static bool _file_to_bytes(options &opts)
{
    auto f = std::ifstream(opts.in_file, std::ios::in | std::ios::binary);

    if (!f.is_open()) {
        std::cerr << "Failed to open file " << opts.in_file << std::endl;
        return false;
    }

    f.seekg(0, std::ios::end);

    opts.actual_size = f.tellg();

    if (opts.offset >= opts.actual_size) {
        std::cerr << "Offset bigger than file size" << std::endl;
        return false;
    }

    f.seekg(0, std::ios::beg);

    opts.bytes = std::make_unique<uint8_t[]>(opts.actual_size);

    f.read(reinterpret_cast<char*>(opts.bytes.get()), opts.actual_size);

    return true;
}

// Don't change the order (on linux these values are color codes)
enum Colors {
    Reset = 0,
#if defined(_WIN32)
    Red     = FOREGROUND_INTENSITY | FOREGROUND_RED,
    Green   = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    Yellow  = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    Blue    = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    Magenta = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED,
    Cyan    = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN,
    White   = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
#elif defined(__unix__)
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
#endif
};


static void _set_text_color(Colors color)
{
#if defined(_WIN32)
    static WORD old_attrs = -1;

    if (old_attrs == -1) {
        CONSOLE_SCREEN_BUFFER_INFO buffer_info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buffer_info);
        old_attrs = buffer_info.wAttributes;
    }

    if (color == Reset)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), old_attrs);
    else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#elif defined(__unix__)
    if (color == Reset)
        std::cout << "\033[m";
    else
        std::cout << "\033[0;3" << color << "m";
#endif
}


static struct timespec diff_time(struct timespec const &end, struct timespec const &start)
{
    struct timespec result;

    if (end.tv_nsec > start.tv_nsec) {
        result.tv_sec = end.tv_sec - start.tv_sec;
        result.tv_nsec = end.tv_nsec - start.tv_nsec;
    } else {
        result.tv_sec = end.tv_sec - start.tv_sec - 1;
        result.tv_nsec = NSEC_PER_SEC + end.tv_nsec - start.tv_nsec;
    }

    return result;
}


void print_instruction(const size_t rip, INSTRUX *instrux, const options &opts)
{
    char instruxText[ND_MIN_BUF_SIZE];
    uint32_t k = 0;

    printf("%*zx ", opts.address_size, rip);

    if (!opts.no_color)
    {
        _set_text_color(Magenta);
        for (uint32_t idx = 0; idx < instrux->PrefLength; idx++, k++)
        {
            printf("%02x", instrux->InstructionBytes[k]);
        }

        _set_text_color(Green);
        for (uint32_t idx = 0; idx < (DWORD)(ND_IS_3DNOW(instrux) ? instrux->OpLength - 1 : instrux->OpLength); idx++, k++)
        {
            printf("%02x", instrux->InstructionBytes[k]);
        }

        _set_text_color(Yellow);
        for (uint32_t idx = 0; idx < (DWORD)(instrux->HasModRm + instrux->HasSib); idx++, k++)
        {
            printf("%02x", instrux->InstructionBytes[k]);
        }

        _set_text_color(Blue);
        for (uint32_t idx = 0; idx < (DWORD)(instrux->DispLength); idx++, k++)
        {
            printf("%02x", instrux->InstructionBytes[k]);
        }

        _set_text_color(Red);
        for (uint32_t idx = 0; idx < (DWORD)(instrux->Imm1Length + instrux->Imm2Length +
            instrux->RelOffsLength + instrux->MoffsetLength +
            instrux->HasSseImm + instrux->AddrLength); idx++, k++)
        {
            printf("%02x", instrux->InstructionBytes[k]);
        }

        if (ND_IS_3DNOW(instrux))
        {
            _set_text_color(Green);
            for (; k < instrux->Length; k++)
            {
                printf("%02x", instrux->InstructionBytes[k]);
            }
        }

        _set_text_color(Reset);
    }

    for (; k < instrux->Length; k++)
    {
        printf("%02x", instrux->InstructionBytes[k]);
    }

    std::cout << gSpaces[16 - instrux->Length];

    NdToText(instrux, rip, sizeof(instruxText), instruxText);

    std::cout << instruxText << std::endl;

    if (opts.extended) {
        const uint8_t opsize[3] = { 2, 4, 8 };
        const uint8_t adsize[3] = { 2, 4, 8 };
        const uint8_t veclen[3] = { 16, 32, 64 };

        printf("         DSIZE: %2d, ASIZE: %2d, VLEN: ",
            opsize[instrux->EfOpMode] * 8, adsize[instrux->AddrMode] * 8);

        if (ND_HAS_VECTOR(instrux)) {
            printf("%2d\n", veclen[instrux->VecMode] * 8);
        } else {
            printf("-\n");
        }

        printf("         ISA Set: %s, Ins cat: %s, Ins class: %d, CET tracked: %s\n",
               ins_set_to_str(instrux->IsaSet).c_str(), ins_cat_to_str(instrux->Category).c_str(), instrux->Instruction,
               instrux->IsCetTracked ? "yes" : "no");

        if (0 != instrux->CpuidFlag.Flag) {
            const char *regs[4] = { "eax", "ecx", "edx", "ebx" };

            printf("         CPUID leaf: 0x%08x", instrux->CpuidFlag.Leaf);

            if (instrux->CpuidFlag.SubLeaf != ND_CFF_NO_SUBLEAF)
            {
                printf(", sub-leaf: 0x%08x", instrux->CpuidFlag.SubLeaf);
            }

            printf(", reg: %s, bit %u\n", regs[instrux->CpuidFlag.Reg], instrux->CpuidFlag.Bit);
        }

        printf("         FLAGS access: ");

        uint32_t all = instrux->FlagsAccess.Tested.Raw | instrux->FlagsAccess.Modified.Raw | instrux->FlagsAccess.Set.Raw
                       | instrux->FlagsAccess.Cleared.Raw | instrux->FlagsAccess.Undefined.Raw;
        const char *flags[22] = { "CF", nullptr, "PF", nullptr, "AF", nullptr, "ZF", "SF", "TF", "IF", "DF", "OF", "IOPL", nullptr, "NT", nullptr, "RF", "VM", "AC", "VIF", "VIP", "ID" };

        for (uint32_t fidx = 0; fidx < 21; fidx++) {
            if (flags[fidx] != nullptr) {
                if (0 == (all & (1ULL << fidx))) {
                    continue;
                }

                printf("%s: ", flags[fidx]);

                if (instrux->FlagsAccess.Tested.Raw & (1ULL << fidx)) {
                    printf("t");
                }

                if (instrux->FlagsAccess.Modified.Raw & (1ULL << fidx)) {
                    printf("m");
                }

                if (instrux->FlagsAccess.Set.Raw & (1ULL << fidx)) {
                    printf("1");
                }

                if (instrux->FlagsAccess.Cleared.Raw & (1ULL << fidx)) {
                    printf("0");
                }

                if (instrux->FlagsAccess.Undefined.Raw & (1ULL << fidx)) {
                    printf("u");
                }

                printf("; ");
            }
        }

        printf("\n");

        printf("         Valid modes: R0: %s, R1: %s, R2: %s, R3: %s, Real: %s, V8086: %s, Prot: %s, Compat: %s, Long: %s, SMM: %s, SGX: %s, TSX: %s, VMXRoot: %s, VMXNonRoot: %s\n",
            instrux->ValidModes.Ring0 ? "yes" : "no",
            instrux->ValidModes.Ring1 ? "yes" : "no",
            instrux->ValidModes.Ring2 ? "yes" : "no",
            instrux->ValidModes.Ring3 ? "yes" : "no",
            instrux->ValidModes.Real ? "yes" : "no",
            instrux->ValidModes.V8086 ? "yes" : "no",
            instrux->ValidModes.Protected ? "yes" : "no",
            instrux->ValidModes.Compat ? "yes" : "no",
            instrux->ValidModes.Long ? "yes" : "no",
            instrux->ValidModes.Smm ? "yes" : "no",
            instrux->ValidModes.Sgx ? "yes" : "no",
            instrux->ValidModes.Tsx ? "yes" : "no",
            instrux->ValidModes.VmxRoot ? "yes" : "no",
            instrux->ValidModes.VmxNonRoot ? "yes" : "no");

        for (uint8_t i = 0; i < instrux->OperandsCount; i++) {
            printf("         Operand %d  %s  Type: %10s, Size: %2d, RawSize: %2d, Encoding: %s", i,
                   instrux->Operands[i].Access.Read && instrux->Operands[i].Access.Write ? "RW" :
                   instrux->Operands[i].Access.Write ? "-W" : instrux->Operands[i].Access.Read ? "R-" : "--",
                   op_type_to_str(instrux->Operands[i].Type).c_str(), (int)instrux->Operands[i].Size,
                   (int)instrux->Operands[i].RawSize, op_enc_to_str(instrux->Operands[i].Encoding).c_str());

            if (ND_OP_MEM == instrux->Operands[i].Type) {
                printf(", ");

                if (instrux->Operands[i].Info.Memory.IsAG) {
                    printf("Address Generator, ");
                }

                if (instrux->Operands[i].Info.Memory.IsBitbase) {
                    printf("Bitbase Addressing, ");
                }

                if (instrux->Operands[i].Info.Memory.IsMib) {
                    printf("MIB Addressing, ");
                }

                if (instrux->Operands[i].Info.Memory.IsVsib) {
                    printf("VSIB Addressing, ");
                }

                if (instrux->Operands[i].Info.Memory.IsStack) {
                    printf("Stack, ");
                }

                if (instrux->Operands[i].Info.Memory.IsShadowStack) {
                    printf("Shadow Stack, ");
                }
            }

            if (ND_OP_REG == instrux->Operands[i].Type) {
                printf(", RegType: %16s, RegSize: %2u, ",
                       reg_type_to_str(instrux->Operands[i].Info.Register.Type).c_str(),
                       instrux->Operands[i].Info.Register.Size);
                if (instrux->Operands[i].Info.Register.Type == ND_REG_MSR)
                {
                    printf("RegId: 0x%08x, RegCount: %u\n",
                           instrux->Operands[i].Info.Register.Reg,
                           instrux->Operands[i].Info.Register.Count);
                }
                else
                {
                    printf("RegId: %u, RegCount: %u\n",
                           instrux->Operands[i].Info.Register.Reg,
                           instrux->Operands[i].Info.Register.Count);
                }
            }

            printf("\n");

            if (instrux->Operands[i].Decorator.HasBroadcast) {
                printf("                Decorator: Broadcast %d bytes element %d times\n",
                    instrux->Operands[i].Decorator.Broadcast.Size,
                    instrux->Operands[i].Decorator.Broadcast.Count);
            }

            if (instrux->Operands[i].Decorator.HasMask) {
                printf("                Decorator: Mask k%d\n", instrux->Operands[i].Decorator.Mask.Msk);
            }

            if (instrux->Operands[i].Decorator.HasZero) {
                printf("                Decorator: Zero (no merging)\n");
            }
        }

        printf("\n");
    }
}


StringBuffer disassemble_one(uint8_t *bytes, size_t size, size_t rip, uint8_t bits, uint8_t vendor /* = ND_VEND_INTEL */)
{
    INSTRUX instrux;

    auto status = NdDecodeEx2(&instrux, bytes, size, bits, bits, bits, vendor);
    if (!ND_SUCCESS(status))
        return byte_to_json(bytes[0], rip);
    else
        return instrux_to_json(&instrux, rip);
}

void shemu_log(PCHAR msg)
{
    printf("%s", msg);
}

ND_BOOL shemu_access_mem(void * /* Ctx */, uint64_t /* Gla */, size_t Size, uint8_t *Buffer, ND_BOOL Store)
{
    if (!Store)
    {
        memset(Buffer, 0, Size);
    }

    return ND_TRUE;
}

void shemu(options &opts)
{
    SHEMU_CONTEXT ctx = { };
    SHEMU_STATUS shstatus;

    auto stack = std::make_unique<uint8_t[]>(STACK_SIZE);
    auto intbuf = std::make_unique<uint8_t[]>(opts.actual_size + STACK_SIZE);
    auto shellcode = std::make_unique<uint8_t[]>(opts.actual_size);

    ctx.Stack = stack.get();
    memset(ctx.Stack, 0, STACK_SIZE);

    ctx.Intbuf = intbuf.get();
    memset(ctx.Intbuf, 0, opts.actual_size + STACK_SIZE);

    ctx.Shellcode = shellcode.get();
    memcpy(ctx.Shellcode, opts.bytes.get(), opts.actual_size);

    ctx.ShellcodeSize = opts.actual_size;
    ctx.StackSize = STACK_SIZE;
    ctx.Registers.RegRsp = 0x101000;
    ctx.IntbufSize = opts.actual_size + STACK_SIZE;

    ctx.Registers.RegFlags = NDR_RFLAG_IF | 2;
    ctx.Registers.RegRip = opts.rip ? opts.rip : 0x200000;
    ctx.Segments.Cs.Selector = 0x10;
    ctx.Segments.Ds.Selector = 0x28;
    ctx.Segments.Es.Selector = 0x28;
    ctx.Segments.Ss.Selector = 0x28;
    ctx.Segments.Fs.Selector = 0x30;
    ctx.Segments.Fs.Base = 0x7FFF0000;
    ctx.Segments.Gs.Selector = 0x30;
    ctx.Segments.Gs.Base = 0x7FFF0000;

    // Dummy values, to resemble regular CR0/CR4 values.
    ctx.Registers.RegCr0 = 0x0000000080050031;
    ctx.Registers.RegCr4 = 0x0000000000170678;

    ctx.Mode = opts.bits;
    ctx.Ring = opts.kernel ? 0 : 3;
    ctx.TibBase = (opts.bits == ND_DATA_32 ? ctx.Segments.Fs.Base : ctx.Segments.Gs.Base);
    ctx.MaxInstructionsCount = 4096;
    ctx.Options = SHEMU_OPT_TRACE_EMULATION;
    ctx.Log = shemu_log;
    ctx.AccessMemory = shemu_access_mem;
    
    uint32_t eax, ebx, ecx, edx;

    eax = ebx = ecx = edx = 0;
    
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
    
    if (!!(ecx & (1UL << 25)))
    {
        ctx.Options |= SHEMU_OPT_SUPPORT_AES;
    }

    // Configurable thresholds.
    ctx.NopThreshold = SHEMU_DEFAULT_NOP_THRESHOLD;
    ctx.StrThreshold = SHEMU_DEFAULT_STR_THRESHOLD;
    ctx.MemThreshold = SHEMU_DEFAULT_MEM_THRESHOLD;

    bool update_rsp = false;

    if (!opts.shctx.empty() && !regs_from_json(opts.shctx, ctx.Registers, update_rsp)) {
        std::cerr << "Failed to parse context file." << std::endl;
        return;
    }

    // Consider the stack base at least with a page before the current RSP. In case of pushes or operations
    // which decrease the RSP, we will always give SHEMU_ABORT_BRANCH_OUTSIDE otherwise.
    ctx.StackBase = ctx.Registers.RegRsp - 0x1000;
    ctx.ShellcodeBase = (ctx.Registers.RegRip ? ctx.Registers.RegRip & PAGE_MASK : 0x200000);

    shstatus = ShemuEmulate(&ctx);

    printf("Emulation terminated with status 0x%08x, flags: 0x%llx, %u NOPs\n",
           shstatus, (unsigned long long)ctx.Flags, ctx.NopCount);
    if (ctx.Flags & SHEMU_FLAG_NOP_SLED)
    {
        printf("        SHEMU_FLAG_NOP_SLED\n");
    }
    if (ctx.Flags & SHEMU_FLAG_LOAD_RIP)
    {
        printf("        SHEMU_FLAG_LOAD_RIP\n");
    }
    if (ctx.Flags & SHEMU_FLAG_WRITE_SELF)
    {
        printf("        SHEMU_FLAG_WRITE_SELF\n");
    }
    if (ctx.Flags & SHEMU_FLAG_TIB_ACCESS)
    {
        printf("        SHEMU_FLAG_TIB_ACCESS\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SYSCALL)
    {
        printf("        SHEMU_FLAG_SYSCALL\n");
    }
    if (ctx.Flags & SHEMU_FLAG_STACK_STR)
    {
        printf("        SHEMU_FLAG_STACK_STR\n");
    }
    if (ctx.Flags & SHEMU_FLAG_KPCR_ACCESS)
    {
        printf("        SHEMU_FLAG_KPCR_ACCESS\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SWAPGS)
    {
        printf("        SHEMU_FLAG_SWAPGS\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SYSCALL_MSR_READ)
    {
        printf("        SHEMU_FLAG_SYSCALL_MSR_READ\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SYSCALL_MSR_WRITE)
    {
        printf("        SHEMU_FLAG_SYSCALL_MSR_WRITE\n");
    }

    if (!opts.shdec_file.empty()) {
        auto f = std::ofstream(opts.shdec_file, std::ios::out | std::ios::binary);

        if (!f.is_open())
            std::cerr << "Failed to open file " << opts.shdec_file<< std::endl;
        else
            f.write(reinterpret_cast<char*>(ctx.Shellcode), ctx.ShellcodeSize);
    }
}

size_t disassemble(options &opts)
{
    struct timespec start;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);

    size_t icount = 0, miss_count = 0, ibytes = 0;
    size_t rel_rip = opts.offset;
    size_t total_disasm = 0;
    auto bytes = opts.bytes.get();
    auto disasm_size = std::min(opts.actual_size - opts.offset, opts.size);

    opts.address_size = int(std::ceil(((8 * sizeof(opts.actual_size)) - __builtin_clzll(opts.actual_size)) / 4.0));

    while ((total_disasm < disasm_size) && (icount < opts.count)) {
        INSTRUX instrux;

        auto status = NdDecodeEx2(&instrux,
                                  &bytes[rel_rip],
                                  opts.actual_size - rel_rip,
                                  opts.bits,
                                  opts.bits,
                                  opts.bits,
                                  opts.vendor);

        if (!ND_SUCCESS(status)) {
            if (!opts.dump_stats) {

                if (opts.json_output) {
                    auto j = byte_to_json(bytes[rel_rip], rel_rip + opts.rip);
                    std::cout << j.GetString() << std::endl;
                } else {
                    printf("%*zx ", opts.address_size, rel_rip + opts.rip);
                    printf("%02x", bytes[rel_rip]);
                    printf("%s", gSpaces[16 - opts.address_size]);
                    printf("db 0x%02x\n", bytes[rel_rip]);
                }
            }

            rel_rip++;
            total_disasm++;

            miss_count++;
        } else {
            icount++;
            ibytes += instrux.Length;

            if (!opts.dump_stats) {
                if (opts.json_output) {
                    auto j = instrux_to_json(&instrux, rel_rip + opts.rip);
                    std::cout << j.GetString() << std::endl;
                } else
                    print_instruction(rel_rip + opts.rip, &instrux, opts);
            } else if (opts.json_output)
                instrux_to_json(&instrux, rel_rip + opts.rip);

            rel_rip += instrux.Length;
            total_disasm += instrux.Length;
        }
    }

    struct timespec end;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);

    if (opts.dump_stats)
    {
        auto result = diff_time(end, start);

        long total_ns = result.tv_sec * NSEC_PER_SEC + result.tv_nsec;

        printf("Disassembled %zu instructions took %ld.%09ld seconds, %lu ns / instr.\n",
               icount, result.tv_sec, result.tv_nsec, total_ns / icount);
        printf("Invalid: %zu/%zu (%.2f) bytes\n", miss_count, ibytes,
               (static_cast<double>(miss_count) / static_cast<double>(disasm_size)) * 100.0);
    }

    opts.rip += rel_rip;

    return icount;
}


static bool _validate_and_fix_args(options& opts)
{
    if (!opts.interactive && !opts.comm) {
        int total = 0;

        if (!opts.hex_string.empty())
            total++;

        if (!opts.hex_file.empty())
            total++;

        if (!opts.in_file.empty())
            total++;

        if (total == 0) {
            std::cerr << "Give hex, file or hex-file..." << std::endl;
            return false;
        } else if (total > 1) {
            std::cerr << "Only one of hex, file or hex-file can be present..." << std::endl;
            return false;
        }
    }

    if (opts.interactive || opts.comm) {
        if (!opts.in_file.empty() || !opts.hex_string.empty() || !opts.hex_file.empty()) {
            std::cerr << "Interactive mode doesn't work with file, hex-string or hex-file" << std::endl;
            return false;
        }

        if (opts.size || opts.count || opts.offset) {
            std::cerr << "Interactive mode doesn't work with size, count or offset" << std::endl;
            return false;
        }
    }

    switch (opts.bits) {
    case 64:
        opts.bits = ND_DATA_64;
        break;

    case 32:
        opts.bits = ND_DATA_32;
        break;

    case 16:
        opts.bits = ND_DATA_16;
        break;

    default:
        std::cerr << "Please give --bits [64,32,16]" << std::endl;
        return false;
    }

    if (0 == opts.size)
        opts.size = std::numeric_limits<std::size_t>::max();

    if (0 == opts.count)
        opts.count = std::numeric_limits<std::size_t>::max();

    if (!isatty(fileno(stdout)))
        opts.output_redirected = true;

    if (!opts.hex_file.empty())
        opts.shdec_file = opts.hex_file + "_decoded.bin";
    else if (!opts.in_file.empty())
        opts.shdec_file = opts.in_file + "_decoded.bin";
    else
        opts.shdec_file = "hex_string_decoded.bin";

    return true;
}


static size_t _get_hex_opt(argparse::ArgumentParser &parser, const std::string &field)
{
    return std::strtoul(parser.get<std::string>(field).c_str(), nullptr, 0);
}


int main(int argc, const char **argv)
{
    auto opts = options{};

    auto parser = argparse::ArgumentParser(argv[0], "Disassembler tool for Linux");

    parser.add_argument("-i", "--interactive", "Interactive mode", false);
    parser.add_argument("-c", "--comm", "Comm mode", false);
    parser.add_argument("-r", "--rip", "Use this rip to disassemble", false);
    parser.add_argument("-f", "--file", "Use this input file", false);
    parser.add_argument("-x", "--hex", "Use this hex-string", false);
    parser.add_argument("--hexfile", "Use this input file as a hex-string", false);
    parser.add_argument("--no-color", "Don't use colors", false);
    parser.add_argument("--offset", "Use this offset in file/hex-string", false);
    parser.add_argument("--size", "Only disasemble this size from file/hex-string", false);
    parser.add_argument("--count", "Only disasemble this many instructions", false);
    parser.add_argument("--stats", "Dump statistics (time, count, etc.)", false);
    parser.add_argument("-b", "--bits", "Use the arch [16, 32, 64]", false);
    parser.add_argument("--verbose", "Verbose mode", false);
    parser.add_argument("--json", "Output to json", false);
    parser.add_argument("--extended", "Extended instruction info", false);
    parser.add_argument("--shemu", "Emulate the input hexfile", false);
    parser.add_argument("--kernel", "Kernel mode for shemu context", false);
    parser.add_argument("--shctx", "Shemu context file. Must be a JSON file.", false);

    parser.enable_help();

    auto err = parser.parse(argc, argv);
    if (err) {
        std::cout << err << std::endl;
        return -1;
    }

    if (parser.exists("help")) {
        parser.print_help();
        return 0;
    }

    opts.bits = parser.get<uint64_t>("bits");
    opts.interactive = parser.exists("interactive");
    opts.comm = parser.exists("comm");
    opts.offset = _get_hex_opt(parser, "offset");
    opts.size = _get_hex_opt(parser, "size");
    opts.count = _get_hex_opt(parser, "count");
    opts.rip = _get_hex_opt(parser, "rip");
    opts.in_file = parser.get<std::string>("file");
    opts.hex_string = parser.get<std::string>("hex");
    opts.hex_file = parser.get<std::string>("hexfile");
    opts.no_color = parser.exists("no-color");
    opts.dump_stats = parser.exists("stats");
    opts.verbose = parser.exists("verbose");
    opts.json_output = parser.exists("json");
    opts.extended = parser.exists("extended");
    opts.shemu = parser.exists("shemu");
    opts.kernel = parser.exists("kernel");
    opts.shctx_file = parser.get<std::string>("shctx");

    if (opts.verbose) {
        std::cout << "interactive: " << opts.interactive << std::endl;
        std::cout << "comm: " << opts.comm << std::endl;
        std::cout << "rip: " << opts.rip << std::endl;
        std::cout << "bits: " << static_cast<uint16_t>(opts.bits) << std::endl;
        std::cout << "offset: " << opts.offset << std::endl;
        std::cout << "size: " << opts.offset << std::endl;
        std::cout << "count: " << opts.count << std::endl;
        std::cout << "in_file: " << opts.in_file << std::endl;
        std::cout << "no-color: " << opts.no_color << std::endl;
        std::cout << "stats: " << opts.dump_stats << std::endl;
        std::cout << "hex: " << opts.hex_string << std::endl;
        std::cout << "json: " << opts.json_output << std::endl;
        std::cout << "extended: " << opts.extended << std::endl;
        std::cout << "shemu:" << opts.shemu << std::endl;
        std::cout << "kernel:" << opts.kernel << std::endl;
        std::cout << "shctx:" << opts.shctx_file << std::endl;
    }

    if (!_validate_and_fix_args(opts)) {
        return 1;
    }

    if (!opts.interactive && !opts.comm) {
        if (!opts.hex_string.empty() || !opts.hex_file.empty()) {
            if (!_hexstring_to_bytes(opts))
                return 1;
        } else if (!opts.in_file.empty()) {
            if (!_file_to_bytes(opts))
                return 1;
        }

        if (opts.offset >= opts.actual_size)
            return 1;

        if (!opts.shemu) {
            disassemble(opts);
        }
        else {
            _load_shctx(opts);
            shemu(opts);
        }

    } else {
        while (true) {
            opts.hex_string.clear();

            if (!opts.comm)
                std::cout << ">> ";

            std::getline(std::cin, opts.hex_string);

            if (opts.hex_string == "q"
                || opts.hex_string == "quit"
                || opts.hex_string == "exit"
                || opts.hex_string.empty()) {

                if (!opts.comm)
                    std::cout << "Bye!" << std::endl;

                break;
            }

            _hexstring_to_bytes(opts);

            disassemble(opts);

            std::cout.flush();
        }
    }

    return 0;
}
