/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include <iostream>
#include <fstream>
#include <memory>
#include <limits>

#include "external/argparse.h"

#include "disasm.hpp"

static const long NSEC_PER_SEC = (1000ULL * 1000ULL * 1000ULL);

const char *gSpaces[16] =
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

    int bits;

    uint8_t vendor;
    bool no_color;
    bool verbose;
    bool dump_stats;
    bool interactive;
    bool comm;
    bool json_output;

    std::string in_file;
    std::string hex_string;
    std::string hex_file;

    // From here on, these are set internally
    std::unique_ptr<uint8_t[]> bytes;
    size_t actual_size;

    bool output_redirected;
};


extern "C"
{
    int nd_vsnprintf_s(char *buffer, size_t sizeOfBuffer, [[maybe_unused]] size_t count, const char *format, va_list argptr)
    {
        return vsnprintf(buffer, sizeOfBuffer, format, argptr);
    }
    
    void *
    nd_memset(void *s, int c, size_t n)
    {
        return memset(s, c, n);
    }
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
    auto initial_size = opts.hex_string.length() / 2;

    opts.bytes = std::make_unique<uint8_t[]>(initial_size);
    auto bytes = opts.bytes.get();

    for (size_t i = 0; i < opts.hex_string.length(); i += 2) {
        auto pair = opts.hex_string.substr(i, 2);
        char *end_ptr;

        if (pair == "0x" || pair == "0X" || pair == "\\x")
            continue;

        auto b = std::strtoul(pair.c_str(), &end_ptr, 16);

        size_t conv_size = static_cast<size_t>(end_ptr - pair.c_str());

        if (conv_size != pair.length()) {
            std::cerr << "Trying to convert invalid hex number: " << pair << std::endl;
            return false;
        }

        bytes[opts.actual_size++] = b;
    }

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


static struct timespec diff_time(struct timespec &end, struct timespec &start)
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

    printf("%zx ", rip);

    if (!opts.no_color)
    {
        _set_text_color(White);
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
    }

    _set_text_color(Reset);

    for (; k < instrux->Length; k++)
    {
        printf("%02x", instrux->InstructionBytes[k]);
    }

    std::cout << gSpaces[16 - instrux->Length];

    NdToText(instrux, rip, sizeof(instruxText), instruxText);

    std::cout << instruxText << std::endl;

    // if (Options->ExtendedInfo)
    // {
    //     const BYTE opsize[3] = { 2, 4, 8 };
    //     const BYTE adsize[3] = { 2, 4, 8 };
    //     const BYTE veclen[3] = { 16, 32, 64 };

    //     printf("         DSIZE: %2d, ASIZE: %2d, VLEN: ",
    //         opsize[instrux->EfOpMode] * 8, adsize[instrux->AddrMode] * 8);

    //     if (ND_HAS_VECTOR(instrux))
    //     {
    //         printf("%2d\n", veclen[instrux->VecMode] * 8);
    //     }
    //     else
    //     {
    //         printf("-\n");
    //     }

    //     printf("         ISA Set: %s, Ins cat: %s, Ins class: %d, CET tracked: %s\n",
    //         set_to_string(instrux->IsaSet), category_to_string(instrux->Category), instrux->Instruction,
    //         instrux->IsCetTracked ? "yes" : "no");

    //     if (0 != instrux->CpuidFlag.Flag)
    //     {
    //         char *regs[4] = { "eax", "ecx", "edx", "ebx" };

    //         printf("         CPUID leaf: 0x%08x", instrux->CpuidFlag.Leaf);

    //         if (instrux->CpuidFlag.SubLeaf != ND_CFF_NO_SUBLEAF)
    //         {
    //             printf(", sub-leaf: 0x%08x", instrux->CpuidFlag.SubLeaf);
    //         }

    //         printf(", reg: %s, bit %d\n", regs[instrux->CpuidFlag.Reg], instrux->CpuidFlag.Bit);
    //     }

    //     {
    //         DWORD fidx, all;
    //         char *flags[22] = { "CF", NULL, "PF", NULL, "AF", NULL, "ZF", "SF", "TF", "IF", "DF", "OF", "IOPL", NULL, "NT", NULL, "RF", "VM", "AC", "VIF", "VIP", "ID" };

    //         all = instrux->FlagsAccess.Tested.Raw | instrux->FlagsAccess.Modified.Raw | instrux->FlagsAccess.Set.Raw | 
    //               instrux->FlagsAccess.Cleared.Raw | instrux->FlagsAccess.Undefined.Raw;
    //         printf("         FLAGS access: ");

    //         for (fidx = 0; fidx < 21; fidx++)
    //         {
    //             if (flags[fidx] != NULL)
    //             {
    //                 if (0 == (all & (1ULL << fidx)))
    //                 {
    //                     continue;
    //                 }

    //                 printf("%s: ", flags[fidx]);

    //                 if (instrux->FlagsAccess.Tested.Raw & (1ULL << fidx))
    //                 {
    //                     printf("t");
    //                 }

    //                 if (instrux->FlagsAccess.Modified.Raw & (1ULL << fidx))
    //                 {
    //                     printf("m");
    //                 }

    //                 if (instrux->FlagsAccess.Set.Raw & (1ULL << fidx))
    //                 {
    //                     printf("1");
    //                 }

    //                 if (instrux->FlagsAccess.Cleared.Raw & (1ULL << fidx))
    //                 {
    //                     printf("0");
    //                 }

    //                 if (instrux->FlagsAccess.Undefined.Raw & (1ULL << fidx))
    //                 {
    //                     printf("u");
    //                 }

    //                 printf("; ");
    //             }
    //         }

    //         printf("\n");
    //     }

    //     printf("         Valid modes: R0: %s, R1: %s, R2: %s, R3: %s, Real: %s, V8086: %s, Prot: %s, Compat: %s, Long: %s, SMM: %s, SGX: %s, TSX: %s, VMXRoot: %s, VMXNonRoot: %s\n",
    //         instrux->ValidModes.Ring0 ? "yes" : "no",
    //         instrux->ValidModes.Ring1 ? "yes" : "no",
    //         instrux->ValidModes.Ring2 ? "yes" : "no",
    //         instrux->ValidModes.Ring3 ? "yes" : "no",
    //         instrux->ValidModes.Real ? "yes" : "no",
    //         instrux->ValidModes.V8086 ? "yes" : "no",
    //         instrux->ValidModes.Protected ? "yes" : "no",
    //         instrux->ValidModes.Compatibility ? "yes" : "no",
    //         instrux->ValidModes.Long ? "yes" : "no",
    //         instrux->ValidModes.Smm ? "yes" : "no",
    //         instrux->ValidModes.Sgx ? "yes" : "no",
    //         instrux->ValidModes.Tsx ? "yes" : "no",
    //         instrux->ValidModes.VmxRoot ? "yes" : "no",
    //         instrux->ValidModes.VmxNonRoot ? "yes" : "no"
    //         );

    //     for (i = 0; i < instrux->OperandsCount; i++)
    //     {
    //         printf("         Operand %d  %s  Type: %10s, Size: %2d, RawSize: %2d, Encoding: %s", i,
    //             instrux->Operands[i].Access.Read && instrux->Operands[i].Access.Write ? "RW" :
    //             instrux->Operands[i].Access.Write ? "-W" : instrux->Operands[i].Access.Read ? "R-" : "--",
    //             optype_to_string(instrux->Operands[i].Type), instrux->Operands[i].Size,
    //             instrux->Operands[i].RawSize, encoding_to_string(instrux->Operands[i].Encoding)
    //         );

    //         if (ND_OP_MEM == instrux->Operands[i].Type)
    //         {
    //             printf(", ");

    //             if (instrux->Operands[i].Info.Memory.IsAG)
    //             {
    //                 printf("Address Generator, ");
    //             }

    //             if (instrux->Operands[i].Info.Memory.IsBitbase)
    //             {
    //                 printf("Bitbase Addressing, ");
    //             }

    //             if (instrux->Operands[i].Info.Memory.IsMib)
    //             {
    //                 printf("MIB Addressing, ");
    //             }

    //             if (instrux->Operands[i].Info.Memory.IsVsib)
    //             {
    //                 printf("VSIB Addressing, ");
    //             }

    //             if (instrux->Operands[i].Info.Memory.IsStack)
    //             {
    //                 printf("Stack, ");
    //             }

    //             if (instrux->Operands[i].Info.Memory.IsShadowStack)
    //             {
    //                 printf("Shadow Stack, ");
    //             }
    //         }

    //         if (ND_OP_REG == instrux->Operands[i].Type)
    //         {
    //             printf(", Type: %16s, Size: %2d, Reg: %d, Count: %d\n",
    //                 regtype_to_string(instrux->Operands[i].Info.Register.Type),
    //                 instrux->Operands[i].Info.Register.Size,
    //                 instrux->Operands[i].Info.Register.Reg,
    //                 instrux->Operands[i].Info.Register.Count);
    //         }
    //         else
    //         {
    //             printf("\n");
    //         }

    //         if (instrux->Operands[i].Decorator.HasBroadcast)
    //         {
    //             printf("                Decorator: Broadcast %d bytes element %d times\n",
    //                 instrux->Operands[i].Decorator.Broadcast.Size,
    //                 instrux->Operands[i].Decorator.Broadcast.Count);
    //         }

    //         if (instrux->Operands[i].Decorator.HasMask)
    //         {
    //             printf("                Decorator: Mask k%d\n", instrux->Operands[i].Decorator.Mask.Msk);
    //         }

    //         if (instrux->Operands[i].Decorator.HasZero)
    //         {
    //             printf("                Decorator: Zero (no merging)\n");
    //         }
    //     }

    //     printf("\n");
    // }
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


size_t disassemble(options &opts)
{
    struct timespec start;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);

    size_t icount = 0, miss_count = 0, ibytes = 0;
    size_t rel_rip = opts.offset;
    size_t total_disasm = 0;
    auto bytes = reinterpret_cast<uint8_t *>(opts.bytes.get());
    auto disasm_size = std::min(opts.actual_size - opts.offset, opts.size);

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
                    printf("%zx ", rel_rip + opts.rip);
                    printf("%02x", bytes[rel_rip]);
                    printf("%s", gSpaces[16 - 1]);
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

        printf("Disassembled %zu instructions took %ld.%09ld seconds, %ld ns / instr.\n",
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

    return true;
}


static size_t _get_hex_opt(ArgumentParser &parser, const std::string &field)
{
    return std::strtoul(parser.get<std::string>(field).c_str(), nullptr, 0);
}


int main(int argc, char **argv)
{
    auto opts = options{};

    auto parser = ArgumentParser(argv[0]);

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

    try {
        parser.parse(argc, argv);
    } catch (const ArgumentParser::ArgumentNotFound& ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    opts.bits = parser.get<int>("bits");
    opts.interactive = parser.get<bool>("interactive");
    opts.comm = parser.get<bool>("comm");
    opts.offset = _get_hex_opt(parser, "offset");
    opts.size = _get_hex_opt(parser, "size");
    opts.count = _get_hex_opt(parser, "count");
    opts.rip = _get_hex_opt(parser, "rip");
    opts.in_file = parser.get<std::string>("file");
    opts.hex_string = parser.get<std::string>("hex");
    opts.hex_file = parser.get<std::string>("hexfile");
    opts.no_color = parser.get<bool>("no-color");
    opts.dump_stats = parser.get<bool>("stats");
    opts.verbose = parser.get<bool>("verbose");
    opts.json_output = parser.get<bool>("json");

    if (opts.verbose) {
        std::cout << "interactive: " << opts.interactive << std::endl;
        std::cout << "comm: " << opts.comm << std::endl;
        std::cout << "rip: " << opts.rip << std::endl;
        std::cout << "bits: " << opts.bits << std::endl;
        std::cout << "offset: " << opts.offset << std::endl;
        std::cout << "size: " << opts.offset << std::endl;
        std::cout << "count: " << opts.count << std::endl;
        std::cout << "in_file: " << opts.in_file << std::endl;
        std::cout << "no-color: " << opts.no_color << std::endl;
        std::cout << "stats: " << opts.dump_stats << std::endl;
        std::cout << "hex: " << opts.hex_string << std::endl;
        std::cout << "json: " << opts.json_output << std::endl;
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

        disassemble(opts);
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
