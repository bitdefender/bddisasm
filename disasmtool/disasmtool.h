/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef DISASMTOOL_H
#define DISASMTOOL_H


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
    BOOLEAN         Highlight;          // Highlight instruction components, if true.
    BOOLEAN         ExtendedInfo;       // Display extended instruction info, if true.
    BOOLEAN         BitFields;          // Display the various bitfields inside the instruction, if true.
    BOOLEAN         Skip16;             // Automatically jump over 16 bytes after each instruction.
    BOOLEAN         Stats;              // Display disassembly stats (clocks / instruction, instructions / second), if true.
    BOOLEAN         Print;              // Print instruction disassembly, if true.
    uint8_t         Mode;               // Mode - 16, 32 or 64-bit mode.
    uint8_t         Ring;               // Ring - 0, 1, 2 or 3.
    uint8_t         Vendor;             // Preffered vendor.
    uint8_t         Feature;            // Used features.
    char            *FileName;          // Input file, if any.
    size_t          ShemuRegs[ND_MAX_GPR_REGS];
    BOOLEAN         UseShemuRegs;       // If truue, the registers in ShemuRegs will be used for shemu input.
    BOOLEAN         BypassSelfWrites;   // If true, shemu emulation will ignore self-modifications made by the shellcode.

    // Internal.
    INPUT_MODE      InputMode;
    HANDLE          HandleFile;
    HANDLE          HandleMapping;

} DISASM_OPTIONS, *PDISASM_OPTIONS;


#define PAGE_SIZE   0x1000
#define PAGE_MASK   0xFFFFFFFFFFFFF000


#endif // DISASMTOOL_H
