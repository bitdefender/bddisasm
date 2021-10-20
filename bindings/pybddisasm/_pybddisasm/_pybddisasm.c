/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#define _SIGNAL_H
#include <Python.h>
#include <assert.h>
#include "pybddisasm.h"


static char module_docstring[] = "This module provides an interface for bddisasm.";

static char pybddisasm_decode_ex_docstring[] = "Disasemble at the provided address.";
static char pybddisasm_decode_ex2_docstring[] = "Disasemble at the provided address.";
static char pybddisasm_decode_docstring[] = "Disasemble at the provided address.";
static char pybddisasm_to_text_docstring[] = "Disasemble at the provided address and give back only the text.";


static char *OperandTypeToString(ND_OPERAND_TYPE Type)
{
    switch (Type)
    {
    case ND_OP_NOT_PRESENT: return "not_present";
    case ND_OP_REG: return "reg";
    case ND_OP_MEM: return "mem";
    case ND_OP_IMM: return "imm";
    case ND_OP_OFFS: return "offs";
    case ND_OP_ADDR: return "addr";
    case ND_OP_CONST: return "const";
    case ND_OP_BANK: return "bank";
    }

    return "unknown";
}


static char *OperandEncodingToString(ND_OPERAND_ENCODING Encoding)
{
    switch (Encoding)
    {
    case ND_OPE_NP: return "NP";
    case ND_OPE_R: return "R";
    case ND_OPE_M: return "M";
    case ND_OPE_V: return "V";
    case ND_OPE_D: return "D";
    case ND_OPE_O: return "O";
    case ND_OPE_I: return "I";
    case ND_OPE_C: return "C";
    case ND_OPE_1: return "1";
    case ND_OPE_L: return "L";
    case ND_OPE_A: return "A";
    case ND_OPE_E: return "E";
    case ND_OPE_S: return "S";
    }

    return "unknown";
}


static char *RegTypeToString(ND_REG_TYPE RegType)
{
    switch (RegType)
    {
    case ND_REG_NOT_PRESENT: return "NOT_PRESENT";
    case ND_REG_GPR: return "GPR";
    case ND_REG_SEG: return "SEG";
    case ND_REG_FPU: return "FPU";
    case ND_REG_MMX: return "MMX";
    case ND_REG_SSE: return "SSE";
    case ND_REG_CR: return "CR";
    case ND_REG_DR: return "DR";
    case ND_REG_TR: return "TR";
    case ND_REG_BND: return "BND";
    case ND_REG_MSK: return "MSK";
    case ND_REG_TILE: return "TILE";
    case ND_REG_MSR: return "MSR";
    case ND_REG_XCR: return "XCR";
    case ND_REG_SYS: return "SYS";
    case ND_REG_X87: return "X87";
    case ND_REG_MXCSR: return "MXCSR";
    case ND_REG_PKRU: return "PKRU";
    case ND_REG_SSP: return "SSP";
    case ND_REG_FLG: return "FLG";
    case ND_REG_RIP: return "RIP";
    case ND_REG_UIF: return "UIF";
    }

    return "unknown";
}


static char *TupleTypeToString(ND_TUPLE TupleType)
{
    switch (TupleType)
    {
    case ND_TUPLE_None: return "None";
    case ND_TUPLE_FV: return "FV";
    case ND_TUPLE_HV: return "HV";
    case ND_TUPLE_QV: return "QV";
    case ND_TUPLE_T1S8: return "T1S8";
    case ND_TUPLE_T1S16: return "T1S16";
    case ND_TUPLE_T1S: return "T1S";
    case ND_TUPLE_T1F: return "T1F";
    case ND_TUPLE_T2: return "T2";
    case ND_TUPLE_T4: return "T4";
    case ND_TUPLE_T8: return "T8";
    case ND_TUPLE_FVM: return "FVM";
    case ND_TUPLE_HVM: return "HVM";
    case ND_TUPLE_QVM: return "QVM";
    case ND_TUPLE_OVM: return "OVM";
    case ND_TUPLE_M128: return "M128";
    case ND_TUPLE_DUP: return "DUP";
    case ND_TUPLE_T1_4X: return "4X";
    }

    return "unknown";
}


static char *RoundingModeToString(ND_ROUNDING RoundingMode)
{
    switch (RoundingMode)
    {
    case ND_RND_RNE: return "RNE";
    case ND_RND_RD: return "RD";
    case ND_RND_RU: return "RU";
    case ND_RND_RZ: return "RZ";
    }

    return "unknown";
}


static PyObject *_pybddisasm_build_rex(ND_REX *rex)
{
    return Py_BuildValue("{s,B, s,B, s,B, s,B, s,B}",
                         "Rex", rex->Rex,
                         "b", rex->b,
                         "x", rex->x,
                         "r", rex->r,
                         "w", rex->w);
}


static PyObject *_pybddisasm_build_modrm(ND_MODRM *modrm)
{
    return Py_BuildValue("{s,B, s,B, s,B, s,B}",
                         "ModRm", modrm->ModRm,
                         "rm", modrm->rm,
                         "reg", modrm->reg,
                         "mod", modrm->mod);
}


