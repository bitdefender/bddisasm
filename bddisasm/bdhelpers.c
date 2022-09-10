/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include "include/nd_crt.h"
#include "../inc/bddisasm.h"


//
// NdIsInstruxRipRelative
//
ND_BOOL
NdIsInstruxRipRelative(
    const INSTRUX *Instrux
    )
//
// Provided for backwards compatibility with existing code that uses disasm 1.0
//
{
    if (ND_NULL == Instrux)
    {
        return ND_FALSE;
    }
    else
    {
        return Instrux->IsRipRelative;
    }
}


//
// NdGetFullAccessMap
//
NDSTATUS
NdGetFullAccessMap(
    const INSTRUX *Instrux,
    ND_ACCESS_MAP *AccessMap
    )
{
    ND_UINT32 i;
    const ND_OPERAND *pOp;

    // pre-init
    i = 0;
    pOp = (const ND_OPERAND *)ND_NULL;

    // validate
    if (ND_NULL == Instrux)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (ND_NULL == AccessMap)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    for (i = 0; i < Instrux->OperandsCount; i++)
    {
        pOp = &Instrux->Operands[i];

        if (ND_OP_MEM == pOp->Type)
        {
            if (pOp->Info.Memory.IsStack)
            {
                AccessMap->StackAccess |= pOp->Access.Access;
                AccessMap->GprAccess[NDR_RSP] |= ND_ACCESS_READ|ND_ACCESS_WRITE;
                AccessMap->SegAccess[NDR_SS] |= ND_ACCESS_READ;
            }
            else
            {
                AccessMap->MemAccess |= pOp->Access.Access;

                if (pOp->Info.Memory.HasSeg)
                {
                    AccessMap->SegAccess[pOp->Info.Memory.Seg] |= ND_ACCESS_READ;
                }

                if (pOp->Info.Memory.HasBase)
                {
                    AccessMap->GprAccess[pOp->Info.Memory.Base] |= ND_ACCESS_READ;
                }

                if (pOp->Info.Memory.HasIndex)
                {
                    if (pOp->Info.Memory.IsVsib)
                    {
                        AccessMap->SseAccess[pOp->Info.Memory.Index] |= ND_ACCESS_READ;
                    }
                    else
                    {
                        AccessMap->GprAccess[pOp->Info.Memory.Index] |= ND_ACCESS_READ;
                    }
                }
            }
        }
        else if (ND_OP_REG == pOp->Type)
        {
            switch (pOp->Info.Register.Type)
            {
            case ND_REG_GPR:
                {
                    ND_UINT32 k;

                    for (k = 0; k < pOp->Info.Register.Count; k++)
                    {
                        if (pOp->Info.Register.IsHigh8)
                        {
                            AccessMap->GprAccess[pOp->Info.Register.Reg - 4 + k] |= pOp->Access.Access;
                        }
                        else
                        {
                            AccessMap->GprAccess[pOp->Info.Register.Reg + k] |= pOp->Access.Access;
                        }
                    }
                }
                break;
            case ND_REG_SEG:
                AccessMap->SegAccess[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            case ND_REG_FPU:
                AccessMap->FpuAccess[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            case ND_REG_MMX:
                AccessMap->MmxAccess[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            case ND_REG_SSE:
                {
                    ND_UINT32 k;

                    for (k = 0; k < pOp->Info.Register.Count; k++)
                    {
                        AccessMap->SseAccess[pOp->Info.Register.Reg + k] |= pOp->Access.Access;
                    }
                }
                break;
            case ND_REG_CR:
                AccessMap->CrAccess[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            case ND_REG_DR:
                AccessMap->DrAccess[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            case ND_REG_TR:
                AccessMap->TrAccess[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            case ND_REG_BND:
                AccessMap->BndAccess[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            case ND_REG_MSK:
                AccessMap->MskAccess[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            case ND_REG_SYS:
                AccessMap->SysAccess[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            case ND_REG_X87:
                AccessMap->X87Access[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            case ND_REG_FLG:
                AccessMap->FlagsAccess |= pOp->Access.Access;
                break;
            case ND_REG_RIP:
                AccessMap->RipAccess |= pOp->Access.Access;
                break;
            case ND_REG_MXCSR:
                AccessMap->MxcsrAccess |= pOp->Access.Access;
                break;
            case ND_REG_PKRU:
                AccessMap->PkruAccess |= pOp->Access.Access;
                break;
            case ND_REG_SSP:
                AccessMap->SspAccess |= pOp->Access.Access;
                break;
            case ND_REG_TILE:
                AccessMap->TmmAccess[pOp->Info.Register.Reg] |= pOp->Access.Access;
                break;
            default:
                break;
            }
        }
        else if (ND_OP_BANK == Instrux->Operands[i].Type)
        {
            ND_UINT8 j;

            // Bank registers access. This needs special handling. Note that LOADALL/LOADALLD is not supported, as
            // it is too old and it's not valid since the good old 486.
            if (ND_INS_FNSAVE == Instrux->Instruction)
            {
                for (j = 0; j < ND_MAX_FPU_REGS; j++)
                {
                    AccessMap->FpuAccess[j] |= ND_ACCESS_READ;
                }
            }
            else if (ND_INS_FRSTOR == Instrux->Instruction)
            {
                for (j = 0; j < ND_MAX_FPU_REGS; j++)
                {
                    AccessMap->FpuAccess[j] |= ND_ACCESS_WRITE;
                }
            }

            if ((ND_INS_XSAVE == Instrux->Instruction)  || (ND_INS_XSAVEOPT == Instrux->Instruction) ||
                (ND_INS_XSAVES == Instrux->Instruction) || (ND_INS_XSAVEC == Instrux->Instruction))
            {
                for (j = 0; j < ND_MAX_SSE_REGS; j++)
                {
                    AccessMap->SseAccess[j] |= ND_ACCESS_READ;
                }
            }
            else if ((ND_INS_XRSTOR == Instrux->Instruction) || (ND_INS_XRSTORS == Instrux->Instruction))
            {
                for (j = 0; j < ND_MAX_SSE_REGS; j++)
                {
                    AccessMap->SseAccess[j] |= ND_ACCESS_WRITE;
                }
            }
        }
    }

    return ND_STATUS_SUCCESS;
}


//
// NdGetOperandRlut
//
NDSTATUS
NdGetOperandRlut(
    const INSTRUX *Instrux,
    ND_OPERAND_RLUT *Rlut
    )
{
    if (ND_NULL == Instrux)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (ND_NULL == Rlut)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    // Initialize the RLUT. 
    nd_memset(Rlut, 0, sizeof(*Rlut));

    for (ND_UINT8 i = 0; i < Instrux->OperandsCount; i++)
    {
        if (!!(Instrux->Operands[i].Access.Access & ND_ACCESS_ANY_WRITE))
        {
            // We only care about the first 2 destination operands.
            if (Rlut->Dst1 == ND_NULL)
            {
                Rlut->Dst1 = (PND_OPERAND)&Instrux->Operands[i];
            }
            else if (Rlut->Dst2 == ND_NULL)
            {
                Rlut->Dst2 = (PND_OPERAND)&Instrux->Operands[i];
            }
        }

        if (!!(Instrux->Operands[i].Access.Access & ND_ACCESS_ANY_READ))
        {
            // We only care about the first 4 source operands.
            if (Rlut->Src1 == ND_NULL)
            {
                Rlut->Src1 = (PND_OPERAND)&Instrux->Operands[i];
            }
            else if (Rlut->Src2 == ND_NULL)
            {
                Rlut->Src2 = (PND_OPERAND)&Instrux->Operands[i];
            }
            else if (Rlut->Src3 == ND_NULL)
            {
                Rlut->Src3 = (PND_OPERAND)&Instrux->Operands[i];
            }
            else if (Rlut->Src4 == ND_NULL)
            {
                Rlut->Src4 = (PND_OPERAND)&Instrux->Operands[i];
            }
        }

        if (Instrux->Operands[i].Type == ND_OP_MEM)
        {
            // We only care about the first 2 memory operands.
            if (Rlut->Mem1 == ND_NULL)
            {
                Rlut->Mem1 = (PND_OPERAND)&Instrux->Operands[i];
            }
            else if (Rlut->Mem2 == ND_NULL)
            {
                Rlut->Mem2 = (PND_OPERAND)&Instrux->Operands[i];
            }

            if (Instrux->Operands[i].Info.Memory.IsStack)
            {
                Rlut->Stack = (PND_OPERAND)&Instrux->Operands[i];
            }
        }

        if (Instrux->Operands[i].Type == ND_OP_REG && Instrux->Operands->Flags.IsDefault)
        {
            switch (Instrux->Operands[i].Info.Register.Type)
            {
            case ND_REG_FLG:
                Rlut->Flags = (PND_OPERAND)&Instrux->Operands[i];
                break;
            case ND_REG_RIP:
                Rlut->Rip = (PND_OPERAND)&Instrux->Operands[i];;
                break;
            case ND_REG_SEG:
                if (Instrux->Operands[i].Info.Register.Reg == NDR_CS)
                {
                    Rlut->Cs = (PND_OPERAND)&Instrux->Operands[i];
                }
                else if (Instrux->Operands[i].Info.Register.Reg == NDR_SS)
                {
                    Rlut->Ss = (PND_OPERAND)&Instrux->Operands[i];
                }
                break;
            case ND_REG_GPR:
                if (Instrux->Operands[i].Info.Register.Reg < 8)
                {
                    *(&Rlut->Rax + Instrux->Operands[i].Info.Register.Reg) = (PND_OPERAND)&Instrux->Operands[i];
                }
                break;
            default:
                break;
            }
        }
    }

    return ND_STATUS_SUCCESS;
}