#ifndef __MEM_MEM_H__
#define __MEM_MEM_H__

#include <limits.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/types.h>
#include <zero/trix.h>
#include <mt/mtx.h>

#define MEM_LK_BIT_OFS    0
#define MEM_LK_BIT        ((m_atomic_t)1L << 0)

#if defined(__GNUC__) || defined(__clang__)

#define PTRALIGNED(ptr, aln)    __builtin_assume_aligned(ptr, aln)

#else

#define PTRALIGNED(ptr, aln)    (ptr)

#endif

#define MEM_SMALL_BLK           0
#define MEM_RUN_BLK             1
#define MEM_BIG_BLK             2

#define MEM_BLK_SLAB            (2 * PAGESIZE)
#define MEM_BLK_MIN     	CLSIZE
#define MEM_BLK_MAX     	(MEM_BLK_SLAB / 2)
#define MEM_BLK_SMALL_MAX       (MEM_BLK_QUEUES / 4)
#define MEM_BLK_BIG_MIN         (MEM_BLK_QUEUES / 2)
#define MEM_BLK_QUEUES  	(MEM_BLK_SLAB / MEM_BLK_MIN)
#define MEM_RUN_SLAB       	(MEM_RUN_QUEUES * PAGESIZE)
#define MEM_RUN_MIN       	PAGESIZE
#define MEM_RUN_MAX             (MEM_RUN_QUEUES * MEM_RUN_MIN)
/* fixed for run allocation bitmap of a single long */
#define MEM_RUN_QUEUES          (CHAR_BIT * sizeof(long))
#define MEM_RUN_SMALL_MAX       (MEM_RUN_QUEUES / 4)
#define MEM_RUN_BIG_MIN         (MEM_RUN_QUEUES / 2)

/*
 * - small allocation blocks (CLSIZE to 2 * PAGESIZE) are allocated from
 *   slabs of 4 * PAGESIZE
 *   - sizes are multiples of 2 * CLSIZE
 * - mid-size allocations (2 * PAGESIZE + 1 to 128 * PAGESIZE) are served from
 *   buffers of 256 * PAGESIZE
 *   - sizes are multiples of 4 * PAGESIZE
 * - big allocations are mapped directly and not buffered in queues at all
 */

#define membufsize()            (roundup2(sizeof(struct membuf), CLSIZE))
#define memblkqueue(sz)         (((sz) + MEM_BLK_MIN - 1) >> CLSIZELOG2)
#define memblksize(q)           (((q) + 1) << CLSIZELOG2)
#define memrunqueue(sz)         (((sz) + MEM_RUN_MIN - 1) >> PAGESIZELOG2)
#define memrunblksize(q)        (((q) + 1) << PAGESIZELOG2)
#define memblktype(sz)                                                  \
    (((sz) <= MEM_BLK_MAX)                                              \
     ? MEM_SMALL_BLK                                                    \
     : (((sz) <= MEM_RUN_MAX)                                           \
        ? MEM_RUN_BLK                                                   \
        : MEM_BIG_BLK))
#define memalnsize(sz, aln)                                             \
    ((aln) <= CLSIZE ? (sz) : ((sz) + (aln) - 1))
#define memalnblktype(sz, aln)                                          \
    memblktype(memalnsize(sz, aln))

#define memblkissmag(blk)       \
    (((blk)->info & MEM_BLK_TYPE_MASK) == 0)
#define memblkisrun(blk)                                                \
    (((blk)->info & MEM_BLK_TYPE_MASK) == MEM_RUN_BLK_BIT)
#define memblkisbig(blk)                                                \
    (((blk)->info & MEM_BLK_TYPE_MASK) == MEM_BIG_BLK_BIT)
#define memblkbuf(blk)                                                  \
    ((struct membuf *)((blk)->info & MEM_BLK_ADR_MASK))
#define memblkid(blk, adr)                                              \
    memblknum(memblkbuf(blk), adr)
#define memblkqid(blk)                                                  \
    (((buf)->info >> MEM_BUF_TYPE_BITS) & ~((1L << MEM_BUF_QUEUE_BITS) - 1))

#define memblkadr(buf, blk)                                             \
    (memblkisbig(blk)                                                   \
     ? (blk)->adr                                                       \
     : membufbase(buf) + memblknum(memblkbuf(blk), memblkid(blk)))

#define membufbase(buf)                                                 \
    ((buf)->adr)
#define membuftype(buf)                                                 \
    ((buf)->info & ((1L << MEM_BUF_TYPE_BITS) - 1))
#define membufblknum(buf, adr)                                          \
    ((membuftype(buf) == MEM_SMALL_BLK)                                 \
     ? memblknum(buf, adr)                                              \
     : memrunblknum(buf, adr))

