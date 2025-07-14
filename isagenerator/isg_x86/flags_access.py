#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from isg_x86.common import *


# Valid flags. These values go with the 'f' metadata key.
valid_flags = [
    'CF',           # Carry.
    'PF',           # Parity.
    'AF',           # Auxiliary.
    'ZF',           # Zero.
    'SF',           # Sign.
    'TF',           # Trap.
    'IF',           # Interrupt.
    'DF',           # Direction.
    'OF',           # Overflow.
    'IOPL',         # I/O privilege level.
    'NT',           # Nested Task.
    'RF',           # Resume Flag.
    'VM',           # V8086 mmode.
    'AC',           # Alignment Check.
    'VIF',          # Virtual IF.
    'VIP',          # Virtual IP.
    'ID'            # CPUID ID flag.
]

# Valid flags access. These values go with the 'f' metadata key.
valid_flag_op = [
    'm',            # modified.
    't',            # tested.
    '0',            # cleared.
    '1',            # set.
    'u',            # undefined.
    'n',            # not accessed.
]


# Flags access templates; many instructions have the same flag usage, so define them here.
# Syntax: NAME: FLAG=modifier|FLAG=modifier|.... See the definitions for some examples.
# Accepted flags: CF, PF, AF, ZF, TF, IF, OF, DF, AC, IOPL, RF, NT, VIF, VIP, VM
# Accepted modifiers: t (tested), m (modified according to the result), 0 (cleared), 1 (set), u (undefined)
template_flags = {
    "ARITH"       : "CF=m|PF=m|AF=m|ZF=m|SF=m|OF=m",        # All arithmetic instructions: ADD, SUB, CMP, XADD.
    "ARITHC"      : "CF=t|CF=m|PF=m|AF=m|ZF=m|SF=m|OF=m",   # Arithmetic with carry instructions: ADC, SBB.
    "INCDEC"      : "PF=m|AF=m|ZF=m|SF=m|OF=m",             # INC/DEC do not alter CF.
    "LOGIC"       : "CF=0|PF=m|AF=u|ZF=m|SF=m|OF=0",        # Logic instructions: OR, AND, XOR, TEST.
    "ROT"         : "CF=m|OF=m",                            # Rotate instructions.
    "ROTC"        : "CF=t|CF=m|OF=m",                       # Rotate carry instructions.
    "SHIFT"       : "CF=m|PF=m|AF=u|ZF=m|SF=m|OF=m",        # Shift instructions.
    "SHIFTD"      : "CF=u|PF=m|AF=u|ZF=m|SF=m|OF=u",        # Shidft double instructions.
    "MUL"         : "CF=m|PF=u|AF=u|ZF=u|SF=u|OF=m",        # Multiply instructions.
    "DIV"         : "CF=u|PF=u|AF=u|ZF=u|SF=u|OF=u",        # Division instructions.
    "AADM"        : "CF=u|PF=m|AF=u|ZF=m|SF=m|OF=u",        # AAD/AAM.
    "AAAS"        : "CF=m|PF=u|AF=t|AF=m|ZF=u|SF=u|OF=u",   # AAA/AAS.
    "DAAS"        : "CF=t|CF=m|PF=m|AF=t|AF=m|ZF=m|OF=u",   # DAA/DAS.
    "IO"          : "IOPL=t|VM=t",                          # I/O instructions.
    "IOS"         : "DF=t|IOPL=t|VM=t",                     # I/O string operations.
    "INT"         : "VM=t|VM=m|IF=m|NT=m|AC=m|RF=m|TF=m",   # INT instructions.
    "CMPS"        : "CF=m|PF=m|AF=m|ZF=m|SF=m|OF=m|DF=t",   # Used by CMPS/SCAS and REP CMPS/REP SCAS.
    "REPCMPS"     : "CF=m|PF=m|AF=m|ZF=t|ZF=m|SF=m|OF=m|DF=t",
    "PCMPSTR"     : "CF=m|PF=0|AF=0|ZF=m|SF=m|OF=m",        # Used by PCMPESTRM, PCMPESTRI, PCMPISTRM, PCMPISTRI.
    "MOVCRDR"     : "CF=u|PF=u|AF=u|ZF=u|SF=u|OF=u",        # MOV to/from control/debug registers, all flags are undefined.
    "VMX"         : "CF=m|PF=0|AF=0|ZF=m|SF=0|OF=0",        # Used by VMX instructions.
    "BT"          : "CF=m|PF=u|AF=u|SF=u|OF=u",             # Used by BT, BTS, BTR, BTC instructions.
    "COMIS"       : "CF=m|PF=m|ZF=m",                       # COMIS instructions.
    "VPTEST"      : "CF=m|PF=0|AF=0|ZF=m|SF=0|OF=0",        # VPTEST instructions.
    "WAITPKG"     : "CF=m|PF=0|AF=0|ZF=0|SF=0|OF=0",        # UMWAIT, TPAUSE instructions.
    "ENQCMD"      : "CF=0|PF=0|AF=0|ZF=m|SF=0|OF=0",        # ENQCMD instructions.
    "KORTEST"     : "CF=m|PF=0|AF=0|ZF=m|SF=0|OF=0",        # KORTEST instructions.
    "CO"          : "OF=t",
    "CNO"         : "OF=t",
    "CC"          : "CF=t",
    "CNC"         : "CF=t",
    "CZ"          : "ZF=t",
    "CNZ"         : "ZF=t",
    "CBE"         : "CF=t|ZF=t",
    "CNBE"        : "CF=t|ZF=t",
    "CS"          : "SF=t",
    "CNS"         : "SF=t",
    "CP"          : "PF=t",
    "CNP"         : "PF=t",
    "CL"          : "SF=t|OF=t",
    "CNL"         : "SF=t|OF=t",
    "CLE"         : "SF=t|ZF=t|OF=t",
    "CNLE"        : "SF=t|ZF=t|OF=t",
    "AESKL"       : "CF=0|PF=0|AF=0|ZF=m|SF=0|OF=0",        # AES Key Locker instructions.
    "ZERO"        : "CF=0|PF=0|AF=0|ZF=0|SF=0|OF=0",        # All flags are zeroed.
    "UINTR"       : "CF=m|PF=0|AF=0|ZF=0|SF=0|OF=0",        # UINTR flags access, as done by TESTUI.
    "CMPSFP"      : "CF=m|PF=m|AF=0|ZF=m|SF=m|OF=m",        # AVX 10.2 Compare Scalar FP With Enhanced EFLAGS.
}


