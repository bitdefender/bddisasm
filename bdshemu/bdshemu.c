/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include "bdshemu.h"
#include "include/bdshemu_common.h"
#include "../bddisasm/include/bddisasm_crt.h"

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) ((void)(P))
#endif


//
// shemu_internal_printf - simple version
//

void
shemu_internal_printf(
    SHEMU_CONTEXT *Context,
    char *formatstring,
    ...
    )
{
    char buff[768];
    va_list args;

    if (ND_NULL == Context->Log)
    {
        return;
    }

    nd_memzero(buff, sizeof(buff));
    va_start(args, formatstring);

    nd_vsnprintf_s(buff, sizeof(buff), sizeof(buff) - 1, formatstring, args);

    va_end(args);

    Context->Log(buff, Context->AuxData);
}


//
// shemu_memcpy
//
void *
shemu_memcpy(
    void *Dest,
    const void *Source,
    ND_SIZET Size
    )
{
    if (ND_NULL == Dest)
    {
        return ND_NULL;
    }

    if (ND_NULL == Source)
    {
        return ND_NULL;
    }

    for (ND_SIZET index = 0; index < Size; index++)
    {
        ((char *)Dest)[index] = ((const char *)Source)[index];
    }

    return Dest;
}


//
// ShemuHexlify
//
void
ShemuHexlify(
    ND_UINT8 *Value,
    ND_UINT64 ValueSize,
    char *Hex,
    ND_UINT64 HexSize
    )
{
    const char hexDigits[16] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    nd_memzero(Hex, (ND_SIZET)HexSize);

    if (HexSize <= ValueSize * 2)
    {
        return;
    }

    // Convert instruction bytes to string.
    for (ND_UINT32 i = 0; i < ValueSize; i++)
    {
        Hex[(i * 2) + 0] = hexDigits[(Value[i] >> 4) & 0xF];
        Hex[(i * 2) + 1] = hexDigits[(Value[i] >> 0) & 0xF];
    }

    // Since we make sure HexSize is greater than twice the size of the Value, and since we always nd_memzero Hex 
    // at the beginning, we are guaranteed that Hex will be properly NULL terminated.
}


//
// ShemuBmpStateUpdate
//
void
ShemuBmpStateUpdate(
    ND_UINT8 *Bitmap,
    ND_UINT64 Size,
    ND_UINT64 Start,
    ND_UINT64 Count,
    ND_UINT8 Flags,
    ND_BOOL Clear
    )
//
// Sets the indicated state inside the Bitmap.
//
{
    ND_UINT64 i;

    if (ND_NULL == Bitmap || Start >= Size || Count > Size || Start + Count > Size)
    {
        return;
    }

    for (i = 0; i < Count; i++)
    {
        if (!Clear)
        {
            Bitmap[Start + i] |= Flags;
        }
        else
        {
            Bitmap[Start + i] &= ~Flags;
        }
    }
}


//
// ShemuBmpStateCheck
//
ND_BOOL
ShemuBmpStateCheck(
    ND_UINT8 *Bitmap,
    ND_UINT64 Size,
    ND_UINT64 Start,
    ND_UINT64 Count,
    ND_UINT8 Flags
    )
//
// Check if any of the state bytes in the Bitmap has any of the Flags set.
// 
// @returns ND_TRUE if any state byte contains the indicated Flags.
//
{
    ND_UINT64 i;

    if (ND_NULL == Bitmap || Start >= Size || Count > Size || Start + Count > Size)
    {
        return ND_FALSE;
    }

    for (i = 0; i < Count; i++)
    {
        if (Bitmap[Start + i] & Flags)
        {
            return ND_TRUE;
        }
    }

    return ND_FALSE;
}


//
// ShemuIsShellcodePtr
//
ND_BOOL
ShemuIsShellcodePtr(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size
    )
{
    return (Gla >= Context->ShellcodeBase && Gla < Context->ShellcodeBase + Context->ShellcodeSize &&
            Gla + Size > Context->ShellcodeBase && Gla + Size <= Context->ShellcodeBase + Context->ShellcodeSize &&
            Size <= Context->ShellcodeSize);
}


//
// ShemuIsStackPtr
//
ND_BOOL
ShemuIsStackPtr(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size
    )
{
    return (Gla >= Context->StackBase && Gla < Context->StackBase + Context->StackSize &&
            Gla + Size > Context->StackBase && Gla + Size <= Context->StackBase + Context->StackSize &&
            Size <= Context->StackSize);
}


//
// ShemuIsIcachePtr
//
ND_BOOL
ShemuIsIcachePtr(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size
    )
{
    return (Gla >= Context->Icache.Address && Gla < Context->Icache.Address + Context->Icache.Size &&
            Gla + Size > Context->Icache.Address && Gla + Size <= Context->Icache.Address + Context->Icache.Size &&
            Size <= Context->Icache.Size);
}


