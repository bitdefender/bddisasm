/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <Intrin.h>

// Main disasm header file.
#include "bdshemu.h"
#include "bddisasm.h"

#include "disasmtool.h"


const char *gSpaces[16] = 
{
    "",
    "  ",
    "    ",
    "      ",
    "        ",
    "          ",
    "            ",
    "              ",
    "                ",
    "                  ",
    "                    ",
    "                      ",
    "                        ",
    "                          ",
    "                            ",
    "                              ",
};


#if !defined(BDDISASM_HAS_VSNPRINTF)
//
// nd_vsnprintf
//
int
nd_vsnprintf_s(
    char *buffer,
    size_t sizeOfBuffer,
    size_t count,
    const char *format,
    va_list argptr
    )
{
    return _vsnprintf_s(buffer, sizeOfBuffer, count, format, argptr);
}
#endif // !defined(BDDISASM_HAS_VSNPRINTF)

#if !defined(BDDISASM_HAS_MEMSET)
void* 
nd_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}
#endif // !defined(BDDISASM_HAS_MEMSET)

void 
ShemuLog(
    __in PCHAR Data
    )
{
    printf("%s", Data);
}

bool 
ShemuAccessMem(
    __in PSHEMU_CONTEXT Ctx, 
    __in uint64_t Gla, 
    __in size_t Size, 
    __inout uint8_t *Buffer, 
    __in bool Store
    )
{
    UNREFERENCED_PARAMETER(Ctx);
    UNREFERENCED_PARAMETER(Gla);

    if (!Store)
    {
        // On loads, always return 0.
        memset(Buffer, 0, Size);
    }
    else
    {
        // On stores, do nothing.
    }

    return true;
}


const char* 
set_to_string(
    __in ND_INS_SET Set
    )
{
    switch (Set)
    {
    case ND_SET_3DNOW:                 return "3DNOW";
    case ND_SET_ADX:                   return "ADX";
    case ND_SET_AES:                   return "AES";
    case ND_SET_AMD:                   return "AMD";
    case ND_SET_AMXBF16:               return "AMX-BF16";
    case ND_SET_AMXFP16:               return "AMX-FP16";
    case ND_SET_AMXINT8:               return "AMX-INT8";
    case ND_SET_AMXTILE:               return "AMX-TILE";
    case ND_SET_AMXCOMPLEX:            return "AMX-COMPLEX";
    case ND_SET_AVX:                   return "AVX";
    case ND_SET_AVX2:                  return "AVX2";
    case ND_SET_AVX2GATHER:            return "AVX2GATHER";
    case ND_SET_AVX5124FMAPS:          return "AVX5124FMAPS";
    case ND_SET_AVX5124VNNIW:          return "AVX5124VNNIW";
    case ND_SET_AVX512BF16:            return "AVX512BF16";
    case ND_SET_AVX512BITALG:          return "AVX512BITALG";
    case ND_SET_AVX512BW:              return "AVX512BW";
    case ND_SET_AVX512CD:              return "AVX512CD";
    case ND_SET_AVX512DQ:              return "AVX512DQ";
    case ND_SET_AVX512ER:              return "AVX512ER";
    case ND_SET_AVX512F:               return "AVX512F";
    case ND_SET_AVX512IFMA:            return "AVX512IFMA";
    case ND_SET_AVX512PF:              return "AVX512PF";
    case ND_SET_AVX512VBMI:            return "AVX512VBMI";
    case ND_SET_AVX512VBMI2:           return "AVX512VBMI2";
    case ND_SET_AVX512VNNI:            return "AVX512VNNI";
    case ND_SET_AVX512VP2INTERSECT:    return "AVX512VP2INTERSECT";
    case ND_SET_AVX512VPOPCNTDQ:       return "AVX512VPOPCNTDQ";
    case ND_SET_AVX512FP16:            return "AVX512FP16";
    case ND_SET_AVXIFMA:               return "AVXIFMA";
    case ND_SET_AVXNECONVERT:          return "AVXNECONVERT";
    case ND_SET_AVXVNNI:               return "AVXVNNI";
    case ND_SET_AVXVNNIINT8:           return "AVXVNNIINT8";
    case ND_SET_BMI1:                  return "BMI1";
    case ND_SET_BMI2:                  return "BMI2";
    case ND_SET_CET_SS:                return "CET_SS";
    case ND_SET_CET_IBT:               return "CET_IBT";
    case ND_SET_CLDEMOTE:              return "CLDEMOTE";
    case ND_SET_CLFSH:                 return "CLFSH";
    case ND_SET_CLFSHOPT:              return "CLFSHOPT";
    case ND_SET_CLWB:                  return "CLWB";
    case ND_SET_CLZERO:                return "CLZERO";
    case ND_SET_CMPCCXADD:             return "CMPCCXADD";
    case ND_SET_CMPXCHG16B:            return "CMPXCHG16B";
    case ND_SET_CYRIX:                 return "CYRIX";
    case ND_SET_CYRIX_SMM:             return "CYRIX_SMM";
    case ND_SET_ENQCMD:                return "ENQCMD";
    case ND_SET_F16C:                  return "F16C";
    case ND_SET_FMA:                   return "FMA";
    case ND_SET_FMA4:                  return "FMA4";
    case ND_SET_FRED:                  return "FRED";
    case ND_SET_FXSAVE:                return "FXSAVE";
    case ND_SET_GFNI:                  return "GFNI";
    case ND_SET_HRESET:                return "HRESET";
    case ND_SET_I186:                  return "I186";
    case ND_SET_I286PROT:              return "I286PROT";
    case ND_SET_I286REAL:              return "I286REAL";
    case ND_SET_I386:                  return "I386";
    case ND_SET_I486:                  return "I486";
    case ND_SET_I486REAL:              return "I486REAL";
    case ND_SET_I64:                   return "I64";
    case ND_SET_I86:                   return "I86";
    case ND_SET_INVPCID:               return "INVPCID";
    case ND_SET_INVLPGB:               return "INVLPGB";
    case ND_SET_KL:                    return "KL";
    case ND_SET_LKGS:                  return "LKGS";
    case ND_SET_LONGMODE:              return "LONGMODE";
    case ND_SET_LWP:                   return "LWP";
    case ND_SET_LZCNT:                 return "LZCNT";
    case ND_SET_MCOMMIT:               return "MCOMMIT";
    case ND_SET_MMX:                   return "MMX";
    case ND_SET_MOVBE:                 return "MOVBE";
    case ND_SET_MOVDIR64B:             return "MOVDIR64B";
    case ND_SET_MOVDIRI:               return "MOVDIRI";
    case ND_SET_MPX:                   return "MPX";
    case ND_SET_MSRLIST:               return "MSRLIST";
    case ND_SET_MWAITT:                return "MWAITT";
    case ND_SET_PAUSE:                 return "PAUSE";
    case ND_SET_PCLMULQDQ:             return "PCLMULQDQ";
    case ND_SET_PCONFIG:               return "PCONFIG";
    case ND_SET_PENTIUMREAL:           return "PENTIUMREAL";
    case ND_SET_PKU:                   return "PKU";
    case ND_SET_POPCNT:                return "POPCNT";
    case ND_SET_PPRO:                  return "PPRO";
    case ND_SET_PREFETCHITI:           return "PREFETCHITI";
    case ND_SET_PREFETCH_NOP:          return "PREFETCH_NOP";
    case ND_SET_PTWRITE:               return "PTWRITE";
    case ND_SET_RAOINT:                return "RAOINT";
    case ND_SET_RDPID:                 return "RDPID";
    case ND_SET_RDPMC:                 return "RDPMC";
    case ND_SET_RDPRU:                 return "RDPRU";
    case ND_SET_RDRAND:                return "RDRAND";
    case ND_SET_RDSEED:                return "RDSEED";
    case ND_SET_RDTSCP:                return "RDTSCP";
    case ND_SET_RDWRFSGS:              return "RDWRFSGS";
    case ND_SET_SERIALIZE:             return "SERIALIZE";
    case ND_SET_SGX:                   return "SGX";
    case ND_SET_SHA:                   return "SHA";
    case ND_SET_SMAP:                  return "SMAP";
    case ND_SET_SMX:                   return "SMX";
    case ND_SET_SNP:                   return "SNP";
    case ND_SET_SSE:                   return "SSE";
    case ND_SET_SSE2:                  return "SSE2";
    case ND_SET_SSE3:                  return "SSE3";
    case ND_SET_SSE4:                  return "SSE4";
    case ND_SET_SSE42:                 return "SSE42";
    case ND_SET_SSE4A:                 return "SSE4A";
    case ND_SET_SSSE3:                 return "SSSE3";
    case ND_SET_SVM:                   return "SVM";
    case ND_SET_TBM:                   return "TBM";
    case ND_SET_TDX:                   return "TDX";
    case ND_SET_TSX:                   return "TSX";
    case ND_SET_TSXLDTRK:              return "TSXLDTRK";
    case ND_SET_UD:                    return "UD";
    case ND_SET_UINTR:                 return "UINTR";
    case ND_SET_UNKNOWN:               return "UNKNOWN";
    case ND_SET_VAES:                  return "VAES";
    case ND_SET_VPCLMULQDQ:            return "VPCLMULQDQ";
    case ND_SET_VTX:                   return "VTX";
    case ND_SET_WAITPKG:               return "WAITPKG";
    case ND_SET_WBNOINVD:              return "WBNOINVD";
    case ND_SET_WRMSRNS:               return "WRMSRNS";
    case ND_SET_X87:                   return "X87";
    case ND_SET_XOP:                   return "XOP";
    case ND_SET_XSAVE:                 return "XSAVE";
    case ND_SET_XSAVEC:                return "XSAVEC";
    case ND_SET_XSAVES:                return "XSAVES";
    default: return "???";
    }
}