static PyObject *_pybddisasm_build_sib(ND_SIB *sib)
{
    return Py_BuildValue("{s,B, s,B, s,B, s,B}",
                         "Sib", sib->Sib,
                         "base", sib->base,
                         "index", sib->index,
                         "scale", sib->scale);
}


static PyObject *_pybddisasm_build_drex(ND_DREX *drex)
{
    return Py_BuildValue("{s,B, s,B, s,B, s,B, s,B, s,B, s,B}",
                         "Drex", drex->Drex,
                         "b", drex->b,
                         "x", drex->x,
                         "r", drex->r,
                         "oc0", drex->oc0,
                         "vd", drex->vd,
                         "d", drex->d);
}


static PyObject *_pybddisasm_build_op_access(ND_OPERAND_ACCESS *access)
{
    return Py_BuildValue("{s,B, s,B, s,B, s,B, s,B}",
                         "Access", access->Access,
                         "Read", access->Read,
                         "Write", access->Write,
                         "CondRead", access->CondRead,
                         "CondWrite", access->CondWrite);
}


static PyObject *_pybddisasm_build_op_flags(ND_OPERAND_FLAGS *flags)
{
    return Py_BuildValue("{s,B, s,B, s,B, s,B}",
                         "Flags", flags->Flags,
                         "IsDefault", flags->IsDefault,
                         "SignExtendedOp1", flags->SignExtendedOp1,
                         "SignExtendedDws", flags->SignExtendedDws);
}


static PyObject *_pybddisasm_build_op_reg(ND_OPDESC_REGISTER *reg)
{
    return Py_BuildValue("{s,s, s,B, s,I, s,I}",
                         "Type", RegTypeToString(reg->Type),
                         "Size", reg->Size,
                         "Reg", reg->Reg,
                         "Count", reg->Count);
}


static PyObject *_pybddisasm_build_op_memory(ND_OPDESC_MEMORY *mem)
{
    PyObject *nd_vsib = Py_BuildValue("{s,B, s,B, s,B}",
                                      "IndexSize", mem->Vsib.IndexSize,
                                      "ElemSize", mem->Vsib.ElemSize,
                                      "ElemCount", mem->Vsib.ElemCount);

    PyObject *p = Py_BuildValue("{"
                                "s,B,"     // HasSeg
                                "s,B,"     // HasBase
                                "s,B,"     // HasIndex
                                "s,B,"     // HasDisp
                                "s,B,"     // HasCompDisp
                                "s,B,"     // HasBroadcast

                                "s,B,"     // IsRipRel
                                "s,B,"     // IsStack
                                "s,B,"     // IsShadowStack
                                "s,B,"     // IsDirect
                                "s,B,"     // IsBitbase
                                "s,B,"     // IsAG
                                "s,B,"     // IsMib
                                "s,B,"     // IsVsib

                                "s,B,"     // BaseSize
                                "s,B,"     // IndexSize
                                "s,B,"     // DispSize
                                "s,B,"     // CompDispSize

                                "s,O,"     // Vsib

                                "s,B,"     // Seg
                                "s,B,"     // Base
                                "s,B,"     // Index
                                "s,B,"     // Scale

                                "s,K"      // Disp
                                "}",

                                "HasSeg", mem->HasSeg,
                                "HasBase", mem->HasBase,
                                "HasIndex", mem->HasIndex,
                                "HasDisp", mem->HasDisp,
                                "HasCompDisp", mem->HasCompDisp,
                                "HasBroadcast", mem->HasBroadcast,

                                "IsRipRel", mem->IsRipRel,
                                "IsStack", mem->IsStack,
                                "IsShadowStack", mem->IsShadowStack,
                                "IsDirect", mem->IsDirect,
                                "IsBitbase", mem->IsBitbase,
                                "IsAG", mem->IsAG,
                                "IsMib", mem->IsMib,
                                "IsVsib", mem->IsVsib,

                                "BaseSize", mem->BaseSize,
                                "IndexSize", mem->IndexSize,
                                "DispSize", mem->DispSize,
                                "CompDispSize", mem->CompDispSize,

                                "Vsib", nd_vsib,

                                "Seg", mem->Seg,
                                "Base", mem->Base,
                                "Index", mem->Index,
                                "Scale", mem->Index,

                                "Disp", mem->Disp);
    Py_XDECREF(nd_vsib);

    return p;
}


static PyObject *_pybddisasm_build_op_immediate(ND_OPDESC_IMMEDIATE *imm)
{
    return Py_BuildValue("{s,K}", "Imm", imm->Imm);
}


static PyObject *_pybddisasm_build_op_rel_offset(ND_OPDESC_RELOFFSET *rel_offset)
{
    return Py_BuildValue("{s,K}", "Rel", rel_offset->Rel);
}


static PyObject *_pybddisasm_build_op_address(ND_OPDESC_ADDRESS *address)
{
    return Py_BuildValue("{s,H, s,K}",
                         "BaseSeg", address->BaseSeg,
                         "Offset", address->Offset);
}


