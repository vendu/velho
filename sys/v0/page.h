#ifndef __V0_PAGE_H__
#define __V0_PAGE_H__

/* page-table entry flag-bits */
#define V0_PAGE_EXEC   (1 << 0)         // execute permission
#define V0_PAGE_WRITE  (1 << 1)         // write permission
#define V0_PAGE_READ   (1 << 2)         // read permission
#define V0_PAGE_SYS    (1 << 3)         // system (ring 0) page
#define V0_PAGE_CORE   (1 << 4)         // page present in RAM
#define V0_PAGE_DIRTY  (1 << 5)         // page has been written to
#define V0_PAGE_WIRE   (1 << 6)         // page is wired to RAM
#define V0_PAGE_BUF    (1 << 7)         // page belongs to buffer cache zone
#define V0_PAGE_BURST  (1 << 8)         // use combined writes (burst writes)
#define V0_PAGE_GLOBAL (1 << 9)         // page may be shared across processes
#define V0_PAGE_TOUCH  (1 << 10)        // page has been accessed
#define V0_PAGE_DIRECT (1 << 11)        // page is not cached

#endif /* __V0_PAGE_H__ */

