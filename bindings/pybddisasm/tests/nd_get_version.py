#
# Copyright (c) 2023 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import sys
import unittest

import pybddisasm

class TestNdGetVersion(unittest.TestCase):
    def test_nd_get_version_exception(self):
        self.assertRaises(Exception, pybddisasm.nd_get_version())

    def test_nd_get_version_output(self):
        major, minor, revision, build_date, build_time = pybddisasm.nd_get_version()

        print("\nversion = %ld.%ld.%ld (%s, %s)\n" % (major, minor, revision, build_date, build_time))

        self.assertIsInstance(major, int, "'Major' is not instance of 'int'")
        self.assertIsInstance(minor, int, "'Minor' is not instance of 'int'")
        self.assertIsInstance(revision, int, "'Revision' is not instance of 'int'")
        self.assertIsInstance(build_date, str, "'BuildDate' is not instance of 'str'")
        self.assertIsInstance(build_time, str, "'BuildDate' is not instance of 'str'")

if __name__ == '__main__':
    unittest.main()