//
// ShemuCopyMem
//
void
ShemuCopyMem(
    ND_UINT8 *Destination,
    ND_UINT8 *Source,
    ND_UINT64 Size
    )
{
    shemu_memcpy(Destination, Source, (ND_SIZET)Size);
}


//
// ShemuMemLoad
//
SHEMU_STATUS
ShemuMemLoad(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size,
    ND_UINT8 *Value
    )
{
    if (ShemuIsShellcodePtr(Context, Gla, Size))
    {
        // Shellcode access.
        if (0 == (Context->Options & SHEMU_OPT_DIRECT_MAPPED_SHELL))
        {
            // Shellocode is local copy, we can directly access it.
            ShemuCopyMem(Value, Context->Shellcode + (Gla - Context->ShellcodeBase), Size);
        }
        else
        {
            // Shellcode is directly mapped, we must go through the shellcode access API.
            if ((ND_NULL == Context->AccessShellcode) ||
                !Context->AccessShellcode(Context, Gla, (ND_SIZET)Size, Value, ND_FALSE))
            {
                return SHEMU_ABORT_GLA_OUTSIDE;
            }
        }
    }
    else if (ShemuIsStackPtr(Context, Gla, Size))
    {
        ShemuCopyMem(Value, Context->Stack + (Gla - Context->StackBase), Size);
    }
    else
    {
        ND_UINT32 selfOffset = 0;
        ND_BOOL res = ND_FALSE;

        // We allow a maximum number of external memory accesses, due to performance reasons.
        if (++Context->ExtMemAccess > Context->MemThreshold)
        {
            goto _check_special_access;
        }

        // NOTE: The accessed GLA may partially access an internal address (shellcode or stack) and an external address.
        // Since the AccessMemory callback can be provided with the full SHEMU_CONTEXT, the integrator can choose how
        // to handle those accesses; some options are:
        // - Don't handle them at all, and return error (ND_FALSE);
        // - Handle them by reading the actual memory value; this has the disadvantage that if the shellcode/stack 
        //   portion has been modified due to emulation, the AccessMemory function would return the original memory 
        //   value;
        // - Handle them properly, by returning the emulated values for the internal addresses, and the external
        //   values for the external addresses.
        // bdshemu does not care directly about this, and lets the integrator choose his own strategy.
        if (ND_NULL != Context->AccessMemory)
        {
            res = Context->AccessMemory(Context, Gla, (ND_SIZET)Size, Value, ND_FALSE);
        }

        if (res)
        {
            return SHEMU_SUCCESS;
        }
        
    _check_special_access:
        if (Context->ArchType == SHEMU_ARCH_TYPE_X86)
        {
            selfOffset = Context->Arch.X86.Mode == ND_CODE_32 ? 0x18 : 0x30;
        }
        else
        {
            return SHEMU_ABORT_INVALID_PARAMETER;
        }

        // If we got here, the external memory access has not been handled.
        if (Gla == Context->TibBase + selfOffset)
        {
            // TEB.Self access.
            if (Size == 4)
            {
                *(ND_UINT32*)Value = (ND_UINT32)Context->TibBase;
                return SHEMU_SUCCESS;
            }
            else if (Size == 8)
            {
                *(ND_UINT64*)Value = (ND_UINT64)Context->TibBase;
                return SHEMU_SUCCESS;
            }
        }

        return SHEMU_ABORT_GLA_OUTSIDE;
    }

    return SHEMU_SUCCESS;
}