#define memnblk(qid)            (g_mem.nblktab[(qid)])
#define memblknum(buf, adr)                                             \
    ((long)(((uintptr_t)(adr) - (uintptr_t)membufbase(buf))             \
            * g_mem.blkdivtab[(run)->qid]))

#define memnrun(qid)            (g_mem.nruntab[(qid)])
#define memrunblknum(buf, adr)                                          \
    ((long)(((uintptr_t)(adr) - (uintptr_t)membufbase(buf))             \
            * g_mem.rundivtab[(buf)->qid]))
#define memnrunslab(q)                                                  \
    (((q) <= MEM_RUN_SMALL_MAX)                                         \
     ? 8                                                                \
     : (((q) <= MEM_RUN_BIG_MIN)                                        \
        ? 4                                                             \
        : 2))

#define membigsize(blk)                                                 \
    ((blk)->info & MEM_BLK_SIZE_MASK)

#define memnblkbuf(q)                                                   \
    (((q) <= MEM_BLK_SMALL_MAX)                                         \
     ? 4                                                                \
     : (((q) <= MEM_BLK_BIG_MIN)                                        \
        ? 2                                                             \
        : 1))

/* info member bits */
#define MEM_RUN_BLK_BIT         (1 << 1)
#define MEM_BIG_BLK_BIT         (1 << 2)
#define MEM_BLK_TYPE_MASK       (MEM_RUN_BLK_BIT | MEM_BIG_BLK_BIT)
#define MEM_BLK_ADR_MASK        (~MEM_BLK_TYPE_MASK)
#define MEM_BLK_SIZE_MASK       (~(PAGESIZE - 1))
struct memblk {
    uintptr_t   page;           // allocation page address
    uintptr_t   info;           // block type + buffer address
};

struct memhashtab {
    size_t              ncur;
    size_t              nmax;
    struct memhashtab  *prev;
    struct memhashtab  *next;
    struct memblk       items[30];
};

struct memmag {
    long        ndx;            // current stack/table offset/index
    long        nblk;           // number of blocks in stack
    long        pad[2];
    void       *stk[VLA];
};

struct memrun {
    size_t      size;           // buffer allocation size
    long        nblk;           // number of blocks in buffer
    long        bits;           // allocation bitmap, 1 for allocated run
    long        pad;
};

#if 0
/*
 * - 8 machine words in size
 */
struct memmap {
    mtmtx       mtx;            // reference count
    void       *adr;            // allocation [page] address
    long        size;           // size of map in bytes
    long        pid;            // process ID
    long        uid;            // user ID
    long        gid;            // group ID
    long        perm;           // permission bits
    void       *acl;            // access control list
};
#endif

#define MEM_CACHE_BUFS          16
#define MEM_BUF_SIZE     	(2 * PAGESIZE)
#define MEM_BUF_STK_OFS  	(PAGESIZE)
#if (MEM_BUF_SIZE < 2 * MEM_BLK_QUEUES * WORDSIZE)
#error fix MEM_BUF_SIZE in <vnd/mem.h>
#endif

/* 4 machine words */
#define MEM_BUF_TYPE_BITS       2
#define MEM_BUF_QUEUE_BITS      (PAGESIZELOG2 - MEM_BUF_TYPE_BITS)
#define MEM_BUF_BASE_MASK       (~(PAGESIZE - 1))
struct membuf {
    void                *adr;   // address of first allocation
    long                 qid;
    struct membuf       *prev;  // previous magazine in queue
    struct membuf       *next;  // next magazine in queue
    union {
        struct memmag    mag;
        struct memrun    run;
    } hdr;
};

struct membufq {
    mtmtx               mtx;
    struct memtls      *tls;
    struct membuf      *head;
    struct membuf      *tail;
};

struct memtls {
    struct membufq      blktab[MEM_BLK_QUEUES];
    long                nblktab[MEM_BLK_QUEUES];
};

/* flg-member */
#define MEM_INIT_BIT            ((m_atomic_t)(1 << 0))
struct memglob {
    /* allocation queue tables */
    struct membufq              blktab[MEM_BLK_QUEUES];
    long                        nblktab[MEM_BLK_QUEUES];
    float                       blkdivtab[MEM_BLK_QUEUES];
    struct membufq              runtab[MEM_RUN_QUEUES];
    long                        nruntab[MEM_RUN_QUEUES];
    float                       rundivtab[MEM_BLK_QUEUES];
    volatile struct membuf     *bufhdrq;
    void                       *hash;
    mtmtx                       mtx;
    m_atomic_t                  flg;
};

void  * memgetblk(struct membufq *queue, long qid);
void  * memgetrunblk(struct membufq *queue, long qid);
void  * memmapbig(size_t size);

#endif /* __MEM_MEM_H__ */

