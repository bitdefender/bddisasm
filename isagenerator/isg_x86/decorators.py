#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from isg_x86.common import *


# Valid decorators that can be present in either mnemonic or operand.
valid_decorators = [
    '{K}',          # Masking support.
    '{z}',          # Zeroing support.
    '{sae}',        # Surpress All Exceptions.
    '{er}',         # Embedded Rounding.
    '|B32',         # Broadcast 32.
    '|B64',         # Broadcast 64.
    '|B16',         # Broadcast 16.
    '{ND}',         # New data destination.
    '{NF}',         # No flags.
    '{ZU}',         # Zero-upper.
]

# Maps decorator to global flags.
deco_flags = {
    '{K}'   : 'MASK', 
    '{z}'   : 'ZERO', 
    '{sae}' : 'SAE', 
    '{er}'  : 'ER', 
    '|B32'  : 'BROADCAST', 
    '|B64'  : 'BROADCAST', 
    '|B16'  : 'BROADCAST',
    '{ND}'  : 'ND',
    '{ZU}'  : 'ZU',
    '{NF}'  : 'NF',
}

# Maps decorator to operand flags.
deco_op_flags = {
    '{K}'   : 'MASK', 
    '{z}'   : 'ZERO', 
    '{sae}' : 'SAE',
    '{er}'  : 'ER',
    '|B32'  : 'B32', 
    '|B64'  : 'B64', 
    '|B16'  : 'B16',
}


class Decorators:
    """
    Describes instruction (EVEX) decorators.

    Attributes
    ----------
    Names: list[str]
        A list with valid decorators.
    """
    def __init__(self):
        self.Names = []

    def add(self, value: str):
        self.Names.append(value)
