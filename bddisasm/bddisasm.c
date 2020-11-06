/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include "include/nd_crt.h"
#include "../inc/bddisasm.h"

// The table definitions.
#include "include/tabledefs.h"

// Handy macros.
#define RET_EQ(x, y, z)     if ((x) == (y)) { return (z); }
#define RET_GE(x, y, z)     if ((x) >= (y)) { return (z); }
#define RET_GT(x, y, z)     if ((x) >  (y)) { return (z); }


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
    "EQ_UQ", "NGE", "NGT", "false", "NEQ_OQ", "GE", "GT", "TRUE",
    "EQ_OS", "LT_OQ", "LE_OQ", "UNORD_S", "NEQ_US", "NLT_UQ", "NLE_UQ", "ORD_S",
    "EQ_US", "NGE_UQ", "NGT_UQ", "FALSE_OS", "NEQ_OS", "GE_OQ", "GT_OQ", "TRUE_US",
};

static const char *gEmbeddedRounding[] =
{
    "rn", "rd", "ru", "rz",
};

static const uint16_t gOperandMap[] =
{
    ND_OPE_D,       // ND_OPT_A
    ND_OPE_V,       // ND_OPT_B
    ND_OPE_R,       // ND_OPT_C
    ND_OPE_R,       // ND_OPT_D
    ND_OPE_M,       // ND_OPT_E
    ND_OPE_S,       // ND_OPT_F
    ND_OPE_R,       // ND_OPT_G
    ND_OPE_V,       // ND_OPT_H
    ND_OPE_I,       // ND_OPT_I
    ND_OPE_D,       // ND_OPT_J
    ND_OPE_S,       // ND_OPT_K
    ND_OPE_L,       // ND_OPT_L
    ND_OPE_M,       // ND_OPT_M
    ND_OPE_M,       // ND_OPT_N
    ND_OPE_D,       // ND_OPT_O
    ND_OPE_R,       // ND_OPT_P
    ND_OPE_M,       // ND_OPT_Q
    ND_OPE_M,       // ND_OPT_R
    ND_OPE_R,       // ND_OPT_S
    ND_OPE_R,       // ND_OPT_T
    ND_OPE_M,       // ND_OPT_U
    ND_OPE_R,       // ND_OPT_V
    ND_OPE_M,       // ND_OPT_W
    ND_OPE_S,       // ND_OPT_X
    ND_OPE_S,       // ND_OPT_Y
    ND_OPE_O,       // ND_OPT_Z
    ND_OPE_R,       // ND_OPT_rB
    ND_OPE_M,       // ND_OPT_mB
    ND_OPE_R,       // ND_OPT_rK
    ND_OPE_V,       // ND_OPT_vK
    ND_OPE_M,       // ND_OPT_mK
    ND_OPE_A,       // ND_OPT_aK
    ND_OPE_R,       // ND_OPT_mR
    ND_OPE_M,       // ND_OPT_mM
    ND_OPE_R,       // ND_OPT_rT
    ND_OPE_M,       // ND_OPT_mT
    ND_OPE_V,       // ND_OPT_vT
    ND_OPE_1,       // ND_OPT_1

    ND_OPE_S,       // ND_OPT_RIP
    ND_OPE_S,       // ND_OPT_MXCSR
    ND_OPE_S,       // ND_OPT_PKRU
    ND_OPE_S,       // ND_OPT_SSP
    ND_OPE_S,       // ND_OPT_UIF

    ND_OPE_S,       // ND_OPT_GPR_AH
    ND_OPE_S,       // ND_OPT_GPR_rAX
    ND_OPE_S,       // ND_OPT_GPR_rCX
    ND_OPE_S,       // ND_OPT_GPR_rDX
    ND_OPE_S,       // ND_OPT_GPR_rBX
    ND_OPE_S,       // ND_OPT_GPR_rSP
    ND_OPE_S,       // ND_OPT_GPR_rBP
    ND_OPE_S,       // ND_OPT_GPR_rSI
    ND_OPE_S,       // ND_OPT_GPR_rDI
    ND_OPE_S,       // ND_OPT_GPR_rR8
    ND_OPE_S,       // ND_OPT_GPR_rR9
    ND_OPE_S,       // ND_OPT_GPR_rR11

    ND_OPE_S,       // ND_OPT_SEG_CS
    ND_OPE_S,       // ND_OPT_SEG_SS
    ND_OPE_S,       // ND_OPT_SEG_DS
    ND_OPE_S,       // ND_OPT_SEG_ES
    ND_OPE_S,       // ND_OPT_SEG_FS
    ND_OPE_S,       // ND_OPT_SEG_GS

    ND_OPE_S,       // ND_OPT_FPU_ST0
    ND_OPE_M,       // ND_OPT_FPU_STX

    ND_OPE_S,       // ND_OPT_SSE_XMM0
    ND_OPE_S,       // ND_OPT_SSE_XMM1
    ND_OPE_S,       // ND_OPT_SSE_XMM2
    ND_OPE_S,       // ND_OPT_SSE_XMM3
    ND_OPE_S,       // ND_OPT_SSE_XMM4
    ND_OPE_S,       // ND_OPT_SSE_XMM5
    ND_OPE_S,       // ND_OPT_SSE_XMM6
    ND_OPE_S,       // ND_OPT_SSE_XMM7

    ND_OPE_S,       // ND_OPT_MEM_rBX_AL (as used by XLAT)
    ND_OPE_S,       // ND_OPT_MEM_rDI (as used by masked moves)
    ND_OPE_S,       // ND_OPT_MEM_SHS
    ND_OPE_S,       // ND_OPT_MEM_SHSP
    ND_OPE_S,       // ND_OPT_MEM_SHS0

    ND_OPE_L,       // ND_OPT_Im2z

    ND_OPE_S,       // ND_OPT_CR_0
    ND_OPE_S,       // ND_OPT_IDTR
    ND_OPE_S,       // ND_OPT_GDTR
    ND_OPE_S,       // ND_OPT_LDTR
    ND_OPE_S,       // ND_OPT_TR

    ND_OPE_S,       // ND_OPT_X87_CONTROL
    ND_OPE_S,       // ND_OPT_X87_TAG
    ND_OPE_S,       // ND_OPT_X87_STATUS

    ND_OPE_E,       // ND_OPT_MSR
    ND_OPE_E,       // ND_OPT_XCR
    ND_OPE_S,       // ND_OPT_MSR_TSC
    ND_OPE_S,       // ND_OPT_MSR_TSCAUX
    ND_OPE_S,       // ND_OPT_MSR_SEIP
    ND_OPE_S,       // ND_OPT_MSR_SESP
    ND_OPE_S,       // ND_OPT_MSR_SCS
    ND_OPE_S,       // ND_OPT_MSR_STAR
    ND_OPE_S,       // ND_OPT_MSR_LSTAR
    ND_OPE_S,       // ND_OPT_MSR_FMASK
    ND_OPE_S,       // ND_OPT_MSR_FSBASE
    ND_OPE_S,       // ND_OPT_MSR_GSBASE
    ND_OPE_S,       // ND_OPT_MSR_KGSBASE
    ND_OPE_S,       // ND_OPT_XCR_0
    ND_OPE_S,       // ND_OPT_REG_BANK
    ND_OPE_S,       // Unused.
};


