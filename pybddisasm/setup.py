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

VERSION = (0, 1, 2)
LIBRARY_VERSION = (1, 28, 0)
LIBRARY_INSTRUX_SIZE = 856

packages = ['pybddisasm']
requires = ['setuptools']
here = os.path.abspath(os.path.dirname(__file__))

def _check_library_version():
    version_header = '../inc/version.h'
    with open(version_header, 'r') as file:
        data = file.read()

    major = re.search(r'(?<=\bDISASM_VERSION_MAJOR).*(\d+)', data)
    if not major:
        print('error: Major version not found!')
        sys.exit(1)

    minor = re.search(r'(?<=\bDISASM_VERSION_MINOR).*(\d+)', data)
    if not minor:
        print('error: Minor version not found!')
        sys.exit(1)

    revision = re.search(r'(?<=\bDISASM_VERSION_REVISION).*(\d+)', data)
    if not revision:
        print('error: Revision version not found!')
        sys.exit(1)

    major = major.group(0).strip()
    minor = minor.group(0).strip()
    revision = revision.group(0).strip()

    if int(major) != LIBRARY_VERSION[0] or int(minor) != LIBRARY_VERSION[1] or int(revision) != LIBRARY_VERSION[2]:
        print('error: The version of the library is not compatible with the pybddisasm!')
        print('error: Library : %s.%s.%s - pybddisasm : %d.%d.%d' % (major, minor, revision, LIBRARY_VERSION[0],
               LIBRARY_VERSION[1], LIBRARY_VERSION[2]))
        sys.exit(1)

_check_library_version()

with open('README.md', 'r', 'utf-8') as f:
    readme = f.read()

class BinaryDistribution(Distribution):
    def has_ext_modules(arg):
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
                      define_macros = [('AMD64', None), ('Py_LIMITED_API', None), ('LIBRARY_INSTRUX_SIZE', LIBRARY_INSTRUX_SIZE)],
                      include_dirs = ['../inc'],
                      libraries = ['bddisasm'],
                      library_dirs = ['/usr/local/lib', '../bin/x64/Release'],
                      py_limited_api=True)],
    distclass=BinaryDistribution
)
