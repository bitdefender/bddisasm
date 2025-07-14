/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef DISASMTOOL_H
#define DISASMTOOL_H

#include <stdbool.h>
#include <stdint.h>

#ifndef WIN32
#define _In_
#define _Inout_
#define _Out_
#define _Success_(x)

typedef char CHAR, *PCHAR;
typedef unsigned char BYTE, *PBYTE;

typedef int32_t INT32;
typedef uint32_t DWORD;

#define UNREFERENCED_PARAMETER(P)   (void)(P)
#define ARRAYSIZE(A)                (sizeof(A)/sizeof((A)[0]))
#endif // !WIN32

typedef enum _DISASM_COMMAND
{
    commandNone = 0,
    commandDecode,
    commandShemu,
} DISASM_COMMAND;


typedef enum _INPUT_MODE
{
    inputNone = 0,
    inputFile,
    inputHex
} INPUT_MODE;


typedef struct _DISASM_OPTIONS
{
    DISASM_COMMAND  Command;            // Command.
    uint8_t         *Buffer;            // The buffer containing the instructions.
    size_t          Size;               // Buffer size.
    size_t          Offset;             // Offset inside the buffer.
    size_t          Rip;                // Virtual RIP.
    bool            Highlight;          // Highlight instruction components, if true.
    bool            ExtendedInfo;       // Display extended instruction info, if true.
    bool            BitFields;          // Display the various bitfields inside the instruction, if true.
    bool            Skip16;             // Automatically jump over 16 bytes after each instruction.
    bool            Skip1;              // Automatically jump over one single byte after each instruction.
    bool            Stats;              // Display disassembly stats (clocks / instruction, instructions / second), if true.
    bool            Print;              // Print instruction disassembly, if true.
    uint8_t         Mode;               // Mode - 16, 32 or 64-bit mode.
    uint8_t         Ring;               // Ring - 0, 1, 2 or 3.
    uint8_t         Vendor;             // Preffered vendor.
    uint32_t        Feature;            // Used features.
    char            *FileName;          // Input file, if any.
    size_t          ShemuRegs[ND_MAX_GPR_REGS];
    bool            UseShemuRegs;       // If truue, the registers in ShemuRegs will be used for shemu input.
    bool            BypassSelfWrites;   // If true, shemu emulation will ignore self-modifications made by the shellcode.
    bool            SkipWriteDecoded;   // If true, shemu emulation will no longer write the decoded file.

    // Internal.
    INPUT_MODE      InputMode;
} DISASM_OPTIONS, *PDISASM_OPTIONS;


#define PAGE_SIZE   0x1000
#define PAGE_MASK   0xFFFFFFFFFFFFF000

#define FG_Black        "\033[1;30m"
#define FG_Red          "\033[1;31m"
#define FG_Green        "\033[1;32m"
#define FG_Yellow       "\033[1;33m"
#define FG_Blue         "\033[1;34m"
#define FG_Magenta      "\033[1;35m"
#define FG_Cyan         "\033[1;36m"
#define FG_White        "\033[1;37m"

void set_bold_fg_color(const char *Color);
void reset_fg_color(void);

#if !defined(WIN32) && (defined(ND_ARCH_X64) || defined(ND_ARCH_X86))
#include <cpuid.h>
#endif // defined(ND_ARCH_X64) || defined(ND_ARCH_X86)

#if defined(ND_ARCH_X64) || defined(ND_ARCH_X86)
#ifdef WIN32
#define cpuid       __cpuid
#else
#define __rdtsc     __builtin_ia32_rdtsc

static void cpuid(int cpuInfo[4], int function_id)
{
    unsigned int *cpuinfo = (unsigned int *)cpuInfo;
    __get_cpuid(function_id, &cpuinfo[0], &cpuinfo[1], &cpuinfo[2], &cpuinfo[3]);
}
#endif // WIN32
#endif // defined(ND_ARCH_X64) || defined(ND_ARCH_X86)

#endif // DISASMTOOL_H
