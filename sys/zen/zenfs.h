#ifndef __ZEN_ZENFS_H__
#define __ZEN_ZENFS_H__

#include <stdint.h>
#iinclude <zero/cdefs.h>

typedef uint32_t zenfsword_t;
typedef uint16_t zenfshalf_t;
typedef uint8_t  zenfsbyte_t;
typedef uint64_t zenfssize_t;
typedef uint64_t zenfsoff_t;
typedef uint32_t zenfsino_t;
typedef uint32_t zenfsuid_t;
typedef uint32_t zenfsgid_t;
typedef uint64_t zenfstime_t;

#define ZEN_FS_BLOCK_SIZE       4096
#define ZEN_FS_DIRECT_BLOCKS    12
#define ZEN_FS_INDIRECT_BLOCKS  3
#define ZEN_FS_NAME_LEN         255     // filename without terminating NUL/'\0'
#define ZEN_FS_UNKNOWN_NODE     0
#define ZEN_FS_CHR_NODE         1
#define ZEN_FS_BLK_NODE         2
#define ZEN_FS_REG_NODE         3
#define ZEN_FS_DIR_NODE         4
#define ZEN_FS_FIFO_NODE        5
#define ZEN_FS_SOCK_NODE        6
#define ZEN_FS_LNK_NODE         7
/* struct zenfscommon is exactly 128 bytes */
struct zenfscommon {
    zenfssize_t size;   // file size in bytes, 0 means inode unused     0
    zenfstime_t atime;  // access time                                  8
    zenfstime_t mtime;  // modification time                            16
    zenfstime_t ctime;  // inode change time                            24
    zenfsuid_t  uid;    // owner user ID                                32
    zenfsgid_t  gid;    // owner group ID                               36
    zenfsword_t nlink;  // # of links to file                           40
    zenfsword_t nblk;   // # of 1K blocks used                          44
    zenfsword_t type;   // node type                                    48
    zenfsword_t flags;  // permissions                                  52
    zenfsword_t gen;    // generation ID                                56
    zenfsino_t  meta;   // 0 or metadata inode ID                       60
    zenfsino_t  extatr; // 0 or extended attributes inode ID            64
    zenfsino_t  dblks[ZEN_FS_DIRECT_BLOCKS];    // direct blocks        68
    zenfsino_t  iblks[ZEN_FS_INDIRECT_BLOCKS];  // indirect blocks      116
};

struct zenfsdirent {
    zenfsoff_t  ofs;            // 64-bit
    zenfsino_t  ino;            // 32-bit
    zenfshalf_t namelen;        // 16-bit lenght of name
    zenfsbyte_t type;           // ZEN_FS_*_NODE
    zenfsbyte_t _pad;           // pad to boundary of 32/64 bits
    zenfsbyte_t name[VLA];
};

#endif /* __ZEN_ZENFS_H__ */

