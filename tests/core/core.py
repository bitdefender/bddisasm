#
# Copyright (c) 2024 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import sys
import os
import json
import re
import shutil
import time

from core import objects
from core import util
from core import report
from core import summary
from core import generate
from core import output
from core import diff


class Core(object):
    def __init__(
        self, f_disasm: str, f_nasm: str, enable_verbose: bool, enable_report: bool
    ):
        self._f_disasm = f_disasm
        self._f_nasm = f_nasm
        self._verbse = enable_verbose

        self._report = report.Report(enable_report)
        self._summary = summary.Summary()
        self._generate = generate.Generate(self._f_disasm, self._f_nasm, self._verbse)

        self._retcode = util.RETCODE_SUCCESS

    def retcode(self) -> int:
        """
        Returns the tests results.
        """
        return self._retcode

    def summary(self) -> type(None):
        """
        Shows the tests summary.
        """
        self._summary.show()

    def rdfile(self, f_path: str) -> str:
        """
        Reads the content of the provided file.
        """
        with open(f_path, "r") as fp:
            return fp.read()

    def rdjson(self, f_path: str) -> dict:
        """
        Reads the content of the provided json file.
        """
        with open(f_path, "r") as fp:
            return json.load(fp)

    def process(self, f_conf):
        """
        Iterate the configuration file and calls the 'run' method to run the tests.
        """
        conf_data = self.rdjson(f_conf)

        if not os.path.exists(f_conf):
            util.pr_warn("[-] conf file does not exist")
            return util.RETCODE_ERROR

        if self._verbse:
            util.pr_debug("[*] dump conf ...")
            util.pr_debug(json.dumps(conf_data, indent=4))

        for item in conf_data:
            data = [item]
            d_path = os.path.dirname(f_conf)

            for elem in data:
                self.run(elem, d_path)

    def run(self, elem: dict, d_path: str) -> type(None):
        """
        This function is used to compare the '.result' files with the output produced by disasmtool for the corresponding '.test' files.

        The first step is to run the 'disasmtool' process to generate the output for the '.test' file. Next, the text is converted into a dictionary
        using objects from the 'output.py' module in order to check if there are differences between them.
        Moreover, the unproccessed texts are also compared.
        """
        f_source = elem["f_source"]
        f_result = elem["f_result"]
        f_test = elem["f_test"]
        options = elem["options"]
        command = elem["command"]

        f_result = os.path.join(d_path, f_result)
        f_test = os.path.join(d_path, f_test)
        f_source = os.path.join(d_path, f_source)

        self._generate.run(elem, d_path)

        if not os.path.exists(f_result):
            util.pr_debug("[*] f_result does not exits : %s" % f_result)
            return

        self._summary.total()

        util.pr_debug("[*] running %s ..." % (os.path.splitext(f_result)[0]))
        if self._verbse:
            util.pr_debug("[*] processing ...\n%s" % (json.dumps(elem, indent=4)))

        if not os.path.exists(f_result):
            util.pr_warn("[-] %s does not exits. skip ..." % (f_result))
            return

        cmd = [
            self._f_disasm,
            command,
            "-exi",
            *options.split(" "),
            "-f",
            f_test,
        ]

        if self._verbse:
            util.pr_debug("[*] run subprocess using cmd : %s" % (cmd))
        stdout, _, returncode = util.rsubprocess(cmd)
        if returncode != 0:
            return

        if self._verbse:
            util.pr_debug("[*] read result file : %s" % (f_result))

        result = self.rdfile(f_result)
        subresult = stdout.replace("\r", "")

        if command == "decode":
            objresult = output.OutputDecode(result)
            objtest = output.OutputDecode(subresult)

        if command == "shemu":
            objresult = output.OutputShemu(result)
            objtest = output.OutputShemu(subresult)

        outresult = objresult.read()
        outtest = objtest.read()

        self._summary.run()

        ldiff = list()
        diffobj = diff.Diff()
        for it in range(0, len(outresult)):
            ldiff.append(diffobj.diffobj(outresult[it], outtest[it]))

        result = result.replace("\r", "")
        subprocess = subresult.replace("\r", "")

        if result != subresult:
            util.pr_error("[-] raw result diff")

        if any(ldiff):
            self._summary.failed()
            util.pr_error("[*]\tFAILED")

            self._retcode = util.RETCODE_FAILURE

            self._report.rwrite(result, subresult, os.path.splitext(f_result)[0], ldiff)
        else:
            self._summary.passed()
            util.pr_info("[*]\tPASSED")

    def regenerate(self, f_conf: str) -> None:
        """
        Regenerate the test files. The function uses the overwrite option from Generate object (see generate.py).
        """
        conf_data = self.rdjson(f_conf)

        if self._verbse:
            util.pr_debug("[*] dump conf ...")
            util.pr_debug(json.dumps(conf_data, indent=4))

        for item in conf_data:
            data = [item]
            d_path = os.path.dirname(f_conf)

            for elem in data:
                self._generate.run(elem, d_path, True)
