#
# Copyright (c) 2020 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
"""Early initialization and main entry point."""

import sys
import argparse
import pybddisasm


def _auto_int(x):
    return int(x, 0)


def _get_argparser():
    """Get the argpase parser."""
    parser = argparse.ArgumentParser(prog='pybddisasm')

    parser.add_argument('-b', '--arch', choices=[16, 32, 64], default=64, type=int)

    group = parser.add_mutually_exclusive_group()
    group.add_argument('-f', '--file', dest='file')
    group.add_argument('--hex', dest='hexstring', nargs='*')

    parser.add_argument('-r', '--rip', dest='rip', default=0, type=_auto_int,
                        help='Disassemble considering the given RIP.')
    parser.add_argument('--offset', dest='offset', default=0, type=_auto_int,
                        help='Disassemble starting at the given offset. Only valid for --file option.')
    parser.add_argument('--size', dest='size', default=0, type=_auto_int,
                        help='Disassemble only the given size. Only valid for --file option.')
    parser.add_argument('-c', '--highlight', dest='highlight', action='store_true',
                        help='Color the instruction bytes by group (opcode, prefix, etc).')
    parser.add_argument('-e', '--ext-info', dest='ext_info', action='store_true')
    parser.add_argument('--vendor', dest='vendor',
                        choices=['intel', 'amd', 'cyrix', 'geode', 'any'], default='any')

    return parser


def main():
    """Parse the args and run the app."""
    parser = _get_argparser()

    args = parser.parse_args(sys.argv[1:])

    if not args.file and not args.hexstring:
        parser.print_usage()
        return 1

    from pybddisasm import app
    app.run(args)
