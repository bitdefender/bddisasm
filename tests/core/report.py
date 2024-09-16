#
# Copyright (c) 2024 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import time

from core import util


class Report(object):
    def __init__(self, enabled: bool):
        self._fd = None
        self._f_name = None
        self._ts = int(time.time())
        self._enabled = enabled

        self.ropen()

    def __del__(self) -> type(None):
        """
        Close the fd when the object is destroyed.
        """
        if not self._enabled:
            return
        self.rclose()
        util.pr_debug("[*] report written to file : %s" % (self._f_name))

    def ropen(self) -> type(None):
        """
        Open the file and store the fd.
        """
        if not self._enabled:
            return

        self._f_name = str(self._ts) + ".report"
        util.pr_debug("[*] write report to file : %s" % (self._f_name))
        self._fd = open(self._f_name, "w")

    def rclose(self) -> type(None):
        """
        Close the fd handle.
        """
        if not self._enabled:
            return

        self._fd.close()

    def rwrite(self, expected, actual, f_path, ldiff) -> type(None):
        """
        Write in the file the differences between the two outputs, but also their complete content.
        """
        if not self._enabled:
            return

        self._fd.write("-" * 90)
        self._fd.write("\n")
        self._fd.write("f_path = %s" % (f_path))
        self._fd.write("\n")

        for diff in ldiff:
            self._fd.write("=" * 40 + " DIFF     " + "=" * 40)
            self._fd.write("\n")

            for elem in diff:
                self._fd.write(elem[0])
                self._fd.write("\n")
                self._fd.write(elem[1])
                self._fd.write("\n")

        self._fd.write("=" * 40 + " EXPECTED " + "=" * 40)
        self._fd.write("\n")
        self._fd.write(expected)

        self._fd.write("=" * 40 + " ACTUAL   " + "=" * 40)
        self._fd.write("\n")
        self._fd.write(actual)
