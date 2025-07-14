#!/usr/bin/env python3
#
# Copyright (c) 2025 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import glob
import re

from isg_x86 import common
from isg_x86 import instrux
from isg_x86 import cpuid


def parse_entry(
    entry: str
) -> instrux.Instruction:
    """
    Parse one instruction specification line, and return an Instruction object.
    For examples on how an entry must look, check any entry inside table_* files.
    An instruction specification must contain 5 elements:
        1. The mnemonic
        2. The explicit operands (n/a or empty if none is present)
        3. The implicit operands (n/a or empty if none is present)
        4. The encoding (mandatory, cannot be empty)
        5. Metadata
    The instruction metadata includes several 'key:value' pairs; 'key' can be one of the following:
        1.  'a': instruction attributes
        2.  's': instruction set
        3.  'c': instruction class
        4.  'p': accepted prefixes
        5.  'w': operand access map
        6.  'i': CPUID feature flag; by default, the instruction set is used to look up a potential CPUID flag
        7.  'l': EVEX tuple type
        8.  'e': VEX/EVEX exception type
        9.  'v': EVEX mode (normal EVEX, extended VEX, extended legacy, conditional)
        10. 'f': flags access ('m': modified, 't': tested, 'u': undefined, '0': cleared, '1': set to 1)
        11. 'u': FPU flags access
        12. 'm': valid operating modes
        13. 'x': SIMD exceptions/MXCSR flags

    Raises
    ------
    InvalidSpecificationException
        - If the specification is incomplete
        - If an unknwon operand type or size is used
        - If an invalid encoding is specified
        - If an unknown metadata key is specified
        - If any metadata value is invalid/unknown

    Parameters
    ----------
    entry: str
        The instruction specification.

    Returns
    -------
    An object representing the instruction.
    """
    # Make sure this is not a comment. Skip comments.
    if entry.startswith('#') or len(entry) < 4:
        return None

    # Preprocess: remove comments, CR/LF
    entry = entry.replace('\x0D', '').replace('\x0A', '')
    com = entry.find('#')
    if -1 != com: entry = entry[:com]

    # Space can't be the first character.
    if entry[0] == ' ':
        raise common.InvalidSpecificationException(
            "Space cannot be the first character!")

    components = entry.split(';')
    if len(components) != 5:
        raise common.InvalidSpecificationException(
            "Expected 5 components per line, but found %d (missing semicolon?)!" % 
            len(components))

    mnemonic = components[0].strip()
    expops   = components[1].strip().split(',')
    impops   = components[2].strip().split(',')
    encoding = components[3].strip()
    meta     = components[4].strip().split(',')

    if len(mnemonic) < 1:
        raise common.InvalidSpecificationException(
            "Mnemonic cannot be empty!")

    if len(expops) == 1 and expops[0] in common.absent_op:
        expops = []

    if len(impops) == 1 and impops[0] in common.absent_op:
        impops = []

    return instrux.Instruction(mnemonic, expops, impops, encoding, meta)


def parse_ins_file(
    fpath: str
) -> list[instrux.Instruction]:
    """
    Parse an entire instruction specification file, and return a list of Instruction objects.

    Parameters
    ----------
    fpath: str
        The file containing multiple instruction specifications.

    Returns
    -------
    A list containing Instruction objects generated from the specs inside the provided file.
    """
    instructions = []
    lcount = 0
    for line in open(fpath, 'rt'):
        lcount += 1
        try:
            ins = parse_entry(line)
            if ins: instructions.append(ins)
        except Exception as e:
            print('ERROR: Parsing failed at %s:%d: %s' % (fpath, lcount, e))
            raise
    return instructions


def parse_cff_file(
    fpath: str
) -> list[cpuid.CpuidFeatureFlag]:
    """
    Parse the CPUID feature flags file, and return a list of CpuidFeatureFlag objects.

    Parameters
    ----------
    fpath: str
        The file containing multiple CPUID feature specifications.

    Returns
    -------
    A list containing CpuidFeatureFlag objects generated from the specs inside the provided file.
    """
    features = []
    for line in open(fpath, 'rt'):
        if line.startswith('#'):
            continue
        res = re.findall(r'([^\s]+)\s+:\s+(0x[0-9a-fA-F]+),\s+(0x[0-9a-fA-F]+),\s+(EAX|ECX|EDX|EBX),\s+(\d+)', line)
        if not res:
            continue
        res = res[0]
        features.append(cpuid.CpuidFeatureFlag(res[0], res[1], res[2], res[3], res[4]))
    return features
