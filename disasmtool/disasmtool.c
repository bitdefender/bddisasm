/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef uint64_t    QWORD, *PQWORD;

// Main disasm header file.
#include "bdshemu\bdshemu.h"
#include "bddisasm.h"


#define PAGE_SIZE 0x1000
#define PAGE_MASK 0xFFFFFFFFFFFFF000

typedef struct _DISASM_OPTIONS
{
    uint8_t     *Buffer;        // The buffer containing the instructions.
    size_t      Size;           // Buffer size.
    size_t      Offset;         // Offset inside the buffer.
    size_t      Rip;            // Virtual RIP.
    char        *Target;        // If in search mode, this indicates the instruction to be searched for.
    BOOLEAN     Highlight;      // Highlight instruction components, if true.
    BOOLEAN     ExtendedInfo;   // Display extended instruction info, if true.
    BOOLEAN     Stats;          // Display disassembly stats (clocks / instruction, instructions / second), if true.
    BOOLEAN     Search;         // Search for the Target instruction in the provided buffer.
    BOOLEAN     Print;          // Print instruction disassembly, if true.
    uint8_t     Mode;           // Mode - 16, 32 or 64 bit mode.
    uint8_t     Ring;           // Ring - 0, 1, 2 or 3.
    uint8_t     Vendor;         // Preffered vendor.
    uint8_t     Feature;        // Used features.
    char        *FileName;      // Input file, if any.
    size_t      ShemuRegs[NDR_R15 + 1];
    BOOLEAN     UseShemuRegs;
} DISASM_OPTIONS, *PDISASM_OPTIONS;

char *gSpaces[16] = 
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


//
// nd_vsnprintf
//
int nd_vsnprintf_s(
    char *buffer,
    size_t sizeOfBuffer,
    size_t count,
    const char *format,
    va_list argptr
    )
{
    return _vsnprintf_s(buffer, sizeOfBuffer, count, format, argptr);
}

void* nd_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}


//
// set_to_string
//
const char* set_to_string(
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
    case ND_SET_AMXINT8:               return "AMX-INT8";
    case ND_SET_AMXTILE:               return "AMX-TILE";
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
    case ND_SET_AVXVNNI:               return "AVXVNNI";
    case ND_SET_BMI1:                  return "BMI1";
    case ND_SET_BMI2:                  return "BMI2";
    case ND_SET_CET_SS:                return "CET_SS";
    case ND_SET_CET_IBT:               return "CET_IBT";
    case ND_SET_CLDEMOTE:              return "CLDEMOTE";
    case ND_SET_CLFSH:                 return "CLFSH";
    case ND_SET_CLFSHOPT:              return "CLFSHOPT";
    case ND_SET_CLWB:                  return "CLWB";
    case ND_SET_CLZERO:                return "CLZERO";
    case ND_SET_CMPXCHG16B:            return "CMPXCHG16B";
    case ND_SET_CYRIX:                 return "CYRIX";
    case ND_SET_CYRIX_SMM:             return "CYRIX_SMM";
    case ND_SET_ENQCMD:                return "ENQCMD";
    case ND_SET_F16C:                  return "F16C";
    case ND_SET_FMA:                   return "FMA";
    case ND_SET_FMA4:                  return "FMA4";
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
    case ND_SET_LONGMODE:              return "LONGMODE";
    case ND_SET_LWP:                   return "LWP";
    case ND_SET_LZCNT:                 return "LZCNT";
    case ND_SET_MCOMMIT:               return "MCOMMIT";
    case ND_SET_MMX:                   return "MMX";
    case ND_SET_MOVBE:                 return "MOVBE";
    case ND_SET_MOVDIR64B:             return "MOVDIR64B";
    case ND_SET_MOVDIRI:               return "MOVDIRI";
    case ND_SET_MPX:                   return "MPX";
    case ND_SET_MWAITT:                return "MWAITT";
    case ND_SET_PAUSE:                 return "PAUSE";
    case ND_SET_PCLMULQDQ:             return "PCLMULQDQ";
    case ND_SET_PCOMMIT:               return "PCOMMIT";
    case ND_SET_PCONFIG:               return "PCONFIG";
    case ND_SET_PENTIUMREAL:           return "PENTIUMREAL";
    case ND_SET_PKU:                   return "PKU";
    case ND_SET_POPCNT:                return "POPCNT";
    case ND_SET_PPRO:                  return "PPRO";
    case ND_SET_PREFETCH_NOP:          return "PREFETCH_NOP";
    case ND_SET_PTWRITE:               return "PTWRITE";
    case ND_SET_RDPID:                 return "RDPID";
    case ND_SET_RDPMC:                 return "RDPMC";
    case ND_SET_RDPRU:                 return "RDPRU";
    case ND_SET_RDRAND:                return "RDRAND";
    case ND_SET_RDSEED:                return "RDSEED";
    case ND_SET_RDTSCP:                return "RDTSCP";
    case ND_SET_RDWRFSGS:              return "RDWRFSGS";
    case ND_SET_SCC:                   return "SCC";
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
    case ND_SET_X87:                   return "X87";
    case ND_SET_XOP:                   return "XOP";
    case ND_SET_XSAVE:                 return "XSAVE";
    case ND_SET_XSAVEC:                return "XSAVEC";
    case ND_SET_XSAVES:                return "XSAVES";
    default: return "???";
    }
}


