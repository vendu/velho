#ifndef __V0_MEM_H__
#define __V0_MEM_H__

#include <stddef.h>

/* page-table entry flag-bits */
#define V0_PAGE_EXEC   (1 << 0)
#define V0_PAGE_WRITE  (1 << 1)
#define V0_PAGE_READ   (1 << 2)
#define V0_PAGE_CORE   (1 << 3)
#define V0_PAGE_DIRTY  (1 << 4)
#define V0_PAGE_WIRE   (1 << 5)
#define V0_PAGE_BUF    (1 << 6)
#define V0_PAGE_DMA    (1 << 7)
#define V0_PAGE_SYS    (1 << 8)
#define V0_PAGE_GLOBAL (1 << 9)
#define V0_PAGE_ACCESS (1 << 10)

#define V0_SEG_EXEC    (1 << 0)
#define V0_SEG_WRITE   (1 << 1)
#define V0_SEG_READ    (1 << 2)
#define V0_SEG_SYS     (1 << 3)
#define V0_SEG_PERMS   (V0_SEG_EXEC | V0_SEG_WRITE | V0_SEG_READ | V0_SEG_SYS)
#define V0_SEG_DOWN    (1 << 4)
#define V0_SEG_DYN     (1 << 5)
#define V0_SEG_ZERO    (1 << 6)
#define V0_SEG_BYTES   (1 << 7)
#define V0_SEG_PARMS   (V0_SEG_DOWN | V0_SEG_DYN | V0_SEG_ZERO | V0_SEG_BYTES)

struct v0segdesc {
    v0pagedesc base;            // base address + flags
    v0pagedesc lim;             // limit (byte address or size in pages)
};

#endif /* __V0_MEM_H__ */

