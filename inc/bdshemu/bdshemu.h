/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef BDSHEMU_H
#define BDSHEMU_H


#include "../bddisasm.h"


//
// Print function. Used to log emulation traces.
//
typedef void
(*ShemuPrint)(
    char *Data          // Data to be printed.
    );


//
// Access memory function. Simply return true if the access was handled, or false if it wasn't.
// If the function is not provided in SHEMU_CONTEXT, or if it returns false, the emulation will be terminated.
// Note that the integrator is free to handle external memory access as it pleases. 
// Loads could always yield the same value (0), a random value or they could return actual memory values.
// Stores could be discarded, or they could be buffered in a store-buffer like structure.
// Note that by using the ShemuContext, the integrator knows whether the access is user or supervisor (the Ring field
// inside ShemuContext), and he knows whether it is 16/32/64 bit mode (Mode field inside ShemuContext).
// 
typedef bool
(*ShemuMemAccess)(
    void *ShemuContext, // Shemu emulation context.
    uint64_t Gla,       // Linear address to be accessed.
    size_t Size,        // Number of bytes to access.
    uint8_t *Buffer,    // Contains the read content (if Store is false), or the value to be stored at Gla.
    bool Store          // If false, read content at Gla. Otherwise, write content at Gla.
    );


// Percent of emulated instructions that must be NOP to consider a NOP sled is present.
#define SHEMU_DEFAULT_NOP_THRESHOLD     75
// Consecutive printable characters on stack to consider a stack string access.
#define SHEMU_DEFAULT_STR_THRESHOLD     8
// Will not emulate more than this number of external memory accesses. Once this threshold is exceeded, any external
// access will abort the emulation.
#define SHEMU_DEFAULT_MEM_THRESHOLD     0


//
// General purpose registers.
//
typedef struct _SHEMU_GPR_REGS
{
    uint64_t RegRax;
    uint64_t RegRcx;
    uint64_t RegRdx;
    uint64_t RegRbx;
    uint64_t RegRsp;
    uint64_t RegRbp;
    uint64_t RegRsi;
    uint64_t RegRdi;
    uint64_t RegR8;
    uint64_t RegR9;
    uint64_t RegR10;
    uint64_t RegR11;
    uint64_t RegR12;
    uint64_t RegR13;
    uint64_t RegR14;
    uint64_t RegR15;
    uint64_t RegCr2;
    uint64_t RegFlags;
    uint64_t RegDr7;
    uint64_t RegRip;
    uint64_t RegCr0;
    uint64_t RegCr4;
    uint64_t RegCr3;
    uint64_t RegCr8;
    uint64_t RegIdtBase;
    uint64_t RegIdtLimit;
    uint64_t RegGdtBase;
    uint64_t RegGdtLimit;

    uint64_t FpuRip;
} SHEMU_GPR_REGS, *PSHEMU_GPR_REGS;


//
// Segment register (with its hidden part).
//
typedef struct _SHEMU_SEG
{
    uint64_t    Base;
    uint64_t    Limit;
    uint64_t    Selector;
    uint64_t    AccessRights;
} SHEMU_SEG, *PSHEMU_SEG;


//
// The segment registers.
//
typedef struct _SHEMU_SEG_REGS
{
    SHEMU_SEG   Es;
    SHEMU_SEG   Cs;
    SHEMU_SEG   Ss;
    SHEMU_SEG   Ds;
    SHEMU_SEG   Fs;
    SHEMU_SEG   Gs;
} SHEMU_SEG_REGS, *PSHEMU_SEG_REGS;