static PyObject *_pybddisasm_build_op_const(ND_OPDESC_CONSTANT *constant)
{
    return Py_BuildValue("{s,K}", "Const", constant->Const);
}


static PyObject *_pybddisasm_build_operand(ND_OPERAND *operand)
{
    PyObject *nd_access = _pybddisasm_build_op_access(&operand->Access);
    PyObject *nd_flags = _pybddisasm_build_op_flags(&operand->Flags);

    PyObject *nd_operand = NULL;

    switch (operand->Type)
    {
    case ND_OP_REG:
        nd_operand = _pybddisasm_build_op_reg(&operand->Info.Register);
        break;
    case ND_OP_MEM:
        nd_operand = _pybddisasm_build_op_memory(&operand->Info.Memory);
        break;
    case ND_OP_IMM:
        nd_operand = _pybddisasm_build_op_immediate(&operand->Info.Immediate);
        break;
    case ND_OP_OFFS:
        nd_operand = _pybddisasm_build_op_rel_offset(&operand->Info.RelativeOffset);
        break;
    case ND_OP_ADDR:
        nd_operand = _pybddisasm_build_op_address(&operand->Info.Address);
        break;
    case ND_OP_CONST:
        nd_operand = _pybddisasm_build_op_const(&operand->Info.Constant);
        break;
    case ND_OP_BANK:
        nd_operand = Py_BuildValue("{}");
        break;
    default:
        PyErr_SetString(PyExc_RuntimeError, "invalid operand type...");
        Py_RETURN_NONE;
    }

    PyObject *p = Py_BuildValue("{"
                                "s,s,"     // Type
                                "s,s,"     // Encoding
                                "s,I,"     // Size
                                "s,I,"     // RawSize
                                "s,O,"     // Access
                                "s,O,"     // Flags
                                "s,O"      // Operand
                                "}",
                                "Type", OperandTypeToString(operand->Type),
                                "Encoding", OperandEncodingToString(operand->Encoding),
                                "Size", operand->Size,
                                "RawSize", operand->RawSize,
                                "Access", nd_access,
                                "Flags", nd_flags,
                                "Operand", nd_operand);

    Py_XDECREF(nd_access);
    Py_XDECREF(nd_flags);
    Py_XDECREF(nd_operand);

    return p;
}


static PyObject *_pybddisasm_build_operands(ND_OPERAND *operands, size_t count)
{
    char op_str_format[1024] = {'[', 0};
    size_t last = 1;

    PyObject *nd_operands[ND_MAX_OPERAND] = {0};
    size_t op;

    for (op = 0; op < count && last < sizeof(op_str_format); op++)
    {
        if (op > 0)
        {
            op_str_format[last++] = ',';
        }

        op_str_format[last++] = 'O';

        nd_operands[op] = _pybddisasm_build_operand(&operands[op]);
    }

    op_str_format[last] = ']';

    PyObject *p = Py_BuildValue(op_str_format,
                                nd_operands[0],
                                nd_operands[1],
                                nd_operands[2],
                                nd_operands[3],
                                nd_operands[4],
                                nd_operands[5],
                                nd_operands[6],
                                nd_operands[7],
                                nd_operands[8],
                                nd_operands[9]);

    for (size_t o = 0; o < op; o++)
    {
        Py_XDECREF(nd_operands[o]);
    }

    return p;
}


static PyObject *_pybddisasm_build_exs(INSTRUX *instr)
{
    return Py_BuildValue("{"
                         "s,B,"         // w
                         "s,B,"         // r
                         "s,B,"         // x
                         "s,B,"         // b
                         "s,B,"         // rp
                         "s,B,"         // p
                         "s,B,"         // m
                         "s,B,"         // l
                         "s,B,"         // v
                         "s,B,"         // vp
                         "s,B,"         // bm
                         "s,B,"         // e
                         "s,B,"         // z
                         "s,B,"         // k
                         "s,B"          // s
                         "}",
                         "w", instr->Exs.w,
                         "r", instr->Exs.r,
                         "x", instr->Exs.x,
                         "b", instr->Exs.b,
                         "rp", instr->Exs.rp,
                         "p", instr->Exs.p,
                         "m", instr->Exs.m,
                         "l", instr->Exs.l,
                         "v", instr->Exs.v,
                         "vp", instr->Exs.vp,
                         "bm", instr->Exs.bm,
                         "e", instr->Exs.e,
                         "z", instr->Exs.z,
                         "k", instr->Exs.k,
                         "s", instr->Exs.s);
}


static PyObject *_pybddisasm_build_address(INSTRUX *instr)
{
    return Py_BuildValue("{s,I, s,H}",
                         "Ip", instr->Address.Ip,
                         "Cs", instr->Address.Cs);
}


static PyObject *_pybddisasm_build_cpuid_flag(ND_CPUID_FLAG *cpuid_flag)
{
    return Py_BuildValue("{"
                         "s,K,"         // Flag
                         "s,I,"         // Leaf
                         "s,I,"         // SubLeaf
                         "s,B,"         // Reg
                         "s,B"          // Bit
                         "}",
                         "Flag", cpuid_flag->Flag,
                         "Leaf", cpuid_flag->Leaf,
                         "SubLeaf", cpuid_flag->SubLeaf,
                         "Reg", cpuid_flag->Reg,
                         "Bit", cpuid_flag->Bit);
}


