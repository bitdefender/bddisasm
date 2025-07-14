#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from isg_x86.common import *


# Instruction attributes. These values go with the 'a' metadata key.
valid_attributes = [
    # Misc instruction properties.
    'MODRM',        # Mod r/m is present.
    '3DNOW',        # Instruction uses 3dnow encoding.
    'COND',         # Instruction has condition encoded in lower 4 bit of the opcode.
    'LOCKSP',       # Special lock - MOV CR on amd can use LOCK to access CR8 in 32 bit mode.
    'VECTOR',       # Vector instruction.
    'S66',          # 0x66 prefix changes length even if it is in special map (66, f2, f3).
    'SHS',          # Instruction accesses the shadow stack.
    'MFR',          # The Mod inside Mod R/M is forced to register. No SIB/disp present.
    'CETT',         # Instruction is CET tracked.
    'SERIAL',       # Instruction is serializing.
    'SCALABLE',     # EVEX.pp prefix can be 0 or 1, simulating the presence of 0x66 prefix.

    # 64-bit modifiers.
    'I64',          # Instruction invalid in 64 bit mode.
    'O64',          # Instruction valid only in 64 bit mode.
    'F64',          # Operand size forced to 64 bit.
    'D64',          # Operand size defaults to 64 bit.

    # Per operand indicators.
    'OP1DEF',       # Operand 1 is default (implicit).
    'OP2DEF',       # Operand 2 is default (implicit).
    'OP2SIGNEXO1',  # Operand 2 is sign-extended to the size of the first operand.
    'OP3SIGNEXO1',  # Operand 3 is sign-extended to the size of the first operand.
    'OP1SIGNEXDW',  # Operand 1 is sign-extended to the size of the default word.

    # Addressing flags.
    'AG',           # Instruction uses address generation, no memory access.
    'BITBASE',      # Instruction uses bitbase addressing.
    'VSIB',         # Instruction uses VSIB addressing.
    'MIB',          # Instruction uses MIB addressing.
    'SIBMEM',       # Instruction uses sibmem addressing (AMX instructions).

    # Ignored fields.
    'LIG',          # *vex.L is ignored.
    'WIG',          # *vex.W is ignored.
    'I67',          # Ignore the address size override (0x67) prefix in 64 bit mode.
    'IER',          # Ignore embedded rounding for the instruction.
    'IWO64',        # The VEX/EVEX.W field is ignored outside 64 bit mode, and behaves as if it's 0.

    # Restrictions - these fields being present leads to #UD.
    'MMASK',        # Instruction must have mask specified (mask cannot be k0).
    'NOMZ',         # Zeroing not allowed with memory addressing.
    'NOL0',         # Vector length 128 not supported.
    'NOA16',        # 16 bit addressing not supported.
    'NORIPREL',     # RIP relative addressing not supported.
    'NO66',         # 0x66 prefix causes #UD.
    'NO67',         # 0x67 prefix causes #UD.
    'NOREP',        # REP prefixes will cause #UD.
    'NOREX2',       # REX2 prefix will cause #UD.
    'NOV',          # XOP/VEX/EVEX.v will cause #UD if not logical 0.
    'NOVP',         # EVEX.v' will cause #UD if not logical 0.
]


class Attributes:
    """
    Describes the instruction attributes.

    Attributes
    ----------
    Names: list[str]
        A list with attributes. Each attribute must be one listed in valid_attributes.
    """
    def __init__(self):
        # By default, no attributes are present.
        self.Names = []

    def add(self, value: str):
        for a in value.split('|'):
            # Make sure it's a valid attribute.
            if a not in valid_attributes:
                raise InvalidSpecificationException(
                    "Unknown attribute specifier '%s', expecting one of [%s]" % 
                    (a, ','.join(valid_attributes)))
                    
            # If not present already, add it.
            if a not in self.Names:
                self.Names.append(a)
                
    def contains(self, value: str) -> bool:
        if value not in valid_attributes:
            raise InvalidSpecificationException(
                    "Unknown attribute specifier '%s', expecting one of [%s]" % 
                    (value, ','.join(valid_attributes)))
        
        return value in self.Names
        
    def instruction_attributes(self) -> list[str]:
        # Returns only the instruction attributes, without the operand attributes.
        op_attributes = ['OP0', 'OP1', 'OP2', 'OP3', 'OP4', 'OP5', 'OP6', 'OP7']
        
        return [a for a in self.Names if a[0:3] not in op_attributes]
