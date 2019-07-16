#ifndef __V0_CONF_H__
#define __V0_CONF_H__

#define V0_MACHINE_TYPE      V0_PAPILIO_PRO
#define V0_PAPILIO_PRO       0

#if (V0_MACHINE_TYPE == V0_PAPILI0_PRO)
#include <v0/hw/papilio.h>
#else
#errof machine-type not specified in <v0/conf.h>
#endif

#define V0_SERVER_CONF       0
#define V0_DESKTOP_CONF      0
#define V0_LOW_LATENCY_CONF  1

#if (V0_LOW_LATENCY_DESKTOP)
#define V0_TMR_HZ            512
#define V0_TMR_SLICE_MS      2
#elif (V0_DESKTOP_CONF)
#define V0_TMR_HZ            128
#define V0_TMR_SLICE_MS      8
#elif (V0_SERVER_CONF
#define V0_TMR_HZ            32
#define V0_TMR_SLICE         32
#else
#errof hots-type not specified in <v0/conf.h>
#endif
#define V0_PAGE_SIZE         4096               // system page size
#define V0_HUGE_PAGE_SIZE    131072             // system huge-page size
#define V0_FB_ADR            0xe0000000         // 3.5 gigabytes
#define V0_PAGE_SHIFT        12                 // log2(V0_PAGE_SIZE)
#define V0_CACHE_LINE_SIZE   32                 // cacheline size
#define V0_CACHE_LINE_SHIFT  5                  // log2(V0_CACHE_LINE_SIZE)
#define V0_CACHE_STRIDE_SIZE 128                // cache fetch size
#define V0_WORD_SIZE         4                  // native user register size
#define V0_MAX_INSTS         256                // max # of instruction IDs
#define _MEGABYTE            (1024U * 1024U)
#define V0_INTR_VECTOR       0x00000000         // system interrupt vector
#define V0_TEXT_ADR          V0_PAGE_SIZE       // system text segment address
#define V0_SYS_STACK         0x00000000         // system stack top
#if defined(V0_THREADS)
#define V0_TASK_STACK_SIZE   (4 * V0_PAGE_SIZE) // per-thread stack size
#define V0_PROC_THREADS      16                 // max # of per-process threads
#define V0_MAX_THREADS       256                // max # of system threads
#define V0_THR_ID_BITS       8                  // # of bits in thread-IDs
#endif

/* optional features */
#define V0_PTHREAD           1                  // POSIX-thread support

#endif /* __V0_CONF_H__ */

