#
# Copyright (c) 2024 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
from core import util

PASSED = "PASSED"
FAILED = "FAILED"
RUN = "RUN"
TOTAL = "TOTAL"


class Summary(object):
    def __init__(self):
        self._data = {PASSED: 0, FAILED: 0, RUN: 0, TOTAL: 0}

    def passed(self) -> type(None):
        """
        Increment for each test failed.
        """
        self._data[PASSED] += 1

    def failed(self) -> type(None):
        """
        Increment for each test failed.
        """
        self._data[FAILED] += 1

    def total(self) -> type(None):
        """
        Increment for each test.
        """
        self._data[TOTAL] += 1

    def run(self) -> type(None):
        """
        Increment for each test run.
        """
        self._data[RUN] += 1

    def show(self) -> type(None):
        """
        Show the summary of the results after running the tests.
        """
        util.pr_debug("[*] %s%s%s" % ("=" * 25, " SUMMARY ", "=" * 25))
        util.pr_debug("[*] TOTAL    : %d" % (self._data[TOTAL]))
        util.pr_debug("[*] RUN      : %d" % (self._data[RUN]))
        util.pr_info("[*] PASSED   : %d" % (self._data[PASSED]))
        util.pr_error("[*] FAILED   : %d" % (self._data[FAILED]))
        util.pr_debug("[*] %s" % ("=" * (50 + len(" SUMMARY "))))
