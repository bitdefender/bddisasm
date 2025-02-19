/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef BDSHEMU_COMMON_H_
#define BDSHEMU_COMMON_H_

#if defined(_MSC_VER)
#include <sal.h>
#else
#define _Analysis_assume_(x)
#endif


#ifndef BDDISASM_NO_FORMAT
#define shemu_printf(Context, formatstring, ...) shemu_internal_printf(Context, formatstring, __VA_ARGS__)
#else
#define shemu_printf(Context, formatstring, ...) do { (void)formatstring; shemu_internal_printf(Context, NULL, __VA_ARGS__); } while (0)
#endif

// The SHELLBMP and STACKBMP are two bitmaps which hold the state of each shellcode byte and each stack byte.
// Inside SHELLBMP, we store whether a shellcode byte has been fetched for execution or not, and whether it was 
// modified or not.
// Inside STACKBMP, we store whether a stack byte is part of a previously saved RIP value or not.
#define STACKBMP(ctx)           ((ctx)->Intbuf)
#define STACKBMP_SIZE(ctx)      ((ctx)->StackSize)
#define SHELLBMP(ctx)           (!((ctx)->Options & SHEMU_OPT_DIRECT_MAPPED_SHELL) ? (ctx)->Intbuf + (ctx)->StackSize : ND_NULL)
#define SHELLBMP_SIZE(ctx)      (!((ctx)->Options & SHEMU_OPT_DIRECT_MAPPED_SHELL) ? (ctx)->ShellcodeSize : 0)
#define MAX(a, b)               ((a) < (b) ? (b) : (a))
#define MIN(a, b)               ((a) > (b) ? (b) : (a))


// Flags used for the shellcode. These are set inside SHELLBMP. Each byte at index X inside SHELLBMP indicates the state 
// of the shellcode byte at index X.
#define SHELL_BYTE_DIRTY        (0x01)      // The location inside the shellcode has been written.
#define SHELL_BYTE_FETCHED      (0x02)      // The location inside the shellcode has been fetched for execution.
#define SHELL_BYTE_IBYTES       (0x04)      // The location inside the shellcode represents executed instruction 
                                            // bytes. Does not include the first instruction byte, which is marked
                                            // using the SHELL_BYTE_FETCHED flag.

// Flags used for the stack. These are set inside STACKBMP. Each byte at index X inside STACKBMP indicates the state
// of the stack byte at index X.
#define STACK_BYTE_RIP          (0x01)      // The location inside the stack contains a RIP portion.

// Flags used for the GPR tracking mechanism. Each byte X inside the GPR tracker array indicates the state of said GPR.
#define GPR_TRACK_READ          (0x01)      // The GPR has been read, by any instruction (including RMW).
#define GPR_TRACK_DIRTY         (0x02)      // The GPR has been modified.


#define GET_BITS(val, start, stop)  (((val) >> (start)) & ((1ULL << ((stop) - (start) + 1)) - 1))

#define ShemuShellBmpStateSet(Context, Start, Count, Flags)     \
    ShemuBmpStateUpdate(SHELLBMP(Context), SHELLBMP_SIZE(Context), Start, Count, Flags, ND_FALSE)

#define ShemuShellBmpStateClear(Context, Start, Count, Flags)   \
    ShemuBmpStateUpdate(SHELLBMP(Context), SHELLBMP_SIZE(Context), Start, Count, Flags, ND_TRUE)

#define ShemuShellBmpStateCheck(Context, Start, Count, Flags)    \
    ShemuBmpStateCheck(SHELLBMP(Context), SHELLBMP_SIZE(Context), Start, Count, Flags)

#define ShemuStackBmpStateSet(Contxt, Start, Count, Flags)      \
    ShemuBmpStateUpdate(STACKBMP(Context), STACKBMP_SIZE(Context), Start, Count, Flags, ND_FALSE)

#define ShemuStackBmpStateClear(Context, Start, Count, Flags)   \
    ShemuBmpStateUpdate(STACKBMP(Context), STACKBMP_SIZE(Context), Start, Count, Flags, ND_TRUE)

#define ShemuStackBmpStateCheck(Context, Start, Count, Flags)    \
    ShemuBmpStateCheck(STACKBMP(Context), STACKBMP_SIZE(Context), Start, Count, Flags)




void
shemu_internal_printf(
    SHEMU_CONTEXT *Context,
    char *formatstring,
    ...
    );

void *
shemu_memcpy(
    void *Dest,
    const void *Source,
    ND_SIZET Size
    );

void
ShemuHexlify(
    ND_UINT8 *Value,
    ND_UINT64 ValueSize,
    char *Hex,
    ND_UINT64 HexSize
    );

void
ShemuBmpStateUpdate(
    ND_UINT8 *Bitmap,
    ND_UINT64 Size,
    ND_UINT64 Start,
    ND_UINT64 Count,
    ND_UINT8 Flags,
    ND_BOOL Clear
    );

ND_BOOL
ShemuBmpStateCheck(
    ND_UINT8 *Bitmap,
    ND_UINT64 Size,
    ND_UINT64 Start,
    ND_UINT64 Count,
    ND_UINT8 Flags
    );

ND_BOOL
ShemuIsShellcodePtr(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size
    );

ND_BOOL
ShemuIsStackPtr(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size
    );

ND_BOOL
ShemuIsIcachePtr(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size
    );

SHEMU_STATUS
ShemuMemLoad(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size,
    ND_UINT8 *Value
    );

SHEMU_STATUS
ShemuMemStore(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size,
    ND_UINT8 *Value
    );

SHEMU_STATUS
ShemuMemFetch(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size,
    ND_UINT8 *Bytes
    );

void
ShemuFlushIcache(
    SHEMU_CONTEXT *Context
    );

void
ShemuDisplayMemValue(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size,
    ND_UINT8 *Value,
    ND_BOOL Load
    );

#endif // BDSHEMU_COMMON_H_