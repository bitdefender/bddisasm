#!/usr/bin/env python
#
# Copyright (c) 2020 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#

import os
import sys
import re

from setuptools import find_packages, setup, Command, Extension, Distribution
from codecs import open

VERSION = (0, 1, 0)

packages = ['pybddisasm']
requires = [ "setuptools"]
here = os.path.abspath(os.path.dirname(__file__))

with open('README.md', 'r', 'utf-8') as f:
    readme = f.read()

class BinaryDistribution(Distribution):
    """Distribution which always forces a binary package with platform name"""
    def has_ext_modules(foo):
        return True

    def is_pure(self):
        return False

setup(
    name="pybddisasm",
    version='.'.join(map(str, VERSION)),
    author="Bitdefender",
    description="The Bitdefender disassembler Python wrapper",
    long_description = readme,
    long_description_content_type = "text/markdown",
    url = "https://github.com/bitdefender/bddisasm",
    packages=packages,
    license="Apache Software License",
    package_data={'': ['LICENSE', 'NOTICE'], 'pybddisasm': ['*.pem']},
    package_dir={'pybddisasm': 'pybddisasm'},
    platforms = ["Windows", "Linux"],
    include_package_data=True,
    python_requires=">=3.4",
    setup_requires=['wheel'],
    install_requires=requires,
    zip_safe=False,
    classifiers=[
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'License :: OSI Approved :: Apache Software License',
        'Operating System :: OS Independent',
    ],
    ext_modules = [Extension("_pybddisasm",
                      sources = ["_pybddisasm/_pybddisasm.c", "_pybddisasm/pybddisasm.c"],
                      define_macros = [('AMD64', None), ('Py_LIMITED_API', None)],
                      include_dirs = ['../inc'],
                      libraries = ['bddisasm'],
                      py_limited_api=True)],
    distclass=BinaryDistribution
)
