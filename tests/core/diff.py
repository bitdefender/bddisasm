#
# Copyright (c) 2024 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#

from core import util


class Diff(object):
    def __init__(self):
        self._diff = list()

    def diff(self, dict1: dict, dict2: dict) -> type(None):
        """
        Find the differences between two dictionaries and store them.
        """
        for key in dict1:
            if key in dict2:
                if type(dict1[key]) is dict:
                    self.diff(dict1[key], dict2[key])
                if dict1[key] != dict2[key]:
                    result = [
                        " - [%s] : %s" % (key, dict1[key]),
                        " + [%s] : %s" % (key, dict2[key]),
                    ]
                    self._diff.append(result)
            else:
                result = [" - KEY %s : " % (key), " + KEY %s : " % (key)]

    def diffobj(self, actual: object, expected: object) -> list:
        """
        Each object contains a list of dictionaries. For each dictionary, the function checks if they are is any difference.
        """
        self._diff.clear()
        if len(actual) != len(expected):
            self._diff.append("invalid dimensions")
            return None

        for it in range(0, len(actual)):
            self.diff(actual[it]._data, expected[it]._data)

        if self._diff:
            for elem in self._diff:
                util.pr_color(elem[0], util.PRINT_COLOR_RED)
                util.pr_color(elem[1], util.PRINT_COLOR_GREEN)
        return self._diff