//
// Emulation context. All of these fields must be provided as input, although most of them can be 0.
//
typedef struct _SHEMU_CONTEXT
{
    // Current instruction. Doesn't have to be provided; it always contains the currently emulated instruction.
    // When #ShemuEmulate returns, this will contain the last emulated instruction. In case of an emulation failure,
    // it can be inspected, to gather more info about what went wrong.
    INSTRUX         Instruction;

    // General purpose registers state. On input, the initial state. Will be updated after each emulated instruction.
    SHEMU_GPR_REGS  Registers;

    // Segment registers state. On input, the initial state. May be updated after some instructions.
    SHEMU_SEG_REGS  Segments;

    // MMX register state. 8 x 8 bytes = 64 bytes for the MMX registers. Can be provided on input, if needed.
    uint64_t        MmxRegisters[ND_MAX_MMX_REGS];

    // SSE registers state. 32 x 64 bytes = 2048 bytes for the SSE registers. Can be provided on input, if needed.
    uint8_t         SseRegisters[ND_MAX_SSE_REGS * ND_MAX_REGISTER_SIZE];

    // General purpose registers write bitmap. After the first write, a register will be marked dirty in here. 
    // Should be 0 on input.
    uint16_t        DirtyGprBitmap;

    // Operating mode (ND_CODE_16, ND_CODE_32 or ND_CODE_64). Must be provided as input.
    uint8_t         Mode;

    // Operating ring (0, 1, 2, 3). Must be provided as input.
    uint8_t         Ring;

    // The suspicious code to be emulated. Must be provided as input.
    uint8_t         *Shellcode;

    // Virtual stack. RSP will point somewhere inside. Must be allocated as input, and it can be initialized with
    // actual stack contents. Can also be 0-filled.
    uint8_t         *Stack;

    // Internal use. Must be at least the size of the shell + stack. Needs not be initialized, but must be allocated
    // and accessible on input.
    uint8_t         *Intbuf;

    // Shellcode base address (the address the shellcode would see). Must be provided as input.
    uint64_t        ShellcodeBase;

    // Stack base address (the RSP the shellcode would see). Must be provided as input.
    uint64_t        StackBase;

    // Shellcode size. Must be provided as input. Usually just a page in size, but can be larger.
    uint32_t        ShellcodeSize;

    // Stack size. Must be provided as input. Minimum two pages.
    uint32_t        StackSize;

    // Internal buffer size. Must be provided as input. Must be at least the size of the shell + stack.
    uint32_t        IntbufSize;

    // Number of NOPs encountered. Should be 0 on input.
    uint32_t        NopCount;

    // The length of the string constructed on the stack, if any. Should be 0 on input.
    uint32_t        StrLength;

    // Number of external memory access (outside stack/shellcode). Should be 0 on input.
    uint32_t        ExtMemAccess;

    // Number of emulated instructions. Should be 0 on input. Once InstructionsCount reaches MaxInstructionsCount,
    // emulation will stop.
    uint32_t        InstructionsCount;

    // Max number of instructions that should be emulated. Once this limit has been reached, emulation will stop. 
    // Lower values will mean faster processing, but less chances of detection. Higher values mean low performance,
    // but very high chances of yielding useful results. Must be provided as input. 
    uint32_t        MaxInstructionsCount;

    // Base address of the Thread Information Block (the TIB the shellcode would normally see). Must be provided as 
    // input.
    uint64_t        TibBase;

    // Shellcode Flags (see SHEMU_FLAG_*). Must be provided as input.
    uint64_t        Flags;

    // Emulation options. See SHEMU_OPT_* for possible options. Must be provided as input.
    uint32_t        Options;

    // Percent of NOPs (out of total instructions emulated) that trigger NOP sled detection. Must be provided as input. 
    uint32_t        NopThreshold;

    // Stack string length threshold. Stack-constructed strings must be at least this long to trigger stack string
    // detection. Must be provided as input.
    uint32_t        StrThreshold;

    // Number of external mem accesses threshold. No more than this number of external accesses will be issued. Must 
    // be provided as input.
    uint32_t        MemThreshold;

    // Optional auxiliary data, provided by the integrator. Can be NULL, or can point to integrator specific data.
    // Shemu will not use this data in any way, but callbacks that receive a SHEMU_CONTEXT pointer (such as
    // #AccessMemory) can use it to reference integrator private information.
    void            *AuxData;

    // If provided, will be used for tracing. Can be NULL.
    ShemuPrint      Log;

    // If provided, will try to access additional memory. Can be NULL.
    ShemuMemAccess  AccessMemory;

} SHEMU_CONTEXT, *PSHEMU_CONTEXT;


