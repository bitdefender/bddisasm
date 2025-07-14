#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from isg_x86.common import *


# Valid EVEX modes. These values go with the 'v' metadata key.
valid_evex_mode = [
    'none',         # Legacy EVEX
    'vex',          # EVEX extension for VEX
    'legacy',       # EVEX extension for legacy instructions
    'cond',         # EVEX extension for conditional instructons
]


class EvexMode:
    """
    Describes the instruction EVEX mode.

    Attributes
    ----------
    Name: str
        The EVEX mode, which must be one of valid_evex_mode. 
        If the instruction is not EVEX encoded, the Name shall remain None.
    """
    def __init__(self):
        self.Name = None

    def add(self, value: str):
        if value not in valid_evex_mode:
            raise InvalidSpecificationException(
                "Unknown evex extension type '%s', expecting one of [%s]" % 
                (value, ','.join(valid_evex_mode)))
        self.Name = value
