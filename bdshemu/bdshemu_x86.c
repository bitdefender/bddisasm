/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
//
// bdshemu.c
//

#include "../inc/bdshemu.h"
#include "../bddisasm/include/bddisasm_crt.h"
#include "include/bdshemu_common.h"

#if defined(ND_ARCH_X64) || defined(ND_ARCH_X86)
#ifdef __clang__
#include <wmmintrin.h>
#else
#include <immintrin.h>
#endif // __clang__
#endif // defined(ND_ARCH_X64) || defined(ND_ARCH_X86)

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
    shstatus = ShemuX86GetOperandValue(ctx, op, val);                                                                  \
    if (SHEMU_SUCCESS != shstatus)                                                                                     \
    {                                                                                                                  \
        return shstatus;                                                                                               \
    }                                                                                                                  \
}

#define SET_OP(ctx, op, val) {                                                                                         \
    shstatus = ShemuX86SetOperandValue(ctx, op, val);                                                                  \
    if (SHEMU_SUCCESS != shstatus)                                                                                     \
    {                                                                                                                  \
        return shstatus;                                                                                               \
    }                                                                                                                  \
}

#define GET_FLAG(ctx, flg)                  (!!((ctx)->Arch.X86.Registers.RegFlags & (flg)))
#define SET_FLAG(ctx, flg, val)             ShemuX86SetFlag(ctx, flg, val)
#define SET_FLAGS(ctx, dst, src1, src2, fm) ShemuX86SetFlags(ctx, dst.Value.Qwords[0], src1.Value.Qwords[0],           \
                                                             src2.Value.Qwords[0], dst.Size, fm)


//
// ShemuX86SetFlag
//
static void
ShemuX86SetFlag(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Flag,
    ND_UINT64 Value
    )
{
    // {NF} present for instruction, no flags will be modified.
    if (Context->Arch.X86.Instruction.HasNf)
    {
        return;
    }

    if (Value)
    {
        Context->Arch.X86.Registers.RegFlags |= Flag;
    }
    else
    {
        Context->Arch.X86.Registers.RegFlags &= ~Flag;
    }
}


//
// ShemuX86SetFlags
//
static void
ShemuX86SetFlags(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Dst,
    ND_UINT64 Src1,
    ND_UINT64 Src2,
    ND_OPERAND_SIZE Size,
    ND_UINT8 FlagsMode
    )
{
    ND_UINT8 pfArr[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

    // {NF} present for instruction, no flags will be modified.
    if (Context->Arch.X86.Instruction.HasNf)
    {
        return;
    }

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
        Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_PF;
    }
    else
    {
        Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_PF;
    }

    // ZF set if the result is zero.
    if (Dst == 0)
    {
        Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_ZF;
    }
    else
    {
        Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_ZF;
    }

    // SF is set if the sign flag is set.
    if (ND_GET_SIGN(Size, Dst) != 0)
    {
        Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_SF;
    }
    else
    {
        Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_SF;
    }

    // OF and CF are handled differently for some instructions.
    if (FM_LOGIC == FlagsMode)
    {
        // OF and CF are cleared on logic instructions.
        Context->Arch.X86.Registers.RegFlags &= ~(NDR_RFLAG_OF | NDR_RFLAG_CF);
    }
    else if (FM_SHL == FlagsMode)
    {
        // CF is the last bit shifted out of the destination.
        if ((Src2 <= Size * 8ULL) && ND_GET_BIT((Size * 8ULL) - Src2, Src1))
        {
            Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_CF;
        }
        else
        {
            Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_CF;
        }

        if (Src2 == 1)
        {
            if (ND_GET_BIT(Size * 8ULL - 1, Src1) ^ ND_GET_BIT(Size * 8ULL - 2, Src1))
            {
                Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_OF;
            }
            else
            {
                Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_OF;
            }
        }
    }
    else if (FM_SHR == FlagsMode)
    {
        // CF is the last bit shifted out of the destination.
        // Src2 - 1 is ok - this function does not get called if Src2 is 0.
        if (ND_GET_BIT(Src2 - 1, Src1))
        {
            Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_CF;
        }
        else
        {
            Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_CF;
        }

        if (Src2 == 1)
        {
            if (ND_GET_BIT(Size * 8ULL - 1, Src1))
            {
                Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_OF;
            }
            else
            {
                Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_OF;
            }
        }
    }
    else if (FM_SAR == FlagsMode)
    {
        // CF is the last bit shifted out of the destination. In case of SAR, if the shift ammount exceeds the operand
        // size, CF will be 1 if the result is -1, or 0 if the result is 0.
        // Src2 - 1 is ok - this function does not get called if Src2 is 0.
        if (ND_GET_BIT(Src2 - 1, Src1) || ((Src2 >= (ND_UINT64)Size * 8) && Dst != 0))
        {
            Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_CF;
        }
        else
        {
            Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_CF;
        }

        Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_OF;
    }
    else
    {
        // Set CF.
        if ((FM_SUB == FlagsMode) && ((Src1 < Src2) || (Src1 == Src2 && Dst != 0)))
        {
            Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_CF;
        }
        else if ((FM_ADD == FlagsMode) && ((Dst < Src1) || (Dst == Src1 && Src2 != 0)))
        {
            Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_CF;
        }
        else
        {
            Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_CF;
        }

        // Set OF.
        if (FM_SUB == FlagsMode)
        {
            if ((ND_GET_SIGN(Size, Src1) && !ND_GET_SIGN(Size, Src2) && !ND_GET_SIGN(Size, Dst)) ||
                (!ND_GET_SIGN(Size, Src1) && ND_GET_SIGN(Size, Src2) && ND_GET_SIGN(Size, Dst)))
            {
                Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_OF;
            }
            else
            {
                Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_OF;
            }
        }
        else if (FM_ADD == FlagsMode)
        {
            if (ND_GET_SIGN(Size, Src1) == ND_GET_SIGN(Size, Src2) && 
                ND_GET_SIGN(Size, Src1) != ND_GET_SIGN(Size, Dst))
            {
                Context->Arch.X86.Registers.RegFlags |= NDR_RFLAG_OF;
            }
            else
            {
                Context->Arch.X86.Registers.RegFlags &= ~NDR_RFLAG_OF;
            }
        }
    }
}


//
// ShemuX86EvalCondition
//
static ND_BOOL
ShemuX86EvalCondition(
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
// ShemuX86GetGprValue
//
static ND_UINT64
ShemuX86GetGprValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg,
    ND_UINT32 Size,
    ND_BOOL High8
    )
{
    if (High8 && Reg >= 4)
    {
        Reg = Reg - 4;
    }

    // Any read of the GPR, before being modified, counts as being "saved". The reason here is that we cannot
    // easily track a proper save in memory, as the register may be copied in another register, and only then
    // saved.
    if (!(Context->Arch.X86.GprTracker[Reg] & GPR_TRACK_DIRTY))
    {
        Context->Arch.X86.GprTracker[Reg] |= GPR_TRACK_READ;
    }

    switch (Size)
    {
    case 1:
        if (High8)
        {
            // AH, CH, DH or BH accessed.
            return (*(&Context->Arch.X86.Registers.RegRax + Reg) >> 8) & 0xff;
        }
        else
        {
            return *(&Context->Arch.X86.Registers.RegRax + Reg) & 0xff;
        }

    case 2:
        return *(&Context->Arch.X86.Registers.RegRax + Reg) & 0xffff;

    case 4:
        return *(&Context->Arch.X86.Registers.RegRax + Reg) & 0xffffffff;

    default:
        return *(&Context->Arch.X86.Registers.RegRax + Reg);
    }
}


//
// ShemuX86GetGprValue
//
static void
ShemuX86SetGprValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg,
    ND_UINT32 Size,
    ND_UINT64 Value,
    ND_BOOL High8
    )
{
    if (High8 && Reg >= 4)
    {
        Reg = Reg - 4;
    }

    // If ZeroUpper semantic is enabled for APX instruction, make sure to zero out the destination.
    // {ZU} can be either explicit (example: IMUL), or implicit (example: any {ND} instruction).
    if (Context->Arch.X86.Instruction.HasZu || Context->Arch.X86.Instruction.HasNd)
    {
        *(&Context->Arch.X86.Registers.RegRax + Reg) = 0;
    }

    switch (Size)
    {
    case 1:
        if (High8)
        {
            // AH, CH, DH or BH accessed.
            *((ND_UINT8 *)(&Context->Arch.X86.Registers.RegRax + Reg) + 1) = Value & 0xFF;
        }
        else
        {
            *((ND_UINT8 *)(&Context->Arch.X86.Registers.RegRax + Reg)) = Value & 0xff;
        }
        break;

    case 2:
        *((ND_UINT16 *)(&Context->Arch.X86.Registers.RegRax + Reg)) = Value & 0xffff;
        break;

    case 4:
        // Higher ND_UINT32 is always set to zero.
        *(&Context->Arch.X86.Registers.RegRax + Reg) = Value & 0xffffffff;
        break;

    default:
        *(&Context->Arch.X86.Registers.RegRax + Reg) = Value;
        break;
    }

    // Mark the GPR as being dirty/written.
    if (!(Context->Arch.X86.GprTracker[Reg] & GPR_TRACK_READ))
    {
        Context->Arch.X86.GprTracker[Reg] |= GPR_TRACK_DIRTY;
    }
}


//
// ShemuX86GetSegValue
//
static ND_UINT64
ShemuX86GetSegValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg
    )
{
    switch (Reg)
    {
    case NDR_ES:
        return Context->Arch.X86.Segments.Es.Selector;
    case NDR_CS:
        return Context->Arch.X86.Segments.Cs.Selector;
    case NDR_SS:
        return Context->Arch.X86.Segments.Ss.Selector;
    case NDR_DS:
        return Context->Arch.X86.Segments.Ds.Selector;
    case NDR_FS:
        return Context->Arch.X86.Segments.Fs.Selector;
    case NDR_GS:
        return Context->Arch.X86.Segments.Gs.Selector;
    }

    return 0;
}


//
// ShemuX86SetSegValue
//
static void
ShemuX86SetSegValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg,
    ND_UINT16 Value
    )
{
    switch (Reg)
    {
    case NDR_ES:
        Context->Arch.X86.Segments.Es.Selector = Value;
        break;
    case NDR_CS:
        Context->Arch.X86.Segments.Cs.Selector = Value;
        break;
    case NDR_SS:
        Context->Arch.X86.Segments.Ss.Selector = Value;
        break;
    case NDR_DS:
        Context->Arch.X86.Segments.Ds.Selector = Value;
        break;
    case NDR_FS:
        Context->Arch.X86.Segments.Fs.Selector = Value;
        break;
    case NDR_GS:
        Context->Arch.X86.Segments.Gs.Selector = Value;
        break;
    }
}


//
// ShemuX86GetSegBase
//
static ND_UINT64
ShemuX86GetSegBase(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg
    )
{
    switch (Reg)
    {
    case NDR_ES:
        return Context->Arch.X86.Segments.Es.Base;
    case NDR_CS:
        return Context->Arch.X86.Segments.Cs.Base;
    case NDR_SS:
        return Context->Arch.X86.Segments.Ss.Base;
    case NDR_DS:
        return Context->Arch.X86.Segments.Ds.Base;
    case NDR_FS:
        return Context->Arch.X86.Segments.Fs.Base;
    case NDR_GS:
        return Context->Arch.X86.Segments.Gs.Base;
    }

    return 0;
}


//
// ShemuX86IsSelectorValid
//
static ND_BOOL
ShemuX86IsSelectorValid(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Reg,
    ND_UINT16 Value
    )
{
    // Allow NULL selectors in 64 bit mode inside ES, DS, FS, GS.
    if ((Value == 0) && (Context->Arch.X86.Mode == ND_CODE_64) && (Reg != NDR_SS))
    {
        return ND_TRUE;
    }

    if ((Value & 0xFFF8) >= 0x80)
    {
        // Too large value.
        return ND_FALSE;
    }

    if ((Value & 4) != 0)
    {
        // LDT selector.
        return ND_FALSE;
    }

    if ((Context->Arch.X86.Ring == 0) && ((Value & 3) != 0))
    {
        // Ring 0 selector, with RPL != 0.
        return ND_FALSE;
    }

    if ((Context->Arch.X86.Ring == 3) && ((Value & 3) != 3))
    {
        // Ring 3 selector, with RPL != 3.
        return ND_FALSE;
    }

    return ND_TRUE;
}


