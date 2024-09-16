#
# Copyright (c) 2024 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import sys
import os
import argparse
import traceback
import ctypes
import platform

from ctypes import wintypes

from core import core
from core import util


def main() -> int:
    parser = argparse.ArgumentParser(description="BDDISASM/BDSHEMU Test-Suite")
    parser.add_argument(
        "--conf",
        nargs="+",
        help="Path of the conf files",
        action="store",
        required=True,
    )

    parser.add_argument(
        "--disasm",
        help="Path of the disasm tool",
        action="store",
        required=True,
    )

    parser.add_argument(
        "--nasm",
        help="Path of the nasm tool (used to generate samples)",
        action="store",
        required=False,
    )

    parser.add_argument(
        "--report",
        help="Generate report",
        action="store_true",
        default=False,
        required=False,
    )

    parser.add_argument(
        "--regenerate",
        help="Regenerate test files",
        action="store_true",
        default=False,
        required=False,
    )

    parser.add_argument(
        "--verbose",
        help="Verbose",
        action="store_true",
        default=False,
        required=False,
    )

    args = parser.parse_args(sys.argv[1:])
    f_conf = args.conf
    f_disasm = args.disasm
    f_nasm = args.nasm
    verbose = args.verbose
    report = args.report
    regenerate = args.regenerate

    util.pr_debug("[*] options :")
    util.pr_debug("[*]\tconf           : %s" % (f_conf))
    util.pr_debug("[*]\tdisasm         : %s" % (f_disasm))
    util.pr_debug("[*]\tnasm           : %s" % (f_nasm))
    util.pr_debug("[*]\treport         : %s" % (report))
    util.pr_debug("[*]\tregenrate      : %s" % (regenerate))
    util.pr_debug("[*]\tverbose        : %s " % (verbose))

    coreobj = core.Core(f_disasm, f_nasm, verbose, report)
    try:
        for conf in f_conf:
            if regenerate:
                coreobj.regenerate(conf)
            else:
                coreobj.process(conf)

        if not regenerate:
            coreobj.summary()
    except Exception as ex:
        util.pr_debug(traceback.format_exc())
        return util.RETCODE_ERROR

    return coreobj.retcode()

if __name__ == "__main__":
    if platform.system() == "Windows":
        ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004
        STD_OUTPUT_HANDLE = -11
        kernel32 = ctypes.windll.kernel32
        stdout = kernel32.GetStdHandle(STD_OUTPUT_HANDLE)
        mode = wintypes.DWORD()
        mode = kernel32.GetConsoleMode(stdout, ctypes.byref(mode))
        kernel32.SetConsoleMode(stdout, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)

    retcode = main()
    sys.exit(retcode)
