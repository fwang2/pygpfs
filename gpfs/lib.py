#
# Copyright (c) 2016
#
# Feiyi Wang / Oak Ridge National Laboratory
#
#

import os
from _lib import lib, ffi

GPFS_64BIT_INODES = True

def error(path=None):
    errno = ffi.errno
    strerror = os.strerror(ffi.errno)
    if path:
        raise IOError(errno, strerror, path)
    else:
        raise IOError(errno, strerror)


def _inode(d):
    st = os.stat(d)
    return st.st_ino


def _open_inodescan(fs, fssnap_id = ffi.NULL):
    if GPFS_64BIT_INODES:
        maxIno = ffi.new("gpfs_ino64_t*")
        iscan = lib.gpfs_open_inodescan64(fs, fssnap_id, maxIno)
    else:
        maxIno = ffi.new("gpfs_ino_t*")
        iscan = lib.gpfs_open_inode(fs, fssnap_id, maxIno)

    if iscan == ffi.NULL:
        raise error()
    else:
        return iscan, maxIno[0]

def _next_inode(iscan, termIno=0):
    if GPFS_64BIT_INODES:
        iattr = ffi.new("gpfs_iattr64_t **")
        res = lib.gpfs_next_inode64(iscan, termIno, iattr)
    else:
        iattr = ffi.new("gpfs_iattr_t **")
        res = lib.gpfs_next_inode(iscan, termIno, iattr)

    if res == 0:
        if iattr[0] == ffi.NULL:
            return None
        else:
            return InodeAttr(iattr[0])
    else:
        raise error()

def _close_inodescan(iscan):
    lib.gpfs_close_inodescan(iscan)

def _free_fssnaphandle(fs):
    lib.gpfs_free_fssnaphandle(fs)

def _get_fssnaphandle_by_path(pth):
    fs = lib.gpfs_get_fssnaphandle_by_path(pth)
    if fs == ffi.NULL:
        raise error(pth)
    else:
        return fs

class InodeAttr:
    def __init__(self, attr):
        self.ia_nlink = attr.ia_nlink
        self.ia_inode = attr.ia_inode
        self.ia_blocks = attr.ia_blocks
        self.ia_size = attr.ia_size
        self.ia_uid = attr.ia_uid
        self.ia_gid = attr.ia_gid

    def __repr__(self):
        return "ino=%s, size=%s" % (self.ia_inode, self.ia_size)

class DirEntry:
    def __init__(self, dirx, path):
        self.d_type, self.d_ino = dirx.d_type, dirx.d_ino
        if dirx.d_type == lib.GPFS_DE_DIR:
            self.type = "DIR"
        elif dirx.d_type == lib.GPFS_DE_REG:
            self.type = "REG"
        elif dirx.d_type == lib.GPFS_DE_LNK:
            self.type = "LNK"
        else:
            self.type = "UKN"

        self.d_name = ffi.string(dirx.d_name)
        self.d_path = os.path.join(path, self.d_name)

    def __repr__(self):
        return "dentry: ino=%s, type=%s, path=%s\n" % (self.d_ino, self.type, self.d_path)

def inode_max(path, fssnap_id = ffi.NULL):
    fs = _get_fssnaphandle_by_path(path)
    iscan, maxIno = _open_inodescan(fs, fssnap_id)
    _close_inodescan(iscan)
    _free_fssnaphandle(fs)
    return maxIno

def inode_scan(path, termIno=None, fssnap_id = ffi.NULL):
    lib.gpfs_lib_init(0)
    fs = _get_fssnaphandle_by_path(path)
    if fs == ffi.NULL:
        raise error()

    iscan, maxIno = _open_inodescan(fs, fssnap_id)
    if termIno is None:
        termIno = maxIno
    iattr = _next_inode(iscan, termIno)

    while iattr:
        yield iattr
        iattr = _next_inode(iscan, termIno)

    _close_inodescan(iscan)
    _free_fssnaphandle(fs)
    lib.gpfs_lib_term(0)

def dir_scan(path, inode = None):
    """
    if inode is None, acquire inode number through os.
    path is used to construct DirEntry() to provide as parent
    skip . and ..
    """
    if inode is None:
        inode = _inode(path)

    lib.gpfs_lib_init(0)

    fs = _get_fssnaphandle_by_path(path)
    if fs == ffi.NULL:
        raise error(path)

    dirx = lib.gpfs_iopen(fs, inode, lib.O_RDONLY, ffi.NULL, ffi.NULL)
    if dirx == ffi.NULL:
        raise error(path)

    dirent = ffi.new("gpfs_direntx_t **")

    while True:
        rc = lib.gpfs_ireaddir(dirx, dirent)
        if rc!=0:
            raise error()

        if dirent[0] == ffi.NULL:
           break

        if ffi.string(dirent[0].d_name) in [".", ".."]:
            continue
        else:
            yield DirEntry(dirent[0], path)

    _free_fssnaphandle(fs)
    lib.gpfs_lib_term(0)
