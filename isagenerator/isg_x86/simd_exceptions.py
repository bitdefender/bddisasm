#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from isg_x86.common import *


# Valid SIMD Floating-Point Exceptions.
valid_simd_exceptions = [
     'IE',          # Invalid Operation Exception.
     'DE',          # Denormal Exception.
     'ZE',          # Divide-by-Zero Exception.
     'OE',          # Overflow Exception.
     'UE',          # Underflow Exception.
     'PE',          # Precision Exception.
]


class SimdExceptions:
    """
    Describes the SIMD exceptions.

    Attributes
    ----------
    Names: list[str]
        The list of SIMD exceptions the instruction may generate. Each entry
        must be one of valid_simd_exceptions.
    """
    def __init__(self):
        self.Names = []

    def add(self, value: str):
        # value can specify multiple SIMD exceptions, separated by '|'. 
        # Example: 'IE|OE'.
        for e in value.split('|'):
            if e not in valid_simd_exceptions:
                raise InvalidSpecificationException(
                    "Unknown SIMDexception '%s', expecting one of [%s]" % 
                    (e, ','.join(valid_simd_exceptions)))
            self.Names.append(e)
