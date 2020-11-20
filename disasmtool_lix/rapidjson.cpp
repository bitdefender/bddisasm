/*
 * Copyright (c) 2020 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
#include "disasm.hpp"

#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/reader.h>


using namespace rapidjson;


static void _start_object(Writer<StringBuffer> &writer)
{
    writer.StartObject();
}


static void _start_object(Writer<StringBuffer> &writer, const char *key)
{
    writer.Key(key);
    writer.StartObject();
}

static void _end_object(Writer<StringBuffer> &writer)
{
    writer.EndObject();
}


static void _start_array(Writer<StringBuffer> &writer)
{
    writer.StartArray();
}


static void _start_array(Writer<StringBuffer> &writer, const char *key)
{
    writer.Key(key);
    writer.StartArray();
}

static void _end_array(Writer<StringBuffer> &writer)
{
    writer.EndArray();
}


static void _write_uint(Writer<StringBuffer> &writer, unsigned int value)
{
    writer.Uint(value);
}


static void _write_uint(Writer<StringBuffer> &writer, const char *key, unsigned int value)
{
    writer.Key(key);
    writer.Uint(value);
}


static void _write_int(Writer<StringBuffer> &writer, int value)
{
    writer.Int(value);
}


static void _write_int(Writer<StringBuffer> &writer, const char *key, int value)
{
    writer.Key(key);
    writer.Int(value);
}


static void _write_uint64(Writer<StringBuffer> &writer, uint64_t value)
{
    writer.Int(value);
}


static void _write_uint64(Writer<StringBuffer> &writer, const char *key, uint64_t value)
{
    writer.Key(key);
    writer.Int(value);
}


static void _write_int64(Writer<StringBuffer> &writer, int64_t value)
{
    writer.Int(value);
}


static void _write_int64(Writer<StringBuffer> &writer, const char *key, int64_t value)
{
    writer.Key(key);
    writer.Int(value);
}


static void _write_string(Writer<StringBuffer> &writer, const std::string& value)
{
     writer.String(value.c_str());
}


static void _write_string(Writer<StringBuffer> &writer, const char *key, const std::string& value)
{
    writer.Key(key);
    writer.String(value.c_str());
}


static void _write_string(Writer<StringBuffer> &writer, const char *value)
{
    writer.String(value);
}


static void _write_string(Writer<StringBuffer> &writer, const char *key, const char *value)
{
    writer.Key(key);
    writer.String(value);
}


static void _write_bool(Writer<StringBuffer> &writer, bool value)
{
    writer.Bool(value);
}


static void _write_bool(Writer<StringBuffer> &writer, const char *key, bool value)
{
    writer.Key(key);
    writer.Bool(value);
}


static int _disasm_size_to_int(const uint8_t mode)
{
    switch (mode) {
    case 0: return 16;
    case 1: return 32;
    case 2: return 64;
    }

    return mode;
}


static void _write_rex(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    if (!instrux->HasRex)
        return;

    _start_object(writer, "rex");

    _write_uint(writer, "value", instrux->Rex.Rex);

    _write_uint(writer, "b", static_cast<uint8_t>(instrux->Rex.b));
    _write_uint(writer, "x", static_cast<uint8_t>(instrux->Rex.x));
    _write_uint(writer, "r", static_cast<uint8_t>(instrux->Rex.r));
    _write_uint(writer, "w", static_cast<uint8_t>(instrux->Rex.w));

    _end_object(writer);
}


static void _write_modrm(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    if (!instrux->HasModRm)
        return;

    _start_object(writer, "modrm");

    _write_uint(writer, "value", instrux->ModRm.ModRm);
    _write_uint(writer, "rm", static_cast<uint8_t>(instrux->ModRm.rm));
    _write_uint(writer, "reg", static_cast<uint8_t>(instrux->ModRm.reg));
    _write_uint(writer, "mod", static_cast<uint8_t>(instrux->ModRm.mod));

    _end_object(writer);
}


static void _write_sib(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    if (!instrux->HasSib)
        return;

    _start_object(writer, "sib");

    _write_uint(writer, "value", instrux->Sib.Sib);
    _write_uint(writer, "base", static_cast<uint8_t>(instrux->Sib.base));
    _write_uint(writer, "index", static_cast<uint8_t>(instrux->Sib.index));
    _write_uint(writer, "scale", static_cast<uint8_t>(instrux->Sib.scale));

    _end_object(writer);
}


static void _write_drex(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    if (!instrux->HasDrex)
        return;

    _start_object(writer, "drex");

    _write_uint(writer, "value", instrux->Drex.Drex);
    _write_uint(writer, "b", static_cast<uint8_t>(instrux->Drex.b));
    _write_uint(writer, "x", static_cast<uint8_t>(instrux->Drex.x));
    _write_uint(writer, "r", static_cast<uint8_t>(instrux->Drex.r));
    _write_uint(writer, "oc", static_cast<uint8_t>(instrux->Drex.oc0));
    _write_uint(writer, "vd", static_cast<uint8_t>(instrux->Drex.vd));
    _write_uint(writer, "d", static_cast<uint8_t>(instrux->Drex.d));

    _end_object(writer);
}


static void _write_vex2(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    if (!instrux->HasVex || ND_VEXM_2B != instrux->VexMode)
        return;

    _start_object(writer, "vex2");

    _start_array(writer, "value");
    _write_uint(writer, instrux->Vex2.Vex[0]);
    _write_uint(writer, instrux->Vex2.Vex[1]);
    _end_array(writer);

    _write_uint(writer, "op", instrux->Vex2.op);
    _write_uint(writer, "p", static_cast<uint8_t>(instrux->Vex2.p));
    _write_uint(writer, "l", static_cast<uint8_t>(instrux->Vex2.l));
    _write_uint(writer, "v", static_cast<uint8_t>(instrux->Vex2.v));
    _write_uint(writer, "r", static_cast<uint8_t>(instrux->Vex2.r));

    _end_object(writer);
}


static void _write_vex3(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    if (!instrux->HasVex || ND_VEXM_3B != instrux->VexMode)
        return;

    _start_object(writer, "vex3");

    _start_array(writer, "value");
    _write_uint(writer, instrux->Vex3.Vex[0]);
    _write_uint(writer, instrux->Vex3.Vex[1]);
    _write_uint(writer, instrux->Vex3.Vex[2]);
    _end_array(writer);

    _write_uint(writer, "op", instrux->Vex3.op);
    _write_uint(writer, "m", static_cast<uint8_t>(instrux->Vex3.m));
    _write_uint(writer, "b", static_cast<uint8_t>(instrux->Vex3.b));
    _write_uint(writer, "x", static_cast<uint8_t>(instrux->Vex3.x));
    _write_uint(writer, "r", static_cast<uint8_t>(instrux->Vex3.r));
    _write_uint(writer, "p", static_cast<uint8_t>(instrux->Vex3.p));
    _write_uint(writer, "l", static_cast<uint8_t>(instrux->Vex3.l));
    _write_uint(writer, "v", static_cast<uint8_t>(instrux->Vex3.v));
    _write_uint(writer, "w", static_cast<uint8_t>(instrux->Vex3.w));

    _end_object(writer);
}


static void _write_xop(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    if (!instrux->HasXop)
        return;

    _start_object(writer, "xop");

    _start_array(writer, "value");
    _write_uint(writer, instrux->Xop.Xop[0]);
    _write_uint(writer, instrux->Xop.Xop[1]);
    _write_uint(writer, instrux->Xop.Xop[2]);
    _end_array(writer);

    _write_uint(writer, "op", instrux->Xop.op);
    _write_uint(writer, "m", static_cast<uint8_t>(instrux->Xop.m));
    _write_uint(writer, "b", static_cast<uint8_t>(instrux->Xop.b));
    _write_uint(writer, "x", static_cast<uint8_t>(instrux->Xop.x));
    _write_uint(writer, "r", static_cast<uint8_t>(instrux->Xop.r));
    _write_uint(writer, "p", static_cast<uint8_t>(instrux->Xop.p));
    _write_uint(writer, "l", static_cast<uint8_t>(instrux->Xop.l));
    _write_uint(writer, "v", static_cast<uint8_t>(instrux->Xop.v));
    _write_uint(writer, "w", static_cast<uint8_t>(instrux->Xop.w));

    _end_object(writer);
}


static void _write_evex(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    if (!instrux->HasEvex)
        return;

    _start_object(writer, "evex");

    _start_array(writer, "value");
    _write_uint(writer, instrux->Evex.Evex[0]);
    _write_uint(writer, instrux->Evex.Evex[1]);
    _write_uint(writer, instrux->Evex.Evex[2]);
    _write_uint(writer, instrux->Evex.Evex[3]);
    _end_array(writer);

    _write_uint(writer, "op", instrux->Evex.op);
    _write_uint(writer, "m", static_cast<uint8_t>(instrux->Evex.m));
    _write_uint(writer, "zero", static_cast<uint8_t>(instrux->Evex.zero));
    _write_uint(writer, "rp", static_cast<uint8_t>(instrux->Evex.rp));
    _write_uint(writer, "b", static_cast<uint8_t>(instrux->Evex.b));
    _write_uint(writer, "x", static_cast<uint8_t>(instrux->Evex.x));
    _write_uint(writer, "r", static_cast<uint8_t>(instrux->Evex.r));
    _write_uint(writer, "p", static_cast<uint8_t>(instrux->Evex.p));
    _write_uint(writer, "one", static_cast<uint8_t>(instrux->Evex.one));
    _write_uint(writer, "v", static_cast<uint8_t>(instrux->Evex.v));
    _write_uint(writer, "w", static_cast<uint8_t>(instrux->Evex.w));
    _write_uint(writer, "a", static_cast<uint8_t>(instrux->Evex.a));
    _write_uint(writer, "vp", static_cast<uint8_t>(instrux->Evex.vp));
    _write_uint(writer, "bm", static_cast<uint8_t>(instrux->Evex.bm));
    _write_uint(writer, "l", static_cast<uint8_t>(instrux->Evex.l));
    _write_uint(writer, "z", static_cast<uint8_t>(instrux->Evex.z));

    _end_object(writer);
}


static void _write_exs(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    _start_object(writer, "exs");

    _write_uint(writer, "w", static_cast<uint8_t>(instrux->Exs.w));
    _write_uint(writer, "r", static_cast<uint8_t>(instrux->Exs.r));
    _write_uint(writer, "x", static_cast<uint8_t>(instrux->Exs.x));
    _write_uint(writer, "b", static_cast<uint8_t>(instrux->Exs.b));
    _write_uint(writer, "rp", static_cast<uint8_t>(instrux->Exs.rp));
    _write_uint(writer, "p", static_cast<uint8_t>(instrux->Exs.p));
    _write_uint(writer, "m", static_cast<uint8_t>(instrux->Exs.m));
    _write_uint(writer, "l", static_cast<uint8_t>(instrux->Exs.l));
    _write_uint(writer, "v", static_cast<uint8_t>(instrux->Exs.v));
    _write_uint(writer, "vp", static_cast<uint8_t>(instrux->Exs.vp));
    _write_uint(writer, "bm", static_cast<uint8_t>(instrux->Exs.bm));
    _write_uint(writer, "e", static_cast<uint8_t>(instrux->Exs.e));
    _write_uint(writer, "z", static_cast<uint8_t>(instrux->Exs.z));
    _write_uint(writer, "k", static_cast<uint8_t>(instrux->Exs.k));
    _write_uint(writer, "s", static_cast<uint8_t>(instrux->Exs.s));

    _end_object(writer);
}


static void _write_prefixes(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    _start_array(writer, "prefixes");

    if (instrux->HasRex)
        _write_string(writer, "rex");

    if (instrux->HasVex)
        _write_string(writer, "vex");

    if (instrux->HasXop)
        _write_string(writer, "xop");

    if (instrux->HasEvex)
        _write_string(writer, "evex");

    if (instrux->HasMvex)
        _write_string(writer, "mvex");

    if (instrux->HasOpSize)
        _write_string(writer, "op_size");

    if (instrux->HasAddrSize)
        _write_string(writer, "addr_size");

    if (instrux->HasLock)
        _write_string(writer, "lock");

    if (instrux->HasRepnzXacquireBnd)
        _write_string(writer, "repnz_xacquire_bnd");

    if (instrux->HasRepRepzXrelease)
        _write_string(writer, "rep_repz_xrelease");

    if (instrux->HasSeg)
        _write_string(writer, "seg");

    _end_array(writer);
}


static void _write_chunks(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    _start_array(writer, "chunks");

    if (instrux->HasModRm)
        _write_string(writer, "mod_rm");

    if (instrux->HasSib)
        _write_string(writer, "sib");

    if (instrux->HasDrex)
        _write_string(writer, "drex");

    if (instrux->HasDisp)
        _write_string(writer, "disp");

    if (instrux->HasAddr)
        _write_string(writer, "addr");

    if (instrux->HasMoffset)
        _write_string(writer, "moffset");

    if (instrux->HasImm1)
        _write_string(writer, "imm1");

    if (instrux->HasImm2)
        _write_string(writer, "imm2");

    if (instrux->HasImm3)
        _write_string(writer, "imm3");

    if (instrux->HasRelOffs)
        _write_string(writer, "rel_offs");

    if (instrux->HasSseImm)
        _write_string(writer, "sse_imm");

    if (instrux->HasCompDisp)
        _write_string(writer, "comp_disp");

    if (instrux->HasBroadcast)
        _write_string(writer, "broadcast");

    if (instrux->HasMask)
        _write_string(writer, "mask");

    if (instrux->HasZero)
        _write_string(writer, "zero");

    if (instrux->HasEr)
        _write_string(writer, "er");

    if (instrux->HasSae)
        _write_string(writer, "sae");

    if (instrux->SignDisp)
        _write_string(writer, "sign_disp");

    _end_array(writer);
}


static void _write_access(Writer<StringBuffer> &writer, const char *key, const uint8_t access)
{
    _start_array(writer, key);

    if (access & ND_ACCESS_READ)
        _write_string(writer, "read");

    if (access & ND_ACCESS_WRITE)
        _write_string(writer, "write");

    if (access & ND_ACCESS_COND_READ)
        _write_string(writer, "cond_read");

    if (access & ND_ACCESS_COND_WRITE)
        _write_string(writer, "cond_write");

    if (access == ND_ACCESS_NONE)
        _write_string(writer, "none");

    _end_array(writer);
}


static void _start_write_default_op(Writer<StringBuffer> &writer, const ND_OPERAND &op)
{
    _start_object(writer);

    _start_array(writer, "flags");

    if (op.Flags.IsDefault)
        _write_string(writer, "default");

    if (op.Flags.SignExtendedOp1)
        _write_string(writer, "sign_extended_op1");

    if (op.Flags.SignExtendedDws)
        _write_string(writer, "sign_extended_dws");

    _end_array(writer);

    _write_uint(writer, "size", op.Size);
    _write_uint(writer, "raw_size", op.RawSize);

    _write_access(writer, "access", op.Access.Access);

    _start_object(writer, "decorator");

    if (op.Decorator.HasMask)
        _write_uint(writer, "mask", op.Decorator.Mask.Msk); // TODO: k0-k7

    if (op.Decorator.HasZero)
        _write_bool(writer, "zeroing", true);

    if (op.Decorator.HasBroadcast) {
        _start_object(writer, "broadcast");

        _write_uint(writer, "count", op.Decorator.Broadcast.Count);
        _write_uint(writer, "size", op.Decorator.Broadcast.Size);

        _end_object(writer);
    }

    if (op.Decorator.HasEr)
        _write_bool(writer, "er", true);

    if (op.Decorator.HasSae)
        _write_bool(writer, "sae", true);

    _end_object(writer); // "decorator"

    switch (op.Type) {
    case ND_OP_NOT_PRESENT:
        _write_string(writer, "type", "not_preset");
        break;

    case ND_OP_REG:
        _write_string(writer, "type", "register");
        break;

    case ND_OP_MEM:
        _write_string(writer, "type", "memory");
        break;

    case ND_OP_IMM:
        _write_string(writer, "type", "immediate");
        break;

    case ND_OP_OFFS:
        _write_string(writer, "type", "offset");
        break;

    case ND_OP_ADDR:
        _write_string(writer, "type", "address");
        break;

    case ND_OP_CONST:
        _write_string(writer, "type", "constant");
        break;

    case ND_OP_BANK:
        _write_string(writer, "type", "bank");
        break;

    default:
        _write_string(writer, "type", "<unknown>");
        break;
    }

    // returns without closing the object
}


static void _write_op_reg(Writer<StringBuffer> &writer, const ND_OPERAND &op)
{
    const auto& reg = op.Info.Register;

    _write_uint(writer, "size", reg.Size);

    _write_string(writer, "reg_type", reg_type_to_str(reg.Type));
    _write_string(writer, "reg", reg_to_str(reg.Reg, reg.Type));

    _write_uint(writer, "reg_id", reg.Reg);
    _write_uint(writer, "count", reg.Count);

    if (reg.IsHigh8)
        _write_bool(writer, "is_high8", true);

    if (reg.IsBlock)
        _write_bool(writer, "is_block", true);
}


static void _write_op_mem(Writer<StringBuffer> &writer, const ND_OPERAND &op)
{
    const auto& mem = op.Info.Memory;

    _start_array(writer, "flags");

    if (mem.IsRipRel)
        _write_string(writer, "rip_rel");

    if (mem.IsStack)
        _write_string(writer, "stack");

    if (mem.IsString)
        _write_string(writer, "string");

    if (mem.IsShadowStack)
        _write_string(writer, "shadow_stack");

    if (mem.IsDirect)
        _write_string(writer, "direct");

    if (mem.IsBitbase)
        _write_string(writer, "bitbase");

    if (mem.IsAG)
        _write_string(writer, "ag");

    if (mem.IsMib)
        _write_string(writer, "mib");

    if (mem.HasBroadcast)
        _write_string(writer, "broadcast");

    _end_array(writer);

    if (mem.IsVsib) {
        _start_object(writer, "vsib");

        _write_uint(writer, "index_size", mem.Vsib.IndexSize);
        _write_uint(writer, "elem_size", mem.Vsib.ElemSize);
        _write_uint(writer, "elem_count", mem.Vsib.ElemCount);

        _end_object(writer);
    }

    if (mem.HasSeg)
        _write_uint(writer, "seg", mem.Seg);

    if (mem.HasBase) {
        _write_uint(writer, "base_size", mem.BaseSize);
        _write_uint(writer, "base", mem.Base);
    }

    if (mem.HasIndex) {
        _write_uint(writer, "index_size", mem.IndexSize);
        _write_uint(writer, "index", mem.Index);
        _write_uint(writer, "scale", mem.Scale);
    }

    if (mem.HasDisp || mem.HasCompDisp) {
        if (mem.HasDisp)
            _write_uint(writer, "disp_size", mem.DispSize);

        if (mem.HasCompDisp)
            _write_uint(writer, "comp_disp_size", mem.CompDispSize);

        _write_uint(writer, "disp", mem.Disp);
    }
}


static void _write_operands(Writer<StringBuffer> &writer, const INSTRUX *instrux)
{
    _start_array(writer, "operands");

    for (auto i = 0; i < instrux->OperandsCount; i++) {
        const auto& op = instrux->Operands[i];

        _start_write_default_op(writer, op);

        _start_object(writer, "info");

        switch (op.Type) {
        case ND_OP_REG:
            _write_op_reg(writer, op);
            break;

        case ND_OP_MEM:
            _write_op_mem(writer, op);
            break;

        case ND_OP_IMM:
            _write_uint64(writer, "imm", op.Info.Immediate.Imm);
            break;

        case ND_OP_OFFS:
            _write_uint64(writer, "rel", op.Info.RelativeOffset.Rel);
            break;

        case ND_OP_ADDR:
            _write_uint(writer, "base_seg", op.Info.Address.BaseSeg);
            _write_uint64(writer, "offset", op.Info.Address.Offset);

            break;

        case ND_OP_CONST:
            _write_uint64(writer, "const", op.Info.Constant.Const);
            break;

        case ND_OP_NOT_PRESENT:
        case ND_OP_BANK:
            break;
        }

        _end_object(writer); // "info"
        _end_object(writer); // the one from _start_write_default_op
    }

    _end_array(writer);
}


static void _write_valid_modes(Writer<StringBuffer> &writer, const ND_VALID_MODES &modes)
{
    _start_array(writer, "valid_modes");

    if (modes.Ring0)
        _write_string(writer, "ring0");

    if (modes.Ring1)
        _write_string(writer, "ring1");

    if (modes.Ring2)
        _write_string(writer, "ring2");

    if (modes.Ring3)
        _write_string(writer, "ring3");

    if (modes.Real)
        _write_string(writer, "real");

    if (modes.V8086)
        _write_string(writer, "v8086");

    if (modes.Protected)
        _write_string(writer, "protected");

    if (modes.Compat)
        _write_string(writer, "compatibility");

    if (modes.Long)
        _write_string(writer, "long");

    if (modes.Smm)
        _write_string(writer, "smm");

    if (modes.Sgx)
        _write_string(writer, "sgx");

    if (modes.Tsx)
        _write_string(writer, "tsx");

    if (modes.VmxRoot)
        _write_string(writer, "vmx_root");

    if (modes.VmxNonRoot)
        _write_string(writer, "vmx_non_root");

    if (modes.VmxOff)
        _write_string(writer, "vmx_off");

    _end_array(writer);
}


static void _write_valid_prefixes(Writer<StringBuffer> &writer, const ND_VALID_PREFIXES &prefixes)
{
    _start_array(writer, "valid_prefixes");

    if (prefixes.Rep)
        _write_string(writer, "rep");

    if (prefixes.RepCond)
        _write_string(writer, "rep_cond");

    if (prefixes.Lock)
        _write_string(writer, "lock");

    if (prefixes.Hle)
        _write_string(writer, "hle");

    if (prefixes.Xacquire)
        _write_string(writer, "xacquire");

    if (prefixes.Xrelease)
        _write_string(writer, "xrelease");

    if (prefixes.Bnd)
        _write_string(writer, "bnd");

    if (prefixes.Bhint)
        _write_string(writer, "bhint");

    if (prefixes.HleNoLock)
        _write_string(writer, "hle_no_lock");

    if (prefixes.Dnt)
        _write_string(writer, "dnt");

    _end_array(writer);
}


static void _write_valid_decorators(Writer<StringBuffer> &writer, const ND_VALID_DECORATORS &decorators)
{
    _start_array(writer, "valid_decorators");

    if (decorators.Er)
        _write_string(writer, "er");

    if (decorators.Sae)
        _write_string(writer, "Sae");

    if (decorators.Zero)
        _write_string(writer, "Zero");

    if (decorators.Mask)
        _write_string(writer, "mask");

    if (decorators.Broadcast)
        _write_string(writer, "broadcast");

    _end_array(writer);
}


static void _write_cpuid_flag(Writer<StringBuffer> &writer, const ND_CPUID_FLAG &cpuid_flag)
{
    if (0 == cpuid_flag.Flag)
        return;

    _start_object(writer, "cpuid_flag");

    _write_uint64(writer, "flag", cpuid_flag.Flag);
    _write_uint(writer, "leaf", cpuid_flag.Leaf);
    _write_uint(writer, "sub_leaf", static_cast<uint32_t>(cpuid_flag.SubLeaf));
    _write_uint(writer, "reg", static_cast<uint8_t>(cpuid_flag.Reg));
    _write_uint(writer, "bit", static_cast<uint8_t>(cpuid_flag.Bit));

    _end_object(writer);
}


static void _write_rflags(Writer<StringBuffer> &writer, const ND_RFLAGS &rflags, const char *key)
{
    _start_array(writer, key);

    if (rflags.CF)
        _write_string(writer, "cf");

    if (rflags.PF)
        _write_string(writer, "pf");

    if (rflags.AF)
        _write_string(writer, "af");

    if (rflags.ZF)
        _write_string(writer, "zf");

    if (rflags.SF)
        _write_string(writer, "sf");

    if (rflags.TF)
        _write_string(writer, "tf");

    if (rflags.IF)
        _write_string(writer, "if");

    if (rflags.DF)
        _write_string(writer, "df");

    if (rflags.OF)
        _write_string(writer, "of");

    if (rflags.IOPL)
        _write_string(writer, "iopl");

    if (rflags.NT)
        _write_string(writer, "nt");

    if (rflags.RF)
        _write_string(writer, "rf");

    if (rflags.VM)
        _write_string(writer, "vm");

    if (rflags.AC)
        _write_string(writer, "ac");

    if (rflags.VIF)
        _write_string(writer, "vif");

    if (rflags.VIP)
        _write_string(writer, "vip");

    if (rflags.ID)
        _write_string(writer, "id");

    _end_array(writer);
}


static void _write_flags_access(Writer<StringBuffer> &writer, INSTRUX *instrux)
{
    if (ND_ACCESS_NONE == instrux->FlagsAccess.RegAccess)
        return;

    _start_object(writer, "flags_access");

    _write_access(writer, "access", instrux->FlagsAccess.RegAccess);

    if ((instrux->FlagsAccess.RegAccess & ND_ACCESS_ANY_READ) && instrux->FlagsAccess.Tested.Raw)
        _write_rflags(writer, instrux->FlagsAccess.Tested, "tested");

    if ((instrux->FlagsAccess.RegAccess & ND_ACCESS_ANY_WRITE) && instrux->FlagsAccess.Modified.Raw)
        _write_rflags(writer, instrux->FlagsAccess.Modified, "modified");

    if (instrux->FlagsAccess.Set.Raw)
        _write_rflags(writer, instrux->FlagsAccess.Set, "set");

    if (instrux->FlagsAccess.Cleared.Raw)
        _write_rflags(writer, instrux->FlagsAccess.Cleared, "cleared");

    if (instrux->FlagsAccess.Undefined.Raw)
        _write_rflags(writer, instrux->FlagsAccess.Undefined, "undefined");

    _end_object(writer);
}


StringBuffer instrux_to_json(INSTRUX *instrux, size_t rip, bool text_only /* = false */)
{
    char text[ND_MIN_BUF_SIZE];

    auto status = NdToText(instrux, rip, sizeof(text), text);
    if (!ND_SUCCESS(status))
        text[0] = 0;

    auto s = StringBuffer {nullptr, 4096};
    auto writer = Writer<StringBuffer> {s, nullptr};

    _start_object(writer);

    _write_string(writer, "text", text);

    if (text_only) {
        _end_object(writer);
        return s;
    }

    _write_uint(writer, "def_code", _disasm_size_to_int(instrux->DefCode));
    _write_uint(writer, "def_data", _disasm_size_to_int(instrux->DefData));
    _write_uint(writer, "def_stack", _disasm_size_to_int(instrux->DefStack));

    _write_string(writer, "enc_mode", enc_mode_to_str(instrux->EncMode));

    _write_uint(writer, "addr_mode", _disasm_size_to_int(instrux->AddrMode));
    _write_uint(writer, "op_mode", _disasm_size_to_int(instrux->OpMode));
    _write_uint(writer, "ef_op_mode", _disasm_size_to_int(instrux->EfOpMode));
    _write_uint(writer, "vec_mode", _disasm_size_to_int(instrux->VecMode) * 8);
    _write_uint(writer, "ef_vec_mnode", _disasm_size_to_int(instrux->EfVecMode) * 8);

    _write_rex(writer, instrux);
    _write_modrm(writer, instrux);
    _write_sib(writer, instrux);
    _write_drex(writer, instrux);
    _write_vex2(writer, instrux);
    _write_vex3(writer, instrux);
    _write_xop(writer, instrux);
    _write_evex(writer, instrux);

    if (instrux->Rep)
        _write_uint(writer, "rep", instrux->Rep);

    if (instrux->Seg)
        _write_uint(writer, "seg", instrux->Seg);

    _write_uint(writer, "stack_words", instrux->StackWords);

    _write_exs(writer, instrux);
    _write_prefixes(writer, instrux);
    _write_chunks(writer, instrux);

    if (instrux->IsRepeated)
        _write_bool(writer, "is_repeated", true);

    if (instrux->IsXacquireEnabled)
        _write_bool(writer, "is_xacquire_enabled", true);

    if (instrux->IsXreleaseEnabled)
        _write_bool(writer, "is_xrelease_enabled", true);

    if (instrux->IsRipRelative)
        _write_bool(writer, "is_rip_relative", true);

    if (instrux->IsCetTracked)
        _write_bool(writer, "is_cet_tracked", true);

    if (instrux->HasMandatory66)
        _write_bool(writer, "mandatory_66", true);

    if (instrux->HasMandatoryF2)
        _write_bool(writer, "mandatory_f2", true);

    if (instrux->HasMandatoryF3)
        _write_bool(writer, "mandatory_f3", true);

    _write_uint(writer, "length", instrux->Length);
    _write_uint(writer, "word_length", static_cast<uint32_t>(instrux->WordLength));
    _write_uint(writer, "pref_length", static_cast<uint32_t>(instrux->PrefLength));

    _start_object(writer, "address");

    _write_uint(writer, "ip", instrux->Address.Ip);
    _write_uint(writer, "cs", instrux->Address.Cs);

    _end_object(writer);

    _write_uint(writer, "op_offset", static_cast<uint8_t>(instrux->OpOffset));
    _write_uint(writer, "main_op_offset", static_cast<uint8_t>(instrux->MainOpOffset));

    if (instrux->HasAddr) {
        _start_object(writer, "addr");

        _write_uint(writer, "len", static_cast<uint8_t>(instrux->AddrLength));
        _write_uint(writer, "offset", static_cast<uint8_t>(instrux->AddrOffset));

        _end_object(writer);
    }

    if (instrux->HasMoffset) {
        _start_object(writer, "moffset");

        _write_uint64(writer, "value", instrux->Moffset);
        _write_uint(writer, "len", static_cast<uint8_t>(instrux->MoffsetLength));
        _write_uint(writer, "offset", static_cast<uint8_t>(instrux->MoffsetOffset));

        _end_object(writer);
    };

    if (instrux->HasDisp) {
        _start_object(writer, "displacement");

        _write_uint(writer, "value", instrux->Displacement);
        _write_uint(writer, "len", static_cast<uint8_t>(instrux->DispLength));
        _write_uint(writer, "offset", static_cast<uint8_t>(instrux->DispOffset));

        _end_object(writer);
    }

    if (instrux->HasRelOffs) {
        _start_object(writer, "relative_offset");

        _write_uint(writer, "value", instrux->RelativeOffset);
        _write_uint(writer, "len", static_cast<uint8_t>(instrux->RelOffsLength));
        _write_uint(writer, "offset", static_cast<uint8_t>(instrux->RelOffsOffset));

        _end_object(writer);
    }

    if (instrux->HasImm1) {
        _start_object(writer, "immediate1");

        _write_uint64(writer, "value", instrux->Immediate1);
        _write_uint(writer, "len", static_cast<uint8_t>(instrux->Imm1Length));
        _write_uint(writer, "offset", static_cast<uint8_t>(instrux->Imm1Offset));

        _end_object(writer);
    }

    if (instrux->HasImm2) {
        _start_object(writer, "immediate2");

        _write_uint(writer, "value", instrux->Immediate2);
        _write_uint(writer, "len", static_cast<uint8_t>(instrux->Imm2Length));
        _write_uint(writer, "offset", static_cast<uint8_t>(instrux->Imm2Offset));

        _end_object(writer);
    }

    if (instrux->HasImm3) {
        _start_object(writer, "immediate3");

        _write_uint(writer, "value", instrux->Immediate3);
        _write_uint(writer, "len", static_cast<uint8_t>(instrux->Imm3Length));
        _write_uint(writer, "offset", static_cast<uint8_t>(instrux->Imm3Offset));

        _end_object(writer);
    }

    if (instrux->HasSseImm) {
        _start_object(writer, "sse_immediate");

        _write_uint(writer, "value", instrux->SseImmediate);
        _write_uint(writer, "offset", static_cast<uint8_t>(instrux->SseImmOffset));

        _end_object(writer);
    }

    _write_uint(writer, "sse_condition", instrux->SseCondition);
    _write_uint(writer, "condition", instrux->Condition);

    _write_uint(writer, "operands_count", instrux->OperandsCount);
    _write_uint(writer, "exp_operands_count", instrux->ExpOperandsCount);
    _write_uint(writer, "operands_encoding_map", instrux->OperandsEncodingMap);

    _write_operands(writer, instrux);

    _write_access(writer, "rip_access", instrux->RipAccess);
    _write_access(writer, "memory_access", instrux->MemoryAccess);
    _write_access(writer, "stack_access", instrux->StackAccess);

    _write_flags_access(writer, instrux);

    _write_uint(writer, "attributes", instrux->Attributes);

    _write_string(writer, "class", ins_class_to_str(instrux->Iclass));
    _write_string(writer, "category", ins_cat_to_str(instrux->Category));
    _write_string(writer, "set", ins_set_to_str(instrux->IsaSet));

    _write_cpuid_flag(writer, instrux->CpuidFlag);

    _write_valid_modes(writer, instrux->ValidModes);
    _write_valid_prefixes(writer, instrux->ValidPrefixes);
    _write_valid_decorators(writer, instrux->ValidDecorators);

    _write_string(writer, "mnemonic", instrux->Mnemonic);

    _start_array(writer, "opcode_bytes");

    for (int i = 0; i < instrux->OpLength; i++)
        _write_uint(writer, instrux->OpCodeBytes[i]);

    _end_array(writer);

    _write_uint(writer, "primary_opcode", instrux->PrimaryOpCode);

    _start_array(writer, "bytes");

    for (int i = 0; i < instrux->Length; i++)
        _write_uint(writer, instrux->InstructionBytes[i]);

    _end_array(writer);

    _end_object(writer);

    return s;
}