static PyObject *_pybddisasm_build_flags_access(INSTRUX *instr)
{
    return Py_BuildValue("{"
                         "s,B,"        // RegAccess
                         "s,I,"        // Tested
                         "s,I,"        // Modified
                         "s,I,"        // Set
                         "s,I,"        // Cleared
                         "s,I"         // Undefined
                         "}",
                         "RegAccess", instr->FlagsAccess.RegAccess,
                         "Tested", instr->FlagsAccess.Tested,
                         "Modified", instr->FlagsAccess.Modified,
                         "Set", instr->FlagsAccess.Set,
                         "Cleared", instr->FlagsAccess.Cleared,
                         "Undefined", instr->FlagsAccess.Undefined);
}


static PyObject *_pybddisasm_build_valid_modes(ND_VALID_MODES *valid_modes)
{
    return Py_BuildValue("{"
                         "s,H,"         // Raw

                         "s,B,"         // Ring0
                         "s,B,"         // Ring1
                         "s,B,"         // Ring2
                         "s,B,"         // Ring3

                         "s,B,"         // Real
                         "s,B,"         // V8086
                         "s,B,"         // Protected
                         "s,B,"         // Compatibility
                         "s,B,"         // Long
                         "s,B,"         // Smm

                         "s,B,"         // Sgx
                         "s,B,"         // Tsx

                         "s,B,"         // VmxRoot
                         "s,B,"         // VmxNonRoot
                         "s,B"          // VmxOff
                         "}",

                         "Raw", valid_modes->Raw,

                         "Ring0", valid_modes->Ring0,
                         "Ring1", valid_modes->Ring1,
                         "Ring2", valid_modes->Ring2,
                         "Ring3", valid_modes->Ring3,

                         "Real", valid_modes->Real,
                         "V8086", valid_modes->V8086,
                         "Protected", valid_modes->Protected,
                         "Compatibility", valid_modes->Compat,
                         "Long", valid_modes->Long,
                         "Smm", valid_modes->Smm,

                         "Sgx", valid_modes->Sgx,
                         "Tsx", valid_modes->Tsx,

                         "VmxRoot", valid_modes->VmxRoot,
                         "VmxNonRoot", valid_modes->VmxNonRoot,
                         "VmxOff", valid_modes->VmxOff);
}


static PyObject *_pybddisasm_build_valid_prefixes(ND_VALID_PREFIXES *valid_prefixes)
{
    return Py_BuildValue("{"
                         "s,H,"         // Raw
                         "s,B,"         // Rep,
                         "s,B,"         // RepCond,
                         "s,B,"         // Lock,
                         "s,B,"         // Hle,
                         "s,B,"         // Xacquire,
                         "s,B,"         // Xrelease,
                         "s,B,"         // Bnd,
                         "s,B,"         // Bhint,
                         "s,B,"         // HleNoLock,
                         "s,B"          // Dnt,
                         "}",

                         "Raw", valid_prefixes->Raw,
                         "Rep", valid_prefixes->Rep,
                         "RepCond", valid_prefixes->RepCond,
                         "Lock", valid_prefixes->Lock,
                         "Hle", valid_prefixes->Hle,
                         "Xacquire", valid_prefixes->Xacquire,
                         "Xrelease", valid_prefixes->Xrelease,
                         "Bnd", valid_prefixes->Bnd,
                         "Bhint", valid_prefixes->Bhint,
                         "HleNoLock", valid_prefixes->HleNoLock,
                         "Dnt", valid_prefixes->Dnt);
}


static PyObject *_pybddisasm_build_valid_decorators(ND_VALID_DECORATORS *valid_deorators)
{
    return Py_BuildValue("{"
                         "s,H,"         // Raw
                         "s,B,"         // Er
                         "s,B,"         // Sae
                         "s,B,"         // Zero
                         "s,B,"         // Mask
                         "s,B"          // Broadcast
                         "}",

                         "Raw", valid_deorators->Raw,
                         "Er", valid_deorators->Er,
                         "Sae", valid_deorators->Sae,
                         "Zero", valid_deorators->Zero,
                         "Mask", valid_deorators->Mask,
                         "Broadcast", valid_deorators->Broadcast);
}