const char* 
category_to_string(
    __in ND_INS_CATEGORY Category
    )
{
    switch (Category)
    {
    case ND_CAT_3DNOW:                return "3DNOW";
    case ND_CAT_AES:                  return "AES";
    case ND_CAT_AESKL:                return "AESKL";
    case ND_CAT_ARITH:                return "ARITH";
    case ND_CAT_AMX:                  return "AMX";
    case ND_CAT_AVX:                  return "AVX";
    case ND_CAT_AVX2:                 return "AVX2";
    case ND_CAT_AVX2GATHER:           return "AVX2GATHER";
    case ND_CAT_AVX512:               return "AVX512";
    case ND_CAT_AVX512BF16:           return "AVX512BF16";
    case ND_CAT_AVX512VBMI:           return "AVX512VBMI";
    case ND_CAT_AVX512VP2INTERSECT:   return "AVX512VP2INTERSECT";
    case ND_CAT_AVX512FP16:           return "AVX512FP16";
    case ND_CAT_AVXIFMA:              return "AVXIFMA";
    case ND_CAT_AVXVNNI:              return "AVXVNNI";
    case ND_CAT_AVXVNNIINT8:          return "AVXVNNIINT8";
    case ND_CAT_AVXNECONVERT:         return "AVXNECONVERT";
    case ND_CAT_BITBYTE:              return "BITBYTE";
    case ND_CAT_BLEND:                return "BLEND";
    case ND_CAT_BMI1:                 return "BMI1";
    case ND_CAT_BMI2:                 return "BMI2";
    case ND_CAT_BROADCAST:            return "BROADCAST";
    case ND_CAT_CALL:                 return "CALL";
    case ND_CAT_CET:                  return "CET";
    case ND_CAT_CLDEMOTE:             return "CLDEMOTE";
    case ND_CAT_CMOV:                 return "CMOV";
    case ND_CAT_CMPCCXADD:            return "CMPCCXADD";
    case ND_CAT_COMPRESS:             return "COMPRESS";
    case ND_CAT_COND_BR:              return "COND_BR";
    case ND_CAT_CONFLICT:             return "CONFLICT";
    case ND_CAT_CONVERT:              return "CONVERT";
    case ND_CAT_DATAXFER:             return "DATAXFER";
    case ND_CAT_DECIMAL:              return "DECIMAL";
    case ND_CAT_ENQCMD:               return "ENQCMD";
    case ND_CAT_EXPAND:               return "EXPAND";
    case ND_CAT_FLAGOP:               return "FLAGOP";
    case ND_CAT_FMA4:                 return "FMA4";
    case ND_CAT_GATHER:               return "GATHER";
    case ND_CAT_GFNI:                 return "GFNI";
    case ND_CAT_HRESET:               return "HRESET";
    case ND_CAT_I386:                 return "I386";
    case ND_CAT_IFMA:                 return "IFMA";
    case ND_CAT_INTERRUPT:            return "INTERRUPT";
    case ND_CAT_IO:                   return "IO";
    case ND_CAT_IOSTRINGOP:           return "IOSTRINGOP";
    case ND_CAT_KL:                   return "KL";
    case ND_CAT_KMASK:                return "KMASK";
    case ND_CAT_KNL:                  return "KNL";
    case ND_CAT_LKGS:                 return "LKGS";
    case ND_CAT_LOGIC:                return "LOGIC";
    case ND_CAT_LOGICAL:              return "LOGICAL";
    case ND_CAT_LOGICAL_FP:           return "LOGICAL_FP";
    case ND_CAT_LWP:                  return "LWP";
    case ND_CAT_LZCNT:                return "LZCNT";
    case ND_CAT_MISC:                 return "MISC";
    case ND_CAT_MMX:                  return "MMX";
    case ND_CAT_MOVDIR64B:            return "MOVDIR64B";
    case ND_CAT_MOVDIRI:              return "MOVDIRI";
    case ND_CAT_MPX:                  return "MPX";
    case ND_CAT_NOP:                  return "NOP";
    case ND_CAT_PADLOCK:              return "PADLOCK";
    case ND_CAT_PCLMULQDQ:            return "PCLMULQDQ";
    case ND_CAT_PCONFIG:              return "PCONFIG";
    case ND_CAT_POP:                  return "POP";
    case ND_CAT_PREFETCH:             return "PREFETCH";
    case ND_CAT_PTWRITE:              return "PTWRITE";
    case ND_CAT_PUSH:                 return "PUSH";
    case ND_CAT_RAOINT:               return "RAO-INT";
    case ND_CAT_RDPID:                return "RDPID";
    case ND_CAT_RDRAND:               return "RDRAND";
    case ND_CAT_RDSEED:               return "RDSEED";
    case ND_CAT_RDWRFSGS:             return "RDWRFSGS";
    case ND_CAT_RET:                  return "RET";
    case ND_CAT_ROTATE:               return "ROTATE";
    case ND_CAT_SCATTER:              return "SCATTER";
    case ND_CAT_SEGOP:                return "SEGOP";
    case ND_CAT_SEMAPHORE:            return "SEMAPHORE";
    case ND_CAT_SGX:                  return "SGX";
    case ND_CAT_SHA:                  return "SHA";
    case ND_CAT_SHIFT:                return "SHIFT";
    case ND_CAT_SMAP:                 return "SMAP";
    case ND_CAT_SSE:                  return "SSE";
    case ND_CAT_SSE2:                 return "SSE2";
    case ND_CAT_STRINGOP:             return "STRINGOP";
    case ND_CAT_STTNI:                return "STTNI";
    case ND_CAT_SYSCALL:              return "SYSCALL";
    case ND_CAT_SYSRET:               return "SYSRET";
    case ND_CAT_SYSTEM:               return "SYSTEM";
    case ND_CAT_TDX:                  return "TDX";
    case ND_CAT_UD:                   return "UD";
    case ND_CAT_UINTR:                return "UINTR";
    case ND_CAT_UNCOND_BR:            return "UNCOND_BR";
    case ND_CAT_UNKNOWN:              return "UNKNOWN";
    case ND_CAT_VAES:                 return "VAES";
    case ND_CAT_VFMA:                 return "VFMA";
    case ND_CAT_VFMAPS:               return "VFMAPS";
    case ND_CAT_VNNI:                 return "VNNI";
    case ND_CAT_VNNIW:                return "VNNIW";
    case ND_CAT_VPCLMULQDQ:           return "VPCLMULQDQ";
    case ND_CAT_VPOPCNT:              return "VPOPCNT";
    case ND_CAT_VTX:                  return "VTX";
    case ND_CAT_WAITPKG:              return "WAITPKG";
    case ND_CAT_WBNOINVD:             return "WBNOINVD";
    case ND_CAT_WIDENOP:              return "WIDENOP";
    case ND_CAT_WIDE_KL:              return "WIDE_KL";
    case ND_CAT_X87_ALU:              return "X87_ALU";
    case ND_CAT_XOP:                  return "XOP";
    case ND_CAT_XSAVE:                return "XSAVE";
    default: return "???";
    }
}


const char* 
optype_to_string(
    __in ND_OPERAND_TYPE OpType
    )
{
    switch (OpType)
    {
    case ND_OP_REG: return "Register";
    case ND_OP_IMM: return "Immediate";
    case ND_OP_CONST: return "Constant";
    case ND_OP_MEM: return "Memory";
    case ND_OP_ADDR: return "Address";
    case ND_OP_OFFS: return "Offset";
    case ND_OP_BANK: return "Bank";
    default: return "???";
    }
}


