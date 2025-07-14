#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from isg_x86.common import *


class FpuFlagsAccess:
    """
    Describes the FPU flags access.

    Attributes
    ----------
    Names: list[str]
        The FPU flags access mode, if the instruction is FPU. 4 entries must be
        present in Names, one for each FPU flag (C0, C1, C2, C3). Each entry must
        have one of the following values: 
            0 - cleared to 0
            1 - set to 1
            m - modified
            u - undefined.
    """
    def __init__(self):
        self.Names = []

    def add(self, value: str):
        # Value must be of the form: 'C0=0|C1=1|C2=m|C3=u'.
        self.Names = ['u', 'u', 'u', 'u'] # each one is undefined.

        for v in value.split('|'):
            flg, acc = v.split('=')
            if flg not in ['C0', 'C1', 'C2', 'C3']:
                raise InvalidSpecificationException(
                    "Unknown FPU flag: %s" % flg)
            if acc not in ['0', '1', 'm', 'u']:
                raise InvalidSpecificationException(
                    "Unknown FPU flag access: %s" % acc)
            self.Names[int(flg[1])] = acc
