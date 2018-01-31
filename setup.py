#!/usr/bin/env python

import os
import sys

from setuptools import setup

if sys.platform == 'darwin':
    print("pygpfs wrapper doesn't support MacOS")
    sys.exit(1)

VERSION = '0.1'
DESCRIPTION = "GPFS python wrapper"

setup(
    name="pygpfs",
    version=VERSION,
    description=DESCRIPTION,
    author='Feiyi Wang',
    author_email='fwang2@gmail.com',
    license='Apache',
    packages=['gpfs'],
    ext_package='gpfs',
    platforms=['Linux'],
    install_requires=['cffi>=1.0.0'],
    setup_requires=['cffi>=1.0.0'],
    cffi_modules=['gpfs/lib_build.py:ffibuilder'],
    zip_safe=False,
)
