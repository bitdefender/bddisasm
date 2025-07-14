#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from isg_x86.common import *


# Valid EVEX tuples. These values go with the 'l' metadata key.
valid_tuples = [
    'fv',           # Full Vector, Load+Op (Full Vector Dword/Qword).
    'hv',           # Half Vector, Load+Op (Half Vector).
    'qv',           # Quarter vector, Load+op (Quarter Vector, FP16)
    'fvm',          # Full Vector Memory, Load/store or subDword full vector.
    'hvm',          # Half Vector Memory, SubQword Conversion.
    'qvm',          # Quarter Vector Memory, SubDword Conversion.
    'ovm',          # Oct Vector Memory, SubWord Conversion.
    'dup',          # Dup, VMOVDDUP.
    'm128',         # Mem 128, Shift count from memory.
    't1s8',         # Tuple 1 Scalar, 8 bit, 1Tuple less than Full Vector.
    't1s16',        # Tuple 1 Scalar, 16 bit, 1Tuple less than Full Vector.
    't1s',          # Tuple 1 Scalar, 32/64 bit, 1Tuple less than Full Vector.
    't1f',          # Tuple 1 Fixed, 1 Tuple memsize not affected by EVEX.W.
    't2',           # Tuple 2, Broadcast (2 elements).
    't4',           # Tuple 4, Broadcast (4 elements).
    't8',           # Tuple 8, Broadcast (8 elements).
    't1_4x',
]


class EvexTuple:
    """
    Describes the instruction EVEX tuple type.

    Attributes
    ----------
    Name: str
        The EVEX tuple type, which must be one of valid_tuples 
        If the instruction is not EVEX encoded, the Name shall remain None.
    """
    def __init__(self):
        self.Name = None

    def add(self, value: str):
        if value not in valid_tuples:
            raise InvalidSpecificationException(
                "Unknown tuple specifier '%s', expecting one of [%s]" % 
                (value, ','.join(valid_tuples)))
        self.Name = value
