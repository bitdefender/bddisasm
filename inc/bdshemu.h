/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef BDSHEMU_H
#define BDSHEMU_H


#include "bddisasm.h"


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
typedef ND_BOOL
(*ShemuMemAccess)(
    void *ShemuContext, // Shemu emulation context.
    ND_UINT64 Gla,      // Linear address to be accessed.
    ND_SIZET Size,      // Number of bytes to access.
    ND_UINT8 *Buffer,   // Contains the read content (if Store is false), or the value to be stored at Gla.
    ND_BOOL Store       // If false, read content at Gla. Otherwise, write content at Gla.
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
    ND_UINT64   RegRax;
    ND_UINT64   RegRcx;
    ND_UINT64   RegRdx;
    ND_UINT64   RegRbx;
    ND_UINT64   RegRsp;
    ND_UINT64   RegRbp;
    ND_UINT64   RegRsi;
    ND_UINT64   RegRdi;
    ND_UINT64   RegR8;
    ND_UINT64   RegR9;
    ND_UINT64   RegR10;
    ND_UINT64   RegR11;
    ND_UINT64   RegR12;
    ND_UINT64   RegR13;
    ND_UINT64   RegR14;
    ND_UINT64   RegR15;
    ND_UINT64   RegCr2;
    ND_UINT64   RegFlags;
    ND_UINT64   RegDr7;
    ND_UINT64   RegRip;
    ND_UINT64   RegCr0;
    ND_UINT64   RegCr4;
    ND_UINT64   RegCr3;
    ND_UINT64   RegCr8;
    ND_UINT64   RegIdtBase;
    ND_UINT64   RegIdtLimit;
    ND_UINT64   RegGdtBase;
    ND_UINT64   RegGdtLimit;
    ND_UINT64   FpuRip;
} SHEMU_GPR_REGS, *PSHEMU_GPR_REGS;