//
// ShemuX86IsAddressAligned
//
static ND_BOOL
ShemuX86IsAddressAligned(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Address
    )
{
    if (Context->Arch.X86.Mode == ND_CODE_64)
    {
        return Address % 8 == 0;
    }
    else
    {
        return Address % 4 == 0;
    }
}


//
// ShemuX86TrackLoopStart
//
static void
ShemuX86TrackLoopStart(
    SHEMU_CONTEXT *Context,
    ND_UINT64 Rip,
    ND_UINT64 Target
    )
{
    if (Context->LoopTrack.Active)
    {
        return;
    }

    Context->LoopTrack.Active = ND_TRUE;
    Context->LoopTrack.Iteration = 1;
    Context->LoopTrack.Address = Rip;
    Context->LoopTrack.Target = Target;

    if (!!(Context->Options & SHEMU_OPT_TRACE_LOOPS))
    {
        shemu_printf(Context, "        Loop BEGIN from RIP 0x%016llx, TARGET 0x%016llx, ITER %llu\n",
                     Context->LoopTrack.Address, Context->LoopTrack.Target, Context->LoopTrack.Iteration);
    }
}


//
// ShemuX86TrackLoopStop
//
static void
ShemuX86TrackLoopStop(
    SHEMU_CONTEXT *Context
    )
{
    if (!Context->LoopTrack.Active)
    {
        return;
    }

    if (!!(Context->Options & SHEMU_OPT_TRACE_LOOPS))
    {
        shemu_printf(Context, "        Loop BREAK from RIP 0x%016llx, TARGET 0x%016llx, ITER %llu\n",
                     Context->LoopTrack.Address, Context->LoopTrack.Target, Context->LoopTrack.Iteration);
    }

    Context->LoopTrack.Active = ND_FALSE;
    Context->LoopTrack.Address = 0;
    Context->LoopTrack.Target = 0;
    Context->LoopTrack.Iteration = 0;
}


//
// ShemuX86TrackLoop
//
static ND_BOOL
ShemuX86TrackLoop(
    SHEMU_CONTEXT *Context,
    ND_BOOL Taken
    )
{
    ND_UINT64 rip, target;

    // The address of the loop instruction itself, not the next instruction.
    rip = Context->Arch.X86.Registers.RegRip - Context->Arch.X86.Instruction.Length;

    // The branches we monitor always have a relative offset, which is the first operand.
    target = Context->Arch.X86.Registers.RegRip + Context->Arch.X86.Instruction.Operands[0].Info.RelativeOffset.Rel;

    if (target < rip)
    {
        // Case 1: Backward branch.
        if (Taken)
        {
            // Taken backward branch instruction.
            if (rip == Context->LoopTrack.Address)
            {
                // Current active loop, this is a new iteration. Example:
                // label1:                  <--+
                //      ...                    |
                //      loop    label1      ---+
                //      ...
                // Update loop information.
                Context->LoopTrack.Iteration++;
            }
            else
            {
                // New loop, or loop within our current loop. Always track the inner most loop. Example:
                // label1:                  <--+
                //      ...                    |
                // label2:              <--+   |
                //      ...                |   |
                //      loop    label2  ---+   |
                //      ...                    |
                //      loop    label1      ---+
                //      ...
                // Start tracking the (potentially new) loop.
                ShemuX86TrackLoopStop(Context);

                ShemuX86TrackLoopStart(Context, rip, target);
            }
        }
        else
        {
            // Not taken backward branch instruction.
            if (rip == Context->LoopTrack.Address)
            {
                // Current loop ends, the loop instruction is no longer taken. Example:
                // label1:
                //      ...
                //      loop    label1      ---+
                //      ...                 <--+
                // Stop tracking.
                ShemuX86TrackLoopStop(Context);
            }
            else
            {
                // Not taken backward branch, but not our current loop.
                // Nothing to do.
            }
        }
    }
    else
    {
        // Case 2: Forward branch.
        if (Taken)
        {
            // Taken forward branch instruction.
            if (target < Context->LoopTrack.Target || target > Context->LoopTrack.Address)
            {
                // Break instruction. Example:
                // label1:
                //      ...
                //      jnz     label2      ---+
                //      ...                    |
                //      loop    label1         |
                //      ...                    |
                // label2:                  <--+
                //      ...
                // Stop tracking.
                ShemuX86TrackLoopStop(Context);
            }
            else
            {
                // If instruction. Example:
                // label1:
                //      ...
                //      jnz     label2      ---+
                //      ...                    |
                // label2:                  <--+
                //      ...
                //      loop    label1
                //      ...
                // Nothing to do.
            }
        }
        else
        {
            // Not taken forward branch instruction.
            // Nothing to do.
        }
    }

    // In theory, we can override whether the branch is taken or not, but for now, this is not used.
    return Taken;
}


//
// ShemuX86ComputeLinearAddress
//
static ND_UINT64
ShemuX86ComputeLinearAddress(
    SHEMU_CONTEXT *Context,
    PND_OPERAND Operand
    )
{
    ND_UINT64 gla = 0;

    if (Operand->Info.Memory.HasBase)
    {
        gla += ShemuX86GetGprValue(Context, Operand->Info.Memory.Base, Operand->Info.Memory.BaseSize, ND_FALSE);
    }

    if (Operand->Info.Memory.HasIndex)
    {
        gla += ShemuX86GetGprValue(Context, Operand->Info.Memory.Index, Operand->Info.Memory.IndexSize, ND_FALSE) *
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
        gla += Context->Arch.X86.Registers.RegRip;
    }

    // Special handling for BT, BTR, BTS, BTC instructions with bitbase addressing.
    if (Operand->Info.Memory.IsBitbase)
    {
        ND_UINT64 bitbase, op1size, op2size, reg;

        op1size = Context->Arch.X86.Instruction.Operands[0].Size;
        op2size = Context->Arch.X86.Instruction.Operands[1].Size;

        reg = ((ND_UINT64*)&Context->Arch.X86.Registers.RegRax)[Context->Arch.X86.Instruction.Operands[1].Info.Register.Reg];

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
    switch (Context->Arch.X86.Instruction.AddrMode)
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
        gla += ShemuX86GetSegBase(Context, Operand->Info.Memory.Seg);

        if (Context->Arch.X86.Mode != ND_CODE_64)
        {
            // Truncate to 32 bit outside 64 bit.
            gla &= 0xFFFFFFFF;
        }
    }

    return gla;
}



