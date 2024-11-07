#!/usr/bin/env python
#
# Copyright (c) 2023 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#

import os
import sys
import re

from setuptools import find_packages, setup, Command, Extension, Distribution
from codecs import open

VERSION = (0, 3, 0)
LIBRARY_VERSION = (2, 3, 0)
DIR_INCLUDE = '../../inc'

here = os.path.abspath(os.path.dirname(__file__))

__swig_opts = ['-I%s' % (DIR_INCLUDE)]
__extra_compile_args = ['-march=westmere']
__sources = ['pybddisasm/pybddisasm.c', 'pybddisasm/pybddisasm.i']
__libraries = ['bddisasm']
__library_dirs = ['../../build', '../../bin/x64/Release']
__packages = ['pybddisasm']
__requires = ['setuptools']

class BinaryDistribution(Distribution):
    def has_ext_modules(arg):
        return True

    def is_pure(self):
        return False

def __fn_validate_compatibility():
    version_header = '%s/bddisasm_version.h' % (DIR_INCLUDE)
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

__fn_validate_compatibility()

with open('README.md', 'r', 'utf-8') as f:
    readme = f.read()

setup(
    name="pybddisasm",
    version='.'.join(map(str, VERSION)),
    author="Bitdefender",
    description="The Bitdefender disassembler Python wrapper",
    long_description = readme,
    long_description_content_type = "text/markdown",
    url = "https://github.com/bitdefender/bddisasm",
    license="Apache Software License",
    package_data={'': ['LICENSE', 'NOTICE'], 'pybddisasm': ['pybddisasm/pybddisasm.py']},
    package_dir={'pybddisasm': 'pybddisasm'},
    platforms = ["Windows", "Linux"],
    packages=__packages,
    include_package_data=True,
    py_modules = ["pybddisasm"],
    python_requires=">=3.5",
    setup_requires=['wheel'],
    install_requires=__requires,
    zip_safe=False,
    classifiers=[
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'License :: OSI Approved :: Apache Software License',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: POSIX :: Linux'
    ],
    ext_modules = [Extension("_pybddisasm",
                      swig_opts = __swig_opts,
                      extra_compile_args = __extra_compile_args,
                      sources = __sources,
                      define_macros = [('AMD64', None)],
                      include_dirs = [DIR_INCLUDE],
                      libraries = __libraries,
                      library_dirs = __library_dirs)],
    distclass=BinaryDistribution
)
