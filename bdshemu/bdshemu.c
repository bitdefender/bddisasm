/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//
// bdshemu.c
//

#include "bdshemu.h"
#include "../bddisasm/include/nd_crt.h"

#ifdef __clang__
#include <wmmintrin.h>
#else
#if defined(ND_ARCH_X64) || defined(ND_ARCH_X86)
#include <immintrin.h>
#endif // defined(ND_ARCH_X64) || defined(ND_ARCH_X86)
#endif // __clang__

//
// A generic emulator value.
//
typedef struct _SHEMU_VALUE
{
    union
    {
        ND_UINT8        Bytes[ND_MAX_REGISTER_SIZE];
        ND_UINT16       Words[ND_MAX_REGISTER_SIZE / sizeof(ND_UINT16)];
        ND_UINT32       Dwords[ND_MAX_REGISTER_SIZE / sizeof(ND_UINT32)];
        ND_UINT64       Qwords[ND_MAX_REGISTER_SIZE / sizeof(ND_UINT64)];

        struct
        {
            ND_UINT16   FpuControlWord;
            ND_UINT16   FpuStatusWord;
            ND_UINT16   FpuTagWord;
            ND_UINT16   Rsvd;
            ND_UINT32   FpuDataPointer;
            ND_UINT32   FpuInstructionPointer;
            ND_UINT32   FpuLastInstructionOpcode;
        } FpuEnvironment;

        struct
        {
            ND_UINT16   FpuControlWord;
            ND_UINT16   FpuStatuwsWord;
            ND_UINT16   FpuTagWord;
            ND_UINT16   FpuOpcode;
            ND_UINT64   FpuRip;
            ND_UINT64   FpuDataPointer;
            ND_UINT32   Mxcsr;
            ND_UINT32   MxcsrMask;
        } XsaveArea;

        struct
        {
            ND_UINT16   Limit;
            ND_UINT64   Base;
        } Descriptor;

    } Value;

    ND_OPERAND_SIZE     Size;
} SHEMU_VALUE, *PSHEMU_VALUE;


enum
{
    FM_LOGIC,
    FM_SHL,
    FM_SHR,
    FM_SAR,
    FM_SUB,
    FM_ADD,
} FLAGS_MODE;


#define GET_OP(ctx, op, val) {                                                                                         \
    SHEMU_STATUS status = ShemuGetOperandValue(ctx, op, val);                                                          \
    if (SHEMU_SUCCESS != status)                                                                                       \
    {                                                                                                                  \
        return status;                                                                                                 \
    }                                                                                                                  \
}

#define SET_OP(ctx, op, val) {                                                                                         \
    SHEMU_STATUS status = ShemuSetOperandValue(ctx, op, val);                                                          \
    if (SHEMU_SUCCESS != status)                                                                                       \
    {                                                                                                                  \
        return status;                                                                                                 \
    }                                                                                                                  \
}

#define GET_FLAG(ctx, flg)          (!!((ctx)->Registers.RegFlags & (flg)))
#define SET_FLAG(ctx, flg, val)     ((ctx)->Registers.RegFlags = (val) ? ((ctx)->Registers.RegFlags | flg) :           \
                                                                         ((ctx)->Registers.RegFlags & ~(flg)))
#define SET_FLAGS(ctx, dst, src1, src2, fm) ShemuSetFlags(ctx, dst.Value.Qwords[0], src1.Value.Qwords[0],               \
                                                          src2.Value.Qwords[0], dst.Size, fm)

#define SHELLBMP(ctx)               ((ctx)->Intbuf)
#define STACKBMP(ctx)               ((ctx)->Intbuf + (ctx)->ShellcodeSize)
#define SHELLBMP_SIZE(ctx)          ((ctx)->ShellcodeSize)
#define STACKBMP_SIZE(ctx)          ((ctx)->StackSize)
#define MAX(a, b)                   ((a) < (b) ? (b) : (a))
#define MIN(a, b)                   ((a) > (b) ? (b) : (a))


//
// ShemuPrintf - simple version
//
#ifndef BDDISASM_NO_FORMAT
static void
shemu_printf(
    SHEMU_CONTEXT *Context,
    char *formatstring,
    ...
    )
{
    char buff[1024];
    va_list args;

    if (ND_NULL == Context->Log)
    {
        return;
    }

    nd_memzero(buff, sizeof(buff));
    va_start(args, formatstring);

    nd_vsnprintf_s(buff, sizeof(buff), sizeof(buff) - 1, formatstring, args);

    va_end(args);

    Context->Log(buff);
}
#else
#define shemu_printf(Context, formatstring, ...)
#endif // !BDDISASM_NO_FORMAT


//
// shemu_memcpy
//
static void *
shemu_memcpy(
    void *Dest,
    const void *Source,
    ND_SIZET Size
    )
{
    void *start = Dest;
    ND_UINT32 index = 0;

    if (ND_NULL == Dest)
    {
        return ND_NULL;
    }

    if (ND_NULL == Source)
    {
        return ND_NULL;
    }

    while (Size--)
    {
        *(char *)Dest = *((char *)Source + index);
        Dest = (char *)Dest + 1;
        index++;
    }

    return start;
}


//
// ShemuBts
//
inline static ND_UINT8
ShemuBts(
    ND_UINT8 *BitMap,
    ND_UINT64 Position
    )
{
    ND_UINT8 old;

    old = (BitMap[Position / 8] >> (Position % 8)) & 1;

    BitMap[Position / 8] |= 1 << (Position % 8);

    return old;
}


//
// ShemuBtr
//
inline static ND_UINT8
ShemuBtr(
    ND_UINT8 *BitMap,
    ND_UINT64 Position
    )
{
    ND_UINT8 old;

    old = (BitMap[Position / 8] >> (Position % 8)) & 1;

    BitMap[Position / 8] &= ~(1 << (Position % 8));

    return old;
}


//
// ShemuBt
//
inline static ND_UINT8
ShemuBt(
    ND_UINT8 *BitMap,
    ND_UINT64 Position
    )
{
    return (BitMap[Position / 8] >> (Position % 8)) & 1;
}


//
// ShemuSetBits
//
static void
ShemuSetBits(
    ND_UINT8 *Bitmap,
    ND_UINT64 Start,
    ND_UINT64 Size,
    ND_BOOL Val
    )
//
// No size validations here; the caller has to make sure the ranges are all good.
//
{
    ND_UINT64 i;

    for (i = 0; i < Size; i++)
    {
        if (Val)
        {
            ShemuBts(Bitmap, (ND_UINT64)(Start + i));
        }
        else
        {
            ShemuBtr(Bitmap, (ND_UINT64)(Start + i));
        }
    }
}


////
//// ShemuAllBitsSet
////
//static ND_BOOL
//ShemuAllBitsSet(
//    ND_UINT8 *Bitmap,
//    ND_UINT64 Start,
//    ND_UINT32 Size
//    )
////
//// No size validations here; the caller has to make sure the ranges are all good.
////
//{
//    ND_UINT32 i;
//
//    for (i = 0; i < Size; i++)
//    {
//        if (!ShemuBt(Bitmap, (ND_UINT64)(Start + i)))
//        {
//            return ND_FALSE;
//        }
//    }
//
//    return ND_TRUE;
//}


//
// ShemuAnyBitsSet
//
static ND_BOOL
ShemuAnyBitsSet(
    ND_UINT8 *Bitmap,
    ND_UINT64 Start,
    ND_UINT32 Size
    )
//
// No size validations here; the caller has to make sure the ranges are all good.
//
{
    ND_UINT32 i;

    for (i = 0; i < Size; i++)
    {
        if (ShemuBt(Bitmap, (ND_UINT64)(Start + i)))
        {
            return ND_TRUE;
        }
    }

    return ND_FALSE;
}


