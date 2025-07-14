#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from isg_x86.common import *


# Accepted access types. These values go with the 'w' metadata key (operand access map).
valid_access = [
    'N',            # No access.
    'P',            # Prefetch access.
    'R',            # Read.
    'W',            # Write.
    'CR',           # Conditional read.
    'CW',           # Conditional write.
    'RW',           # Read-Write.
    'CRW',          # Conditional Read-Write.
    'RCW',          # Read-Conditional Write.
    'CRCW',         # Conditional Read-Conditional Write.
]


class OperandAccessMap:
    """
    Describes the instruction operands access types.

    Attributes
    ----------
    Names: list[str]
        A list of operands accesses, one for each operand. The length of this list must
        be equal to the total number of instruction operands.
    """
    def __init__(self):
        self.Names = []

    def add(self, value: str):
        for r in value.split('|'):
            if r not in valid_access:
                raise InvalidSpecificationException(
                    "Unknown operand access specifier '%s', expecting one of [%s]" % 
                    (r, ','.join(valid_access)))
            self.Names.append(r)

    def count(self):
        return len(self.Names)
