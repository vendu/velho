#ifndef __ZEN_VM_H__
#define __ZEN_VM_H__

/* zen kernel virtual memory interface */
#include <v0/conf.h>
#include <v0/mem.h>

#define ZEN_PAGE_SIZE           V0_PAGE_SIZE
#define ZEN_PAGE_SHIFT          V0_PAGE_SHIFT

/* page-table entry flag-bits */
#define ZEN_PAGE_EXEC   	(1 << 0)        // page is executable [code]
#define ZEN_PAGE_WRITE  	(1 << 1)        // page can be written on
#define ZEN_PAGE_READ   	(1 << 2)        // page can be read
#define ZEN_PAGE_CORE   	(1 << 3)        // page is present in RAM/caches
#define ZEN_PAGE_DIRTY  	(1 << 4)        // page has been modified
#define ZEN_PAGE_WIRE   	(1 << 5)        // page is wired to physical RAM
#define ZEN_PAGE_DISCARD 	(1 << 6)        // page is I/O-buffer
#define ZEN_PAGE_BURST  	(1 << 7)        // burst-mode memory transfars
#define ZEN_PAGE_DMA    	(1 << 8)        // direct memory access I/O
#define ZEN_PAGE_SYS    	(1 << 9)        // system/ring #0 page
#define ZEN_PAGE_GLOBAL 	(1 << 10)       // system-global page
#define ZEN_PAGE_NONTEMP 	(1 << 11)       // non-temporal, bypass cache
#define ZEN_PAGE_FLAGS          ZEN_PAGE_SHIFT
#define ZEN_PAGE_BITS           (UINT32_C(ZEN_PAGE_SIZE - 1))
#define ZEN_PAGE_FRAME  	(~UINT32_C(ZEN_PAGE_SIZE - 1))

struct vmmempage {
    int8_t             *ptr;
    uintptr_t           flg;
    struct vmmempage   *prev;
    struct vmmempage   *next;
};

struct vmmemseg {
    const char *name;
    int8_t     *ptr;
    size_t      size;
    uintptr_t   flg;
};

#endif /* __ZEN_VM_H__ */

