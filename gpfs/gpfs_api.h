/*
 * fwang2@gmail.com
 */

#define O_RDONLY ...

#define GPFS_DE_OTHER   0
#define GPFS_DE_DIR     4
#define GPFS_DE_REG     8
#define GPFS_DE_LNK     10
#define GPFS_DE_DEL     16



typedef int... gpfs_off64_t;
typedef int... gpfs_mode_t;
typedef int... gpfs_dev_t;
typedef int... gpfs_mask_t;
typedef int... gpfs_pool_t;

typedef int... gpfs_ino_t;
typedef int... gpfs_ino64_t;

typedef int... gpfs_gen_t;
typedef int... gpfs_gen64_t;

typedef int... gpfs_gid_t;
typedef int... gpfs_gid64_t;

typedef int... gpfs_uid_t;
typedef int... gpfs_uid64_t;

typedef int... gpfs_nlink_t;
typedef int... gpfs_nlink64_t;

typedef int... gpfs_snapid_t;
typedef int... gpfs_snapid64_t;

typedef int gpfs_file_t;
typedef struct gpfs_ifile gpfs_ifile_t;
typedef struct gpfs_iscan gpfs_iscan_t;
typedef struct gpfs_fssnap_handle gpfs_fssnap_handle_t;

typedef struct gpfs_fssnap_id {
    char opaque[48];
} gpfs_fssnap_id_t;


typedef struct gpfs_timestruc
{
  long long    tv_sec;
  unsigned int tv_nsec;
} gpfs_timestruc_t;


typedef struct gpfs_timestruc64
{
  long long    tv_sec;
  unsigned int tv_nsec;
} gpfs_timestruc64_t;


typedef struct gpfs_direntx
{
  int            d_version;     /* this struct's version */
  unsigned short d_reclen;      /* actual size of this struct including
                                   null terminated variable length d_name */
  unsigned short d_type;        /* Types are defined below */
  gpfs_ino_t     d_ino;         /* File inode number */
  gpfs_gen_t     d_gen;         /* Generation number for the inode */
  char d_name[...]; /* 256 */
} gpfs_direntx_t;



typedef struct gpfs_direntx64
{
  int d_version;
  unsigned short d_reclen;
  unsigned short d_type;
  gpfs_ino64_t d_ino;
  gpfs_gen64_t d_gen;
  unsigned int d_flags;
  char d_name[...]; /* 1028 */
} gpfs_direntx64_t;


/* fill this up if 32-bit support is required */
typedef struct gpfs_iattr {
    ...;
} gpfs_iattr_t;

typedef struct gpfs_iattr64
{
  int                ia_version;    /* this struct version */
  int                ia_reclen;     /* sizeof this structure */
  int                ia_checksum;   /* validity check on iattr struct */
  gpfs_mode_t        ia_mode;       /* access mode */
  gpfs_uid64_t       ia_uid;        /* owner uid */
  gpfs_gid64_t       ia_gid;        /* owner gid */
  gpfs_ino64_t       ia_inode;      /* file inode number */
  gpfs_gen64_t       ia_gen;        /* inode generation number */
  gpfs_nlink64_t     ia_nlink;      /* number of links */
  gpfs_off64_t       ia_size;       /* file size in bytes */
  gpfs_off64_t       ia_blocks;     /* 512 byte blocks of disk held by file */
  gpfs_timestruc64_t ia_atime;      /* time of last access */
  gpfs_timestruc64_t ia_mtime;      /* time of last data modification */
  unsigned int       ia_flags;      /* flags (defined below) */
  /* next four bytes were ia_pad2 */
  unsigned char      ia_repl_data;  /* data replication factor */
  unsigned char      ia_repl_data_max; /* data replication max factor */
  unsigned char      ia_repl_meta;  /* meta data replication factor */
  unsigned char      ia_repl_meta_max; /* meta data replication max factor */
  gpfs_timestruc64_t ia_ctime;      /* time of last status change */
  int                ia_blocksize;  /* preferred block size for io */
  gpfs_timestruc64_t ia_createtime; /* creation time */
  gpfs_mask_t        ia_mask;       /* initial attribute mask (not used) */
  unsigned int       ia_xperm;      /* extended attributes (defined below) */
  gpfs_dev_t         ia_dev;        /* id of device containing file */
  gpfs_dev_t         ia_rdev;       /* device id (if special file) */
  unsigned int       ia_pcacheflags; /* pcache inode bits */
  gpfs_snapid64_t    ia_modsnapid;  /* snapshot id of last modification */
  unsigned int       ia_filesetid;  /* fileset ID */
  unsigned int       ia_datapoolid; /* storage pool ID for data */
  gpfs_ino64_t       ia_inode_space_mask; /* inode space mask of this file system */
                                          /* This value is saved in the iattr structure
                                             during backup and used during restore */
  ...;
} gpfs_iattr64_t;