//
// ShemuSetFlags
//
static void
ShemuSetFlags(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Dst,
    ND_UINT64 Src1,
    ND_UINT64 Src2,
    ND_OPERAND_SIZE Size,
    ND_UINT8 FlagsMode
    )
{
    ND_UINT8 pfArr[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

    // Mask the operands with their respective size.
    Dst = ND_TRIM(Size, Dst);
    Src1 = ND_TRIM(Size, Src1);
    Src2 = ND_TRIM(Size, Src2);

    if (FlagsMode == FM_SHL || FlagsMode == FM_SHR || FlagsMode == FM_SAR)
    {
        // Shift with 0 count does not affect flags.
        if (Src2 == 0)
        {
            return;
        }
    }

    // PF set if the first bytes has an even number of 1 bits.
    if ((pfArr[Dst & 0xF] + pfArr[(Dst >> 4) & 0xF]) % 2 == 0)
    {
        Context->Registers.RegFlags |= NDR_RFLAG_PF;
    }
    else
    {
        Context->Registers.RegFlags &= ~NDR_RFLAG_PF;
    }

    // ZF set if the result is zero.
    if (Dst == 0)
    {
        Context->Registers.RegFlags |= NDR_RFLAG_ZF;
    }
    else
    {
        Context->Registers.RegFlags &= ~NDR_RFLAG_ZF;
    }

    // SF is set if the sign flag is set.
    if (ND_GET_SIGN(Size, Dst) != 0)
    {
        Context->Registers.RegFlags |= NDR_RFLAG_SF;
    }
    else
    {
        Context->Registers.RegFlags &= ~NDR_RFLAG_SF;
    }

    // OF and CF are handled differently for some instructions.
    if (FM_LOGIC == FlagsMode)
    {
        // OF and CF are cleared on logic instructions.
        Context->Registers.RegFlags &= ~(NDR_RFLAG_OF | NDR_RFLAG_CF);
    }
    else if (FM_SHL == FlagsMode)
    {
        // CF is the last bit shifted out of the destination.
        if (ND_GET_BIT((Size * 8ULL) - Src2, Src1))
        {
            Context->Registers.RegFlags |= NDR_RFLAG_CF;
        }
        else
        {
            Context->Registers.RegFlags &= ~NDR_RFLAG_CF;
        }

        if (Src2 == 1)
        {
            if (ND_GET_BIT(Size * 8ULL - 1, Src1) ^ ND_GET_BIT(Size * 8ULL - 2, Src1))
            {
                Context->Registers.RegFlags |= NDR_RFLAG_OF;
            }
            else
            {
                Context->Registers.RegFlags &= ~NDR_RFLAG_OF;
            }
        }
    }
    else if (FM_SHR == FlagsMode)
    {
        // CF is the last bit shifted out of the destination.
        if (ND_GET_BIT(Src2 - 1, Src1))
        {
            Context->Registers.RegFlags |= NDR_RFLAG_CF;
        }
        else
        {
            Context->Registers.RegFlags &= ~NDR_RFLAG_CF;
        }

        if (Src2 == 1)
        {
            if (ND_GET_BIT(Size * 8ULL - 1, Src1))
            {
                Context->Registers.RegFlags |= NDR_RFLAG_OF;
            }
            else
            {
                Context->Registers.RegFlags &= ~NDR_RFLAG_OF;
            }
        }
    }
    else if (FM_SAR == FlagsMode)
    {
        // CF is the last bit shifted out of the destination. In case of SAR, if the shift ammount exceeds the operand
        // size, CF will be 1 if the result is -1, or 0 if the result is 0.
        if (ND_GET_BIT(Src2 - 1, Src1) || ((Src2 >= (ND_UINT64)Size * 8) && Dst != 0))
        {
            Context->Registers.RegFlags |= NDR_RFLAG_CF;
        }
        else
        {
            Context->Registers.RegFlags &= ~NDR_RFLAG_CF;
        }

        Context->Registers.RegFlags &= ~NDR_RFLAG_OF;
    }
    else
    {
        // Set CF.
        if ((FM_SUB == FlagsMode) && ((Src1 < Src2) || (Src1 == Src2 && Dst != 0)))
        {
            Context->Registers.RegFlags |= NDR_RFLAG_CF;
        }
        else if ((FM_ADD == FlagsMode) && ((Dst < Src1) || (Dst == Src1 && Src2 != 0)))
        {
            Context->Registers.RegFlags |= NDR_RFLAG_CF;
        }
        else
        {
            Context->Registers.RegFlags &= ~NDR_RFLAG_CF;
        }

        // Set OF.
        if (FM_SUB == FlagsMode)
        {
            if ((ND_GET_SIGN(Size, Src1) && !ND_GET_SIGN(Size, Src2) && !ND_GET_SIGN(Size, Dst)) ||
                (!ND_GET_SIGN(Size, Src1) && ND_GET_SIGN(Size, Src2) && ND_GET_SIGN(Size, Dst)))
            {
                Context->Registers.RegFlags |= NDR_RFLAG_OF;
            }
            else
            {
                Context->Registers.RegFlags &= ~NDR_RFLAG_OF;
            }
        }
        else if (FM_ADD == FlagsMode)
        {
            if (ND_GET_SIGN(Size, Src1) == ND_GET_SIGN(Size, Src2) && 
                ND_GET_SIGN(Size, Src1) != ND_GET_SIGN(Size, Dst))
            {
                Context->Registers.RegFlags |= NDR_RFLAG_OF;
            }
            else
            {
                Context->Registers.RegFlags &= ~NDR_RFLAG_OF;
            }
        }
    }
}


//
// ShemuEvalCondition
//
static ND_BOOL
ShemuEvalCondition(
    SHEMU_CONTEXT *Context,
    ND_UINT8 ConditionCode
    )
{
    switch (ConditionCode)
    {
    case ND_COND_OVERFLOW: // O
        if (GET_FLAG(Context, NDR_RFLAG_OF) == 1)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_NOT(ND_COND_OVERFLOW): // NO
        if (GET_FLAG(Context, NDR_RFLAG_OF) == 0)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_CARRY: // C/B/NAE
        if (GET_FLAG(Context, NDR_RFLAG_CF) == 1)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_NOT(ND_COND_CARRY): // NC/NB/AE
        if (GET_FLAG(Context, NDR_RFLAG_CF) == 0)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_ZERO: // E/Z
        if (GET_FLAG(Context, NDR_RFLAG_ZF) == 1)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_NOT(ND_COND_ZERO): // NE/NZ
        if (GET_FLAG(Context, NDR_RFLAG_ZF) == 0)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_BELOW_OR_EQUAL: // BE/NA
        if ((GET_FLAG(Context, NDR_RFLAG_CF) | (GET_FLAG(Context, NDR_RFLAG_ZF))) == 1)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_NOT(ND_COND_BELOW_OR_EQUAL): // A/NBE
        if ((GET_FLAG(Context, NDR_RFLAG_CF) | (GET_FLAG(Context, NDR_RFLAG_ZF))) == 0)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_SIGN: // S
        if (GET_FLAG(Context, NDR_RFLAG_SF) == 1)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_NOT(ND_COND_SIGN): // NS
        if (GET_FLAG(Context, NDR_RFLAG_SF) == 0)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_PARITY: // P
        if (GET_FLAG(Context, NDR_RFLAG_PF) == 1)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_NOT(ND_COND_PARITY): // NP
        if (GET_FLAG(Context, NDR_RFLAG_PF) == 0)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_LESS: // L/NGE
        if ((GET_FLAG(Context, NDR_RFLAG_SF) ^ GET_FLAG(Context, NDR_RFLAG_OF)) == 1)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_NOT(ND_COND_LESS): // NL/GE
        if ((GET_FLAG(Context, NDR_RFLAG_SF) ^ GET_FLAG(Context, NDR_RFLAG_OF)) == 0)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_LESS_OR_EQUAL: // LE/NG
        if (((GET_FLAG(Context, NDR_RFLAG_SF) ^ GET_FLAG(Context, NDR_RFLAG_OF)) |
            (GET_FLAG(Context, NDR_RFLAG_ZF))) == 1)
        {
            return ND_TRUE;
        }
        break;
    case ND_COND_NOT(ND_COND_LESS_OR_EQUAL): // NLE/G
        if (((GET_FLAG(Context, NDR_RFLAG_SF) ^ GET_FLAG(Context, NDR_RFLAG_OF)) |
            (GET_FLAG(Context, NDR_RFLAG_ZF))) == 0)
        {
            return ND_TRUE;
        }
        break;
    }

    return ND_FALSE;

}


//
// ShemuIsShellcodePtr
//
inline static ND_BOOL
ShemuIsShellcodePtr(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT32 Size
    )
{
    return (Gla >= Context->ShellcodeBase && Gla < Context->ShellcodeBase + Context->ShellcodeSize &&
            Gla + Size > Context->ShellcodeBase && Gla + Size <= Context->ShellcodeBase + Context->ShellcodeSize);
}


//
// ShemuIsStackPtr
//
inline static ND_BOOL
ShemuIsStackPtr(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT32 Size
)
{
    return (Gla >= Context->StackBase && Gla < Context->StackBase + Context->StackSize &&
            Gla + Size > Context->StackBase && Gla + Size <= Context->StackBase + Context->StackSize);
}


//
// ShemuGetGprValue
//
static ND_UINT64
ShemuGetGprValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg,
    ND_UINT32 Size,
    ND_BOOL High8
    )
{
    switch (Size)
    {
    case 1:
        if (High8)
        {
            // AH, CH, DH or BH accessed.
            return (*(&Context->Registers.RegRax + Reg - 4) >> 8) & 0xff;
        }
        else
        {
            return *(&Context->Registers.RegRax + Reg) & 0xff;
        }

    case 2:
        return *(&Context->Registers.RegRax + Reg) & 0xffff;

    case 4:
        return *(&Context->Registers.RegRax + Reg) & 0xffffffff;

    default:
        return *(&Context->Registers.RegRax + Reg);
    }
}


//
// ShemuGetGprValue
//
static void
ShemuSetGprValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg,
    ND_UINT32 Size,
    ND_UINT64 Value,
    ND_BOOL High8
    )
{
    ND_UINT32 bit;

    switch (Size)
    {
    case 1:
        if (High8)
        {
            // AH, CH, DH or BH accessed.
            *((ND_UINT8 *)(&Context->Registers.RegRax + Reg - 4) + 1) = Value & 0xFF;
        }
        else
        {
            *((ND_UINT8 *)(&Context->Registers.RegRax + Reg)) = Value & 0xff;
        }
        break;

    case 2:
        *((ND_UINT16 *)(&Context->Registers.RegRax + Reg)) = Value & 0xffff;
        break;

    case 4:
        // Higher ND_UINT32 is always set to zero.
        *(&Context->Registers.RegRax + Reg) = Value & 0xffffffff;
        break;

    default:
        *(&Context->Registers.RegRax + Reg) = Value;
        break;
    }

    if (High8)
    {
        bit = Reg - 4;
    }
    else
    {
        bit = Reg;
    }

    // Mark the GPR as being dirty/written.
    Context->DirtyGprBitmap |= (1 << bit);
}


//
// ShemuCmpGprValue
//
static ND_BOOL
ShemuCmpGprValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg,
    ND_UINT32 Size,
    ND_UINT64 Value,
    ND_BOOL High8
    )
{
    switch (Size)
    {
    case 1:
        if (High8)
        {
            // AH, CH, DH or BH.
            return *((ND_UINT8 *)(&Context->Registers.RegRax + Reg - 4) + 1) == (Value & 0xff);
        }
        else
        {
            return *((ND_UINT8 *)(&Context->Registers.RegRax + Reg)) == (Value & 0xff);
        }

    case 2:
        return *((ND_UINT16 *)(&Context->Registers.RegRax + Reg)) == (Value & 0xffff);

    case 4:
        return *((ND_UINT32 *)(&Context->Registers.RegRax + Reg)) == (Value & 0xffffffff);

    default:
        return *(&Context->Registers.RegRax + Reg) == Value;
    }
}


//
// ShemuGetSegValue
//
static ND_UINT64
ShemuGetSegValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg
    )
{
    switch (Reg)
    {
    case NDR_ES:
        return Context->Segments.Es.Selector;
    case NDR_CS:
        return Context->Segments.Cs.Selector;
    case NDR_SS:
        return Context->Segments.Ss.Selector;
    case NDR_DS:
        return Context->Segments.Ds.Selector;
    case NDR_FS:
        return Context->Segments.Fs.Selector;
    case NDR_GS:
        return Context->Segments.Gs.Selector;
    }

    return 0;
}


//
// ShemuSetSegValue
//
static void
ShemuSetSegValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg,
    ND_UINT16 Value
    )
{
    switch (Reg)
    {
    case NDR_ES:
        Context->Segments.Es.Selector = Value;
        break;
    case NDR_CS:
        Context->Segments.Cs.Selector = Value;
        break;
    case NDR_SS:
        Context->Segments.Ss.Selector = Value;
        break;
    case NDR_DS:
        Context->Segments.Ds.Selector = Value;
        break;
    case NDR_FS:
        Context->Segments.Fs.Selector = Value;
        break;
    case NDR_GS:
        Context->Segments.Gs.Selector = Value;
        break;
    }
}


//
// ShemuGetSegBase
//
static ND_UINT64
ShemuGetSegBase(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg
    )
{
    switch (Reg)
    {
    case NDR_ES:
        return Context->Segments.Es.Base;
    case NDR_CS:
        return Context->Segments.Cs.Base;
    case NDR_SS:
        return Context->Segments.Ss.Base;
    case NDR_DS:
        return Context->Segments.Ds.Base;
    case NDR_FS:
        return Context->Segments.Fs.Base;
    case NDR_GS:
        return Context->Segments.Gs.Base;
    }

    return 0;
}


//
// ShemuComputeLinearAddress
//
static ND_UINT64
ShemuComputeLinearAddress(
    SHEMU_CONTEXT *Context,
    PND_OPERAND Operand
    )
{
    ND_UINT64 gla = 0;

    if (Operand->Info.Memory.HasBase)
    {
        gla += ShemuGetGprValue(Context, Operand->Info.Memory.Base, Operand->Info.Memory.BaseSize, ND_FALSE);
    }

    if (Operand->Info.Memory.HasIndex)
    {
        gla += ShemuGetGprValue(Context, Operand->Info.Memory.Index, Operand->Info.Memory.IndexSize, ND_FALSE) *
                                Operand->Info.Memory.Scale;
    }

    // Note that this also handles the case where moffset (absolute addressing) is used, as HasDisp will be set when
    // IsDirect is also set.
    if (Operand->Info.Memory.HasDisp)
    {
        gla += Operand->Info.Memory.Disp;
    }

    if (Operand->Info.Memory.IsRipRel)
    {
        gla += Context->Registers.RegRip;
    }

    // Special handling for BT, BTR, BTS, BTC instructions with bitbase addressing.
    if (Operand->Info.Memory.IsBitbase)
    {
        ND_UINT64 bitbase, op1size, op2size, reg;

        op1size = Context->Instruction.Operands[0].Size;
        op2size = Context->Instruction.Operands[1].Size;

        reg = ((ND_UINT64*)&Context->Registers.RegRax)[Context->Instruction.Operands[1].Info.Register.Reg];

        // Note: only BT* with register source (NOT immediate) support bitbase addressing.
        bitbase = ND_SIGN_EX(op2size, reg);

        if (bitbase & (1ULL << 63))
        {
            gla -= ((~bitbase >> 3) & ~(op1size - 1)) + op1size;
        }
        else
        {
            gla += (bitbase >> 3) & ~(op1size - 1);
        }
    }

    // Special handling for stack operations: if we have a PUSH, we have to subtract the accessed size, as, in fact,
    // [RSP - size] is accessed, not [RSP].
    if (Operand->Info.Memory.IsStack)
    {
        if (Operand->Access.Write || Operand->Access.CondWrite)
        {
            gla -= Operand->Size;
        }
    }

    // Make sure we truncate the linear address to the address size.
    switch (Context->Instruction.AddrMode)
    {
    case ND_ADDR_32:
        gla &= 0xFFFFFFFF;
        break;
    case ND_ADDR_16:
        gla &= 0xFFFF;
    default:
        break;
    }

    // Memory operands usually have a segment. Note that we don't care about any segment checks, since we're most
    // likely be provided with flat segments. If checks should be needed, dedicated callbacks should be added.
    if (Operand->Info.Memory.HasSeg)
    {
        gla += ShemuGetSegBase(Context, Operand->Info.Memory.Seg);

        if (Context->Mode != ND_CODE_64)
        {
            // Truncate to 32 bit outside 64 bit.
            gla &= 0xFFFFFFFF;
        }
    }

    return gla;
}


