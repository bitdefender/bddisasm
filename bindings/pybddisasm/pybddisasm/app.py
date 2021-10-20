#
# Copyright (c) 2020 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from pybddisasm.bddisasm import *
from pybddisasm.helpers import *


def run(args):
    if args.file:
        return disassemble_file(args.file, offset=args.offset, size=args.size, rip=args.rip,
                                arch=args.arch, highlight=args.highlight, ext_info=args.ext_info)
    elif args.hexstring:
        return disassemble_hexstring(args.hexstring, offset=args.offset, size=args.size, rip=args.rip,
                                     arch=args.arch, highlight=args.highlight, ext_info=args.ext_info)