const char* 
regtype_to_string(
    __in ND_REG_TYPE RegType
    )
{
    switch (RegType)
    {
    case ND_REG_GPR: return "General Purpose";
    case ND_REG_SEG: return "Segment";
    case ND_REG_FPU: return "FP";
    case ND_REG_MMX: return "MMX";
    case ND_REG_SSE: return "Vector";
    case ND_REG_BND: return "Bound";
    case ND_REG_MSK: return "Mask";
    case ND_REG_TILE: return "Tile";
    case ND_REG_SYS: return "System";
    case ND_REG_MSR: return "Model Specific";
    case ND_REG_XCR: return "Extended Control";
    case ND_REG_CR:  return "Control";
    case ND_REG_DR:  return "Debug";
    case ND_REG_TR:  return "Test";
    case ND_REG_MXCSR: return "MXCSR";
    case ND_REG_PKRU: return "PKRU";
    case ND_REG_SSP: return "SSP";
    case ND_REG_FLG: return "Flags";
    case ND_REG_RIP: return "IP";
    case ND_REG_UIF: return "UIF";
    default: return "???";
    }
}


const char* 
encoding_to_string(
    __in ND_OPERAND_ENCODING Encoding
    )
{
    switch (Encoding)
    {
    case ND_OPE_R: return "R";
    case ND_OPE_M: return "M";
    case ND_OPE_V: return "V";
    case ND_OPE_O: return "O";
    case ND_OPE_I: return "I";
    case ND_OPE_D: return "D";
    case ND_OPE_C: return "C";
    case ND_OPE_1: return "1";
    case ND_OPE_A: return "A";
    case ND_OPE_L: return "L";
    case ND_OPE_E: return "E";
    case ND_OPE_S: return "S";
    default: return "";
    }
}


const char*
tuple_to_string(
    __in ND_TUPLE Tuple
    )
{
    switch (Tuple)
    {
    case ND_TUPLE_FV: return "Full";
    case ND_TUPLE_HV: return "Half";
    case ND_TUPLE_FVM: return "Full Mem";
    case ND_TUPLE_HVM: return "Half Mem";
    case ND_TUPLE_QVM: return "Quarter Mem";
    case ND_TUPLE_OVM: return "Eigth Mem";
    case ND_TUPLE_T1S: return "Tuple 1 Scalar";
    case ND_TUPLE_T1F: return "Tuple 1 Fixes";
    case ND_TUPLE_T2: return "Tuple 2";
    case ND_TUPLE_T4: return "Tuple 4";
    case ND_TUPLE_T8: return "Tuple 8";
    case ND_TUPLE_M128: return "Mem 128";
    case ND_TUPLE_DUP: return "MOVDDUP";
    case ND_TUPLE_T1S8: return "Tuple 1 scalar, 8 bit";
    case ND_TUPLE_T1S16: return "Tuple 1 scalar, 16 bit";
    case ND_TUPLE_T1_4X: return "Tuple 4 x 32 bit";
    default: return "None";
    }
}


const char*
exception_evex_to_string(
    __in ND_EX_TYPE_EVEX ExClass
    )
{
    switch (ExClass)
    {
    case ND_EXT_E1: return "E1";
    case ND_EXT_E1NF: return "E1NF";
    case ND_EXT_E2: return "E2";
    case ND_EXT_E3: return "E3";
    case ND_EXT_E3NF: return "E3NF";
    case ND_EXT_E4: return "E4";
    case ND_EXT_E4nb: return "E4.nb";
    case ND_EXT_E4NF: return "E4NF";
    case ND_EXT_E4NFnb: return "E4NF.nb";
    case ND_EXT_E5: return "E5";
    case ND_EXT_E5NF: return "E5NF";
    case ND_EXT_E6: return "E6";
    case ND_EXT_E6NF: return "E6NF";
    case ND_EXT_E7NM: return "E7NM";
    case ND_EXT_E9: return "E9";
    case ND_EXT_E9NF: return "E9NF";
    case ND_EXT_E10: return "E10";
    case ND_EXT_E10NF: return "E10NF";
    case ND_EXT_E11: return "E11";
    case ND_EXT_E12: return "E12";
    case ND_EXT_E12NP: return "E12NP";
    default: return "None";
    }
}


BYTE
hex_to_bin(
    __in char HexByte
    )
{
    // Transforms one hex-digit to a number.
    if ((HexByte >= '0') && (HexByte <= '9'))
    {
        return HexByte - '0';
    }
    else if ((HexByte >= 'A') && (HexByte <= 'F'))
    {
        return HexByte - 'A' + 10;
    }
    else if ((HexByte >= 'a') && (HexByte <= 'f'))
    {
        return HexByte - 'a' + 10;
    }

    return 0;
}


INT32
regstr_to_idx(
    __in const char* Reg
    )
{
    static const char* reg64[] =
    {
        "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
        "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15"
    };

    for (INT32 i = 0; i < (INT32)ARRAYSIZE(reg64); i++)
    {
        if (!_stricmp(Reg, reg64[i]))
        {
            return i;
        }
    }

    return -1;
}


_Success_(return)
BOOLEAN
match_gpr(
    __in const char* Arg,
    __out DWORD* Index
    )
{
    // Check if the provided argument is a register.
    if (Arg[0] == '-')
    {
        INT32 idx = regstr_to_idx(Arg + 1); // this will be the name of the register or the NULL terminator
        if (idx >= 0)
        {
            *Index = idx;
            return TRUE;
        }
    }

    return FALSE;
}