//
// Segment register (with its hidden part).
//
typedef struct _SHEMU_SEG
{
    ND_UINT64   Base;
    ND_UINT64   Limit;
    ND_UINT64   Selector;
    ND_UINT64   AccessRights;
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
    ND_UINT64       MmxRegisters[ND_MAX_MMX_REGS];

    // SSE registers state. 32 x 64 bytes = 2048 bytes for the SSE registers. Can be provided on input, if needed.
    ND_UINT8        SseRegisters[ND_MAX_SSE_REGS * ND_MAX_REGISTER_SIZE];

    // General purpose registers write bitmap. After the first write, a register will be marked dirty in here. 
    // Should be 0 on input.
    ND_UINT16       DirtyGprBitmap;

    // Operating mode (ND_CODE_16, ND_CODE_32 or ND_CODE_64). Must be provided as input.
    ND_UINT8        Mode;

    // Operating ring (0, 1, 2, 3). Must be provided as input.
    ND_UINT8        Ring;

    // The suspicious code to be emulated. Must be provided as input.
    ND_UINT8        *Shellcode;

    // Virtual stack. RSP will point somewhere inside. Must be allocated as input, and it can be initialized with
    // actual stack contents. Can also be 0-filled.
    ND_UINT8        *Stack;

    // Internal use. Must be at least the size of the shell + stack. Needs not be initialized, but must be allocated
    // and accessible on input.
    ND_UINT8        *Intbuf;

    // Shellcode base address (the address the shellcode would see). Must be provided as input.
    ND_UINT64       ShellcodeBase;

    // Stack base address (the RSP the shellcode would see). Must be provided as input.
    ND_UINT64       StackBase;

    // Shellcode size. Must be provided as input. Usually just a page in size, but can be larger.
    ND_UINT32       ShellcodeSize;

    // Stack size. Must be provided as input. Minimum two pages.
    ND_UINT32       StackSize;

    // Internal buffer size. Must be provided as input. Must be at least the size of the shell + stack.
    ND_UINT32       IntbufSize;

    // Number of NOPs encountered. Should be 0 on input.
    ND_UINT32       NopCount;

    // The length of the string constructed on the stack, if any. Should be 0 on input.
    ND_UINT32       StrLength;

    // Number of external memory access (outside stack/shellcode). Should be 0 on input.
    ND_UINT32       ExtMemAccess;

    // Number of emulated instructions. Should be 0 on input. Once InstructionsCount reaches MaxInstructionsCount,
    // emulation will stop.
    ND_UINT32       InstructionsCount;

    // Max number of instructions that should be emulated. Once this limit has been reached, emulation will stop. 
    // Lower values will mean faster processing, but less chances of detection. Higher values mean low performance,
    // but very high chances of yielding useful results. Must be provided as input. 
    ND_UINT32       MaxInstructionsCount;

    // Base address of the Thread Information Block (the TIB the shellcode would normally see). Must be provided as 
    // input.
    ND_UINT64       TibBase;

    // Shellcode Flags (see SHEMU_FLAG_*). Must be provided as input.
    ND_UINT64       Flags;

    // Emulation options. See SHEMU_OPT_* for possible options. Must be provided as input.
    ND_UINT32       Options;

    // Percent of NOPs (out of total instructions emulated) that trigger NOP sled detection. Must be provided as input. 
    ND_UINT32       NopThreshold;

    // Stack string length threshold. Stack-constructed strings must be at least this long to trigger stack string
    // detection. Must be provided as input.
    ND_UINT32       StrThreshold;

    // Number of external mem accesses threshold. No more than this number of external accesses will be issued. Must 
    // be provided as input.
    ND_UINT32       MemThreshold;

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
#define SHEMU_FLAG_TIB_ACCESS                   0x00000008  // The code accesses the PEB field inside TIB.
#define SHEMU_FLAG_SYSCALL                      0x00000010  // The code does a direct syscall/sysenter/int 0x2e|0x80.
#define SHEMU_FLAG_STACK_STR                    0x00000020  // The code constructs & uses strings on the stack.
#define SHEMU_FLAG_TIB_ACCESS_WOW32             0x00000040  // The code accesses the Wow32Reserved field inside TIB.
#define SHEMU_FLAG_HEAVENS_GATE                 0x00000080  // The code uses Heaven's gate to switch into 64 bit mode.
#define SHEMU_FLAG_STACK_PIVOT                  0x00000100  // The code switched the stack using XCHG esp, *.
#define SHEMU_FLAG_SUD_ACCESS                   0x00000200  // The code accesses the KUSER_SHARED_DATA page.
// Kernel specific flags.
#define SHEMU_FLAG_KPCR_ACCESS                  0x00010000  // KPCR current thread access via gs:[0x188]/fs:[0x124].
#define SHEMU_FLAG_SWAPGS                       0x00020000  // SWAPGS was executed.
#define SHEMU_FLAG_SYSCALL_MSR_READ             0x00040000  // A SYSCALL/SYSENTER MSR read.
#define SHEMU_FLAG_SYSCALL_MSR_WRITE            0x00080000  // A SYSCALL/SYSENTER MSR write.
#define SHEMU_FLAG_SIDT                         0x00100000  // SIDT was executed.


//
// Emulation options.
//
#define SHEMU_OPT_TRACE_EMULATION               0x00000001  // Trace each emulated instruction.
#define SHEMU_OPT_STOP_ON_EXPLOIT               0x00000002  // When shellcode indications are confirmed, stop emulation.
#define SHEMU_OPT_BYPASS_SELF_WRITES            0x00000004  // When a shellcode self-modifies, the modification will
                                                            // not be committed. Use this when emulating an already
                                                            // decoded shellcode, where emulating the decryption again
                                                            // will in fact scramble the shellcode and make it useless.
#define SHEMU_OPT_SUPPORT_AES                   0x00010000  // Indicates that AES instructions are supported, and 
                                                            // therefore, the AES intrinsics can be used to emulate
                                                            // AES decryption.


//
// At least this amount must be allocated for internal use.
//
#define SHEMU_INTERNAL_BUFFER_SIZE(ctx)         ((ctx)->ShellcodeSize + (ctx)->StackSize)


#ifdef __cplusplus 
extern "C" {
#endif

//
// API
//
SHEMU_STATUS
ShemuEmulate(
    SHEMU_CONTEXT *Context
    );

#ifdef __cplusplus 
}
#endif

#endif // BDSHEMU_H
