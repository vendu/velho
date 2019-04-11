#ifndef __ZEN_TYPES_H__
#define __ZEN_TYPES_H__

#include <stddef.h>
#include <stdint.h>
#if defined(__v0__)
#include <zen/bsp/v0.h>
#endif
#include <zen/types.h>

typedef intmax_t        zenlong;        // system maximum
typedef uintmax_t       zenulong;       // unsigned system maximum
typedef int32_t         zenid_t;        // generic object ID
typedef uint32_t        zendev_t;       // device type
typedef int64_t         zenoff_t;       // file-system offset
typedef int32_t         zenuid_t;       // user ID
typedef int32_t         zengid_t;       // group ID

union m_task {
#if defined(__v0__)
    struct v0tcb        v0;
#endif
};

#define ZEN_ROOT_UID    INT32_C(0)
#define ZEN_ROOT_GID    INT32_C(0)
#define ZEN_NO_GID      (~INT32_C(0))
struct zencred {
    zenuid_t            uid;
    zengid_t            gid;
    m_uword_t           nxgid;
    zengid_t           *xgidtab;
};

struct zenacl {
    void               *data;
    m_word_t          (*chk)(zenuid_t uid, zenuid_t gid, struct zenacl *acl);
};

struct zenperm {
    m_uword_t           flags;
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
    struct zencred     *cred;
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

struct zensys {
    struct zentask     *systasktab[ZEN_SYS_TASKS];
    struct zentask     *tasktab[ZEN_PROC_TASKS];
    struct m_page_t    *pagedir[ZEN_PAGE_DIR_ITEMS];
    m_uword_t           ndesc;
    m_desc_t           *desctab;
    m_uword_t           nnodehash;
    struct zenvfsnode  *nodehash;
    m_uword_t           nmembuf;
    struct zenmembuf   *membuftab;
};

#endif /* __ZEN_TYPES_H__ */

