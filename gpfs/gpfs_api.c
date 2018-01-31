#include <Python.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/*
 * When GPFS_64BIT_INODES is defined, use the 64-bit interface definitions
 * as the default.
 *
 * #define GPFS_64BIT_INODES
 *
 * We will support 32-bit/64-bit in python wrapper code
 *
 */

#include <gpfs.h>
