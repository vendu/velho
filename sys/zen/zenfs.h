#ifndef __ZEN_ZENFS_H__
#define __ZEN_ZENFS_H__

/* zenfs aims to be a somewhat-modernised version of berkeley ffs */

#include <stdint.h>
#include <zero/cdefs.h>

#define ZEN_FS                  1
#define ZENFS_NAME              "zenfs"

typedef uint32_t zenfsword_t;
typedef uint16_t zenfshalf_t;
typedef uint8_t  zenfsbyte_t;
typedef uint64_t zenfssize_t;
typedef uint64_t zenfsoff_t;
typedef uint32_t zenfsino_t;
typedef uint32_t zenfsuid_t;
typedef uint32_t zenfsgid_t;
typedef uint64_t zenfstime_t;

#define ZEN_FS_SOFTUP_BIT       (1 << 0)
#define ZEN_FS_JOURNAL_BIT      (1 << 1)
#define ZEN_FS_DIRTY_BIT        (1 << 2)
#define ZEN_FS_MOUNT_BIT        (1 << 3)
#define ZEN_FS_MAGIC            0x13620348
#define ZEN_FS_BLOCK_SIZE       4096    // filesystem disk block size
#define ZEN_FS_DIRECT_BLOCKS    12      // # of direct blocks allocated in inode
#define ZEN_FS_INDIRECT_BLOCKS  4       // # of direct block IDs in inode
#define ZEN_FS_NAME_LEN         255     // filename without terminating NUL/'\0'
#define ZEN_FS_LINK_SIZE        64      // filename embedded into on-disk inode
#define ZEN_FS_UNKNOWN_NODE     ZEN_VFS_UNKNOWN_NODE    // uninitialised type
#define ZEN_FS_CHR_NODE         ZEN_VFS_CHR_NODE        // character device
#define ZEN_FS_BLK_NODE         ZEN_VFS_BLK_NODE        // block device
#define ZEN_FS_REG_NODE         ZEN_VFS_FILE_NODE       // regular file object
#define ZEN_FS_DIR_NODE         ZEN_VFS_DIR_NODE        // directory object
#define ZEN_FS_FIFO_NODE        ZEN_VFS_FIFO_NODE       // pipe/FIFO object
#define ZEN_FS_SOCK_NODE        ZEN_VFS_SOCK_NODE       // socket object
#define ZEN_FS_LNK_NODE         ZEN_VFS_SYMLINK_NODE    // symbolic link
/* exactly 64 bytes */
struct zenfsblktabs {
    zenfsino_t          dir[ZEN_FS_DIRECT_BLOCKS];      // direct blocks
    zenfsino_t          indir[ZEN_FS_INDIRECT_BLOCKS];  // indirect blocks
};
/* exactly 64 bytes */
union zenfslink {
    struct zenfsblktabs blktabs;                // fs block book-keeping
    uint8_t             name[ZEN_FS_LINK_SIZE]; // CHR, BLK, FIFO, SOCK, LNK
};
/* struct zenfscommon is exactly 128 bytes and on-disk verbatim */
struct zenfscommon {
    zenfssize_t         size;   // file size in bytes                   0
    zenfssize_t         nblk;   // # of 1K blocks used                  8
    zenfstime_t         atime;  // access time                          16
    zenfstime_t         mtime;  // modification time                    24
    zenfstime_t         ctime;  // inode change time                    32
    zenfsuid_t          uid;    // owner user ID                        40
    zenfsgid_t          gid;    // owner group ID                       44
    zenfshalf_t         nlink;  // # of links to file                   48
    zenfsbyte_t         type;   // node type                            50
    zenfsbyte_t         ver;    // filesystem version                   51
    zenfsword_t         gen;    // generation ID                        52
    zenfsino_t          meta;   // 0 or metadata inode ID               56
    zenfsino_t          extatr; // 0 or extended attributes inode ID    60
    union zenfslink     link;   // block book-keeping or object name    64
};

struct zenfsdirent {
    zenfssize_t         ofs;            // 64-bit offset
    zenfsino_t          ino;            // 32-bit inode number
    zenfshalf_t         namelen;        // 16-bit length of name
    zenfsbyte_t         type;           // ZEN_FS_*_NODE
    zenfsbyte_t         ver;            // filesystem version
    zenfsbyte_t         name[VLA];      // padded to 64-bit boundary
};

#endif /* __ZEN_ZENFS_H__ */

