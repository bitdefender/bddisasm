/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include "include/nd_crt.h"
#include "../inc/bddisasm.h"



#ifndef BDDISASM_NO_FORMAT

static const char *gReg8Bit[] =
{
    "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
};

static const char *gReg8Bit64[] =
{
    "al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil",
    "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b",
};

static const char *gReg16Bit[] =
{
    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di",
    "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w",
};

static const char *gReg32Bit[] =
{
    "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi",
    "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d",
};

static const char *gReg64Bit[] =
{
    "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
    "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15"
};

static const char *gRegFpu[] =
{
    "st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7",
};

static const char *gRegMmx[] =
{
    "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7",
};

static const char *gRegControl[] =
{
    "cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7",
    "cr8", "cr9", "cr10", "cr11", "cr12", "cr13", "cr14", "cr15",
};

static const char *gRegDebug[] =
{
    "dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7",
    "dr8", "dr9", "dr10", "dr11", "dr12", "dr13", "dr14", "dr15",
};

static const char *gRegTest[] =
{
    "tr0", "tr1", "tr2", "tr3", "tr4", "tr5", "tr6", "tr7",
    "tr8", "tr9", "tr10", "tr11", "tr12", "tr13", "tr14", "tr15",
};

static const char *gRegXmm[] =
{
    "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7",
    "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15",
    "xmm16", "xmm17", "xmm18", "xmm19", "xmm20", "xmm21", "xmm22", "xmm23",
    "xmm24", "xmm25", "xmm26", "xmm27", "xmm28", "xmm29", "xmm30", "xmm31",
};

static const char *gRegYmm[] =
{
    "ymm0", "ymm1", "ymm2", "ymm3", "ymm4", "ymm5", "ymm6", "ymm7",
    "ymm8", "ymm9", "ymm10", "ymm11", "ymm12", "ymm13", "ymm14", "ymm15",
    "ymm16", "ymm17", "ymm18", "ymm19", "ymm20", "ymm21", "ymm22", "ymm23",
    "ymm24", "ymm25", "ymm26", "ymm27", "ymm28", "ymm29", "ymm30", "ymm31"
};

static const char *gRegZmm[] =
{
    "zmm0", "zmm1", "zmm2", "zmm3", "zmm4", "zmm5", "zmm6", "zmm7",
    "zmm8", "zmm9", "zmm10", "zmm11", "zmm12", "zmm13", "zmm14", "zmm15",
    "zmm16", "zmm17", "zmm18", "zmm19", "zmm20", "zmm21", "zmm22", "zmm23",
    "zmm24", "zmm25", "zmm26", "zmm27", "zmm28", "zmm29", "zmm30", "zmm31",
};

static const char *gRegSeg[] =
{
    "es", "cs", "ss", "ds", "fs", "gs", "segr6", "segr7",
};

static const char *gRegBound[] =
{
    "bnd0", "bnd1", "bnd2", "bnd3",
};

static const char *gRegMask[] =
{
    "k0", "k1", "k2", "k3", "k4", "k5", "k6", "k7",
};

static const char *gRegTile[] =
{
    "tmm0", "tmm1", "tmm2", "tmm3", "tmm4", "tmm5", "tmm6", "tmm7",
};

static const char *gConditionCodes[] =
{
    "EQ", "LT", "LE", "UNORD", "NEQ", "NLT", "NLE", "ORD",
    "EQ_UQ", "NGE", "NGT", "ND_FALSE", "NEQ_OQ", "GE", "GT", "TRUE",
    "EQ_OS", "LT_OQ", "LE_OQ", "UNORD_S", "NEQ_US", "NLT_UQ", "NLE_UQ", "ORD_S",
    "EQ_US", "NGE_UQ", "NGT_UQ", "FALSE_OS", "NEQ_OS", "GE_OQ", "GT_OQ", "TRUE_US",
};

static const char *gEmbeddedRounding[] =
{
    "rn", "rd", "ru", "rz",
};