//
// ShemuGetMemValue
//
static SHEMU_STATUS
ShemuGetMemValue(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT32 Size,
    ND_UINT8 *Value
    )
{
    ND_UINT8 *addr;
    ND_UINT32 offset;

    if (ShemuIsShellcodePtr(Context, Gla, Size))
    {
        addr = Context->Shellcode;
        offset = (ND_UINT32)(Gla - Context->ShellcodeBase);
    }
    else if (ShemuIsStackPtr(Context, Gla, Size))
    {
        addr = Context->Stack;
        offset = (ND_UINT32)(Gla - Context->StackBase);
    }
    else
    {
        ND_BOOL res = ND_FALSE;

        // We allow a maximum number of external memory accesses, due to performance reasons.
        if (++Context->ExtMemAccess > Context->MemThreshold)
        {
            return SHEMU_ABORT_GLA_OUTSIDE;
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
            res = Context->AccessMemory(Context, Gla, Size, Value, ND_FALSE);
        }

        if (res)
        {
            return SHEMU_SUCCESS;
        }

        return SHEMU_ABORT_GLA_OUTSIDE;
    }

    switch (Size)
    {
    case 1:
        *Value = *(addr + offset);
        break;
    case 2:
        *(ND_UINT16 *)Value = *(ND_UINT16 *)(addr + offset);
        break;
    case 4:
        *(ND_UINT32 *)Value = *(ND_UINT32 *)(addr + offset);
        break;
    case 8:
        *(ND_UINT64 *)Value = *(ND_UINT64 *)(addr + offset);
        break;
    default:
        shemu_memcpy(Value, addr + offset, Size);
        break;
    }

    return SHEMU_SUCCESS;
}


//
// ShemuSetMemValue
//
static SHEMU_STATUS
ShemuSetMemValue(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Gla,
    ND_UINT32 Size,
    ND_UINT8 *Value
    )
{
    ND_UINT8 *addr;
    ND_UINT32 offset;

    if (ShemuIsShellcodePtr(Context, Gla, Size))
    {
        addr = Context->Shellcode;
        offset = (ND_UINT32)(Gla - Context->ShellcodeBase);

        // Bypass self-writes, if needed to.
        if (!!(Context->Options & SHEMU_OPT_BYPASS_SELF_WRITES))
        {
            return SHEMU_SUCCESS;
        }
    }
    else if (ShemuIsStackPtr(Context, Gla, Size))
    {
        addr = Context->Stack;
        offset = (ND_UINT32)(Gla - Context->StackBase);
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
            res = Context->AccessMemory(Context, Gla, Size, Value, ND_TRUE);
        }

        if (res)
        {
            return SHEMU_SUCCESS;
        }

        return SHEMU_ABORT_GLA_OUTSIDE;
    }

    switch (Size)
    {
    case 1:
        *(addr + offset) = *Value & 0xff;
        break;
    case 2:
        *(ND_UINT16 *)(addr + offset) = *(ND_UINT16 *)Value & 0xffff;
        break;
    case 4:
        *(ND_UINT32 *)(addr + offset) = *(ND_UINT32 *)Value & 0xffffffff;
        break;
    case 8:
        *(ND_UINT64 *)(addr + offset) = *(ND_UINT64 *)Value;
        break;
    default:
        shemu_memcpy(addr + offset, Value, Size);
        break;
    }

    return SHEMU_SUCCESS;
}


//
// ShemuGetOperandValue
//
static SHEMU_STATUS
ShemuGetOperandValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Operand,
    SHEMU_VALUE *Value
    )
{
    SHEMU_STATUS status;
    PND_OPERAND op = &Context->Instruction.Operands[Operand];

    Value->Size = op->Size;
    if (Value->Size > sizeof(Value->Value))
    {
        return SHEMU_ABORT_OP_TOO_LARGE;
    }

    if (op->Type == ND_OP_REG)
    {
        switch (op->Info.Register.Type)
        {
        case ND_REG_GPR:
            Value->Value.Qwords[0] = ShemuGetGprValue(Context, op->Info.Register.Reg, op->Size, 
                                                      op->Info.Register.IsHigh8);
            break;

        case ND_REG_SEG:
            Value->Value.Qwords[0] = ShemuGetSegValue(Context, op->Info.Register.Reg);
            break;

        case ND_REG_MMX:
            Value->Value.Qwords[0] = Context->MmxRegisters[op->Info.Register.Reg];
            break;

        case ND_REG_SSE:
            shemu_memcpy(Value->Value.Bytes,
                         &Context->SseRegisters[op->Info.Register.Reg], 
                         op->Size);
            break;

        case ND_REG_RIP:
            Value->Value.Qwords[0] = ND_TRIM(op->Size, Context->Registers.RegRip);
            break;

        case ND_REG_FLG:
            Value->Value.Qwords[0] = ND_TRIM(op->Size, Context->Registers.RegFlags);
            break;

        case ND_REG_CR:
            switch (op->Info.Register.Reg)
            {
            case NDR_CR0:
                Value->Value.Qwords[0] = Context->Registers.RegCr0;
                break;
            case NDR_CR2:
                Value->Value.Qwords[0] = Context->Registers.RegCr2;
                break;
            case NDR_CR3:
                Value->Value.Qwords[0] = Context->Registers.RegCr3;
                break;
            case NDR_CR4:
                Value->Value.Qwords[0] = Context->Registers.RegCr4;
                break;
            case NDR_CR8:
                Value->Value.Qwords[0] = Context->Registers.RegCr8;
                break;
            default:
                return SHEMU_ABORT_REGISTER_NOT_SUPPORTED;
            }
            break;
            

        default:
            return SHEMU_ABORT_REGISTER_NOT_SUPPORTED;
        }
    }
    else if (op->Type == ND_OP_MEM)
    {
        ND_UINT64 gla = ShemuComputeLinearAddress(Context, op);
        ND_UINT32 offset;
        ND_UINT8 seg;

        if (op->Info.Memory.IsAG)
        {
            // Address generation instruction, the result is the linear address itself.
            Value->Value.Qwords[0] = gla;
            goto done_gla;
        }

        if (Context->Ring == 3)
        {
            // User-mode TIB offset that contains the PEB address.
            offset = Context->Mode == ND_CODE_32 ? 0x30 : 0x60;
            seg = Context->Mode == ND_CODE_32 ? ND_PREFIX_G2_SEG_FS : ND_PREFIX_G2_SEG_GS;
        }
        else
        {
            // Kernel-mode KPCR offset that contains the current KTHREAD address.
            offset = Context->Mode == ND_CODE_32 ? 0x124 : 0x188;
            seg = Context->Mode == ND_CODE_32 ? ND_PREFIX_G2_SEG_FS : ND_PREFIX_G2_SEG_GS;
        }

        // Check if this is a TIB/PCR access. Make sure the FS/GS register is used for the access, in order to avoid
        // ND_FALSE positives where legitimate code accesses a linear TIB directly.
        // Note that this covers accesses to the PEB field inside the TIB.
        if (gla == Context->TibBase + offset && Context->Instruction.Seg == seg)
        {
            Context->Flags |= SHEMU_FLAG_TIB_ACCESS;
        }

        // Note that this covers accesses to the Wow32Reserved in Wow64 mode. That field can be used to issue
        // syscalls.
        if (gla == Context->TibBase + 0xC0 && Context->Instruction.Seg == seg && Context->Mode == ND_CODE_32)
        {
            Context->Flags |= SHEMU_FLAG_TIB_ACCESS_WOW32;
        }

        // Check for accesses inside the KUSER_SHARED_DATA (SharedUserData). This page contains some
        // global system information, it may host shellcodes, and is hard-coded at this address.
        if (gla >= 0x7FFE0000 && gla < 0x7FFE1000)
        {
            Context->Flags |= SHEMU_FLAG_SUD_ACCESS;
        }

        // Check if we are reading a previously saved RIP. Ignore RET category, which naturally uses the saved RIP.
        // Also, ignore RMW instruction which naturally read the current value - this could happen if the code
        // modifies the return value, for example "ADD qword [rsp], r8".
        if (Context->Instruction.Category != ND_CAT_RET && !(op->Access.Access & ND_ACCESS_ANY_WRITE) &&
            ShemuIsStackPtr(Context, gla, op->Size) &&
            ShemuAnyBitsSet(STACKBMP(Context), gla - Context->StackBase, op->Size))
        {
            Context->Flags |= SHEMU_FLAG_LOAD_RIP;
        }

        // Get the memory value.
        status = ShemuGetMemValue(Context, gla, MIN(op->Size, Value->Size), Value->Value.Bytes);
        if (SHEMU_SUCCESS != status)
        {
            return status;
        }

        // If this is a stack access, we need to update the stack pointer.
        if (op->Info.Memory.IsStack)
        {
            ND_UINT64 regval = ShemuGetGprValue(Context, NDR_RSP, (2 << Context->Instruction.DefStack), ND_FALSE);

            regval += op->Size;

            ShemuSetGprValue(Context, NDR_RSP, (2 << Context->Instruction.DefStack), regval, ND_FALSE);
        }

        // If this is a string operation, make sure we update RSI/RDI.
        if (op->Info.Memory.IsString)
        {
            ND_UINT64 regval = ShemuGetGprValue(Context, op->Info.Memory.Base, op->Info.Memory.BaseSize, ND_FALSE);

            regval = GET_FLAG(Context, NDR_RFLAG_DF) ? regval - op->Size : regval + op->Size;

            ShemuSetGprValue(Context, op->Info.Memory.Base, op->Info.Memory.BaseSize, regval, ND_FALSE);
        }

done_gla:;
    }
    else if (op->Type == ND_OP_IMM)
    {
        Value->Value.Qwords[0] = op->Info.Immediate.Imm;
    }
    else if (op->Type == ND_OP_CONST)
    {
        Value->Value.Qwords[0] = op->Info.Constant.Const;
    }
    else if (op->Type == ND_OP_OFFS)
    {
        Value->Value.Qwords[0] = op->Info.RelativeOffset.Rel;
    }
    else
    {
        return SHEMU_ABORT_UNSUPPORTED_INSTRUX;
    }

    return SHEMU_SUCCESS;
}


