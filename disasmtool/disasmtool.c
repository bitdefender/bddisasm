/*
 * Copyright (c) 2020 Bitdefender
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
#include <time.h>

// Main disasm header file.
#include "bdshemu.h"
#include "bddisasm.h"

#include "disasmtool.h"

#ifndef WIN32
#define _stricmp    strcasecmp
#endif


void
handle_shemu(
    PDISASM_OPTIONS Options
);

void
handle_disasm(
    PDISASM_OPTIONS Options
);


#if !defined(BDDISASM_HAS_VSNPRINTF)
//
// nd_vsnprintf
//
int
nd_vsnprintf_s(
    char *buffer,
    size_t sizeOfBuffer,
    size_t count,
    const char *format,
    va_list argptr
)
{
    return _vsnprintf_s(buffer, sizeOfBuffer, count, format, argptr);
}
#endif // !defined(BDDISASM_HAS_VSNPRINTF)

#if !defined(BDDISASM_HAS_MEMSET)
void* 
nd_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}
#endif // !defined(BDDISASM_HAS_MEMSET)


void set_bold_fg_color(const char *Color)
{
    printf(Color);
}


void reset_fg_color(void)
{
    printf("\033[0m");
}


ND_BOOL
is_hex_digit(
    _In_ char Digit
)
{
    return (Digit >= '0' && Digit <= '9') ||
           (Digit >= 'A' && Digit <= 'F') ||
           (Digit >= 'a' && Digit <= 'f');
}


BYTE
hex_to_bin(
    _In_ char HexByte
)
{
    // Transforms one hex-digit to a number.
    if ((HexByte >= '0') && (HexByte <= '9'))
    {
        return HexByte - '0';
    }
    else if ((HexByte >= 'A') && (HexByte <= 'F'))
    {
        return HexByte - 'A' + 10;
    }
    else if ((HexByte >= 'a') && (HexByte <= 'f'))
    {
        return HexByte - 'a' + 10;
    }

    return 0;
}


INT32
regstr_to_idx(
    _In_ const char *Reg
)
{
    static const char *reg64[] =
    {
        "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
        "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15"
    };

    for (INT32 i = 0; i < (INT32)ARRAYSIZE(reg64); i++)
    {
        if (!_stricmp(Reg, reg64[i]))
        {
            return i;
        }
    }

    return -1;
}


_Success_(return)
bool
match_gpr(
    _In_ const char *Arg,
    _Out_ DWORD *Index
)
{
    // Check if the provided argument is a register.
    if (Arg[0] == '-')
    {
        INT32 idx = regstr_to_idx(Arg + 1); // this will be the name of the register or the NULL terminator
        if (idx >= 0)
        {
            *Index = idx;
            return true;
        }
    }

    return false;
}


void print_help(void)
{
    uint32_t major, minor, revision;
    char *date, *time;

    NdGetVersion(&major, &minor, &revision, &date, &time);

    printf("bddisasm version %u.%u.%u, built on %s %s\n", major, minor, revision, date, time);
    printf("\n");
    printf("IMPORTANT:\n");
    printf("    This tool is only meant to exemplify bddisasm integration.\n");
    printf("\n");
    printf("USAGE:\n");
    printf("    disasm COMMAND INPUT MODE [OPTIONS]\n");
    printf("\n");

    printf("COMMAND can be one of:\n");
    printf("    decode      - Will decode the input, and print each instruction (default).\n");
    printf("    shemu       - Will run the shellcode-emulator on the input, and print the emulation trace.\n");
    printf("\n");

    printf("INPUT is mandatory and can be one of:\n");
    printf("    -f file     - Specify input `file` name.\n");
    printf("    -h hex      - Specify input `hex` string. Accepted formats are:\n");
    printf("        Plain   hex string; example: 33C090CCC3\n");
    printf("        Escaped hex string; example: \\x33\\xC0\\x90\\xCC\\xC3\n");

    printf("\n");

    printf("MODE sets the decode mode:\n");
    printf("    -b16        - Decode in 16-bit mode.\n");
    printf("    -b32        - Decode in 32-bit mode.\n");
    printf("    -b64        - Decode in 64-bit mode (default).\n");
    printf("\n");

    printf("OPTIONS which are common among different modes:\n");
    printf("    -o offset   - Start processing from the indicated `offset` (default is 0).\n");
    printf("    -r rip      - Use the indicated `rip` for disassembly (default is 0).\n");
    printf("    -vvendor    - Set prefered vendor (default is any). The following are valid `vendor` values:\n");
    printf("       intel, amd, any\n");
    printf("    -tfeature   - Set prefered feature mode (default is all). The following are valid `feature` values (multiple can be used):\n");
    printf("       none, all, mpx, cet, cldm, piti, movrs, bhi\n");
    printf("\n");

    printf("OPTIONS valid only with decode command:\n");
    printf("    -hl         - Highlight instruction parts. The colors used are:\n");

    set_bold_fg_color(FG_White);
    printf("        light white      prefixes\n");
    set_bold_fg_color(FG_Green);
    printf("        light green      opcodes\n");
    set_bold_fg_color(FG_Yellow);
    printf("        light yellow     modrm and sib\n");
    set_bold_fg_color(FG_Blue);
    printf("        light blue       displacement\n");
    set_bold_fg_color(FG_Red);
    printf("        light red        relative offset, immediate, address\n");
    reset_fg_color();

    printf("    -nv         - Don't print disassembly. Use this only for performance tests.\n");
    printf("    -iv         - Print performance statistics.\n");
    printf("    -exi        - Print extended info about instructions.\n");
    printf("    -bits       - Print the instruction bit fields.\n");
    printf("    -skip16     - Skip 16 bytes after each decoded instruction. Useful when decoding invalid instructions.\n");
    printf("    -skip1      - Skip a single byte after each decoded instruction. Useful when decoding every possible offset.\n");
    printf("\n");
    printf("OPTIONS valid only with shemu command:\n");
    printf("    -reg val    - Set register `reg` to value `val` for emulation. `reg` must be the plain 64-bit register name (ie: rax).\n");
    printf("    -k          - Specify kernel mode for shemu emulation (default is user-mode).\n");
    printf("    -bw         - Bypass self-modifications in shemu.\n");
    printf("    -skipdecoded - The decoded file will not be written to disk.\n");
    printf("\n");
    printf("\n");
    printf("EXAMPLES:\n");
    printf("    Decode 64-bit code from file test.bin:\n");
    printf("        disasm -f test.bin\n");
    printf("        disasm -b64 -f test.bin\n");
    printf("        disasm decode -b64 -f test.bin\n");
    printf("    Decode 64-bit from a hex-buffer, and display extended instruction information:\n");
    printf("        disasm -b64 -h 909033C0 -exi\n");
    printf("    Decode from hex-string, highlight instruction components & display instruction bitfields:\n");
    printf("        disasm -h 90505833C0E80000000058CC -hl -bits\n");
    printf("    Emulate a potential 32-bit shellcode from test file shell.bin:\n");
    printf("        disasm shemu -b32 -f shell.bin\n");
    printf("    Emulate a potential 32-bit shellcode from test file shell.bin, and specify some input registers:\n");
    printf("        disasm shemu -b32 -f shell.bin -rax 0x100 -rcx 0xABCD -rsp 0x1000\n");
    printf("    Run a quick benchmark on file test.bin:\n");
    printf("        disasm -f test.bin -nv -iv\n");
    printf("\n");
}


void
cleanup_context(
    _Inout_ DISASM_OPTIONS *Options
)
{
    if (Options->InputMode == inputFile)
    {
        if (NULL != Options->Buffer)
        {
            free(Options->Buffer);
        }
    }
}


void *read_file(const char *Path, size_t *Size)
{
    void *buffer;
    FILE *fd = fopen(Path, "rb");
    if (fd == NULL)
    {
        printf("fopen failed for \"%s\"\n", Path);
        return NULL;
    }

    fseek(fd, 0ull, SEEK_END);
    *Size = ftell(fd);
    rewind(fd);

    buffer = malloc(*Size);
    if (buffer != NULL)
    {
        size_t readCount = fread(buffer, 1, *Size, fd);
        if (readCount != *Size)
        {
            printf("Only %zx bytes were read! Expected %zx\n", readCount, *Size);
            *Size = readCount;
        }
    }

    fclose(fd);
    return buffer;
}


_Success_(return)
bool
parse_input(
    _Inout_ DISASM_OPTIONS* Options
)
{
    static BYTE hexbuf[4096];

    if (inputNone == Options->InputMode)
    {
        printf("Expecting an input mode: either -f or -h!\n");
        return false;
    }

    if (inputFile == Options->InputMode)
    {
        Options->Buffer = read_file(Options->FileName, &Options->Size);
        if (Options->Buffer == NULL)
        {
            printf("Couldn't read '%s'\n", Options->FileName);
            cleanup_context(Options);
            return false;
        }

        if (Options->Size == 0)
        {
            printf("The input file '%s' is empty\n", Options->FileName);
            cleanup_context(Options);
            return false;
        }
    }
    else
    {
        DWORD idx = 0;

        Options->Size = strlen(Options->FileName);

        if (Options->Size < 2)
        {
            printf("Min 1-byte buffer needed!\n");
            return false;
        }

        // Extract each byte from the provided hex input.
        for (size_t bidx = 0; bidx < Options->Size - 1; )
        {
            // '\x'
            if (Options->FileName[bidx] == '\\' && Options->FileName[bidx + 1] == 'x')
            {
                bidx += 2;
                continue;
            }

            // '0x'
            if (Options->FileName[bidx] == '0' && Options->FileName[bidx + 1] == 'x')
            {
                bidx += 2;
                continue;
            }

            // '0x'
            if (Options->FileName[bidx] == '0' && Options->FileName[bidx + 1] == 'X')
            {
                bidx += 2;
                continue;
            }

            // ' '
            // '0x'
            if (Options->FileName[bidx] == ' ')
            {
                bidx += 1;
                continue;
            }

            if (is_hex_digit(Options->FileName[bidx]) &&
                is_hex_digit(Options->FileName[bidx + 1]))
            {
                hexbuf[idx++] = ((hex_to_bin(Options->FileName[bidx]) << 4) |
                                 (hex_to_bin(Options->FileName[bidx + 1]))) & 0xFF;
                bidx += 2;
                continue;
            }

            printf("Unexpected hex-character encountered: %c!\n", Options->FileName[bidx]);
            return false;
        }

        Options->FileName = NULL;
        Options->Size = idx;
        Options->Buffer = hexbuf;
    }

    // Make sure the offset is valid & points within the buffer.
    if (Options->Offset >= Options->Size)
    {
        printf("The offset exceeds the buffer size!\n");
        cleanup_context(Options);
        return false;
    }

    return true;
}


_Success_(return)
bool
parse_arguments(
    _In_ int argc,
    _In_ char* argv[],
    _Out_ DISASM_OPTIONS *Options
)
{
    int i;

    if (argc < 2 || NULL == argv)
    {
        print_help();
        return false;
    }

    memset(Options, 0, sizeof(*Options));

    // Initialize default options.
    Options->Command = commandDecode;
    Options->Mode = ND_CODE_64;
    Options->Ring = 3;
    Options->Print = true;
    Options->Vendor = ND_VEND_ANY;
    Options->Feature = ND_FEAT_ALL;
    Options->SkipWriteDecoded = false;

    i = 1;
    while (i < argc)
    {       
        DWORD gprIdx;

        if (match_gpr(argv[i], &gprIdx))
        {
            // Register value (used by shemu).
            if (i + 1 >= argc)
            {
                printf("No value given for %s!\n", argv[i]);
            }
            else
            {
                Options->ShemuRegs[gprIdx] = (size_t)strtoull(argv[i + 1], NULL, 0);
                Options->UseShemuRegs = true;
                i++;
            }
        }
        else if (strcmp(argv[i], "shemu") == 0)
        {
            // shemu command - will emulate.
            Options->Command = commandShemu;
        }
        else if (strcmp(argv[i], "decode") == 0)
        {
            // decode command.
            Options->Command = commandDecode;
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'f' && argv[i][2] == 0)
        {
            // File input mode.
            if (i + 1 < argc)
            {
                Options->InputMode = inputFile;
                Options->FileName = argv[i + 1];
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'h' && argv[i][2] == 0)
        {
            // Hex-string input mode.
            if (i + 1 < argc)
            {
                Options->InputMode = inputHex;
                Options->FileName = argv[i + 1];
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'o' && argv[i][2] == 0)
        {
            // Offset inside the provided buffer.
            if (i + 1 < argc)
            {
                Options->Offset = (size_t)strtoull(argv[i + 1], NULL, 0);
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'r' && argv[i][2] == 0)
        {
            // Rip. Can be any value, as it's used only for disassembly.
            if (i + 1 < argc)
            {
                Options->Rip = (size_t)strtoull(argv[i + 1], NULL, 0);
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'k' && argv[i][2] == 0)
        {
            // Kernel-mode for shemu.
            Options->Ring = 0;
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'b' && argv[i][2] == 'w' && argv[i][3] == 0)
        {
            // Bypass self writes in shemu.
            Options->BypassSelfWrites = true;
        }
        else if (0 == strcmp(argv[i], "-b16"))
        {
            // 16-bit mode.
            Options->Mode = ND_CODE_16;
        } 
        else if (0 == strcmp(argv[i], "-b32"))
        {
            // 32-bit mode.
            Options->Mode = ND_CODE_32;
        } 
        else if (0 == strcmp(argv[i], "-b64"))
        {
            // 64-bit mode.
            Options->Mode = ND_CODE_64;
        }
        else if (0 == strcmp(argv[i], "-vintel"))
        {
            // Prefer Intel instructions.
            Options->Vendor = ND_VEND_INTEL;
        }
        else if (0 == strcmp(argv[i], "-vamd"))
        {
            // Prefer AMD instructions.
            Options->Vendor = ND_VEND_AMD;
        }
        else if (0 == strcmp(argv[i], "-vany"))
        {
            // Try to decode everything.
            Options->Vendor = ND_VEND_ANY;
        }
        else if (0 == strcmp(argv[i], "-tall"))
        {
            // Enable all features.
            Options->Feature = ND_FEAT_ALL;
        }
        else if (0 == strcmp(argv[i], "-tmpx"))
        {
            // Enable MPX. 
            if (Options->Feature == ND_FEAT_ALL)
            {
                Options->Feature = 0;
            }

            Options->Feature |= ND_FEAT_MPX;
        }
        else if (0 == strcmp(argv[i], "-tcet"))
        {
            // Enable CET.
            if (Options->Feature == ND_FEAT_ALL)
            {
                Options->Feature = 0;
            }

            Options->Feature |= ND_FEAT_CET;
        }
        else if (0 == strcmp(argv[i], "-tcldm"))
        {
            // Enable Cache Line Demote.
            if (Options->Feature == ND_FEAT_ALL)
            {
                Options->Feature = 0;
            }

            Options->Feature |= ND_FEAT_CLDEMOTE;
        }
        else if (0 == strcmp(argv[i], "-tpiti"))
        {
            // Enable Prefetch for instruction fetch.
            if (Options->Feature == ND_FEAT_ALL)
            {
                Options->Feature = 0;
            }

            Options->Feature |= ND_FEAT_PITI;
        }
        else if (0 == strcmp(argv[i], "-tmovrs"))
        {
            // Enable Prefetch for instruction fetch.
            if (Options->Feature == ND_FEAT_ALL)
            {
                Options->Feature = 0;
            }

            Options->Feature |= ND_FEAT_MOVRS;
        }
        else if (0 == strcmp(argv[i], "-tbhi"))
        {
            // Enable Prefetch for instruction fetch.
            if (Options->Feature == ND_FEAT_ALL)
            {
                Options->Feature = 0;
            }

            Options->Feature |= ND_FEAT_BHI;
        }
        else if (0 == strcmp(argv[i], "-tnone"))
        {
            // No feature support.
            Options->Feature = ND_FEAT_NONE;
        }
        else if (0 == strcmp(argv[i], "-nv"))
        {
            // Do not print anything.
            Options->Print = false;
        } 
        else if (0 == strcmp(argv[i], "-hl"))
        {
            // Highlight instruction components.
            Options->Highlight = true;
        }
        else if (0 == strcmp(argv[i], "-iv"))
        {
            // Print statistics.
            Options->Stats = true;
        }
        else if (0 == strcmp(argv[i], "-exi"))
        {
            // Print extended instruction information.
            Options->ExtendedInfo = true;
        }
        else if (0 == strcmp(argv[i], "-bits"))
        {
            // Print instruction bitfields.
            Options->BitFields = true;
        }
        else if (0 == strcmp(argv[i], "-skip16"))
        {
            // Skip 16 bytes after each decoded instruction.
            Options->Skip16 = true;
        }
        else if (0 == strcmp(argv[i], "-skip1"))
        {
            // Skip a single byte after each decoded instruction.
            Options->Skip1 = true;
        }
        else if (0 == strcmp(argv[i], "-skipdecoded"))
        {
            Options->SkipWriteDecoded = true;
        }
        else
        {
            printf("Unknown option: '%s'\n", argv[i]);
        }

        i++;
    }

    // Parse the input.
    if (!parse_input(Options))
    {
        printf("Could not find a valid input!\n");
        return false;
    }

    return true;
}

int 
main(
    _In_ int argc, 
    _In_ char* argv[]
)
{
    DISASM_OPTIONS options = { 0 };

    // Parse arguments & extract relevant options.
    if (!parse_arguments(argc, argv, &options))
    {
        return -1;
    }

    // Handle the indicated command.
    if (options.Command == commandShemu)
    {
        handle_shemu(&options);
    }
    else
    {
        handle_disasm(&options);
    }

    // Will free any memory allocated during argument parsing, and will close any handles.
    cleanup_context(&options);

    return 0;
}