static PyObject *_pybddisasm_build_instr_dict(INSTRUX *instr, uint64_t rip)
{
    PyObject *nd_rex = _pybddisasm_build_rex(&instr->Rex);

    PyObject *nd_modrm = _pybddisasm_build_modrm(&instr->ModRm);

    PyObject *nd_sib = _pybddisasm_build_sib(&instr->Sib);

    PyObject *nd_drex = _pybddisasm_build_drex(&instr->Drex);

    PyObject *nd_op_array = _pybddisasm_build_operands(instr->Operands, instr->OperandsCount);

    PyObject *nd_exs = _pybddisasm_build_exs(instr);

    PyObject *nd_address = _pybddisasm_build_address(instr);

    PyObject *nd_cpuid_flag = _pybddisasm_build_cpuid_flag(&instr->CpuidFlag);

    PyObject *nd_valid_modes = _pybddisasm_build_valid_modes(&instr->ValidModes);
    PyObject *nd_valid_prefixes = _pybddisasm_build_valid_prefixes(&instr->ValidPrefixes);
    PyObject *nd_valid_decorators = _pybddisasm_build_valid_decorators(&instr->ValidDecorators);

    char instr_text[ND_MIN_BUF_SIZE] = {0};
    NDSTATUS status = nd_to_text(instr, rip, sizeof(instr_text), instr_text);
    if (!ND_SUCCESS(status))
    {
        strcpy(instr_text, "<error decoding>");
    }

    PyObject *nd_flags_access = _pybddisasm_build_flags_access(instr);

    PyObject *p = Py_BuildValue("{"
                                "s,B,"         // DefCode
                                "s,B,"         // DefData
                                "s,B,"         // DefStack
                                "s,B,"         // EncMode
                                "s,B,"         // VexMode
                                "s,B,"         // AddrMode,
                                "s,B,"         // OpMode,
                                "s,B,"         // EfOpMode,
                                "s,B,"         // VecMode,
                                "s,B,"         // EfVecMode,

                                "s,B,"         // HasRex,
                                "s,B,"         // HasVex,
                                "s,B,"         // HasXop,
                                "s,B,"         // HasEvex,
                                "s,B,"         // HasOpSize,
                                "s,B,"         // HasAddrSize,
                                "s,B,"         // HasLock,
                                "s,B,"         // HasRepnzXacquireBnd,
                                "s,B,"         // HasRepRepzXrelease,
                                "s,B,"         // HasSeg,

                                "s,B,"         // IsRepeated,
                                "s,B,"         // IsXacquireEnabled,
                                "s,B,"         // IsXreleaseEnabled,
                                "s,B,"         // IsRipRelative,
                                "s,B,"         // IsCetTracked,

                                "s,B,"         // HasModRm,
                                "s,B,"         // HasSib,
                                "s,B,"         // HasDrex,
                                "s,B,"         // HasDisp,
                                "s,B,"         // HasAddr,
                                "s,B,"         // HasMoffset,
                                "s,B,"         // HasImm1,
                                "s,B,"         // HasImm2,
                                "s,B,"         // HasImm3,
                                "s,B,"         // HasRelOffs,
                                "s,B,"         // HasSseImm,
                                "s,B,"         // HasCompDisp,
                                "s,B,"         // HasBroadcast,
                                "s,B,"         // HasMask,
                                "s,B,"         // HasZero,
                                "s,B,"         // HasEr,
                                "s,B,"         // HasSae,

                                "s,B,"         // SignDisp,

                                "s,B,"         // HasMandatory66,
                                "s,B,"         // HasMandatoryF2,
                                "s,B,"         // HasMandatoryF3,

                                "s,B,"         // Length,

                                "s,B,"         // WordLength,
                                "s,B,"         // PrefLength,

                                "s,B,"         // OpLength,
                                "s,B,"         // DispLength,
                                "s,B,"         // AddrLength,
                                "s,B,"         // MoffsetLength,
                                "s,B,"         // Imm1Length,
                                "s,B,"         // Imm2Length,
                                "s,B,"         // Imm3Length,
                                "s,B,"         // RelOffsLength,

                                "s,B,"          // OpOffset,
                                "s,B,"          // MainOpOffset,
                                "s,B,"          // DispOffset,
                                "s,B,"          // AddrOffset,
                                "s,B,"          // MoffsetOffset,
                                "s,B,"          // Imm1Offset,
                                "s,B,"          // Imm2Offset,
                                "s,B,"          // Imm3Offset,

                                "s,B,"         // RelOffsOffset,
                                "s,B,"         // SseImmOffset,
                                "s,B,"         // ModRmOffset,

                                "s,B,"         // StackWords,

                                "s,B,"         // Rep,
                                "s,B,"         // Seg,

                                "s,O,"         // Rex,
                                "s,O,"         // ModRm,
                                "s,O,"         // Sib,
                                "s,O,"         // Drex,

                                "s,O,"         // Exs,

                                "s,O,"         // Address,

                                "s,K,"         // Moffset,
                                "s,I,"         // Displacement,
                                "s,I,"         // RelativeOffset,
                                "s,K,"         // Immediate1,
                                "s,B,"         // Immediate2,
                                "s,B,"         // Immediate3,
                                "s,B,"         // SseImmediate,
                                "s,B,"         // SseCondition,
                                "s,B,"         // Condition,
                                "s,B,"         // Predicate,

                                "s,B,"         // OperandsCount,
                                "s,B,"         // ExpOperandsCount,

                                "s,H,"         // OperandsEncodingMap,
                                "s,O,"         // Operands,

                                "s,B,"         // RipAccess,
                                "s,B,"         // StackAccess,
                                "s,B,"         // MemoryAccess,

                                "s,O,"         // FlagsAccess,

                                "s,s,"         // TupleType,
                                "s,s,"         // RoundingMode,

                                "s,I,"         // Attributes,

                                "s,I,"         // Iclass,
                                "s,I,"         // Category,
                                "s,I,"         // IsaSet,
                                "s,O,"         // CpuidFlag,

                                "s,O,"         // ValidModes,
                                "s,O,"         // ValidPrefixes,
                                "s,O,"         // ValidDecorators,

                                "s,s,"         // Mnemonic,
                                "s,y#,"        // OpCodeBytes,
                                "s,B,"         // PrimaryOpCode,
                                "s,y#,"        // InstructionBytes,
                                "s,s"          // NdToText
                                "}",

                                "DefCode", instr->DefCode,
                                "DefData", instr->DefData,
                                "DefStack", instr->DefStack,
                                "EncMode", instr->EncMode,
                                "VexMode", instr->VexMode,
                                "AddrMode", instr->AddrMode,
                                "OpMode", instr->OpMode,
                                "EfOpMode", instr->EfOpMode,
                                "VecMode", instr->VecMode,
                                "EfVecMode", instr->EfVecMode,

                                "HasRex", instr->HasRex,
                                "HasVex", instr->HasVex,
                                "HasXop", instr->HasXop,
                                "HasEvex", instr->HasEvex,
                                "HasOpSize", instr->HasOpSize,
                                "HasAddrSize", instr->HasAddrSize,
                                "HasLock", instr->HasLock,
                                "HasRepnzXacquireBnd", instr->HasRepnzXacquireBnd,
                                "HasRepRepzXrelease", instr->HasRepRepzXrelease,
                                "HasSeg", instr->HasSeg,

                                "IsRepeated", instr->IsRepeated,
                                "IsXacquireEnabled", instr->IsXacquireEnabled,
                                "IsXreleaseEnabled", instr->IsXreleaseEnabled,
                                "IsRipRelative", instr->IsRipRelative,
                                "IsCetTracked", instr->IsCetTracked,

                                "HasModRm", instr->HasModRm,
                                "HasSib", instr->HasSib,
                                "HasDrex", instr->HasDrex,
                                "HasDisp", instr->HasDisp,
                                "HasAddr", instr->HasAddr,
                                "HasMoffset", instr->HasMoffset,
                                "HasImm1", instr->HasImm1,
                                "HasImm2", instr->HasImm2,
                                "HasImm3", instr->HasImm3,
                                "HasRelOffs", instr->HasRelOffs,
                                "HasSseImm", instr->HasSseImm,
                                "HasCompDisp", instr->HasCompDisp,
                                "HasBroadcast", instr->HasBroadcast,
                                "HasMask", instr->HasMask,
                                "HasZero", instr->HasZero,
                                "HasEr", instr->HasEr,
                                "HasSae", instr->HasSae,

                                "SignDisp", instr->SignDisp,

                                "HasMandatory66", instr->HasMandatory66,
                                "HasMandatoryF2", instr->HasMandatoryF2,
                                "HasMandatoryF3", instr->HasMandatoryF3,

                                "Length", instr->Length,

                                "WordLength", instr->WordLength,
                                "PrefLength", instr->PrefLength,

                                "OpLength", instr->OpLength,
                                "DispLength", instr->DispLength,
                                "AddrLength", instr->AddrLength,
                                "MoffsetLength", instr->MoffsetLength,
                                "Imm1Length", instr->Imm1Length,
                                "Imm2Length", instr->Imm2Length,
                                "Imm3Length", instr->Imm3Length,
                                "RelOffsLength", instr->RelOffsLength,

                                "OpOffset", instr->OpOffset,
                                "MainOpOffset", instr->MainOpOffset,
                                "DispOffset", instr->DispOffset,
                                "AddrOffset", instr->AddrOffset,
                                "MoffsetOffset", instr->MoffsetOffset,
                                "Imm1Offset", instr->Imm1Offset,
                                "Imm2Offset", instr->Imm2Offset,
                                "Imm3Offset", instr->Imm3Offset,

                                "RelOffsOffset", instr->RelOffsOffset,
                                "SseImmOffset", instr->SseImmOffset,
                                "ModRmOffset", instr->ModRmOffset,

                                "StackWords", instr->StackWords,

                                "Rep", instr->Rep,
                                "Seg", instr->Seg,
                                "Rex", nd_rex,
                                "ModRm", nd_modrm,
                                "Sib", nd_sib,
                                "Drex", nd_drex,

                                "Exs", nd_exs,

                                "Address", nd_address,

                                "Moffset", instr->Moffset,
                                "Displacement", instr->Displacement,
                                "RelativeOffset", instr->RelativeOffset,
                                "Immediate1", instr->Immediate1,
                                "Immediate2", instr->Immediate2,
                                "Immediate3", instr->Immediate3,
                                "SseImmediate", instr->SseImmediate,
                                "SseCondition", instr->SseCondition,
                                "Condition", instr->Condition,
                                "Predicate", instr->Predicate,

                                "OperandsCount", instr->OperandsCount,
                                "ExpOperandsCount", instr->ExpOperandsCount,

                                "OperandsEncodingMap", instr->OperandsEncodingMap,
                                "Operands", nd_op_array,

                                "RipAccess", instr->RipAccess,
                                "StackAccess", instr->StackAccess,
                                "MemoryAccess", instr->MemoryAccess,

                                "FlagsAccess", nd_flags_access,

                                "TupleType", TupleTypeToString(instr->TupleType),
                                "RoundingMode", RoundingModeToString(instr->RoundingMode),

                                "Attributes", instr->Attributes,

                                "Iclass", instr->Iclass,
                                "Category", instr->Category,
                                "IsaSet", instr->IsaSet,
                                "CpuidFlag", nd_cpuid_flag,

                                "ValidModes", nd_valid_modes,
                                "ValidPrefixes", nd_valid_prefixes,
                                "ValidDecorators", nd_valid_decorators,

                                "Mnemonic", instr->Mnemonic,
                                "OpCodeBytes", instr->OpCodeBytes, sizeof(instr->OpCodeBytes),
                                "PrimaryOpCode", instr->PrimaryOpCode,
                                "InstructionBytes", instr->InstructionBytes, instr->Length,
                                "Text", instr_text);

    Py_XDECREF(nd_rex);
    Py_XDECREF(nd_modrm);
    Py_XDECREF(nd_sib);
    Py_XDECREF(nd_drex);
    Py_XDECREF(nd_op_array);
    Py_XDECREF(nd_exs);
    Py_XDECREF(nd_address);
    Py_XDECREF(nd_cpuid_flag);
    Py_XDECREF(nd_valid_modes);
    Py_XDECREF(nd_valid_prefixes);
    Py_XDECREF(nd_valid_decorators);
    Py_XDECREF(nd_flags_access);

    return p;
}