//
// category_to_string
//
const char* category_to_string(
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
    case ND_CAT_AVXVNNI:              return "AVXVNNI";
    case ND_CAT_BITBYTE:              return "BITBYTE";
    case ND_CAT_BLEND:                return "BLEND";
    case ND_CAT_BMI1:                 return "BMI1";
    case ND_CAT_BMI2:                 return "BMI2";
    case ND_CAT_BROADCAST:            return "BROADCAST";
    case ND_CAT_CALL:                 return "CALL";
    case ND_CAT_CET:                  return "CET";
    case ND_CAT_CLDEMOTE:             return "CLDEMOTE";
    case ND_CAT_CMOV:                 return "CMOV";
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


//
// optype_to_string
//
const char* optype_to_string(
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


//
// regtype_to_string
//
const char* regtype_to_string(
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


//
// encoding_to_string
//
const char* encoding_to_string(
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


//
// tuple_to_string
//
const char *tuple_to_string(
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


//
// exception_to_string
//
const char *exception_evex_to_string(
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


//
// hex_to_bin
//
BYTE hex_to_bin(
    __in char HexByte
    )
{
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


//
// str_strip
// Copies the InputString in OutputString, while eliminating the characters specified in Tokens
//
VOID str_strip(
    __in char* InString,
    __in DWORD LenInString,
    __in char* Tokens,
    __in DWORD LenTokString,
    __out char* OutString,
    __in DWORD CapOutString // capacity of out string including null character
)
{
    DWORD lenInStr, lenOutStr, lenTokStr;
    DWORD itInStr, itTokStr;
    lenTokStr = 0;
    lenOutStr = 0;

    if ((NULL == InString) || (NULL == Tokens) || (NULL == OutString))
    {
        return;
    }
    
    lenInStr = (DWORD)strnlen_s(InString, ND_MIN_BUF_SIZE);
    lenTokStr = (DWORD)strnlen_s(Tokens, ND_MIN_BUF_SIZE);

    lenInStr = (lenInStr > LenInString) ? LenInString : lenInStr;
    lenTokStr = (lenTokStr > LenTokString) ? LenTokString : lenTokStr;
    

    if ((0 == lenInStr) || (0 == lenTokStr))
    {
        return;
    }

    for (itInStr = 0; itInStr < lenInStr; itInStr++)
    {
        BOOLEAN found = FALSE;
        for (itTokStr = 0; itTokStr < lenTokStr; itTokStr++)
        {
            if (InString[itInStr] == Tokens[itTokStr])
            {
                found = TRUE;
                break;
            }
        }

        if (!found)
        {
            // not enough space, bail out
            if (lenOutStr >= CapOutString - 1)
            {
                return;
            }

            OutString[lenOutStr] = InString[itInStr];
            lenOutStr++;
        }
    }
}


//
// regstr_to_idx
//
INT32 regstr_to_idx(
    __in const char* Reg
    )
{
    static const char* reg64[] =
    {
        "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
        "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15"
    };

    for (INT32 i = 0; i < ARRAYSIZE(reg64); i++)
    {
        if (!_stricmp(Reg, reg64[i]))
        {
            return i;
        }
    }

    return -1;
}


//
// match_gpr
//
BOOLEAN
match_gpr(
    __in const char* Arg,
    __in DWORD* Index
    )
{
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

//
// print_instruction
//
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

    printf("%s", gSpaces[16 - Instrux->Length]);

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
                tuple_to_string(Instrux->TupleType));
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
                printf("exception type: %s\n", exception_evex_to_string(Instrux->ExceptionType));
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
}


//
// handle_search
//
void
handle_search(
    __in PDISASM_OPTIONS Options
    )
{
    NDSTATUS status;
    INSTRUX instrux;
    ND_CONTEXT ctx;
    SIZE_T rip = 0, i;
    char text[ND_MIN_BUF_SIZE], target[ND_MIN_BUF_SIZE];
    char *token1, *token2, *ctx1, *ctx2;
    BOOLEAN match;

    NdInitContext(&ctx);

    ctx.DefCode = Options->Mode;
    ctx.DefData = Options->Mode;
    ctx.DefStack = Options->Mode;
    ctx.VendMode = Options->Vendor;
    ctx.FeatMode = Options->Feature;

    // Disassemble
    rip = 0;
    while (rip < Options->Size)
    {
        status = NdDecodeWithContext(&instrux, Options->Buffer + rip, Options->Size - rip, &ctx);
        if (!ND_SUCCESS(status))
        {
            goto _continue;
        }

        NdToText(&instrux, rip, ND_MIN_BUF_SIZE, text);

        // Copy the target.
        memcpy(target, Options->Target, ND_MIN_BUF_SIZE);
        for (i = 0; i < ND_MIN_BUF_SIZE; i++)
        {
            if (target[i] == ',')
            {
                target[i] = ' ';
            }

            if (text[i] == ',')
            {
                text[i] = ' ';
            }
        }

        _strlwr_s(target, ND_MIN_BUF_SIZE);
        _strlwr_s(text, ND_MIN_BUF_SIZE);

        // Try to match.
        token1 = strtok_s(target, " ", &ctx1);
        token2 = strtok_s(text, " ", &ctx2);

        match = TRUE;

        while (token1 && token2)
        {
            if (0 == strcmp(token1, "*"))
            {
                goto _continue_match;
            }

            if (strlen(token1) != strlen(token2))
            {
                match = FALSE;
                break;
            }

            for (i = 0; i < strlen(token1); i++)
            {
                if ((token1[i] != '?') && (token1[i] != token2[i]))
                {
                    match = FALSE;
                    break;
                }
            }

            if (!match)
            {
                break;
            }

_continue_match:
            token1 = strtok_s(NULL, " ", &ctx1);
            token2 = strtok_s(NULL, " ", &ctx2);
        }

        // Unfinished token - leave.
        if (token1 || token2)
        {
            goto _continue;
        }

        if (match)
        {
            SIZE_T rip2 = rip, count = 0;

            print_instruction(rip, &instrux, Options);
            rip2 += instrux.Length;

            instrux.DefCode = Options->Mode;
            instrux.DefData = Options->Mode;
            instrux.DefStack = Options->Mode;
            instrux.VendMode = Options->Vendor;
            instrux.FeatMode = Options->Feature;

            while (rip2 < Options->Size && count++ < 8)
            {
                status = NdDecodeWithContext(&instrux, Options->Buffer + rip2, Options->Size - rip2, &ctx);
                if (!ND_SUCCESS(status))
                {
                    printf("%p ERROR\n", (void*)rip2);
                    rip2++;
                }
                else
                {
                    print_instruction(rip2, &instrux, Options);
                    rip2 += instrux.Length;
                }
            }

            printf("-----------------------------------------------------------------------\n");
        }

_continue:
        rip++;
    }
}



//
// handle_disasm
//
void
handle_disasm(
    __in PDISASM_OPTIONS Options
    )
{
    INSTRUX instrux;
    ND_CONTEXT ctx = { 0 };
    unsigned long long icount = 0, istart, iend, start, end, itotal = 0;
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

        istart = __rdtsc();
        status = NdDecodeWithContext(&instrux, buffer + rip, fsize - rip, &ctx);
        iend = __rdtsc();

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

            rip++;
        }
        else
        {
            if (Options->Print)
            {
                print_instruction(rip + Options->Rip, &instrux, Options);
            }

            rip += instrux.Length;
        }
    }

    end = clock();

    if (Options->Stats)
    {
        printf("Disassembled %llu instructions in %llums, %4.4f instructions/second, %4.6f clocks/instruction\n",
            icount, end - start, icount / (double)(end - start) * 1000, itotal / (double)icount);
    }
}


//
// set_shemuctx_file
//
void set_shemuctx_file(
    __in const char *FileName,
    __in DISASM_OPTIONS *Options
    )
{
    HANDLE hFile;
    DWORD fileSize;
    BOOL readFileSuccess;
    PCHAR fileBuf = NULL;
    DWORD bytesRead;

    PCHAR lineStart, lineToken, ctx, lineBufCtx;
    char lineBuf[ND_MIN_BUF_SIZE] = { 0 };
    INT32 regIdx;
    QWORD regVal;

    // pre-init
    hFile = INVALID_HANDLE_VALUE;
    fileSize = 0;
    regIdx = 0;
    regVal = 0;
    ctx = lineBufCtx = NULL;

    if ((NULL == FileName) || (NULL == Options))
    {
        goto cleanup_and_exit;
    }

    hFile = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        goto cleanup_and_exit;
    }

    fileSize = GetFileSize(hFile, NULL);
    if (INVALID_FILE_SIZE == fileSize)
    {
        goto cleanup_and_exit;
    }

    fileBuf = (PCHAR)VirtualAlloc(NULL, fileSize + 3ull, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (NULL == fileBuf)
    {
        goto cleanup_and_exit;
    }

    readFileSuccess = ReadFile(hFile, fileBuf, fileSize, &bytesRead, NULL);
    if (!readFileSuccess)
    {
        printf("ReadFile failed: GLA: %x\n", GetLastError());
        goto cleanup_and_exit;
    }

    lineStart = strtok_s(fileBuf, "\r\n", &ctx);
    while (lineStart)
    {
        char regStr[ND_MIN_BUF_SIZE] = { 0 };
        char valStr[ND_MIN_BUF_SIZE] = { 0 };
        BOOLEAN isRip = FALSE;

        if (0 == strnlen_s(lineStart, ND_MIN_BUF_SIZE))
        {
            goto next_line;
        }
        strcpy_s(lineBuf, ND_MIN_BUF_SIZE, lineStart);
        
        lineToken = strtok_s(lineBuf, ":", &lineBufCtx);
        if (NULL == lineToken)
        {
            goto next_line;
        }
        
        str_strip(lineToken, (DWORD)strnlen_s(lineToken, ND_MIN_BUF_SIZE), " ,\"", 
            (DWORD)strnlen_s(" ,\"", ND_MIN_BUF_SIZE), regStr, ND_MIN_BUF_SIZE);

        // first element is the register
        regIdx = regstr_to_idx(regStr);
        if (regIdx < 0 || regIdx >= ARRAYSIZE(Options->ShemuRegs))
        {
            if (!_stricmp(regStr, "rip"))
            {
                isRip = TRUE;
            }
            else
            {
                printf("Corrupt line: %s\n", lineStart);
                goto next_line;
            }
        }

        // second element is the value
        lineToken = strtok_s(NULL, ":", &lineBufCtx);
        if ((NULL == lineToken))
        {
            printf("Corrupt line: %s\n", lineStart);
            goto next_line;
        }

        str_strip(lineToken, (DWORD)strnlen_s(lineToken, ND_MIN_BUF_SIZE), " ,\"", 
            (DWORD)strnlen_s(" ,\"", ND_MIN_BUF_SIZE), valStr, ND_MIN_BUF_SIZE);
        regVal = strtoull(valStr, NULL, 0);

        if (!isRip)
        {
            Options->ShemuRegs[regIdx] = (size_t)regVal;
        }
        else
        {
            Options->Rip = (size_t)regVal;
        }

    next_line:
        lineStart = strtok_s(NULL, "\r\n", &ctx);
        memset(lineBuf, 0, ND_MIN_BUF_SIZE);
    }

cleanup_and_exit:
    if (INVALID_HANDLE_VALUE != hFile)
    {
        CloseHandle(hFile);
    }
    if (NULL != fileBuf)
    {
        VirtualFree(fileBuf, 0, MEM_RELEASE);
    }

}

void ShemuLog(PCHAR data)
{
    printf("%s", data);
}

bool ShemuAccessMem(PSHEMU_CONTEXT Ctx, uint64_t Gla, size_t Size, uint8_t *Buffer, bool Store)
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

//
// handle_shemu
//
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
    ctx.Shellcode = malloc(shellSize);
    if (NULL == ctx.Shellcode)
    {
        printf("Memory error: couldn't allocated %zu bytes!\n", fsize);
        free(fNameDecoded);
        return;
    }

#define STACK_SIZE 0x2000

    ctx.Stack = malloc(STACK_SIZE);
    if (NULL == ctx.Stack)
    {
        printf("Memory error: couldn't allocated %zu bytes!\n", fsize);
        free(fNameDecoded);
        return;
    }

    ctx.Intbuf = malloc(shellSize + STACK_SIZE);
    if (NULL == ctx.Intbuf)
    {
        printf("Memory error: couldn't allocated %zu bytes!\n", fsize);
        free(fNameDecoded);
        return;
    }

    memset(ctx.Shellcode, 0, shellSize);
    memset(ctx.Stack, 0, STACK_SIZE);
    memcpy((BYTE *)ctx.Shellcode, (BYTE *)buffer, fsize);
    memset(ctx.Intbuf, 0, shellSize + STACK_SIZE);

    ctx.ShellcodeBase = (rip != 0 ? rip & PAGE_MASK : 0x200000);
    ctx.ShellcodeSize = (DWORD)shellSize;
    ctx.StackBase = 0x100000;
    ctx.StackSize = STACK_SIZE;
    ctx.Registers.RegRsp = 0x101000;
    ctx.IntbufSize = (DWORD)shellSize + STACK_SIZE;

    ctx.Registers.RegFlags = NDR_RFLAG_IF | 2;
    ctx.Registers.RegRip = ctx.ShellcodeBase + offset;

    ctx.Segments.Cs.Selector = 0x10;
    ctx.Segments.Ds.Selector = 0x28;
    ctx.Segments.Es.Selector = 0x28;
    ctx.Segments.Ss.Selector = 0x28;
    ctx.Segments.Fs.Selector = 0x30;
    ctx.Segments.Fs.Base = 0x7FFF0000;
    ctx.Segments.Gs.Selector = 0x30;
    ctx.Segments.Gs.Base = 0x7FFF0000;

    // Dummy values, to resemble regular CR0/CR4 values.
    ctx.Registers.RegCr0 = 0x0000000080050031;
    ctx.Registers.RegCr4 = 0x0000000000170678;

    ctx.Mode = Options->Mode;
    ctx.Ring = Options->Ring;
    ctx.TibBase = Options->Mode == ND_CODE_32 ? ctx.Segments.Fs.Base : ctx.Segments.Gs.Base;
    ctx.MaxInstructionsCount = 4096;
    ctx.Flags = 0;
    ctx.Options = SHEMU_OPT_TRACE_EMULATION;
    ctx.Log = &ShemuLog;
    ctx.AccessMemory = &ShemuAccessMem;

    // Configurable thresholds.
    ctx.NopThreshold = SHEMU_DEFAULT_NOP_THRESHOLD;
    ctx.StrThreshold = SHEMU_DEFAULT_STR_THRESHOLD;
    ctx.MemThreshold = SHEMU_DEFAULT_MEM_THRESHOLD;

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

    shstatus = ShemuEmulate(&ctx);

    printf("Emulation terminated with status 0x%08x, flags: 0x%llx, %u NOPs\n", 
           shstatus, (unsigned long long)ctx.Flags, ctx.NopCount);
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
    if (ctx.Flags & SHEMU_FLAG_TIB_ACCESS)
    {
        printf("        SHEMU_FLAG_TIB_ACCESS\n");
    }
    if (ctx.Flags & SHEMU_FLAG_SYSCALL)
    {
        printf("        SHEMU_FLAG_SYSCALL\n");
    }
    if (ctx.Flags & SHEMU_FLAG_STACK_STR)
    {
        printf("        SHEMU_FLAG_STACK_STR\n");
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

    if (fNameDecoded != NULL)
    {
        hFile = CreateFileA(fNameDecoded, GENERIC_WRITE, FILE_SHARE_READ, NULL, 
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hFile)
        {
            printf("Could not open the file %s : 0x%08x\n", fNameDecoded, GetLastError());
            return;
        }

        WriteFile(hFile, (BYTE *)ctx.Shellcode, (DWORD)fsize, &outSize, NULL);
        if (outSize == 0)
        {
            printf("No bytes written to %s!\n", fNameDecoded);
        }

        CloseHandle(hFile);
        free(fNameDecoded);
    }

    free(ctx.Shellcode);
    free(ctx.Stack);
    free(ctx.Intbuf);
}


//
// _tmain
//
int main(
    __in int argc, 
    __in char* argv[]
    )
{
    HANDLE hFile, hMapping;
    DWORD fsize, offset;
    SIZE_T rip;
    char text[ND_MIN_BUF_SIZE], *fname, *target, *shemuCtxFname;
    BYTE mode, print, highlight, fmode, hmode, stats, exi, vend, feat, search, isShemu, isShemuCtxf, isKernel;
    INT ret, i;
    BYTE hexbuf[256], *buffer;
    DISASM_OPTIONS options;
    
    // preinit
    buffer = NULL;
    memset(text, 0, sizeof(text));
    memset(&options, 0, sizeof(options));
    print = 1;
    highlight = 0;
    fmode = 0;
    hmode = 0;
    stats = 0;
    exi = 0;
    offset = 0;
    search = 0;
    feat = ND_FEAT_ALL;
    vend = ND_VEND_ANY;
    fname = NULL;
    mode = ND_CODE_16;
    hFile = NULL;
    hMapping = NULL;
    target = NULL;
    shemuCtxFname = NULL;
    rip = 0;
    isShemu = 0;
    isShemuCtxf = 0;
    isKernel = 0;

    if (NULL == argv)
    {
        return -1;
    }

    if (argc < 3)
    {
        uint32_t major, minor, revision;
        char *date, *time;

        NdGetVersion(&major, &minor, &revision, &date, &time);

        printf("Napoca Disassembler version %u.%u.%u, built on %s %s\n", major, minor, revision, date, time);
        printf("Usage: disasm -f file|-h hex-string -b[16|32|64] [-nv] [-iv] [-hl] [-s] [-c] [-shctxf contextfile] "
            "[-reg_name reg_val]\n");
        printf("        -f file          specify input file\n");
        printf("        -h hexstring     specify hex string with instructions\n");
        printf("        -o offset        start disasm at specified offset\n");
        printf("        -r rip           use the provided RIP\n");
        printf("        -b[16|32|64]     set decoding mode; default is 16\n");
        printf("        -v[intel|amd|cyrix|mpx|any] set preferred vendor\n");
        printf("        -t[none|all|mpx|cet|cldm] set preferred feature mode; default is all\n");
        printf("        -s \"ins\"       search for the given instructions\n");
        printf("        -nv              don't print disassembly\n");
        printf("        -iv              display statistics\n");
        printf("        -exi             print extended info about instructions\n");
        printf("        -shemu           emulate the file/hex-string\n");
        printf("        -shctxf contextfile specify file that contains the context for shemu. "
            "Ignored if shemu is not used. Overrides registers specified in command line\n");
        printf("            File format: each row contains the format: [regname]:[regvalue]\n");
        printf("        -regname regval specify registers to be set for the shemu context. Ignored if shemu is not used\n");
        printf("            Examples of valid command line register naming: \"RegRax\" ; \"rax\" ; \"reg_rax\"\n");
        printf("        -k               specify kernel mode for shemu emulation. Ignore if shemu is not specified.\n");
        printf("        -hl              highlight instruction parts:\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
            FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
        printf("            light white      prefixes\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        printf("            light green      opcodes\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
        printf("            light yellow     modrm and sib\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE|FOREGROUND_INTENSITY);
        printf("            light blue       displacement\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_INTENSITY);
        printf("            light red        relative offset, immediate, address\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
        printf("Instrux size: 0x%zx bytes\n", sizeof(INSTRUX));
        ret = -1;
        goto cleanup_and_exit;
    }

    i = 1;
    while (i < argc)
    {       
        DWORD gprIdx;

        if (match_gpr(argv[i], &gprIdx))
        {
            if (i + 1 >= argc)
            {
                printf("No value given for %s!\n", argv[i]);
            }
            else
            {
                options.ShemuRegs[gprIdx] = (size_t)strtoull(argv[i + 1], NULL, 0);
                options.UseShemuRegs = TRUE;
                i++;
            }
        }
        else if (strcmp(argv[i], "-shctxf") == 0)
        {
            if (i + 1 < argc)
            {
                isShemuCtxf = 1;
                options.UseShemuRegs = TRUE;

                shemuCtxFname = argv[i + 1];
                i++;
            }
        }
        else if (strcmp(argv[i], "-shemu") == 0)
        {
            isShemu = 1;
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'f' && argv[i][2] == 0)
        {
            if (i + 1 < argc)
            {
                fmode = 1;
                fname = argv[i + 1];
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'h' && argv[i][2] == 0)
        {
            if (i + 1 < argc)
            {
                hmode = 1;
                fname = argv[i + 1];
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'o' && argv[i][2] == 0)
        {
            if (i + 1 < argc)
            {
                sscanf_s(argv[i + 1], "%x", &offset);
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'r' && argv[i][2] == 0)
        {
            if (i + 1 < argc)
            {
                sscanf_s(argv[i + 1], "%zx", &rip);
                i++;
            }
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'k' && argv[i][2] == 0)
        {
            isKernel = 1;
        }
        else if (0 == strcmp(argv[i], "-b16"))
        {
            mode = ND_CODE_16;
        } 
        else if (0 == strcmp(argv[i], "-b32"))
        {
            mode = ND_CODE_32;
        } 
        else if (0 == strcmp(argv[i], "-b64"))
        {
            mode = ND_CODE_64;
        }
        else if (0 == strcmp(argv[i], "-vintel"))
        {
            vend = ND_VEND_INTEL;
        }
        else if (0 == strcmp(argv[i], "-vamd"))
        {
            vend = ND_VEND_AMD;
        }
        else if (0 == strcmp(argv[i], "-vgeode"))
        {
            vend = ND_VEND_GEODE;
        }
        else if (0 == strcmp(argv[i], "-vcyrix"))
        {
            vend = ND_VEND_CYRIX;
        }
        else if (0 == strcmp(argv[i], "-vany"))
        {
            vend = ND_VEND_ANY;
        }
        else if (0 == strcmp(argv[i], "-tall"))
        {
            feat = ND_FEAT_ALL;
        }
        else if (0 == strcmp(argv[i], "-tmpx"))
        {
            if (feat == ND_FEAT_ALL)
            {
                feat = 0;
            }

            feat |= ND_FEAT_MPX;
        }
        else if (0 == strcmp(argv[i], "-tcet"))
        {
            if (feat == ND_FEAT_ALL)
            {
                feat = 0;
            }

            feat |= ND_FEAT_CET;
        }
        else if (0 == strcmp(argv[i], "-tcldm"))
        {
            if (feat == ND_FEAT_ALL)
            {
                feat = 0;
            }

            feat |= ND_FEAT_CLDEMOTE;
        }
        else if (0 == strcmp(argv[i], "-tnone"))
        {
            feat = ND_FEAT_NONE;
        }
        else if (0 == strcmp(argv[i], "-nv"))
        {
            print = 0;
        } 
        else if (0 == strcmp(argv[i], "-hl"))
        {
            highlight = 1;
        }
        else if (0 == strcmp(argv[i], "-iv"))
        {
            stats = 1;
        }
        else if (0 == strcmp(argv[i], "-exi"))
        {
            exi = 1;
        }
        else if (0 == strcmp(argv[i], "-s"))
        {
            search = 1;

            if (i + 1 == argc)
            {
                printf("-s requires an argument!\n");
                return -1;
            }

            target = argv[++i];

            if (strlen(target) >= ND_MIN_BUF_SIZE)
            {
                printf("Target instruction too long! Max is %d bytes!\n", ND_MIN_BUF_SIZE);
                return -1;
            }
        }
        else
        {
            printf("Unknown option: '%s'\n", argv[i]);
        }

        i++;
    }

    if (0 == fmode && 0 == hmode)
    {
        printf("Expecting -f or -h option!\n");
        return -1;
    }

    if (fmode)
    {
        // Open the file.
        hFile = CreateFileA(fname, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, 
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hFile)
        {
            printf("Couldn't open file '%s': 0x%08x\n", fname, GetLastError());
            ret = -1;
            goto cleanup_and_exit;
        }

        // Create a file mapping.
        hMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, 0, "DisasmFile");
        if (NULL == hMapping)
        {
            printf("Couldn't create file mapping for '%s': 0x%08x\n", argv[1], GetLastError());
            ret = -1;
            goto cleanup_and_exit;
        }

        // Map the file.
        buffer = MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (NULL == buffer)
        {
            printf("Couldn't map the view for '%s': 0x%08x\n", argv[1], GetLastError());
            ret = -1;
            goto cleanup_and_exit;
        }

        fsize = GetFileSize(hFile, NULL);
    }
    else
    {
        DWORD idx;

        memset(hexbuf, 0, sizeof(hexbuf));

        fsize = (DWORD)strlen(fname);

        if (fsize > 512)
        {
            printf("Max 256-bytes buffer accepted!\n");
            return -1;
        }
        
        if (fsize % 2 == 1)
        {
            printf("Even-sized hex buffer expected!\n");
            return -1;
        }

        for (idx = 0; idx < fsize / 2; idx++)
        {
            hexbuf[idx] = ((hex_to_bin(fname[idx * 2]) << 4) | (hex_to_bin(fname[idx * 2 + 1]))) & 0xFF;
        }

        fname = NULL;
        fsize /= 2;
        buffer = hexbuf;
    }

    if (offset >= fsize)
    {
        printf("The offset exceeds the buffer size!\n");
        ret = -1;
        goto cleanup_and_exit;
    }

    options.FileName = fname;
    options.Buffer = buffer;
    options.Size = fsize;
    options.ExtendedInfo = exi;
    options.Highlight = highlight;
    options.Mode = mode;
    options.Ring = isKernel ? 0 : 3;
    options.Offset = offset;
    options.Stats = stats;
    options.Search = search;
    options.Target = target;
    options.Print = print;
    options.Vendor = vend;
    options.Feature = feat;
    options.Rip = rip;

    if (isShemu)
    {
        if (isShemuCtxf)
        {
            set_shemuctx_file(shemuCtxFname, &options);
        }

        handle_shemu(&options);
    }
    else if (search)
    {
        handle_search(&options);
    }
    else
    {
        handle_disasm(&options);
    }

    // All should be good.
    ret = 0;

cleanup_and_exit:
    if (fmode)
    {
        if (NULL != buffer)
        {
            UnmapViewOfFile(buffer);

            buffer = NULL;
        }

        if (NULL != hMapping)
        {
            CloseHandle(hMapping);

            hMapping = NULL;
        }

        if ((NULL != hFile) && (INVALID_HANDLE_VALUE != hFile))
        {
            CloseHandle(hFile);

            hFile = NULL;
        }
    }

    return ret;
}
