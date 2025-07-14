/*
 * Copyright (c) 2025 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <Intrin.h>
#endif // WIN32

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Main disasm header file.
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


const char* 
set_to_string(
    _In_ ND_INS_SET Set
)
{
    switch (Set)
    {
    case ND_SET_3DNOW:                 return "3DNOW";
    case ND_SET_ADX:                   return "ADX";
    case ND_SET_AES:                   return "AES";
    case ND_SET_APX_F:                 return "APX_F";
    case ND_SET_AMD:                   return "AMD";
    case ND_SET_AMXBF16:               return "AMX-BF16";
    case ND_SET_AMXFP16:               return "AMX-FP16";
    case ND_SET_AMXFP8:                return "AMX-FP8";
    case ND_SET_AMXINT8:               return "AMX-INT8";
    case ND_SET_AMXTILE:               return "AMX-TILE";
    case ND_SET_AMXCOMPLEX:            return "AMX-COMPLEX";
    case ND_SET_AMXTF32:               return "AMX-TF32";
    case ND_SET_AMXAVX512:             return "AMX-AVX512";
    case ND_SET_AMXMOVRS:              return "AMX-MOVRS";
    case ND_SET_AMXTRANSPOSE:          return "AMX-TRANSPOSE";
    case ND_SET_AVX:                   return "AVX";
    case ND_SET_AVX102:                return "AVX10_2";
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
    case ND_SET_AVXVNNIINT16:          return "AVXVNNIINT16";
    case ND_SET_BMI1:                  return "BMI1";
    case ND_SET_BMI2:                  return "BMI2";
    case ND_SET_BHI_DIS_S:             return "BHI_DIS_S";
    case ND_SET_CET_SS:                return "CET_SS";
    case ND_SET_CET_IBT:               return "CET_IBT";
    case ND_SET_CLDEMOTE:              return "CLDEMOTE";
    case ND_SET_CLFSH:                 return "CLFSH";
    case ND_SET_CLFSHOPT:              return "CLFSHOPT";
    case ND_SET_CLWB:                  return "CLWB";
    case ND_SET_CLZERO:                return "CLZERO";
    case ND_SET_CMPCCXADD:             return "CMPCCXADD";
    case ND_SET_CMPXCHG16B:            return "CMPXCHG16B";
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
    case ND_SET_MOVRS:                 return "MOVRS";
    case ND_SET_MPX:                   return "MPX";
    case ND_SET_MSRLIST:               return "MSRLIST";
    case ND_SET_MSR_IMM:               return "MSR_IMM";
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
    case ND_SET_SHA512:                return "SHA512";
    case ND_SET_SM3:                   return "SM3";
    case ND_SET_SM4:                   return "SM4";
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
    case ND_SET_TSE:                   return "TSE";
    case ND_SET_TSX:                   return "TSX";
    case ND_SET_TSXLDTRK:              return "TSXLDTRK";
    case ND_SET_UD:                    return "UD";
    case ND_SET_UINTR:                 return "UINTR";
    case ND_SET_UNKNOWN:               return "UNKNOWN";
    case ND_SET_USER_MSR:              return "USER_MSR";
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
    _In_ ND_INS_CATEGORY Category
)
{
    switch (Category)
    {
    case ND_CAT_3DNOW:                return "3DNOW";
    case ND_CAT_AES:                  return "AES";
    case ND_CAT_AESKL:                return "AESKL";
    case ND_CAT_ARITH:                return "ARITH";
    case ND_CAT_AMX:                  return "AMX";
    case ND_CAT_APX:                  return "APX";
    case ND_CAT_AVX:                  return "AVX";
    case ND_CAT_AVX10BF16:            return "AVX10BF16";
    case ND_CAT_AVX10CMPSFP:          return "AVX10CMPSFP";
    case ND_CAT_AVX10CONVERT:         return "AVX10CONVERT";
    case ND_CAT_AVX10INT:             return "AVX10INT";
    case ND_CAT_AVX10MINMAX:          return "AVX10MINMAX";
    case ND_CAT_AVX10PARTCOPY:        return "AVX10PARTCOPY";
    case ND_CAT_AVX10SCONVERT:        return "AVX10SCONVERT";
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
    case ND_CAT_AVXVNNIINT16:         return "AVXVNNIINT16";
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
    case ND_CAT_SHA512:               return "SHA512";
    case ND_CAT_SHIFT:                return "SHIFT";
    case ND_CAT_SM3:                  return "SM3";
    case ND_CAT_SM4:                  return "SM4";
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
    case ND_CAT_USER_MSR:             return "USER_MSR";
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
    _In_ ND_OPERAND_TYPE OpType
)
{
    switch (OpType)
    {
    case ND_OP_REG: return "Register";
    case ND_OP_IMM: return "Immediate";
    case ND_OP_CONST: return "Constant";
    case ND_OP_MEM: return "Memory";
    case ND_OP_ADDR: return "Address";
    case ND_OP_ADDR_NEAR: return "Address";
    case ND_OP_OFFS: return "Offset";
    case ND_OP_BANK: return "Bank";
    case ND_OP_DFV: return "Default flags";
    default: return "???";
    }
}


const char* 
regtype_to_string(
    _In_ ND_REG_TYPE RegType
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
    _In_ ND_OPERAND_ENCODING Encoding
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
    _In_ ND_TUPLE Tuple
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
exception_type_to_string(
    _In_ ND_EX_TYPE ExType
)
{
    switch (ExType)
    {
    case ND_EXT_1: return "1";
    case ND_EXT_2: return "2";
    case ND_EXT_3: return "3";
    case ND_EXT_4: return "4";
    case ND_EXT_5: return "5";
    case ND_EXT_6: return "6";
    case ND_EXT_7: return "7";
    case ND_EXT_8: return "8";
    case ND_EXT_9: return "9";
    case ND_EXT_10: return "10";
    case ND_EXT_11: return "11";
    case ND_EXT_12: return "12";
    case ND_EXT_13: return "13";
    case ND_EXT_14: return "14";

    case ND_EXT_K20: return "K20";
    case ND_EXT_K21: return "K21";

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

    case ND_EXT_AMX_E1: return "AMX-E1";
    case ND_EXT_AMX_E2: return "AMX-E2";
    case ND_EXT_AMX_E3: return "AMX-E3";
    case ND_EXT_AMX_E4: return "AMX-E4";
    case ND_EXT_AMX_E5: return "AMX-E5";
    case ND_EXT_AMX_E6: return "AMX-E6";
    case ND_EXT_AMX_E7: return "AMX-E7";
    case ND_EXT_AMX_E8: return "AMX-E8";
    case ND_EXT_AMX_E9: return "AMX-E9";
    case ND_EXT_AMX_E10: return "AMX-E10";
    case ND_EXT_AMX_E11: return "AMX-E11";

    case ND_EXT_AMX_EVEX_E1: return "AMX-EVEX-E1";
    case ND_EXT_AMX_EVEX_E2: return "AMX-EVEX-E2";
    case ND_EXT_AMX_EVEX_E3: return "AMX-EVEX-E3";
    case ND_EXT_AMX_EVEX_E4: return "AMX-EVEX-E4";
    case ND_EXT_AMX_EVEX_E5: return "AMX-EVEX-E5";
    case ND_EXT_AMX_EVEX_E6: return "AMX-EVEX-E6";
    case ND_EXT_AMX_EVEX_E7: return "AMX-EVEX-E7";
    case ND_EXT_AMX_EVEX_E8: return "AMX-EVEX-E8";
    case ND_EXT_AMX_EVEX_E9: return "AMX-EVEX-E9";
    case ND_EXT_AMX_EVEX_E10: return "AMX-EVEX-E10";
    case ND_EXT_AMX_EVEX_E11: return "AMX-EVEX-E11";

    case ND_EXT_APX_EVEX_BMI: return "APX-EVEX-BMI";
    case ND_EXT_APX_EVEX_CCMP: return "APX-EVEX-CCMP";
    case ND_EXT_APX_EVEX_WRSS: return "APX-EVEX-WRSS";
    case ND_EXT_APX_EVEX_WRUSS: return "APX-EVEX-WRUSS";
    case ND_EXT_APX_EVEX_CFCMOV: return "APX-EVEX-CFCMOV";
    case ND_EXT_APX_EVEX_CMPCCXADD: return "APX-EVEX-CMPCCXADD";
    case ND_EXT_APX_EVEX_ENQCMD: return "APX-EVEX-ENQCMD";
    case ND_EXT_APX_EVEX_INT: return "APX-EVEX-INT";
    case ND_EXT_APX_EVEX_INVEPT: return "APX-EVEX-INVEPT";
    case ND_EXT_APX_EVEX_INVPCID: return "APX-EVEX-INVPCID";
    case ND_EXT_APX_EVEX_INVVPID: return "APX-EVEX-INVVPID";
    case ND_EXT_APX_EVEX_KEYLOCKER: return "APX-EVEX-KEYLOCKER";
    case ND_EXT_APX_EVEX_KMOV: return "APX-EVEX-KMOV";
    case ND_EXT_APX_EVEX_PP2: return "APX-EVEX-PP2";
    case ND_EXT_APX_EVEX_SHA: return "APX-EVEX-SHA";
    case ND_EXT_APX_EVEX_RAOINT: return "APX-EVEX-RAO-INT";
    case ND_EXT_APX_EVEX_USER_MSR: return "APX-EVEX-USER-MSR";
    case ND_EXT_APX_EVEX_MOVRS: return "APX-EVEX-MOVRS";

    default: return "None";
    }
}


void
print_instruction(
    _In_ size_t Rip,
    _In_ PINSTRUX Instrux,
    _In_ PDISASM_OPTIONS Options
)
{
    char instruxText[ND_MIN_BUF_SIZE];
    DWORD k = 0, idx = 0, i = 0;

    printf("%016zX ", Rip);

    if (Options->Highlight)
    {

#ifdef WIN32
        // Enable virtual terminal processing on Windows, so we can display colors.
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD consoleMode;

        if (consoleHandle == INVALID_HANDLE_VALUE)
        {
            return;
        }

        if (!GetConsoleMode(consoleHandle, &consoleMode))
        {
            return;
        }

        SetConsoleMode(consoleHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

        // Display prefixes.
        set_bold_fg_color(FG_White);
        for (idx = 0; idx < Instrux->PrefLength; idx++, k++)
        {
            printf("%02x", Instrux->InstructionBytes[k]);
        }
        reset_fg_color();

        // Display opcodes.
        set_bold_fg_color(FG_Green);
        for (idx = 0; idx < (DWORD)(ND_IS_3DNOW(Instrux) ? Instrux->OpLength - 1 : Instrux->OpLength); idx++, k++)
        {
            printf("%02x", Instrux->InstructionBytes[k]);
        }
        reset_fg_color();

        // Display modrm and sib.
        set_bold_fg_color(FG_Yellow);
        for (idx = 0; idx < (DWORD)(Instrux->HasModRm + Instrux->HasSib); idx++, k++)
        {
            printf("%02x", Instrux->InstructionBytes[k]);
        }
        reset_fg_color();

        // Display displacement.
        set_bold_fg_color(FG_Blue);
        for (idx = 0; idx < (DWORD)(Instrux->DispLength); idx++, k++)
        {
            printf("%02x", Instrux->InstructionBytes[k]);
        }
        reset_fg_color();

        // Display relative offset/moffset/immediates.
        set_bold_fg_color(FG_Red);
        for (idx = 0; idx < (DWORD)(Instrux->Imm1Length + Instrux->Imm2Length +
            Instrux->RelOffsLength + Instrux->MoffsetLength +
            Instrux->HasSseImm + Instrux->AddrLength); idx++, k++)
        {
            printf("%02x", Instrux->InstructionBytes[k]);
        }
        reset_fg_color();

        if (ND_IS_3DNOW(Instrux))
        {
            set_bold_fg_color(FG_Green);
            for (; k < Instrux->Length; k++)
            {
                printf("%02x", Instrux->InstructionBytes[k]);
            }
            reset_fg_color();
        }

#ifdef WIN32
        // Reset console mode on Windows.
        SetConsoleMode(consoleHandle, consoleMode);
#endif
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

        if (Instrux->ExceptionType != ND_EXT_None)
        {
            printf("        Exception class: %s, ", 
                Instrux->ExceptionType >= ND_EXT_1 && Instrux->ExceptionType <= ND_EXT_14 ? "SSE/VEX" :
                Instrux->ExceptionType >= ND_EXT_E1 && Instrux->ExceptionType <= ND_EXT_E12NP ? "EVEX" :
                Instrux->ExceptionType >= ND_EXT_K20 && Instrux->ExceptionType <= ND_EXT_K21 ? "Opmask" : 
                Instrux->ExceptionType >= ND_EXT_AMX_E1 && Instrux->ExceptionType <= ND_EXT_AMX_E11 ? "AMX" : 
                Instrux->ExceptionType >= ND_EXT_AMX_EVEX_E1 && Instrux->ExceptionType < ND_EXT_MAX ? "APX" : "???");

            printf("exception type: %s\n", exception_type_to_string(Instrux->ExceptionType));
        }

        if (Instrux->SimdExceptions.Raw != 0)
        {
            printf("        SIMD Exceptions: %s%s%s%s%s%s\n",
                Instrux->SimdExceptions.IE ? "I" : "",
                Instrux->SimdExceptions.DE ? "D" : "",
                Instrux->SimdExceptions.ZE ? "Z" : "",
                Instrux->SimdExceptions.OE ? "O" : "",
                Instrux->SimdExceptions.UE ? "U" : "",
                Instrux->SimdExceptions.PE ? "P" : "");
        }

        if (Instrux->RflAccess != 0)
        {
            DWORD fidx, all;
            bool individual = false;
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

                    individual = true;

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
                Instrux->Operands[i].Type == ND_OP_IMM ? Instrux->Operands[i].Info.Immediate.RawSize :
                Instrux->Operands[i].Type == ND_OP_OFFS ? Instrux->Operands[i].Info.RelativeOffset.RawSize :
                Instrux->Operands[i].Size, 
                encoding_to_string(Instrux->Operands[i].Encoding)
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
                    Instrux->Operands[i].Info.Memory.Broadcast.Size,
                    Instrux->Operands[i].Info.Memory.Broadcast.Count);
            }

            if (Instrux->Operands[i].Decorator.HasMask)
            {
                printf("                Decorator: Mask k%d\n", Instrux->Operands[i].Decorator.Msk);
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
            if (Instrux->EvexMode == ND_EVEXM_EVEX)
            {
                printf("            EVEX:   0x%02x 0x%02x 0x%02x 0x%02x > mmm: %d, B4: %d, R4: %d, B3: %d, X3: %d, R3: %d, pp: %d, X4: %u, "
                    "vvvv: %d, W: %d, aaa: %d, V': %d, b: %d, L'L: %d, z: %d\n",
                    Instrux->Evex.Evex[0], 
                    Instrux->Evex.Evex[1], 
                    Instrux->Evex.Evex[2], 
                    Instrux->Evex.Evex[3],
                    Instrux->Evex.m, 
                    Instrux->Evex.b4, 
                    Instrux->Evex.rp, 
                    Instrux->Evex.b, 
                    Instrux->Evex.x, 
                    Instrux->Evex.r,
                    Instrux->Evex.p, 
                    Instrux->Evex.u, 
                    Instrux->Evex.v, 
                    Instrux->Evex.w, 
                    Instrux->Evex.a, 
                    Instrux->Evex.vp,
                    Instrux->Evex.bm, 
                    Instrux->Evex.l,
                    Instrux->Evex.z);
            }
            else if (Instrux->EvexMode == ND_EVEXM_LEGACY)
            {
                printf("            EVEX:   0x%02x 0x%02x 0x%02x 0x%02x > mmm: %d, B4: %d, R4: %d, B3: %d, X3: %d, R3: %d, pp: %d, X4: %u, "
                    "vvvv: %d, W: %d, zero: %d, NF: %d, V4: %d, ND: %d, zero: %d\n",
                    Instrux->EvexLegacy.Evex[0], 
                    Instrux->EvexLegacy.Evex[1], 
                    Instrux->EvexLegacy.Evex[2], 
                    Instrux->EvexLegacy.Evex[3],
                    Instrux->EvexLegacy.m, 
                    Instrux->EvexLegacy.b4, 
                    Instrux->EvexLegacy.rp, 
                    Instrux->EvexLegacy.b, 
                    Instrux->EvexLegacy.x, 
                    Instrux->EvexLegacy.r,
                    Instrux->EvexLegacy.p, 
                    Instrux->EvexLegacy.x4, 
                    Instrux->EvexLegacy.v, 
                    Instrux->EvexLegacy.w, 
                    Instrux->EvexLegacy.zero01, 
                    Instrux->EvexLegacy.nf,
                    Instrux->EvexLegacy.v4, 
                    Instrux->EvexLegacy.nd,
                    Instrux->EvexLegacy.zero53);
            }
            else if (Instrux->EvexMode == ND_EVEXM_VEX)
            {
                printf("            EVEX:   0x%02x 0x%02x 0x%02x 0x%02x > mmm: %d, B4: %d, R4: %d, B3: %d, X3: %d, R3: %d, pp: %d, X4: %u, "
                    "vvvv: %d, W: %d, zero: %d, NF: %d, V4: %d, zero: %d, L: %d, zero: %d\n",
                    Instrux->EvexVex.Evex[0], 
                    Instrux->EvexVex.Evex[1], 
                    Instrux->EvexVex.Evex[2], 
                    Instrux->EvexVex.Evex[3],
                    Instrux->EvexVex.m, 
                    Instrux->EvexVex.b4, 
                    Instrux->EvexVex.rp, 
                    Instrux->EvexVex.b, 
                    Instrux->EvexVex.x, 
                    Instrux->EvexVex.r,
                    Instrux->EvexVex.p, 
                    Instrux->EvexVex.x4, 
                    Instrux->EvexVex.v, 
                    Instrux->EvexVex.w, 
                    Instrux->EvexVex.zero01, 
                    Instrux->EvexVex.nf,
                    Instrux->EvexVex.v4, 
                    Instrux->EvexVex.zero41, 
                    Instrux->EvexVex.l,
                    Instrux->EvexVex.zero62);
            }
            else if (Instrux->EvexMode == ND_EVEXM_COND)
            {
                printf("            EVEX:   0x%02x 0x%02x 0x%02x 0x%02x > mmm: %d, B4: %d, R4: %d, B3: %d, X3: %d, R3: %d, pp: %d, X4: %u, "
                    "CZSO: %d%d%d%d, W: %d, SC: %d, ND: %d, zero: %d\n",
                    Instrux->EvexCond.Evex[0], 
                    Instrux->EvexCond.Evex[1], 
                    Instrux->EvexCond.Evex[2], 
                    Instrux->EvexCond.Evex[3],
                    Instrux->EvexCond.m, 
                    Instrux->EvexCond.b4, 
                    Instrux->EvexCond.rp, 
                    Instrux->EvexCond.b, 
                    Instrux->EvexCond.x, 
                    Instrux->EvexCond.r,
                    Instrux->EvexCond.p, 
                    Instrux->EvexCond.x4, 
                    Instrux->EvexCond.CF, 
                    Instrux->EvexCond.ZF, 
                    Instrux->EvexCond.SF, 
                    Instrux->EvexCond.OF, 
                    Instrux->EvexCond.w, 
                    Instrux->EvexCond.sc, 
                    Instrux->EvexCond.nd,
                    Instrux->EvexCond.zero53);
            }
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
    _In_ PDISASM_OPTIONS Options
)
{
    INSTRUX instrux;
    ND_CONTEXT ctx = { 0 };
    unsigned long long icount = 0, istart, iend, start, end, itotal = 0, tilen = 0, ticount = 0;
    size_t rip, fsize = Options->Size;
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
                printf("%016zX ", rip + Options->Rip);

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
            else if (Options->Skip1)
            {
                rip++;
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
            icount, (end - start) * 1000 / CLOCKS_PER_SEC, icount / ((double)(end - start) / CLOCKS_PER_SEC), itotal / (double)icount, tilen / (double)ticount);
    }
}
