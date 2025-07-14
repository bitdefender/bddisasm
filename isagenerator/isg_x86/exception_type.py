#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
class ExceptionType:
    """
    Describes the instruction exception type..

    Attributes
    ----------
    Name: str
        The exception type, as a name. The exception name can be anything, 
        but it will be translated to a `ND_EXT` enum, which must exist.
    """
    def __init__(self):
        self.Name = None

    def add(self, value: str):
        self.Name = value
