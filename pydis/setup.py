#!/usr/bin/env python
#
# Copyright (c) 2020 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#

import os

from codecs import open

from setuptools import find_packages, setup, Command, Extension

here = os.path.abspath(os.path.dirname(__file__))

packages = ['pydis']

requires = [
	"setuptools"
]

about = {}
with open(os.path.join(here, 'pydis', '__version__.py'), 'r', 'utf-8') as f:
    exec(f.read(), about)

with open('README.md', 'r', 'utf-8') as f:
    readme = f.read()

setup(
    name=about['__title__'],
    version=about['__version__'],
    packages=packages,
    package_data={'': ['LICENSE', 'NOTICE'], 'pydis': ['*.pem']},
    package_dir={'pydis': 'pydis'},
    include_package_data=True,
    python_requires=">=3.4",
    setup_requires=['wheel'],
    install_requires=requires,
    zip_safe=False,
    classifiers=[
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: Implementation :: CPython',
        'Programming Language :: Python :: Implementation :: PyPy'
    ],
    ext_modules = [Extension("_pydis",
                      sources = ["_pydis/_pydis.c", "_pydis/pydis.c"],
                      define_macros = [('AMD64', None), ('PYDIS_BUILD', None)],
                      include_dirs = ['../inc'],
                      libraries = ['bddisasm'],
                      library_dirs = ['../bin/x64/Release'])]
)