/* fssnap */
gpfs_fssnap_handle_t *
gpfs_get_fssnaphandle_by_path(const char *pathName);

gpfs_fssnap_handle_t *
gpfs_get_fssnaphandle_by_name(const char *fsName, const char *snapName);

void gpfs_free_fssnaphandle(gpfs_fssnap_handle_t* fssnapHandle);

int gpfs_get_fssnapid_from_fssnaphandle(
    gpfs_fssnap_handle_t *fssnapHandle,
    gpfs_fssnap_id_t* fssnapId);


/* open inodescan */

gpfs_iscan_t *gpfs_open_inodescan(
    gpfs_fssnap_handle_t *fssnapHandle,
    const gpfs_fssnap_id_t *prev_fssnapId,
    gpfs_ino_t *maxIno);


gpfs_iscan_t *gpfs_open_inodescan64(
    gpfs_fssnap_handle_t *fssnapHandle,
    const gpfs_fssnap_id_t *prev_fssnapId,
    gpfs_ino64_t *maxIno);


/* read next inode */

int gpfs_next_inode(
    gpfs_iscan_t *iscan,
    gpfs_ino_t  termIno,
    const gpfs_iattr_t **iattr);


int gpfs_next_inode64(
    gpfs_iscan_t *iscan,
    gpfs_ino64_t  termIno,
    const gpfs_iattr64_t **iattr);


/* obtain a snapshot name using its file system snapshot handle */
const char *gpfs_get_snapname_from_fssnaphandle(
    gpfs_fssnap_handle_t *fssnapHandle);

/* close a file given its inode handle */
void gpfs_iclose(gpfs_ifile_t *ifile);


/* Opens a file or directory by inode number.*/

gpfs_ifile_t *gpfs_iopen(
    gpfs_fssnap_handle_t *fssnapHandle,
    gpfs_ino_t ino,
    int open_flags,
    const gpfs_iattr_t *statxbuf,
    const char *symLink);

gpfs_ifile_t *gpfs_iopen64(
    gpfs_fssnap_handle_t *fssnapHandle,
    gpfs_ino64_t ino,
    int open_flags,
    const gpfs_iattr64_t *statxbuf,
    const char *symLink);

/* read next directory entry*/

int gpfs_ireaddir(
    gpfs_ifile_t *idir,
    const gpfs_direntx_t **dirent);


int gpfs_ireaddir64(
    gpfs_ifile_t *idir,
    const gpfs_direntx64_t **dirent);


/* Advances an inode scan to the specified inode number. */

int gpfs_seek_inode(
    gpfs_iscan_t *iscan,
    gpfs_ino_t ino);



int gpfs_seek_inode64(
    gpfs_iscan_t *iscan,
    gpfs_ino64_t ino);



/* re-allocates disk storage for a GPFS file. */
int gpfs_prealloc(
    gpfs_file_t fileDesc,
    gpfs_off64_t startOffset,
    gpfs_off64_t bytesToPrealloc);


/* set up repeated gpfs api calls */
int gpfs_lib_init(int flags);
int gpfs_lib_term(int flags);


void gpfs_close_inodescan(gpfs_iscan_t *iscan);
