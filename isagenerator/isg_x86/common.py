#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#

# Use one of these value to indicate absent operands.
absent_op = ['n/a', '']

class InvalidSpecificationException(Exception):
    def __init__(self, value: str):
        self.value = value

    def __str__(self):
        return repr(self.value)
