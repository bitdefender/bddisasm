#
# Copyright (c) 2020 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import os
import sys

from pybddisasm.bddisasm import *

try:
    from termcolor import colored
except:
    colored = None


_SPACES = [
    '',
    '  ',
    '    ',
    '      ',
    '        ',
    '          ',
    '            ',
    '              ',
    '                ',
    '                  ',
    '                    ',
    '                      ',
    '                        ',
    '                          ',
    '                            ',
    '                              ',
]


def print_internal(string, foreground=None, highlight=True):
    no_colors = (foreground is None) or (not highlight)

    if not colored or no_colors:
        sys.stdout.write(string)
    else:
        text = colored(string, foreground)
        sys.stdout.write(text)


def print_instruction(instruction, rip, highlight=False, ext_info=False):
    k = 0

    print_internal('%016x ' % rip)

    # prefixes
    for ibyte in range(0, instruction.PrefLength):
        print_internal('%02x' % instruction.InstructionBytes[ibyte])

    # opcodes
    k += instruction.PrefLength
    for ibyte in range(k, k + instruction.OpLength):
        print_internal('%02x' % instruction.InstructionBytes[ibyte], 'green', highlight)

    # modrm and sib
    k += instruction.OpLength
    for ibyte in range(k, k + instruction.HasModRm + instruction.HasSib):
        print_internal('%02x' % instruction.InstructionBytes[ibyte], 'yellow', highlight)

    # displacement
    k += instruction.HasModRm + instruction.HasSib
    for ibyte in range(k, k + instruction.DispLength):
        print_internal('%02x' % instruction.InstructionBytes[ibyte], 'blue', highlight)

    # relative offset/moffset/immediates
    rest = instruction.Imm1Length + instruction.Imm2Length + instruction.RelOffsLength + \
        instruction.MoffsetLength + instruction.HasSseImm + instruction.AddrLength

    k += instruction.DispLength
    for ibyte in range(k, k + rest):
        print_internal('%02x' % instruction.InstructionBytes[ibyte], 'red', highlight)

    # the rest of the bytes
    k += rest
    for ibyte in range(k, instruction.Length):
        print_internal('%02x' % instruction.InstructionBytes[ibyte])

    print_internal('%s' % _SPACES[16 - instruction.Length])
    print_internal('%s' % instruction.Text)

    if ext_info:
        print_internal('\n')
        print_internal('%r' % instruction)

    print_internal('\n')


def disassemble_file(filepath, offset=0, size=0, rip=0, arch=64,
                     highlight=True, vendor='any', ext_info=False):
    if not filepath:
        return

    with open(filepath, 'rb') as f:
        total = 0
        file_size = os.path.getsize(filepath)

        if not size:
            size = file_size

        while offset < file_size and total < size:
            to_read = file_size - offset
            if to_read > 15:
                to_read = 15

            f.seek(offset, 0)
            buff = f.read(to_read)

            current_rip = rip + total

            instr = nd_decode_ex2(buff, arch, arch, arch, vendor, current_rip)
            if instr:
                print_instruction(instr, current_rip, highlight, ext_info)
                offset += instr['Length']
                total += instr['Length']
            else:
                sys.stdout.write('%016x %02x %s db 0x%02x' % (current_rip, buff[0],
                                                              _SPACES[15], buff[0]))

                if str.isalpha(chr(buff[0])):
                    sys.stdout.write(str(buff[0]))

                sys.stdout.write('\n')

                offset += 1
                total += 1


def disassemble_hexstring(hexstring, offset=0, size=0, rip=0, arch=64,
                          highlight=True, vendor='any', ext_info=False):
    if not hexstring:
        return

    buff = bytes.fromhex(''.join(hexstring))

    total = 0
    if not size:
        size = len(buff)

    while total < size:
        current_rip = rip + total

        instr = nd_decode_ex2(buff[total:total+16], arch, arch, arch, vendor, current_rip)
        if instr:
            print_instruction(instr, current_rip, highlight, ext_info)
            offset += instr['Length']
            total += instr['Length']
        else:
            sys.stdout.write('%016x %02x %s db 0x%02x\n' % (current_rip, buff[offset],
                                                            _SPACES[15], buff[offset]))
            offset += 1
            total += 1
