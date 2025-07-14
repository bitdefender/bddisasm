#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from isg_x86.common import *


# Accepted prefixes. These values go with the 'p' metadata key.
valid_prefixes = [
    'REP',          # Rep prefix is accepted.
    'REPC',         # Conditional rep prefix is accepted.
    'HLE',          # Hardware Lock Elision accepted.
    'BND',          # Bound prefix accepted (MPX).
    'LOCK',         # Lock prefix accepted.
    'BH',           # Branch hints accepted.
    'XACQUIRE',     # Xacquire prefix accepted.
    'XRELEASE',     # Xrelease prefix accepted.
    'HLEWOL',       # HLE prefix is accepted without lock - used by MOV instructions.
    'DNT',          # Do Not Track prefix accepted (CET).
]


class PrefixMap:
    """
    Describes the list of prefixes the instruction accepts.

    Attributes
    ----------
    Names: list[str]
        The list of accepted prefixes. Each entry must be one of valid_prefixes.
    """
    def __init__(self):
        self.Names = []

    def add(self, value: str):
        # value can specify multiple prefixes, separated by '|'. 
        # Example: 'REP|LOCK'.
        for p in value.split('|'):
            if p not in valid_prefixes:
                raise InvalidSpecificationException(
                    "Unknown prefix specifier '%s', expecting one of [%s]" % 
                    (p, ','.join(valid_prefixes)))
            if p not in self.Names:
                self.Names.append(p)
