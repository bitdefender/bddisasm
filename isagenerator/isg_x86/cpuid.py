#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from dataclasses import dataclass


@dataclass
class CpuidFeatureFlag:
    """
    Describes a CPUID feature flag.

    Attributes
    ----------
    Name: str
        Feature name.
    Leaf: str
        Leaf number (the EAX input value).
    SubLeaf: str
        SubLeaf number (the ECX input value. '0xFFFFFFFF' if no subleaf present).
    Reg: str
        The output register that contains the feature indication.
    Bit: str
        The bit inside Reg that indicates whether the feature is present or not.
    """
    Name    : str
    Leaf    : int
    SubLeaf : int
    Reg     : str
    Bit     : int

    def __str__(self):
        return '%s: %s, %s, %s, %s' % (self.Name, self.Leaf, self.SubLeaf, self.Reg, self.Bit)
