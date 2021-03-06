#ifndef __V0_MEM_H__
#define __V0_MEM_H__

#include <stddef.h>
#include <stdint.h>
#include <zero/hash.h>
#include <v0/v0.h>

#define V0_PAGE_EXEC            (1 << 0)        // page is executable [code]
#define V0_PAGE_WRITE           (1 << 1)        // page can be written on
#define V0_PAGE_READ            (1 << 2)        // page can be read
#define V0_PAGE_CORE            (1 << 3)        // page is present in RAM/caches
#define V0_PAGE_DIRTY           (1 << 4)        // page has been modified
#define V0_PAGE_WIRE            (1 << 5)        // page is wired to physical RAM
#define V0_PAGE_DISCARD         (1 << 6)        // page is I/O-buffer
#define V0_PAGE_BURST           (1 << 7)        // burst-mode memory transfars
#define V0_PAGE_DMA             (1 << 8)        // direct memory access I/O
#define V0_PAGE_SYS             (1 << 9)        // system/ring #0 page
#define V0_PAGE_GLOBAL          (1 << 10)       // system-global page
#define V0_PAGE_NONTEMP         (1 << 11)       // non-temporal, bypass cache
#define V0_PAGE_FLAGS           V0_PAGE_SHIFT

#define V0_PAGE_FRAME_MASK      (~(V0_PAGE_SIZE - 1))

#endif /* __V0_MEM_H__ */