//
// ShemuSetOperandValue
//
static SHEMU_STATUS
ShemuSetOperandValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Operand,
    SHEMU_VALUE *Value
    )
{
    SHEMU_STATUS status;
    PND_OPERAND op = &Context->Instruction.Operands[Operand];

    if (op->Type == ND_OP_REG)
    {
        switch (op->Info.Register.Type)
        {
        case ND_REG_GPR:
            if (Context->Instruction.Instruction == ND_INS_XCHG &&
                op->Info.Register.Reg == NDR_RSP)
            {
                Context->Flags |= SHEMU_FLAG_STACK_PIVOT;
            }

            ShemuSetGprValue(Context, op->Info.Register.Reg, op->Size, Value->Value.Qwords[0], 
                             op->Info.Register.IsHigh8);
            break;

        case ND_REG_SEG:
            ShemuSetSegValue(Context, op->Info.Register.Reg, Value->Value.Words[0]);
            break;

        case ND_REG_MMX:
            Context->MmxRegisters[op->Info.Register.Reg] = Value->Value.Qwords[0];
            // Only log these when they're written.
            if (Context->Options & SHEMU_OPT_TRACE_EMULATION)
            {
                shemu_printf(Context, "        MM%d = 0x%016llx\n", op->Info.Register.Reg, Value->Value.Qwords[0]);
            }
            break;

        case ND_REG_SSE:
            if (Context->Instruction.EncMode != ND_ENCM_LEGACY)
            {
                // Zero the entire register first, if we have a VEX/EVEX encoded instruction.
                nd_memzero(&Context->SseRegisters[op->Info.Register.Reg], ND_MAX_REGISTER_SIZE);
            }
            else
            {
                // Zero upper bits in the 128 bits register, if operand size is less than 16 bytes.
                // Upper bits in the YMM/ZMM register are preserved.
                nd_memzero(&Context->SseRegisters[op->Info.Register.Reg], 16);
            }
            // Copy the value.
            shemu_memcpy(&Context->SseRegisters[op->Info.Register.Reg],
                         Value->Value.Bytes, 
                         op->Size);
            // Only log these when they're written.
            if (Context->Options & SHEMU_OPT_TRACE_EMULATION)
            {
                shemu_printf(Context,
                             "        %cMM%d (HI_32) = 0x%016llx%016llx%016llx%016llx\n",
                             op->Size == 16 ? 'X' : op->Size == 32 ? 'Y' : 'Z', op->Info.Register.Reg,
                             Value->Value.Qwords[7], Value->Value.Qwords[6], 
                             Value->Value.Qwords[5], Value->Value.Qwords[4]);
                shemu_printf(Context,
                             "        %cMM%d (LO_32) = 0x%016llx%016llx%016llx%016llx\n",
                             op->Size == 16 ? 'X' : op->Size == 32 ? 'Y' : 'Z', op->Info.Register.Reg,
                             Value->Value.Qwords[3], Value->Value.Qwords[2], 
                             Value->Value.Qwords[1], Value->Value.Qwords[0]);
            }
            break;

        case ND_REG_RIP:
            Context->Registers.RegRip = ND_TRIM(op->Size, Value->Value.Qwords[0]);
            break;

        case ND_REG_FLG:
            if (op->Size == 2)
            {
                *((ND_UINT16*)&Context->Registers.RegFlags) = Value->Value.Words[0];
            }
            else
            {
                Context->Registers.RegFlags = Value->Value.Qwords[0];
            }
            // Handle reserved bits.
            Context->Registers.RegFlags |= (1ULL << 1);
            Context->Registers.RegFlags &= ~((1ULL << 3) | (1ULL << 5) | (1ULL << 15));
            Context->Registers.RegFlags &= 0x3FFFFF;
            break;

        case ND_REG_CR:
            switch (op->Info.Register.Reg)
            {
            case NDR_CR0:
                Context->Registers.RegCr0 = Value->Value.Qwords[0];
                break;
            case NDR_CR2:
                Context->Registers.RegCr2 = Value->Value.Qwords[0];
                break;
            case NDR_CR3:
                Context->Registers.RegCr3 = Value->Value.Qwords[0];
                break;
            case NDR_CR4:
                Context->Registers.RegCr4 = Value->Value.Qwords[0];
                break;
            case NDR_CR8:
                Context->Registers.RegCr8 = Value->Value.Qwords[0];
                break;
            default:
                return SHEMU_ABORT_REGISTER_NOT_SUPPORTED;
            }
            break;

        default:
            return SHEMU_ABORT_REGISTER_NOT_SUPPORTED;
        }
    }
    else if (op->Type == ND_OP_MEM)
    {
        // Compute the GLA.
        ND_UINT64 gla = ShemuComputeLinearAddress(Context, op);

        // Handle self-write. We store a 1 for each written byte inside the shellcode space. Once the modified bytes
        // are executed, we can trigger the self-write detection.
        if (ShemuIsShellcodePtr(Context, gla, op->Size))
        {
            ShemuSetBits(SHELLBMP(Context), gla - Context->ShellcodeBase, op->Size, 1);
        }

        // Handle RIP save on the stack.
        if (ShemuIsStackPtr(Context, gla, MAX(op->Size, Context->Instruction.WordLength)))
        {
            ND_UINT8 stckstrlen = 0;
            ND_UINT32 i;

            // Note: only Context->Instruction.WordLength bits are flagged as RIP, as that is the RIP size.
            if (Context->Instruction.Instruction == ND_INS_CALLNR ||
                Context->Instruction.Instruction == ND_INS_CALLNI)
            {
                ShemuSetBits(STACKBMP(Context), gla - Context->StackBase, Context->Instruction.WordLength, 1);
            }
            else if (Context->Instruction.Instruction == ND_INS_FNSTENV)
            {
                // OK: op->Size will be the FPU state size.
                ShemuSetBits(STACKBMP(Context), (gla + 0xC) - Context->StackBase, Context->Instruction.WordLength, 1);
            }
            else if (Context->Instruction.Instruction == ND_INS_FXSAVE || 
                Context->Instruction.Instruction == ND_INS_FXSAVE64)
            {
                // OK: op->Size will be the FXSAVE size.
                ShemuSetBits(STACKBMP(Context), (gla + 0x8) - Context->StackBase, Context->Instruction.WordLength, 1);
            }
            else
            {
                // Something is written on a previously saved RIP; reset it.
                ShemuSetBits(STACKBMP(Context), gla - Context->StackBase, op->Size, 0);
            }

            // Check if a string is being saved on the stack. Typically used by shellcodes like this:
            // PUSH str0
            // PUSH str1
            // ...
            // PUSH strn
            // Other variants may exist, but all we care about are stores on the stack, and all are checked.
            // Note that we will ignore registers which have not been modified during emulation; those are considered
            // input values for the emulated code, and may be pointers or other data. We are interested only in
            // stack values built within the emulate code.
            for (i = 0; i < Value->Size; i++)
            {
                unsigned char c = Value->Value.Bytes[i];

                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
                    c == '\\' || c == '/' || c == ':' || c == ' ')
                {
                    stckstrlen++;
                }
                else
                {
                    break;
                }
            }

            if (stckstrlen == Value->Size)
            {
                // Make sure the value is not present inside a non-dirty GPR. 
                for (i = 0; i < 16; i++)
                {
                    if (ShemuCmpGprValue(Context, i, Value->Size, Value->Value.Qwords[0], ND_FALSE) &&
                        (0 == (Context->DirtyGprBitmap & (1 << i))))
                    {
                        // A register is saved on the stack, but that register wasn't written during the emulation.
                        stckstrlen = 0;
                        break;
                    }
                }
            }

            Context->StrLength += stckstrlen;

            if (Context->StrLength >= Context->StrThreshold)
            {
                Context->Flags |= SHEMU_FLAG_STACK_STR;
            }
            
            if (stckstrlen != Value->Size)
            {
                // Not a full string stored on the stack, reset the counter.
                Context->StrLength = 0;
            }
        }

        // Set the value.
        status = ShemuSetMemValue(Context, gla, MIN(op->Size, Value->Size), Value->Value.Bytes);
        if (SHEMU_SUCCESS != status)
        {
            return status;
        }

        // If this is a stack access, we need to update the stack pointer.
        if (op->Info.Memory.IsStack)
        {
            ND_UINT64 regval = ShemuGetGprValue(Context, NDR_RSP, (2 << Context->Instruction.DefStack), ND_FALSE);

            regval -= op->Size;

            ShemuSetGprValue(Context, NDR_RSP, (2 << Context->Instruction.DefStack), regval, ND_FALSE);
        }

        // If this is a string operation, make sure we update RSI/RDI.
        if (op->Info.Memory.IsString)
        {
            ND_UINT64 regval = ShemuGetGprValue(Context, op->Info.Memory.Base, op->Info.Memory.BaseSize, ND_FALSE);

            regval = GET_FLAG(Context, NDR_RFLAG_DF) ? regval - op->Size : regval + op->Size;

            ShemuSetGprValue(Context, op->Info.Memory.Base, op->Info.Memory.BaseSize, regval, ND_FALSE);
        }
    }
    else
    {
        return SHEMU_ABORT_INVALID_INSTRUX;
    }

    return SHEMU_SUCCESS;
}


//
// ShemuMultiply64Unsigned
//
static void
ShemuMultiply64Unsigned(
    ND_UINT64 Operand1,
    ND_UINT64 Operand2,
    ND_UINT64 *ResHigh,
    ND_UINT64 *ResLow
    )
{
    ND_UINT64 xLow = (ND_UINT64)(ND_UINT32)Operand1;
    ND_UINT64 xHigh = Operand1 >> 32;
    ND_UINT64 yLow = (ND_UINT64)(ND_UINT32)Operand2;
    ND_UINT64 yHigh = Operand2 >> 32;

    ND_UINT64 p0 = xLow * yLow;
    ND_UINT64 p1 = xLow * yHigh;
    ND_UINT64 p2 = xHigh * yLow;
    ND_UINT64 p3 = xHigh * yHigh;

    ND_UINT32 cy = (ND_UINT32)(((p0 >> 32) + (ND_UINT32)p1 + (ND_UINT32)p2) >> 32);

    *ResLow = p0 + (p1 << 32) + (p2 << 32);
    *ResHigh = p3 + (p1 >> 32) + (p2 >> 32) + cy;
}


//
// ShemuMultiply64Signed
//
static void
ShemuMultiply64Signed(
    ND_SINT64 Operand1,
    ND_SINT64 Operand2,
    ND_SINT64 *ResHigh,
    ND_SINT64 *ResLow
    )
{
    ShemuMultiply64Unsigned((ND_UINT64)Operand1, (ND_UINT64)Operand2, (ND_UINT64 *)ResHigh, (ND_UINT64 *)ResLow);
    if (Operand1 < 0LL) *ResHigh -= Operand2;
    if (Operand2 < 0LL) *ResHigh -= Operand1;
}


//
// ShemuCheckDiv
//
static ND_BOOL
ShemuCheckDiv(
    ND_UINT64 Divident,
    ND_UINT64 Divider,
    ND_UINT8 Size            // The size of the Source (Divider). The Divident is twice as large.
    )
{
    // Returns ND_TRUE if all checks are OK, and Divident / Divider will not cause #DE.

    if (Divider == 0)
    {
        // Division by zero.
        return ND_FALSE;
    }

    // If the result won't fit in the destination, a #DE would be generated.
    switch (Size)
    {
    case 1:
        if (((Divident >> 8) & 0xFF) >= Divider)
        {
            return ND_FALSE;
        }
        break;

    case 2:
        if (((Divident >> 16) & 0xFFFF) >= Divider)
        {
            return ND_FALSE;
        }
        break;

    case 4:
        if (((Divident >> 32) & 0xFFFFFFFF) >= Divider)
        {
            return ND_FALSE;
        }
        break;

    default:
        // 64 bit source division is not supported.
        return ND_FALSE;
    }

    return ND_TRUE;
}


//
// ShemuCheckIdiv
//
static ND_BOOL
ShemuCheckIdiv(
    ND_SINT64 Divident,
    ND_SINT64 Divider,
    ND_UINT8 Size            // The size of the Source (Divider).
    )
{
    ND_BOOL neg1, neg2;
    ND_UINT64 quotient, max;

    neg1 = Divident < 0;
    neg2 = Divider < 0;

    if (neg1)
    {
        Divident = -Divident;
    }

    if (neg2)
    {
        Divider = -Divider;
    }

    // Do checks when dividing positive values.
    if (!ShemuCheckDiv(Divident, Divider, Size))
    {
        return ND_FALSE;
    }

    // Get the positive quotient.
    quotient = (ND_UINT64)Divident / (ND_UINT64)Divider;

    max = (Size == 1) ? 0x80 : (Size == 2) ? 0x8000 : (Size == 4) ? 0x80000000 : 0x8000000000000000;

    if (neg1 ^ neg2)
    {
        // The Divident and the Divider have different signs, the quotient must be negative. If it's positive => #DE.
        if (ND_GET_SIGN(Size, quotient) && quotient != max)
        {
            return ND_FALSE;
        }
    }
    else
    {
        // Both the Divident and the Divider are positive/negative, so a positive result must be produced. If it's 
        // negative => #DE.
        if (ND_GET_SIGN(Size, quotient))
        {
            return ND_FALSE;
        }
    }

    return ND_TRUE;
}


//
// ShemuPrintContext
//
#ifndef BDDISASM_NO_FORMAT
static void
ShemuPrintContext(
    SHEMU_CONTEXT *Context
    )
{
    char text[ND_MIN_BUF_SIZE] = { 0 };

    NdToText(&Context->Instruction, Context->Registers.RegRip, ND_MIN_BUF_SIZE, text);

    shemu_printf(Context, "        RAX = 0x%016llx RCX = 0x%016llx RDX = 0x%016llx RBX = 0x%016llx\n",
        Context->Registers.RegRax, Context->Registers.RegRcx, Context->Registers.RegRdx, Context->Registers.RegRbx);
    shemu_printf(Context, "        RSP = 0x%016llx RBP = 0x%016llx RSI = 0x%016llx RDI = 0x%016llx\n",
        Context->Registers.RegRsp, Context->Registers.RegRbp, Context->Registers.RegRsi, Context->Registers.RegRdi);
    shemu_printf(Context, "        R8  = 0x%016llx R9  = 0x%016llx R10 = 0x%016llx R11 = 0x%016llx\n",
        Context->Registers.RegR8, Context->Registers.RegR9, Context->Registers.RegR10, Context->Registers.RegR11);
    shemu_printf(Context, "        R12 = 0x%016llx R13 = 0x%016llx R14 = 0x%016llx R15 = 0x%016llx\n",
        Context->Registers.RegR12, Context->Registers.RegR13, Context->Registers.RegR14, Context->Registers.RegR15);
    shemu_printf(Context, "        RIP = 0x%016llx RFLAGS = 0x%016llx ", 
        Context->Registers.RegRip, Context->Registers.RegFlags);
    shemu_printf(Context, "  CF:%d PF:%d AF:%d ZF:%d SF:%d TF:%d IF:%d DF:%d OF:%d\n",
        GET_FLAG(Context, NDR_RFLAG_CF),
        GET_FLAG(Context, NDR_RFLAG_PF),
        GET_FLAG(Context, NDR_RFLAG_AF),
        GET_FLAG(Context, NDR_RFLAG_ZF),
        GET_FLAG(Context, NDR_RFLAG_SF),
        GET_FLAG(Context, NDR_RFLAG_TF),
        GET_FLAG(Context, NDR_RFLAG_IF),
        GET_FLAG(Context, NDR_RFLAG_DF),
        GET_FLAG(Context, NDR_RFLAG_OF));

    shemu_printf(Context, "Emulating: 0x%016llx %s\n", Context->Registers.RegRip, text);
}
#else
#define ShemuPrintContext(Context)
#endif // !BDDISASM_NO_FORMAT 