void
print_instruction(
    __in SIZE_T Rip,
    __in PINSTRUX Instrux,
    __in PDISASM_OPTIONS Options
    )
{
    char instruxText[ND_MIN_BUF_SIZE];
    DWORD k = 0, idx = 0, i = 0;

    printf("%p ", (void*)(Rip));

    if (Options->Highlight)
    {
        // Display prefixes.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
        for (idx = 0; idx < Instrux->PrefLength; idx++, k++)
        {
            printf("%02x", Instrux->InstructionBytes[k]);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

        // Display opcodes.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        for (idx = 0; idx < (DWORD)(ND_IS_3DNOW(Instrux) ? Instrux->OpLength - 1 : Instrux->OpLength); idx++, k++)
        {
            printf("%02x", Instrux->InstructionBytes[k]);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

        // Display modrm and sib.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
        for (idx = 0; idx < (DWORD)(Instrux->HasModRm + Instrux->HasSib); idx++, k++)
        {
            printf("%02x", Instrux->InstructionBytes[k]);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

        // Display displacement.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_BLUE | FOREGROUND_INTENSITY);

        for (idx = 0; idx < (DWORD)(Instrux->DispLength); idx++, k++)
        {
            printf("%02x", Instrux->InstructionBytes[k]);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

        // Display relative offset/moffset/immediates.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_RED | FOREGROUND_INTENSITY);
        for (idx = 0; idx < (DWORD)(Instrux->Imm1Length + Instrux->Imm2Length +
            Instrux->RelOffsLength + Instrux->MoffsetLength +
            Instrux->HasSseImm + Instrux->AddrLength); idx++, k++)
        {
            printf("%02x", Instrux->InstructionBytes[k]);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

        if (ND_IS_3DNOW(Instrux))
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            for (; k < Instrux->Length; k++)
            {
                printf("%02x", Instrux->InstructionBytes[k]);
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        }
    }

    for (; k < Instrux->Length; k++)
    {
        printf("%02x", Instrux->InstructionBytes[k]);
    }

    printf("%s", gSpaces[16 - (Instrux->Length & 0xF)]);

    NdToText(Instrux, Rip, ND_MIN_BUF_SIZE, instruxText);

    printf("%s\n", instruxText);

    if (Options->ExtendedInfo)
    {
        const BYTE opsize[3] = { 2, 4, 8 };
        const BYTE adsize[3] = { 2, 4, 8 };
        const BYTE veclen[3] = { 16, 32, 64 };

        printf("        DSIZE: %2d, ASIZE: %2d, VLEN: ", opsize[Instrux->EfOpMode] * 8, adsize[Instrux->AddrMode] * 8);

        if (ND_HAS_VECTOR(Instrux))
        {
            printf("%2d\n", veclen[Instrux->VecMode] * 8);
        }
        else
        {
            printf("-\n");
        }

        printf("        ISA Set: %s, Ins cat: %s, CET tracked: %s\n",
            set_to_string(Instrux->IsaSet), category_to_string(Instrux->Category),
            Instrux->IsCetTracked ? "yes" : "no");

        if (0 != Instrux->CpuidFlag.Flag)
        {
            char *regs[4] = { "eax", "ecx", "edx", "ebx" };

            printf("        CPUID leaf: 0x%08x", Instrux->CpuidFlag.Leaf);

            if (Instrux->CpuidFlag.SubLeaf != ND_CFF_NO_SUBLEAF)
            {
                printf(", sub-leaf: 0x%08x", Instrux->CpuidFlag.SubLeaf);
            }

            printf(", reg: %s, bit: %u\n", regs[Instrux->CpuidFlag.Reg], Instrux->CpuidFlag.Bit);
        }

        if (Instrux->HasEvex)
        {
            printf("        EVEX Tuple Type: %s\n", 
                tuple_to_string((ND_TUPLE)Instrux->TupleType));
        }

        if (Instrux->ExceptionClass != ND_EXC_None)
        {
            printf("        Exception class: %s, ", 
                Instrux->ExceptionClass == ND_EXC_SSE_AVX ? "SSE/VEX" :
                Instrux->ExceptionClass == ND_EXC_EVEX ? "EVEX" :
                Instrux->ExceptionClass == ND_EXC_OPMASK ? "Opmask" : 
                Instrux->ExceptionClass == ND_EXC_AMX ? "AMX" : "???");

            switch (Instrux->ExceptionClass)
            {
            case ND_EXC_SSE_AVX:
                printf("exception type: %d\n", Instrux->ExceptionType);
                break;
            case ND_EXC_EVEX:
                printf("exception type: %s\n", exception_evex_to_string((ND_EX_TYPE_EVEX)Instrux->ExceptionType));
                break;
            case ND_EXC_OPMASK:
                printf("exception type: K%d\n", Instrux->ExceptionType + 19);
                break;
            case ND_EXC_AMX:
                printf("exception type: AMX-E%d\n", Instrux->ExceptionType);
                break;
            default:
                printf("exception type: ???\n");
                break;
            }
        }

        if (Instrux->FlagsAccess.RegAccess != 0)
        {
            DWORD fidx, all;
            BOOLEAN individual = FALSE;
            char *flags[22] = { "CF", NULL, "PF", NULL, "AF", NULL, "ZF", "SF", "TF", "IF", "DF", 
                "OF", "IOPL",NULL, "NT", NULL, "RF", "VM", "AC", "VIF", "VIP", "ID" };

            all = Instrux->FlagsAccess.Tested.Raw | Instrux->FlagsAccess.Modified.Raw | Instrux->FlagsAccess.Set.Raw | 
                  Instrux->FlagsAccess.Cleared.Raw | Instrux->FlagsAccess.Undefined.Raw;
            printf("        FLAGS access\n                ");

            for (fidx = 0; fidx < 21; fidx++)
            {
                if (flags[fidx] != NULL)
                {
                    if (0 == (all & (1ULL << fidx)))
                    {
                        continue;
                    }

                    individual = TRUE;

                    printf("%s: ", flags[fidx]);

                    if (Instrux->FlagsAccess.Tested.Raw & (1ULL << fidx))
                    {
                        printf("t");
                    }

                    if (Instrux->FlagsAccess.Modified.Raw & (1ULL << fidx))
                    {
                        printf("m");
                    }

                    if (Instrux->FlagsAccess.Set.Raw & (1ULL << fidx))
                    {
                        printf("1");
                    }

                    if (Instrux->FlagsAccess.Cleared.Raw & (1ULL << fidx))
                    {
                        printf("0");
                    }

                    if (Instrux->FlagsAccess.Undefined.Raw & (1ULL << fidx))
                    {
                        printf("u");
                    }

                    printf(", ");
                }
            }

            if (!individual)
            {
                printf("Entire register");
            }

            printf("\n");
        }

        if (Instrux->IsaSet == ND_SET_X87)
        {
#define FPU_FLG_ACC_TO_STR(x)   ((x) == ND_FPU_FLAG_UNDEFINED ? "u" : \
                                (x) == ND_FPU_FLAG_MODIFIED ? "m" : (x) == ND_FPU_FLAG_SET_0 ? "0" : "1")

            printf("        FPU flags access\n                C0: %s, C1: %s, C2: %s, C3: %s, \n",
                FPU_FLG_ACC_TO_STR(Instrux->FpuFlagsAccess.C0), FPU_FLG_ACC_TO_STR(Instrux->FpuFlagsAccess.C1),
                FPU_FLG_ACC_TO_STR(Instrux->FpuFlagsAccess.C2), FPU_FLG_ACC_TO_STR(Instrux->FpuFlagsAccess.C3));
        }

        printf("        Valid modes\n"
               "                R0: %s, R1: %s, R2: %s, R3: %s\n"
               "                Real: %s, V8086: %s, Prot: %s, Compat: %s, Long: %s\n"
               "                SMM on: %s, SMM off: %s, SGX on: %s, SGX off: %s, TSX on: %s, TSX off: %s\n"
               "                VMXRoot: %s, VMXNonRoot: %s, VMXRoot SEAM: %s, VMXNonRoot SEAM: %s, VMX off: %s\n",
               Instrux->ValidModes.Ring0 ? "yes" : "no",
               Instrux->ValidModes.Ring1 ? "yes" : "no",
               Instrux->ValidModes.Ring2 ? "yes" : "no",
               Instrux->ValidModes.Ring3 ? "yes" : "no",
               Instrux->ValidModes.Real ? "yes" : "no",
               Instrux->ValidModes.V8086 ? "yes" : "no",
               Instrux->ValidModes.Protected ? "yes" : "no",
               Instrux->ValidModes.Compat ? "yes" : "no",
               Instrux->ValidModes.Long ? "yes" : "no",
               Instrux->ValidModes.Smm ? "yes" : "no",
               Instrux->ValidModes.SmmOff ? "yes" : "no",
               Instrux->ValidModes.Sgx ? "yes" : "no",
               Instrux->ValidModes.SgxOff ? "yes" : "no",
               Instrux->ValidModes.Tsx ? "yes" : "no",
               Instrux->ValidModes.TsxOff ? "yes" : "no",
               Instrux->ValidModes.VmxRoot ? "yes" : "no",
               Instrux->ValidModes.VmxNonRoot ? "yes" : "no",
               Instrux->ValidModes.VmxRootSeam ? "yes" : "no",
               Instrux->ValidModes.VmxNonRootSeam ? "yes" : "no",
               Instrux->ValidModes.VmxOff ? "yes" : "no"
               );

        printf("        Valid prefixes\n"
               "                REP: %s, REPcc: %s, LOCK: %s\n"
               "                HLE: %s, XACQUIRE only: %s, XRELEASE only: %s\n"
               "                BND: %s, BHINT: %s, DNT: %s\n", 
               Instrux->ValidPrefixes.Rep ? "yes" : "no",
               Instrux->ValidPrefixes.RepCond ? "yes" : "no",
               Instrux->ValidPrefixes.Lock ? "yes" : "no",
               Instrux->ValidPrefixes.Hle ? "yes" : "no",
               Instrux->ValidPrefixes.Xacquire ? "yes" : "no",
               Instrux->ValidPrefixes.Xrelease ? "yes" : "no",
               Instrux->ValidPrefixes.Bnd ? "yes" : "no",
               Instrux->ValidPrefixes.Bhint ? "yes" : "no",
               Instrux->ValidPrefixes.Dnt ? "yes" : "no"
               );

        for (i = 0; i < Instrux->OperandsCount; i++)
        {
            printf("        Operand: %u, Acc:  %s,  Type: %10s, Size: %2d, RawSize: %2d, Encoding: %s", i,
                Instrux->Operands[i].Access.Access == ND_ACCESS_READ ? "R-" :
                Instrux->Operands[i].Access.Access == ND_ACCESS_WRITE ? "-W" :
                Instrux->Operands[i].Access.Access == (ND_ACCESS_READ|ND_ACCESS_WRITE) ? "RW" :
                Instrux->Operands[i].Access.Access == ND_ACCESS_COND_READ ? "CR" :
                Instrux->Operands[i].Access.Access == ND_ACCESS_COND_WRITE ? "CW" :
                Instrux->Operands[i].Access.Access == (ND_ACCESS_COND_READ|ND_ACCESS_COND_WRITE) ? "CRCW" :
                Instrux->Operands[i].Access.Access == (ND_ACCESS_READ | ND_ACCESS_COND_WRITE) ? "RCW" :
                Instrux->Operands[i].Access.Access == (ND_ACCESS_COND_READ|ND_ACCESS_WRITE) ? "CRW" :
                Instrux->Operands[i].Access.Access == ND_ACCESS_PREFETCH ? "P" : "--",
                optype_to_string(Instrux->Operands[i].Type), (int)Instrux->Operands[i].Size,
                (int)Instrux->Operands[i].RawSize, encoding_to_string(Instrux->Operands[i].Encoding)
            );

            if (ND_OP_MEM == Instrux->Operands[i].Type)
            {
                printf(", ");

                if (Instrux->Operands[i].Info.Memory.IsRipRel)
                {
                    printf("RIP relative: yes, ");
                }

                if (Instrux->Operands[i].Info.Memory.IsAG)
                {
                    printf("Address Generator: yes, ");
                }

                if (Instrux->Operands[i].Info.Memory.IsBitbase)
                {
                    printf("Bitbase Addressing: yes, ");
                }

                if (Instrux->Operands[i].Info.Memory.IsMib)
                {
                    printf("MIB Addressing: yes, ");
                }

                if (Instrux->Operands[i].Info.Memory.IsVsib)
                {
                    printf("VSIB Addressing: yes, ");
                }

                if (Instrux->Operands[i].Info.Memory.IsSibMem)
                {
                    printf("Sibmem Addressing: yes, ");
                }

                if (Instrux->Operands[i].Info.Memory.IsStack)
                {
                    printf("Stack: yes, ");
                }

                if (Instrux->Operands[i].Info.Memory.IsString)
                {
                    printf("String: yes, ");
                }

                if (Instrux->Operands[i].Info.Memory.IsShadowStack)
                {
                    printf("Shadow stack: %d, ", Instrux->Operands[i].Info.Memory.ShStkType);
                }

                if (Instrux->Operands[i].Info.Memory.HasCompDisp)
                {
                    printf("Compressed displacement: yes, ");
                }

                if (Instrux->Operands[i].Info.Memory.HasBroadcast)
                {
                    printf("Broadcast: yes, ");
                }

                printf("\n                ");

                if (Instrux->Operands[i].Info.Memory.HasSeg)
                {
                    printf("Segment: %d, ", Instrux->Operands[i].Info.Memory.Seg);
                }

                if (Instrux->Operands[i].Info.Memory.HasBase)
                {
                    printf("Base: %d, ", Instrux->Operands[i].Info.Memory.Base);
                }

                if (Instrux->Operands[i].Info.Memory.HasIndex)
                {
                    printf("Index: %d * %d, ", 
                           Instrux->Operands[i].Info.Memory.Index, Instrux->Operands[i].Info.Memory.Scale);
                }

                if (Instrux->Operands[i].Info.Memory.HasDisp)
                {
                    printf("Displacement: 0x%016llx, ", (unsigned long long)Instrux->Operands[i].Info.Memory.Disp);
                }

                if (Instrux->Operands[i].Info.Memory.IsVsib)
                {
                    printf("\n                ");
                    printf("VSIB index size: %d, VSIB element size: %d, VSIB element count: %d",
                        Instrux->Operands[i].Info.Memory.Vsib.IndexSize,
                        Instrux->Operands[i].Info.Memory.Vsib.ElemSize,
                        Instrux->Operands[i].Info.Memory.Vsib.ElemCount);
                }
            }

            if (ND_OP_REG == Instrux->Operands[i].Type)
            {
                printf(", RegType: %16s, RegSize: %2u, ",
                       regtype_to_string(Instrux->Operands[i].Info.Register.Type),
                       Instrux->Operands[i].Info.Register.Size);
                if (Instrux->Operands[i].Info.Register.Type == ND_REG_MSR)
                {
                    printf("RegId: 0x%08x, RegCount: %u\n",
                        Instrux->Operands[i].Info.Register.Reg,
                        Instrux->Operands[i].Info.Register.Count);
                }
                else
                {
                    printf("RegId: %u, RegCount: %u\n",
                           Instrux->Operands[i].Info.Register.Reg,
                           Instrux->Operands[i].Info.Register.Count);
                }
            }
            else
            {
                printf("\n");
            }

            if (Instrux->Operands[i].Decorator.HasBroadcast)
            {
                printf("                Decorator: Broadcast %d bytes element %d times\n",
                    Instrux->Operands[i].Decorator.Broadcast.Size,
                    Instrux->Operands[i].Decorator.Broadcast.Count);
            }

            if (Instrux->Operands[i].Decorator.HasMask)
            {
                printf("                Decorator: Mask k%d\n", Instrux->Operands[i].Decorator.Mask.Msk);
            }

            if (Instrux->Operands[i].Decorator.HasZero)
            {
                printf("                Decorator: Zero (no merging)\n");
            }
        }

        printf("\n");
    }

    if (Options->BitFields && Instrux->HasModRm)
    {
        printf("        Instruction bit fields:\n");

        if (Instrux->HasEvex)
        {
            printf("            EVEX:   0x%02x 0x%02x 0x%02x 0x%02x > mm: %d, R': %d, B: %d, X: %d, R: %d, pp: %d, "
                "vvvv: %d, W: %d, aaa: %d, V': %d, b: %d, L'L: %d, z: %d\n",
                Instrux->Evex.Evex[0], Instrux->Evex.Evex[1], Instrux->Evex.Evex[2], Instrux->Evex.Evex[3],
                Instrux->Evex.m, Instrux->Evex.rp, Instrux->Evex.b, Instrux->Evex.x, Instrux->Evex.r, Instrux->Evex.p,
                Instrux->Evex.v, Instrux->Evex.w, Instrux->Evex.a, Instrux->Evex.vp, Instrux->Evex.bm, Instrux->Evex.l,
                Instrux->Evex.z);
        }

        if (Instrux->HasVex)
        {
            if (Instrux->VexMode == ND_VEXM_2B)
            {
                printf("            VEX2:   0x%02x 0x%02x > pp: %d, L: %d, vvvv: %d, R: %d\n",
                    Instrux->Vex2.Vex[0], Instrux->Vex2.Vex[1], Instrux->Vex2.p, Instrux->Vex2.l, Instrux->Vex2.v,
                    Instrux->Vex2.r);
            }
            else
            {
                printf("            VEX3:   0x%02x 0x%02x 0x%02x > m-mmmm: %d, B: %d, X: %d, R: %d, pp: %d, L: %d, vvvv: %d, W: %d\n",
                    Instrux->Vex3.Vex[0], Instrux->Vex3.Vex[1], Instrux->Vex3.Vex[2], Instrux->Vex3.m, Instrux->Vex3.b,
                    Instrux->Vex3.x, Instrux->Vex3.r, Instrux->Vex3.p, Instrux->Vex3.l, Instrux->Vex3.v, Instrux->Vex3.w);
            }
        }

        if (Instrux->HasXop)
        {
            printf("            XOP:    0x%02x 0x%02x 0x%02x > m: %d, B: %d, X: %d, R: %d, p: %d, L: %d, v: %d, W: %d\n",
                Instrux->Xop.Xop[0], Instrux->Xop.Xop[1], Instrux->Xop.Xop[2], Instrux->Xop.m, Instrux->Xop.b,
                Instrux->Xop.x, Instrux->Xop.r, Instrux->Xop.p, Instrux->Xop.l, Instrux->Xop.v, Instrux->Xop.w);
        }

        if (Instrux->HasRex)
        {
            printf("            REX:    0x%02x > B: %d, X: %d, R: %d, W: %d\n",
                Instrux->Rex.Rex, Instrux->Rex.b, Instrux->Rex.x, Instrux->Rex.r, Instrux->Rex.w);
        }

        if (Instrux->HasModRm)
        {
            printf("            ModR/M: 0x%02x > mod: %d, reg: %d, rm: %d\n",
                Instrux->ModRm.ModRm, Instrux->ModRm.mod, Instrux->ModRm.reg, Instrux->ModRm.rm);
        }

        if (Instrux->HasSib)
        {
            printf("            SIB:    0x%02x > scale: %d, index: %d, base: %d\n",
                Instrux->Sib.Sib, Instrux->Sib.scale, Instrux->Sib.index, Instrux->Sib.base);
        }
    }
}


void
handle_disasm(
    __in PDISASM_OPTIONS Options
    )
{
    INSTRUX instrux;
    ND_CONTEXT ctx = { 0 };
    unsigned long long icount = 0, istart, iend, start, end, itotal = 0, tilen = 0, ticount = 0;
    SIZE_T rip, fsize = Options->Size;
    PBYTE buffer = Options->Buffer;

    start = clock();

    NdInitContext(&ctx);

    ctx.DefCode = Options->Mode;
    ctx.DefData = Options->Mode;
    ctx.DefStack = Options->Mode;
    ctx.VendMode = Options->Vendor;
    ctx.FeatMode = Options->Feature;

    // Disassemble
    rip = Options->Offset;
    while (rip < Options->Size)
    {
        NDSTATUS status;

        icount++;

#if defined(ND_ARCH_X86) || defined(ND_ARCH_X64)
        istart = __rdtsc();
#else
        istart = 0;
#endif

        status = NdDecodeWithContext(&instrux, buffer + rip, fsize - rip, &ctx);

#if defined(ND_ARCH_X86) || defined(ND_ARCH_X64)
        iend = __rdtsc();
#else
        iend = 1;
#endif

        itotal += iend - istart;
        if (!ND_SUCCESS(status))
        {
            if (Options->Print)
            {
                printf("%p ", (void*)(rip + Options->Rip));

                printf("%02x", buffer[rip]);

                printf("%s", gSpaces[16 - 1]);

                printf("db 0x%02x (0x%08x)\n", buffer[rip], status);
            }

            if (Options->Skip16)
            {
                rip += 16;
            }
            else
            {
                rip++;
            }
        }
        else
        {
            tilen += instrux.Length;
            ticount++;

            if (Options->Print)
            {
                print_instruction(rip + Options->Rip, &instrux, Options);
            }

            if (Options->Skip16)
            {
                rip += 16;
            }
            else
            {
                rip += instrux.Length;
            }
        }
    }

    end = clock();

    if (Options->Stats)
    {
        printf("Disassembled %llu instructions in %llums, %4.4f instructions/second, %4.6f clocks/instruction, average ilen %4.6f bytes\n",
            icount, end - start, icount / (double)(end - start) * 1000, itotal / (double)icount, tilen / (double)ticount);
    }
}


void
handle_shemu(
    PDISASM_OPTIONS Options
    )
{
    SHEMU_CONTEXT ctx;
    SHEMU_STATUS shstatus;
    char *fileName;
    HANDLE hFile;
    DWORD outSize;
    SIZE_T rip = 0, fsize = Options->Size, offset = 0, decFileNameLength = 0, shellSize;
    char *fNameDecoded;
    PBYTE buffer = Options->Buffer;

    memset(&ctx, 0, sizeof(ctx));
    rip = Options->Rip;
    fileName = Options->FileName;

    if (fileName == NULL)
    {
        decFileNameLength = sizeof("hex_string_decoded.bin");
        fNameDecoded = (char *)malloc(sizeof(char) * decFileNameLength);
    }
    else
    {
        decFileNameLength = strlen(fileName) + sizeof("_decoded.bin");
        fNameDecoded = (char *)malloc(sizeof(char) * decFileNameLength);
    }

    if (NULL == fNameDecoded)
    {
        printf("Could not allocate file name.\n");
    }
    else
    {
        if (fileName == NULL)
        {
            sprintf_s(fNameDecoded, decFileNameLength, "hex_string_decoded.bin");
        }
        else
        {
            sprintf_s(fNameDecoded, decFileNameLength, "%s_decoded.bin", fileName);
        }
    }
   
    if (Options->Offset < PAGE_SIZE)
    {
        offset = Options->Offset;
    }

    // A little extra space, since shellcodes tend to do accesses after their code...
    shellSize = fsize + 0x100;

    // Allocate the shellcode, stack, shell bitmap and stack bitmaps.
    ctx.Shellcode = (uint8_t *)malloc(shellSize);
    if (NULL == ctx.Shellcode)
    {
        printf("Memory error: couldn't allocated %zu bytes!\n", fsize);
        goto cleanup_and_exit;
    }

#define STACK_SIZE 0x2000

    ctx.Stack = (uint8_t *)malloc(STACK_SIZE);
    if (NULL == ctx.Stack)
    {
        printf("Memory error: couldn't allocated %zu bytes!\n", fsize);
        goto cleanup_and_exit;
    }

    ctx.Intbuf = (uint8_t *)malloc(shellSize + STACK_SIZE);
    if (NULL == ctx.Intbuf)
    {
        printf("Memory error: couldn't allocated %zu bytes!\n", fsize);
        goto cleanup_and_exit;
    }

    memset(ctx.Shellcode, 0, shellSize);
    memset(ctx.Stack, 0, STACK_SIZE);
    memcpy((BYTE *)ctx.Shellcode, (BYTE *)buffer, fsize);
    memset(ctx.Intbuf, 0, shellSize + STACK_SIZE);

    // Use the provided RIP, if any. Otherwise, use a hard-coded value.
    ctx.ShellcodeBase = (rip != 0 ? rip & PAGE_MASK : 0x200000);
    ctx.ShellcodeSize = (DWORD)shellSize;
    ctx.StackBase = (ctx.ShellcodeBase & PAGE_MASK) - STACK_SIZE - 0x1000;
    ctx.StackSize = STACK_SIZE;
    
    ctx.IntbufSize = (DWORD)shellSize + STACK_SIZE;

    ctx.Mode = Options->Mode;
    ctx.Ring = Options->Ring;

    ctx.Registers.RegFlags = NDR_RFLAG_IF | 2;
    ctx.Registers.RegRip = ctx.ShellcodeBase + offset;
    ctx.Registers.RegRsp = ctx.StackBase + STACK_SIZE / 2;

    if (ctx.Mode == ND_CODE_64)
    {
        ctx.Segments.Cs.Selector = (ctx.Ring == 3) ? 0x33 : 0x10;
        ctx.Segments.Ds.Selector = (ctx.Ring == 3) ? 0x2b : 0x18;
        ctx.Segments.Es.Selector = (ctx.Ring == 3) ? 0x2b : 0x18;
        ctx.Segments.Ss.Selector = (ctx.Ring == 3) ? 0x2b : 0x18;
        ctx.Segments.Fs.Selector = (ctx.Ring == 3) ? 0x2b : 0x00;
        ctx.Segments.Gs.Selector = (ctx.Ring == 3) ? 0x53 : 0x00;

        ctx.Segments.Fs.Base = 0;
        ctx.Segments.Gs.Base = 0x7FFF0000;
    }
    else
    {
        ctx.Segments.Cs.Selector = (ctx.Ring == 3) ? 0x1b : 0x08;
        ctx.Segments.Ds.Selector = (ctx.Ring == 3) ? 0x23 : 0x10;
        ctx.Segments.Es.Selector = (ctx.Ring == 3) ? 0x23 : 0x10;
        ctx.Segments.Ss.Selector = (ctx.Ring == 3) ? 0x23 : 0x10;
        ctx.Segments.Fs.Selector = (ctx.Ring == 3) ? 0x3b : 0x30;
        ctx.Segments.Gs.Selector = (ctx.Ring == 3) ? 0x23 : 0x00;

        ctx.Segments.Fs.Base = 0x7FFF0000;
        ctx.Segments.Gs.Base = 0;
    }

    // Dummy values, to resemble regular CR0/CR4 values.
    ctx.Registers.RegCr0 = 0x0000000080050031;
    ctx.Registers.RegCr4 = 0x0000000000170678;

    if (Options->UseShemuRegs)
    {
        // Copy the new GPRs
        memcpy(&ctx.Registers.RegRax, Options->ShemuRegs, sizeof(Options->ShemuRegs));

        // Update the stack to point to the new RSP, if one exists
        if (ctx.Registers.RegRsp != 0)
        {
            // Consider the stack base at least with a page before the current RSP. In case of pushes or operations
            // which decrease the RSP, we will always give SHEMU_ABORT_BRANCH_OUTSIDE otherwise.
            ctx.StackBase = ctx.Registers.RegRsp - 0x1000;
        }
    }


    ctx.TibBase = 0x7FFF0000;
    ctx.MaxInstructionsCount = 4096;
    ctx.Flags = 0;
    ctx.Options = SHEMU_OPT_TRACE_EMULATION;
    ctx.Log = &ShemuLog;
    ctx.AccessMemory = (ShemuMemAccess)&ShemuAccessMem;

    // Configurable thresholds.
    ctx.NopThreshold = SHEMU_DEFAULT_NOP_THRESHOLD;
    ctx.StrThreshold = SHEMU_DEFAULT_STR_THRESHOLD;
    ctx.MemThreshold = SHEMU_DEFAULT_MEM_THRESHOLD;

    // Check for AES support.
    int regs[4] = { 0 };

#if defined(ND_ARCH_X86) || defined(ND_ARCH_X64)
    __cpuid(regs, 1);
#endif

    // CPUID leaf function 1, register ECX, bit 25 indicates AES-NI support.
    if (!!(regs[2] & (1UL << 25)))
    {
        ctx.Options |= SHEMU_OPT_SUPPORT_AES;
    }

    if (Options->BypassSelfWrites)
    {
        ctx.Options |= SHEMU_OPT_BYPASS_SELF_WRITES;
    }

    shstatus = ShemuEmulate(&ctx);

    printf("Emulation terminated with status 0x%08x, flags: 0x%llx, %u NOPs, %u total instructions\n", 
           shstatus, (unsigned long long)ctx.Flags, ctx.NopCount, ctx.InstructionsCount);
    if (ctx.Flags & SHEMU_FLAG_NOP_SLED)
    {
        printf("        SHEMU_FLAG_NOP_SLED\n");
    }
    if (ctx.Flags & SHEMU_FLAG_LOAD_RIP)
    {
        printf("        SHEMU_FLAG_LOAD_RIP\n");
    }
    if (ctx.Flags & SHEMU_FLAG_WRITE_SELF)
    {
        printf("        SHEMU_FLAG_WRITE_SELF\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SYSCALL)
    {
        printf("        SHEMU_FLAG_SYSCALL\n");
    }
    if (ctx.Flags & SHEMU_FLAG_STACK_STR)
    {
        printf("        SHEMU_FLAG_STACK_STR\n");
    }
    if (ctx.Flags & SHEMU_FLAG_TIB_ACCESS_WOW32)
    {
        printf("        SHEMU_FLAG_TIB_ACCESS_WOW32\n");
    }
    if (ctx.Flags & SHEMU_FLAG_HEAVENS_GATE)
    {
        printf("        SHEMU_FLAG_HEAVENS_GATE\n");
    }
    if (ctx.Flags & SHEMU_FLAG_STACK_PIVOT)
    {
        printf("        SHEMU_FLAG_STACK_PIVOT\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SUD_ACCESS)
    {
        printf("        SHEMU_FLAG_SUD_ACCESS\n");
    }
    if (ctx.Flags & SHEMU_FLAG_KPCR_ACCESS)
    {
        printf("        SHEMU_FLAG_KPCR_ACCESS\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SWAPGS)
    {
        printf("        SHEMU_FLAG_SWAPGS\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SYSCALL_MSR_READ)
    {
        printf("        SHEMU_FLAG_SYSCALL_MSR_READ\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SYSCALL_MSR_WRITE)
    {
        printf("        SHEMU_FLAG_SYSCALL_MSR_WRITE\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SIDT)
    {
        printf("        SHEMU_FLAG_SIDT\n");
    }

    if (fNameDecoded != NULL)
    {
        // If a decoded file name is present, dump the code, as it look after emulation, on disk.
        // If the shellcode decrypted itself, the decoded file will contain the plain-text version.
        hFile = CreateFileA(fNameDecoded, GENERIC_WRITE, FILE_SHARE_READ, NULL, 
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hFile)
        {
            printf("Could not open the file %s : 0x%08x\n", fNameDecoded, GetLastError());
            goto cleanup_and_exit;
        }

        WriteFile(hFile, (BYTE *)ctx.Shellcode, (DWORD)fsize, &outSize, NULL);
        if (outSize == 0)
        {
            printf("No bytes written to %s!\n", fNameDecoded);
        }

        CloseHandle(hFile);
    }

cleanup_and_exit:
    if (NULL != fNameDecoded)
    {
        free(fNameDecoded);
    }

    if (NULL != ctx.Shellcode)
    {
        free(ctx.Shellcode);
    }

    if (NULL != ctx.Stack)
    {
        free(ctx.Stack);
    }

    if (NULL != ctx.Intbuf)
    {
        free(ctx.Intbuf);
    }
}


void print_help()
{
    uint32_t major, minor, revision;
    char *date, *time;

    NdGetVersion(&major, &minor, &revision, &date, &time);

    printf("bddisasm version %u.%u.%u, built on %s %s\n", major, minor, revision, date, time);
    printf("\n");
    printf("IMPORTANT:\n");
    printf("    This tool is only meant to exemplify bddisasm integration.\n");
    printf("\n");
    printf("USAGE:\n");
    printf("    disasm COMMAND INPUT MODE [OPTIONS]\n");
    printf("\n");

    printf("COMMAND can be one of:\n");
    printf("    decode      - Will decode the input, and print each instruction (default).\n");
    printf("    shemu       - Will run the shellcode-emulator on the input, and print the emulation trace.\n");
    printf("\n");

    printf("INPUT is mandatory and can be one of:\n");
    printf("    -f file     - Specify input `file` name.\n");
    printf("    -h hex      - Specify input `hex` string. Accepted formats are:\n");
    printf("        Plain   hex string; example: 33C090CCC3\n");
    printf("        Escaped hex string; example: \\x33\\xC0\\x90\\xCC\\xC3\n");

    printf("\n");

    printf("MODE sets the decode mode:\n");
    printf("    -b16        - Decode in 16-bit mode.\n");
    printf("    -b32        - Decode in 32-bit mode.\n");
    printf("    -b64        - Decode in 64-bit mode (default).\n");
    printf("\n");

    printf("OPTIONS which are common among different modes:\n");
    printf("    -o offset   - Start processing from the indicated `offset` (default is 0).\n");
    printf("    -r rip      - Use the indicated `rip` for disassembly (default is 0).\n");
    printf("    -v vendor   - Set prefered vendor (default is any). The following are valid `vendor` values:\n");
    printf("       intel, amd, cyrix, mpx, any\n");
    printf("    -t feature  - Set prefered feature mode (default is all). The following are valid `feature` values (multiple can be used):\n");
    printf("       none, all, mpx, cet, cldm, piti\n");
    printf("\n");

    printf("OPTIONS valid only with decode command:\n");
    printf("    -hl         - Highlight instruction parts. The colors used are:\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
        FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
    printf("        light white      prefixes\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    printf("        light green      opcodes\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
    printf("        light yellow     modrm and sib\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE|FOREGROUND_INTENSITY);
    printf("        light blue       displacement\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_INTENSITY);
    printf("        light red        relative offset, immediate, address\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
    printf("    -nv         - Don't print disassembly. Use this only for performance tests.\n");
    printf("    -iv         - Print performance statistics.\n");
    printf("    -exi        - Print extended info about instructions.\n");
    printf("    -bits       - Print the instruction bit fields.\n");
    printf("    -skip16     - Skip 16 bytes after each decoded instruction. Useful when decoding invalid instructions.\n");
    printf("\n");
    printf("OPTIONS valid only with shemu command:\n");
    printf("    -reg val    - Set register `reg` to value `val` for emulation. `reg` must be the plain 64-bit register name (ie: rax).\n");
    printf("    -k          - Specify kernel mode for shemu emulation (default is user-mode).\n");
    printf("    -bw         - Bypass self-modifications in shemu.\n");
    printf("\n");
    printf("\n");
    printf("EXAMPLES:\n");
    printf("    Decode 64-bit code from file test.bin:\n");
    printf("        disasm -f test.bin\n");
    printf("        disasm -b64 -f test.bin\n");
    printf("        disasm decode -b64 -f test.bin\n");
    printf("    Decode 64-bit from a hex-buffer, and display extended instruction information:\n");
    printf("        disasm -b64 -h 909033C0 -exi\n");
    printf("    Decode from hex-string, highlight instruction components & display instruction bitfields:\n");
    printf("        disasm -h 90505833C0E80000000058CC -hl -bits\n");
    printf("    Emulate a potential 32-bit shellcode from test file shell.bin:\n");
    printf("        disasm shemu -b32 -f shell.bin\n");
    printf("    Emulate a potential 32-bit shellcode from test file shell.bin, and specify some input registers:\n");
    printf("        disasm shemu -b32 -f shell.bin -rax 0x100 -rcx 0xABCD -rsp 0x1000\n");
    printf("    Run a quick benchmark on file test.bin:\n");
    printf("        disasm -f test.bin -nv -iv\n");
    printf("\n");
}


void
cleanup_context(
    __inout DISASM_OPTIONS *Options
    )
{
    if (Options->InputMode == inputFile)
    {
        if (NULL != Options->Buffer)
        {
            UnmapViewOfFile(Options->Buffer);
        }

        if (NULL != Options->HandleMapping && INVALID_HANDLE_VALUE != Options->HandleMapping)
        {
            CloseHandle(Options->HandleMapping);
        }

        if (NULL != Options->HandleFile && INVALID_HANDLE_VALUE != Options->HandleFile)
        {
            CloseHandle(Options->HandleFile);
        }
    }
}


_Success_(return)
BOOLEAN
parse_input(
    __inout DISASM_OPTIONS* Options
    )
{
    static BYTE hexbuf[4096];

    if (inputNone == Options->InputMode)
    {
        printf("Expecting an input mode: either -f or -h!\n");
        return FALSE;
    }

    if (inputFile == Options->InputMode)
    {
        // Open the file.
        Options->HandleFile = CreateFileA(Options->FileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, 
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == Options->HandleFile)
        {
            printf("Couldn't open file '%s': 0x%08x\n", Options->FileName, GetLastError());
            cleanup_context(Options);
            return FALSE;
        }

        // Create a file mapping.
        Options->HandleMapping = CreateFileMappingA(Options->HandleFile, NULL, PAGE_READWRITE, 0, 0, "DisasmFile");
        if (NULL == Options->HandleMapping)
        {
            printf("Couldn't create file mapping for '%s': 0x%08x\n", Options->FileName, GetLastError());
            cleanup_context(Options);
            return FALSE;
        }

        // Map the file.
        Options->Buffer = (BYTE *)MapViewOfFile(Options->HandleMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (NULL == Options->Buffer)
        {
            printf("Couldn't map the view for '%s': 0x%08x\n", Options->FileName, GetLastError());
            cleanup_context(Options);
            return FALSE;
        }

        Options->Size = GetFileSize(Options->HandleFile, NULL);
    }
    else
    {
        DWORD idx, sx = 0, mx, of;

        Options->Size = (DWORD)strlen(Options->FileName);

        if (Options->Size < 2)
        {
            printf("Min 1-byte buffer needed!\n");
            return FALSE;
        }
        
        // Since we expect a hex string, the buffer must be even-sized.
        if (Options->Size % 2 == 1)
        {
            printf("Even-sized hex buffer expected!\n");
            return FALSE;
        }

        // If the buffer starts with \x, assume it's escaped format. Note that we only check
        // the first two characters for \x - afterwards, we will only look after the hex-digits
        // at offsets 2 & 3 inside each 4-characters chunk.
        if (Options->FileName[0] == '\\' && Options->FileName[1] == 'x')
        {
            sx = 1;
        }

        // If escaped format is used, buffer must be at least 4 characters long (1 byte).
        if (sx && Options->Size < 4)
        {
            printf("Min 1-byte buffer needed!\n");
            return FALSE;
        }

        if (sx)
        {
            mx = 4;
            of = 2;
        }
        else
        {
            mx = 2;
            of = 0;
        }

        // Check for maximum size.
        if (Options->Size / mx > sizeof(hexbuf))
        {
            printf("Max %zu bytes buffer accepted!\n", sizeof(hexbuf));
            return FALSE;
        }

        // Extract each byte from the provided hex input.
        for (idx = 0; idx < Options->Size / mx; idx++)
        {
            hexbuf[idx] = ((hex_to_bin(Options->FileName[idx * mx + of]) << 4) | 
                           (hex_to_bin(Options->FileName[idx * mx + of + 1]))) & 0xFF;
        }

        Options->FileName = NULL;
        Options->Size /= sx ? 4 : 2;
        Options->Buffer = hexbuf;
    }

    // Make sure the offset is valid & points within the buffer.
    if (Options->Offset >= Options->Size)
    {
        printf("The offset exceeds the buffer size!\n");
        cleanup_context(Options);
        return FALSE;
    }

    return TRUE;
}


_Success_(return)
BOOLEAN
parse_arguments(
    __in int argc,
    __in char* argv[],
    __out DISASM_OPTIONS *Options
    )
{
    int i;

    if (argc < 2 || NULL == argv)
    {
        print_help();
        return FALSE;
    }

    memset(Options, 0, sizeof(*Options));

    // Initialize default options.
    Options->Command = commandDecode;
    Options->Mode = ND_CODE_64;
    Options->Ring = 3;
    Options->Print = TRUE;
    Options->Vendor = ND_VEND_ANY;
    Options->Feature = ND_FEAT_ALL;

    i = 1;
    while (i < argc)
    {       
        DWORD gprIdx;

        if (match_gpr(argv[i], &gprIdx))
        {
            // Register value (used by shemu).
            if (i + 1 >= argc)
            {
                printf("No value given for %s!\n", argv[i]);
            }
            else
            {
                Options->ShemuRegs[gprIdx] = (size_t)strtoull(argv[i + 1], NULL, 0);
                Options->UseShemuRegs = TRUE;
                i++;
            }
        }
        else if (strcmp(argv[i], "shemu") == 0)
        {
            // shemu command - will emulate.
            Options->Command = commandShemu;
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'f' && argv[i][2] == 0)
        {
            // File input mode.
            if (i + 1 < argc)
            {
                Options->InputMode = inputFile;
                Options->FileName = argv[i + 1];
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'h' && argv[i][2] == 0)
        {
            // Hex-string input mode.
            if (i + 1 < argc)
            {
                Options->InputMode = inputHex;
                Options->FileName = argv[i + 1];
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'o' && argv[i][2] == 0)
        {
            // Offset inside the provided buffer.
            if (i + 1 < argc)
            {
                sscanf_s(argv[i + 1], "%zx", &Options->Offset);
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'r' && argv[i][2] == 0)
        {
            // Rip. Can be any value, as it's used only for disassembly.
            if (i + 1 < argc)
            {
                sscanf_s(argv[i + 1], "%zx", &Options->Rip);
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'k' && argv[i][2] == 0)
        {
            // Kernel-mode for shemu.
            Options->Ring = 0;
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'b' && argv[i][2] == 'w' && argv[i][3] == 0)
        {
            // Bypass self writes in shemu.
            Options->BypassSelfWrites = TRUE;
        }
        else if (0 == strcmp(argv[i], "-b16"))
        {
            // 16-bit mode.
            Options->Mode = ND_CODE_16;
        } 
        else if (0 == strcmp(argv[i], "-b32"))
        {
            // 32-bit mode.
            Options->Mode = ND_CODE_32;
        } 
        else if (0 == strcmp(argv[i], "-b64"))
        {
            // 64-bit mode.
            Options->Mode = ND_CODE_64;
        }
        else if (0 == strcmp(argv[i], "-v intel"))
        {
            // Prefer Intel instructions.
            Options->Vendor = ND_VEND_INTEL;
        }
        else if (0 == strcmp(argv[i], "-v amd"))
        {
            // Prefer AMD instructions.
            Options->Vendor = ND_VEND_AMD;
        }
        else if (0 == strcmp(argv[i], "-v geode"))
        {
            // Prefer Geode instructions.
            Options->Vendor = ND_VEND_GEODE;
        }
        else if (0 == strcmp(argv[i], "-v cyrix"))
        {
            // Prefer Cyrix instructions.
            Options->Vendor = ND_VEND_CYRIX;
        }
        else if (0 == strcmp(argv[i], "-v any"))
        {
            // Try to decode everything.
            Options->Vendor = ND_VEND_ANY;
        }
        else if (0 == strcmp(argv[i], "-t all"))
        {
            // Enable all features.
            Options->Feature = ND_FEAT_ALL;
        }
        else if (0 == strcmp(argv[i], "-t mpx"))
        {
            // Enable MPX. 
            if (Options->Feature == ND_FEAT_ALL)
            {
                Options->Feature = 0;
            }

            Options->Feature |= ND_FEAT_MPX;
        }
        else if (0 == strcmp(argv[i], "-t cet"))
        {
            // Enable CET.
            if (Options->Feature == ND_FEAT_ALL)
            {
                Options->Feature = 0;
            }

            Options->Feature |= ND_FEAT_CET;
        }
        else if (0 == strcmp(argv[i], "-t cldm"))
        {
            // Enable Cache Line Demote.
            if (Options->Feature == ND_FEAT_ALL)
            {
                Options->Feature = 0;
            }

            Options->Feature |= ND_FEAT_CLDEMOTE;
        }
        else if (0 == strcmp(argv[i], "-t piti"))
        {
            // Enable Prefetch for instruction fetch.
            if (Options->Feature == ND_FEAT_ALL)
            {
                Options->Feature = 0;
            }

            Options->Feature |= ND_FEAT_PITI;
        }
        else if (0 == strcmp(argv[i], "-t none"))
        {
            // No feature support.
            Options->Feature = ND_FEAT_NONE;
        }
        else if (0 == strcmp(argv[i], "-nv"))
        {
            // Do not print anything.
            Options->Print = FALSE;
        } 
        else if (0 == strcmp(argv[i], "-hl"))
        {
            // Highlight instruction components.
            Options->Highlight = TRUE;
        }
        else if (0 == strcmp(argv[i], "-iv"))
        {
            // Print statistics.
            Options->Stats = TRUE;
        }
        else if (0 == strcmp(argv[i], "-exi"))
        {
            // Print extended instruction information.
            Options->ExtendedInfo = TRUE;
        }
        else if (0 == strcmp(argv[i], "-bits"))
        {
            // Print instruction bitfields.
            Options->BitFields = TRUE;
        }
        else if (0 == strcmp(argv[i], "-skip16"))
        {
            // Skip 16 bytes after each decoded instruction.
            Options->Skip16 = TRUE;
        }
        else
        {
            printf("Unknown option: '%s'\n", argv[i]);
        }

        i++;
    }

    // Parse the input.
    if (!parse_input(Options))
    {
        printf("Could not find a valid input!\n");
        return FALSE;
    }

    return TRUE;
}


int 
main(
    __in int argc, 
    __in char* argv[]
    )
{
    DISASM_OPTIONS options = { 0 };

    // Parse arguments & extract relevant options.
    if (!parse_arguments(argc, argv, &options))
    {
        return -1;
    }

    // Handle the indicated command.
    if (options.Command == commandShemu)
    {
        handle_shemu(&options);
    }
    else
    {
        handle_disasm(&options);
    }

    // Will free any memory allocated during argument parsing, and will close any handles.
    cleanup_context(&options);

    return 0;
}
