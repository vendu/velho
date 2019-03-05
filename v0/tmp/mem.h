#ifndef __V0_MEM_H__
#define __V0_MEM_H__

#include <stddef.h>

/* page-table entry flag-bits */
#define V0_MEM_EXEC   (1 << 0)
#define V0_MEM_WRITE  (1 << 1)
#define V0_MEM_READ   (1 << 2)
#define V0_MEM_CORE   (1 << 3)
#define V0_MEM_DIRTY  (1 << 4)
#define V0_MEM_WIRE   (1 << 5)
#define V0_MEM_BUF    (1 << 6)
#define V0_MEM_DMA    (1 << 7)
#define V0_MEM_SYS    (1 << 8)
#define V0_MEM_GLOBAL (1 << 9)
#define V0_MEM_ACCESS (1 << 10)

struct v0memcred {
    long pid;                   // ID of owning process
    long uid;                   // ID of owner user
    long gid;                   // ID of owner group
    long perm;                  // access permissions
};

struct v0memseg {
    struct v0memcred  cred;     // segment credentials
    void             *base;     // segment base address
    size_t            size;     // segment size
    long              flg;      // segment flags
    long              _pad;     // pad structure to 8 machine words
};

#endif /* __V0_MEM_H__ */