//
// ShemuEmulate
//
SHEMU_STATUS
ShemuEmulate(
    SHEMU_CONTEXT *Context
    )
{
    SHEMU_VALUE res = { 0 }, dst = { 0 }, src = { 0 }, rcx = { 0 }, aux = { 0 };
    ND_BOOL stop = ND_FALSE, cf;
    ND_UINT16 cs = 0;
    ND_UINT64 tsc = 0x1248fe7a5c30;

    if (ND_NULL == Context)
    {
        return SHEMU_ABORT_INVALID_PARAMETER;
    }

    if (ND_NULL == Context->Shellcode)
    {
        return SHEMU_ABORT_INVALID_PARAMETER;
    }

    if (ND_NULL == Context->Stack)
    {
        return SHEMU_ABORT_INVALID_PARAMETER;
    }

    if (ND_NULL == Context->Intbuf)
    {
        return SHEMU_ABORT_INVALID_PARAMETER;
    }

    if (SHEMU_INTERNAL_BUFFER_SIZE(Context) > Context->IntbufSize)
    {
        return SHEMU_ABORT_INVALID_PARAMETER;
    }

    if (Context->NopThreshold == 0)
    {
        Context->NopThreshold = SHEMU_DEFAULT_NOP_THRESHOLD;
    }

    if (Context->StrThreshold == 0)
    {
        Context->StrThreshold = SHEMU_DEFAULT_STR_THRESHOLD;
    }

    while (Context->InstructionsCount++ < Context->MaxInstructionsCount)
    {
        NDSTATUS ndstatus;
        ND_UINT64 rip;
        ND_UINT32 i;

        tsc++;

        // Reset all the operands to 0.
        nd_memzero(&dst, sizeof(dst));
        nd_memzero(&src, sizeof(src));
        nd_memzero(&res, sizeof(res));
        nd_memzero(&aux, sizeof(aux));
        nd_memzero(&rcx, sizeof(rcx));

        // The stop flag has been set, this means we've reached a valid instruction, but that instruction cannot be
        // emulated (for example, SYSCALL, INT, system instructions, etc).
        if (stop)
        {
            return SHEMU_ABORT_CANT_EMULATE;
        }

        // If we already have a detection and we wish to stop on detections, do so now.
        if ((0 != Context->Flags) && (0 != (Context->Options & SHEMU_OPT_STOP_ON_EXPLOIT)))
        {
            return SHEMU_ABORT_SHELLCODE_DETECTED;
        }

        // Make sure the RIP is pointing in the right area. We test only 1 byte - the decoder will make sure it can
        // access as many bytes as needed and return error in case it can't.
        if (!ShemuIsShellcodePtr(Context, Context->Registers.RegRip, 1))
        {
            return SHEMU_ABORT_BRANCH_OUTSIDE;
        }

        // Get the offset inside the shellcode buffer.
        rip = Context->Registers.RegRip - Context->ShellcodeBase;

        // Decode the next instruction.
        ndstatus = NdDecodeEx(&Context->Instruction, Context->Shellcode + rip,
                              Context->ShellcodeSize - (ND_SIZET)rip, Context->Mode, Context->Mode);
        if (!ND_SUCCESS(ndstatus))
        {
            if (ND_STATUS_BUFFER_TOO_SMALL == ndstatus)
            {
                return SHEMU_ABORT_BRANCH_OUTSIDE;
            }
            else
            {
                return SHEMU_ABORT_INVALID_INSTRUX;
            }
        }

        // Paranoid check...
        if (!ShemuIsShellcodePtr(Context, Context->Registers.RegRip, Context->Instruction.Length))
        {
            return SHEMU_ABORT_BRANCH_OUTSIDE;
        }

        // Check if we just fetched an instruction from a previously written area, to raise self-write alert.
        if (ShemuAnyBitsSet(SHELLBMP(Context), rip, Context->Instruction.Length))
        {
            Context->Flags |= SHEMU_FLAG_WRITE_SELF;
        }

        // Dump the context.
        if (Context->Options & SHEMU_OPT_TRACE_EMULATION)
        {
            ShemuPrintContext(Context);
        }

        // The RIP is incremented BEFORE actually emulating the instruction. This is what the CPU does as well.
        Context->Registers.RegRip += Context->Instruction.Length;

        // FPU instructions are "pass-through", we just want to save the RIP, so we can emulate FNSTENV.
        if ((Context->Instruction.IsaSet == ND_SET_X87) && (Context->Instruction.Instruction != ND_INS_FNSTENV))
        {
            Context->Registers.FpuRip = Context->Registers.RegRip - Context->Instruction.Length;
            continue;
        }

        switch (Context->Instruction.Instruction)
        {
        case ND_INS_FNSTENV:
            src.Size = Context->Instruction.Operands[0].Size;
            src.Value.FpuEnvironment.FpuInstructionPointer = (ND_UINT32)Context->Registers.FpuRip;
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_FXSAVE:
        case ND_INS_FXSAVE64:
            src.Size = MIN(Context->Instruction.Operands[0].Size, sizeof(src.Value.XsaveArea));
            src.Value.XsaveArea.FpuRip = Context->Registers.FpuRip;
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_MOV_CR:
            if (Context->Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            // Fall through.

        case ND_INS_MOV:
        case ND_INS_MOVZX:
            GET_OP(Context, 1, &src);
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_MOVSX:
        case ND_INS_MOVSXD:
            GET_OP(Context, 1, &src);
            GET_OP(Context, 0, &dst);
            dst.Value.Qwords[0] = ND_SIGN_EX(src.Size, src.Value.Qwords[0]);
            SET_OP(Context, 0, &dst);
            break;

        case ND_INS_CMOVcc:
            GET_OP(Context, 0, &dst);
            GET_OP(Context, 1, &src);
            if (ShemuEvalCondition(Context, Context->Instruction.Condition))
            {
                SET_OP(Context, 0, &src);
            }
            else
            {
                // Write back the same value that was already present in destination. This has the side-effect of 
                // clearing the upper 32 bit in the 64 bit destination register while in long mode.
                SET_OP(Context, 0, &dst);
            }
            break;

        case ND_INS_SETcc:
            if (ShemuEvalCondition(Context, Context->Instruction.Condition))
            {
                src.Size = Context->Instruction.Operands[0].Size;
                src.Value.Qwords[0] = 1;
            }
            else
            {
                src.Size = Context->Instruction.Operands[0].Size;
                src.Value.Qwords[0] = 0;
            }
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_XLATB:
            GET_OP(Context, 1, &src);
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_XCHG:
            GET_OP(Context, 1, &src);
            GET_OP(Context, 0, &dst);
            SET_OP(Context, 1, &dst);
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_XADD:
            GET_OP(Context, 1, &src);
            GET_OP(Context, 0, &dst);
            res.Size = dst.Size;
            res.Value.Qwords[0] = dst.Value.Qwords[0] + src.Value.Qwords[0];
            SET_FLAGS(Context, res, dst, src, FM_ADD);
            SET_OP(Context, 1, &dst);
            SET_OP(Context, 0, &res);
            break;

        case ND_INS_CMPXCHG:
            GET_OP(Context, 2, &src);
            GET_OP(Context, 0, &dst);

            res.Size = src.Size;
            // Note: The accumulator is compared with the destination, not the other way around.
            res.Value.Qwords[0] = src.Value.Qwords[0] - dst.Value.Qwords[0];

            SET_FLAGS(Context, res, src, dst, FM_SUB);

            if (src.Value.Qwords[0] == dst.Value.Qwords[0])
            {
                GET_OP(Context, 1, &src);
                SET_OP(Context, 0, &src);
            }
            else
            {
                SET_OP(Context, 2, &dst);
            }
            break;

        case ND_INS_ADD:
        case ND_INS_ADC:
            GET_OP(Context, 0, &dst);
            GET_OP(Context, 1, &src);

            res.Size = src.Size;
            res.Value.Qwords[0] = dst.Value.Qwords[0] + src.Value.Qwords[0];

            if (ND_INS_ADC == Context->Instruction.Instruction)
            {
                res.Value.Qwords[0] += GET_FLAG(Context, NDR_RFLAG_CF);
            }

            SET_FLAGS(Context, res, dst, src, FM_ADD);
            SET_OP(Context, 0, &res);

            break;

        case ND_INS_SUB:
        case ND_INS_SBB:
        case ND_INS_CMP:
            GET_OP(Context, 0, &dst);
            GET_OP(Context, 1, &src);

            res.Size = src.Size;
            res.Value.Qwords[0] = dst.Value.Qwords[0] - src.Value.Qwords[0];

            if (ND_INS_SBB == Context->Instruction.Instruction)
            {
                res.Value.Qwords[0] -= GET_FLAG(Context, NDR_RFLAG_CF);
            }

            SET_FLAGS(Context, res, dst, src, FM_SUB);

            if (ND_INS_CMP != Context->Instruction.Instruction)
            {
                SET_OP(Context, 0, &res);
            }

            break;

        case ND_INS_INC:
            GET_OP(Context, 0, &dst);
            src.Size = dst.Size;
            src.Value.Qwords[0] = 1;
            res.Size = src.Size;
            res.Value.Qwords[0] = dst.Value.Qwords[0] + src.Value.Qwords[0];
            cf = GET_FLAG(Context, NDR_RFLAG_CF);
            SET_FLAGS(Context, res, dst, src, FM_ADD);
            SET_FLAG(Context, NDR_RFLAG_CF, cf);
            SET_OP(Context, 0, &res);
            break;

        case ND_INS_DEC:
            GET_OP(Context, 0, &dst);
            src.Size = dst.Size;
            src.Value.Qwords[0] = 1;
            res.Size = src.Size;
            res.Value.Qwords[0] = dst.Value.Qwords[0] - src.Value.Qwords[0];
            cf = GET_FLAG(Context, NDR_RFLAG_CF);
            SET_FLAGS(Context, res, dst, src, FM_SUB);
            SET_FLAG(Context, NDR_RFLAG_CF, cf);
            SET_OP(Context, 0, &res);
            break;

        case ND_INS_PUSH:
        case ND_INS_PUSHF:
            GET_OP(Context, 0, &src);
            SET_OP(Context, 1, &src);
            break;

        case ND_INS_POP:
        case ND_INS_POPF:
            GET_OP(Context, 1, &src);
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_PUSHA:
        case ND_INS_PUSHAD:
            src.Size = 32;
            src.Value.Dwords[7] = (ND_UINT32)Context->Registers.RegRax;
            src.Value.Dwords[6] = (ND_UINT32)Context->Registers.RegRcx;
            src.Value.Dwords[5] = (ND_UINT32)Context->Registers.RegRdx;
            src.Value.Dwords[4] = (ND_UINT32)Context->Registers.RegRbx;
            src.Value.Dwords[3] = (ND_UINT32)Context->Registers.RegRsp;
            src.Value.Dwords[2] = (ND_UINT32)Context->Registers.RegRbp;
            src.Value.Dwords[1] = (ND_UINT32)Context->Registers.RegRsi;
            src.Value.Dwords[0] = (ND_UINT32)Context->Registers.RegRdi;
            SET_OP(Context, 1, &src);
            break;

        case ND_INS_POPA:
        case ND_INS_POPAD:
            GET_OP(Context, 1, &src);
            Context->Registers.RegRax = src.Value.Dwords[7];
            Context->Registers.RegRcx = src.Value.Dwords[6];
            Context->Registers.RegRdx = src.Value.Dwords[5];
            Context->Registers.RegRbx = src.Value.Dwords[4];
            Context->Registers.RegRsp = src.Value.Dwords[3];
            Context->Registers.RegRbp = src.Value.Dwords[2];
            Context->Registers.RegRsi = src.Value.Dwords[1];
            Context->Registers.RegRdi = src.Value.Dwords[0];
            break;

        case ND_INS_LEA:
            GET_OP(Context, 1, &src);
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_SHL:
        case ND_INS_SAL:
        case ND_INS_SHR:
        case ND_INS_SAR:
            GET_OP(Context, 0, &dst);
            GET_OP(Context, 1, &src);

            if (dst.Size == 8)
            {
                src.Value.Qwords[0] &= 0x3f;
            }
            else
            {
                src.Value.Qwords[0] &= 0x1f;
            }

            res.Size = dst.Size;

            if (ND_INS_SHL == Context->Instruction.Instruction || 
                ND_INS_SAL == Context->Instruction.Instruction)
            {
                res.Value.Qwords[0] = dst.Value.Qwords[0] << src.Value.Qwords[0];
            }
            else if (ND_INS_SHR == Context->Instruction.Instruction)
            {
                res.Value.Qwords[0] = dst.Value.Qwords[0] >> src.Value.Qwords[0];
            }
            else
            {
                ND_SINT64 val = ND_SIGN_EX(dst.Size, dst.Value.Qwords[0]);
                val = val >> src.Value.Qwords[0];
                res.Value.Qwords[0] = (ND_UINT64)val;
            }

            if (src.Value.Qwords[0] != 0)
            {
                // 0 bit shifts do not affect the flags.
                if (ND_INS_SHL == Context->Instruction.Instruction || 
                    ND_INS_SAL == Context->Instruction.Instruction)
                {
                    SET_FLAGS(Context, res, dst, src, FM_SHL);
                }
                else if (ND_INS_SHR == Context->Instruction.Instruction)
                {
                    SET_FLAGS(Context, res, dst, src, FM_SHR);
                }
                else
                {
                    SET_FLAGS(Context, res, dst, src, FM_SAR);
                }
            }

            SET_OP(Context, 0, &res);
            break;

        case ND_INS_RCL:
        case ND_INS_RCR:
        case ND_INS_ROL:
        case ND_INS_ROR:
            {
                ND_UINT32 cnt, tempcnt, cntmask;
                ND_UINT8 tempCF = 0;

                GET_OP(Context, 0, &dst);
                GET_OP(Context, 1, &src);

                cnt = (ND_UINT32)src.Value.Qwords[0];
                tempcnt = 0;
                cntmask = ((dst.Size == 8) ? 0x3F : 0x1F);

                if (ND_INS_RCL == Context->Instruction.Instruction || 
                    ND_INS_RCR == Context->Instruction.Instruction)
                {
                    switch (dst.Size)
                    {
                    case 1: tempcnt = (cnt & 0x1F) % 9; break;
                    case 2: tempcnt = (cnt & 0x1F) % 17; break;
                    case 4: tempcnt = (cnt & 0x1F); break;
                    case 8: tempcnt = (cnt & 0x3F); break;
                    default: break;
                    }
                }
                else
                {
                    tempcnt = (cnt & cntmask) % (dst.Size * 8);
                }

                if (ND_INS_RCL == Context->Instruction.Instruction)
                {
                    while (tempcnt != 0)
                    {
                        tempCF = ND_MSB(dst.Size, dst.Value.Qwords[0]);
                        dst.Value.Qwords[0] = (dst.Value.Qwords[0] << 1) + GET_FLAG(Context, NDR_RFLAG_CF);
                        SET_FLAG(Context, NDR_RFLAG_CF, tempCF);
                        tempcnt--;
                    }

                    if ((cnt & cntmask) == 1)
                    {
                        SET_FLAG(Context, NDR_RFLAG_OF, ND_MSB(dst.Size, dst.Value.Qwords[0]) ^
                                                            GET_FLAG(Context, NDR_RFLAG_CF));
                    }
                }
                else if (ND_INS_RCR == Context->Instruction.Instruction)
                {
                    if ((cnt & cntmask) == 1)
                    {
                        SET_FLAG(Context, NDR_RFLAG_OF, ND_MSB(dst.Size, dst.Value.Qwords[0]) ^
                                                            GET_FLAG(Context, NDR_RFLAG_CF));
                    }

                    while (tempcnt != 0)
                    {
                        tempCF = ND_LSB(dst.Size, dst.Value.Qwords[0]);
                        dst.Value.Qwords[0] = (dst.Value.Qwords[0] >> 1) + 
                                              ((ND_UINT64)GET_FLAG(Context, NDR_RFLAG_CF) << (dst.Size * 8 - 1));
                        SET_FLAG(Context, NDR_RFLAG_CF, tempCF);
                        tempcnt--;
                    }
                }
                else if (ND_INS_ROL == Context->Instruction.Instruction)
                {
                    while (tempcnt != 0)
                    {
                        tempCF = ND_MSB(dst.Size, dst.Value.Qwords[0]);
                        dst.Value.Qwords[0] = (dst.Value.Qwords[0] << 1) + tempCF;
                        tempcnt--;
                    }

                    if ((cnt & cntmask) != 0)
                    {
                        SET_FLAG(Context, NDR_RFLAG_CF, dst.Value.Qwords[0] & 1);
                    }

                    if ((cnt & cntmask) == 1)
                    {
                        SET_FLAG(Context, NDR_RFLAG_OF, ND_MSB(dst.Size, dst.Value.Qwords[0]) ^
                                                            GET_FLAG(Context, NDR_RFLAG_CF));
                    }
                }
                else // ND_INS_ROR
                {
                    while (tempcnt != 0)
                    {
                        tempCF = ND_LSB(dst.Size, dst.Value.Qwords[0]);
                        dst.Value.Qwords[0] = (dst.Value.Qwords[0] >> 1) + ((ND_UINT64)tempCF << (dst.Size * 8 - 1));
                        tempcnt--;
                    }

                    if ((cnt & cntmask) != 0)
                    {
                        SET_FLAG(Context, NDR_RFLAG_CF, ND_MSB(dst.Size, dst.Value.Qwords[0]));
                    }

                    if ((cnt & cntmask) == 1)
                    {
                        SET_FLAG(Context, NDR_RFLAG_OF, ND_MSB(dst.Size, dst.Value.Qwords[0]) ^ ND_GET_BIT(dst.Size * 8ULL - 2, dst.Value.Qwords[0]));
                    }
                }

                SET_OP(Context, 0, &dst);
            }
            break;

        case ND_INS_OR:
        case ND_INS_XOR:
        case ND_INS_AND:
        case ND_INS_TEST:
            GET_OP(Context, 0, &dst);
            GET_OP(Context, 1, &src);

            res.Size = dst.Size;

            if (ND_INS_OR == Context->Instruction.Instruction)
            {
                res.Value.Qwords[0] = dst.Value.Qwords[0] | src.Value.Qwords[0];
            }
            else if (ND_INS_XOR == Context->Instruction.Instruction)
            {
                res.Value.Qwords[0] = dst.Value.Qwords[0] ^ src.Value.Qwords[0];
            }
            else
            {
                res.Value.Qwords[0] = dst.Value.Qwords[0] & src.Value.Qwords[0];
            }

            if (ND_INS_TEST != Context->Instruction.Instruction)
            {
                SET_OP(Context, 0, &res);
            }

            SET_FLAGS(Context, res, dst, src, FM_LOGIC);
            break;

        case ND_INS_NOT:
            GET_OP(Context, 0, &dst);
            dst.Value.Qwords[0] = ~dst.Value.Qwords[0];
            SET_OP(Context, 0, &dst);
            break;

        case ND_INS_NEG:
            GET_OP(Context, 0, &src);
            dst.Size = src.Size;
            dst.Value.Qwords[0] = 0;
            res.Size = src.Size;
            res.Value.Qwords[0] = dst.Value.Qwords[0] - src.Value.Qwords[0];
            SET_OP(Context, 0, &res);
            SET_FLAGS(Context, res, dst, src, FM_SUB);
            SET_FLAG(Context, NDR_RFLAG_CF, src.Value.Qwords[0] != 0);
            break;

        case ND_INS_BT:
        case ND_INS_BTS:
        case ND_INS_BTR:
        case ND_INS_BTC:
            GET_OP(Context, 0, &dst);
            GET_OP(Context, 1, &src);

            src.Value.Qwords[0] %= dst.Size * 8ULL;

            // Store the bit inside CF.
            SET_FLAG(Context, NDR_RFLAG_CF, (dst.Value.Qwords[0] >> src.Value.Qwords[0]) & 1);

            if (ND_INS_BTS == Context->Instruction.Instruction)
            {
                dst.Value.Qwords[0] |= (1ULL << src.Value.Qwords[0]);
            }
            else if (ND_INS_BTR == Context->Instruction.Instruction)
            {
                dst.Value.Qwords[0] &= ~(1ULL << src.Value.Qwords[0]);
            }
            else if (ND_INS_BTC == Context->Instruction.Instruction)
            {
                dst.Value.Qwords[0] ^= (1ULL << src.Value.Qwords[0]);
            }

            if (ND_INS_BT != Context->Instruction.Instruction)
            {
                SET_OP(Context, 0, &dst);
            }

            break;

        case ND_INS_Jcc:
            if (ShemuEvalCondition(Context, Context->Instruction.Condition))
            {
                // Modify the RIP if the branch is taken.
                GET_OP(Context, 1, &aux);
                aux.Value.Qwords[0] += Context->Instruction.Operands[0].Info.RelativeOffset.Rel;
                SET_OP(Context, 1, &aux);
            }
            break;

        case ND_INS_JrCXZ:
            // Fetch the rCX value. It could be CX, ECX or RCX, depending on address size.
            GET_OP(Context, 1, &rcx);
            if (rcx.Value.Qwords[0] == 0)
            {
                // Modify the RIP if the branch is taken.
                GET_OP(Context, 2, &aux);
                aux.Value.Qwords[0] += Context->Instruction.Operands[0].Info.RelativeOffset.Rel;
                SET_OP(Context, 2, &aux);
            }
            break;

        case ND_INS_LOOP:
        case ND_INS_LOOPNZ:
        case ND_INS_LOOPZ:
            // rCX is decremented first. Note that the size depends on address size.
            GET_OP(Context, 1, &rcx);
            rcx.Value.Qwords[0]--;
            SET_OP(Context, 1, &rcx);
            if (rcx.Value.Qwords[0] > 0)
            {
                if (((ND_INS_LOOPNZ == Context->Instruction.Instruction) && (0 == GET_FLAG(Context, NDR_RFLAG_ZF))) ||
                    ((ND_INS_LOOPZ  == Context->Instruction.Instruction) && (0 != GET_FLAG(Context, NDR_RFLAG_ZF))) ||
                     (ND_INS_LOOP   == Context->Instruction.Instruction))
                {
                    // Modify the RIP if the branch is taken.
                    GET_OP(Context, 2, &aux);
                    aux.Value.Qwords[0] += Context->Instruction.Operands[0].Info.RelativeOffset.Rel;
                    SET_OP(Context, 2, &aux);
                }
            }
            break;

        case ND_INS_JMPNR:
            GET_OP(Context, 1, &aux);
            aux.Value.Qwords[0] += Context->Instruction.Operands[0].Info.RelativeOffset.Rel;
            SET_OP(Context, 1, &aux);
            break;

        case ND_INS_JMPNI:
            GET_OP(Context, 0, &src);
            SET_OP(Context, 1, &src);   // Set the RIP to the new value.
            break;

        case ND_INS_CALLNR:
            // Save the EIP on the stack.
            GET_OP(Context, 1, &aux);
            SET_OP(Context, 2, &aux);
            aux.Value.Qwords[0] += Context->Instruction.Operands[0].Info.RelativeOffset.Rel;
            SET_OP(Context, 1, &aux);
            break;

        case ND_INS_CALLNI:
            GET_OP(Context, 0, &src);
            GET_OP(Context, 1, &dst);   // The RIP
            SET_OP(Context, 2, &dst);   // Save the RIP on the stack.
            SET_OP(Context, 1, &src);   // Set the RIP to the new value.
            break;

        case ND_INS_RETN:
            if (!Context->Instruction.HasImm1)
            {
                // The simple RET form, 0xC3
                GET_OP(Context, 1, &src);
                SET_OP(Context, 0, &src);
            }
            else
            {
                // The RET Imm16 form, 0xC2
                GET_OP(Context, 3, &src);
                SET_OP(Context, 1, &src);

                // Patch the RSP register.
                GET_OP(Context, 2, &aux);
                aux.Value.Qwords[0] += Context->Instruction.Operands[0].Info.Immediate.Imm;
                SET_OP(Context, 2, &aux);
            }
            break;

        case ND_INS_JMPFD:
        case ND_INS_CALLFD:
            cs = (ND_UINT16)Context->Instruction.Operands[0].Info.Address.BaseSeg;
            goto check_far_branch;

        case ND_INS_JMPFI:
        case ND_INS_CALLFI:
        case ND_INS_IRET:
        case ND_INS_RETF:
            if (Context->Instruction.Instruction == ND_INS_RETF)
            {
                if (Context->Instruction.Operands[0].Type == ND_OP_IMM)
                {
                    // RETF imm
                    GET_OP(Context, 3, &src);
                }
                else
                {
                    // RETF
                    GET_OP(Context, 2, &src);
                }
            }
            else if (Context->Instruction.Instruction == ND_INS_IRET)
            {
                // IRET
                 GET_OP(Context, 2, &src);
            }
            else
            {
                // JMP/CALL far
                GET_OP(Context, 0, &src);
            }

            // The destination code segment is the second WORD/DWORD/QWORD.
            switch (Context->Instruction.WordLength)
            {
            case 2:
                cs = (ND_UINT16)src.Value.Words[1];
                break;
            case 4:
                cs = (ND_UINT16)src.Value.Dwords[1];
                break;
            case 8:
                cs = (ND_UINT16)src.Value.Qwords[1];
                break;
            default:
                cs = 0;
                break;
            }

check_far_branch:
            if (Context->Mode == ND_CODE_32 && cs == 0x33)
            {
                Context->Flags |= SHEMU_FLAG_HEAVENS_GATE;
            }

            // We may, in the future, emulate far branches, but they imply some tricky context switches (including
            // the default TEB), so it may not be as straight forward as it seems. For now, all we wish to achieve 
            // is detection of far branches in long-mode, from Wow 64.
            stop = ND_TRUE;
            break;

        case ND_INS_LODS:
        case ND_INS_STOS:
        case ND_INS_MOVS:
            // Fetch the rCX register, which is the third operand in case of repeated instructions.
            while (Context->InstructionsCount < Context->MaxInstructionsCount)
            {
                GET_OP(Context, 2, &rcx);

                if (Context->Instruction.IsRepeated && (rcx.Value.Qwords[0] == 0))
                {
                    break;
                }

                // Load the source into the destination.
                GET_OP(Context, 1, &src);
                SET_OP(Context, 0, &src);

                if (Context->Instruction.IsRepeated)
                {
                    // Decrement RCX.
                    rcx.Value.Qwords[0]--;
                    SET_OP(Context, 2, &rcx);
                }
                else
                {
                    break;
                }

                Context->InstructionsCount++;
            }
            break;

        case ND_INS_SCAS:
        case ND_INS_CMPS:
            while (Context->InstructionsCount < Context->MaxInstructionsCount)
            {
                GET_OP(Context, 2, &rcx);

                if (Context->Instruction.IsRepeated && (rcx.Value.Qwords[0] == 0))
                {
                    break;
                }

                GET_OP(Context, 0, &dst);
                GET_OP(Context, 1, &src);

                res.Size = dst.Size;
                res.Value.Qwords[0] = dst.Value.Qwords[0] - src.Value.Qwords[0];

                ShemuSetFlags(Context, res.Value.Qwords[0], dst.Value.Qwords[0], src.Value.Qwords[0], res.Size, FM_SUB);

                if (Context->Instruction.IsRepeated)
                {
                    // Decrement RCX.
                    rcx.Value.Qwords[0]--;
                    SET_OP(Context, 2, &rcx);

                    if (Context->Instruction.HasRepRepzXrelease && !GET_FLAG(Context, NDR_RFLAG_ZF))
                    {
                        break;
                    }

                    if (Context->Instruction.HasRepnzXacquireBnd && GET_FLAG(Context, NDR_RFLAG_ZF))
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }

                Context->InstructionsCount++;
            }
            break;

        case ND_INS_MUL:
        case ND_INS_IMUL:
            if (Context->Instruction.ExpOperandsCount == 1)
            {
                // MUL or IMUL with a single explicit operand.
                GET_OP(Context, 0, &dst);
                GET_OP(Context, 1, &src);
                res.Size = dst.Size * 2;
            }
            else if (Context->Instruction.ExpOperandsCount == 2)
            {
                // IMUL with 2 explicit operands.
                GET_OP(Context, 0, &dst);
                GET_OP(Context, 1, &src);
                res.Size = dst.Size;
            }
            else
            {
                // IMUL with 3 operands. The first operand is the write-only destination.
                GET_OP(Context, 1, &dst);
                GET_OP(Context, 2, &src);
                res.Size = dst.Size;
            }


            if (dst.Size == 1)
            {
                if (ND_INS_MUL == Context->Instruction.Instruction)
                {
                    res.Value.Words[0] = dst.Value.Bytes[0] * src.Value.Bytes[0];
                }
                else
                {
                    res.Value.Words[0] = (ND_SINT8)dst.Value.Bytes[0] * (ND_SINT8)src.Value.Bytes[0];
                }
            }
            else if (dst.Size == 2)
            {
                if (ND_INS_MUL == Context->Instruction.Instruction)
                {
                    res.Value.Dwords[0] = dst.Value.Words[0] * src.Value.Words[0];
                }
                else
                {
                    res.Value.Dwords[0] = (ND_SINT16)dst.Value.Words[0] * (ND_SINT16)src.Value.Words[0];
                }
            }
            else if (dst.Size == 4)
            {
                if (ND_INS_MUL == Context->Instruction.Instruction)
                {
                    res.Value.Qwords[0] = dst.Value.Qwords[0] * src.Value.Qwords[0];
                }
                else
                {
                    res.Value.Qwords[0] = (ND_SINT64)(ND_SINT32)dst.Value.Dwords[0] * (ND_SINT64)(ND_SINT32)src.Value.Dwords[0];
                }
            }
            else
            {
                if (ND_INS_MUL == Context->Instruction.Instruction)
                {
                    ShemuMultiply64Unsigned(dst.Value.Qwords[0], src.Value.Qwords[0],
                                            &res.Value.Qwords[1], &res.Value.Qwords[0]);
                }
                else
                {
                    ShemuMultiply64Signed(dst.Value.Qwords[0], src.Value.Qwords[0],
                                          (ND_SINT64*)&res.Value.Qwords[1], (ND_SINT64*)&res.Value.Qwords[0]);
                }
            }

            if (Context->Instruction.ExpOperandsCount == 1)
            {
                // The result is stored in AX, DX:AX, EDX:EAX or RDX:RAX for the single-operand form.
                switch (dst.Size)
                {
                case 1:
                    *((ND_UINT16*)&Context->Registers.RegRax) = res.Value.Words[0];
                    break;
                case 2:
                    *((ND_UINT16*)&Context->Registers.RegRdx) = res.Value.Words[1];
                    *((ND_UINT16*)&Context->Registers.RegRax) = res.Value.Words[0];
                    break;
                case 4:
                    Context->Registers.RegRdx = res.Value.Dwords[1];
                    Context->Registers.RegRax = res.Value.Dwords[0];
                    break;
                case 8:
                    Context->Registers.RegRdx = res.Value.Qwords[1];
                    Context->Registers.RegRax = res.Value.Qwords[0];
                    break;
                }
            }
            else
            {
                // The result is truncated and stored in the destination operand for the 2 & 3 operands forms.
                SET_OP(Context, 0, &res);
            }

            // Set the flags.
            if (ND_INS_MUL == Context->Instruction.Instruction)
            {
                ND_UINT8 cfof = 0;

                // CF and OF are set to 0 if the high part of the result is 0, otherwise they are set to 1.
                switch (dst.Size)
                {
                case 1:
                    cfof = (0 == res.Value.Bytes[1]) ? 0 : 1;
                    break;
                case 2:
                    cfof = (0 == res.Value.Words[1]) ? 0 : 1;
                    break;
                case 4:
                    cfof = (0 == res.Value.Dwords[1]) ? 0 : 1;
                    break;
                case 8:
                    cfof = (0 == res.Value.Qwords[1]) ? 0 : 1;
                    break;
                }

                SET_FLAG(Context, NDR_RFLAG_CF, cfof);
                SET_FLAG(Context, NDR_RFLAG_OF, cfof);
            }
            else
            {
                // The CF and OF flags are set when the signed integer value of the intermediate product differs from
                // the sign extended operand - size - truncated product, otherwise the CF and OF flags are cleared.
                ND_UINT8 cfof = 0, sign = 0;

                sign = ND_MSB(dst.Size, res.Value.Qwords[0]);

                switch (dst.Size)
                {
                case 1:
                    cfof = (0 == res.Value.Bytes[1] && 0 == sign) || 
                           ((ND_UINT8)-1 == res.Value.Bytes[1] && 1 == sign) ? 0 : 1;
                    break;
                case 2:
                    cfof = (0 == res.Value.Words[1] && 0 == sign) || 
                           ((ND_UINT16)-1 == res.Value.Words[1] && 1 == sign) ? 0 : 1;
                    break;
                case 4:
                    cfof = (0 == res.Value.Dwords[1] && 0 == sign) || 
                           ((ND_UINT32)-1 == res.Value.Dwords[1] && 1 == sign) ? 0 : 1;
                    break;
                case 8:
                    cfof = (0 == res.Value.Qwords[1] && 0 == sign) || 
                           ((ND_UINT64)-1 == res.Value.Qwords[1] && 1 == sign) ? 0 : 1;
                    break;
                }

                SET_FLAG(Context, NDR_RFLAG_CF, cfof);
                SET_FLAG(Context, NDR_RFLAG_OF, cfof);
            }

            break;

        case ND_INS_DIV:
        case ND_INS_IDIV:
            // DIV and IDIV only exist with a single explicit operand encoding. All flags are undefined.
            GET_OP(Context, 0, &src);

            if (src.Size == 1)
            {
                ND_UINT16 divident;

                divident = (ND_UINT16)Context->Registers.RegRax;

                if (ND_INS_DIV == Context->Instruction.Instruction)
                {
                    if (!ShemuCheckDiv(divident, src.Value.Bytes[0], 1))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Bytes[0] = (ND_UINT8)(divident / src.Value.Bytes[0]);
                    res.Value.Bytes[1] = (ND_UINT8)(divident % src.Value.Bytes[0]);
                }
                else
                {
                    if (!ShemuCheckIdiv((ND_SINT64)(ND_SINT16)divident, (ND_SINT64)(ND_SINT8)src.Value.Bytes[0], 1))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Bytes[0] = (ND_SINT8)((ND_SINT16)divident / (ND_SINT8)src.Value.Bytes[0]);
                    res.Value.Bytes[1] = (ND_SINT8)((ND_SINT16)divident % (ND_SINT8)src.Value.Bytes[0]);
                }

                // Result in AX (AL - quotient, AH - reminder).
                *((ND_UINT16*)&Context->Registers.RegRax) = res.Value.Words[0];
            }
            else if (src.Size == 2)
            {
                ND_UINT32 divident;

                divident = ((ND_UINT32)(ND_UINT16)Context->Registers.RegRdx << 16) | 
                            (ND_UINT32)(ND_UINT16)Context->Registers.RegRax;

                if (ND_INS_DIV == Context->Instruction.Instruction)
                {
                    if (!ShemuCheckDiv(divident, src.Value.Words[0], 2))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Words[0] = (ND_UINT16)(divident / src.Value.Words[0]);
                    res.Value.Words[1] = (ND_UINT16)(divident % src.Value.Words[0]);
                }
                else
                {
                    if (!ShemuCheckIdiv((ND_SINT64)(ND_SINT32)divident, (ND_SINT64)(ND_SINT16)src.Value.Words[0], 2))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Words[0] = (ND_SINT16)((ND_SINT32)divident / (ND_SINT16)src.Value.Words[0]);
                    res.Value.Words[1] = (ND_SINT16)((ND_SINT32)divident % (ND_SINT16)src.Value.Words[0]);
                }

                *((ND_UINT16*)&Context->Registers.RegRdx) = res.Value.Words[1];
                *((ND_UINT16*)&Context->Registers.RegRax) = res.Value.Words[0];
            }
            else if (src.Size == 4)
            {
                ND_UINT64 divident;

                divident = ((ND_UINT64)(ND_UINT32)Context->Registers.RegRdx << 32) | 
                            (ND_UINT64)(ND_UINT32)Context->Registers.RegRax;

                if (ND_INS_DIV == Context->Instruction.Instruction)
                {
                    if (!ShemuCheckDiv(divident, src.Value.Dwords[0], 4))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Dwords[0] = (ND_UINT32)(divident / src.Value.Dwords[0]);
                    res.Value.Dwords[1] = (ND_UINT32)(divident % src.Value.Dwords[0]);
                }
                else
                {
                    if (!ShemuCheckIdiv((ND_SINT64)divident, (ND_SINT64)(ND_SINT32)src.Value.Dwords[0], 4))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Dwords[0] = (ND_SINT32)((ND_SINT64)divident / (ND_SINT32)src.Value.Dwords[0]);
                    res.Value.Dwords[1] = (ND_SINT32)((ND_SINT64)divident % (ND_SINT32)src.Value.Dwords[0]);
                }

                Context->Registers.RegRdx = res.Value.Dwords[1];
                Context->Registers.RegRax = res.Value.Dwords[0];
            }
            else if (src.Size == 8)
            {
                /// Not implemented!
            }

            break;

        case ND_INS_CLD:
            SET_FLAG(Context, NDR_RFLAG_DF, 0);
            break;

        case ND_INS_STD:
            SET_FLAG(Context, NDR_RFLAG_DF, 1);
            break;

        case ND_INS_CLC:
            SET_FLAG(Context, NDR_RFLAG_CF, 0);
            break;

        case ND_INS_STC:
            SET_FLAG(Context, NDR_RFLAG_CF, 1);
            break;

        case ND_INS_CMC:
            Context->Registers.RegFlags ^= NDR_RFLAG_CF;
            break;

        case ND_INS_STI:
            if (Context->Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            SET_FLAG(Context, NDR_RFLAG_IF, 1);
            break;

        case ND_INS_CLI:
            if (Context->Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            SET_FLAG(Context, NDR_RFLAG_IF, 0);
            break;

        case ND_INS_SAHF:
            {
                ND_UINT8 ah = (Context->Registers.RegRax >> 8) & 0xFF;
                // Handle reserved bits.
                ah |= (1 << 1);
                ah &= ~((1 << 3) | (1 << 5));
                ((ND_UINT8 *)&Context->Registers.RegFlags)[0] = ah;
            }
            break;

        case ND_INS_LAHF:
            {
                ND_UINT8 ah = ((ND_UINT8 *)&Context->Registers.RegFlags)[0];
                ((ND_UINT8 *)&Context->Registers.RegRax)[1] = ah;
            }
            break;

        case ND_INS_SALC:
            if (GET_FLAG(Context, NDR_RFLAG_CF))
            {
                *((ND_UINT8 *)&Context->Registers.RegRax) = 0xFF;
            }
            else
            {
                *((ND_UINT8 *)&Context->Registers.RegRax) = 0x0;
            }
            break;

        case ND_INS_NOP:
            Context->NopCount++;
            break;

        case ND_INS_WAIT:
            break;

        case ND_INS_CBW:
        case ND_INS_CWDE:
        case ND_INS_CDQE:
            GET_OP(Context, 1, &src);
            dst.Size = src.Size * 2;
            dst.Value.Qwords[0] = ND_SIGN_EX(src.Size, src.Value.Qwords[0]);
            SET_OP(Context, 0, &dst);
            break;

        case ND_INS_CWD:
        case ND_INS_CDQ:
        case ND_INS_CQO:
            GET_OP(Context, 1, &src);
            dst.Size = src.Size;
            if (ND_GET_SIGN(src.Size, src.Value.Qwords[0]))
            {
                dst.Value.Qwords[0] = 0xFFFFFFFFFFFFFFFF;
            }
            else
            {
                dst.Value.Qwords[0] = 0;
            }
            SET_OP(Context, 0, &dst);
            break;

        case ND_INS_AAA:
        case ND_INS_AAD:
        case ND_INS_AAM:
        case ND_INS_AAS:
        case ND_INS_DAA:
        case ND_INS_DAS:
            // Ignore these for now.
            break;

        case ND_INS_ENDBR:
            // Acts as a NOP, it's just a hint to the decoder.
            break;

        case ND_INS_LFENCE:
        case ND_INS_SFENCE:
        case ND_INS_MFENCE:
            // Nothing can be done for them, really.
            break;

        case ND_INS_CPUID:
            // OK; EAX, EBX, ECX and EDX are modified, which also zeroes the high 32 bit.
            Context->Registers.RegRax = 0;
            Context->Registers.RegRbx = 0;
            Context->Registers.RegRcx = 0;
            Context->Registers.RegRdx = 0;
            break;

        // Some basic MMX/SSE instructions supported.
        case ND_INS_EMMS:
            nd_memzero(Context->MmxRegisters, sizeof(Context->MmxRegisters));
            break;

        case ND_INS_MOVD:
        case ND_INS_MOVQ:
        case ND_INS_MOVDQU:
        case ND_INS_MOVDQA:
            GET_OP(Context, 1, &src);
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_PUNPCKLBW:
            GET_OP(Context, 0, &dst);
            GET_OP(Context, 1, &src);
            if (dst.Size == 8)
            {
                // Operating on MMX register.
                dst.Value.Bytes[7] = src.Value.Bytes[3];
                dst.Value.Bytes[6] = dst.Value.Bytes[3];
                dst.Value.Bytes[5] = src.Value.Bytes[2];
                dst.Value.Bytes[4] = dst.Value.Bytes[2];
                dst.Value.Bytes[3] = src.Value.Bytes[1];
                dst.Value.Bytes[2] = dst.Value.Bytes[1];
                dst.Value.Bytes[1] = src.Value.Bytes[0];
            }
            else
            {
                // Operating on XMM register.
                dst.Value.Bytes[15] = src.Value.Bytes[7];
                dst.Value.Bytes[14] = dst.Value.Bytes[7];
                dst.Value.Bytes[13] = src.Value.Bytes[6];
                dst.Value.Bytes[12] = dst.Value.Bytes[6];
                dst.Value.Bytes[11] = src.Value.Bytes[5];
                dst.Value.Bytes[10] = dst.Value.Bytes[5];
                dst.Value.Bytes[9] = src.Value.Bytes[4];
                dst.Value.Bytes[8] = dst.Value.Bytes[4];
                dst.Value.Bytes[7] = src.Value.Bytes[3];
                dst.Value.Bytes[6] = dst.Value.Bytes[3];
                dst.Value.Bytes[5] = src.Value.Bytes[2];
                dst.Value.Bytes[4] = dst.Value.Bytes[2];
                dst.Value.Bytes[3] = src.Value.Bytes[1];
                dst.Value.Bytes[2] = dst.Value.Bytes[1];
                dst.Value.Bytes[1] = src.Value.Bytes[0];
            }
            SET_OP(Context, 0, &dst);
            break;

        case ND_INS_PXOR:
            GET_OP(Context, 0, &dst);
            GET_OP(Context, 1, &src);
            for (i = 0; i < dst.Size; i++)
            {
                dst.Value.Bytes[i] ^= src.Value.Bytes[i];
            }
            SET_OP(Context, 0, &dst);
            break;

        // Some basic AVX/AVX2 instructions support.
        case ND_INS_VMOVD:
        case ND_INS_VMOVQ:
        case ND_INS_VMOVDQU:
        case ND_INS_VMOVDQA:
            GET_OP(Context, 1, &src);
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_VPBROADCASTB:
        case ND_INS_VPBROADCASTW:
        case ND_INS_VPBROADCASTD:
        case ND_INS_VPBROADCASTQ:
            GET_OP(Context, 1, &src);
            dst.Size = Context->Instruction.Operands[0].Size;
            for (i = 0; i < dst.Size / src.Size; i++)
            {
                switch (src.Size)
                {
                case 1:
                    dst.Value.Bytes[i] = src.Value.Bytes[0];
                    break;
                case 2:
                    dst.Value.Words[i] = src.Value.Words[0];
                    break;
                case 4:
                    dst.Value.Dwords[i] = src.Value.Dwords[0];
                    break;
                default:
                    dst.Value.Qwords[i] = src.Value.Qwords[0];
                    break;
                }
            }
            SET_OP(Context, 0, &dst);
            break;

        case ND_INS_VPXOR:
            GET_OP(Context, 1, &dst);
            GET_OP(Context, 2, &src);
            for (i = 0; i < dst.Size; i++)
            {
                dst.Value.Bytes[i] ^= src.Value.Bytes[i];
            }
            SET_OP(Context, 0, &dst);
            break;

        // Software interrupt/SYSCALL/SYSENTER.
        case ND_INS_INT:
            if (Context->Instruction.Immediate1 == 0x80 ||
                Context->Instruction.Immediate1 == 0x2E)
            {
                Context->Flags |= SHEMU_FLAG_SYSCALL;
            }

            stop = ND_TRUE;
            break;

        case ND_INS_SYSCALL:
        case ND_INS_SYSENTER:
            Context->Flags |= SHEMU_FLAG_SYSCALL;
            stop = ND_TRUE;
            break;

        // Some basic privileged instructions supported, specific to kernel-mode shellcodes.
        case ND_INS_SWAPGS:
            if (Context->Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            Context->Flags |= SHEMU_FLAG_SWAPGS;
            stop = ND_TRUE;
            break;

        case ND_INS_RDMSR:
            if (Context->Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            if ((Context->Registers.RegRcx == 0xC0000082 && ND_CODE_64 == Context->Mode) ||
                (Context->Registers.RegRcx == 0x00000176 && ND_CODE_32 == Context->Mode))
            {
                Context->Flags |= SHEMU_FLAG_SYSCALL_MSR_READ;
            }

            stop = ND_TRUE;
            break;

        case ND_INS_WRMSR:
            if (Context->Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            if ((Context->Registers.RegRcx == 0xC0000082 && ND_CODE_64 == Context->Mode) ||
                (Context->Registers.RegRcx == 0x00000176 && ND_CODE_32 == Context->Mode))
            {
                Context->Flags |= SHEMU_FLAG_SYSCALL_MSR_WRITE;
            }

            stop = ND_TRUE;
            break;

        case ND_INS_SIDT:
            if (Context->Ring == 0)
            {
                // Flag this only in ring0, as we treat the SHEMU_FLAG_SIDT as a ring0 specific indicator - it can be
                // used to locate the kernel image.
                Context->Flags |= SHEMU_FLAG_SIDT;
            }

            stop = ND_TRUE;
            break;

#if defined(ND_ARCH_X64) || defined(ND_ARCH_X86)
        case ND_INS_AESIMC:
        case ND_INS_AESDEC:
        case ND_INS_AESDECLAST:
        {
            __m128i val, key;

            // Make sure AES support is present, and we can emulate AES decryption using AES instructions.
            if (0 == (Context->Options & SHEMU_OPT_SUPPORT_AES))
            {
                stop = ND_TRUE;
                break;
            }

            GET_OP(Context, 0, &dst);
            GET_OP(Context, 1, &src);

            shemu_memcpy(&val, &dst, 16);
            shemu_memcpy(&key, &src, 16);

            if (Context->Instruction.Instruction == ND_INS_AESDEC)
            {
                val = _mm_aesdec_si128(val, key);
            }
            else if (Context->Instruction.Instruction == ND_INS_AESDECLAST)
            {
                val = _mm_aesdeclast_si128(val, key);
            }
            else if (Context->Instruction.Instruction == ND_INS_AESIMC)
            {
                val = _mm_aesimc_si128(key);
            }

            shemu_memcpy(&dst, &val, 16);

            SET_OP(Context, 0, &dst);
            break;
        }
#endif

        case ND_INS_RDTSC:
            src.Size = 4;
            // Set EAX to lower 32 bits.
            src.Value.Dwords[0] = tsc & 0xFFFFFFFF;
            SET_OP(Context, 0, &src);
            // Set EDX to upper 32 bits.
            src.Value.Dwords[0] = tsc >> 32;
            SET_OP(Context, 1, &src);
            break;


        default:
            return SHEMU_ABORT_UNSUPPORTED_INSTRUX;
        }
    }

    // Minimum percent of the instructions were NOPs => consider we have a NOP sled. Note that we get here only if
    // the maximum number of instructions has been emulated successfully; if the emulation is aborted for any reason,
    // this code will have no effect.
    if ((Context->InstructionsCount >= Context->MaxInstructionsCount / 2) && 
        (Context->NopCount >= Context->InstructionsCount * Context->NopThreshold / 100))
    {
        Context->Flags |= SHEMU_FLAG_NOP_SLED;
        return SHEMU_ABORT_SHELLCODE_DETECTED;
    }

    return SHEMU_SUCCESS;
}