static const uint8_t gDispsizemap16[4][8] =
{
    { 0, 0, 0, 0, 0, 0, 2, 0 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 2, 2, 2, 2, 2, 2, 2, 2 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};

static const uint8_t gDispsizemap[4][8] =
{
    { 0, 0, 0, 0, 0, 4, 0, 0 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 4, 4, 4, 4, 4, 4, 4, 4 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};


//
// NdGetVersion
//
void
NdGetVersion(
    uint32_t *Major,
    uint32_t *Minor,
    uint32_t *Revision,
    char **BuildDate,
    char **BuildTime
    )
{
    if (NULL != Major)
    {
        *Major = DISASM_VERSION_MAJOR;
    }

    if (NULL != Minor)
    {
        *Minor = DISASM_VERSION_MINOR;
    }

    if (NULL != Revision)
    {
        *Revision = DISASM_VERSION_REVISION;
    }

//
// Do not use __TIME__ and __DATE__ macros when compiling against a kernel tree.
//
#if defined(__KERNEL__) && defined(__GNUC__)

    if (NULL != BuildDate)
    {
        *BuildDate = NULL;
    }

    if (NULL != BuildTime)
    {
        *BuildTime = NULL;
    }

#else

    if (NULL != BuildDate)
    {
        *BuildDate = __DATE__;
    }

    if (NULL != BuildTime)
    {
        *BuildTime = __TIME__;
    }

#endif

}

#ifndef KERNEL_MODE
//
// NdSprintf
//
static NDSTATUS
NdSprintf(
    char *Destination,
    size_t DestinationSize,
    const char *Formatstring,
    ...
    )
//
// Wrapper on vsnprintf.
//
{
    int res;
    va_list args;

    if (NULL == Destination)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (NULL == Formatstring)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    nd_memzero(Destination, DestinationSize);

    va_start(args, Formatstring);

    // _vsnprintf is used instead of the more secure _vsnprintf_s because the mini-Petru supports just
    // the unsecured version, and we depend on it.
    res = nd_vsnprintf_s(Destination, DestinationSize, DestinationSize - 1, Formatstring, args);

    va_end(args);

    if ((res < 0) || ((size_t)res >= DestinationSize - 1))
    {
        return ND_STATUS_BUFFER_OVERFLOW;
    }

    return ND_STATUS_SUCCESS;
}
#else
#define NdSprintf(Destination, DestinationSize, Formatstring, ...) RtlStringCbPrintfA(Destination,                  \
                                                                                      DestinationSize,              \
                                                                                      Formatstring,                 \
                                                                                      __VA_ARGS__);
#endif


//
// NdFetchData
//
static uint64_t
NdFetchData(
    const uint8_t *Buffer,
    uint8_t Size
    )
{
    return (4 == Size) ? ND_FETCH_32(Buffer) :
           (1 == Size) ? ND_FETCH_8(Buffer) :
           (8 == Size) ? ND_FETCH_64(Buffer) :
           (2 == Size) ? ND_FETCH_16(Buffer) :
           0;
}


//
// NdFetchXop
//
static NDSTATUS
NdFetchXop(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size
    )
{
    // Offset points to the 0x8F XOP prefix.
    // One more byte has to follow, the modrm or the second XOP byte.
    RET_GT((size_t)Offset + 2, Size, ND_STATUS_BUFFER_TOO_SMALL);

    if (((Code[Offset + 1] & 0x1F) >= 8))
    {
        // XOP found, make sure the third byte is here.
        RET_GT((size_t)Offset + 3, Size, ND_STATUS_BUFFER_TOO_SMALL);

        // Make sure we don't have any other prefix.
        if (Instrux->HasOpSize || Instrux->HasRepnzXacquireBnd || Instrux->HasRepRepzXrelease || Instrux->HasRex)
        {
            return ND_STATUS_XOP_WITH_PREFIX;
        }

        // Fill in XOP info.
        Instrux->HasXop = true;
        Instrux->EncMode = ND_ENCM_XOP;
        Instrux->Xop.Xop[0] = Code[Offset];
        Instrux->Xop.Xop[1] = Code[Offset + 1];
        Instrux->Xop.Xop[2] = Code[Offset + 2];

        Instrux->Exs.w = Instrux->Xop.w;
        Instrux->Exs.r = ~Instrux->Xop.r;
        Instrux->Exs.x = ~Instrux->Xop.x;
        Instrux->Exs.b = ~Instrux->Xop.b;
        Instrux->Exs.l = Instrux->Xop.l;
        Instrux->Exs.v = ~Instrux->Xop.v;
        Instrux->Exs.m = Instrux->Xop.m;
        Instrux->Exs.p = Instrux->Xop.p;

        // if we are in non 64 bit mode, we must make sure that none of the extended registers are being addressed.
        if (Instrux->DefCode != ND_CODE_64)
        {
            // Xop.R and Xop.X must be 1 (inverted).
            if ((Instrux->Exs.r | Instrux->Exs.x) == 1)
            {
                return ND_STATUS_INVALID_ENCODING_IN_MODE;
            }

            // Xop.V must be less than 8.
            if ((Instrux->Exs.v & 0x8) == 0x8)
            {
                return ND_STATUS_INVALID_ENCODING_IN_MODE;
            }

            // Xop.B is ignored, so we force it to 0.
            Instrux->Exs.b = 0;
        }

        // Update Instrux length & offset, and make sure we don't exceed 15 bytes.
        Instrux->Length += 3;
        if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
        {
            return ND_STATUS_INSTRUCTION_TOO_LONG;
        }
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchVex2
//
static NDSTATUS
NdFetchVex2(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size
    )
{
    // One more byte has to follow, the modrm or the second VEX byte.
    RET_GT((size_t)Offset + 2, Size, ND_STATUS_BUFFER_TOO_SMALL);

    // VEX is available only in 32 & 64 bit mode.
    if ((ND_CODE_64 == Instrux->DefCode) || ((Code[Offset + 1] & 0xC0) == 0xC0))
    {
        // Make sure we don't have any other prefix.
        if (Instrux->HasOpSize || Instrux->HasRepnzXacquireBnd ||
            Instrux->HasRepRepzXrelease || Instrux->HasRex || Instrux->HasLock)
        {
            return ND_STATUS_VEX_WITH_PREFIX;
        }

        // Fill in VEX2 info.
        Instrux->VexMode = ND_VEXM_2B;
        Instrux->HasVex = true;
        Instrux->EncMode = ND_ENCM_VEX;
        Instrux->Vex2.Vex[0] = Code[Offset];
        Instrux->Vex2.Vex[1] = Code[Offset + 1];

        Instrux->Exs.m = 1; // For VEX2 instructions, always use the second table.
        Instrux->Exs.r = ~Instrux->Vex2.r;
        Instrux->Exs.v = ~Instrux->Vex2.v;
        Instrux->Exs.l = Instrux->Vex2.l;
        Instrux->Exs.p = Instrux->Vex2.p;

        // Update Instrux length & offset, and make sure we don't exceed 15 bytes.
        Instrux->Length += 2;
        if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
        {
            return ND_STATUS_INSTRUCTION_TOO_LONG;
        }
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchVex3
//
static NDSTATUS
NdFetchVex3(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size
    )
{
    // One more byte has to follow, the modrm or the second VEX byte.
    RET_GT((size_t)Offset + 2, Size, ND_STATUS_BUFFER_TOO_SMALL);

    // VEX is available only in 32 & 64 bit mode.
    if ((ND_CODE_64 == Instrux->DefCode) || ((Code[Offset + 1] & 0xC0) == 0xC0))
    {
        // VEX found, make sure the third byte is here.
        RET_GT((size_t)Offset + 3, Size, ND_STATUS_BUFFER_TOO_SMALL);

        // Make sure we don't have any other prefix.
        if (Instrux->HasOpSize || Instrux->HasRepnzXacquireBnd ||
            Instrux->HasRepRepzXrelease || Instrux->HasRex || Instrux->HasLock)
        {
            return ND_STATUS_VEX_WITH_PREFIX;
        }

        // Fill in XOP info.
        Instrux->VexMode = ND_VEXM_3B;
        Instrux->HasVex = true;
        Instrux->EncMode = ND_ENCM_VEX;
        Instrux->Vex3.Vex[0] = Code[Offset];
        Instrux->Vex3.Vex[1] = Code[Offset + 1];
        Instrux->Vex3.Vex[2] = Code[Offset + 2];

        Instrux->Exs.r = ~Instrux->Vex3.r;
        Instrux->Exs.x = ~Instrux->Vex3.x;
        Instrux->Exs.b = ~Instrux->Vex3.b;
        Instrux->Exs.m = Instrux->Vex3.m;
        Instrux->Exs.w = Instrux->Vex3.w;
        Instrux->Exs.v = ~Instrux->Vex3.v;
        Instrux->Exs.l = Instrux->Vex3.l;
        Instrux->Exs.p = Instrux->Vex3.p;

        // Do validations in case of VEX outside 64 bits.
        if (Instrux->DefCode != ND_CODE_64)
        {
            // Vex.R and Vex.X have been tested by the initial if.

            // Vex.vvvv must be less than 8.
            Instrux->Exs.v &= 7;

            // Vex.B is ignored, so we force it to 0.
            Instrux->Exs.b = 0;
        }

        // Update Instrux length & offset, and make sure we don't exceed 15 bytes.
        Instrux->Length += 3;
        if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
        {
            return ND_STATUS_INSTRUCTION_TOO_LONG;
        }
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchEvex
//
static NDSTATUS
NdFetchEvex(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size
    )
{
    // One more byte has to follow, the modrm or the second VEX byte.
    RET_GT((size_t)Offset + 2, Size, ND_STATUS_BUFFER_TOO_SMALL);

    if ((ND_CODE_64 != Instrux->DefCode) && ((Code[Offset + 1] & 0xC0) != 0xC0))
    {
        // BOUND instruction in non-64 bit mode, not EVEX.
        return ND_STATUS_SUCCESS;
    }

    // EVEX found, make sure all the bytes are present. At least 4 bytes in total must be present.
    RET_GT((size_t)Offset + 4, Size, ND_STATUS_BUFFER_TOO_SMALL);

    // This is EVEX.
    Instrux->HasEvex = true;
    Instrux->EncMode = ND_ENCM_EVEX;
    Instrux->Evex.Evex[0] = Code[Offset + 0];
    Instrux->Evex.Evex[1] = Code[Offset + 1];
    Instrux->Evex.Evex[2] = Code[Offset + 2];
    Instrux->Evex.Evex[3] = Code[Offset + 3];

    // Legacy prefixes are not accepted with EVEX.
    if (Instrux->HasOpSize || Instrux->HasRepnzXacquireBnd || Instrux->HasRepRepzXrelease || Instrux->HasRex)
    {
        return ND_STATUS_EVEX_WITH_PREFIX;
    }

    // Do the opcode independent checks. Opcode dependent checks are done when decoding each
    if (Instrux->Evex.zero != 0 || Instrux->Evex.one != 1 || Instrux->Evex.m == 0)
    {
        return ND_STATUS_INVALID_ENCODING;
    }

    // Fill in the generic extension bits
    Instrux->Exs.r = ~Instrux->Evex.r;
    Instrux->Exs.x = ~Instrux->Evex.x;
    Instrux->Exs.b = ~Instrux->Evex.b;
    Instrux->Exs.rp = ~Instrux->Evex.rp;
    Instrux->Exs.m = Instrux->Evex.m;
    Instrux->Exs.w = Instrux->Evex.w;
    Instrux->Exs.v = ~Instrux->Evex.v;
    Instrux->Exs.vp = ~Instrux->Evex.vp;
    Instrux->Exs.p = Instrux->Evex.p;
    Instrux->Exs.z = Instrux->Evex.z;
    Instrux->Exs.l = Instrux->Evex.l;
    Instrux->Exs.bm = Instrux->Evex.bm;
    Instrux->Exs.k = Instrux->Evex.a;

    // Do EVEX validations outside 64 bits mode.
    if (ND_CODE_64 != Instrux->DefCode)
    {
        // Evex.R and Evex.X must be 1. If they're not, we have BOUND instruction. This is checkked in the
        // first if. Note that they are inverted inside the Evex prefix.
        Instrux->Exs.r = 0;
        Instrux->Exs.x = 0;

        // Evex.B is ignored, so we force it to 0.
        Instrux->Exs.b = 0;

        // Evex.R' is ignored, so we force it to 0.
        Instrux->Exs.rp = 0;

        // High bit inside Evex.VVVV is ignored, so we force it to 0.
        Instrux->Exs.v &= 0x7;

        // Evex.V' is ignored.
        Instrux->Exs.vp = 0;
    }

    // Update Instrux length & offset, and make sure we don't exceed 15 bytes.
    Instrux->Length += 4;
    if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
    {
        return ND_STATUS_INSTRUCTION_TOO_LONG;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchPrefixes
//
static NDSTATUS
NdFetchPrefixes(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size
    )
{
    NDSTATUS status;
    bool morePrefixes;
    uint8_t prefix;

    morePrefixes = true;

    while (morePrefixes)
    {
        morePrefixes = false;

        RET_GT((size_t)Offset + 1, Size, ND_STATUS_BUFFER_TOO_SMALL);

        prefix = Code[Offset];

        // Speedup: if the current byte is not a prefix of any kind, leave now. This will be the case most of the times.
        if (ND_PREF_CODE_NONE == gPrefixesMap[prefix])
        {
            status = ND_STATUS_SUCCESS;
            goto done_prefixes;
        }

        if (ND_PREF_CODE_STANDARD == gPrefixesMap[prefix])
        {
            switch (prefix)
            {
            case ND_PREFIX_G0_LOCK:
                Instrux->HasLock = true;
                morePrefixes = true;
                break;
            case ND_PREFIX_G1_REPE_REPZ:
                Instrux->Rep = ND_PREFIX_G1_REPE_REPZ;
                Instrux->HasRepRepzXrelease = true;
                morePrefixes = true;
                break;
            case ND_PREFIX_G1_REPNE_REPNZ:
                Instrux->Rep = ND_PREFIX_G1_REPNE_REPNZ;
                Instrux->HasRepnzXacquireBnd = true;
                morePrefixes = true;
                break;
            case ND_PREFIX_G2_SEG_CS:
            case ND_PREFIX_G2_SEG_SS:
            case ND_PREFIX_G2_SEG_DS:
            case ND_PREFIX_G2_SEG_ES:
            case ND_PREFIX_G2_SEG_FS:
            case ND_PREFIX_G2_SEG_GS:
                if (ND_CODE_64 == Instrux->DefCode)
                {
                    // Do not overwrite FS/GS with ES/CS/DS/SS in 64 bit mode. In 64 bit mode, only FS/GS overrides
                    // are considered.
                    if (prefix == ND_PREFIX_G2_SEG_FS || prefix == ND_PREFIX_G2_SEG_GS)
                    {
                        Instrux->Seg = prefix;
                        Instrux->HasSeg = true;
                    }
                }
                else
                {
                    Instrux->Seg = prefix;
                    Instrux->HasSeg = true;
                }
                if (prefix == ND_PREFIX_G2_BR_TAKEN || prefix == ND_PREFIX_G2_BR_NOT_TAKEN)
                {
                    Instrux->Bhint = prefix;
                    Instrux->HasSeg = true;
                }
                morePrefixes = true;
                break;
            case ND_PREFIX_G3_OPERAND_SIZE:
                Instrux->HasOpSize = true;
                morePrefixes = true;
                break;
            case ND_PREFIX_G4_ADDR_SIZE:
                Instrux->HasAddrSize = true;
                morePrefixes = true;
                break;
            default:
                break;
            }
        }

        // REX must precede the opcode byte. However, if one or more other prefixes are present, the instruction
        // will still decode & execute properly, but REX will be ignored.
        if (morePrefixes && Instrux->HasRex)
        {
            Instrux->HasRex = false;
            Instrux->Rex.Rex = 0;
            Instrux->Exs.w = 0;
            Instrux->Exs.r = 0;
            Instrux->Exs.x = 0;
            Instrux->Exs.b = 0;
        }

        // Check for REX.
        if ((ND_CODE_64 == Instrux->DefCode) && (ND_PREF_CODE_REX == gPrefixesMap[prefix]))
        {
            Instrux->HasRex = true;
            Instrux->Rex.Rex = prefix;
            Instrux->Exs.w = Instrux->Rex.w;
            Instrux->Exs.r = Instrux->Rex.r;
            Instrux->Exs.x = Instrux->Rex.x;
            Instrux->Exs.b = Instrux->Rex.b;
            morePrefixes = true;
        }

        // We have found prefixes, update the instruction length and the current offset.
        if (morePrefixes)
        {
            Instrux->Length++, Offset++;
            if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
            {
                return ND_STATUS_INSTRUCTION_TOO_LONG;
            }
        }
    }

    // We must have at least one more free byte after the prefixes, which will be either the opcode, either
    // XOP/VEX/EVEX/MVEX prefix.
    RET_GT((size_t)Offset + 1, Size, ND_STATUS_BUFFER_TOO_SMALL);

    // Try to match a XOP/VEX/EVEX/MVEX prefix.
    if (ND_PREF_CODE_EX == gPrefixesMap[Code[Offset]])
    {
        // Check for XOP
        if (Code[Offset] == ND_PREFIX_XOP)
        {
            status = NdFetchXop(Instrux, Code, Offset, Size);
            if (!ND_SUCCESS(status))
            {
                return status;
            }
        }
        else if (Code[Offset] == ND_PREFIX_VEX_2B)
        {
            status = NdFetchVex2(Instrux, Code, Offset, Size);
            if (!ND_SUCCESS(status))
            {
                return status;
            }
        }
        else if (Code[Offset] == ND_PREFIX_VEX_3B)
        {
            status = NdFetchVex3(Instrux, Code, Offset, Size);
            if (!ND_SUCCESS(status))
            {
                return status;
            }
        }
        else if (Code[Offset] == ND_PREFIX_EVEX)
        {
            status = NdFetchEvex(Instrux, Code, Offset, Size);
            if (!ND_SUCCESS(status))
            {
                return status;
            }
        }
        else
        {
            return ND_STATUS_INVALID_INSTRUX;
        }
    }
    else
    {
        Instrux->EncMode = ND_ENCM_LEGACY;
    }

done_prefixes:
    // The total length of the instruction is the total length of the prefixes right now.
    Instrux->PrefLength = Instrux->OpOffset = Instrux->Length;

    return ND_STATUS_SUCCESS;
}


//
// NdFetchOpcode
//
static NDSTATUS
NdFetchOpcode(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size
    )
{
    // At least one byte must be available, for the fetched opcode.
    RET_GT((size_t)Offset + 1, Size, ND_STATUS_BUFFER_TOO_SMALL);

    Instrux->OpCodeBytes[Instrux->OpLength++] = Code[Offset];

    Instrux->Length++;
    if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
    {
        return ND_STATUS_INSTRUCTION_TOO_LONG;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchModrm
//
static NDSTATUS
NdFetchModrm(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size
    )
{
    // At least one byte must be available, for the modrm byte.
    RET_GT((size_t)Offset + 1, Size, ND_STATUS_BUFFER_TOO_SMALL);

    // If we get called, we assume we have ModRM.
    Instrux->HasModRm = true;

    // Fetch the ModRM byte & update the offset and the instruction length.
    Instrux->ModRm.ModRm = Code[Offset];
    Instrux->ModRmOffset = Offset;

    Instrux->Length++, Offset++;

    // Make sure we don't exceed the maximum instruction length.
    if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
    {
        return ND_STATUS_INSTRUCTION_TOO_LONG;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchModrmAndSib
//
static NDSTATUS
NdFetchModrmAndSib(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size
    )
{
    // At least one byte must be available, for the modrm byte.
    RET_GT((size_t)Offset + 1, Size, ND_STATUS_BUFFER_TOO_SMALL);

    // If we get called, we assume we have ModRM.
    Instrux->HasModRm = true;

    // Fetch the ModRM byte & update the offset and the instruction length.
    Instrux->ModRm.ModRm = Code[Offset];
    Instrux->ModRmOffset = Offset;

    Instrux->Length++, Offset++;

    // Make sure we don't exceed the maximum instruction length.
    if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
    {
        return ND_STATUS_INSTRUCTION_TOO_LONG;
    }

    // If needed, fetch the SIB.
    if ((Instrux->ModRm.rm == NDR_RSP) && (Instrux->ModRm.mod != 3) && (Instrux->AddrMode != ND_ADDR_16))
    {
        // At least one more byte must be available, for the sib.
        RET_GT((size_t)Offset + 1, Size, ND_STATUS_BUFFER_TOO_SMALL);

        // SIB present.
        Instrux->HasSib = true;

        Instrux->Sib.Sib = Code[Offset];
        Instrux->Length++;

        // Make sure we don't exceed the maximum instruction length.
        if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
        {
            return ND_STATUS_INSTRUCTION_TOO_LONG;
        }
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchDisplacement
//
static NDSTATUS
NdFetchDisplacement(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size
    )
//
// Will decode the displacement from the instruction. Will fill in extracted information in Instrux,
// and will update the instruction length.
//
{
    uint8_t displSize;

    displSize = 0;

    if (ND_ADDR_16 == Instrux->AddrMode)
    {
        displSize = gDispsizemap16[Instrux->ModRm.mod][Instrux->ModRm.rm];
    }
    else
    {
        displSize = gDispsizemap[Instrux->ModRm.mod][Instrux->HasSib ? Instrux->Sib.base : Instrux->ModRm.rm];
    }

    if (0 != displSize)
    {
        static const uint32_t signMask[4] = { 0x80, 0x8000, 0, 0x80000000 };

        // Make sure enough buffer space is available.
        RET_GT((size_t)Offset + displSize, Size, ND_STATUS_BUFFER_TOO_SMALL);

        // If we get here, we have displacement.
        Instrux->HasDisp = true;

        Instrux->Displacement = (uint32_t)NdFetchData(Code + Offset, displSize);
        Instrux->SignDisp = (Instrux->Displacement & signMask[displSize - 1]) ? true : false;

        // Fill in displacement info.
        Instrux->DispLength = displSize;
        Instrux->DispOffset = Offset;
        Instrux->Length += displSize;
        if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
        {
            return ND_STATUS_INSTRUCTION_TOO_LONG;
        }
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchAddress
//
static NDSTATUS
NdFetchAddress(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size,
    uint8_t AddressSize
    )
{
    //. Make sure the
    RET_GT((size_t)Offset + AddressSize, Size, ND_STATUS_BUFFER_TOO_SMALL);

    Instrux->HasAddr = true;
    Instrux->AddrLength = AddressSize;
    Instrux->AddrOffset = Offset;

    Instrux->Address.Ip = (uint32_t)NdFetchData(Code + Offset, Instrux->AddrLength - 2);
    Instrux->Address.Cs = (uint16_t)NdFetchData(Code + Offset + Instrux->AddrLength - 2, 2);

    Instrux->Length += Instrux->AddrLength;
    if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
    {
        return ND_STATUS_INSTRUCTION_TOO_LONG;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchImmediate
//
static NDSTATUS
NdFetchImmediate(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size,
    uint8_t ImmediateSize
    )
{
    uint64_t imm;

    RET_GT((size_t)Offset + ImmediateSize, Size, ND_STATUS_BUFFER_TOO_SMALL);

    imm = NdFetchData(Code + Offset, ImmediateSize);

    if (Instrux->HasImm2)
    {
        Instrux->HasImm3 = true;
        Instrux->Imm3Length = ImmediateSize;
        Instrux->Imm3Offset = Offset;
        Instrux->Immediate3 = (uint8_t)imm;
    }
    else if (Instrux->HasImm1)
    {
        Instrux->HasImm2 = true;
        Instrux->Imm2Length = ImmediateSize;
        Instrux->Imm2Offset = Offset;
        Instrux->Immediate2 = (uint8_t)imm;
    }
    else
    {
        Instrux->HasImm1 = true;
        Instrux->Imm1Length = ImmediateSize;
        Instrux->Imm1Offset = Offset;
        Instrux->Immediate1 = imm;
    }

    Instrux->Length += ImmediateSize;
    if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
    {
        return ND_STATUS_INSTRUCTION_TOO_LONG;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchRelativeOffset
//
static NDSTATUS
NdFetchRelativeOffset(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size,
    uint8_t RelOffsetSize
    )
{
    // Make sure we don't outrun the buffer.
    RET_GT((size_t)Offset + RelOffsetSize, Size, ND_STATUS_BUFFER_TOO_SMALL);

    Instrux->HasRelOffs = true;
    Instrux->RelOffsLength = RelOffsetSize;
    Instrux->RelOffsOffset = Offset;

    Instrux->RelativeOffset = (uint32_t)NdFetchData(Code + Offset, RelOffsetSize);

    Instrux->Length += RelOffsetSize;
    if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
    {
        return ND_STATUS_INSTRUCTION_TOO_LONG;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchMoffset
//
static NDSTATUS
NdFetchMoffset(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size,
    uint8_t MoffsetSize
    )
{
    RET_GT((size_t)Offset + MoffsetSize, Size, ND_STATUS_BUFFER_TOO_SMALL);

    Instrux->HasMoffset = true;
    Instrux->MoffsetLength = MoffsetSize;
    Instrux->MoffsetOffset = Offset;

    Instrux->Moffset = NdFetchData(Code + Offset, MoffsetSize);

    Instrux->Length += MoffsetSize;
    if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
    {
        return ND_STATUS_INSTRUCTION_TOO_LONG;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdFetchSseImmediate
//
static NDSTATUS
NdFetchSseImmediate(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size,
    uint8_t SseImmSize
    )
{
    RET_GT((size_t)Offset + SseImmSize, Size, ND_STATUS_BUFFER_TOO_SMALL);

    Instrux->HasSseImm = true;
    Instrux->SseImmOffset = Offset;
    Instrux->SseImmediate = *(Code + Offset);

    Instrux->Length += SseImmSize;
    if (Instrux->Length > ND_MAX_INSTRUCTION_LENGTH)
    {
        return ND_STATUS_INSTRUCTION_TOO_LONG;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdGetSegOverride
//
static uint8_t
NdGetSegOverride(
    INSTRUX *Instrux,
    uint8_t DefaultSeg
    )
{
    // In 64 bit mode, the segment override is ignored, except for FS and GS.
    if ((Instrux->DefCode == ND_CODE_64) &&
        (Instrux->Seg != ND_PREFIX_G2_SEG_FS) &&
        (Instrux->Seg != ND_PREFIX_G2_SEG_GS))
    {
        return DefaultSeg;
    }

    switch (Instrux->Seg)
    {
    case ND_PREFIX_G2_SEG_CS:
        return NDR_CS;
    case ND_PREFIX_G2_SEG_DS:
        return NDR_DS;
    case ND_PREFIX_G2_SEG_ES:
        return NDR_ES;
    case ND_PREFIX_G2_SEG_SS:
        return NDR_SS;
    case ND_PREFIX_G2_SEG_FS:
        return NDR_FS;
    case ND_PREFIX_G2_SEG_GS:
        return NDR_GS;
    default:
        return DefaultSeg;
    }
}


//
// NdGetCompDispSize
//
static uint8_t
NdGetCompDispSize(
    const INSTRUX *Instrux,
    uint32_t MemSize
    )
{
    static const uint8_t fvszLut[2][2][4] =
    {
        { { 16, 32, 64, 0 }, { 4, 4, 4, 0 }, },
        { { 16, 32, 64, 0 }, { 8, 8, 8, 0 }, },
    };

    static const uint8_t hvszLut[2][4] =  { { 8, 16, 32, 0 }, { 4, 4, 4, 0 }, };
    static const uint8_t dupszLut[4] = { 8, 32, 64, 0 };
    static const uint8_t fvmszLut[4] = { 16, 32, 64, 0 };
    static const uint8_t hvmszLut[4] = { 8, 16, 32, 0 };
    static const uint8_t qvmszLut[4] = { 4, 8, 16, 0 };
    static const uint8_t ovmszLut[4] = { 2, 4, 8, 0 };

    switch (Instrux->TupleType)
    {
    case ND_TUPLE_FV:
        return fvszLut[Instrux->Exs.w][Instrux->Exs.bm][Instrux->Exs.l];
    case ND_TUPLE_HV:
        return hvszLut[Instrux->Exs.bm][Instrux->Exs.l];
    case ND_TUPLE_DUP:
        return dupszLut[Instrux->Exs.l];
    case ND_TUPLE_FVM:
        return fvmszLut[Instrux->Exs.l];
    case ND_TUPLE_HVM:
        return hvmszLut[Instrux->Exs.l];
    case ND_TUPLE_QVM:
        return qvmszLut[Instrux->Exs.l];
    case ND_TUPLE_OVM:
        return ovmszLut[Instrux->Exs.l];
    case ND_TUPLE_M128:
        return 16;
    case ND_TUPLE_T1S8:
        return 1;
    case ND_TUPLE_T1S16:
        return 2;
    case ND_TUPLE_T1S:
        return !!(Instrux->Attributes & ND_FLAG_WIG) ? 4 : Instrux->Exs.w ? 8 : 4;
    case ND_TUPLE_T1F:
        return (uint8_t)MemSize;
    case ND_TUPLE_T2:
        return Instrux->Exs.w ? 16 : 8;
    case ND_TUPLE_T4:
        return Instrux->Exs.w ? 32 : 16;
    case ND_TUPLE_T8:
        return 32;
    case ND_TUPLE_T1_4X:
        return 16;
    default:
        // Default - we assume byte granularity for memory accesses, therefore, no scaling will be done.
        return 1;
    }
}


//
// NdParseOperand
//
static NDSTATUS
NdParseOperand(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size,
    uint32_t Index,
    uint64_t Specifier
    )
{
    NDSTATUS status;
    PND_OPERAND operand;
    uint8_t opt, ops, opf, opa, opd, opb;
    ND_REG_SIZE vsibRegSize;
    uint8_t vsibIndexSize, vsibIndexCount;
    ND_OPERAND_SIZE size, bcstSize;
    bool width;

    // pre-init
    status = ND_STATUS_SUCCESS;
    vsibRegSize = 0;
    vsibIndexSize = vsibIndexCount = 0;
    size = bcstSize = 0;

    // Get actual width.
    width = Instrux->Exs.w && !(Instrux->Attributes & ND_FLAG_WIG);

    // Get operand components.
    opt = ND_OP_TYPE(Specifier);
    ops = ND_OP_SIZE(Specifier);
    opf = ND_OP_FLAGS(Specifier);
    opa = ND_OP_ACCESS(Specifier);
    opd = ND_OP_DECORATORS(Specifier);
    opb = ND_OP_BLOCK(Specifier);

    // Get a pointer to our op.
    operand = &Instrux->Operands[Index];

    // Fill in the flags.
    operand->Flags.Flags = opf;

    // Store operand access modes.
    operand->Access.Access = opa;


    //
    // Fill in operand size.
    //
    switch (ops)
    {
    case ND_OPS_asz:
        // Size given by the address mode.
        size = 2 << Instrux->AddrMode;
        break;

    case ND_OPS_ssz:
        // Size given by the stack mode.
        size = 2 << Instrux->DefStack;
        break;

    case ND_OPS_0:
        // No memory access. 0 operand size.
        size = 0;
        break;

    case ND_OPS_b:
        // Byte, regardless of operand-size attribute.
        size = ND_SIZE_8BIT;
        break;

    case ND_OPS_w:
        // Word, regardless of operand-size attribute.
        size = ND_SIZE_16BIT;
        break;

    case ND_OPS_d:
        // Dword, regardless of operand-size attribute.
        size = ND_SIZE_32BIT;
        break;

    case ND_OPS_q:
        // Qword, regardless of operand-size attribute.
        size = ND_SIZE_64BIT;
        break;

    case ND_OPS_dq:
        // Double-Qword, regardless of operand-size attribute.
        size = ND_SIZE_128BIT;
        break;

    case ND_OPS_qq:
        // Quad-Quadword (256-bits), regardless of operand-size attribute.
        size = ND_SIZE_256BIT;
        break;

    case ND_OPS_oq:
        // Octo-Quadword (512-bits), regardless of operand-size attribute.
        size = ND_SIZE_512BIT;
        break;

    case ND_OPS_fa:
        // 80 bits packed BCD.
        size = ND_SIZE_80BIT;
        break;

    case ND_OPS_fw:
        // 16 bits real number.
        size = ND_SIZE_16BIT;
        break;

    case ND_OPS_fd:
        // 32 bit real number.
        size = ND_SIZE_32BIT;
        break;

    case ND_OPS_fq:
        // 64 bit real number.
        size = ND_SIZE_64BIT;
        break;

    case ND_OPS_ft:
        // 80 bit real number.
        size = ND_SIZE_80BIT;
        break;

    case ND_OPS_fe:
        // 14 bytes or 28 bytes FPU environment.
        size = (Instrux->EfOpMode == ND_OPSZ_16) ? ND_SIZE_112BIT : ND_SIZE_224BIT;
        break;

    case ND_OPS_fs:
        // 94 bytes or 108 bytes FPU state.
        size = (Instrux->EfOpMode == ND_OPSZ_16) ? ND_SIZE_752BIT : ND_SIZE_864BIT;
        break;

    case ND_OPS_rx:
        // 512 bytes extended state.
        size = ND_SIZE_4096BIT;
        break;

    case ND_OPS_cl:
        // The size of one cache line.
        size = ND_SIZE_CACHE_LINE;
        break;

    case ND_OPS_v:
        // Word, doubleword or quadword (in 64-bit mode), depending on operand-size attribute.
        {
            static const uint8_t szLut[3] = { ND_SIZE_16BIT, ND_SIZE_32BIT, ND_SIZE_64BIT };

            size = szLut[Instrux->EfOpMode];
        }
        break;

    case ND_OPS_y:
        // Doubleword or quadword (in 64-bit mode), depending on operand-size attribute.
        {
            static const uint8_t szLut[3] = { ND_SIZE_32BIT, ND_SIZE_32BIT, ND_SIZE_64BIT };

            size = szLut[Instrux->EfOpMode];
        }
        break;

    case ND_OPS_yf:
        // Always uint64_t in 64 bit mode and uint32_t in 16/32 bit mode.
        {
            static const uint8_t szLut[3] = { ND_SIZE_32BIT, ND_SIZE_32BIT, ND_SIZE_64BIT };

            size = szLut[Instrux->DefCode];
        }
        break;

    case ND_OPS_z:
        // Word for 16-bit operand-size or double word for 32 or 64-bit operand-size.
        {
            static const uint8_t szLut[3] = { ND_SIZE_16BIT, ND_SIZE_32BIT, ND_SIZE_32BIT };

            size = szLut[Instrux->EfOpMode];
        }
        break;

    case ND_OPS_a:
        // Two one-word operands in memory or two double-word operands in memory,
        // depending on operand-size attribute (used only by the BOUND instruction).
        {
            static const uint8_t szLut[3] = { ND_SIZE_16BIT * 2, ND_SIZE_32BIT * 2, 0 };

            if (Instrux->DefCode > ND_CODE_32)
            {
                return ND_STATUS_INVALID_INSTRUX;
            }

            size = szLut[Instrux->EfOpMode];
        }
        break;

    case ND_OPS_c:
        // Byte or word, depending on operand-size attribute.
        switch (Instrux->DefCode)
        {
        case ND_CODE_16:
            size = Instrux->HasOpSize ? ND_SIZE_16BIT : ND_SIZE_8BIT;
            break;
        case ND_CODE_32:
            size = Instrux->HasOpSize ? ND_SIZE_16BIT : ND_SIZE_32BIT;
            break;
        case ND_CODE_64:
            size = ND_SIZE_64BIT;
            break;
        default:
            return ND_STATUS_INVALID_INSTRUX;
        }
        break;

    case ND_OPS_p:
        // 32-bit, 48-bit, or 80-bit pointer, depending on operand-size attribute.
        {
            static const uint8_t szLut[3] = { ND_SIZE_32BIT, ND_SIZE_48BIT, ND_SIZE_80BIT };

            size = szLut[Instrux->EfOpMode];
        }
        break;

    case ND_OPS_s:
        // 6-byte or 10-byte pseudo-descriptor.
        {
            static const uint8_t szLut[3] = { ND_SIZE_48BIT, ND_SIZE_48BIT, ND_SIZE_80BIT };

            size = szLut[Instrux->DefCode];
        }
        break;

    case ND_OPS_l:
        // 64 bit in 16 or 32 bit mode, 128 bit in long mode. Used by BNDMOV instruction.
        {
            static const uint8_t szLut[3] = { ND_SIZE_64BIT, ND_SIZE_64BIT, ND_SIZE_128BIT };

            size = szLut[Instrux->DefCode];
        }
        break;

    case ND_OPS_x:
        // dq, qq or oq based on the operand-size attribute.
        {
            static const uint8_t szLut[3] = { ND_SIZE_128BIT, ND_SIZE_256BIT, ND_SIZE_512BIT };

            size = szLut[Instrux->EfVecMode];
        }
        break;

    case ND_OPS_n:
        // 128, 256 or 512 bit, depending on vector length.
        {
            static const uint8_t szLut[3] = { ND_SIZE_128BIT, ND_SIZE_256BIT, ND_SIZE_512BIT };

            size = szLut[Instrux->EfVecMode];
        }
        break;

    case ND_OPS_u:
        // 256 or 512 bit, depending on vector length.
        {
            static const uint8_t szLut[3] = { 0, ND_SIZE_256BIT, ND_SIZE_512BIT };

            if (ND_VECM_128 == Instrux->EfVecMode)
            {
                return ND_STATUS_INVALID_INSTRUX;
            }

            size = szLut[Instrux->EfVecMode];
        }
        break;

    case ND_OPS_e:
        // eighth = word or dword or qword
        {
            static const uint8_t szLut[3] = { ND_SIZE_16BIT, ND_SIZE_32BIT, ND_SIZE_64BIT };

            size = szLut[Instrux->EfVecMode];
        }
        break;

    case ND_OPS_f:
        // fourth = dword or qword or oword
        {
            static const uint8_t szLut[3] = { ND_SIZE_32BIT, ND_SIZE_64BIT, ND_SIZE_128BIT };

            size = szLut[Instrux->EfVecMode];
        }
        break;

    case ND_OPS_h:
        // half = qword or oword or yword
        {
            static const uint8_t szLut[3] = { ND_SIZE_64BIT, ND_SIZE_128BIT, ND_SIZE_256BIT };

            size = szLut[Instrux->EfVecMode];
        }
        break;

    case ND_OPS_pd:
    case ND_OPS_ps:
        // packed double or packed single precision values.
        {
            static const uint8_t szLut[3] = { ND_SIZE_128BIT, ND_SIZE_256BIT, ND_SIZE_512BIT };

            size = szLut[Instrux->EfVecMode];
        }
        break;

    case ND_OPS_ss:
        // Scalar single.
        size = ND_SIZE_32BIT;
        break;

    case ND_OPS_sd:
        // Scalar double.
        size = ND_SIZE_64BIT;
        break;

    case ND_OPS_mib:
        // MIB addressing, used by MPX instructions.
        size = 0;
        break;

    case ND_OPS_vm32x:
    case ND_OPS_vm32y:
    case ND_OPS_vm32z:
        // 32 bit indexes from XMM, YMM or ZMM register.
        vsibIndexSize  = ND_SIZE_32BIT;
        vsibIndexCount = (Instrux->Exs.l == 0) ? 4 : ((Instrux->Exs.l == 1) ? 8 : 16);
        vsibRegSize = (ops == ND_OPS_vm32x) ? ND_SIZE_128BIT :
                      (ops == ND_OPS_vm32y) ? ND_SIZE_256BIT :
                                              ND_SIZE_512BIT;
        size = vsibIndexCount * (width ? ND_SIZE_64BIT : ND_SIZE_32BIT);
        break;

    case ND_OPS_vm32h:
        // 32 bit indexes from XMM or YMM.
        vsibIndexSize = ND_SIZE_32BIT;
        vsibIndexCount = (Instrux->Exs.l == 0) ? 2 : ((Instrux->Exs.l == 1) ? 4 : 8);
        vsibRegSize = (Instrux->Exs.l == 0) ? ND_SIZE_128BIT :
                      (Instrux->Exs.l == 1) ? ND_SIZE_128BIT :
                                              ND_SIZE_256BIT;
        size = vsibIndexCount * (width ? ND_SIZE_64BIT : ND_SIZE_32BIT);
        break;

    case ND_OPS_vm32n:
        // 32 bit indexes from XMM, YMM or ZMM register.
        vsibIndexSize = ND_SIZE_32BIT;
        vsibIndexCount = (Instrux->Exs.l == 0) ? 4 : ((Instrux->Exs.l == 1) ? 8 : 16);
        vsibRegSize = (Instrux->Exs.l == 0) ? ND_SIZE_128BIT :
                      (Instrux->Exs.l == 1) ? ND_SIZE_256BIT :
                                              ND_SIZE_512BIT;
        size = vsibIndexCount * (width ? ND_SIZE_64BIT : ND_SIZE_32BIT);
        break;

    case ND_OPS_vm64x:
    case ND_OPS_vm64y:
    case ND_OPS_vm64z:
        // 64 bit indexes from XMM, YMM or ZMM register.
        vsibIndexSize = ND_SIZE_64BIT;
        vsibIndexCount = (Instrux->Exs.l == 0) ? 2 : ((Instrux->Exs.l == 1) ? 4 : 8);
        vsibRegSize = (ops == ND_OPS_vm64x) ? ND_SIZE_128BIT :
                      (ops == ND_OPS_vm64y) ? ND_SIZE_256BIT :
                                              ND_SIZE_512BIT;
        size = vsibIndexCount * (width ? ND_SIZE_64BIT : ND_SIZE_32BIT);
        break;

    case ND_OPS_vm64h:
        // 64 bit indexes from XMM or YMM.
        vsibIndexSize = ND_SIZE_64BIT;
        vsibIndexCount = (Instrux->Exs.l == 0) ? 1 : ((Instrux->Exs.l == 1) ? 2 : 4);
        vsibRegSize = (Instrux->Exs.l == 0) ? ND_SIZE_128BIT :
                      (Instrux->Exs.l == 1) ? ND_SIZE_128BIT :
                                              ND_SIZE_256BIT;
        size = vsibIndexCount * (width ? ND_SIZE_64BIT : ND_SIZE_32BIT);
        break;

    case ND_OPS_vm64n:
        // 64 bit indexes from XMM, YMM or ZMM register.
        vsibIndexSize = ND_SIZE_64BIT;
        vsibIndexCount = (Instrux->Exs.l == 0) ? 2 : ((Instrux->Exs.l == 1) ? 4 : 8);
        vsibRegSize = (Instrux->Exs.l == 0) ? ND_SIZE_128BIT :
                      (Instrux->Exs.l == 1) ? ND_SIZE_256BIT :
                                              ND_SIZE_512BIT;
        size = vsibIndexCount * (width ? ND_SIZE_64BIT : ND_SIZE_32BIT);
        break;

    case ND_OPS_v2:
    case ND_OPS_v3:
    case ND_OPS_v4:
    case ND_OPS_v8:
        // Multiple words accessed.
        {
            static const uint8_t szLut[3] = { ND_SIZE_16BIT, ND_SIZE_32BIT, ND_SIZE_64BIT };
            uint8_t scale = 1;

            scale = (ops == ND_OPS_v2) ? 2 : ((ops == ND_OPS_v3) ? 3 : ((ops == ND_OPS_v4) ? 4 : 8));

            size =  scale * szLut[Instrux->EfOpMode];
        }
        break;

    case ND_OPS_12:
        // SAVPREVSSP instruction reads/writes 4 + 8 bytes from the shadow stack.
        size = 12;
        break;

    case ND_OPS_t:
        // Tile register. The actual size depends on how the TILECFG register has been programmed, but it can be 
        // up to 1K in size.
        size = ND_SIZE_1KB;
        break;

    case ND_OPS_384:
        // 384 bit Key Locker handle.
        size = ND_SIZE_384BIT;
        break;

    case ND_OPS_512:
        // 512 bit Key Locker handle.
        size = ND_SIZE_512BIT;
        break;

    case ND_OPS_unknown:
        size = ND_SIZE_UNKNOWN;
        break;

    default:
        return ND_STATUS_INVALID_INSTRUX;
    }

    // Store operand info.
    operand->Size = operand->RawSize = bcstSize = size;

    //
    // Fill in the operand type.
    //
    switch (opt)
    {
    case ND_OPT_CONST_1:
        // operand is an implicit constant (used by shift/rotate instruction).
        operand->Type = ND_OP_CONST;
        operand->Info.Constant.Const = 1;
        break;

    case ND_OPT_RIP:
        // The operand is the instruction pointer.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_RIP;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = 0;
        Instrux->RipAccess |= operand->Access.Access;
        break;

    case ND_OPT_GPR_rAX:
        // Operator is the accumulator.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_RAX;
        break;

    case ND_OPT_GPR_AH:
        // Operator is the accumulator.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = ND_SIZE_8BIT;
        operand->Info.Register.Reg = NDR_AH;
        operand->Info.Register.IsHigh8 = true;
        break;

    case ND_OPT_GPR_rCX:
        // Operator is the counter register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_RCX;
        break;

    case ND_OPT_GPR_rDX:
        // Operator is rDX.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_RDX;
        break;

    case ND_OPT_GPR_rBX:
        // Operator is BX.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_RBX;
        break;

    case ND_OPT_GPR_rBP:
        // Operand is rBP.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_RBP;
        break;

    case ND_OPT_GPR_rSP:
        // Operand is rSP.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_RSP;
        break;

    case ND_OPT_GPR_rSI:
        // Operand is rSI.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_RSI;
        break;

    case ND_OPT_GPR_rDI:
        // Operand is rDI.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_RDI;
        break;

    case ND_OPT_GPR_rR8:
        // Operand is R8.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_R8;
        break;

    case ND_OPT_GPR_rR9:
        // Operand is R9.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_R9;
        break;

    case ND_OPT_GPR_rR11:
        // Operand is R11.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_R11;
        break;

    case ND_OPT_SEG_CS:
        // Operand is the CS register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SEG;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_CS;
        break;

    case ND_OPT_SEG_SS:
        // Operand is the SS register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SEG;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_SS;
        break;

    case ND_OPT_SEG_DS:
        // Operand is the DS register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SEG;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_DS;
        break;

    case ND_OPT_SEG_ES:
        // Operand is the ES register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SEG;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_ES;
        break;

    case ND_OPT_SEG_FS:
        // Operand is the FS register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SEG;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_FS;
        break;

    case ND_OPT_SEG_GS:
        // Operand is the GS register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SEG;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_GS;
        break;

    case ND_OPT_FPU_ST0:
        // Operand is the ST(0) register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_FPU;
        operand->Info.Register.Size = ND_SIZE_80BIT;
        operand->Info.Register.Reg = 0;
        break;

    case ND_OPT_FPU_STX:
        // Operand is the ST(i) register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_FPU;
        operand->Info.Register.Size = ND_SIZE_80BIT;
        operand->Info.Register.Reg = Instrux->ModRm.rm;
        break;

    case ND_OPT_SSE_XMM0:
    case ND_OPT_SSE_XMM1:
    case ND_OPT_SSE_XMM2:
    case ND_OPT_SSE_XMM3:
    case ND_OPT_SSE_XMM4:
    case ND_OPT_SSE_XMM5:
    case ND_OPT_SSE_XMM6:
    case ND_OPT_SSE_XMM7:
        // Operand is a hard-coded XMM register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SSE;
        operand->Info.Register.Size = ND_SIZE_128BIT;
        operand->Info.Register.Reg = opt - ND_OPT_SSE_XMM0;
        break;

    // Special operands. These are always implicit, and can't be encoded inside the instruction.
    case ND_OPT_CR_0:
        // The operand is implicit and is control register 0.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_CR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_CR0;
        break;

    case ND_OPT_SYS_GDTR:
        // The operand is implicit and is the global descriptor table register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SYS;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_GDTR;
        break;

    case ND_OPT_SYS_IDTR:
        // The operand is implicit and is the interrupt descriptor table register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SYS;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_IDTR;
        break;

    case ND_OPT_SYS_LDTR:
        // The operand is implicit and is the local descriptor table register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SYS;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_LDTR;
        break;

    case ND_OPT_SYS_TR:
        // The operand is implicit and is the task register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SYS;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = NDR_TR;
        break;

    case ND_OPT_X87_CONTROL:
        // The operand is implicit and is the x87 control word.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SYS;
        operand->Info.Register.Size = ND_SIZE_16BIT;
        operand->Info.Register.Reg = NDR_X87_CONTROL;
        break;

    case ND_OPT_X87_TAG:
        // The operand is implicit and is the x87 tag word.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SYS;
        operand->Info.Register.Size = ND_SIZE_16BIT;
        operand->Info.Register.Reg = NDR_X87_TAG;
        break;

    case ND_OPT_X87_STATUS:
        // The operand is implicit and is the x87 status word.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SYS;
        operand->Info.Register.Size = ND_SIZE_16BIT;
        operand->Info.Register.Reg = NDR_X87_STATUS;
        break;

    case ND_OPT_MXCSR:
        // The operand is implicit and is the MXCSR.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MXCSR;
        operand->Info.Register.Size = ND_SIZE_32BIT;
        operand->Info.Register.Reg = 0;
        break;

    case ND_OPT_PKRU:
        // The operand is the PKRU register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_PKRU;
        operand->Info.Register.Size = ND_SIZE_32BIT;
        operand->Info.Register.Reg = 0;
        break;

    case ND_OPT_SSP:
        // The operand is the SSP register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SSP;
        operand->Info.Register.Size = operand->Size;
        operand->Info.Register.Reg = 0;
        break;

    case ND_OPT_UIF:
        // The operand is the User Interrupt Flag.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_UIF;
        operand->Info.Register.Size = ND_SIZE_8BIT; // 1 bit, in fact, but there is no size defined for one bit.
        operand->Info.Register.Reg = 0;
        break;

    case ND_OPT_MSR:
        // The operand is implicit and is a MSR (usually selected by the ECX register).
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = 0xFFFFFFFF;
        break;

    case ND_OPT_MSR_TSC:
        // The operand is implicit and is the IA32_TSC.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_TSC;
        break;

    case ND_OPT_MSR_TSCAUX:
        // The operand is implicit and is the IA32_TSCAUX.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_TSC_AUX;
        break;

    case ND_OPT_MSR_SCS:
        // The operand is implicit and is the IA32_SYSENTER_CS.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_SYSENTER_CS;
        break;

    case ND_OPT_MSR_SESP:
        // The operand is implicit and is the IA32_SYSENTER_ESP.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_SYSENTER_ESP;
        break;

    case ND_OPT_MSR_SEIP:
        // The operand is implicit and is the IA32_SYSENTER_EIP.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_SYSENTER_EIP;
        break;

    case ND_OPT_MSR_STAR:
        // The operand is implicit and is the IA32_STAR.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_STAR;
        break;

    case ND_OPT_MSR_LSTAR:
        // The operand is implicit and is the IA32_STAR.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_LSTAR;
        break;

    case ND_OPT_MSR_FMASK:
        // The operand is implicit and is the IA32_FMASK.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_FMASK;
        break;

    case ND_OPT_MSR_FSBASE:
        // The operand is implicit and is the IA32_FS_BASE MSR.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_FS_BASE;
        break;

    case ND_OPT_MSR_GSBASE:
        // The operand is implicit and is the IA32_GS_BASE MSR.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_GS_BASE;
        break;

    case ND_OPT_MSR_KGSBASE:
        // The operand is implicit and is the IA32_KERNEL_GS_BASE MSR.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = NDR_IA32_KERNEL_GS_BASE;
        break;

    case ND_OPT_XCR:
        // The operand is implicit and is an extended control register (usually selected by ECX register).
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_XCR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = 0xFF;
        break;

    case ND_OPT_XCR_0:
        // The operand is implicit and is XCR0.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_XCR;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = 0;
        break;

    case ND_OPT_REG_BANK:
        // Multiple registers are accessed.
        if ((Instrux->Instruction == ND_INS_PUSHA) || (Instrux->Instruction == ND_INS_POPA))
        {
            operand->Type = ND_OP_REG;
            operand->Size = operand->RawSize = Instrux->WordLength;
            operand->Info.Register.Type = ND_REG_GPR;
            operand->Info.Register.Size = Instrux->WordLength;
            operand->Info.Register.Reg = NDR_EAX;
            operand->Info.Register.Count = 8;
            operand->Info.Register.IsBlock = true;
        }
        else
        {
            operand->Type = ND_OP_BANK;
        }
        break;

    case ND_OPT_A:
        // Fetch the address. NOTE: The size can't be larger than 8 bytes.
        status = NdFetchAddress(Instrux, Code, Offset, Size, (uint8_t)size);
        if (!ND_SUCCESS(status))
        {
            return status;
        }

        // Fill in operand info.
        operand->Type = ND_OP_ADDR;
        operand->Info.Address.BaseSeg = Instrux->Address.Cs;
        operand->Info.Address.Offset = Instrux->Address.Ip;
        break;

    case ND_OPT_B:
        // General purpose register encoded in VEX.vvvv field.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;

        // EVEX.V' must be 0, if a GPR is encoded using EVEX encoding.
        if (Instrux->Exs.vp != 0)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }

        operand->Info.Register.Reg = (uint8_t)Instrux->Exs.v;
        break;

    case ND_OPT_C:
        // Control register, encoded in modrm.reg.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_CR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.r << 3) | Instrux->ModRm.reg);
        // On some AMD processors, the presence of the LOCK prefix before MOV to/from control registers allows accessing
        // higher 8 control registers.
        if ((ND_CODE_64 != Instrux->DefCode) && (Instrux->HasLock))
        {
            operand->Info.Register.Reg |= 0x8;
        }

        // Only CR0, CR2, CR3, CR4 & CR8 valid.
        if (operand->Info.Register.Reg != 0 &&
            operand->Info.Register.Reg != 2 &&
            operand->Info.Register.Reg != 3 &&
            operand->Info.Register.Reg != 4 &&
            operand->Info.Register.Reg != 8)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }

        break;

    case ND_OPT_D:
        // Debug register, encoded in modrm.reg.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_DR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.r << 3) | Instrux->ModRm.reg);

        // Only DR0-DR7 valid.
        if (operand->Info.Register.Reg >= 8)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }

        break;

    case ND_OPT_T:
        // Test register, encoded in modrm.reg.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_TR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.r << 3) | Instrux->ModRm.reg);

        // Only TR0-TR7 valid, only on 486.
        if (operand->Info.Register.Reg >= 8)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }

        break;

    case ND_OPT_S:
        // Segment register, encoded in modrm.reg.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SEG;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = Instrux->ModRm.reg;

        // Only ES, CS, SS, DS, FS, GS valid.
        if (operand->Info.Register.Reg >= 6)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }

        // If CS is loaded - #UD.
        if ((operand->Info.Register.Reg == NDR_CS) && operand->Access.Write)
        {
            return ND_STATUS_CS_LOAD;
        }

        break;

    case ND_OPT_E:
        // General purpose register or memory, encoded in modrm.rm.
        if (Instrux->ModRm.mod == 3)
        {
            operand->Type = ND_OP_REG;
            operand->Info.Register.Type = ND_REG_GPR;
            operand->Info.Register.Size = (ND_REG_SIZE)size;
            operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.b << 3) | Instrux->ModRm.rm);
            operand->Info.Register.IsHigh8 = (operand->Info.Register.Size == 1) &&
                                             (operand->Info.Register.Reg  >= 4) &&
                                             (ND_ENCM_LEGACY == Instrux->EncMode) &&
                                             !Instrux->HasRex;
        }
        else
        {
            goto memory;
        }
        break;

    case ND_OPT_F:
        // The flags register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_FLG;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = 0;
        Instrux->FlagsAccess.RegAccess |= operand->Access.Access;
        break;

    case ND_OPT_K:
        // The operand is the stack.
        {
            static const uint8_t szLut[3] = { ND_SIZE_16BIT, ND_SIZE_32BIT, ND_SIZE_64BIT };

            Instrux->MemoryAccess |= operand->Access.Access;
            operand->Type = ND_OP_MEM;
            operand->Info.Memory.IsStack = true;
            operand->Info.Memory.HasBase = true;
            operand->Info.Memory.Base = NDR_RSP;
            operand->Info.Memory.BaseSize = szLut[Instrux->DefStack];
            operand->Info.Memory.HasSeg = true;
            operand->Info.Memory.Seg = NDR_SS;
            Instrux->StackWords = (uint8_t)(operand->Size / Instrux->WordLength);
            Instrux->StackAccess |= operand->Access.Access;
        }
        break;

    case ND_OPT_G:
        // General purpose register encoded in modrm.reg.
        if (Instrux->Exs.rp == 1)
        {
            return ND_STATUS_INVALID_ENCODING;
        }

        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;

        // EVEX.R' must be 0 if a GPR is encoded.
        if (Instrux->Exs.rp != 0)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }

        operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.r << 3) | Instrux->ModRm.reg);
        operand->Info.Register.IsHigh8 = (operand->Info.Register.Size == 1) &&
                                         (operand->Info.Register.Reg >= 4) &&
                                         (ND_ENCM_LEGACY == Instrux->EncMode) &&
                                         !Instrux->HasRex;
        break;

    case ND_OPT_R:
        // General purpose register encoded in modrm.rm.
        if ((Instrux->ModRm.mod == 3) || (0 != (Instrux->Attributes & ND_FLAG_MFR)))
        {
            operand->Type = ND_OP_REG;
            operand->Info.Register.Type = ND_REG_GPR;
            operand->Info.Register.Size = (ND_REG_SIZE)size;
            operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.b << 3) | Instrux->ModRm.rm);
            operand->Info.Register.IsHigh8 = (operand->Info.Register.Size == 1) &&
                                             (operand->Info.Register.Reg >= 4) &&
                                             (ND_ENCM_LEGACY == Instrux->EncMode) &&
                                             !Instrux->HasRex;
        }
        else
        {
            return ND_STATUS_INVALID_ENCODING;
        }
        break;

    case ND_OPT_I:
        // Immediate, encoded in instructon bytes.
        {
            uint64_t imm;

            // Fetch the immediate. NOTE: The size won't exceed 8 bytes.
            status = NdFetchImmediate(Instrux, Code, Offset, Size, (uint8_t)size);
            if (!ND_SUCCESS(status))
            {
                return status;
            }

            // Get the last immediate.
            if (Instrux->HasImm3)
            {
                imm = Instrux->Immediate3;
            }
            else if (Instrux->HasImm2)
            {
                imm = Instrux->Immediate2;
            }
            else
            {
                imm = Instrux->Immediate1;
            }

            operand->Type = ND_OP_IMM;

            if (operand->Flags.SignExtendedDws)
            {
                static const uint8_t wszLut[3] = { ND_SIZE_16BIT, ND_SIZE_32BIT, ND_SIZE_64BIT };

                // Get the default word size: the immediate is sign extended to the default word size.
                operand->Size = wszLut[Instrux->EfOpMode];

                operand->Info.Immediate.Imm = ND_SIGN_EX(size, imm);
            }
            else if (operand->Flags.SignExtendedOp1)
            {
                // The immediate is sign extended to the size of the first operand.
                operand->Size = Instrux->Operands[0].Size;

                operand->Info.Immediate.Imm = ND_SIGN_EX(size, imm);
            }
            else
            {
                operand->Info.Immediate.Imm = imm;
            }
        }
        break;

    case ND_OPT_Im2z:
        {
            operand->Type = ND_OP_IMM;
            operand->Info.Immediate.Imm = Instrux->SseImmediate & 3;
        }
        break;

    case ND_OPT_J:
        // Fetch the relative offset. NOTE: The size of the relative can't exceed 4 bytes.
        status = NdFetchRelativeOffset(Instrux, Code, Offset, Size, (uint8_t)size);
        if (!ND_SUCCESS(status))
        {
            return status;
        }

        // The instruction is RIP relative.
        Instrux->IsRipRelative = true;

        operand->Type = ND_OP_OFFS;
        // The relative offset is forced to the default word length. Care must be taken with the 32 bit
        // branches that have 0x66 prefix (in 32 bit mode)!
        operand->Size = Instrux->WordLength;
        operand->Info.RelativeOffset.Rel = ND_SIGN_EX(size, Instrux->RelativeOffset);

        break;

    case ND_OPT_N:
        // The R/M field of the ModR/M byte selects a packed-quadword, MMX technology register.
        if (Instrux->ModRm.mod != 3)
        {
            return ND_STATUS_INVALID_ENCODING;
        }

        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MMX;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = Instrux->ModRm.rm;
        break;

    case ND_OPT_P:
        // The reg field of the ModR/M byte selects a packed quadword MMX technology register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MMX;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = Instrux->ModRm.reg;
        break;

    case ND_OPT_Q:
        // A ModR/M byte follows the opcode and specifies the operand. The operand is either an MMX technology
        // register or a memory address. If it is a memory address, the address is computed from a segment register
        // and any of the following values: a base register, an index register, a scaling factor, and a displacement
        if (Instrux->ModRm.mod == 3)
        {
            operand->Type = ND_OP_REG;
            operand->Info.Register.Type = ND_REG_MMX;
            operand->Info.Register.Size = ND_SIZE_64BIT;
            operand->Info.Register.Reg = Instrux->ModRm.rm;
        }
        else
        {
            goto memory;
        }
        break;

    case ND_OPT_O:
        // Absolute address, encoded in instruction bytes.
        {
            // NOTE: The moffset len can't exceed 8 bytes.
            status = NdFetchMoffset(Instrux, Code, Offset, Size, 2 << Instrux->AddrMode);
            if (!ND_SUCCESS(status))
            {
                return status;
            }

            // operand info.
            Instrux->MemoryAccess |= operand->Access.Access;
            operand->Type = ND_OP_MEM;
            operand->Info.Memory.HasDisp = true;
            operand->Info.Memory.IsDirect = true;
            operand->Info.Memory.DispSize = Instrux->MoffsetLength;
            operand->Info.Memory.Disp = Instrux->Moffset;
            operand->Info.Memory.HasSeg = true;
            operand->Info.Memory.Seg = NdGetSegOverride(Instrux, NDR_DS);
        }
        break;

    case ND_OPT_M:
        // Modrm based memory addressing.
        if (Instrux->ModRm.mod == 3)
        {
            return ND_STATUS_INVALID_ENCODING;
        }

memory:
        Instrux->MemoryAccess |= operand->Access.Access;
        operand->Type = ND_OP_MEM;
        operand->Info.Memory.HasSeg = true;

        if (ND_ADDR_16 == Instrux->AddrMode)
        {
            // 16 bit addressing, make sure the instruction supports this.
            if (!!(Instrux->Attributes & ND_FLAG_NOA16))
            {
                return ND_STATUS_16_BIT_ADDRESSING_NOT_SUPPORTED;
            }

            switch (Instrux->ModRm.rm)
            {
            case 0:
                // [bx + si]
                operand->Info.Memory.HasBase = true;
                operand->Info.Memory.HasIndex = true;
                operand->Info.Memory.Scale = 1;
                operand->Info.Memory.Base = NDR_BX;
                operand->Info.Memory.Index = NDR_SI;
                operand->Info.Memory.BaseSize = ND_SIZE_16BIT;
                operand->Info.Memory.IndexSize = ND_SIZE_16BIT;
                operand->Info.Memory.Seg = NDR_DS;
                break;
            case 1:
                // [bx + di]
                operand->Info.Memory.HasBase = true;
                operand->Info.Memory.HasIndex = true;
                operand->Info.Memory.Scale = 1;
                operand->Info.Memory.Base = NDR_BX;
                operand->Info.Memory.Index = NDR_DI;
                operand->Info.Memory.BaseSize = ND_SIZE_16BIT;
                operand->Info.Memory.IndexSize = ND_SIZE_16BIT;
                operand->Info.Memory.Seg = NDR_DS;
                break;
            case 2:
                // [bp + si]
                operand->Info.Memory.HasBase = true;
                operand->Info.Memory.HasIndex = true;
                operand->Info.Memory.Scale = 1;
                operand->Info.Memory.Base = NDR_BP;
                operand->Info.Memory.Index = NDR_SI;
                operand->Info.Memory.BaseSize = ND_SIZE_16BIT;
                operand->Info.Memory.IndexSize = ND_SIZE_16BIT;
                operand->Info.Memory.Seg = NDR_SS;
                break;
            case 3:
                // [bp + di]
                operand->Info.Memory.HasBase = true;
                operand->Info.Memory.HasIndex = true;
                operand->Info.Memory.Scale = 1;
                operand->Info.Memory.Base = NDR_BP;
                operand->Info.Memory.Index = NDR_DI;
                operand->Info.Memory.BaseSize = ND_SIZE_16BIT;
                operand->Info.Memory.IndexSize = ND_SIZE_16BIT;
                operand->Info.Memory.Seg = NDR_SS;
                break;
            case 4:
                // [si]
                operand->Info.Memory.HasBase = true;
                operand->Info.Memory.Base = NDR_SI;
                operand->Info.Memory.BaseSize = ND_SIZE_16BIT;
                operand->Info.Memory.Seg = NDR_DS;
                break;
            case 5:
                // [di]
                operand->Info.Memory.HasBase = true;
                operand->Info.Memory.Base = NDR_DI;
                operand->Info.Memory.BaseSize = ND_SIZE_16BIT;
                operand->Info.Memory.Seg = NDR_DS;
                break;
            case 6:
                // [bp]
                if (Instrux->ModRm.mod != 0)
                {
                    // If mod is not zero, than we have "[bp + displacement]".
                    operand->Info.Memory.HasBase = true;
                    operand->Info.Memory.Base = NDR_BP;
                    operand->Info.Memory.BaseSize = ND_SIZE_16BIT;
                    operand->Info.Memory.Seg = NDR_SS;
                }
                else
                {
                    // If mod is zero, than we only have a displacement that is used to directly address mem.
                    operand->Info.Memory.Seg = NDR_DS;
                }
                break;
            case 7:
                // [bx]
                operand->Info.Memory.HasBase = true;
                operand->Info.Memory.Base = NDR_BX;
                operand->Info.Memory.BaseSize = ND_SIZE_16BIT;
                operand->Info.Memory.Seg = NDR_DS;
                break;
            }

            // Store the displacement.
            operand->Info.Memory.HasDisp = !!Instrux->HasDisp;
            operand->Info.Memory.DispSize = Instrux->DispLength;
            operand->Info.Memory.Disp = ND_SIGN_EX(Instrux->DispLength, Instrux->Displacement);
        }
        else
        {
            uint8_t defsize = (Instrux->AddrMode == ND_ADDR_32 ? ND_SIZE_32BIT : ND_SIZE_64BIT);

            // Implicit segment is DS.
            operand->Info.Memory.Seg = NDR_DS;

            if (Instrux->HasSib)
            {
                // Check for base.
                if ((Instrux->ModRm.mod == 0) && (Instrux->Sib.base == NDR_RBP))
                {
                    // Mod is mem without displacement and base reg is RBP -> no base reg used.
                    // Note that this addressing mode is not RIP relative.
                }
                else
                {
                    operand->Info.Memory.HasBase = true;
                    operand->Info.Memory.BaseSize = defsize;
                    operand->Info.Memory.Base = (uint8_t)((Instrux->Exs.b << 3) | Instrux->Sib.base);

                    if ((operand->Info.Memory.Base == NDR_RSP) || (operand->Info.Memory.Base == NDR_RBP))
                    {
                        operand->Info.Memory.Seg = NDR_SS;
                    }
                }

                // Check for index.
                if ((((Instrux->Exs.x << 3) | Instrux->Sib.index) != NDR_RSP) || ND_HAS_VSIB(Instrux))
                {
                    // Index * Scale is present.
                    operand->Info.Memory.HasIndex = true;
                    operand->Info.Memory.IndexSize = defsize;
                    operand->Info.Memory.Index = (uint8_t)((Instrux->Exs.x << 3) | Instrux->Sib.index);

                    if (ND_HAS_VSIB(Instrux))
                    {
                        operand->Info.Memory.IndexSize = vsibRegSize;
                        operand->Info.Memory.Index |= Instrux->Exs.vp << 4;
                    }

                    operand->Info.Memory.Scale = 1 << Instrux->Sib.scale;
                }
            }
            else
            {
                if ((Instrux->ModRm.mod == 0) && (Instrux->ModRm.rm == NDR_RBP))
                {
                    //
                    // RIP relative addressing addresses a memory region relative to the current RIP; However,
                    // the current RIP, when executing the current instruction, is already updated and points
                    // to the next instruction, therefore, we must add the instruction length also to the final
                    // address. Note that RIP relative addressing is used even if the instruction uses 32 bit
                    // addressing, as long as we're in long mode.
                    //
                    operand->Info.Memory.IsRipRel = Instrux->IsRipRelative = (Instrux->DefCode == ND_CODE_64);

                    // Some instructions (example: MPX) don't support RIP relative addressing.
                    if (operand->Info.Memory.IsRipRel && !!(Instrux->Attributes & ND_FLAG_NO_RIP_REL))
                    {
                        return ND_STATUS_RIP_REL_ADDRESSING_NOT_SUPPORTED;
                    }
                }
                else
                {
                    operand->Info.Memory.HasBase = true;
                    operand->Info.Memory.BaseSize = defsize;
                    operand->Info.Memory.Base = (uint8_t)((Instrux->Exs.b << 3) | Instrux->ModRm.rm);

                    if ((operand->Info.Memory.Base == NDR_RSP) || (operand->Info.Memory.Base == NDR_RBP))
                    {
                        operand->Info.Memory.Seg = NDR_SS;
                    }
                }
            }

            operand->Info.Memory.HasDisp = Instrux->HasDisp;
            operand->Info.Memory.DispSize = Instrux->DispLength;
            operand->Info.Memory.Disp = ND_SIGN_EX(Instrux->DispLength, Instrux->Displacement);
        }

        // Get the segment. Note that in long mode, segment prefixes are ignored, except for FS and GS.
        if (Instrux->HasSeg)
        {
            operand->Info.Memory.Seg = NdGetSegOverride(Instrux, operand->Info.Memory.Seg);
        }

        // Handle VSIB addressing.
        if (ND_HAS_VSIB(Instrux))
        {
            // VSIB requires SIB.
            if (!Instrux->HasSib)
            {
                return ND_STATUS_VSIB_WITHOUT_SIB;
            }

            operand->Info.Memory.IsVsib = true;

            operand->Info.Memory.Vsib.IndexSize = vsibIndexSize;
            operand->Info.Memory.Vsib.ElemCount = vsibIndexCount;
            operand->Info.Memory.Vsib.ElemSize = (uint8_t)(size / vsibIndexCount);
        }

        // Handle sibmem addressing, as used by Intel AMX instructions.
        if (ND_HAS_SIBMEM(Instrux))
        {
            // sibmem requires SIB to be present.
            if (!Instrux->HasSib)
            {
                return ND_STATUS_INVALID_ENCODING;
            }

            operand->Info.Memory.IsSibMem = true;
        }

        if (Instrux->HasEvex)
        {
            // Handle compressed displacement, if any. Note that most EVEX instructions with 8 bit displacement
            // use compressed displacement addressing.
            if ((Instrux->HasDisp) && (ND_SIZE_8BIT == Instrux->DispLength))
            {
                Instrux->HasCompDisp = true;

                operand->Info.Memory.HasCompDisp = true;
                operand->Info.Memory.CompDispSize = NdGetCompDispSize(Instrux, operand->Size);
            }

            // If we have broadcast, the operand size is fixed to either 32 bit, either 64 bit, depending on bcast size.
            // Therefore, we will override the rawSize with either 32 or 64 bits. Note that bcstSize will save the total
            // size of the access, and it will be used to compute the number of broadcasted elements: bcstSize / rawSize.
            if ((Instrux->Exs.bm) && (opd & (ND_OPD_B32 | ND_OPD_B64)))
            {
                Instrux->HasBroadcast = true;
                operand->Info.Memory.HasBroadcast = true;

                if (opd & ND_OPD_B32)
                {
                    size = ND_SIZE_32BIT;
                }
                else if (opd & ND_OPD_B64)
                {
                    size = ND_SIZE_64BIT;
                }
                else
                {
                    size = width ? ND_SIZE_64BIT : ND_SIZE_32BIT;
                }

                // Override operand size.
                operand->Size = operand->RawSize = size;
            }
        }

        // MIB, if any. Used by some MPX instructions.
        operand->Info.Memory.IsMib = ND_HAS_MIB(Instrux);

        // Bitbase, if any. Used by BT* instructions when the first op is mem and the second one reg.
        operand->Info.Memory.IsBitbase = ND_HAS_BITBASE(Instrux);

        // AG, if this is the case.
        if (ND_HAS_AG(Instrux))
        {
            operand->Info.Memory.IsAG = true;

            // Address generation instructions ignore the segment prefixes. Examples are LEA and MPX instructions.
            operand->Info.Memory.HasSeg = false;
            operand->Info.Memory.Seg = 0;
        }

        // Shadow Stack Access, if this is the case.
        if (ND_HAS_SHS(Instrux))
        {
            operand->Info.Memory.IsShadowStack = true;
            operand->Info.Memory.ShStkType = ND_SHSTK_EXPLICIT;
        }

        break;


    case ND_OPT_H:
        // Vector register, encoded in VEX/EVEX.vvvv.
        if (ND_ENCM_LEGACY == Instrux->EncMode)
        {
            return ND_STATUS_HINT_OPERAND_NOT_USED;
        }
        else
        {
            operand->Type = ND_OP_REG;
            operand->Info.Register.Type = ND_REG_SSE;
            operand->Info.Register.Size = (ND_REG_SIZE)(size < ND_SIZE_128BIT ? ND_SIZE_128BIT : size);
            operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.vp << 4) | Instrux->Exs.v);
        }
        break;

    case ND_OPT_L:
        // Vector register, encoded in immediate.
        status = NdFetchSseImmediate(Instrux, Code, Offset, Size, 1);
        if (!ND_SUCCESS(status))
        {
            return status;
        }

        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SSE;
        operand->Info.Register.Size = (ND_REG_SIZE)(size < ND_SIZE_128BIT ? ND_SIZE_128BIT : size);
        operand->Info.Register.Reg = (Instrux->SseImmediate >> 4) & 0xF;

        if (Instrux->DefCode != ND_CODE_64)
        {
            operand->Info.Register.Reg &= 0x7;
        }

        break;

    case ND_OPT_U:
        // Vector register encoded in modrm.rm.
        if (Instrux->ModRm.mod != 3)
        {
            return ND_STATUS_INVALID_ENCODING;
        }

        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SSE;
        operand->Info.Register.Size = (ND_REG_SIZE)(size < ND_SIZE_128BIT ? ND_SIZE_128BIT : size);
        operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.b << 3) | Instrux->ModRm.rm);
        if (Instrux->HasEvex || Instrux->HasMvex)
        {
            operand->Info.Register.Reg |= Instrux->Exs.x << 4;
        }
        break;

    case ND_OPT_V:
        // Vector register encoded in modrm.reg.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_SSE;
        operand->Info.Register.Size = (ND_REG_SIZE)(size < ND_SIZE_128BIT ? ND_SIZE_128BIT : size);
        operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.r << 3) | Instrux->ModRm.reg);
        if (Instrux->HasEvex || Instrux->HasMvex)
        {
            operand->Info.Register.Reg |= Instrux->Exs.rp << 4;
        }
        break;

    case ND_OPT_W:
        // Vector register or memory encoded in modrm.rm.
        if (Instrux->ModRm.mod == 3)
        {
            operand->Type = ND_OP_REG;
            operand->Info.Register.Type = ND_REG_SSE;
            operand->Info.Register.Size = (ND_REG_SIZE)(size < ND_SIZE_128BIT ? ND_SIZE_128BIT : size);
            operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.b << 3) | Instrux->ModRm.rm);
            if (Instrux->HasEvex || Instrux->HasMvex)
            {
                operand->Info.Register.Reg |= Instrux->Exs.x << 4;
            }
        }
        else
        {
            goto memory;
        }
        break;

    case ND_OPT_X:
    case ND_OPT_Y:
    case ND_OPT_MEM_rDI:
        // RSI/RDI based addressing, as used by string instructions.
        Instrux->MemoryAccess |= operand->Access.Access;
        operand->Type = ND_OP_MEM;
        operand->Info.Memory.HasBase = true;
        operand->Info.Memory.BaseSize = 2 << Instrux->AddrMode;
        operand->Info.Memory.HasSeg = true;
        operand->Info.Memory.Base = (uint8_t)(((opt == ND_OPT_X) ? NDR_RSI : NDR_RDI));
        operand->Info.Memory.IsString = (ND_OPT_X == opt || ND_OPT_Y == opt);
        // DS:rSI supports segment overriding. ES:rDI does not.
        if (opt == ND_OPT_Y)
        {
            operand->Info.Memory.Seg = NDR_ES;
        }
        else
        {
            operand->Info.Memory.Seg = NdGetSegOverride(Instrux, NDR_DS);
        }
        break;

    case ND_OPT_MEM_rBX_AL:
        // [rBX + AL], used by XLAT.
        Instrux->MemoryAccess |= operand->Access.Access;
        operand->Type = ND_OP_MEM;
        operand->Info.Memory.HasBase = true;
        operand->Info.Memory.HasIndex = true;
        operand->Info.Memory.BaseSize = 2 << Instrux->AddrMode;
        operand->Info.Memory.IndexSize = ND_SIZE_8BIT;  // Always 1 Byte.
        operand->Info.Memory.Base = NDR_RBX;            // Always rBX.
        operand->Info.Memory.Index = NDR_AL;            // Always AL.
        operand->Info.Memory.Scale = 1;                 // Always 1.
        operand->Info.Memory.HasSeg = true;
        operand->Info.Memory.Seg = NdGetSegOverride(Instrux, NDR_DS);
        break;

    case ND_OPT_MEM_SHS:
        // Shadow stack access using the current SSP.
        Instrux->MemoryAccess |= operand->Access.Access;
        operand->Type = ND_OP_MEM;
        operand->Info.Memory.IsShadowStack = true;
        operand->Info.Memory.ShStkType = ND_SHSTK_SSP_LD_ST;
        break;

    case ND_OPT_MEM_SHS0:
        // Shadow stack access using the IA32_PL0_SSP.
        Instrux->MemoryAccess |= operand->Access.Access;
        operand->Type = ND_OP_MEM;
        operand->Info.Memory.IsShadowStack = true;
        operand->Info.Memory.ShStkType = ND_SHSTK_PL0_SSP;
        break;

    case ND_OPT_MEM_SHSP:
        // Shadow stack push/pop access.
        Instrux->MemoryAccess |= operand->Access.Access;
        operand->Type = ND_OP_MEM;
        operand->Info.Memory.IsShadowStack = true;
        operand->Info.Memory.ShStkType = ND_SHSTK_SSP_PUSH_POP;
        break;

    case ND_OPT_Z:
        // A GPR Register is selected by the low 3 bits inside the opcode. REX.B can be used to extend it.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_GPR;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.b << 3) | (Instrux->PrimaryOpCode & 0x7));
        operand->Info.Register.IsHigh8 = (operand->Info.Register.Size == 1) &&
                                         (operand->Info.Register.Reg >= 4) &&
                                         (ND_ENCM_LEGACY == Instrux->EncMode) &&
                                         !Instrux->HasRex;
        break;

    case ND_OPT_rB:
        // reg inside modrm selects a BND register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_BND;
        operand->Info.Register.Size = (ND_REG_SIZE)size;
        operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.r << 3) | Instrux->ModRm.reg);
        if (operand->Info.Register.Reg >= 4)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }
        break;

    case ND_OPT_mB:
        // rm inside modrm selects either a BND register, either memory.
        if (Instrux->ModRm.mod == 3)
        {
            operand->Type = ND_OP_REG;
            operand->Info.Register.Type = ND_REG_BND;
            operand->Info.Register.Size = (ND_REG_SIZE)size;
            operand->Info.Register.Reg = (uint8_t)((Instrux->Exs.b << 3) | Instrux->ModRm.rm);
            if (operand->Info.Register.Reg >= 4)
            {
                return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
            }
        }
        else
        {
            goto memory;
        }
        break;

    case ND_OPT_rK:
        // reg inside modrm selects a mask register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSK;

        // Opcode dependent #UD, R and R' must be zero (1 actually, but they're inverted).
        if ((Instrux->Exs.r != 0) || (Instrux->Exs.rp != 0))
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }

        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = (uint8_t)(Instrux->ModRm.reg);

        break;

    case ND_OPT_vK:
        // vex.vvvv selects a mask register.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSK;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = (uint8_t)Instrux->Exs.v;
        if (operand->Info.Register.Reg >= 8)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }
        break;

    case ND_OPT_mK:
        // rm inside modrm selects either a mask register, either memory.
        if (Instrux->ModRm.mod == 3)
        {
            operand->Type = ND_OP_REG;
            operand->Info.Register.Type = ND_REG_MSK;
            operand->Info.Register.Size = ND_SIZE_64BIT;
            // X and B are ignored when Msk registers are being addressed.
            operand->Info.Register.Reg = Instrux->ModRm.rm;
        }
        else
        {
            goto memory;
        }
        break;

    case ND_OPT_aK:
        // aaa inside evex selects either a mask register, which is used for masking a destination operand.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_MSK;
        operand->Info.Register.Size = ND_SIZE_64BIT;
        operand->Info.Register.Reg = Instrux->Exs.k;
        break;

    case ND_OPT_rM:
        // Sigh. reg field inside mod r/m encodes memory. This encoding is used by MOVDIR64b and ENQCMD instructions.
        // When the ModR/M.reg field is used to select a memory operand, the following apply:
        // - The ES segment register is used as a base
        // - The ES segment register cannot be overridden
        // - The size of the base register is selected by the address size, not the operand size.
        operand->Type = ND_OP_MEM;
        operand->Info.Memory.HasBase = true;
        operand->Info.Memory.Base = (uint8_t)((Instrux->Exs.r << 3) | Instrux->ModRm.reg);
        operand->Info.Memory.BaseSize = 2 << Instrux->AddrMode;
        operand->Info.Memory.HasSeg = true;
        operand->Info.Memory.Seg = NDR_ES;
        break;

    case ND_OPT_mM:
        // Sigh. rm field inside mod r/m encodes memory, even if mod is 3.
        operand->Type = ND_OP_MEM;
        operand->Info.Memory.HasBase = true;
        operand->Info.Memory.Base = (uint8_t)((Instrux->Exs.m << 3) | Instrux->ModRm.rm);
        operand->Info.Memory.BaseSize = 2 << Instrux->AddrMode;
        operand->Info.Memory.HasSeg = true;
        operand->Info.Memory.Seg = NdGetSegOverride(Instrux, NDR_DS);
        break;

    case ND_OPT_rT:
        // Tile register encoded in ModR/M.reg field.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_TILE;
        operand->Info.Register.Size = size;
        operand->Info.Register.Reg = Instrux->ModRm.reg;

        // #UD if a tile register > 7 is encoded.
        if (Instrux->Exs.r != 0)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }

        break;

    case ND_OPT_mT:
        // Tile register encoded in ModR/M.rm field.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_TILE;
        operand->Info.Register.Size = size;
        operand->Info.Register.Reg = Instrux->ModRm.rm;

        // #UD if a tile register > 7 is encoded.
        if (Instrux->Exs.b != 0)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }

        break;

    case ND_OPT_vT:
        // Tile register encoded in vex.vvvv field.
        operand->Type = ND_OP_REG;
        operand->Info.Register.Type = ND_REG_TILE;
        operand->Info.Register.Size = size;
        operand->Info.Register.Reg = Instrux->Exs.v;

        // #UD if a tile register > 7 is encoded.
        if (operand->Info.Register.Reg > 7)
        {
            return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
        }

        break;

    default:
        return ND_STATUS_INVALID_INSTRUX;
    }

    // Handle block addressing - used by AVX512_4FMAPS and AVX512_4VNNIW instructions. Also used by VP2INTERSECTD/Q
    // instructions. Also note that in block addressing, the base of the block is masked using the size of the block;
    // for example, for a block size of 1, the first register must be even; For a block size of 4, the first register
    // must be divisible by 4.
    if (operand->Type == ND_OP_REG)
    {
        if (opb != 0)
        {
            operand->Info.Register.Count = opb;
            operand->Info.Register.Reg &= ~(opb - 1);
            operand->Info.Register.IsBlock = true;
        }
        else
        {
            operand->Info.Register.Count = 1;
        }
    }

    // Store the operand encoding inside the bitmap.
    Instrux->OperandsEncodingMap |= (1 << gOperandMap[opt]);

    operand->Encoding = (ND_OPERAND_ENCODING)gOperandMap[opt];

    // Handle decorators.
    if (0 != opd)
    {
        // Check for mask register. Mask if present only if the operand supports masking and if the
        // mask register is not k0 (which implies "no masking").
        if ((opd & ND_OPD_MASK) && (Instrux->Exs.k != 0))
        {
            operand->Decorator.HasMask = true;
            operand->Decorator.Mask.Msk = (uint8_t)Instrux->Exs.k;
            Instrux->HasMask = true;
        }

        // Check for zeroing. The operand must support zeroing and the z bit inside vex3 must be set. Note that
        // zeroing is allowed only for register destinations, and NOT for memory.
        if ((opd & ND_OPD_Z) && (Instrux->Exs.z) && (operand->Type != ND_OP_MEM))
        {
            operand->Decorator.HasZero = true;
            Instrux->HasZero = true;
        }

        // Check for broadcast again. We've already filled the broadcast size before parsing the op size.
        if (((opd & ND_OPD_B32) || (opd & ND_OPD_B64)) && (Instrux->Exs.bm) && (Instrux->ModRm.mod != 3))
        {
            operand->Decorator.HasBroadcast = true;
            operand->Decorator.Broadcast.Size = (uint8_t)operand->Size;
            operand->Decorator.Broadcast.Count = (uint8_t)(bcstSize / operand->Size);
        }

        if (opd & ND_OPD_SAE)
        {
            operand->Decorator.HasSae = Instrux->HasSae;
        }

        if (opd & ND_OPD_ER)
        {
            operand->Decorator.HasEr = Instrux->HasEr;
        }
    }

    return status;
}


