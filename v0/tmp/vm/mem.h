#ifndef __V0_VM_MEM_H__
#define __V0_VM_MEM_H__

/* VIRTUAL MEMORY SUPPORT */
#define V0_PAGE_SIZE_LOG2 12
#define V0_PAGE_PRESENT   (1U << 0)
#define V0_PAGE_WRITE     (1U << 1)
#define V0_PAGE_READ      (1U << 2)
#define V0_PAGE_UNCACHED  (1U << 3)
#define V0_PAGE_WRITEBACK (1U << 4)
#define V0_PAGE_ACCESSED  (1U << 5)
#define V0_PAGE_DIRTY     (1U << 6)
#define V0_PAGE_HUGE      (1U << 7)
#define V0_PAGE_GLOBAL    (1U << 8)
#define V0_PAGE_FLAG1     (1U << 9)
#define V0_PAGE_FLAG2     (1U << 10)
#define V0_PAGE_FLAG3     (1U << 11)
#define V0_PAGE_MASK      0xfffff000

#endif /* __V0_VM_MEM_H__ */