static uint8_t _pybddisasm_py_code_to_disasm(uint8_t code)
{
    if (16 == code)
    {
        return ND_CODE_16;
    }
    else if (32 == code)
    {
        return ND_CODE_32;
    }
    else if (64 == code)
    {
        return ND_CODE_64;
    }

    PyErr_Format(PyExc_ValueError, "Invalid code: %d (expected 16|32|64)\n", code);
    return 0;
}


static uint8_t _pybddisasm_py_data_to_disasm(uint8_t data)
{
    if (16 == data)
    {
        return ND_DATA_16;
    }
    else if (32 == data)
    {
        return ND_DATA_32;
    }
    else if (64 == data)
    {
        return ND_DATA_64;
    }

    PyErr_Format(PyExc_ValueError, "Invalid data: %d (expected 16|32|64)\n", data);
    return 0;
}


static uint8_t _pybddisasm_py_stack_to_disasm(uint8_t stack)
{
    if (16 == stack)
    {
        return ND_STACK_16;
    }
    else if (32 == stack)
    {
        return ND_STACK_32;
    }
    else if (64 == stack)
    {
        return ND_STACK_64;
    }

    PyErr_Format(PyExc_ValueError, "Invalid stack: %d (expected 16|32|64)\n", stack);
    return 0;
}