//
// NdSprintf
//
static NDSTATUS
NdSprintf(
    char *Destination,
    ND_SIZET DestinationSize,
    const char *Formatstring,
    ...
    )
//
// Wrapper on vsnprintf.
//
{
    int res;
    va_list args;

    if (ND_NULL == Destination)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (ND_NULL == Formatstring)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    nd_memzero(Destination, DestinationSize);

    va_start(args, Formatstring);

    // _vsnprintf is used instead of the more secure _vsnprintf_s because the mini-Petru supports just
    // the unsecured version, and we depend on it.
    res = nd_vsnprintf_s(Destination, DestinationSize, DestinationSize - 1, Formatstring, args);

    va_end(args);

    if ((res < 0) || ((ND_SIZET)res >= DestinationSize - 1))
    {
        return ND_STATUS_BUFFER_OVERFLOW;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdToText
//
NDSTATUS
NdToText(
    const INSTRUX *Instrux,
    ND_UINT64 Rip,
    ND_UINT32 BufferSize,
    char *Buffer
    )
{
    NDSTATUS status;
    char *res, temp[64];
    ND_UINT32 opIndex, opsStored;
    const ND_OPERAND *pOp;
    ND_BOOL alignmentStored;

    // pre-init
    status = ND_STATUS_SUCCESS;
    res = (char *)ND_NULL;
    opIndex = 0;
    opsStored = 0;
    pOp = (const ND_OPERAND *)ND_NULL;
    alignmentStored = ND_FALSE;

    // Validate args.
    if (ND_NULL == Instrux)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (ND_NULL == Buffer)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (BufferSize < ND_MIN_BUF_SIZE)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    // init the text.
    nd_memzero(Buffer, BufferSize);
    nd_memzero(temp, sizeof(temp));

    // First, store the prefixes.
    if (Instrux->Rep != 0)
    {
        // Check for REPZ/REPNZ support, and store prefixes.
        if (ND_REPC_SUPPORT(Instrux))
        {
            if (Instrux->Rep == ND_PREFIX_G1_REPE_REPZ)
            {
                res = nd_strcat_s(Buffer, BufferSize, "REPZ ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            else if (Instrux->Rep == ND_PREFIX_G1_REPNE_REPNZ)
            {
                res = nd_strcat_s(Buffer, BufferSize, "REPNZ ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
        }

        // Check for REP support and store prefixes.
        if (ND_REP_SUPPORT(Instrux))
        {
            if (Instrux->Rep == ND_PREFIX_G1_REPE_REPZ)
            {
                res = nd_strcat_s(Buffer, BufferSize, "REP ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            else if (Instrux->Rep == ND_PREFIX_G1_REPNE_REPNZ)
            {
                res = nd_strcat_s(Buffer, BufferSize, "REPNZ ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
        }

        if (Instrux->IsXreleaseEnabled)
        {
            res = nd_strcat_s(Buffer, BufferSize, "XRELEASE ");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
        else if (Instrux->IsXacquireEnabled)
        {
            res = nd_strcat_s(Buffer, BufferSize, "XACQUIRE ");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
    }

    if (Instrux->HasLock)
    {
        if (ND_LOCK_SUPPORT(Instrux))
        {
            res = nd_strcat_s(Buffer, BufferSize, "LOCK ");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
    }

    if (Instrux->Rep == ND_PREFIX_G1_BND)
    {
        if (ND_BND_SUPPORT(Instrux))
        {
            res = nd_strcat_s(Buffer, BufferSize, "BND ");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
    }

    if (Instrux->HasSeg && ND_BHINT_SUPPORT(Instrux))
    {
        switch (Instrux->Seg)
        {
        case ND_PREFIX_G2_BR_TAKEN:
            res = nd_strcat_s(Buffer, BufferSize, "BHT ");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            break;

        case ND_PREFIX_G2_BR_NOT_TAKEN:
            res = nd_strcat_s(Buffer, BufferSize, "BHNT ");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            break;

        case ND_PREFIX_G2_BR_ALT:
            res = nd_strcat_s(Buffer, BufferSize, "BHALT ");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            break;

        default:
            break;
        }
    }

    if (Instrux->HasSeg && ND_DNT_SUPPORT(Instrux))
    {
        if (!Instrux->IsCetTracked)
        {
            res = nd_strcat_s(Buffer, BufferSize, "DNT ");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
    }

    // Store the mnemonic.
    res = nd_strcat_s(Buffer, BufferSize, Instrux->Mnemonic);
    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);

    // Store condition code, if any.
    if (ND_HAS_SSE_CONDITION(Instrux))
    {
        res = nd_strcat_s(Buffer, BufferSize, gConditionCodes[Instrux->SseCondition]);
        RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
    }

    // If there are no explicit operands, we can leave.
    if (0 == Instrux->ExpOperandsCount)
    {
        return ND_STATUS_SUCCESS;
    }

    // Now the operands.
    for (opIndex = 0; opIndex < Instrux->OperandsCount; opIndex++)
    {
        status = ND_STATUS_SUCCESS;

        pOp = &Instrux->Operands[opIndex];

        if (pOp->Type == ND_OP_NOT_PRESENT)
        {
            break;
        }

        if (pOp->Flags.IsDefault)
        {
            continue;
        }

        // If this is a mask operand that has been used as masking for a previous operand, than we
        // can safely skip it. We check this by seeing where is the operand encoded. If it's encoded
        // in the evex.aaa field, than it is a conventional mask.
        if ((pOp->Encoding == ND_OPE_A) && (pOp->Type == ND_OP_REG) &&
            (pOp->Info.Register.Type == ND_REG_MSK) && (opIndex > 0))
        {
            continue;
        }

        // Store alignment.
        if (!alignmentStored)
        {
            ND_SIZET idx = 0;

            while ((idx < BufferSize) && (Buffer[idx]))
            {
                idx++;
            }

            while ((idx < 9) && (idx + 1 < BufferSize))
            {
                Buffer[idx++] = 0x20;
            }

            if (idx + 1 < BufferSize)
            {
                Buffer[idx++] = 0x20;
            }

            Buffer[idx] = 0;

            alignmentStored = ND_TRUE;
        }

        // Store the comma, if this isn't the first operand.
        if (opsStored > 0)
        {
            res = nd_strcat_s(Buffer, BufferSize, ", ");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }

        opsStored++;

        switch (pOp->Type)
        {
        case ND_OP_REG:
            switch (pOp->Info.Register.Type)
            {
            case ND_REG_GPR:
            {
                if (pOp->Info.Register.Reg >= ND_MAX_GPR_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                // General purpose register.
                switch (pOp->Info.Register.Size)
                {
                case ND_SIZE_8BIT:
                    // 8 bit register.
                    if ((Instrux->EncMode != ND_ENCM_LEGACY) || Instrux->HasRex)
                    {
                        res = nd_strcat_s(Buffer, BufferSize, gReg8Bit64[pOp->Info.Register.Reg]);
                        RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    }
                    else
                    {
                        res = nd_strcat_s(Buffer, BufferSize, gReg8Bit[pOp->Info.Register.Reg]);
                        RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    }
                    break;

                case ND_SIZE_16BIT:
                    // 16 bit register.
                    res = nd_strcat_s(Buffer, BufferSize, gReg16Bit[pOp->Info.Register.Reg]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;

                case ND_SIZE_32BIT:
                    // 32 bit register.
                    res = nd_strcat_s(Buffer, BufferSize, gReg32Bit[pOp->Info.Register.Reg]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;

                case ND_SIZE_64BIT:
                    // 64 bit register.
                    res = nd_strcat_s(Buffer, BufferSize, gReg64Bit[pOp->Info.Register.Reg]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;

                default:
                    return ND_STATUS_INVALID_INSTRUX;
                }
            }
            break;

            case ND_REG_SEG:
            {
                if (pOp->Info.Register.Reg >= ND_MAX_SEG_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                res = nd_strcat_s(Buffer, BufferSize, gRegSeg[pOp->Info.Register.Reg]);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

            case ND_REG_FPU:
            {
                if (pOp->Info.Register.Reg >= ND_MAX_FPU_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                res = nd_strcat_s(Buffer, BufferSize, gRegFpu[pOp->Info.Register.Reg]);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

            case ND_REG_MMX:
            {
                if (pOp->Info.Register.Reg >= ND_MAX_MMX_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                res = nd_strcat_s(Buffer, BufferSize, gRegMmx[pOp->Info.Register.Reg]);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

            case ND_REG_SSE:
            {
                if (pOp->Info.Register.Reg >= ND_MAX_SSE_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                switch (pOp->Info.Register.Size)
                {
                case ND_SIZE_128BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gRegXmm[pOp->Info.Register.Reg]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_256BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gRegYmm[pOp->Info.Register.Reg]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_512BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gRegZmm[pOp->Info.Register.Reg]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                default:
                    return ND_STATUS_INVALID_INSTRUX;
                }
            }
            break;

            case ND_REG_CR:
            {
                if (pOp->Info.Register.Reg >= ND_MAX_CR_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                res = nd_strcat_s(Buffer, BufferSize, gRegControl[pOp->Info.Register.Reg]);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

            case ND_REG_DR:
            {
                if (pOp->Info.Register.Reg >= ND_MAX_DR_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                res = nd_strcat_s(Buffer, BufferSize, gRegDebug[pOp->Info.Register.Reg]);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

            case ND_REG_TR:
            {
                if (pOp->Info.Register.Reg >= ND_MAX_TR_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                res = nd_strcat_s(Buffer, BufferSize, gRegTest[pOp->Info.Register.Reg]);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

            case ND_REG_BND:
            {
                // Sanity check.
                if (pOp->Info.Register.Reg >= ND_MAX_BND_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                res = nd_strcat_s(Buffer, BufferSize, gRegBound[pOp->Info.Register.Reg]);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

            case ND_REG_MSK:
            {
                // Sanity check.
                if (pOp->Info.Register.Reg >= ND_MAX_MSK_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                res = nd_strcat_s(Buffer, BufferSize, gRegMask[pOp->Info.Register.Reg]);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

            case ND_REG_TILE:
            {
                // Sanity check.
                if (pOp->Info.Register.Reg >= ND_MAX_TILE_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                res = nd_strcat_s(Buffer, BufferSize, gRegTile[pOp->Info.Register.Reg]);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

            default:
                break;
            }

            if (pOp->Info.Register.Count > 1)
            {
                status = NdSprintf(temp, sizeof(temp), "+%d", pOp->Info.Register.Count - 1);
                if (!ND_SUCCESS(status))
                {
                    return status;
                }

                res = nd_strcat_s(Buffer, BufferSize, temp);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }

            break;

        case ND_OP_BANK:
            // Nothing to show.
            break;

        case ND_OP_CONST:
        {
            // Implicit constant
            status = NdSprintf(temp, sizeof(temp), "%d", pOp->Info.Constant.Const);
            if (!ND_SUCCESS(status))
            {
                return status;
            }

            res = nd_strcat_s(Buffer, BufferSize, temp);
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
        break;

        case ND_OP_IMM:
        {
            switch (pOp->Size)
            {
            case 1:
                status = NdSprintf(temp, sizeof(temp), "0x%02x", (ND_UINT8)pOp->Info.Immediate.Imm);
                break;
            case 2:
                status = NdSprintf(temp, sizeof(temp), "0x%04x", (ND_UINT16)pOp->Info.Immediate.Imm);
                break;
            case 4:
                status = NdSprintf(temp, sizeof(temp), "0x%08x", (ND_UINT32)pOp->Info.Immediate.Imm);
                break;
            case 8:
                status = NdSprintf(temp, sizeof(temp), "0x%016llx", (ND_UINT64)pOp->Info.Immediate.Imm);
                break;
            }
            if (!ND_SUCCESS(status))
            {
                return status;
            }

            res = nd_strcat_s(Buffer, BufferSize, temp);
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
        break;

        case ND_OP_OFFS:
        {
            ND_UINT64 dest = Rip + Instrux->Length + pOp->Info.RelativeOffset.Rel;

            // Truncate to the actual word length.
            switch (Instrux->WordLength)
            {
            case 2:
                dest &= 0xFFFF;
                break;
            case 4:
                dest &= 0xFFFFFFFF;
                break;
            default:
                break;
            }

            status = NdSprintf(temp, sizeof(temp), "0x%llx", dest);
            if (!ND_SUCCESS(status))
            {
                return status;
            }

            res = nd_strcat_s(Buffer, BufferSize, temp);
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
        break;

        case ND_OP_ADDR:
        {
            switch (Instrux->AddrLength)
            {
            case 4:
                status = NdSprintf(temp, sizeof(temp), "0x%04x:0x%04x",
                    pOp->Info.Address.BaseSeg, (ND_UINT16)pOp->Info.Address.Offset);
                break;
            case 6:
                status = NdSprintf(temp, sizeof(temp), "0x%04x:0x%08x",
                    pOp->Info.Address.BaseSeg, (ND_UINT32)pOp->Info.Address.Offset);
                break;
            case 10:
                status = NdSprintf(temp, sizeof(temp), "0x%04x:0x%016llx",
                    pOp->Info.Address.BaseSeg, (ND_UINT64)pOp->Info.Address.Offset);
                break;
            default:
                return ND_STATUS_INVALID_INSTRUX;
            }

            if (!ND_SUCCESS(status))
            {
                return status;
            }

            res = nd_strcat_s(Buffer, BufferSize, temp);
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
        break;

        case ND_OP_MEM:
        {
            // Prepend the size. For VSIB addressing, store the VSIB element size, not the total accessed size.
            ND_OPERAND_SIZE size = pOp->Info.Memory.IsVsib ? pOp->Info.Memory.Vsib.ElemSize : pOp->Size;

            switch (size)
            {
            case 1:
                res = nd_strcat_s(Buffer, BufferSize, "byte ptr ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                break;
            case 2:
                res = nd_strcat_s(Buffer, BufferSize, "word ptr ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                break;
            case 4:
                res = nd_strcat_s(Buffer, BufferSize, "dword ptr ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                break;
            case 6:
                res = nd_strcat_s(Buffer, BufferSize, "fword ptr ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                break;
            case 8:
                res = nd_strcat_s(Buffer, BufferSize, "qword ptr ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                break;
            case 10:
                res = nd_strcat_s(Buffer, BufferSize, "tbyte ptr ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                break;
            case 16:
                res = nd_strcat_s(Buffer, BufferSize, "xmmword ptr ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                break;
            case 32:
                res = nd_strcat_s(Buffer, BufferSize, "ymmword ptr ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                break;
            case 48:
                res = nd_strcat_s(Buffer, BufferSize, "m384 ptr ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                break;
            case 64:
                res = nd_strcat_s(Buffer, BufferSize, "zmmword ptr ");
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                break;
            default:
                break;
            }

            // Perpend the segment, only if it is overridden via a prefix.
            if (pOp->Info.Memory.HasSeg && Instrux->HasSeg)
            {
                if (pOp->Info.Memory.Seg >= ND_MAX_SEG_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                if ((ND_CODE_64 != Instrux->DefCode) || (NDR_FS == pOp->Info.Memory.Seg) ||
                    (NDR_GS == pOp->Info.Memory.Seg))
                {
                    res = nd_strcat_s(Buffer, BufferSize, gRegSeg[pOp->Info.Memory.Seg]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);

                    res = nd_strcat_s(Buffer, BufferSize, ":");
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                }
            }

            // Prepend the "["
            res = nd_strcat_s(Buffer, BufferSize, "[");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);

            // Base, if any.
            if (pOp->Info.Memory.HasBase)
            {
                if (pOp->Info.Memory.Base >= ND_MAX_GPR_REGS)
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                switch (pOp->Info.Memory.BaseSize)
                {
                case ND_SIZE_8BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gReg8Bit[pOp->Info.Memory.Base]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_16BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gReg16Bit[pOp->Info.Memory.Base]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_32BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gReg32Bit[pOp->Info.Memory.Base]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_64BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gReg64Bit[pOp->Info.Memory.Base]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                default:
                    return ND_STATUS_INVALID_INSTRUX;
                }
            }

            // Index, if any. Special treatment for VSIB addressing. Also, perpend a "+" if base is present.
            if (pOp->Info.Memory.HasIndex)
            {
                if (pOp->Info.Memory.Index >= (pOp->Info.Memory.IsVsib ? ND_MAX_SSE_REGS : ND_MAX_GPR_REGS))
                {
                    return ND_STATUS_INVALID_INSTRUX;
                }

                if (pOp->Info.Memory.HasBase)
                {
                    res = nd_strcat_s(Buffer, BufferSize, "+");
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                }

                switch (pOp->Info.Memory.IndexSize)
                {
                case ND_SIZE_8BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gReg8Bit[pOp->Info.Memory.Index]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_16BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gReg16Bit[pOp->Info.Memory.Index]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_32BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gReg32Bit[pOp->Info.Memory.Index]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_64BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gReg64Bit[pOp->Info.Memory.Index]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_128BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gRegXmm[pOp->Info.Memory.Index]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_256BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gRegYmm[pOp->Info.Memory.Index]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case ND_SIZE_512BIT:
                    res = nd_strcat_s(Buffer, BufferSize, gRegZmm[pOp->Info.Memory.Index]);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                default:
                    return ND_STATUS_INVALID_INSTRUX;
                }

                // If index is present, scale is also present.
                if (pOp->Info.Memory.Scale != 1 && !pOp->Info.Memory.IsMib)
                {
                    status = NdSprintf(temp, sizeof(temp), "*%d", pOp->Info.Memory.Scale);
                    if (!ND_SUCCESS(status))
                    {
                        return status;
                    }

                    res = nd_strcat_s(Buffer, BufferSize, temp);
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                }
            }

            // Handle displacement.
            if (pOp->Info.Memory.HasDisp)
            {
                ND_UINT64 normDisp, disp;

                disp = pOp->Info.Memory.Disp;

                // If this is direct addressing (O operand) or we don't have base or index, than we don't normalize
                // the displacement, since it is used as a direct offset. Note that the second condition also
                // includes the RIP-relative case.
                if (pOp->Info.Memory.IsDirect || !(pOp->Info.Memory.HasBase || pOp->Info.Memory.HasIndex))
                {
                    normDisp = disp;
                }
                else
                {
                    switch (pOp->Info.Memory.DispSize)
                    {
                    case 1:
                        normDisp = ((disp & 0x80) ? ~disp + 1ULL : disp) & 0xFF;
                        break;
                    case 2:
                        normDisp = ((disp & 0x8000) ? ~disp + 1ULL : disp) & 0xFFFF;
                        break;
                    case 4:
                        normDisp = ((disp & 0x80000000) ? ~disp + 1ULL : disp) & 0xFFFFFFFF;
                        break;
                    default:
                        normDisp = disp;
                        break;
                    }

                    // Handle compressed displacement. It is fine to cast the normDisp to ND_UINT32, as the
                    // compressed displacement only works with ND_UINT8 displacements. Also, in this phase,
                    // the normDisp is converted to a positive quantity, so no sign-extension is needed.
                    if (pOp->Info.Memory.HasCompDisp)
                    {
                        normDisp = (ND_UINT64)(ND_UINT32)normDisp * pOp->Info.Memory.CompDispSize;
                    }
                }


                // Now displacement.
                if (pOp->Info.Memory.HasBase || pOp->Info.Memory.HasIndex)
                {
                    res = nd_strcat_s(Buffer, BufferSize, Instrux->SignDisp ? "-" : "+");
                    RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
                }

                if (pOp->Info.Memory.IsRipRel)
                {
                    ND_UINT64 target = disp + Rip + Instrux->Length;

                    if (Instrux->AddrMode == ND_ADDR_32)
                    {
                        target &= 0xFFFFFFFF;
                    }

                    status = NdSprintf(temp, sizeof(temp), "rel 0x%llx", target);
                }
                else
                {
                    ND_UINT8 trimSize;

                    trimSize = (Instrux->AddrMode == ND_ADDR_16) ? 2 : ((Instrux->AddrMode == ND_ADDR_32) ? 4 : 8);

                    // Truncate the displacement size to the size of the address length.
                    normDisp = ND_TRIM(trimSize, normDisp);

                    status = NdSprintf(temp, sizeof(temp), "0x%llx", normDisp);
                }
                if (!ND_SUCCESS(status))
                {
                    return status;
                }

                res = nd_strcat_s(Buffer, BufferSize, temp);
                RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
            }

            // And the ending "]"
            res = nd_strcat_s(Buffer, BufferSize, "]");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
        break;

        default:
            return ND_STATUS_INVALID_INSTRUX;
        }

        // Handle memory broadcast.
        if (pOp->Decorator.HasBroadcast)
        {
            status = NdSprintf(temp, sizeof(temp), "{1to%d}", pOp->Decorator.Broadcast.Count);
            if (!ND_SUCCESS(status))
            {
                return status;
            }

            res = nd_strcat_s(Buffer, BufferSize, temp);
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }

        // Handle masking.
        if (pOp->Decorator.HasMask)
        {
            if (pOp->Decorator.Mask.Msk >= ND_MAX_MSK_REGS)
            {
                return ND_STATUS_INVALID_INSTRUX;
            }

            status = NdSprintf(temp, sizeof(temp), "{%s}", gRegMask[pOp->Decorator.Mask.Msk]);
            if (!ND_SUCCESS(status))
            {
                return status;
            }

            res = nd_strcat_s(Buffer, BufferSize, temp);
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }

        // Handle zeroing. Note that zeroing without masking is ignored.
        if (pOp->Decorator.HasZero && pOp->Decorator.HasMask)
        {
            res = nd_strcat_s(Buffer, BufferSize, "{z}");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }

        // Append Suppress All Exceptions decorator.
        if (pOp->Decorator.HasSae && !pOp->Decorator.HasEr)
        {
            // ER implies SAE, so if we have ER, we will list that.
            res = nd_strcat_s(Buffer, BufferSize, ", {sae}");
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }

        // Append Embedded Rounding decorator.
        if (pOp->Decorator.HasEr)
        {
            if (Instrux->RoundingMode >= 4)
            {
                return ND_STATUS_INVALID_INSTRUX;
            }

            status = NdSprintf(temp, sizeof(temp), ", {%s-sae}", gEmbeddedRounding[Instrux->RoundingMode]);
            if (!ND_SUCCESS(status))
            {
                return status;
            }

            res = nd_strcat_s(Buffer, BufferSize, temp);
            RET_EQ(res, ND_NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
    }

    return ND_STATUS_SUCCESS;
}
#else
NDSTATUS
NdToText(
    const INSTRUX *Instrux,
    ND_UINT64 Rip,
    ND_UINT32 BufferSize,
    char *Buffer
    )
{
    UNREFERENCED_PARAMETER(Instrux);
    UNREFERENCED_PARAMETER(Rip);

    // At least make sure the buffer is ND_NULL-terminated so integrators can use NdToText without checking if the
    // BDDISASM_NO_FORMAT macro is defined. This makes switching between versions with formatting and versions without
    // formatting easier.
    if (Buffer != ND_NULL && BufferSize >= 1)
    {
        *Buffer = '\0';
    }

    return ND_STATUS_SUCCESS;
}
#endif // !BDDISASM_NO_FORMAT