//
// NdFindInstruction
//
static NDSTATUS
NdFindInstruction(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t Offset,
    size_t Size,
    ND_INSTRUCTION **InsDef
    )
{
    NDSTATUS status;
    const ND_TABLE *pTable;
    ND_INSTRUCTION *pIns;
    bool stop, redf2, redf3;
    uint32_t nextOpcode, nextIndex;

    UNREFERENCED_PARAMETER(Offset);

    // pre-init
    status = ND_STATUS_SUCCESS;
    pIns = NULL;
    stop = false;
    nextOpcode = 0;
    redf2 = redf3 = false;

    switch (Instrux->EncMode)
    {
    case ND_ENCM_LEGACY:
        pTable = (const ND_TABLE *)gRootTable;
        break;
    case ND_ENCM_XOP:
        pTable = (const ND_TABLE *)gXopTable;
        break;
    case ND_ENCM_VEX:
        pTable = (const ND_TABLE *)gVexTable;
        break;
    case ND_ENCM_EVEX:
        pTable = (const ND_TABLE *)gEvexTable;
        break;
    default:
        pTable = (const ND_TABLE *)NULL;
        break;
    }

    while ((!stop) && (NULL != pTable))
    {
        switch (pTable->Type)
        {
        case ND_ILUT_INSTRUCTION:
            // We've found the leaf entry, which is an instruction - we can leave.
            pIns = (ND_INSTRUCTION *)(((ND_TABLE_INSTRUCTION *)pTable)->Instruction);
            stop = true;
            break;

        case ND_ILUT_OPCODE:
            // We need an opcode to keep going.
            status = NdFetchOpcode(Instrux, Code, Instrux->Length, Size);
            if (!ND_SUCCESS(status))
            {
                stop = true;
                break;
            }
            pTable = (const ND_TABLE *)pTable->Table[Instrux->OpCodeBytes[nextOpcode++]];
            break;

        case ND_ILUT_OPCODE_3DNOW:
            // We need an opcode to select the next table, but the opcode is AFTER the modrm/sib/displacement.
            if (!Instrux->HasModRm)
            {
                // Fetch modrm
                status = NdFetchModrmAndSib(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    stop = true;
                    break;
                }

                // Fetch displacement.
                status = NdFetchDisplacement(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    stop = true;
                    break;
                }
            }

            // Fetch the opcode, which is after the modrm and displacement.
            status = NdFetchOpcode(Instrux, Code, Instrux->Length, Size);
            if (!ND_SUCCESS(status))
            {
                stop = true;
                break;
            }

            pTable = (const ND_TABLE *)pTable->Table[Instrux->OpCodeBytes[nextOpcode++]];
            break;

        case ND_ILUT_MODRM_MOD:
            // We need modrm.mod to select the next table.
            if (!Instrux->HasModRm)
            {
                // Fetch modrm
                status = NdFetchModrmAndSib(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    stop = true;
                    break;
                }

                // Fetch displacement.
                status = NdFetchDisplacement(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    stop = true;
                    break;
                }
            }

            // Next index is either 0 (mem) or 1 (reg)
            pTable = (const ND_TABLE *)pTable->Table[Instrux->ModRm.mod == 3 ? 1 : 0];
            break;

        case ND_ILUT_MODRM_REG:
            // We need modrm.reg to select the next table.
            if (!Instrux->HasModRm)
            {
                // Fetch modrm
                status = NdFetchModrmAndSib(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    stop = true;
                    break;
                }

                // Fetch displacement.
                status = NdFetchDisplacement(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    stop = true;
                    break;
                }
            }

            // Next index is the reg.
            pTable = (const ND_TABLE *)pTable->Table[Instrux->ModRm.reg];
            break;

        case ND_ILUT_MODRM_RM:
            // We need modrm.rm to select the next table.
            if (!Instrux->HasModRm)
            {
                // Fetch modrm
                status = NdFetchModrmAndSib(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    stop = true;
                    break;
                }

                // Fetch displacement.
                status = NdFetchDisplacement(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    stop = true;
                    break;
                }
            }

            // Next index is the rm.
            pTable = (const ND_TABLE *)pTable->Table[Instrux->ModRm.rm];
            break;

        case ND_ILUT_MAN_PREFIX:
            // We have mandatory prefixes.
            if ((Instrux->Rep == 0xF2) && !redf2)
            {
                // We can only redirect once through one mandatory prefix, otherwise we may
                // enter an infinite loop (see CRC32 Gw Eb -> 0x66 0xF2 0x0F ...)
                redf2 = true;
                nextIndex = ND_ILUT_INDEX_MAN_PREF_F2;
                Instrux->HasMandatoryF2 = true;
            }
            else if ((Instrux->Rep == 0xF3) && !redf3)
            {
                redf3 = true;
                nextIndex = ND_ILUT_INDEX_MAN_PREF_F3;
                Instrux->HasMandatoryF3 = true;
            }
            else if (Instrux->HasOpSize)
            {
                nextIndex = ND_ILUT_INDEX_MAN_PREF_66;
                Instrux->HasMandatory66 = true;
            }
            else
            {
                nextIndex = ND_ILUT_INDEX_MAN_PREF_NONE;
            }
            pTable = (const ND_TABLE *)pTable->Table[nextIndex];
            break;

        case ND_ILUT_MODE:
            {
                static const uint8_t indexes[3] =
                {
                    ND_ILUT_INDEX_MODE_16, ND_ILUT_INDEX_MODE_32, ND_ILUT_INDEX_MODE_64
                };

                nextIndex = ND_ILUT_INDEX_MODE_NONE;

                if (NULL != pTable->Table[indexes[Instrux->DefCode]])
                {
                    nextIndex = indexes[Instrux->DefCode];
                }

                pTable = (const ND_TABLE *)pTable->Table[nextIndex];
            }
            break;

        case ND_ILUT_DSIZE:
            {
                static const uint8_t indexes[3] =
                {
                    ND_ILUT_INDEX_DSIZE_16, ND_ILUT_INDEX_DSIZE_32, ND_ILUT_INDEX_DSIZE_64
                };

                nextIndex = ND_ILUT_INDEX_DSIZE_NONE;

                if (NULL != pTable->Table[indexes[Instrux->OpMode]])
                {
                    nextIndex = indexes[Instrux->OpMode];
                }

                // Handle default/forced redirections in 64 bit mode.
                if (ND_CODE_64 == Instrux->DefCode)
                {
                    if ((NULL != pTable->Table[4]) && (!Instrux->HasOpSize || Instrux->Exs.w))
                    {
                        nextIndex = 4;
                    }
                    else if (NULL != pTable->Table[5])
                    {
                        nextIndex = 5;
                    }
                }

                pTable = (const ND_TABLE *)pTable->Table[nextIndex];
            }
            break;

        case ND_ILUT_ASIZE:
            {
                static const uint8_t indexes[3] = {ND_ILUT_INDEX_ASIZE_16, ND_ILUT_INDEX_ASIZE_32, ND_ILUT_INDEX_ASIZE_64};

                nextIndex = ND_ILUT_INDEX_ASIZE_NONE;

                if (NULL != pTable->Table[indexes[Instrux->AddrMode]])
                {
                    nextIndex = indexes[Instrux->AddrMode];
                }

                pTable = (const ND_TABLE *)pTable->Table[nextIndex];
            }
            break;

        case ND_ILUT_AUXILIARY:
            // Auxiliary redirection. Default to table[0] if nothing matches.
            if (Instrux->HasRex && Instrux->Rex.b && (NULL != pTable->Table[ND_ILUT_INDEX_AUX_REXB]))
            {
                nextIndex = ND_ILUT_INDEX_AUX_REXB;
            }
            else if (Instrux->HasRex && Instrux->Rex.w && (NULL != pTable->Table[ND_ILUT_INDEX_AUX_REXW]))
            {
                nextIndex = ND_ILUT_INDEX_AUX_REXW;
            }
            else if ((Instrux->DefCode == ND_CODE_64) && (NULL != pTable->Table[ND_ILUT_INDEX_AUX_O64]))
            {
                nextIndex = ND_ILUT_INDEX_AUX_O64;
            }
            else if (Instrux->Rep == ND_PREFIX_G1_REPE_REPZ && (NULL != pTable->Table[ND_ILUT_INDEX_AUX_F3]))
            {
                nextIndex = ND_ILUT_INDEX_AUX_F3;
            }
            else if ((Instrux->Rep != 0) && (NULL != pTable->Table[ND_ILUT_INDEX_AUX_REP]))
            {
                nextIndex = ND_ILUT_INDEX_AUX_REP;
            }
            else
            {
                nextIndex = ND_ILUT_INDEX_AUX_NONE;
            }
            pTable = (const ND_TABLE *)pTable->Table[nextIndex];
            break;

        case ND_ILUT_VENDOR:
            // Vendor redirection. Go to the vendor specific entry.
            if (NULL != pTable->Table[Instrux->VendMode])
            {
                pTable = (const ND_TABLE *)pTable->Table[Instrux->VendMode];
            }
            else
            {
                pTable = (const ND_TABLE *)pTable->Table[ND_VEND_ANY];
            }
            break;

        case ND_ILUT_FEATURE:
            // Feature redirection. Normally NOP if feature is not set, but may be something else if feature is set.
            if ((NULL != pTable->Table[ND_ILUT_FEATURE_MPX]) && !!(Instrux->FeatMode & ND_FEAT_MPX))
            {
                pTable = (const ND_TABLE *)pTable->Table[ND_ILUT_FEATURE_MPX];
            }
            else if ((NULL != pTable->Table[ND_ILUT_FEATURE_CET]) && !!(Instrux->FeatMode & ND_FEAT_CET))
            {
                pTable = (const ND_TABLE *)pTable->Table[ND_ILUT_FEATURE_CET];
            }
            else if ((NULL != pTable->Table[ND_ILUT_FEATURE_CLDEMOTE]) && !!(Instrux->FeatMode & ND_FEAT_CLDEMOTE))
            {
                pTable = (const ND_TABLE *)pTable->Table[ND_ILUT_FEATURE_CLDEMOTE];
            }
            else
            {
                pTable = (const ND_TABLE *)pTable->Table[ND_ILUT_FEATURE_NONE];
            }
            break;

        case ND_ILUT_VEX_MMMMM:
            pTable = (const ND_TABLE *)pTable->Table[Instrux->Exs.m];
            break;

        case ND_ILUT_VEX_PP:
            pTable = (const ND_TABLE *)pTable->Table[Instrux->Exs.p];
            break;

        case ND_ILUT_VEX_L:
            if (Instrux->HasEvex && Instrux->Exs.bm)
            {
                // We have evex; we need to fetch the modrm now, because we have to make sure we don't have SAE or ER;
                // if we do have SAE or ER, we have to check the modrm byte and see if it is a reg-reg form (mod = 3),
                // in which case L'L is forced to the maximum vector length of the instruction. We know for sure that
                // all EVEX instructions have modrm.
                if (!Instrux->HasModRm)
                {
                    // Fetch modrm
                    status = NdFetchModrmAndSib(Instrux, Code, Instrux->Length, Size);
                    if (!ND_SUCCESS(status))
                    {
                        stop = true;
                        break;
                    }

                    // Fetch displacement.
                    status = NdFetchDisplacement(Instrux, Code, Instrux->Length, Size);
                    if (!ND_SUCCESS(status))
                    {
                        stop = true;
                        break;
                    }
                }

                if (3 == Instrux->ModRm.mod)
                {
                    // We use the maximum vector length of the instruction. If the instruction does not support
                    // SAE or ER, a #UD would be generated. We check for this later.
                    if (NULL != pTable->Table[2])
                    {
                        pTable = (const ND_TABLE *)pTable->Table[2];
                    }
                    else if (NULL != pTable->Table[1])
                    {
                        pTable = (const ND_TABLE *)pTable->Table[1];
                    }
                    else
                    {
                        pTable = (const ND_TABLE *)pTable->Table[0];
                    }
                }
                else
                {
                    // Mod is mem, we simply use L'L for indexing, as no SAE or ER can be present.
                    pTable = (const ND_TABLE *)pTable->Table[Instrux->Exs.l];
                }
            }
            else
            {
                pTable = (const ND_TABLE *)pTable->Table[Instrux->Exs.l];
            }
            break;

        case ND_ILUT_VEX_W:
            pTable = (const ND_TABLE *)pTable->Table[Instrux->Exs.w];
            break;

        default:
            status = ND_STATUS_INTERNAL_ERROR;
            stop = true;
            break;
        }
    }

    if (!ND_SUCCESS(status))
    {
        goto cleanup_and_exit;
    }

    if (NULL != pIns)
    {
        // Bingo! Valid instruction found for the encoding. If Modrm is needed and we didn't fetch it - do it now.
        if ((pIns->Attributes & ND_FLAG_MODRM) && (!Instrux->HasModRm))
        {
            if (0 == (pIns->Attributes & ND_FLAG_MFR))
            {
                // Fetch Mod R/M and SIB.
                status = NdFetchModrmAndSib(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    goto cleanup_and_exit;
                }

                // Fetch displacement.
                status = NdFetchDisplacement(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    goto cleanup_and_exit;
                }
            }
            else
            {
                // Handle special MOV with control and debug registers - the mod is always forced to register. SIB
                // and displacement is ignored.
                status = NdFetchModrm(Instrux, Code, Instrux->Length, Size);
                if (!ND_SUCCESS(status))
                {
                    goto cleanup_and_exit;
                }
            }
        }

        // Store primary opcode.
        Instrux->PrimaryOpCode = Instrux->OpCodeBytes[Instrux->OpLength - 1];

        Instrux->MainOpOffset = ND_IS_3DNOW(Instrux) ?
                                       Instrux->Length - 1 : Instrux->OpOffset + Instrux->OpLength - 1;

        // Make sure the instruction is valid in the given mode.
        if ((ND_CODE_64 == Instrux->DefCode) && (pIns->Attributes & ND_FLAG_I64))
        {
            status = ND_STATUS_INVALID_ENCODING_IN_MODE;
        }

        if ((ND_CODE_64 != Instrux->DefCode) && (pIns->Attributes & ND_FLAG_O64))
        {
            status = ND_STATUS_INVALID_ENCODING_IN_MODE;
        }
    }
    else
    {
        status = ND_STATUS_INVALID_ENCODING;
    }

cleanup_and_exit:
    *InsDef = pIns;

    return status;
}