static char *_pybddisasm_get_contiguous_buffer(Py_buffer *view)
{
    char *buffer = NULL;

    if (PyBuffer_IsContiguous(view, 'C'))
    {
        return view->buf;
    }

    buffer = (char *)malloc(sizeof(char) * view->len);
    if (!buffer)
    {
        PyErr_SetString(PyExc_ValueError, "failed to allocate memory for contiguous buffer!");
        return NULL;
    }

    if (PyBuffer_ToContiguous(buffer, view, view->len, 'C') < 0)
    {
        PyErr_SetString(PyExc_ValueError, "PyBuffer_ToContiguous failed!");
        free(buffer);
        return NULL;
    }

    return buffer;
}


static void _pybddisasm_release_contiguous_buffer(Py_buffer *view, char *buffer)
{
    if (!PyBuffer_IsContiguous(view, 'C'))
    {
        free(buffer);
    }
}


static PyObject *pybddisasm_decode_ex(PyObject *self, PyObject *args)
{
    char *buffer = NULL;
    uint64_t rip = 0;
    uint8_t code, data;
    Py_buffer view;

    (void)self;

    if (!PyArg_ParseTuple(args, "y*BB|K", &view, &code, &data, &rip))
    {
        PyErr_SetString(PyExc_ValueError, "invalid arguments. expected: <buffer[byte-like object]> <code> <data> <rip>");
        Py_RETURN_NONE;
    }

    buffer = _pybddisasm_get_contiguous_buffer(&view);
    if (!buffer)
    {
        Py_RETURN_NONE;
    }

    code = _pybddisasm_py_code_to_disasm(code);
    data = _pybddisasm_py_data_to_disasm(data);

    if (PyErr_Occurred())
    {
        Py_RETURN_NONE;
    }

    INSTRUX instr;

    NDSTATUS status = nd_decode_ex(&instr, buffer, view.len, code, data);
    if (!ND_SUCCESS(status))
    {
        Py_RETURN_NONE;
    }

    _pybddisasm_release_contiguous_buffer(&view, buffer);
    PyBuffer_Release(&view);

    return _pybddisasm_build_instr_dict(&instr, rip);
}