StringBuffer byte_to_json(uint8_t byte, size_t rip)
{
    auto s = StringBuffer {nullptr, 64};
    auto writer = Writer<StringBuffer> {s, nullptr};

    _start_object(writer);

    _write_uint(writer, "byte", byte);
    _write_uint(writer, "length", 1);
    _write_uint64(writer, "rip", rip);

    _end_object(writer);

    return s;
}


const char *json_to_string(StringBuffer &j)
{
    return j.GetString();
}

bool regs_from_json(const std::string &str, SHEMU_GPR_REGS &regs, bool &update_rsp)
{
    Document d;
    if (d.Parse(str.c_str()).IsNull()) {
        return false;
    }

    regs.RegRax = d["rax"].GetUint64();
    regs.RegRcx = d["rcx"].GetUint64();
    regs.RegRdx = d["rdx"].GetUint64();
    regs.RegRbx = d["rbx"].GetUint64();
    regs.RegRsp = d["rsp"].GetUint64();
    regs.RegRbp = d["rbp"].GetUint64();
    regs.RegRsi = d["rsi"].GetUint64();
    regs.RegRdi = d["rdi"].GetUint64();
    regs.RegR8 = d["r8"].GetUint64();
    regs.RegR9 = d["r9"].GetUint64();
    regs.RegR10 = d["r10"].GetUint64();
    regs.RegR11 = d["r11"].GetUint64();
    regs.RegR12 = d["r12"].GetUint64();
    regs.RegR13 = d["r13"].GetUint64();
    regs.RegR14 = d["r14"].GetUint64();
    regs.RegR15 = d["r15"].GetUint64();

    if ((update_rsp = !d["rsp"].IsNull())) {
        regs.RegRsp = d["rsp"].GetUint64();
    }

    if (!d["rip"].IsNull())
        regs.RegRip = d["rip"].GetUint64();

    return true;
}