//
// NdGetVectorLength
//
static __forceinline NDSTATUS
NdGetVectorLength(
    INSTRUX *Instrux
    )
{
    if (Instrux->HasEvex && Instrux->Exs.bm && (Instrux->ModRm.mod == 3) &&
        (ND_ER_SUPPORT(Instrux) || ND_SAE_SUPPORT(Instrux) || !!(Instrux->Attributes & ND_FLAG_IER)))
    {
        // Embedded rounding present, force the vector length to 512.
        if ((Instrux->TupleType == ND_TUPLE_T1S) || (Instrux->TupleType == ND_TUPLE_T1F))
        {
            Instrux->VecMode = Instrux->EfVecMode = ND_VECM_128;
        }
        else
        {
            Instrux->VecMode = Instrux->EfVecMode = ND_VECM_512;
        }

        return ND_STATUS_SUCCESS;
    }

    // Decode VEX vector length. Also take into consideration the "ignore L" flag.
    switch (Instrux->Exs.l)
    {
    case 0:
        Instrux->VecMode = ND_VECM_128;
        Instrux->EfVecMode = ND_VECM_128;
        break;
    case 1:
        Instrux->VecMode = ND_VECM_256;
        Instrux->EfVecMode = (Instrux->Attributes & ND_FLAG_LIG) ? ND_VECM_128 : ND_VECM_256;
        break;
    case 2:
        Instrux->VecMode = ND_VECM_512;
        Instrux->EfVecMode = (Instrux->Attributes & ND_FLAG_LIG) ? ND_VECM_128 : ND_VECM_512;
        break;
    default:
        return ND_STATUS_INVALID_INSTRUX;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdGetAddrAndOpMode
//
static __forceinline NDSTATUS
NdGetAddrAndOpMode(
    INSTRUX *Instrux
    )
{
    // Fill in addressing mode & default op size.
    switch (Instrux->DefCode)
    {
    case ND_CODE_16:
        Instrux->AddrMode = Instrux->HasAddrSize ? ND_ADDR_32 : ND_ADDR_16;
        Instrux->OpMode = Instrux->HasOpSize ? ND_OPSZ_32 : ND_OPSZ_16;
        break;
    case ND_CODE_32:
        Instrux->AddrMode = Instrux->HasAddrSize ? ND_ADDR_16 : ND_ADDR_32;
        Instrux->OpMode = Instrux->HasOpSize ? ND_OPSZ_16 : ND_OPSZ_32;
        break;
    case ND_CODE_64:
        Instrux->AddrMode = Instrux->HasAddrSize ? ND_ADDR_32 : ND_ADDR_64;
        Instrux->OpMode = Instrux->Exs.w ? ND_OPSZ_64 : (Instrux->HasOpSize ? ND_OPSZ_16 : ND_OPSZ_32);
        break;
    default:
        return ND_STATUS_INVALID_INSTRUX;
    }

    return ND_STATUS_SUCCESS;
}


//
// NdGetEffectiveOpMode
//
static __forceinline NDSTATUS
NdGetEffectiveOpMode(
    INSTRUX *Instrux
    )
{
    static const uint8_t szLut[3] = { ND_SIZE_16BIT, ND_SIZE_32BIT, ND_SIZE_64BIT };
    bool width, f64, d64, has66;

    // Extract the flags.
    width = (0 != Instrux->Exs.w) && !(Instrux->Attributes & ND_FLAG_WIG);
    // In 64 bit mode, the operand is forced to 64 bit. Size-changing prefixes are ignored.
    f64 = 0 != (Instrux->Attributes & ND_FLAG_F64) && (ND_VEND_AMD != Instrux->VendMode);
    // In 64 bit mode, the operand defaults to 64 bit. No 32 bit form of the instruction exists. Note that on AMD,
    // only default 64 bit operands exist, even for branches - no operand is forced to 64 bit.
    d64 = (0 != (Instrux->Attributes & ND_FLAG_D64)) ||
          (0 != (Instrux->Attributes & ND_FLAG_F64) && (ND_VEND_AMD == Instrux->VendMode));
    // Check if 0x66 is indeed interpreted as a size changing prefix. Note that if 0x66 is a mandatory prefix,
    // then it won't be interpreted as a size changing prefix. However, there is an exception: MOVBE and CRC32
    // have mandatory 0xF2, and 0x66 is in fact a size changing prefix.
    has66 = Instrux->HasOpSize && (!Instrux->HasMandatory66 || (Instrux->Attributes & ND_FLAG_S66));

    // Fill in the effective operand size.
    switch (Instrux->DefCode)
    {
    case ND_CODE_16:
        Instrux->EfOpMode = has66 ? ND_OPSZ_32 : ND_OPSZ_16;
        break;
    case ND_CODE_32:
        Instrux->EfOpMode = has66 ? ND_OPSZ_16 : ND_OPSZ_32;
        break;
    case ND_CODE_64:
        Instrux->EfOpMode = (width || f64 || (d64 && !has66)) ? ND_OPSZ_64 : (has66 ? ND_OPSZ_16 : ND_OPSZ_32);
        Instrux->AddrMode = !!(Instrux->Attributes & ND_FLAG_I67) ? ND_ADDR_64 : Instrux->AddrMode;
        break;
    default:
        return ND_STATUS_INVALID_INSTRUX;
    }

    // Fill in the default word length. It can't be more than 8 bytes.
    Instrux->WordLength = szLut[Instrux->EfOpMode];

    return ND_STATUS_SUCCESS;
}


//
// NdValidateInstruction
//
static __forceinline NDSTATUS
NdValidateInstruction(
    INSTRUX *Instrux
    )
{
    // If LOCK is present, make sure that the instruction 1. supports LOCKing and 2. the destination is memory.
    // A special case are MOV to/from CRs, on AMD, in 16/32 bit mode.
    if (Instrux->HasLock && (0 == (Instrux->Attributes & ND_FLAG_LOCK_SPECIAL) || (ND_CODE_64 == Instrux->DefCode)) &&
        (!ND_LOCK_SUPPORT(Instrux) || (Instrux->Operands[0].Type != ND_OP_MEM)))
    {
        return ND_STATUS_BAD_LOCK_PREFIX;
    }

    // Some instructions (example: PTWRITE) do not accept the 0x66 prefix.
    if (Instrux->HasOpSize && (0 != (Instrux->Attributes & ND_FLAG_NO66)))
    {
        return ND_STATUS_66_NOT_ACCEPTED;
    }

    // 16 bit addressing is checked when decoding the memory operand (if present).
    // RIP-relative addressing is checked when decoding the memory operand (if present).
    // Register validity is checked when decoding the said register.
    // Memory/register encoding for instructions which don't support it is checked when decoding the operand.

    // VEX/EVEX validations.
    if (ND_ENCM_LEGACY != Instrux->EncMode)
    {
        // Instructions that don't use VEX/XOP vvvv field must set it to 1111b/0, otherwise a #UD will be generated.
        if ((0 == (Instrux->OperandsEncodingMap & (1 << ND_OPE_V))) && (0 != Instrux->Exs.v))
        {
            return ND_STATUS_VEX_VVVV_MUST_BE_ZERO;
        }

        // Some instructions don't support 128 bit vectors.
        if ((ND_VECM_128 == Instrux->EfVecMode) && (0 != (Instrux->Attributes & ND_FLAG_NOL0)))
        {
            return ND_STATUS_INVALID_ENCODING;
        }

        // VSIB instructions have a restriction: the same vector register can't be used by more than one operand.
        // The exception is SCATTER*, which can use the VSIB reg as two sources.
        if (ND_HAS_VSIB(Instrux) && Instrux->Category != ND_CAT_SCATTER)
        {
            uint8_t usedVects[32] = { 0 };
            uint32_t i;

            for (i = 0; i < Instrux->OperandsCount; i++)
            {
                if (Instrux->Operands[i].Type == ND_OP_REG && Instrux->Operands[i].Info.Register.Type == ND_REG_SSE)
                {
                    if (++usedVects[Instrux->Operands[i].Info.Register.Reg] > 1)
                    {
                        return ND_STATUS_INVALID_VSIB_REGS;
                    }
                }
                else if (Instrux->Operands[i].Type == ND_OP_MEM)
                {
                    if (++usedVects[Instrux->Operands[i].Info.Memory.Index] > 1)
                    {
                        return ND_STATUS_INVALID_VSIB_REGS;
                    }
                }
            }
        }

        // Handle AMX exception class.
        if (Instrux->ExceptionClass == ND_EXC_AMX)
        {
            if (Instrux->ExceptionType == ND_EXT_AMX_E4)
            {
                // #UD if srcdest == src1, srcdest == src2 or src1 == src2. All three operands are tile regs.
                if (Instrux->Operands[0].Info.Register.Reg == Instrux->Operands[1].Info.Register.Reg ||
                    Instrux->Operands[0].Info.Register.Reg == Instrux->Operands[2].Info.Register.Reg ||
                    Instrux->Operands[1].Info.Register.Reg == Instrux->Operands[2].Info.Register.Reg)
                {
                    return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
                }
            }
            else
            {
                // #UD if vex.vvvv is not 0 (0b1111 negated) for all other exception classes, as they do not use it.
                if (Instrux->Exs.v != 0)
                {
                    return ND_STATUS_VEX_VVVV_MUST_BE_ZERO;
                }
            }
        }

        if (Instrux->HasEvex)
        {
            // Instructions that don't support masking must have EVEX.aaa = 0.
            if (!ND_MASK_SUPPORT(Instrux) && (0 != Instrux->Exs.k))
            {
                return ND_STATUS_MASK_NOT_SUPPORTED;
            }

            // Some instructions have mandatory masking, and using k0 as a mask triggers #UD.
            if ((Instrux->Attributes & ND_FLAG_MMASK) && (0 == Instrux->Exs.k))
            {
                return ND_STATUS_INVALID_REGISTER_IN_INSTRUCTION;
            }

            // EVEX.z must be 0 if:
            // - zeroing is not supported by the instruction.
            // - zeroing is supported, but the destination is memory.
            // If zeroing is supported and the mask is 0, then zeroing is ignored.
            if (0 != Instrux->Exs.z)
            {
                if (!ND_ZERO_SUPPORT(Instrux))
                {
                    return ND_STATUS_ZEROING_NOT_SUPPORTED;
                }

                if (Instrux->Operands[0].Type == ND_OP_MEM)
                {
                    return ND_STATUS_ZEROING_ON_MEMORY;
                }
            }

            // EVEX.b must be 0 if SAE/ER is not used, but can be ignored if the ignore embedded rounding flag is set.
            if (Instrux->Exs.bm && (Instrux->ModRm.mod == 3) && 
                !ND_SAE_SUPPORT(Instrux) && !ND_ER_SUPPORT(Instrux) &&
                !(Instrux->Attributes & ND_FLAG_IER))
            {
                return ND_STATUS_ER_SAE_NOT_SUPPORTED;
            }

            // EVEX.b must be 0 if broadcast is not supported.
            if (Instrux->Exs.bm && (Instrux->ModRm.mod != 3) && !ND_BROADCAST_SUPPORT(Instrux))
            {
                return ND_STATUS_BROADCAST_NOT_SUPPORTED;
            }
        }
    }

    return ND_STATUS_SUCCESS;
}


//
// NdDecodeEx2
//
NDSTATUS
NdDecodeEx2(
    INSTRUX *Instrux,
    const uint8_t *Code,
    size_t Size,
    uint8_t DefCode,
    uint8_t DefData,
    uint8_t DefStack,
    uint8_t Vendor
    )
{
    ND_CONTEXT opt;

    NdInitContext(&opt);

    opt.DefCode = DefCode;
    opt.DefData = DefData;
    opt.DefStack = DefStack;
    opt.VendMode = Vendor;
    opt.FeatMode = ND_FEAT_ALL; // Optimistically decode everything, as if all features are enabled.

    return NdDecodeWithContext(Instrux, Code, Size, &opt);
}


NDSTATUS
NdDecodeWithContext(
    INSTRUX *Instrux,
    const uint8_t *Code,
    size_t Size,
    ND_CONTEXT *Context
    )
{
    NDSTATUS status;
    PND_INSTRUCTION pIns;
    uint32_t opIndex;
    size_t i;

    // pre-init
    status = ND_STATUS_SUCCESS;
    pIns = NULL;
    opIndex = 0;

    // validate
    if (NULL == Instrux)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (NULL == Code)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (Size == 0)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (NULL == Context)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (ND_CODE_64 < Context->DefCode)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (ND_DATA_64 < Context->DefData)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (ND_VEND_CYRIX < Context->VendMode)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    // Initialize with zero.
    nd_memzero(Instrux, sizeof(INSTRUX));

    Instrux->DefCode = (uint8_t)Context->DefCode;
    Instrux->DefData = (uint8_t)Context->DefData;
    Instrux->DefStack = (uint8_t)Context->DefStack;
    Instrux->VendMode = (uint8_t)Context->VendMode;
    Instrux->FeatMode = (uint8_t)Context->FeatMode;

    // Fetch prefixes. We peek at the first byte, if that's not a prefix, there's no need to call the main decoder.
    if (ND_PREF_CODE_NONE != gPrefixesMap[Code[0]])
    {
        status = NdFetchPrefixes(Instrux, Code, 0, Size);
        if (!ND_SUCCESS(status))
        {
            return status;
        }
    }

    // Get addressing mode & operand size.
    status = NdGetAddrAndOpMode(Instrux);
    if (!ND_SUCCESS(status))
    {
        return status;
    }

    // Start iterating the tables, in order to extract the instruction entry.
    status = NdFindInstruction(Instrux, Code, Instrux->Length, Size, &pIns);
    if (!ND_SUCCESS(status))
    {
        return status;
    }

    // Instruction found, copy information inside the Instrux.
    Instrux->Attributes = pIns->Attributes;
    Instrux->Instruction = Instrux->Iclass = (ND_INS_CLASS)pIns->Instruction;
    Instrux->Category = (ND_INS_CATEGORY)pIns->Category;
    Instrux->IsaSet = (ND_INS_SET)pIns->IsaSet;
    Instrux->FlagsAccess.Undefined.Raw = pIns->SetFlags & pIns->ClearedFlags;
    Instrux->FlagsAccess.Tested.Raw = pIns->TestedFlags;
    Instrux->FlagsAccess.Modified.Raw = pIns->ModifiedFlags;
    Instrux->FlagsAccess.Set.Raw = pIns->SetFlags ^ Instrux->FlagsAccess.Undefined.Raw;
    Instrux->FlagsAccess.Cleared.Raw = pIns->ClearedFlags ^ Instrux->FlagsAccess.Undefined.Raw;
    Instrux->CpuidFlag.Flag = pIns->CpuidFlag;
    Instrux->ValidModes.Raw = pIns->ValidModes;
    Instrux->ValidPrefixes.Raw = pIns->ValidPrefixes;
    Instrux->ValidDecorators.Raw = pIns->ValidDecorators;
    *((uint8_t*)&Instrux->FpuFlagsAccess) = pIns->FpuFlags;
    // Valid for EVEX, VEX and SSE instructions only. A value of 0 means it's not used.
    Instrux->ExceptionClass = pIns->ExcClass;
    Instrux->ExceptionType = pIns->ExcType;
    // Used only by EVEX instructions.
    Instrux->TupleType = pIns->TupleType;

    // Copy the mnemonic, up until the NULL terminator.
    for (i = 0; i < sizeof(Instrux->Mnemonic); i++)
    {
        Instrux->Mnemonic[i] = gMnemonics[pIns->Mnemonic][i];
        if (Instrux->Mnemonic[i] == 0)
        {
            break;
        }
    }

    // Get effective operand mode.
    status = NdGetEffectiveOpMode(Instrux);
    if (!ND_SUCCESS(status))
    {
        return status;
    }

    if (ND_HAS_VECTOR(Instrux))
    {
        // Get vector length.
        status = NdGetVectorLength(Instrux);
        if (!ND_SUCCESS(status))
        {
            return status;
        }
    }

    // Handle condition byte, if present.
    if (ND_HAS_SSE_CONDITION(Instrux))
    {
        Instrux->SseCondition = Instrux->Immediate1 & 0x1F;
    }

    // Handle predicate, if present.
    if (ND_HAS_CONDITION(Instrux))
    {
        Instrux->Condition = Instrux->Predicate = Instrux->PrimaryOpCode & 0xF;
    }

    if (0 != pIns->ValidDecorators)
    {
        // Check for suppress all exceptions.
        if ((Instrux->ValidDecorators.Sae) && (Instrux->Exs.bm) && (Instrux->ModRm.mod == 3))
        {
            Instrux->HasSae = true;
        }

        // Check for embedded rounding. This is available only in reg-reg encodings. Also, if embedded
        // rounding is used, the vector length is forced to 512 bit, as the
        if ((Instrux->ValidDecorators.Er) && (Instrux->Exs.bm) && (Instrux->ModRm.mod == 3))
        {
            Instrux->HasEr = true;
            Instrux->HasSae = true;
            Instrux->RoundingMode = (uint8_t)Instrux->Exs.l;
        }
    }

    Instrux->ExpOperandsCount = ND_EXP_OPS_CNT(pIns->OpsCount);
    Instrux->OperandsCount = Instrux->ExpOperandsCount + ND_IMP_OPS_CNT(pIns->OpsCount);

    // And now decode each operand.
    for (opIndex = 0; opIndex < Instrux->OperandsCount; ++opIndex)
    {
        status = NdParseOperand(Instrux, Code, Instrux->Length, Size, opIndex, pIns->Operands[opIndex]);
        if (!ND_SUCCESS(status))
        {
            return status;
        }
    }

    // Check if the instruction is XACQUIRE or XRELEASE enabled.
    if ((Instrux->Rep != 0) && (Instrux->HasLock || (!!Instrux->ValidPrefixes.HleNoLock)) &&
        (Instrux->Operands[0].Type == ND_OP_MEM))
    {
        if ((ND_XACQUIRE_SUPPORT(Instrux) || ND_HLE_SUPPORT(Instrux)) && (Instrux->Rep == ND_PREFIX_G1_XACQUIRE))
        {
            Instrux->IsXacquireEnabled = true;
        }
        else if ((ND_XRELEASE_SUPPORT(Instrux) || ND_HLE_SUPPORT(Instrux)) && (Instrux->Rep == ND_PREFIX_G1_XRELEASE))
        {
            Instrux->IsXreleaseEnabled = true;
        }
    }

    // Check if the instruction is REPed.
    Instrux->IsRepeated = ((Instrux->Rep != 0) && (ND_REP_SUPPORT(Instrux) || ND_REPC_SUPPORT(Instrux)));

    // Check if the instruction is CET tracked. The do not track prefix (0x3E) works only for indirect near JMP and CALL
    // via register. It is always enabled for indirect far JMP and CALL or near indirect JMP and CALL via memoery.
    Instrux->IsCetTracked = ND_HAS_CETT(Instrux) && ((!ND_DNT_SUPPORT(Instrux)) ||
                                                     (Instrux->Seg != ND_PREFIX_G2_NO_TRACK) ||
                                                     (Instrux->HasModRm && (Instrux->ModRm.mod != 3)));

    // Do instruction validations. These checks are made in order to filter out encodings that would normally
    // be invalid and would generate #UD.
    status = NdValidateInstruction(Instrux);
    if (!ND_SUCCESS(status))
    {
        return status;
    }

    // Copy the instruction bytes.
    for (opIndex = 0; opIndex < Instrux->Length; opIndex++)
    {
        Instrux->InstructionBytes[opIndex] = Code[opIndex];
    }

    // All done! Instruction successfully decoded!
    return ND_STATUS_SUCCESS;
}


//
// NdDecodeEx
//
NDSTATUS
NdDecodeEx(
    INSTRUX *Instrux,
    const uint8_t *Code,
    size_t Size,
    uint8_t DefCode,
    uint8_t DefData
    )
{
    return NdDecodeEx2(Instrux, Code, Size, DefCode, DefData, DefCode, ND_VEND_ANY);
}


//
// NdDecode
//
NDSTATUS
NdDecode(
    INSTRUX *Instrux,
    const uint8_t *Code,
    uint8_t DefCode,
    uint8_t DefData
    )
{
    return NdDecodeEx2(Instrux, Code, ND_MAX_INSTRUCTION_LENGTH, DefCode, DefData, DefCode, ND_VEND_ANY);
}


//
// NdToText
//
NDSTATUS
NdToText(
    const INSTRUX *Instrux,
    uint64_t Rip,
    uint32_t BufferSize,
    char *Buffer
    )
{
    NDSTATUS status;
    char *res, temp[64];
    uint32_t opIndex, opsStored;
    const ND_OPERAND *pOp;
    bool alignmentStored;

    // pre-init
    status = ND_STATUS_SUCCESS;
    res = NULL;
    opIndex = 0;
    opsStored = 0;
    pOp = NULL;
    alignmentStored = false;

    // Validate args.
    if (NULL == Instrux)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (NULL == Buffer)
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
               RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
           }
           else if (Instrux->Rep == ND_PREFIX_G1_REPNE_REPNZ)
           {
               res = nd_strcat_s(Buffer, BufferSize, "REPNZ ");
               RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
           }
        }

        // Check for REP support and store prefixes.
        if (ND_REP_SUPPORT(Instrux))
        {
           if (Instrux->Rep == ND_PREFIX_G1_REPE_REPZ)
           {
               res = nd_strcat_s(Buffer, BufferSize, "REP ");
               RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
           }
           else if (Instrux->Rep == ND_PREFIX_G1_REPNE_REPNZ)
           {
               res = nd_strcat_s(Buffer, BufferSize, "REPNZ ");
               RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
           }
        }

        if (Instrux->IsXreleaseEnabled)
        {
            res = nd_strcat_s(Buffer, BufferSize, "XRELEASE ");
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
        else if (Instrux->IsXacquireEnabled)
        {
            res = nd_strcat_s(Buffer, BufferSize, "XACQUIRE ");
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
    }

    if (Instrux->HasLock)
    {
        if (ND_LOCK_SUPPORT(Instrux))
        {
            res = nd_strcat_s(Buffer, BufferSize, "LOCK ");
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
    }

    if (Instrux->Rep == ND_PREFIX_G1_BND)
    {
       if (ND_BND_SUPPORT(Instrux))
       {
            res = nd_strcat_s(Buffer, BufferSize, "BND ");
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
       }
    }

    if (Instrux->HasSeg && ND_BHINT_SUPPORT(Instrux))
    {
       switch (Instrux->Bhint)
       {
       case ND_PREFIX_G2_BR_TAKEN:
            res = nd_strcat_s(Buffer, BufferSize, "BHT ");
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
            break;

        case ND_PREFIX_G2_BR_NOT_TAKEN:
            res = nd_strcat_s(Buffer, BufferSize, "BHNT ");
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
            break;

        case ND_PREFIX_G2_BR_ALT:
            res = nd_strcat_s(Buffer, BufferSize, "BHALT ");
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
    }

    // Store the mnemonic.
    res = nd_strcat_s(Buffer, BufferSize, Instrux->Mnemonic);
    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);

    // Store condition code, if any.
    if (ND_HAS_SSE_CONDITION(Instrux))
    {
        res = nd_strcat_s(Buffer, BufferSize, gConditionCodes[Instrux->SseCondition]);
        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
            size_t idx = 0;

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

            alignmentStored = true;
        }

        // Store the comma, if this isn't the first operand.
        if (opsStored > 0)
        {
            res = nd_strcat_s(Buffer, BufferSize, ", ");
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        }
                        else
                        {
                            res = nd_strcat_s(Buffer, BufferSize, gReg8Bit[pOp->Info.Register.Reg]);
                            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        }
                        break;

                    case ND_SIZE_16BIT:
                        // 16 bit register.
                        res = nd_strcat_s(Buffer, BufferSize, gReg16Bit[pOp->Info.Register.Reg]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;

                    case ND_SIZE_32BIT:
                        // 32 bit register.
                        res = nd_strcat_s(Buffer, BufferSize, gReg32Bit[pOp->Info.Register.Reg]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;

                    case ND_SIZE_64BIT:
                        // 64 bit register.
                        res = nd_strcat_s(Buffer, BufferSize, gReg64Bit[pOp->Info.Register.Reg]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                }
                break;

            case ND_REG_FPU:
                {
                    if (pOp->Info.Register.Reg >= ND_MAX_FPU_REGS)
                    {
                        return ND_STATUS_INVALID_INSTRUX;
                    }

                    res = nd_strcat_s(Buffer, BufferSize, gRegFpu[pOp->Info.Register.Reg]);
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                }
                break;

            case ND_REG_MMX:
                {
                    if (pOp->Info.Register.Reg >= ND_MAX_MMX_REGS)
                    {
                        return ND_STATUS_INVALID_INSTRUX;
                    }

                    res = nd_strcat_s(Buffer, BufferSize, gRegMmx[pOp->Info.Register.Reg]);
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_256BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gRegYmm[pOp->Info.Register.Reg]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_512BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gRegZmm[pOp->Info.Register.Reg]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                }
                break;

            case ND_REG_DR:
                {
                    if (pOp->Info.Register.Reg >= ND_MAX_DR_REGS)
                    {
                        return ND_STATUS_INVALID_INSTRUX;
                    }

                    res = nd_strcat_s(Buffer, BufferSize, gRegDebug[pOp->Info.Register.Reg]);
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                }
                break;

            case ND_REG_TR:
                {
                    if (pOp->Info.Register.Reg >= ND_MAX_TR_REGS)
                    {
                        return ND_STATUS_INVALID_INSTRUX;
                    }

                    res = nd_strcat_s(Buffer, BufferSize, gRegTest[pOp->Info.Register.Reg]);
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

        case ND_OP_IMM:
            {
                switch (pOp->Size)
                {
                case 1:
                    status = NdSprintf(temp, sizeof(temp), "0x%02x", (uint8_t)pOp->Info.Immediate.Imm);
                    break;
                case 2:
                    status = NdSprintf(temp, sizeof(temp), "0x%04x", (uint16_t)pOp->Info.Immediate.Imm);
                    break;
                case 4:
                    status = NdSprintf(temp, sizeof(temp), "0x%08x", (uint32_t)pOp->Info.Immediate.Imm);
                    break;
                case 8:
                    status = NdSprintf(temp, sizeof(temp), "0x%016llx", (uint64_t)pOp->Info.Immediate.Imm);
                    break;
                }
                if (!ND_SUCCESS(status))
                {
                   return status;
                }

                res = nd_strcat_s(Buffer, BufferSize, temp);
                RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

        case ND_OP_OFFS:
            {
                uint64_t dest = Rip + Instrux->Length + pOp->Info.RelativeOffset.Rel;

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
                RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
            }
            break;

        case ND_OP_ADDR:
            {
                switch (Instrux->AddrLength)
                {
                case 4:
                    status = NdSprintf(temp, sizeof(temp), "0x%04x:0x%04x",
                                       pOp->Info.Address.BaseSeg, (uint16_t)pOp->Info.Address.Offset);
                    break;
                case 6:
                    status = NdSprintf(temp, sizeof(temp), "0x%04x:0x%08x",
                                       pOp->Info.Address.BaseSeg, (uint32_t)pOp->Info.Address.Offset);
                    break;
                case 10:
                    status = NdSprintf(temp, sizeof(temp), "0x%04x:0x%016llx",
                                       pOp->Info.Address.BaseSeg, (uint64_t)pOp->Info.Address.Offset);
                    break;
                default:
                    return ND_STATUS_INVALID_INSTRUX;
                }

                if (!ND_SUCCESS(status))
                {
                    return status;
                }

                res = nd_strcat_s(Buffer, BufferSize, temp);
                RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case 2:
                    res = nd_strcat_s(Buffer, BufferSize, "word ptr ");
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case 4:
                    res = nd_strcat_s(Buffer, BufferSize, "dword ptr ");
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case 6:
                    res = nd_strcat_s(Buffer, BufferSize, "fword ptr ");
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case 8:
                    res = nd_strcat_s(Buffer, BufferSize, "qword ptr ");
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case 10:
                    res = nd_strcat_s(Buffer, BufferSize, "tbyte ptr ");
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case 16:
                    res = nd_strcat_s(Buffer, BufferSize, "xmmword ptr ");
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case 32:
                    res = nd_strcat_s(Buffer, BufferSize, "ymmword ptr ");
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case 48:
                    res = nd_strcat_s(Buffer, BufferSize, "m384 ptr ");
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    break;
                case 64:
                    res = nd_strcat_s(Buffer, BufferSize, "zmmword ptr ");
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);

                        res = nd_strcat_s(Buffer, BufferSize, ":");
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    }
                }

                // Prepend the "["
                res = nd_strcat_s(Buffer, BufferSize, "[");
                RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);

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
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_16BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gReg16Bit[pOp->Info.Memory.Base]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_32BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gReg32Bit[pOp->Info.Memory.Base]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_64BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gReg64Bit[pOp->Info.Memory.Base]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    }

                    switch (pOp->Info.Memory.IndexSize)
                    {
                    case ND_SIZE_8BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gReg8Bit[pOp->Info.Memory.Index]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_16BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gReg16Bit[pOp->Info.Memory.Index]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_32BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gReg32Bit[pOp->Info.Memory.Index]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_64BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gReg64Bit[pOp->Info.Memory.Index]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_128BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gRegXmm[pOp->Info.Memory.Index]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_256BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gRegYmm[pOp->Info.Memory.Index]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                        break;
                    case ND_SIZE_512BIT:
                        res = nd_strcat_s(Buffer, BufferSize, gRegZmm[pOp->Info.Memory.Index]);
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    }
                }

                // Handle displacement.
                if (pOp->Info.Memory.HasDisp)
                {
                    uint64_t normDisp, disp;

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
                            normDisp = ((disp & 0x80) ? ~((uint8_t)disp) + 1ULL : disp) & 0xFF;
                            break;
                        case 2:
                            normDisp = ((disp & 0x8000) ? ~((uint16_t)disp) + 1ULL : disp) & 0xFFFF;
                            break;
                        case 4:
                            normDisp = ((disp & 0x80000000) ? ~((uint32_t)disp) + 1ULL : disp) & 0xFFFFFFFF;
                            break;
                        default:
                            normDisp = disp;
                            break;
                        }

                        // Handle compressed displacement. It is fine to cast the normDisp to uint32_t, as the
                        // compressed displacement only works with uint8_t displacements. Also, in this phase,
                        // the normDisp is converted to a positive quantity, so no sign-extension is needed.
                        if (pOp->Info.Memory.HasCompDisp)
                        {
                            normDisp = (uint64_t)(uint32_t)normDisp * pOp->Info.Memory.CompDispSize;
                        }
                    }


                    // Now displacement.
                    if (pOp->Info.Memory.HasBase || pOp->Info.Memory.HasIndex)
                    {
                        res = nd_strcat_s(Buffer, BufferSize, Instrux->SignDisp ? "-" : "+");
                        RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                    }

                    if (pOp->Info.Memory.IsRipRel)
                    {
                        uint64_t target = disp + Rip + Instrux->Length;

                        if (Instrux->AddrMode == ND_ADDR_32)
                        {
                            target &= 0xFFFFFFFF;
                        }

                        status = NdSprintf(temp, sizeof(temp), "rel 0x%llx", target);
                    }
                    else
                    {
                        uint8_t trimSize;

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
                    RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
                }

                // And the ending "]"
                res = nd_strcat_s(Buffer, BufferSize, "]");
                RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
        }

        // Handle zeroing. Note that zeroing without masking is ignored.
        if (pOp->Decorator.HasZero && pOp->Decorator.HasMask)
        {
            res = nd_strcat_s(Buffer, BufferSize, "{z}");
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
        }

        // Append Suppress All Exceptions decorator.
        if (pOp->Decorator.HasSae && !pOp->Decorator.HasEr)
        {
            // ER implies SAE, so if we have ER, we will list that.
            res = nd_strcat_s(Buffer, BufferSize, ", {sae}");
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
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
            RET_EQ(res, NULL, ND_STATUS_BUFFER_OVERFLOW);
        }
    }

   return ND_STATUS_SUCCESS;
}


//
// NdIsInstruxRipRelative
//
bool
NdIsInstruxRipRelative(
    const INSTRUX *Instrux
    )
//
// Provided for backwards compatibility with existing code that uses disasm 1.0
//
{
    if (NULL == Instrux)
    {
        return false;
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
    uint32_t i;
    const ND_OPERAND *pOp;

    // pre-init
    i = 0;
    pOp = NULL;

    // validate
    if (NULL == Instrux)
    {
        return ND_STATUS_INVALID_PARAMETER;
    }

    if (NULL == AccessMap)
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
                    uint32_t k;

                    for (k = 0; k < pOp->Info.Register.Count; k++)
                    {
                        AccessMap->GprAccess[pOp->Info.Register.Reg + k] |= pOp->Access.Access;
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
                    uint32_t k;

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
            default:
                break;
            }
        }
        else if (ND_OP_BANK == Instrux->Operands[i].Type)
        {
            uint8_t j;

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


void
NdInitContext(
    ND_CONTEXT *Context
    )
{
    nd_memzero(Context, sizeof(*Context));
}