static PyObject *pybddisasm_decode_ex2(PyObject *self, PyObject *args)
{
    char *str_vend = NULL;
    char *buffer = NULL;
    uint64_t rip = 0;
    uint8_t code, data, stack, vend;
    Py_buffer view;

    (void)self;

    if (!PyArg_ParseTuple(args, "y*BBBs|K", &view, &code, &data, &stack, &str_vend, &rip))
    {
        PyErr_SetString(PyExc_ValueError, "invalid arguments. expected: <buffer[byte-like object]> <code> <data> <stack> <vendor> <rip>");
        Py_RETURN_NONE;
    }

    buffer = _pybddisasm_get_contiguous_buffer(&view);
    if (!buffer)
    {
        Py_RETURN_NONE;
    }

    if (0 == strcmp(str_vend, "any"))
    {
        vend = ND_VEND_ANY;
    }
    else if (0 == strcmp(str_vend, "intel"))
    {
        vend = ND_VEND_INTEL;
    }
    else if (0 == strcmp(str_vend, "amd"))
    {
        vend = ND_VEND_AMD;
    }
    else if (0 == strcmp(str_vend, "geode"))
    {
        vend = ND_VEND_GEODE;
    }
    else if (0 == strcmp(str_vend, "cyrix"))
    {
        vend = ND_VEND_CYRIX;
    }
    else
    {
        PyErr_Format(PyExc_ValueError, "invalid vendor: '%s'", str_vend);
        Py_RETURN_NONE;
    }

    code = _pybddisasm_py_code_to_disasm(code);
    data = _pybddisasm_py_data_to_disasm(data);
    stack = _pybddisasm_py_stack_to_disasm(stack);

    if (PyErr_Occurred())
    {
        Py_RETURN_NONE;
    }

    INSTRUX instr;

    NDSTATUS status = nd_decode_ex2(&instr, buffer, view.len, code, data, stack, vend);
    if (!ND_SUCCESS(status))
    {
        Py_RETURN_NONE;
    }

    _pybddisasm_release_contiguous_buffer(&view, buffer);
    PyBuffer_Release(&view);

    return _pybddisasm_build_instr_dict(&instr, rip);
}


static PyObject *pybddisasm_to_text(PyObject *self, PyObject *args)
{
    char *buffer = NULL;
    uint64_t rip = 0;
    uint8_t code, data;
    Py_buffer view;

    (void)self;

    if (!PyArg_ParseTuple(args, "y*BB|K", &view, &code, &data, &rip))
    {
        PyErr_SetString(PyExc_ValueError, "invalid arguments. expected: <buffer[byte-like object]>  <code> <data> <rip>");
        Py_RETURN_NONE;
    }

    buffer = _pybddisasm_get_contiguous_buffer(&view);
    if (!buffer)
    {
        Py_RETURN_NONE;
    }

    code = _pybddisasm_py_code_to_disasm(code);
    data = _pybddisasm_py_data_to_disasm(data);

    if (PyErr_Occurred())
    {
        Py_RETURN_NONE;
    }

    INSTRUX instr;

    NDSTATUS status = nd_decode_ex(&instr, buffer, view.len, code, data);
    if (!ND_SUCCESS(status))
    {
        Py_RETURN_NONE;
    }

    char instr_text[ND_MIN_BUF_SIZE] = {0};

    status = nd_to_text(&instr, rip, sizeof(instr_text), instr_text);
    if (!ND_SUCCESS(status))
    {
        Py_RETURN_NONE;
    }

    _pybddisasm_release_contiguous_buffer(&view, buffer);
    PyBuffer_Release(&view);

    return Py_BuildValue("{s,s,s,y#}", "text", instr_text, "bytes", instr.InstructionBytes, instr.Length);
}


static PyMethodDef module_methods[] = {
    {"nd_decode_ex", pybddisasm_decode_ex, METH_VARARGS, pybddisasm_decode_ex_docstring},
    {"nd_decode_ex2", pybddisasm_decode_ex2, METH_VARARGS, pybddisasm_decode_ex2_docstring},
    {"nd_decode", pybddisasm_decode_ex, METH_VARARGS, pybddisasm_decode_docstring},
    {"nd_to_text", pybddisasm_to_text, METH_VARARGS, pybddisasm_to_text_docstring},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef pybddisasm =
{
    PyModuleDef_HEAD_INIT,
    "pybddisasm",                               // name of module
    module_docstring,
    -1,                                    // size of per-interpreter state of the module, or -1 if the module keeps state in global variables
    module_methods
};

PyMODINIT_FUNC PyInit__pybddisasm(void)
{
    static_assert(sizeof(INSTRUX) == LIBRARY_INSTRUX_SIZE, "The size of INSTRUX is not compatible with pybddisasm!");

    return PyModule_Create(&pybddisasm);
}
