#ifndef __ZEN_PAGE_H__
#define __ZEN_PAGE_H__

#include <stddef.h>
#include <stdint.h>
#include <velho/hash.h>
#include <v0/v0.h>

#define ZEN_PAGE_EXEC           (1 << 0)        // page is executable [code]
#define ZEN_PAGE_WRITE          (1 << 1)        // page can be written on
#define ZEN_PAGE_READ           (1 << 2)        // page can be read
#define ZEN_PAGE_READ           (1 << 3)        // page uset-accessible
#define ZEN_PAGE_CORE           (1 << 4)        // page is present in RAM/caches
#define ZEN_PAGE_DIRTY          (1 << 5)        // page has been modified
#define ZEN_PAGE_WIRE           (1 << 6)        // page is wired to physical RAM
#define ZEN_PAGE_BUF            (1 << 7)        // page is I/O-buffer
#define ZEN_PAGE_SYS            (1 << 8)        // system/ring #0 page
#define ZEN_PAGE_GLOBAL         (1 << 9)       // system-global page
#define ZEN_PAGE_NONTEMP        (1 << 10)       // non-temporal, bypass cache
#define ZEN_PAGE_FLAGS          11

#define ZEN_PAGE_FRAME_MASK     (~(MEM_PAGE_SIZE - 1))

#define ZEN_PAGE_NULL_FLAGS          0
#define ZEN_PAGE_CODE_FLAGS          (ZEN_MEM_EXEC | ZEN_MEM_READ)
#define ZEN_PAGE_RODATA_FLAGS        ZEN_MEM_READ
#define ZEN_PAGE_DATA_FLAGS          (ZEN_MEM_WRITE | ZEN_MEM_READ | ZEN_MEM_ZERO)
#define ZEN_PAGE_HEAP_FLAGS          (ZEN_MEM_WRITE | ZEN_MEM_READ | ZEN_MEM_DYNAMIC)
#define ZEN_PAGE_USR_STK_FLAGS        (ZEN_MEM_WRITE | ZEN_MEM_READ  | ZEN_MEM_GROW_DOWN)
#define ZEN_PAGE_SYS_STK_FLAGS        (ZEN_MEM_WRITE | ZEN_MEM_READ  | ZEN_MEM_GROW_DOWN | ZEN_MEM_SYS)
#define ZEN_PAGE_SYS_FLAGS           (ZEN_MEM_EXEC | ZEN_MEM_READ | ZEN_MEM_SYS)

struct zenpage {
    m_uword_t           blk;    // page-device block ID
    zendev_t            dev;    // page-device
    void               *pte;    // pointer to page-table entry
    m_uword_t           flags;  // page-flags
    m_uword_t           nflt;   // # of page-faults
    m_uword_t           qid;    // lruq, sizeof(m_word_t) * CHAR_BIT - clz(nflt)
    struct zenpage     *prev;   // previous in queue
    struct zenpage     *next;   // next in queue
};

#endif /* __ZEN_PAGE_H__ */

