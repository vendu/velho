#ifndef __ZEN_TYPES_H__
#define __ZEN_TYPES_H__

#include <stddef.h>
#include <stdint.h>
#if defined(__v0__)
#include <zen/sys/v0.h>
#endif
#include <zen/types.h>

typedef intmax_t        zenlong;        // system maximum
typedef uintmax_t       zenulong;       // unsigned system maximum
typedef int32_t         zenid_t;        // generic object ID
typedef uint32_t        zendev_t;       // device type
typedef int64_t         zenoff_t;       // file-system offset
typedef int32_t         zenuid_t;       // user ID
typedef int32_t         zengid_t;       // group ID
typedef uint32_t        zenperm_t;      // I/O permission flags

struct zendev {
    struct zenvfsfuncs *funcs;
    uint32_t            dev;    // 16-bit major + 16-bit minor device IDs
    uint16_t            bus;
    uint16_t            flg;
};

#define MEM_NULL_FLAGS          0
#define MEM_CODE_FLAGS          (ZEN_MEM_EXEC | ZEN_MEM_READ)
#define MEM_RODATA_FLAGS        ZEN_MEM_READ
#define MEM_DATA_FLAGS          (ZEN_MEM_WRITE | ZEN_MEM_READ | ZEN_MEM_ZERO)
#define MEM_HEAP_FLAGS          (ZEN_MEM_WRITE | ZEN_MEM_READ | ZEN_MEM_DYNAMIC)
#define MEM_USRSTK_FLAGS        (ZEN_MEM_WRITE | ZEN_MEM_READ           \
                                 | ZEN_MEM_GROW_DOWN)
#define MEM_SYS_FLAGS           (ZEN_MEM_EXEC | ZEN_MEM_READ | ZEN_MEM_SYS)
#define MEM_SYSSTK_FLAGS        (ZEN_MEM_WRITE | ZEN_MEM_READ           \
                                 | ZEN_MEM_GROW_DOWN | ZEN_MEM_SYS)
#define ZEN_NULL_SEG            0
#define ZEN_CODE_SEG            1
#define ZEN_RODATA_SEG          2
#define ZEN_DATA_SEG            3
#define ZEN_HEAP_SEG            4
#define ZEN_USRSTK_SEG          5
#define ZEN_SYS_SEG             6
#define ZEN_SYSSTK_SEG          7
#define ZEN_MEM_DYNAMIC         ZEN_MEM_FLAG
#define ZEN_MEM_ZERO            (ZEN_MEM_FLAG << 1)
struct zenseg {
    m_adr_t             base;
    m_adr_t             lim;
    m_uword_t           size;
    m_uword_t           flags;
};

struct zenmap {
    m_adr_t             adr;
    m_uword_t           size;
    m_uword_t           flags;
    struct zencred      cred;
};

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

struct zendesc {
    m_adr_t    buf;             // buffer [page] address
    m_adr_t    ofs;             // buffer offset
    uint32_t   flg;
};

};

struct zensys {
    struct zentask     *systasktab[ZEN_SYS_TASKS];
    struct zentask     *tasktab[ZEN_PROC_TASKS];
    m_page_t           *pagedir[ZEN_PAGE_DIR_ITEMS];
    m_uword_t           ndesc;
    m_desc_t           *desctab;
    m_uword_t           nnodehash;
    struct zenvfsnode  *nodehash;
    m_uword_t           nmembuf;
    struct zenmembuf   *membuftab;
};

#endif /* __ZEN_TYPES_H__ */

