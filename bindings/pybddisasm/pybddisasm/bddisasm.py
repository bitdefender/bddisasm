#
# Copyright (c) 2020 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
"""Interface for disassembling code."""

import _pybddisasm

class NdInstruction(dict):
    """Magic wrapper around a dictionary, that makes all the keys available
    through dot notation (recursively). Examples:

    instruction['Length'] is now instruction.Length
    instruction['InstructionBytes'] is now instruction.InstructionBytes

    You can still access the fileds the old way."""
    def __init__(self, *args, **kwargs):
        super(NdInstruction, self).__init__(*args, **kwargs)
        self.__dict__ = self

        # now set all the dict children to be the same
        for key in self.__dict__:
            if not isinstance(self.__dict__[key], dict):
                continue

            self.__dict__[key] = NdInstruction(self.__dict__[key])

        if 'Operands' in self.__dict__ and isinstance(self.Operands, dict):
            for op in self.Operands:
                self.Operands[op] = NdInstruction(self.Operands[op])


def nd_to_text(code, arch_code_size, arch_data_size=0, rip=0):
    if not arch_data_size:
        arch_data_size = arch_code_size

    return _pybddisasm.nd_to_text(code, arch_code_size, arch_data_size, rip)


def nd_decode(code, arch_code_size, arch_data_size=0, rip=0):
    if not arch_data_size:
        arch_data_size = arch_code_size

    instruction = _pybddisasm.nd_decode_ex(code, arch_code_size, arch_data_size, rip)

    if not instruction:
        return None

    return NdInstruction(instruction)


def nd_decode_ex(code, arch_code_size, arch_data_size=0, rip=0):
    if not arch_data_size:
        arch_data_size = arch_code_size

    instruction = _pybddisasm.nd_decode_ex(code, arch_code_size, arch_data_size, rip)

    if not instruction:
        return None

    return NdInstruction(instruction)


def nd_decode_ex2(code, arch_code_size, arch_data_size, arch_stack_size, vendor='intel', rip=0):
    instruction = _pybddisasm.nd_decode_ex2(code, arch_code_size, arch_data_size,
                                       arch_stack_size, vendor, rip)

    if not instruction:
        return None

    return NdInstruction(instruction)
