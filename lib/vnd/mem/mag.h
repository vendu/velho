#ifndef __MEM_MAG_H__
#define __MEM_MAG_H__

#include <mem/mem.h>

#define MEM_SMALL_BLK           0
#define MEM_RUN_BLK             1
#define MEM_BIG_BLK             2

#define MEM_SMALL_SLAB          (2 * SYS_PAGE_SIZE)
#define MEM_SMALL_MIN     	SYS_CL_SIZE
#define MEM_SMALL_MAX     	(MEM_SMALL_SLAB / 2)
#define MEM_BLK_SMALL_MAX       (MEM_SMALL_QUEUES / 4)
#define MEM_BLK_BIG_MIN         (MEM_SMALL_QUEUES / 2)
#define MEM_SMALL_QUEUES  	(MEM_SMALL_SLAB / MEM_SMALL_MIN)
#define MEM_RUN_SLAB       	(MEM_RUN_QUEUES * SYS_PAGE_SIZE))
#define MEM_RUN_MIN       	SYS_PAGE_SIZE
#define MEM_RUN_MAX             (MEM_RUN_QUEUES * MEM_RUN_MIN)
/* fixed for run allocation bitmap of a single long */
#define MEM_RUN_QUEUES          (CHAR_BIT * sizeof(long))
#define MEM_RUN_SMALL_MAX       (MEM_RUN_QUEUES / 4)
#define MEM_RUN_BIG_MIN         (MEM_RUN_QUEUES / 2)

/*
 * - small allocation blocks (SYS_CL_SIZE to 2 * SYS_PAGE_SIZE) are allocated from
 *   slabs of 4 * SYS_PAGE_SIZE
 *   - sizes are multiples of 2 * SYS_CL_SIZE
 * - mid-size allocations (2 * SYS_PAGE_SIZE + 1 to 128 * SYS_PAGE_SIZE) are served from
 *   buffers of 256 * SYS_PAGE_SIZE
 *   - sizes are multiples of 4 * SYS_PAGE_SIZE
 * - big allocations are mapped directly and not buffered in queues at all
 */

#define membufsize()            (roundup2(sizeof(struct membuf), VAS_CL_SIZE))
#define memblkqueue(sz)         ((sz) >> SYS_CL_SIZE_LOG2)
#define memblksize(q)           (((q) + 1) << SYS_CL_SIZE_LOG2)
#define memrunqueue(sz)         ((sz) >> SYS_PAGE_SIZE_LOG2)
#define memrunblksize(q)        (((q) + 1) << SYS_PAGE_SIZE_LOG2)
#define memblktype(sz)                                                  \
    (((sz) <= MEM_SMALL_MAX)                                            \
     ? MEM_SMALL_BLK                                                    \
     : (((sz) <= MEM_RUN_MAX)                                           \
        ? MEM_RUN_BLK                                                   \
        : MEM_BIG_BLK))

#define memnblk(qid)            (g_mem.nblktab[(qid)])
#define memblknum(run, adr)                                             \
    ((int32_t)(((uintptr_t)(adr) - base) * memblknumtab[(run)->qid]))

#define memnrun(qid)            (g_mem.nruntab[(qid)])
#define memrunblknum(run, adr)                                          \
    ((int32_t)(((uintptr_t)(adr) - base) * memrunnumtab[(run)->qid]))
#define memnrunslab(q)                                                  \
    (((q) <= MEM_RUN_SMALL_MAX)                                         \
     ? 8                                                                \
     : (((q) <= MEM_RUN_BIG_MIN)                                        \
        ? 4                                                             \
        : 2))

#define memnblkbuf(q)                                                   \
    (((q) <= MEM_BLK_SMALL_MAX)                                         \
     ? 4                                                                \
     : (((q) <= MEM_BLK_BIG_MIN)                                        \
        ? 2                                                             \
        : 1))

/*
 * HOOK IDEAS
 * ----------
 * - void * MEMGETBUF(q)
 * - void * MEMMAPBUFS(q)
 * - void * MEMGETMAG(q)
 * - void * MEMMAPMAG(q)
 * - void   MEMQUEUEMAG(adr, q)
 * - void   MEMFREEMAG(adr, q)
 * - void * MEMGETRUN(q)
 * - void * MEMMAPRUN(q)
 * - void   MEMQUEUERUN(adr, q)
 * - void   MEMFREERUN(adr, q)
 * - void * MEMMAPBIG(size)
 * - void   MEMFREEBIG(adr, size)
 */

struct memmag {
    void       *adr;            // address of block #0
    long        nblk;           // number of blocks in stack
    long        ndx;            // current stack/table offset/index
    void      **stk;            // allocation block stack
};

struct memrun {
    void       *adr;            // address of run #0
    long        nblk;           // number of blocks in buffer
    long        nrunpg;         // number of pages in a block
    long        bits;           // allocation bitmap, 1 for allocated run
};

#if 0
/*
 * - 8 machine words in size
 */
struct memmap {
    m_atomic_t  mtx;            // reference count
    void       *adr;            // allocation [page] address
    long        size;           // size of map in bytes
    long        pid;            // process ID
    long        uid;            // user ID
    long        gid;            // group ID
    long        perm;           // permission bits
    void       *acl;            // access control list
};
#endif

#define MEM_CACHE_BUFS   16
#define MEM_BUF_SIZE     	(2 * SYS_PAGE_SIZE)
#define MEM_BUF_STK_OFS  	(SYS_PAGE_SIZE)
#if (MEM_BUF_SIZE < 2 * MEM_SMALL_QUEUES * SYS_WORD_SIZE)
#error fix MEM_BUF_SIZE in <zen/mem.h>
#endif

/* 4 machine words */
struct membuf {
    long                type;   // buffer block type
    struct mempool     *glob;   // pointer to global pool
    m_atomic_t         *prev;   // pointer to previous magazine in queue
    m_atomic_t         *next;   // pointer to next magazine in queue
    union {
        struct memmag  *mag;
        struct memrun  *run;
    } hdr;
};

struct mempool {
    m_atomic_t          mtx;
    struct memtls      *tls;
    struct membuf      *head;
    struct membuf      *tail;
    struct membuf       dummy;
};

struct memtls {
    struct mempool      blktab[MEM_SMALL_QUEUES];
    long                nblktab[MEM_SMALL_QUEUES];
};

/* 2 machine words */
struct memblk {
    uintptr_t           page;
    struct membuf      *buf;
};

/* 64 machine words */
struct memhashtab {
    long                ntab;
    long                nmax;
    struct memblk       data[31];
};

/* flg-member */
#define MEM_INIT_BIT            ((m_atomic_t)(1 << 0))
struct memglob {
    /* allocation queue tables */
    struct mempool      blktab[MEM_SMALL_QUEUES];       // small blocks
    struct mempool      runtab[MEM_RUN_QUEUES];         // run blocks
    long                nblktab[MEM_SMALL_QUEUES];      // small block counts
    long                nruntab[MEM_RUN_QUEUES];        // run counts
    struct membuf      *hdrbufq;                        // buffer header cache
    m_atomic_t          flg;
};

#endif /* __MEM_MAG_H__ */

