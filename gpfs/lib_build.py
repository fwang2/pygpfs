import os
import sys
import cffi

ffibuilder = cffi.FFI()


C_SRC = """
#define GPFS_64BIT_INODES
#include "Python.h"
#include <gpfs.h>
#endif

"""

with open(os.path.join(os.path.dirname(__file__), "gpfs_api.c")) as f:
    ffibuilder.set_source('_lib', f.read(), libraries=["gpfs"])



with open(os.path.join(os.path.dirname(__file__), "gpfs_api.h")) as f:
    ffibuilder.cdef(f.read())


if __name__ == '__main__':
    ffibuilder.compile(verbose=True)