class FlagsAccess:
    """
    Describes the flags (RFLAGS) access.

    Attributes
    ----------
    Names: dict
        Flags access mode. Each key in the dict is one of the access modes (t - tested, m - modified,
        0 - cleared to 0, 1 - set to 1, u - undefined). Each value is a list of flags (from valid_flags)
        that are accessed in that manner.
    """
    def __init__(self):
        # Default: no access to any flags.
        self.Names = {
            't': [],
            'm': [],
            '0': [],
            '1': [],
            'u': []
        }

    def add(self, value: str):
        flgaccess = []

        # Expand templates.
        for v in value.split('|'):
            if v in template_flags:
                flgaccess += template_flags[v].split('|')
            else:
                flgaccess.append(v)

        revflg = {}
        for m in valid_flag_op:
            revflg[m] = []
            
        for flg in flgaccess:
            f, m = flg.split('=')
            if m not in valid_flag_op:
                raise InvalidSpecificationException(
                    "Unknown flag access specifier '%s', expecting one of [%s]" % 
                    (m, ','.join(valid_flag_op)))
            if f not in valid_flags:
                raise InvalidSpecificationException(
                    "Unknown flag specifier '%s', expecting one of [%s]" % 
                    (f, ','.join(valid_flags)))
            revflg[m].append(f)

        self.Names = revflg