//
// ShemuX86GetOperandValue
//
static SHEMU_STATUS
ShemuX86GetOperandValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Operand,
    SHEMU_VALUE *Value
    )
{
    SHEMU_STATUS status;
    PND_OPERAND op = &Context->Arch.X86.Instruction.Operands[Operand];

    Value->Size = op->Size;
    if (Value->Size > sizeof(Value->Value))
    {
        return SHEMU_ABORT_OPERAND_NOT_SUPPORTED;
    }

    if (op->Type == ND_OP_REG)
    {
        switch (op->Info.Register.Type)
        {
        case ND_REG_GPR:
            Value->Value.Qwords[0] = ShemuX86GetGprValue(Context, op->Info.Register.Reg, op->Size, 
                                                         op->Info.Register.IsHigh8);
            break;

        case ND_REG_SEG:
            Value->Value.Qwords[0] = ShemuX86GetSegValue(Context, op->Info.Register.Reg);
            break;

        case ND_REG_MMX:
            Value->Value.Qwords[0] = Context->Arch.X86.MmxRegisters[op->Info.Register.Reg];
            break;

        case ND_REG_SSE:
            shemu_memcpy(Value->Value.Bytes,
                         &Context->Arch.X86.SseRegisters[op->Info.Register.Reg], 
                         op->Size);
            break;

        case ND_REG_RIP:
            Value->Value.Qwords[0] = ND_TRIM(Value->Size, Context->Arch.X86.Registers.RegRip);
            break;

        case ND_REG_FLG:
            Value->Value.Qwords[0] = Context->Arch.X86.Registers.RegFlags;
            break;

        case ND_REG_CR:
            switch (op->Info.Register.Reg)
            {
            case NDR_CR0:
                Value->Value.Qwords[0] = Context->Arch.X86.Registers.RegCr0;
                break;
            case NDR_CR2:
                Value->Value.Qwords[0] = Context->Arch.X86.Registers.RegCr2;
                break;
            case NDR_CR3:
                Value->Value.Qwords[0] = Context->Arch.X86.Registers.RegCr3;
                break;
            case NDR_CR4:
                Value->Value.Qwords[0] = Context->Arch.X86.Registers.RegCr4;
                break;
            case NDR_CR8:
                Value->Value.Qwords[0] = Context->Arch.X86.Registers.RegCr8;
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
        ND_UINT64 gla = ShemuX86ComputeLinearAddress(Context, op);
        ND_UINT32 offsetPeb;
        ND_UINT8 seg;

        if (op->Info.Memory.IsAG)
        {
            // Address generation instruction, the result is the linear address itself.
            Value->Value.Qwords[0] = gla;
            goto done_gla;
        }

        if (Context->Arch.X86.Ring == 3)
        {
            // User-mode TIB offset that contains the PEB address.
            offsetPeb = Context->Arch.X86.Mode == ND_CODE_32 ? 0x30 : 0x60;
            seg = Context->Arch.X86.Mode == ND_CODE_32 ? ND_PREFIX_G2_SEG_FS : ND_PREFIX_G2_SEG_GS;
        }
        else
        {
            // Kernel-mode KPCR offset that contains the current KTHREAD address.
            offsetPeb = Context->Arch.X86.Mode == ND_CODE_32 ? 0x124 : 0x188;
            seg = Context->Arch.X86.Mode == ND_CODE_32 ? ND_PREFIX_G2_SEG_FS : ND_PREFIX_G2_SEG_GS;
        }

        // Check if this is a TIB/PCR access. Make sure the FS/GS register is used for the access, in order to avoid
        // FALSE positives where legitimate code accesses a linear TIB directly.
        // Note that this covers accesses to the PEB field inside the TIB.
        if (Context->Arch.X86.Ring == 3)
        {
            if (Context->Arch.X86.Instruction.Seg == seg &&
                gla == Context->TibBase + offsetPeb)
            {
                Context->Flags |= SHEMU_FLAG_TIB_ACCESS_PEB;
            }
        }

        // Note that this covers accesses to the Wow32Reserved in Wow64 mode. That field can be used to issue
        // syscalls.
        if (gla == Context->TibBase + 0xC0 && Context->Arch.X86.Instruction.Seg == seg && 
            Context->Arch.X86.Mode == ND_CODE_32)
        {
            Context->Flags |= SHEMU_FLAG_TIB_ACCESS_WOW32;
        }

        // Check for accesses inside the KUSER_SHARED_DATA (SharedUserData). This page contains some
        // global system information, it may host shellcodes, and is hard-coded at this address. We
        // only consider accesses to some SUD fields, in order to avoid false-positives.
        if ((gla >= 0x7FFE02D4 && gla < 0x7FFE02D5) || // KdDebuggerEnabled 
            (gla >= 0x7FFE0308 && gla < 0x7FFE1310) || // SystemCall
            (gla >= 0x7FFE0330 && gla < 0x7FFE1338))   // Cookie
        {
            Context->Flags |= SHEMU_FLAG_SUD_ACCESS;
        }

        // Check if we are reading a previously saved RIP. Ignore RET instructions, which naturally access the 
        // saved RIP.
        if (Context->Arch.X86.Instruction.Category != ND_CAT_RET &&
            ShemuIsStackPtr(Context, gla, op->Size) &&
            ShemuStackBmpStateCheck(Context, gla - Context->StackBase, op->Size, STACK_BYTE_RIP))
        {
            PND_OPERAND dst = ND_NULL;

            for (ND_UINT32 i = 0; i < Context->Arch.X86.Instruction.OperandsCount; i++)
            {
                // We fetch the first destination, if any. We ignore the current operand.
                if ((i != Operand) && (Context->Arch.X86.Instruction.Operands[i].Access.Access & ND_ACCESS_ANY_WRITE))
                {
                    dst = &Context->Arch.X86.Instruction.Operands[i];
                    break;
                }
            }

            // We consider a LOAD_RIP detection only if the RIP value is loaded in:
            // - Another memory location;
            // - A register, which is not (excluding bank register access, such as PPOPA):
            //      - A segment register;
            //      - The RIP register;
            //      - The Flags register;
            // Loads which are propagated to other operand types (RIP, segment register, etc.) or which are not 
            // propagated at all (RMW instructions) are not considered, since they don't store the RIP anywehere.
            if ((dst != ND_NULL) &&
                ((dst->Type == ND_OP_BANK) ||
                 (dst->Type == ND_OP_MEM) ||
                 (dst->Type == ND_OP_REG && 
                     dst->Info.Register.Type != ND_REG_SEG && 
                     dst->Info.Register.Type != ND_REG_RIP &&
                     dst->Info.Register.Type != ND_REG_FLG)))
            {
                Context->Flags |= SHEMU_FLAG_LOAD_RIP;
            }
        }

        // Get the memory value.
        status = ShemuMemLoad(Context, gla, Value->Size, Value->Value.Bytes);
        if (SHEMU_SUCCESS != status)
        {
            return status;
        }

        if (Context->Options & SHEMU_OPT_TRACE_MEMORY)
        {
            ShemuDisplayMemValue(Context, gla, Value->Size, Value->Value.Bytes, ND_TRUE);
        }

        // If this is a stack access, we need to update the stack pointer.
        if (op->Info.Memory.IsStack)
        {
            ND_UINT64 regval = ShemuX86GetGprValue(Context, NDR_RSP, (2 << Context->Arch.X86.Instruction.DefStack), ND_FALSE);

            regval += op->Size;

            ShemuX86SetGprValue(Context, NDR_RSP, (2 << Context->Arch.X86.Instruction.DefStack), regval, ND_FALSE);
        }

        // If this is a string operation, make sure we update RSI/RDI.
        if (op->Info.Memory.IsString)
        {
            ND_UINT64 regval = ShemuX86GetGprValue(Context, op->Info.Memory.Base, op->Info.Memory.BaseSize, ND_FALSE);

            regval = GET_FLAG(Context, NDR_RFLAG_DF) ? regval - op->Size : regval + op->Size;

            ShemuX86SetGprValue(Context, op->Info.Memory.Base, op->Info.Memory.BaseSize, regval, ND_FALSE);
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
        return SHEMU_ABORT_OPERAND_NOT_SUPPORTED;
    }

    return SHEMU_SUCCESS;
}


//
// ShemuX86SetOperandValue
//
static SHEMU_STATUS
ShemuX86SetOperandValue(
    SHEMU_CONTEXT *Context,
    ND_UINT32 Operand,
    SHEMU_VALUE *Value
    )
{
    SHEMU_STATUS status;
    PND_OPERAND op = &Context->Arch.X86.Instruction.Operands[Operand];

    // If a stack address is being loaded, check if it points to a string built on the stack.
    if (ShemuIsStackPtr(Context, Value->Value.Qwords[0], Context->StrThreshold))
    {
        ND_UINT64 sptr = Value->Value.Qwords[0];
        ND_UINT32 i, stckstrlen = 0;

        // Check if a string was saved on the stack. Typically used by shellcodes like this:
        // PUSH str0
        // PUSH str1
        // ...
        // PUSH strn
        // Other variants may exist, but all we care about are stores on the stack, and all are checked.

        for (i = 0; i < Context->StrThreshold; i++)
        {
            unsigned char c;

            status = ShemuMemLoad(Context, sptr + i, 1, &c);
            if (SHEMU_SUCCESS != status)
            {
                break;
            }

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

        if (stckstrlen >= Context->StrThreshold)
        {
            Context->Flags |= SHEMU_FLAG_STACK_STR;
        }
    }


    if (op->Type == ND_OP_REG)
    {
        switch (op->Info.Register.Type)
        {
        case ND_REG_GPR:
            if (Context->Arch.X86.Instruction.Instruction == ND_INS_XCHG &&
                op->Info.Register.Reg == NDR_RSP &&
                ShemuX86IsAddressAligned(Context, Value->Value.Qwords[0]))
            {
                // Conditions for a STACK_PIVOT detection:
                // 1. The instruction is XCHG
                // 2. The value loaded in the RSP register is naturally aligned
                // 3. The value points either inside the shellcode or the stack area, and at least 64 bytes are valid
                if (ShemuIsShellcodePtr(Context, Value->Value.Qwords[0], 64) ||
                    ShemuIsStackPtr(Context, Value->Value.Qwords[0], 64))
                {
                    Context->Flags |= SHEMU_FLAG_STACK_PIVOT;
                }
            }

            ShemuX86SetGprValue(Context, op->Info.Register.Reg, op->Size, Value->Value.Qwords[0], 
                             op->Info.Register.IsHigh8);
            break;

        case ND_REG_SEG:
            if (!ShemuX86IsSelectorValid(Context, op->Info.Register.Reg, Value->Value.Words[0]))
            {
                return SHEMU_ABORT_INVALID_SELECTOR;
            }
            ShemuX86SetSegValue(Context, op->Info.Register.Reg, Value->Value.Words[0]);
            break;

        case ND_REG_MMX:
            Context->Arch.X86.MmxRegisters[op->Info.Register.Reg] = Value->Value.Qwords[0];
            // Only log these when they're written.
            if (Context->Options & SHEMU_OPT_TRACE_EMULATION)
            {
                shemu_printf(Context, "        MM%d = 0x%016llx\n", op->Info.Register.Reg, Value->Value.Qwords[0]);
            }
            break;

        case ND_REG_SSE:
            if (Context->Arch.X86.Instruction.EncMode != ND_ENCM_LEGACY)
            {
                // Zero the entire register first, if we have a VEX/EVEX encoded instruction.
                nd_memzero(&Context->Arch.X86.SseRegisters[op->Info.Register.Reg], ND_MAX_REGISTER_SIZE);
            }
            else
            {
                // Zero upper bits in the 128 bits register, if operand size is less than 16 bytes.
                // Upper bits in the YMM/ZMM register are preserved.
                nd_memzero(&Context->Arch.X86.SseRegisters[op->Info.Register.Reg], 16);
            }
            // Copy the value.
            shemu_memcpy(&Context->Arch.X86.SseRegisters[op->Info.Register.Reg],
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
            Context->Arch.X86.Registers.RegRip = ND_TRIM(Value->Size, Value->Value.Qwords[0]);
            break;

        case ND_REG_FLG:
            if (op->Size == 2)
            {
                *((ND_UINT16*)&Context->Arch.X86.Registers.RegFlags) = Value->Value.Words[0];
            }
            else
            {
                Context->Arch.X86.Registers.RegFlags = Value->Value.Qwords[0];
            }
            // Handle reserved bits.
            Context->Arch.X86.Registers.RegFlags |= (1ULL << 1);
            Context->Arch.X86.Registers.RegFlags &= ~((1ULL << 3) | (1ULL << 5) | (1ULL << 15));
            Context->Arch.X86.Registers.RegFlags &= 0x3FFFFF;
            break;

        case ND_REG_CR:
            switch (op->Info.Register.Reg)
            {
            case NDR_CR0:
                Context->Arch.X86.Registers.RegCr0 = Value->Value.Qwords[0];
                break;
            case NDR_CR2:
                Context->Arch.X86.Registers.RegCr2 = Value->Value.Qwords[0];
                break;
            case NDR_CR3:
                Context->Arch.X86.Registers.RegCr3 = Value->Value.Qwords[0];
                break;
            case NDR_CR4:
                Context->Arch.X86.Registers.RegCr4 = Value->Value.Qwords[0];
                break;
            case NDR_CR8:
                Context->Arch.X86.Registers.RegCr8 = Value->Value.Qwords[0];
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
        ND_UINT64 gla = ShemuX86ComputeLinearAddress(Context, op);

        // Handle self-write. We store a 1 for each written byte inside the shellcode space. Once the modified bytes
        // are executed, we can trigger the self-write detection.
        if (ShemuIsShellcodePtr(Context, gla, op->Size))
        {
            ShemuShellBmpStateSet(Context, gla - Context->ShellcodeBase, op->Size, SHELL_BYTE_DIRTY);
        }

        // Handle RIP save on the stack.
        if (ShemuIsStackPtr(Context, gla, MAX(op->Size, Context->Arch.X86.Instruction.WordLength)))
        {
            // Note: only Context->Arch.X86.Instruction.WordLength bits are flagged as RIP, as that is the RIP size.
            if (Context->Arch.X86.Instruction.Instruction == ND_INS_CALLNR ||
                Context->Arch.X86.Instruction.Instruction == ND_INS_CALLNI)
            {
                ShemuStackBmpStateSet(Context, gla - Context->StackBase, 
                                      Context->Arch.X86.Instruction.WordLength, STACK_BYTE_RIP);
            }
            else if (Context->Arch.X86.Instruction.Instruction == ND_INS_FNSTENV)
            {
                // OK: op->Size will be the FPU state size (28 bytes); we only emulate 32 & 64 bit forms, the RIP is 
                // always 4 bytes.
                ShemuStackBmpStateSet(Context, (gla + 0xC) - Context->StackBase, 
                                      MIN(Context->Arch.X86.Instruction.WordLength, 4), STACK_BYTE_RIP);
            }
            else if (Context->Arch.X86.Instruction.Instruction == ND_INS_FXSAVE || 
                Context->Arch.X86.Instruction.Instruction == ND_INS_FXSAVE64)
            {
                // OK: op->Size will be the FXSAVE size (512 bytes).
                ShemuStackBmpStateSet(Context, (gla + 0x8) - Context->StackBase, 
                                      Context->Arch.X86.Instruction.WordLength, STACK_BYTE_RIP);
            }
            else
            {
                // Something is written on a previously saved RIP; reset it.
                ShemuStackBmpStateClear(Context, gla - Context->StackBase, op->Size, STACK_BYTE_RIP);
            }
        }

        // Set the value.
        status = ShemuMemStore(Context, gla, MIN(op->Size, Value->Size), Value->Value.Bytes);
        if (SHEMU_SUCCESS != status)
        {
            return status;
        }

        if (Context->Options & SHEMU_OPT_TRACE_MEMORY)
        {
            ShemuDisplayMemValue(Context, gla, Value->Size, Value->Value.Bytes, ND_FALSE);
        }

        // If this is a stack access, we need to update the stack pointer.
        if (op->Info.Memory.IsStack)
        {
            ND_UINT64 regval = ShemuX86GetGprValue(Context, NDR_RSP, (2 << Context->Arch.X86.Instruction.DefStack), ND_FALSE);

            regval -= op->Size;

            ShemuX86SetGprValue(Context, NDR_RSP, (2 << Context->Arch.X86.Instruction.DefStack), regval, ND_FALSE);
        }

        // If this is a string operation, make sure we update RSI/RDI.
        if (op->Info.Memory.IsString)
        {
            ND_UINT64 regval = ShemuX86GetGprValue(Context, op->Info.Memory.Base, op->Info.Memory.BaseSize, ND_FALSE);

            regval = GET_FLAG(Context, NDR_RFLAG_DF) ? regval - op->Size : regval + op->Size;

            ShemuX86SetGprValue(Context, op->Info.Memory.Base, op->Info.Memory.BaseSize, regval, ND_FALSE);
        }
    }
    else
    {
        return SHEMU_ABORT_OPERAND_NOT_SUPPORTED;
    }

    return SHEMU_SUCCESS;
}


//
// ShemuX86Multiply64Unsigned
//
static void
ShemuX86Multiply64Unsigned(
    SHEMU_VALUE *Operand1,
    SHEMU_VALUE *Operand2,
    SHEMU_VALUE *Result
    )
{
    ND_UINT64 p0, p1, p2, p3, p4;

    // Multiply the 4 32-bit parts into 4 partial products.
    p0 = (ND_UINT64)Operand1->Value.Dwords[0] * (ND_UINT64)Operand2->Value.Dwords[0];
    p1 = (ND_UINT64)Operand1->Value.Dwords[0] * (ND_UINT64)Operand2->Value.Dwords[1];
    p2 = (ND_UINT64)Operand1->Value.Dwords[1] * (ND_UINT64)Operand2->Value.Dwords[0];
    p3 = (ND_UINT64)Operand1->Value.Dwords[1] * (ND_UINT64)Operand2->Value.Dwords[1];
    p4 = (((p0 >> 32) + (p1 & 0xFFFFFFFF) + (p2 & 0xFFFFFFFF)) >> 32) & 0xFFFFFFFF;

    // Fill in the final result (low & high 64-bit parts).
    Result->Value.Qwords[0] = p0 + (p1 << 32) + (p2 << 32);
    Result->Value.Qwords[1] = p3 + (p1 >> 32) + (p2 >> 32) + p4;
}


//
// ShemuX86Multiply64Signed
//
static void
ShemuX86Multiply64Signed(
    SHEMU_VALUE *Operand1,
    SHEMU_VALUE *Operand2,
    SHEMU_VALUE *Result
    )
{
    ShemuX86Multiply64Unsigned(Operand1, Operand2, Result);

    // Negate, if needed.
    if (ND_GET_SIGN(8, Operand1->Value.Qwords[0]))
    {
        Result->Value.Qwords[1] -= Operand2->Value.Qwords[0];
    }

    // Negate, if needed.
    if (ND_GET_SIGN(8, Operand2->Value.Qwords[0]))
    {
        Result->Value.Qwords[1] -= Operand1->Value.Qwords[0];
    }
}


//
// ShemuX86CheckDiv
//
static ND_BOOL
ShemuX86CheckDiv(
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
// ShemuX86CheckIdiv
//
static ND_BOOL
ShemuX86CheckIdiv(
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
    if (!ShemuX86CheckDiv(Divident, Divider, Size))
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
// ShemuX86CountZeroBits
//
static ND_UINT8
ShemuX86CountZeroBits(
    ND_UINT64 Value,
    ND_OPERAND_SIZE Size,
    ND_BOOL Forward
    )
{
    ND_UINT8 cnt = 0;

    if (Forward)
    {
        for (ND_UINT32 i = 0; i < Size * 8 && ND_GET_BIT(i, Value) == 0; i++, cnt++);
    }
    else
    {
        for (ND_SINT32 i = Size * 8 - 1; i >= 0 && ND_GET_BIT(i, Value) == 0; i--, cnt++);
    }

    return cnt;
}


//
// ShemuX86PrintContext
//
#ifndef BDDISASM_NO_FORMAT
static void
ShemuX86PrintContext(
    SHEMU_CONTEXT *Context
    )
{
    char text[ND_MIN_BUF_SIZE] = { 0 };
    char ibytes[ND_MAX_INSTRUCTION_LENGTH * 2 + 2] = { 0 };

    NdToText(&Context->Arch.X86.Instruction, Context->Arch.X86.Registers.RegRip, ND_MIN_BUF_SIZE, text);

    shemu_printf(Context, "        RAX = 0x%016llx RCX = 0x%016llx RDX = 0x%016llx RBX = 0x%016llx\n",
        Context->Arch.X86.Registers.RegRax, Context->Arch.X86.Registers.RegRcx, 
        Context->Arch.X86.Registers.RegRdx, Context->Arch.X86.Registers.RegRbx);
    shemu_printf(Context, "        RSP = 0x%016llx RBP = 0x%016llx RSI = 0x%016llx RDI = 0x%016llx\n",
        Context->Arch.X86.Registers.RegRsp, Context->Arch.X86.Registers.RegRbp, 
        Context->Arch.X86.Registers.RegRsi, Context->Arch.X86.Registers.RegRdi);
    shemu_printf(Context, "        R8  = 0x%016llx R9  = 0x%016llx R10 = 0x%016llx R11 = 0x%016llx\n",
        Context->Arch.X86.Registers.RegR8, Context->Arch.X86.Registers.RegR9, 
        Context->Arch.X86.Registers.RegR10, Context->Arch.X86.Registers.RegR11);
    shemu_printf(Context, "        R12 = 0x%016llx R13 = 0x%016llx R14 = 0x%016llx R15 = 0x%016llx\n",
        Context->Arch.X86.Registers.RegR12, Context->Arch.X86.Registers.RegR13, 
        Context->Arch.X86.Registers.RegR14, Context->Arch.X86.Registers.RegR15);

    if (Context->Options & SHEMU_OPT_SUPPORT_APX)
    {
        shemu_printf(Context, "        R16 = 0x%016llx R17 = 0x%016llx R18 = 0x%016llx R19 = 0x%016llx\n",
            Context->Arch.X86.Registers.RegR16, Context->Arch.X86.Registers.RegR17,
            Context->Arch.X86.Registers.RegR18, Context->Arch.X86.Registers.RegR19);
        shemu_printf(Context, "        R20 = 0x%016llx R21 = 0x%016llx R22 = 0x%016llx R23 = 0x%016llx\n",
            Context->Arch.X86.Registers.RegR20, Context->Arch.X86.Registers.RegR21,
            Context->Arch.X86.Registers.RegR22, Context->Arch.X86.Registers.RegR23);
        shemu_printf(Context, "        R24 = 0x%016llx R25 = 0x%016llx R26 = 0x%016llx R27 = 0x%016llx\n",
            Context->Arch.X86.Registers.RegR24, Context->Arch.X86.Registers.RegR25,
            Context->Arch.X86.Registers.RegR26, Context->Arch.X86.Registers.RegR27);
        shemu_printf(Context, "        R28 = 0x%016llx R29 = 0x%016llx R30 = 0x%016llx R31 = 0x%016llx\n",
            Context->Arch.X86.Registers.RegR28, Context->Arch.X86.Registers.RegR29,
            Context->Arch.X86.Registers.RegR30, Context->Arch.X86.Registers.RegR31);
    }

    shemu_printf(Context, "        RIP = 0x%016llx RFLAGS = 0x%016llx ", 
        Context->Arch.X86.Registers.RegRip, Context->Arch.X86.Registers.RegFlags);
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

    ShemuHexlify(Context->Arch.X86.Instruction.InstructionBytes, Context->Arch.X86.Instruction.Length, 
        ibytes, sizeof(ibytes));
    
    shemu_printf(Context, "IP: 0x%016llx  %-30s  %s\n", Context->Arch.X86.Registers.RegRip, ibytes, text);
    
}
#else
#define ShemuX86PrintContext(Context)
#endif // !BDDISASM_NO_FORMAT 


//
// ShemuX86Emulate
//
SHEMU_STATUS
ShemuX86Emulate(
    SHEMU_CONTEXT *Context
    )
{
    ND_CONTEXT decodeCtx = { 0 };
    SHEMU_VALUE res, dst, src;
    ND_BOOL stop = ND_FALSE, cf, sled = ND_TRUE, taken = ND_FALSE;
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

    if (0 == (Context->Options & SHEMU_OPT_DIRECT_MAPPED_SHELL))
    {
        if (Context->StackSize + Context->ShellcodeSize > Context->IntbufSize)
        {
            return SHEMU_ABORT_INVALID_PARAMETER;
        }
    }
    else
    {
        if (Context->StackSize > Context->IntbufSize)
        {
            return SHEMU_ABORT_INVALID_PARAMETER;
        }

        if (Context->AccessShellcode == ND_NULL)
        {
            // The AccessShellcode callback is mandatory when using the SHEMU_OPT_DIRECT_MAPPED_SHELL option.
            return SHEMU_ABORT_INVALID_PARAMETER;
        }

        // It is unsafe to allow self-writes to proceed, so this options is forced when using 
        // SHEMU_OPT_DIRECT_MAPPED_SHELL emulation.
        Context->Options |= SHEMU_OPT_BYPASS_SELF_WRITES;
    }

    if (Context->ArchType != SHEMU_ARCH_TYPE_X86)
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

    decodeCtx.DefCode = Context->Arch.X86.Mode;
    decodeCtx.DefData = Context->Arch.X86.Mode;
    decodeCtx.DefStack = Context->Arch.X86.Mode;
    decodeCtx.VendMode = ND_VEND_ANY;
    decodeCtx.Options = 0;
    decodeCtx.FeatMode = 0;

    // Enable APX support.
    if (Context->Options & SHEMU_OPT_SUPPORT_APX)
    {
        decodeCtx.FeatMode |= ND_FEAT_APX;
    }

    while (Context->InstructionsCount++ < Context->MaxInstructionsCount)
    {
        SHEMU_STATUS shstatus;
        NDSTATUS ndstatus;
        ND_UINT64 rip;          // Offset, not actual linear address.
        ND_UINT32 i, ilen;
        ND_UINT8 ibytes[16];
        ND_BOOL hasNd = ND_FALSE;

        tsc++;

        // Reset all the operands to 0.
        nd_memzero(&res, sizeof(res));
        nd_memzero(&dst, sizeof(dst));
        nd_memzero(&src, sizeof(src));

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
        if (!ShemuIsShellcodePtr(Context, Context->Arch.X86.Registers.RegRip, 1))
        {
            return SHEMU_ABORT_RIP_OUTSIDE;
        }

        // Get the offset inside the shellcode buffer.
        rip = Context->Arch.X86.Registers.RegRip - Context->ShellcodeBase;

        // Maximum number of bytes we can fetch. No more than 16 bytes.
        ilen = (ND_UINT32)MIN(Context->ShellcodeSize - rip, sizeof(ibytes));

        // Fetch instruction bytes.
        shstatus = ShemuMemFetch(Context, Context->Arch.X86.Registers.RegRip, ilen, ibytes);
        if (SHEMU_SUCCESS != shstatus)
        {
            return shstatus;
        }

        // Decode the next instruction.
        ndstatus = NdDecodeWithContext(&Context->Arch.X86.Instruction, ibytes, ilen, &decodeCtx);
        if (!ND_SUCCESS(ndstatus))
        {
            if (ND_STATUS_BUFFER_TOO_SMALL == ndstatus)
            {
                return SHEMU_ABORT_RIP_OUTSIDE;
            }
            else
            {
                return SHEMU_ABORT_DECODE_ERROR;
            }
        }

        // Paranoid check...
        if (!ShemuIsShellcodePtr(Context, Context->Arch.X86.Registers.RegRip, Context->Arch.X86.Instruction.Length))
        {
            return SHEMU_ABORT_RIP_OUTSIDE;
        }

        // Check if this is a new, unique, address being executed.
        if (!ShemuShellBmpStateCheck(Context, rip, 1, SHELL_BYTE_FETCHED))
        {
            // If SHEMU_OPT_DIRECT_MAPPED_SHELL is not used, this will be incremented for each instruction.
            Context->UniqueCount++;

            // Note: The first instruction byte is marked using SHELL_BYTE_FETCHED, and subsequent bytes using 
            // SHELL_BYTE_IBYTES. For example, if we have the instruction "33C0 (XOR eax, eax)" at address 0x1000,
            // the flags will be set  as follows:
            // Shellcode state for 0x1000: SHELL_BYTE_FETCHED (0x33, the opcode)
            // Shellcode state for 0x1001: SHELL_BYTE_IBYTES  (0xC0, subsequent bytes)
            // Normally, these two flags should never be set together. 

            // Indicate that we've fetched an instruction from this address.
            ShemuShellBmpStateSet(Context, rip, 1, SHELL_BYTE_FETCHED);

            // Indicate that subsequent bytes are part of a fetched & emulated instruction.
            ShemuShellBmpStateSet(Context, rip + 1, Context->Arch.X86.Instruction.Length - 1, SHELL_BYTE_IBYTES);
        }

        // Check if we just fetched an instruction from a previously written area, to raise self-write alert.
        if (ShemuShellBmpStateCheck(Context, rip, Context->Arch.X86.Instruction.Length, SHELL_BYTE_DIRTY))
        {
            Context->Flags |= SHEMU_FLAG_WRITE_SELF;
        }

        // Dump the context.
        if (Context->Options & SHEMU_OPT_TRACE_EMULATION)
        {
            ShemuX86PrintContext(Context);
        }

        // The RIP is incremented BEFORE actually emulating the instruction. This is what the CPU does as well.
        Context->Arch.X86.Registers.RegRip += Context->Arch.X86.Instruction.Length;

        // Bail out early if we encounter a privileged instruction.
        if (Context->Arch.X86.Instruction.ValidModes.Ring3 == 0 && Context->Arch.X86.Ring == 3)
        {
            return SHEMU_ABORT_NO_PRIVILEGE;
        }

        // Some instruction types are unssuported.
        if (Context->Arch.X86.Instruction.Category == ND_CAT_IO ||
            Context->Arch.X86.Instruction.Category == ND_CAT_IOSTRINGOP)
        {
            return SHEMU_ABORT_INSTRUX_NOT_SUPPORTED;
        }

        // There are two important aspects of a NOP sled:
        // 1. The NOPs must be at the beginning of the code;
        // 2. The NOPs must be consecutive;
        // We will only count consecutive NOPs if they're at the beginning of the code.
        if (Context->Arch.X86.Instruction.Instruction != ND_INS_NOP)
        {
            sled = ND_FALSE;
        }

        // Count the total number of '00 00' (ADD [rax], al) instructions.
        if (Context->Arch.X86.Instruction.InstructionBytes[0] == 0 && 
            Context->Arch.X86.Instruction.InstructionBytes[1] == 0)
        {
            Context->NullCount++;
        }

        // FPU instructions are "pass-through", we just want to save the RIP, so we can emulate FNSTENV.
        if ((Context->Arch.X86.Instruction.IsaSet == ND_SET_X87) && 
            (Context->Arch.X86.Instruction.Instruction != ND_INS_FNSTENV))
        {
            PND_OPERAND pMemOp = ND_NULL;

            Context->Arch.X86.Registers.FpuRip = Context->Arch.X86.Registers.RegRip - Context->Arch.X86.Instruction.Length;

            // If the instruction uses a memory operand, validate it, and bail out if it points outside 
            // shellcode or stack memory.
            if (Context->Arch.X86.Instruction.OperandsCount >= 1 && 
                Context->Arch.X86.Instruction.Operands[0].Type == ND_OP_MEM)
            {
                pMemOp = &Context->Arch.X86.Instruction.Operands[0];
            }
            else if (Context->Arch.X86.Instruction.OperandsCount >= 2 && 
                Context->Arch.X86.Instruction.Operands[1].Type == ND_OP_MEM)
            {
                pMemOp = &Context->Arch.X86.Instruction.Operands[1];
            }

            if (ND_NULL != pMemOp)
            {
                ND_UINT64 gla = ShemuX86ComputeLinearAddress(Context, pMemOp);

                if (!ShemuIsShellcodePtr(Context, gla, pMemOp->Size) &&
                    !ShemuIsStackPtr(Context, gla, pMemOp->Size))
                {
                    stop = ND_TRUE;
                }
            }

            continue;
        }

        // This flag can only be set for APX instructions.
        hasNd = !!Context->Arch.X86.Instruction.HasNd;

        switch (Context->Arch.X86.Instruction.Instruction)
        {
        case ND_INS_FNSTENV:
            if (Context->Arch.X86.Instruction.EfOpMode != ND_OPSZ_16)
            {
                src.Size = Context->Arch.X86.Instruction.Operands[0].Size;
                src.Value.FpuEnvironment.FpuInstructionPointer = (ND_UINT32)Context->Arch.X86.Registers.FpuRip;
                SET_OP(Context, 0, &src);
            }
            break;

        case ND_INS_FXSAVE:
        case ND_INS_FXSAVE64:
            src.Size = MIN(Context->Arch.X86.Instruction.Operands[0].Size, sizeof(src.Value.XsaveArea));
            src.Value.XsaveArea.FpuRip = Context->Arch.X86.Registers.FpuRip;
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_MOV_CR:
            if (Context->Arch.X86.Ring != 0)
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
            if (ShemuX86EvalCondition(Context, Context->Arch.X86.Instruction.Condition))
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
            if (ShemuX86EvalCondition(Context, Context->Arch.X86.Instruction.Condition))
            {
                src.Size = Context->Arch.X86.Instruction.Operands[0].Size;
                src.Value.Qwords[0] = 1;
            }
            else
            {
                src.Size = Context->Arch.X86.Instruction.Operands[0].Size;
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
            GET_OP(Context, hasNd ? 1 : 0, &dst);
            GET_OP(Context, hasNd ? 2 : 1, &src);

            res.Size = src.Size;
            res.Value.Qwords[0] = dst.Value.Qwords[0] + src.Value.Qwords[0];

            if (ND_INS_ADC == Context->Arch.X86.Instruction.Instruction)
            {
                res.Value.Qwords[0] += GET_FLAG(Context, NDR_RFLAG_CF);
            }

            SET_FLAGS(Context, res, dst, src, FM_ADD);
            SET_OP(Context, 0, &res);

            break;

        case ND_INS_SUB:
        case ND_INS_SBB:
        case ND_INS_CMP:
            GET_OP(Context, hasNd ? 1 : 0, &dst);
            GET_OP(Context, hasNd ? 2 : 1, &src);

            res.Size = src.Size;
            res.Value.Qwords[0] = dst.Value.Qwords[0] - src.Value.Qwords[0];

            if (ND_INS_SBB == Context->Arch.X86.Instruction.Instruction)
            {
                res.Value.Qwords[0] -= GET_FLAG(Context, NDR_RFLAG_CF);
            }

            SET_FLAGS(Context, res, dst, src, FM_SUB);

            if (ND_INS_CMP != Context->Arch.X86.Instruction.Instruction)
            {
                SET_OP(Context, 0, &res);
            }

            break;

        case ND_INS_INC:
            GET_OP(Context, hasNd ? 1 : 0, &dst);
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
            GET_OP(Context, hasNd ? 1 : 0, &dst);
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
        case ND_INS_PUSHP:
        case ND_INS_PUSHF:
            GET_OP(Context, 0, &src);
            SET_OP(Context, 1, &src);
            break;

        case ND_INS_POP:
        case ND_INS_POPP:
        case ND_INS_POPF:
            GET_OP(Context, 1, &src);
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_PUSH2:
        case ND_INS_PUSH2P:
            // APX extended EVEX can only be enabled in 64-bit mode, so it's safe to always assume 64-bit regs.
            src.Size = 16;
            src.Value.Qwords[0] = ShemuX86GetGprValue(Context, Context->Arch.X86.Instruction.Operands[1].Info.Register.Reg, 8, ND_FALSE);
            src.Value.Qwords[1] = ShemuX86GetGprValue(Context, Context->Arch.X86.Instruction.Operands[0].Info.Register.Reg, 8, ND_FALSE);
            SET_OP(Context, 2, &src);
            break;

        case ND_INS_POP2:
        case ND_INS_POP2P:
            // I know this looks weird, and you would expect a PUSH2 r1, r2 to have a complementary POP2 r1, r2, but
            // according to the current APX specification, this is not the case. The right pair for PUSH2 r1, r2 is, 
            // in fact, POP2 r2, r1 (the order of the popped operands must be reversed to match the PUSH2).
            GET_OP(Context, 2, &src);
            ShemuX86SetGprValue(Context, Context->Arch.X86.Instruction.Operands[0].Info.Register.Reg, 8, src.Value.Qwords[0], ND_FALSE);
            ShemuX86SetGprValue(Context, Context->Arch.X86.Instruction.Operands[1].Info.Register.Reg, 8, src.Value.Qwords[1], ND_FALSE);
            break;

        case ND_INS_PUSHA:
            src.Size = 16;
            src.Value.Words[7] = (ND_UINT16)ShemuX86GetGprValue(Context, NDR_AX, 2, ND_FALSE);
            src.Value.Words[6] = (ND_UINT16)ShemuX86GetGprValue(Context, NDR_CX, 2, ND_FALSE);
            src.Value.Words[5] = (ND_UINT16)ShemuX86GetGprValue(Context, NDR_DX, 2, ND_FALSE);
            src.Value.Words[4] = (ND_UINT16)ShemuX86GetGprValue(Context, NDR_BX, 2, ND_FALSE);
            src.Value.Words[3] = (ND_UINT16)ShemuX86GetGprValue(Context, NDR_SP, 2, ND_FALSE);
            src.Value.Words[2] = (ND_UINT16)ShemuX86GetGprValue(Context, NDR_BP, 2, ND_FALSE);
            src.Value.Words[1] = (ND_UINT16)ShemuX86GetGprValue(Context, NDR_SI, 2, ND_FALSE);
            src.Value.Words[0] = (ND_UINT16)ShemuX86GetGprValue(Context, NDR_DI, 2, ND_FALSE);
            SET_OP(Context, 1, &src); // Operand 1 is the stack (destination).
            break;

        case ND_INS_PUSHAD:
            src.Size = 32;
            src.Value.Dwords[7] = (ND_UINT32)ShemuX86GetGprValue(Context, NDR_EAX, 4, ND_FALSE);
            src.Value.Dwords[6] = (ND_UINT32)ShemuX86GetGprValue(Context, NDR_ECX, 4, ND_FALSE);
            src.Value.Dwords[5] = (ND_UINT32)ShemuX86GetGprValue(Context, NDR_EDX, 4, ND_FALSE);
            src.Value.Dwords[4] = (ND_UINT32)ShemuX86GetGprValue(Context, NDR_EBX, 4, ND_FALSE);
            src.Value.Dwords[3] = (ND_UINT32)ShemuX86GetGprValue(Context, NDR_ESP, 4, ND_FALSE);
            src.Value.Dwords[2] = (ND_UINT32)ShemuX86GetGprValue(Context, NDR_EBP, 4, ND_FALSE);
            src.Value.Dwords[1] = (ND_UINT32)ShemuX86GetGprValue(Context, NDR_ESI, 4, ND_FALSE);
            src.Value.Dwords[0] = (ND_UINT32)ShemuX86GetGprValue(Context, NDR_EDI, 4, ND_FALSE);
            SET_OP(Context, 1, &src); // Operand 1 is the stack (destination).
            break;

        case ND_INS_POPA:
            GET_OP(Context, 1, &src); // Operand 1 is the stack (source).
            ShemuX86SetGprValue(Context, NDR_AX, 2, src.Value.Words[7], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_CX, 2, src.Value.Words[6], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_DX, 2, src.Value.Words[5], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_BX, 2, src.Value.Words[4], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_BP, 2, src.Value.Words[2], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_SI, 2, src.Value.Words[1], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_DI, 2, src.Value.Words[0], ND_FALSE);
            break;

        case ND_INS_POPAD:
            GET_OP(Context, 1, &src); // Operand 1 is the stack (source).
            ShemuX86SetGprValue(Context, NDR_EAX, 4, src.Value.Dwords[7], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_ECX, 4, src.Value.Dwords[6], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_EDX, 4, src.Value.Dwords[5], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_EBX, 4, src.Value.Dwords[4], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_EBP, 4, src.Value.Dwords[2], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_ESI, 4, src.Value.Dwords[1], ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_EDI, 4, src.Value.Dwords[0], ND_FALSE);
            break;

        case ND_INS_LEA:
            GET_OP(Context, 1, &src);
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_SHL:
        case ND_INS_SAL:
        case ND_INS_SHR:
        case ND_INS_SAR:
            GET_OP(Context, hasNd ? 1 : 0, &dst);
            GET_OP(Context, hasNd ? 2 : 1, &src);

            if (dst.Size == 8)
            {
                src.Value.Qwords[0] &= 0x3f;
            }
            else
            {
                src.Value.Qwords[0] &= 0x1f;
            }

            res.Size = dst.Size;

            if (ND_INS_SHL == Context->Arch.X86.Instruction.Instruction || 
                ND_INS_SAL == Context->Arch.X86.Instruction.Instruction)
            {
                res.Value.Qwords[0] = dst.Value.Qwords[0] << src.Value.Qwords[0];
            }
            else if (ND_INS_SHR == Context->Arch.X86.Instruction.Instruction)
            {
                res.Value.Qwords[0] = dst.Value.Qwords[0] >> src.Value.Qwords[0];
            }
            else
            {
                ND_SINT64 val = (ND_SINT64)ND_SIGN_EX(dst.Size, dst.Value.Qwords[0]);
                val = val >> src.Value.Qwords[0];
                res.Value.Qwords[0] = (ND_UINT64)val;
            }

            if (src.Value.Qwords[0] != 0)
            {
                // 0 bit shifts do not affect the flags.
                if (ND_INS_SHL == Context->Arch.X86.Instruction.Instruction || 
                    ND_INS_SAL == Context->Arch.X86.Instruction.Instruction)
                {
                    SET_FLAGS(Context, res, dst, src, FM_SHL);
                }
                else if (ND_INS_SHR == Context->Arch.X86.Instruction.Instruction)
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
                ND_UINT32 cnt, tempcnt, cntmask, bitwidth;
                ND_UINT8 tempCF = 0;

                GET_OP(Context, hasNd ? 1 : 0, &dst);
                GET_OP(Context, hasNd ? 2 : 1, &src);

                cnt = (ND_UINT32)src.Value.Qwords[0];
                cntmask = ((dst.Size == 8) ? 0x3F : 0x1F);
                tempcnt = (cnt & cntmask);
                bitwidth = (ND_UINT32)dst.Size * 8;

                if (ND_INS_RCL == Context->Arch.X86.Instruction.Instruction || 
                    ND_INS_RCR == Context->Arch.X86.Instruction.Instruction)
                {
                    if (dst.Size == 1)
                    {
                        tempcnt %= 9;
                    }
                    else if (dst.Size == 2)
                    {
                        tempcnt %= 17;
                    }
                }
                else
                {
                    tempcnt %= (dst.Size * 8);
                }

                if (ND_INS_RCL == Context->Arch.X86.Instruction.Instruction)
                {
                    tempCF = GET_FLAG(Context, NDR_RFLAG_CF);

                    if (tempcnt != 0)
                    {
                        // tempcnt is in range [1, dst bit width].
                        ND_UINT64 left = (tempcnt == bitwidth) ? 0 : (dst.Value.Qwords[0] << tempcnt);
                        ND_UINT64 right = (tempcnt == 1) ? 0 : (dst.Value.Qwords[0] >> (bitwidth - tempcnt + 1));

                        SET_FLAG(Context, NDR_RFLAG_CF, ND_GET_BIT(bitwidth - tempcnt, dst.Value.Qwords[0]));

                        dst.Value.Qwords[0] = left | ((ND_UINT64)tempCF << (tempcnt - 1)) | right;
                    }

                    if ((cnt & cntmask) == 1)
                    {
                        SET_FLAG(Context, NDR_RFLAG_OF, ND_MSB(dst.Size, dst.Value.Qwords[0]) ^
                                                        GET_FLAG(Context, NDR_RFLAG_CF));
                    }
                }
                else if (ND_INS_RCR == Context->Arch.X86.Instruction.Instruction)
                {
                    tempCF = GET_FLAG(Context, NDR_RFLAG_CF);

                    if ((cnt & cntmask) == 1)
                    {
                        SET_FLAG(Context, NDR_RFLAG_OF, ND_MSB(dst.Size, dst.Value.Qwords[0]) ^
                                                        GET_FLAG(Context, NDR_RFLAG_CF));
                    }

                    if (tempcnt != 0)
                    {
                        // tempcnt is in range [1, dst bit width].
                        ND_UINT64 left = (tempcnt == bitwidth) ? 0 : (dst.Value.Qwords[0] >> tempcnt);
                        ND_UINT64 right = (tempcnt == 1) ? 0 : (dst.Value.Qwords[0] << (bitwidth - tempcnt + 1));

                        SET_FLAG(Context, NDR_RFLAG_CF, ND_GET_BIT(tempcnt - 1, dst.Value.Qwords[0]));

                        dst.Value.Qwords[0] = left | ((ND_UINT64)tempCF << (bitwidth - tempcnt)) | right;
                    }
                }
                else if (ND_INS_ROL == Context->Arch.X86.Instruction.Instruction)
                {
                    if (tempcnt != 0)
                    {
                        // tempcnt is in range [1, dst bit width - 1].
                        ND_UINT64 left = dst.Value.Qwords[0] << tempcnt;
                        ND_UINT64 right = dst.Value.Qwords[0] >> (bitwidth - tempcnt);

                        dst.Value.Qwords[0] = left | right;
                    }

                    if ((cnt & cntmask) != 0)
                    {
                        SET_FLAG(Context, NDR_RFLAG_CF, ND_LSB(dst.Size, dst.Value.Qwords[0]));
                    }

                    if ((cnt & cntmask) == 1)
                    {
                        SET_FLAG(Context, NDR_RFLAG_OF, ND_MSB(dst.Size, dst.Value.Qwords[0]) ^
                                                        GET_FLAG(Context, NDR_RFLAG_CF));
                    }
                }
                else // ND_INS_ROR
                {
                    if (tempcnt != 0)
                    {
                        // tempcnt is in range [1, dst bit width - 1].
                        ND_UINT64 left = (dst.Value.Qwords[0] >> tempcnt);
                        ND_UINT64 right = (dst.Value.Qwords[0] << (bitwidth - tempcnt));

                        dst.Value.Qwords[0] = left | right;
                    }

                    if ((cnt & cntmask) != 0)
                    {
                        SET_FLAG(Context, NDR_RFLAG_CF, ND_MSB(dst.Size, dst.Value.Qwords[0]));
                    }

                    if ((cnt & cntmask) == 1)
                    {
                        SET_FLAG(Context, NDR_RFLAG_OF, ND_MSB(dst.Size, dst.Value.Qwords[0]) ^ 
                                                        ND_GET_BIT(dst.Size * 8ULL - 2, dst.Value.Qwords[0]));
                    }
                }

                SET_OP(Context, 0, &dst);
            }
            break;

        case ND_INS_OR:
        case ND_INS_XOR:
        case ND_INS_AND:
        case ND_INS_TEST:
            GET_OP(Context, hasNd ? 1 : 0, &dst);
            GET_OP(Context, hasNd ? 2 : 1, &src);

            res.Size = dst.Size;

            if (ND_INS_OR == Context->Arch.X86.Instruction.Instruction)
            {
                res.Value.Qwords[0] = dst.Value.Qwords[0] | src.Value.Qwords[0];
            }
            else if (ND_INS_XOR == Context->Arch.X86.Instruction.Instruction)
            {
                res.Value.Qwords[0] = dst.Value.Qwords[0] ^ src.Value.Qwords[0];
            }
            else
            {
                res.Value.Qwords[0] = dst.Value.Qwords[0] & src.Value.Qwords[0];
            }

            if (ND_INS_TEST != Context->Arch.X86.Instruction.Instruction)
            {
                SET_OP(Context, 0, &res);
            }

            SET_FLAGS(Context, res, dst, src, FM_LOGIC);
            break;

        case ND_INS_NOT:
            GET_OP(Context, hasNd ? 1 : 0, &dst);
            dst.Value.Qwords[0] = ~dst.Value.Qwords[0];
            SET_OP(Context, 0, &dst);
            break;

        case ND_INS_NEG:
            GET_OP(Context, hasNd ? 1 : 0, &src);
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

            if (ND_INS_BTS == Context->Arch.X86.Instruction.Instruction)
            {
                dst.Value.Qwords[0] |= (1ULL << src.Value.Qwords[0]);
            }
            else if (ND_INS_BTR == Context->Arch.X86.Instruction.Instruction)
            {
                dst.Value.Qwords[0] &= ~(1ULL << src.Value.Qwords[0]);
            }
            else if (ND_INS_BTC == Context->Arch.X86.Instruction.Instruction)
            {
                dst.Value.Qwords[0] ^= (1ULL << src.Value.Qwords[0]);
            }

            if (ND_INS_BT != Context->Arch.X86.Instruction.Instruction)
            {
                SET_OP(Context, 0, &dst);
            }

            break;

        case ND_INS_Jcc:
            // Initial condition evaluation.
            taken = ShemuX86EvalCondition(Context, Context->Arch.X86.Instruction.Condition);

            // Track the loop. This may override the taken/not taken state.
            taken = ShemuX86TrackLoop(Context, taken);

            if (taken)
            {
                // Modify the RIP if the branch is taken.
                GET_OP(Context, 1, &res);
                res.Value.Qwords[0] += Context->Arch.X86.Instruction.Operands[0].Info.RelativeOffset.Rel;
                SET_OP(Context, 1, &res);
            }

            break;

        case ND_INS_JrCXZ:
            // Fetch the rCX value. It could be CX, ECX or RCX, depending on address size.
            GET_OP(Context, 1, &dst);

            // Initial condition evaluation.
            taken = dst.Value.Qwords[0] == 0;

            // Track the loop. This may override the taken/not taken state.
            taken = ShemuX86TrackLoop(Context, taken);

            if (taken)
            {
                // Modify the RIP if the branch is taken.
                GET_OP(Context, 2, &res);
                res.Value.Qwords[0] += Context->Arch.X86.Instruction.Operands[0].Info.RelativeOffset.Rel;
                SET_OP(Context, 2, &res);
            }

            break;

        case ND_INS_LOOP:
        case ND_INS_LOOPNZ:
        case ND_INS_LOOPZ:
            // rCX is decremented first. Note that the size depends on address size.
            GET_OP(Context, 1, &dst);
            dst.Value.Qwords[0]--;
            SET_OP(Context, 1, &dst);

            // Initial condition evaluation.
            taken = dst.Value.Qwords[0] > 0 && 
                    (((ND_INS_LOOPNZ == Context->Arch.X86.Instruction.Instruction) && (0 == GET_FLAG(Context, NDR_RFLAG_ZF))) ||
                     ((ND_INS_LOOPZ  == Context->Arch.X86.Instruction.Instruction) && (0 != GET_FLAG(Context, NDR_RFLAG_ZF))) ||
                      (ND_INS_LOOP   == Context->Arch.X86.Instruction.Instruction));

            // Track the loop. This may override the taken/not taken state.
            taken = ShemuX86TrackLoop(Context, taken);

            if (taken)
            {
                // Modify the RIP if the branch is taken.
                GET_OP(Context, 2, &res);
                res.Value.Qwords[0] += Context->Arch.X86.Instruction.Operands[0].Info.RelativeOffset.Rel;
                SET_OP(Context, 2, &res);
            }

            break;

        case ND_INS_JMPNR:
            // Track loops.
            ShemuX86TrackLoop(Context, ND_TRUE);

            GET_OP(Context, 1, &res);
            res.Value.Qwords[0] += Context->Arch.X86.Instruction.Operands[0].Info.RelativeOffset.Rel;
            SET_OP(Context, 1, &res);
            break;

        case ND_INS_JMPNI:
            GET_OP(Context, 0, &src);
            SET_OP(Context, 1, &src);   // Set the RIP to the new value.
            break;

        case ND_INS_CALLNR:
            // Save the EIP on the stack.
            GET_OP(Context, 1, &res);
            SET_OP(Context, 2, &res);
            res.Value.Qwords[0] += Context->Arch.X86.Instruction.Operands[0].Info.RelativeOffset.Rel;
            SET_OP(Context, 1, &res);
            break;

        case ND_INS_CALLNI:
            GET_OP(Context, 0, &src);
            GET_OP(Context, 1, &dst);   // The RIP
            SET_OP(Context, 2, &dst);   // Save the RIP on the stack.
            SET_OP(Context, 1, &src);   // Set the RIP to the new value.
            break;

        case ND_INS_RETN:
            if (!Context->Arch.X86.Instruction.HasImm1)
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
                GET_OP(Context, 2, &res);
                res.Value.Qwords[0] += Context->Arch.X86.Instruction.Operands[0].Info.Immediate.Imm;
                SET_OP(Context, 2, &res);
            }
            break;

        case ND_INS_JMPFD:
        case ND_INS_CALLFD:
            cs = (ND_UINT16)Context->Arch.X86.Instruction.Operands[0].Info.Address.BaseSeg;
            goto check_far_branch;

        case ND_INS_JMPFI:
        case ND_INS_CALLFI:
        case ND_INS_IRET:
        case ND_INS_RETF:
            if (Context->Arch.X86.Instruction.Instruction == ND_INS_RETF)
            {
                if (Context->Arch.X86.Instruction.Operands[0].Type == ND_OP_IMM)
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
            else if (Context->Arch.X86.Instruction.Instruction == ND_INS_IRET)
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
            switch (Context->Arch.X86.Instruction.WordLength)
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
            if (Context->Arch.X86.Mode == ND_CODE_32 && cs == 0x33)
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
                // Get the RCX value.
                GET_OP(Context, 2, &dst);

                if (Context->Arch.X86.Instruction.IsRepeated && (dst.Value.Qwords[0] == 0))
                {
                    break;
                }

                // Load the source into the destination.
                GET_OP(Context, 1, &src);
                SET_OP(Context, 0, &src);

                if (Context->Arch.X86.Instruction.IsRepeated)
                {
                    // Decrement RCX.
                    dst.Value.Qwords[0]--;
                    SET_OP(Context, 2, &dst);
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
                // Get the RCX value.
                GET_OP(Context, 2, &dst);

                if (Context->Arch.X86.Instruction.IsRepeated && (dst.Value.Qwords[0] == 0))
                {
                    break;
                }

                // Move on with the source & destination.
                nd_memzero(&dst, sizeof(dst));

                GET_OP(Context, 0, &dst);
                GET_OP(Context, 1, &src);

                res.Size = dst.Size;
                res.Value.Qwords[0] = dst.Value.Qwords[0] - src.Value.Qwords[0];

                SET_FLAGS(Context, res, dst, src, FM_SUB);

                if (Context->Arch.X86.Instruction.IsRepeated)
                {
                    // Read RCX again & decrement it.
                    nd_memzero(&dst, sizeof(dst));
                    GET_OP(Context, 2, &dst);
                    dst.Value.Qwords[0]--;
                    SET_OP(Context, 2, &dst);

                    if (Context->Arch.X86.Instruction.HasRepRepzXrelease && !GET_FLAG(Context, NDR_RFLAG_ZF))
                    {
                        break;
                    }

                    if (Context->Arch.X86.Instruction.HasRepnzXacquireBnd && GET_FLAG(Context, NDR_RFLAG_ZF))
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
            if (Context->Arch.X86.Instruction.ExpOperandsCount == 1)
            {
                // MUL or IMUL with a single explicit operand.
                GET_OP(Context, 0, &dst);
                GET_OP(Context, 1, &src);
                res.Size = dst.Size * 2;
            }
            else if (Context->Arch.X86.Instruction.ExpOperandsCount == 2)
            {
                // IMUL with 2 explicit operands.
                GET_OP(Context, 0, &dst);
                GET_OP(Context, 1, &src);
                res.Size = dst.Size;
            }
            else
            {
                // IMUL with 3 operands. The first operand is the write-only destination.
                // This also covers the {ND} form, which is equivalent to the 3 operand form.
                GET_OP(Context, 1, &dst);
                GET_OP(Context, 2, &src);
                res.Size = dst.Size;
            }


            if (dst.Size == 1)
            {
                if (ND_INS_MUL == Context->Arch.X86.Instruction.Instruction)
                {
                    res.Value.Words[0] = dst.Value.Bytes[0] * src.Value.Bytes[0];
                }
                else
                {
                    res.Value.Words[0] = (ND_UINT16)((ND_SINT8)dst.Value.Bytes[0] * (ND_SINT8)src.Value.Bytes[0]);
                }
            }
            else if (dst.Size == 2)
            {
                if (ND_INS_MUL == Context->Arch.X86.Instruction.Instruction)
                {
                    res.Value.Dwords[0] = dst.Value.Words[0] * src.Value.Words[0];
                }
                else
                {
                    res.Value.Dwords[0] = (ND_UINT32)((ND_SINT16)dst.Value.Words[0] * (ND_SINT16)src.Value.Words[0]);
                }
            }
            else if (dst.Size == 4)
            {
                if (ND_INS_MUL == Context->Arch.X86.Instruction.Instruction)
                {
                    res.Value.Qwords[0] = dst.Value.Qwords[0] * src.Value.Qwords[0];
                }
                else
                {
                    res.Value.Qwords[0] = (ND_UINT64)((ND_SINT64)(ND_SINT32)dst.Value.Dwords[0] * (ND_SINT64)(ND_SINT32)src.Value.Dwords[0]);
                }
            }
            else
            {
                if (ND_INS_MUL == Context->Arch.X86.Instruction.Instruction)
                {
                    ShemuX86Multiply64Unsigned(&dst, &src, &res);
                }
                else
                {
                    ShemuX86Multiply64Signed(&dst, &src, &res);
                }
            }

            if (Context->Arch.X86.Instruction.ExpOperandsCount == 1)
            {
                // The result is stored in AX, DX:AX, EDX:EAX or RDX:RAX for the single-operand form.
                switch (dst.Size)
                {
                case 1:
                    ShemuX86SetGprValue(Context, NDR_AX, 2, res.Value.Words[0], ND_FALSE);
                    break;
                case 2:
                    ShemuX86SetGprValue(Context, NDR_DX, 2, res.Value.Words[1], ND_FALSE);
                    ShemuX86SetGprValue(Context, NDR_AX, 2, res.Value.Words[0], ND_FALSE);
                    break;
                case 4:
                    ShemuX86SetGprValue(Context, NDR_EDX, 4, res.Value.Dwords[1], ND_FALSE);
                    ShemuX86SetGprValue(Context, NDR_EAX, 4, res.Value.Dwords[0], ND_FALSE);
                    break;
                case 8:
                    ShemuX86SetGprValue(Context, NDR_RDX, 8, res.Value.Qwords[1], ND_FALSE);
                    ShemuX86SetGprValue(Context, NDR_RAX, 8, res.Value.Qwords[0], ND_FALSE);
                    break;
                }
            }
            else
            {
                // The result is truncated and stored in the destination operand for the 2 & 3 operands forms.
                SET_OP(Context, 0, &res);
            }

            // Set the flags.
            if (ND_INS_MUL == Context->Arch.X86.Instruction.Instruction)
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
            // No {ND} form for DIV/IDIV.
            GET_OP(Context, 0, &src);

            if (src.Size == 1)
            {
                ND_UINT16 divident;

                divident = (ND_UINT16)ShemuX86GetGprValue(Context, NDR_AX, 2, ND_FALSE);

                if (ND_INS_DIV == Context->Arch.X86.Instruction.Instruction)
                {
                    if (!ShemuX86CheckDiv(divident, src.Value.Bytes[0], 1))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Bytes[0] = (ND_UINT8)(divident / src.Value.Bytes[0]);
                    res.Value.Bytes[1] = (ND_UINT8)(divident % src.Value.Bytes[0]);
                }
                else
                {
                    if (!ShemuX86CheckIdiv((ND_SINT64)(ND_SINT16)divident, (ND_SINT64)(ND_SINT8)src.Value.Bytes[0], 1))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Bytes[0] = (ND_UINT8)(ND_SINT8)((ND_SINT16)divident / (ND_SINT8)src.Value.Bytes[0]);
                    res.Value.Bytes[1] = (ND_UINT8)(ND_SINT8)((ND_SINT16)divident % (ND_SINT8)src.Value.Bytes[0]);
                }

                // Result in AX (AL - quotient, AH - reminder).
                ShemuX86SetGprValue(Context, NDR_AX, 2, res.Value.Words[0], ND_FALSE);
            }
            else if (src.Size == 2)
            {
                ND_UINT32 divident;

                divident = ((ND_UINT32)ShemuX86GetGprValue(Context, NDR_EDX, 2, ND_FALSE) << 16) |
                            (ND_UINT32)ShemuX86GetGprValue(Context, NDR_EAX, 2, ND_FALSE);

                if (ND_INS_DIV == Context->Arch.X86.Instruction.Instruction)
                {
                    if (!ShemuX86CheckDiv(divident, src.Value.Words[0], 2))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Words[0] = (ND_UINT16)(divident / src.Value.Words[0]);
                    res.Value.Words[1] = (ND_UINT16)(divident % src.Value.Words[0]);
                }
                else
                {
                    if (!ShemuX86CheckIdiv((ND_SINT64)(ND_SINT32)divident, (ND_SINT64)(ND_SINT16)src.Value.Words[0], 2))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Words[0] = (ND_UINT16)(ND_SINT16)((ND_SINT32)divident / (ND_SINT16)src.Value.Words[0]);
                    res.Value.Words[1] = (ND_UINT16)(ND_SINT16)((ND_SINT32)divident % (ND_SINT16)src.Value.Words[0]);
                }

                ShemuX86SetGprValue(Context, NDR_DX, 2, res.Value.Words[1], ND_FALSE);
                ShemuX86SetGprValue(Context, NDR_AX, 2, res.Value.Words[0], ND_FALSE);
            }
            else if (src.Size == 4)
            {
                ND_UINT64 divident;

                divident = ((ND_UINT64)ShemuX86GetGprValue(Context, NDR_EDX, 4, ND_FALSE) << 32) |
                            (ND_UINT64)ShemuX86GetGprValue(Context, NDR_EAX, 4, ND_FALSE);

                if (ND_INS_DIV == Context->Arch.X86.Instruction.Instruction)
                {
                    if (!ShemuX86CheckDiv(divident, src.Value.Dwords[0], 4))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Dwords[0] = (ND_UINT32)(divident / src.Value.Dwords[0]);
                    res.Value.Dwords[1] = (ND_UINT32)(divident % src.Value.Dwords[0]);
                }
                else
                {
                    if (!ShemuX86CheckIdiv((ND_SINT64)divident, (ND_SINT64)(ND_SINT32)src.Value.Dwords[0], 4))
                    {
                        stop = ND_TRUE;
                        break;
                    }

                    res.Value.Dwords[0] = (ND_UINT32)(ND_SINT32)((ND_SINT64)divident / (ND_SINT32)src.Value.Dwords[0]);
                    res.Value.Dwords[1] = (ND_UINT32)(ND_SINT32)((ND_SINT64)divident % (ND_SINT32)src.Value.Dwords[0]);
                }

                ShemuX86SetGprValue(Context, NDR_EDX, 4, res.Value.Dwords[1], ND_FALSE);
                ShemuX86SetGprValue(Context, NDR_EAX, 4, res.Value.Dwords[0], ND_FALSE);
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
            Context->Arch.X86.Registers.RegFlags ^= NDR_RFLAG_CF;
            break;

        case ND_INS_STI:
            if (Context->Arch.X86.Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            SET_FLAG(Context, NDR_RFLAG_IF, 1);
            break;

        case ND_INS_CLI:
            if (Context->Arch.X86.Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            SET_FLAG(Context, NDR_RFLAG_IF, 0);
            break;

        case ND_INS_SAHF:
            {
                ND_UINT8 ah = (ND_UINT8)ShemuX86GetGprValue(Context, NDR_AH, 1, ND_TRUE);
                // Handle reserved bits.
                ah |= (1 << 1);
                ah &= ~((1 << 3) | (1 << 5));
                ((ND_UINT8 *)&Context->Arch.X86.Registers.RegFlags)[0] = ah;
            }
            break;

        case ND_INS_LAHF:
            {
                ND_UINT8 ah = ((ND_UINT8 *)&Context->Arch.X86.Registers.RegFlags)[0];
                ShemuX86SetGprValue(Context, NDR_AH, 1, ah, ND_TRUE);
            }
            break;

        case ND_INS_SALC:
            if (GET_FLAG(Context, NDR_RFLAG_CF))
            {
                ShemuX86SetGprValue(Context, NDR_AL, 1, 0xFF, ND_FALSE);
            }
            else
            {
                ShemuX86SetGprValue(Context, NDR_AL, 1, 0x00, ND_FALSE);
            }
            break;

        case ND_INS_NOP:
            if (sled)
            {
                Context->NopCount++;
            }
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

        case ND_INS_BSWAP:
            GET_OP(Context, 0, &src);

            dst.Size = src.Size;

            switch (src.Size)
            {
            case 2:
                // Although undefined, when executing BSWAP with 16 bit operands, the result is set to 0.
                dst.Value.Words[0] = 0;
                break;
            case 4:
                dst.Value.Bytes[3] = src.Value.Bytes[0];
                dst.Value.Bytes[2] = src.Value.Bytes[1];
                dst.Value.Bytes[1] = src.Value.Bytes[2];
                dst.Value.Bytes[0] = src.Value.Bytes[3];
                break;
            case 8:
                dst.Value.Bytes[7] = src.Value.Bytes[0];
                dst.Value.Bytes[6] = src.Value.Bytes[1];
                dst.Value.Bytes[5] = src.Value.Bytes[2];
                dst.Value.Bytes[4] = src.Value.Bytes[3];
                dst.Value.Bytes[3] = src.Value.Bytes[4];
                dst.Value.Bytes[2] = src.Value.Bytes[5];
                dst.Value.Bytes[1] = src.Value.Bytes[6];
                dst.Value.Bytes[0] = src.Value.Bytes[7];
            default:
                break;
            }

            SET_OP(Context, 0, &dst);
            break;

        case ND_INS_BSF:
        case ND_INS_BSR:
            // No APX form.
            GET_OP(Context, 1, &src);

            if (src.Value.Qwords[0] == 0)
            {
                SET_FLAG(Context, NDR_RFLAG_ZF, 1);
            }
            else
            {
                SET_FLAG(Context, NDR_RFLAG_ZF, 0);

                dst.Size = src.Size;
                dst.Value.Qwords[0] = ShemuX86CountZeroBits(src.Value.Qwords[0], 
                                                            src.Size, 
                                                            Context->Arch.X86.Instruction.Instruction == ND_INS_BSF);

                if (Context->Arch.X86.Instruction.Instruction == ND_INS_BSR)
                {
                    dst.Value.Qwords[0] = src.Size * 8ULL - dst.Value.Qwords[0] - 1;
                }

                SET_OP(Context, 0, &dst);
            }
            break;

        case ND_INS_POPCNT:
            // No {ND} form.
            GET_OP(Context, 1, &src);

            dst.Size = src.Size;
            dst.Value.Qwords[0] = 0;

            for (ND_UINT32 bit = 0; bit < src.Size * 8; bit++)
            {
                if (ND_GET_BIT(bit, src.Value.Qwords[0]))
                {
                    dst.Value.Qwords[0]++;
                }
            }

            SET_FLAG(Context, NDR_RFLAG_OF, 0);
            SET_FLAG(Context, NDR_RFLAG_SF, 0);
            SET_FLAG(Context, NDR_RFLAG_ZF, 0);
            SET_FLAG(Context, NDR_RFLAG_CF, 0);
            SET_FLAG(Context, NDR_RFLAG_PF, 0);
            SET_FLAG(Context, NDR_RFLAG_ZF, src.Value.Qwords[0] == 0 ? 1 : 0);

            SET_OP(Context, 0, &dst);
            break;

        case ND_INS_LZCNT:
        case ND_INS_TZCNT:
            // No {ND} form.
            GET_OP(Context, 1, &src);

            dst.Size = src.Size;
            dst.Value.Qwords[0] = ShemuX86CountZeroBits(src.Value.Qwords[0], 
                                                        src.Size, 
                                                        Context->Arch.X86.Instruction.Instruction == ND_INS_TZCNT);

            // Set CF.
            if (dst.Value.Qwords[0] == src.Size * 8ULL)
            {
                SET_FLAG(Context, NDR_RFLAG_CF, 1);
            }
            else
            {
                SET_FLAG(Context, NDR_RFLAG_CF, 0);
            }

            // Set ZF.
            if (dst.Value.Qwords[0] == 0)
            {
                SET_FLAG(Context, NDR_RFLAG_ZF, 1);
            }
            else
            {
                SET_FLAG(Context, NDR_RFLAG_ZF, 0);
            }

            // Set the result.
            SET_OP(Context, 0, &dst);
            break;

        case ND_INS_SHLD:
        case ND_INS_SHRD:
            {
                SHEMU_VALUE cnt = { 0 };

                GET_OP(Context, hasNd ? 1 : 0, &dst);
                GET_OP(Context, hasNd ? 2 : 1, &src);
                GET_OP(Context, hasNd ? 3 : 2, &cnt);

                res.Size = dst.Size;

                if (dst.Size == 8)
                {
                    cnt.Value.Qwords[0] &= 0x3f;
                }
                else
                {
                    cnt.Value.Qwords[0] &= 0x1f;
                }

                if (cnt.Value.Qwords[0] == 0)
                {
                    // 0-shift, destination & flags are not affected.
                }
                else if (cnt.Value.Qwords[0] > dst.Size * 8ull)
                {
                    // Shift count larger that operand size, destination & flags are undefined. Store 0 in destination.
                    SET_OP(Context, 0, &res);
                }
                else
                {
                    // Positive shift, count less than operand size.
                    if (ND_INS_SHLD == Context->Arch.X86.Instruction.Instruction)
                    {
                        res.Value.Qwords[0] = dst.Value.Qwords[0] << cnt.Value.Qwords[0];

                        res.Value.Qwords[0] |= src.Value.Qwords[0] >> (dst.Size * 8ull - cnt.Value.Qwords[0]);

                        SET_FLAGS(Context, res, dst, cnt, FM_SHL);
                    }
                    else
                    {
                        res.Value.Qwords[0] = dst.Value.Qwords[0] >> cnt.Value.Qwords[0];

                        res.Value.Qwords[0] |= src.Value.Qwords[0] << (dst.Size * 8ull - cnt.Value.Qwords[0]);

                        SET_FLAGS(Context, res, dst, cnt, FM_SHR);
                    }

                    SET_OP(Context, 0, &res);
                }
            }
            break;

        case ND_INS_PREFETCH:
        case ND_INS_PREFETCHE:
        case ND_INS_PREFETCHM:
        case ND_INS_PREFETCHNTA:
        case ND_INS_PREFETCHT0:
        case ND_INS_PREFETCHT1:
        case ND_INS_PREFETCHT2:
        case ND_INS_PREFETCHW:
        case ND_INS_PREFETCHWT1:
            // Act as NOPs, they're just hints to the hardware prefetchers.
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
            ShemuX86SetGprValue(Context, NDR_RAX, 8, 0, ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_RCX, 8, 0, ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_RDX, 8, 0, ND_FALSE);
            ShemuX86SetGprValue(Context, NDR_RBX, 8, 0, ND_FALSE);
            break;

        // Some basic MMX/SSE instructions supported.
        case ND_INS_EMMS:
            nd_memzero(Context->Arch.X86.MmxRegisters, sizeof(Context->Arch.X86.MmxRegisters));
            break;

        case ND_INS_MOVD:
        case ND_INS_MOVQ:
        case ND_INS_MOVDQU:
        case ND_INS_MOVDQA:
            // If the source size is less than the destination size, the upper bits will be zero.
            // Note that we don't really care about #GP on unaligned MOVDQA accesses...
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
            dst.Size = Context->Arch.X86.Instruction.Operands[0].Size;
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
            if ((Context->Arch.X86.Instruction.Immediate1 == 0x80 ||
                 Context->Arch.X86.Instruction.Immediate1 == 0x2E) &&
                Context->Arch.X86.Registers.RegRax < 0x1000)
            {
                Context->Flags |= SHEMU_FLAG_SYSCALL;
            }

            // Fall through
        case ND_INS_INT1:
        case ND_INS_INT3:
        case ND_INS_INTO:
            stop = ND_TRUE;
            break;

        case ND_INS_SYSCALL:
        case ND_INS_SYSENTER:
            if (Context->Arch.X86.Registers.RegRax < 0x1000)
            {
                Context->Flags |= SHEMU_FLAG_SYSCALL;
            }
            stop = ND_TRUE;
            break;

        // Some basic privileged instructions supported, specific to kernel-mode shellcodes.
        case ND_INS_SWAPGS:
            if (Context->Arch.X86.Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            Context->Flags |= SHEMU_FLAG_SWAPGS;
            stop = ND_TRUE;
            break;

        case ND_INS_RDMSR:
            if (Context->Arch.X86.Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            // Fetch ECX value.
            GET_OP(Context, 2, &src);

            if ((src.Value.Dwords[0] == 0xC0000082 && ND_CODE_64 == Context->Arch.X86.Mode) ||
                (src.Value.Dwords[0] == 0x00000176 && ND_CODE_32 == Context->Arch.X86.Mode))
            {
                Context->Flags |= SHEMU_FLAG_SYSCALL_MSR_READ;
            }

            stop = ND_TRUE;
            break;

        case ND_INS_WRMSR:
            if (Context->Arch.X86.Ring != 0)
            {
                return SHEMU_ABORT_NO_PRIVILEGE;
            }

            // Fetch ECX value.
            GET_OP(Context, 2, &src);

            if ((src.Value.Dwords[0] == 0xC0000082 && ND_CODE_64 == Context->Arch.X86.Mode) ||
                (src.Value.Dwords[0] == 0x00000176 && ND_CODE_32 == Context->Arch.X86.Mode))
            {
                Context->Flags |= SHEMU_FLAG_SYSCALL_MSR_WRITE;
            }

            stop = ND_TRUE;
            break;

        case ND_INS_SIDT:
            if (Context->Arch.X86.Ring == 0)
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
                break;
            }

            GET_OP(Context, 0, &dst);
            GET_OP(Context, 1, &src);

            shemu_memcpy(&val, &dst, 16);
            shemu_memcpy(&key, &src, 16);

            if (Context->Arch.X86.Instruction.Instruction == ND_INS_AESDEC)
            {
                val = _mm_aesdec_si128(val, key);
            }
            else if (Context->Arch.X86.Instruction.Instruction == ND_INS_AESDECLAST)
            {
                val = _mm_aesdeclast_si128(val, key);
            }
            else if (Context->Arch.X86.Instruction.Instruction == ND_INS_AESIMC)
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

        case ND_INS_RDFSBASE:
        case ND_INS_RDGSBASE:
            src.Size = Context->Arch.X86.Instruction.Operands[0].Size;
            src.Value.Qwords[0] = Context->TibBase;
            SET_OP(Context, 0, &src);
            break;

        case ND_INS_UD0:
        case ND_INS_UD1:
        case ND_INS_UD2:
            stop = ND_TRUE;
            break;

        default:
            return SHEMU_ABORT_INSTRUX_NOT_SUPPORTED;

            break;
        }
    }

    // Minimum percent of the instructions were NOPs => consider we have a NOP sled. Note that we get here only if
    // the maximum number of instructions has been emulated successfully; if the emulation is aborted for any reason,
    // this code will have no effect.
    if ((Context->InstructionsCount >= Context->MaxInstructionsCount / 2) && 
        (Context->NopCount >= Context->InstructionsCount * Context->NopThreshold / 100))
    {
        Context->Flags |= SHEMU_FLAG_NOP_SLED;
    }

    return SHEMU_SUCCESS;
}
