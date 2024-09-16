#
# Copyright (c) 2024 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import os

from core import util


class Generate(object):
    def __init__(self, f_disasm: str, f_nasm: str, verbose: bool):
        self._f_disasm = f_disasm
        self._f_nasm = f_nasm
        self._verbse = verbose

    def run(self, data: str, d_path: str, overwrite=False) -> type(None):
        """
        Generates the '.result' and '.test' files.

        If the 'overwrite' options is used, the '.result' file is regenerated even if it exists.
        If the path to nasm is not provided, the '.test' file is not generated.
        """
        if not self._f_disasm or not os.path.exists(self._f_disasm):
            return

        f_source = data["f_source"]
        f_result = data["f_result"]
        f_test = data["f_test"]
        options = data["options"]
        command = data["command"]

        f_source = os.path.join(d_path, f_source)
        f_result = os.path.join(d_path, f_result)
        f_test = os.path.join(d_path, f_test)

        if self._f_nasm and os.path.exists(self._f_nasm):
            if not os.path.exists(f_test):
                if os.path.exists(f_source):
                    util.pr_debug("[+] generate .test file ... (%s)" % (f_test))
                    cmd = [self._f_nasm, f_source, "-o", f_test]
                    if self._verbse:
                        util.pr_debug("[*] run subprocess using cmd : %s" % (cmd))
                    _, _, _ = util.rsubprocess(cmd)
                else:
                    util.pr_warn(
                        "[-] .asm file is missing (%s) - skip ..." % (f_source)
                    )

        if overwrite or not os.path.exists(f_result):
            util.pr_debug("[+] generate .result file ... (%s)" % (f_result))
            cmd = [self._f_disasm, command, "-exi", *options.split(" "), "-f", f_test]
            if self._verbse:
                util.pr_debug("[*] run subprocess using cmd : %s" % (cmd))
            stdout, _, returncode = util.rsubprocess(cmd)
            if returncode != 0:
                pass
            else:
                stdout = stdout.replace("\r", "")
                with open(f_result, "w") as fd:
                    fd.write(stdout)
