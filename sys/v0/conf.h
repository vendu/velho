#ifndef __V0_CONF_H__
#define __V0_CONF_H__

#include <v0/hw/papilio.h>

#define V0_PAGE_SIZE         4096               // system page size
#define V0_FB_ADR            0xe0000000         // 3.5 gigabytes
#define V0_PAGE_SHIFT        12                 // log2(V0_PAGE_SIZE)
#define V0_CACHE_LINE_SIZE   32                 // cacheline size
#define V0_CACHE_LINE_SHIFT  5                  // log2(V0_CACHE_LINE_SIZE)
#define V0_CACHE_STRIDE_SIZE 128                // cache fetch size
#define V0_WORD_SIZE         4                  // native user register size
#define V0_MAX_INSTS         256                // max # of instruction IDs
#define _MEGABYTE            (1024U * 1024U)
#define V0_INTR_VECTOR       0x00000000         // system interrupt vector
#define V0_SYS_STACK         0x00000000         // system stack top
#define V0_TEXT_ADR          V0_PAGE_SIZE       // system text segment address
#if defined(V0_THREADS)
#define V0_TASK_STACK_SIZE   (4 * V0_PAGE_SIZE) // per-thread stack size
#define V0_NTHR              16
#endif

#endif /* __V0_CONF_H__ */