//
// ShemuMemStore
//
SHEMU_STATUS
ShemuMemStore(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size,
    ND_UINT8 *Value
    )
{
    if (ShemuIsShellcodePtr(Context, Gla, Size))
    {
        // Bypass self-writes, if needed to. No need to invalidate the icache in this case.
        if (!!(Context->Options & SHEMU_OPT_BYPASS_SELF_WRITES))
        {
            return SHEMU_SUCCESS;
        }

        // Flush the instruction cache, if any stored address in it. We can do this AFTER checking
        // for SHEMU_OPT_BYPASS_SELF_WRITES - with that flag set, the shellcode will never be modified.
        if (ShemuIsIcachePtr(Context, Gla, 1) || ShemuIsIcachePtr(Context, Gla + Size - 1, 1))
        {
            ShemuFlushIcache(Context);
        }

        // Shellcode access.
        if (0 == (Context->Options & SHEMU_OPT_DIRECT_MAPPED_SHELL))
        {
            // Shellocode is local copy, we can directly access it.
            ShemuCopyMem(Context->Shellcode + (Gla - Context->ShellcodeBase), Value, Size);
        }
        else
        {
            // Shellcode is directly mapped, we must go through the memory access API. Note that the integrator
            // is free to do whatever it wants with this call - generally, it is a good idea to just discard 
            // modifications. For safety, currently we force the SHEMU_OPT_BYPASS_SELF_WRITES option whenever
            // the SHEMU_OPT_DIRECT_MAPPED_SHELL option is used, to avoid problems.
            if ((ND_NULL == Context->AccessShellcode) ||
                !Context->AccessShellcode(Context, Gla, (ND_SIZET)Size, Value, ND_TRUE))
            {
                return SHEMU_ABORT_GLA_OUTSIDE;
            }
        }
    }
    else if (ShemuIsStackPtr(Context, Gla, Size))
    {
        ShemuCopyMem(Context->Stack + (Gla - Context->StackBase), Value, Size);
    }
    else
    {
        ND_BOOL res = ND_FALSE;

        // We allow a maximum number of external memory accesses, due to performance reasons.
        if (++Context->ExtMemAccess > Context->MemThreshold)
        {
            return SHEMU_ABORT_GLA_OUTSIDE;
        }

        // Handling external stores made by the shellcode can be done in a variety of ways by the integrator. Some
        // of the solutions are:
        // - Abort on external stores; this will cause the emulation to immediately stop;
        // - Discard external stores; this is very simple, and it assumes that modified memory addresses will
        //   not be read later on by the shellcode;
        // - Create a store-buffer like structure, where every external store is cached; when a load is issued on
        //   a previously written address, the value from the store-buffer can be returned;
        // For obvious reasons, actually storing the value at the indicated address is a very, very bad idea.
        if (ND_NULL != Context->AccessMemory)
        {
            res = Context->AccessMemory(Context, Gla, (ND_SIZET)Size, Value, ND_TRUE);
        }

        if (res)
        {
            return SHEMU_SUCCESS;
        }

        return SHEMU_ABORT_GLA_OUTSIDE;
    }

    return SHEMU_SUCCESS;
}


//
// ShemuMemFetch
//
SHEMU_STATUS
ShemuMemFetch(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size,
    ND_UINT8 *Bytes
    )
{
    ND_UINT8 *addr;
    ND_UINT64 offset;

    if (Size > SHEMU_ICACHE_SIZE || Size == 0)
    {
        return SHEMU_ABORT_INVALID_PARAMETER;
    }

    if (!ShemuIsShellcodePtr(Context, Gla, Size))
    {
        return SHEMU_ABORT_RIP_OUTSIDE;
    }

    addr = Context->Shellcode;
    offset = Gla - Context->ShellcodeBase;

    if (0 != (Context->Options & SHEMU_OPT_DIRECT_MAPPED_SHELL))
    {
        // If the entire address is NOT inside the cache, re-fill the entire line, starting with Gla.
        if (!ShemuIsIcachePtr(Context, Gla, 1) || !ShemuIsIcachePtr(Context, Gla + Size - 1, 1))
        {
            // Re-fill the line.
            Context->Icache.Address = Gla;
            Context->Icache.Size = MIN(SHEMU_ICACHE_SIZE, Context->ShellcodeSize - offset);

            if (Context->Icache.Size < Size)
            {
                return SHEMU_ABORT_RIP_OUTSIDE;
            }

            if ((ND_NULL == Context->AccessShellcode) || 
                !Context->AccessShellcode(Context, Gla, (ND_SIZET)Context->Icache.Size, Context->Icache.Icache, ND_FALSE))
            {
                return SHEMU_ABORT_FETCH_ERROR;
            }
        }

        addr = Context->Icache.Icache;
        offset = Gla - Context->Icache.Address;
    }

    shemu_memcpy(Bytes, addr + offset, (ND_SIZET)Size);

    return SHEMU_SUCCESS;
}


void
ShemuFlushIcache(
    SHEMU_CONTEXT *Context
    )
{
    Context->Icache.Address = 0;
    Context->Icache.Size = 0;
}


//
// ShemuDisplayMemValue
//
void
ShemuDisplayMemValue(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT64 Size,
    ND_UINT8 *Value,
    ND_BOOL Load
    )
{
    char svalue[ND_MAX_REGISTER_SIZE * 2 + 2] = { 0 };

    if (Size > ND_MAX_REGISTER_SIZE)
    {
        // Truncate the displayed value to 64 bytes.
        Size = ND_MAX_REGISTER_SIZE;
    }

    ShemuHexlify(Value, Size, svalue, sizeof(svalue));

    shemu_printf(Context, "        Memory %s, address 0x%016llx, size %lld, value %s\n", 
                Load ? "LOAD" : "STOR", Gla, Size, svalue);
}


//
// ShemuEmulate
//
SHEMU_STATUS
ShemuEmulate(
    SHEMU_CONTEXT *Context
    )
{
    if (Context->ArchType == SHEMU_ARCH_TYPE_X86)
    {
        return ShemuX86Emulate(Context);
    }
    else
    {
        return SHEMU_ABORT_INVALID_PARAMETER;
    }
}
