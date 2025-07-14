/*
 * Copyright (c) 2025 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <Intrin.h>
#endif // WIN32

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Main disasm header file.
#include "bdshemu.h"
#include "bddisasm.h"

#include "disasmtool.h"


void 
ShemuLog(
    _In_ PCHAR Data, 
    _In_ void *Context
)
{
    UNREFERENCED_PARAMETER(Context);
    printf("%s", Data);
}


ND_BOOL 
ShemuAccessMem(
    _In_ PSHEMU_CONTEXT Ctx, 
    _In_ ND_UINT64 Gla, 
    _In_ ND_SIZET Size, 
    _Inout_ ND_UINT8 *Buffer, 
    _In_ ND_BOOL Store
)
{
    UNREFERENCED_PARAMETER(Ctx);
    UNREFERENCED_PARAMETER(Gla);

    if (!Store)
    {
        // On loads, always return 0.
        memset(Buffer, 0, Size);
    }
    else
    {
        // On stores, do nothing.
    }

    return ND_TRUE;
}

ND_BOOL
ShemuAccessShellcode(
    _In_ PSHEMU_CONTEXT Ctx, 
    _In_ ND_UINT64 Gla, 
    _In_ ND_SIZET Size, 
    _Inout_ ND_UINT8 *Buffer, 
    _In_ ND_BOOL Store
)
{
    ND_UINT32 offset;

    offset = (ND_UINT32)(Gla - Ctx->ShellcodeBase);

    if (Store)
    {
        memcpy(Ctx->Shellcode + offset, Buffer, Size);
    }
    else
    {
        memcpy(Buffer, Ctx->Shellcode + offset, Size);
    }

    return ND_TRUE;
}


void
handle_shemu(
    PDISASM_OPTIONS Options
    )
{
    SHEMU_CONTEXT ctx;
    SHEMU_STATUS shstatus;
    char *fileName;
    size_t rip = 0, fsize = Options->Size, offset = 0, decFileNameLength = 0, shellSize;
    char *fNameDecoded;
    PBYTE buffer = Options->Buffer;

    memset(&ctx, 0, sizeof(ctx));
    rip = Options->Rip;
    fileName = Options->FileName;

    if (!Options->SkipWriteDecoded)
    {
        if (fileName == NULL)
        {
            fNameDecoded = "hex_string_decoded.bin";
        }
        else
        {
            decFileNameLength = strlen(fileName) + sizeof("_decoded.bin");
            fNameDecoded = malloc(sizeof(char) * decFileNameLength);
        
            // This is safe, we allocated enough space.
            sprintf(fNameDecoded, "%s_decoded.bin", fileName);
        }
    }
    else
    {
        fNameDecoded = NULL;
    }

    offset = Options->Offset;

    // A little extra space, since shellcodes tend to do accesses after their code...
    shellSize = fsize + 0x100;

    // Allocate the shellcode, stack, shell bitmap and stack bitmaps.
    ctx.Shellcode = malloc(shellSize);

#define STACK_SIZE  0x2000ull
    ctx.Stack = malloc(STACK_SIZE);
    ctx.Intbuf = malloc(shellSize + STACK_SIZE);

    memset(ctx.Shellcode, 0, shellSize);
    memset(ctx.Stack, 0, STACK_SIZE);
    memcpy(ctx.Shellcode, buffer, fsize);
    memset(ctx.Intbuf, 0, shellSize + STACK_SIZE);

    // Use the provided RIP, if any. Otherwise, use a hard-coded value.
    ctx.ShellcodeBase = (rip != 0 ? rip & PAGE_MASK : 0x200000);
    ctx.ShellcodeSize = shellSize;
    ctx.StackBase = (ctx.ShellcodeBase & PAGE_MASK) - STACK_SIZE - 0x1000;
    ctx.StackSize = STACK_SIZE;
    
    ctx.IntbufSize = shellSize + STACK_SIZE;

    ctx.ArchType = SHEMU_ARCH_TYPE_X86;

    ctx.Arch.X86.Mode = Options->Mode;
    ctx.Arch.X86.Ring = Options->Ring;

    ctx.Arch.X86.Registers.RegFlags = NDR_RFLAG_IF | 2;
    ctx.Arch.X86.Registers.RegRip = ctx.ShellcodeBase + offset;
    ctx.Arch.X86.Registers.RegRsp = ctx.StackBase + STACK_SIZE / 2;

    if (ctx.Arch.X86.Mode == ND_CODE_64)
    {
        ctx.Arch.X86.Segments.Cs.Selector = (ctx.Arch.X86.Ring == 3) ? 0x33 : 0x10;
        ctx.Arch.X86.Segments.Ds.Selector = (ctx.Arch.X86.Ring == 3) ? 0x2b : 0x18;
        ctx.Arch.X86.Segments.Es.Selector = (ctx.Arch.X86.Ring == 3) ? 0x2b : 0x18;
        ctx.Arch.X86.Segments.Ss.Selector = (ctx.Arch.X86.Ring == 3) ? 0x2b : 0x18;
        ctx.Arch.X86.Segments.Fs.Selector = (ctx.Arch.X86.Ring == 3) ? 0x2b : 0x00;
        ctx.Arch.X86.Segments.Gs.Selector = (ctx.Arch.X86.Ring == 3) ? 0x53 : 0x00;

        ctx.Arch.X86.Segments.Fs.Base = 0;
        ctx.Arch.X86.Segments.Gs.Base = 0x7FFF0000;
    }
    else
    {
        ctx.Arch.X86.Segments.Cs.Selector = (ctx.Arch.X86.Ring == 3) ? 0x1b : 0x08;
        ctx.Arch.X86.Segments.Ds.Selector = (ctx.Arch.X86.Ring == 3) ? 0x23 : 0x10;
        ctx.Arch.X86.Segments.Es.Selector = (ctx.Arch.X86.Ring == 3) ? 0x23 : 0x10;
        ctx.Arch.X86.Segments.Ss.Selector = (ctx.Arch.X86.Ring == 3) ? 0x23 : 0x10;
        ctx.Arch.X86.Segments.Fs.Selector = (ctx.Arch.X86.Ring == 3) ? 0x3b : 0x30;
        ctx.Arch.X86.Segments.Gs.Selector = (ctx.Arch.X86.Ring == 3) ? 0x23 : 0x00;

        ctx.Arch.X86.Segments.Fs.Base = 0x7FFF0000;
        ctx.Arch.X86.Segments.Gs.Base = 0;
    }

    // Dummy values, to resemble regular CR0/CR4 values.
    ctx.Arch.X86.Registers.RegCr0 = 0x0000000080050031;
    ctx.Arch.X86.Registers.RegCr4 = 0x0000000000170678;

    if (Options->UseShemuRegs)
    {
        // Copy the new GPRs
        memcpy(&ctx.Arch.X86.Registers.RegRax, Options->ShemuRegs, sizeof(Options->ShemuRegs));

        // Update the stack to point to the new RSP, if one exists
        if (ctx.Arch.X86.Registers.RegRsp != 0)
        {
            // Consider the stack base at least with a page before the current RSP. In case of pushes or operations
            // which decrease the RSP, we will always give SHEMU_ABORT_RIP_OUTSIDE otherwise.
            ctx.StackBase = ctx.Arch.X86.Registers.RegRsp - 0x1000;
        }
    }

    ctx.TibBase = 0x7FFF0000;
    ctx.MaxInstructionsCount = 10000;
    ctx.Flags = 0;
    ctx.Options = SHEMU_OPT_TRACE_EMULATION | SHEMU_OPT_TRACE_MEMORY | SHEMU_OPT_TRACE_STRINGS | SHEMU_OPT_TRACE_LOOPS;
    ctx.Log = &ShemuLog;
    ctx.AccessMemory = (ShemuMemAccess)&ShemuAccessMem;
    ctx.AccessShellcode = (ShemuMemAccess)&ShemuAccessShellcode;

    // Configurable thresholds.
    ctx.NopThreshold = SHEMU_DEFAULT_NOP_THRESHOLD;
    ctx.StrThreshold = SHEMU_DEFAULT_STR_THRESHOLD;
    ctx.MemThreshold = SHEMU_DEFAULT_MEM_THRESHOLD;

    // Check for AES support.
    int regs[4] = { 0 };

#if defined(ND_ARCH_X86) || defined(ND_ARCH_X64)
    cpuid(regs, 1);
#endif

    // CPUID leaf function 1, register ECX, bit 25 indicates AES-NI support.
    if (!!(regs[2] & (1UL << 25)))
    {
        ctx.Options |= SHEMU_OPT_SUPPORT_AES;
    }

    if (Options->BypassSelfWrites)
    {
        ctx.Options |= SHEMU_OPT_BYPASS_SELF_WRITES;
    }

    ctx.Options |= SHEMU_OPT_SUPPORT_APX;

    shstatus = ShemuX86Emulate(&ctx);

    // Print each set flag after emulation.
    printf("Emulation terminated with status 0x%08x, flags: 0x%llx, %llu NOPs, %llu NULLs, %llu total instructions, %llu unique instructions\n", 
           shstatus, (unsigned long long)ctx.Flags, ctx.NopCount, ctx.NullCount, ctx.InstructionsCount, ctx.UniqueCount);
    
    struct 
    {
        ND_UINT64   Flag;
        const char* Name;
    } const shemuFlags[] = {
#define FLAGENTRY(f)       { .Flag = f, .Name = #f }
        FLAGENTRY(SHEMU_FLAG_NOP_SLED),
        FLAGENTRY(SHEMU_FLAG_LOAD_RIP),
        FLAGENTRY(SHEMU_FLAG_WRITE_SELF),
        FLAGENTRY(SHEMU_FLAG_TIB_ACCESS_PEB),
        FLAGENTRY(SHEMU_FLAG_SYSCALL),
        FLAGENTRY(SHEMU_FLAG_STACK_STR),
        FLAGENTRY(SHEMU_FLAG_TIB_ACCESS_WOW32),
        FLAGENTRY(SHEMU_FLAG_HEAVENS_GATE),
        FLAGENTRY(SHEMU_FLAG_STACK_PIVOT),
        FLAGENTRY(SHEMU_FLAG_SUD_ACCESS),
        FLAGENTRY(SHEMU_FLAG_KPCR_ACCESS),
        FLAGENTRY(SHEMU_FLAG_SWAPGS),
        FLAGENTRY(SHEMU_FLAG_SYSCALL_MSR_READ),
        FLAGENTRY(SHEMU_FLAG_SYSCALL_MSR_WRITE),
        FLAGENTRY(SHEMU_FLAG_SIDT),
#undef  FLAGENTRY
    };

    for (uint32_t fli = 0; fli < ARRAYSIZE(shemuFlags); fli++)
    {
        if (ctx.Flags & shemuFlags[fli].Flag)
        {
            printf("        %s\n", shemuFlags[fli].Name);
        }
    }

    if (fNameDecoded != NULL)
    {
        size_t outSize;
        FILE *file = fopen(fNameDecoded, "wb");
        if (file == NULL)
        {
            printf("Could not create the file %s\n", fNameDecoded);
            goto cleanup_and_exit;
        }

        outSize = fwrite(ctx.Shellcode, 1, fsize, file);

        if (outSize == 0)
        {
            printf("No bytes written to %s!\n", fNameDecoded);
        }

        fclose(file);
    }

cleanup_and_exit:
    if (NULL != fileName && NULL != fNameDecoded)
    {
        free(fNameDecoded);
    }

    if (NULL != ctx.Shellcode)
    {
        free(ctx.Shellcode);
    }

    if (NULL != ctx.Stack)
    {
        free(ctx.Stack);
    }

    if (NULL != ctx.Intbuf)
    {
        free(ctx.Intbuf);
    }
}