typedef unsigned int SHEMU_STATUS;


//
// Emulation abort reasons.
//
#define SHEMU_SUCCESS                           0           // Successfully emulated up to MaxInstructions.
#define SHEMU_ABORT_GLA_OUTSIDE                 1           // Address accessed outside the shellcode or stack page.
#define SHEMU_ABORT_BRANCH_OUTSIDE              2           // A branch outside the shellcode page.
#define SHEMU_ABORT_UNSUPPORTED_INSTRUX         3           // A valid but unsupported instruction was encountered.
#define SHEMU_ABORT_INVALID_INSTRUX             4           // An invalid instruction was encountered.
#define SHEMU_ABORT_ADDRESSING_UNSUPPORTED      5           // An unsupported addressing scheme used (ie, VSIB).
#define SHEMU_ABORT_REGISTER_NOT_SUPPORTED      6           // An unsupported register was used (ie, DR).
#define SHEMU_ABORT_INVALID_PARAMETER           7           // An invalid parameter was supplied.
#define SHEMU_ABORT_OP_TOO_LARGE                8           // An operand that is too large was encountered.
#define SHEMU_ABORT_NO_PRIVILEGE                9           // A privileged instruction outside kernel mode.
#define SHEMU_ABORT_CANT_EMULATE                10          // A valid, but only partially handled instruction.
#define SHEMU_ABORT_SHELLCODE_DETECTED          0xFFFFFFFF  // Shellcode criteria met (see the shellcode flags).


//
// Shellcode flags.
//
// General and user-mode flags.
#define SHEMU_FLAG_NOP_SLED                     0x00000001  // Long sequence of NOP instructions.
#define SHEMU_FLAG_LOAD_RIP                     0x00000002  // The code loads RIP (CALL/POP, FNSTENV/POP, etc.)
#define SHEMU_FLAG_WRITE_SELF                   0x00000004  // The code writes itself (decryption, unpacking, etc.)
#define SHEMU_FLAG_TIB_ACCESS                   0x00000008  // The code accesses the TIB.
#define SHEMU_FLAG_SYSCALL                      0x00000010  // The code does a direct syscall/sysenter.
#define SHEMU_FLAG_STACK_STR                    0x00000020  // The code constructs & uses strings on the stack.
// Kernel specific flags.
#define SHEMU_FLAG_KPCR_ACCESS                  0x00010000  // KPCR current thread access via gs:[0x188]/fs:[0x124].
#define SHEMU_FLAG_SWAPGS                       0x00020000  // SWAPGS was executed.
#define SHEMU_FLAG_SYSCALL_MSR_READ             0x00040000  // A SYSCALL/SYSENTER MSR read.
#define SHEMU_FLAG_SYSCALL_MSR_WRITE            0x00080000  // A SYSCALL/SYSENTER MSR write.


//
// Emulation options.
//
#define SHEMU_OPT_TRACE_EMULATION               0x00000001  // Trace each emulated instruction.
#define SHEMU_OPT_STOP_ON_EXPLOIT               0x00000002  // When shellcode indications are confirmed, stop emulation.


//
// At least this amount must be allocated for internal use.
//
#define SHEMU_INTERNAL_BUFFER_SIZE(ctx)         ((ctx)->ShellcodeSize + (ctx)->StackSize)


//
// API
//
SHEMU_STATUS
ShemuEmulate(
    SHEMU_CONTEXT *Context
    );


#endif // BDSHEMU_H
