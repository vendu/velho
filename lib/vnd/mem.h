#ifndef __VND_MEM_H__
#define __VND_MEM_H__

#include <limits.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/types.h>
#include <mt/mtx.h>
#include <zero/trix.h>
#include <vnd/hash.h>

#define MEM_ZERO_BIT            (1 << 0)

#define MEM_ALIGN_MIN           CLSIZE

#define MEM_LK_BIT_OFS          0
#define MEM_LK_BIT              ((m_atomic_t)1L << 0)

#if defined(__GNUC__) || defined(__clang__)
#define PTRALIGNED(ptr, aln)    __builtin_assume_aligned(ptr, aln)
#else
#define PTRALIGNED(ptr, aln)    (ptr)
#endif

#define mempageadr(adr)         ((uintptr_t)(adr) & (PAGESIZE - 1))
#define memalignptr(ptr, aln)   ((void *)roundup2((uintptr_t)ptr, aln))

#define MEM_BLK                 0
#define MEM_RUN                 1
#define MEM_BIG                 2
#define MEM_BLK_SLAB            (8 * PAGESIZE)
#define MEM_BLK_MIN     	CLSIZE
#define MEM_BLK_MAX     	PAGESIZE
#define MEM_BLK_QUEUES          (MEM_BLK_MAX / MEM_BLK_MIN)
#define MEM_BLK_SMALL_QUEUES    (MEM_BLK_QUEUES / 4)
#define MEM_BLK_BIG_QUEUE_MIN   (MEM_BLK_QUEUES / 2)
#define MEM_BLK_MID_BLOCKS      (MEM_BLK_SLAB_MID / MEM_BLK_MID_MIN)
#define MEM_BLK_BIG_BLOCKS      (MEM_BLK_SLAB_BIG / MEM_BLK_BIG_MIN)
#define MEM_BLK_MAX_BLOCKS      max(MEM_BLK_BLOCKS,                     \
                                    max(MEM_BLK_MID_BLOCKS, MEM_BLK_BIG_BLOCKS))
#define MEM_RUN_MAX             (MEM_RUN_QUEUES * PAGESIZE)
/* fixed for run allocation bitmap of a single long */
#define MEM_RUN_QUEUES          (CHAR_BIT * sizeof(long))
#define MEM_RUN_MID_QUEUE_MIN   (MEM_RUN_QUEUES / 4)
#define MEM_RUN_BIG_QUEUE_MIN   (MEM_RUN_QUEUES / 2)

#define memblkqueue(sz)         (((sz) + MEM_BLK_MIN - 1) >> CLSIZELOG2)
#define memblksize(q)           (((q) + 1) << CLSIZELOG2)
#define memrunqueue(sz)         (((sz) + PAGESIZE - 1) >> PAGESIZELOG2)
#define memrunsize(q)           (((q) + 1) << PAGESIZELOG2)
#define memalloctype(sz)                                                \
    (((sz) <= MEM_BLK_MAX)                                              \
     ? MEM_BLK                                                          \
     : (((sz) <= MEM_RUN_MAX)                                           \
        ? MEM_RUN                                                       \
        : MEM_BIG))
#define memalnalloctype(sz, aln)                                        \
    memalloctype(memalnsize(sz, aln))

#define memblkbuf(blk)                                                  \
    ((struct membuf *)((blk)->buf & MEM_BUF_ADR_MASK))
#define memrunqid(blk)                                                  \
    ((blk)->buf & MEM_BUF_QUEUE_MASK)
#define memblkid(blk, adr)                                              \
    memblknum(memblkbuf(blk), adr)
#define memblkbigsize(blk)                                              \
    ((blk)->info & MEM_BLK_SIZE_MASK)

#define memblkadr(buf, blk, adr)                                        \
    (memblkisbig(blk)                                                   \
     ? ((void *)((blk)->info & MEM_ADR_MASK))                      \
     : membufbase(buf) + memblknum(memblkbuf(blk), memblkid(blk, adr)))
#define membufblknum(buf, adr)                                          \
    ((membuftype(buf) == MEM_BLK)                                       \
     ? memblknum(buf, adr)                                              \
     : (membuftype(buf) == MEM_RUN                                      \
        ? memrunnum(buf, adr)                                           \
        : 0))

#define memnblk(qid)            (g_mem.nblktab[(qid)])
#define memblknum(buf, adr)                                             \
    ((long)(((uintptr_t)(adr) - (uintptr_t)membufbase(buf))             \
            * g_mem.blkdivtab[(run)->qid]))

#define memnrun(qid)            (g_mem.nruntab[(qid)])
#define memrunnum(buf, adr)                                             \
    ((long)(((uintptr_t)(adr) - (uintptr_t)membufbase(buf))             \
            * g_mem.rundivtab[(buf)->qid]))

#define memnblktlsbuf(q)                                                \
    (((q) <= MEM_BLK_SMALL_MAX)                                         \
     ? 4                                                                \
     : (((q) <= MEM_BLK_BIG_MIN)                                        \
        ? 2                                                             \
        : 1))
#define memnblkbuf(q)                                                   \
    (((q) <= MEM_BLK_SMALL_MAX)                                         \
     ? 8                                                                \
     : (((q) <= MEM_BLK_BIG_MIN)                                        \
        ? 4                                                             \
        : 2))
#define memnrunbuf(q)                                                   \
    (((q) <= MEM_RUN_SMALL_MAX)                                         \
     ? 4                                                                \
     : (((q) <= MEM_RUN_BIG_MIN)                                        \
        ? 2                                                             \
        : 1))

static __inline__ size_t
memalnsize(size_t size, size_t align)
{
    if (align <= CLSIZE) {

        return size;
    } else {
        size_t blksize = size;
        size_t alnsize = size;

        blksize--;
        alnsize--;
        blksize -= CLSIZE;                      // size - 1 - CLSIZE
        alnsize += align;                       // size - 1 + align
        if (blksize < MEM_BLK_MAX) {

            return blksize;
        } else if (align <= PAGESIZE) {

            return size;
        } else {

            return alnsize;
        }
    }

    /* NOTREACHED */
    return 0;
}

/* adr-word bits */
#define MEM_TYPE_BITS      2
#define MEM_TYPE_MASK      (((uintptr_t)1 << MEM_TYPE_BITS) - 1)
#define MEM_TLS_SHIFT      MEM_TYPE_BITS
#define MEM_TLS_BIT        ((uintptr_t)1 << MEM_TLS_SHIFT)
#define MEM_ADR_MASK       (~(uintptr_t)(PAGESIZE - 1))
#define MEM_BLK_ADR_MASK   (CLSIZE - 1)
#define memistls(info)                                                  \
    ((info) & MEM_TLS_BIT)
#define memisblk(info)                                                  \
    ((info) & MEM_TYPE_MASK == MEM_BLK)
#define memisrun(info)                                                  \
    ((info) & MEM_TYPE_MASK == MEM_RUN)
#define memisbig(info)                                                  \
    ((info) & MEM_TYPE_MASK == MEM_BIG)

struct memblk {
    uintptr_t   adr;            // allocation address + alignment shift (CLSIZE)
};

#define MEM_HASH_BUF_ADR_MASK   (~(((uintptr_t)1 << MEM_BUF_QUEUE_BITS) - 1))
#define memhashbuf(hash)        ((hash).buf & MEM_HASH_BUF_ADR_MASK)
#define memsethashbuf(buf, qid) ((uintptr_t)(buf) | (qid))
struct memhash {
    uintptr_t   page;           // block page address
    uintptr_t   buf;            // queue ID + allocation buffer address + flags
};

struct memhashtab {
    long                ncur;   // numter of items in subtable
    long                nmax;   // max number of items in subtable
    struct memhashtab  *prev;   // pointer to previous subtable
    struct memhashtab  *next;   // pointer to next subtable
    struct memhash      items[30];
};

struct memblkhdr {
    long                ndx;            // current stack/table offset/index
    long                nblk;           // number of blocks in stack
    struct memblk       stk[VLA];       // allocation stack
};

struct memrunhdr {
    long        nblk;           // number of blocks in buffer
    long        bmap;           // allocation bitmap, 0 for allocated run
    int8_t      alntab[VLA];    // alignment shift counts of PAGESIZE or -1
};

#define MEM_CACHE_INFOS         (PAGESIZE / MEM_BLK_SIZE)
#define MEM_INFO_SIZE     	(4 * WORDSIZE)
#define MEM_CACHE_BLK_BUFS      16
#define MEM_CACHE_RUN_BUFS      8
#define MEM_CACHE_BUFS          (PAGESIZE / membufsize())
#define membufsize()            (roundup2(sizeof(struct membuf), CLSIZE))
#define memblkbufsize()                                                 \
    roundup2(membufsize() + MEM_BLK_MAX_BLOCKS * sizeof(struct memblk), \
             PAGESIZE)
#define memrunbufsize()                                                 \
    roundup2(membufsize() + MEM_RUN_MAX_BLOCKS * sizeof(struct memblk), \
             PAGESIZE)

/* buf-word bits */
#define MEM_BUF_QUEUE_BITS      8
#define MEM_BUF_TLS_SHIFT       MEM_BUF_QUEUE_BITS
#define MEM_BUF_TLS_BIT         ((uintptr_1)1 << MEM_BUF_QUEUE_BITS)
#define MEM_BUF_PAGEMAP_SHIFT   MEM_BUF_QUEUE_BITS
#define MEM_BUF_PAGEMAP_BITS    (CHAR_BIT * sizeof(long) - MEM_BUF_QUEUE_BITS)
#define membufpagenum(buf, ptr)                                         \
    (((uinptr_t)(ptr) - (uinptr_t)(buf)->base) >> PAGESHIFT)
#define membufpagebit(buf, num)                                         \
    ((buf)->info & (1L << (MEM_BUF_QUEUE_BITS + (num))))
#define membufsetpagebit(buf, num)                                      \
    ((buf)->info |= (1L << (MEM_BUF_QUEUE_BITS + (num))))
#define membufclrpagebit(buf, num)                                      \
    ((buf)->info &= ~(1L << (MEM_BUF_QUEUE_BITS + (num))))
struct membuf {
    void                       *adr;    // address of first allocation
    size_t                      size;   // mapped buffer size
    long                        info;   // page-bitmap for blk-class + queue ID
    struct membufq             *queue;  // queue the buffer is on
    struct membuf              *prev;   // previous magazine in queue
    struct membuf              *next;   // next magazine in queue
    union {
        struct memblkhdr        blk;    // block buffer header
        struct memrunhdr        run;    // run buffer header
    } hdr;
};

struct membufq {
    mtmtx               mtx;    // buffer-queue lock mutex
    long                nbuf;   // number of buffers in queue
    struct memtls      *tls;    // pointer to thread-local data
    struct membuf      *head;   // queue head
    struct membuf      *tail;   // queue tail
    long                pad[3]; // pad to multiple of 8 machine words
};

struct memtls {
    struct membufq      blktab[MEM_BLK_QUEUES];
    struct membufq      runtab[MEM_RUN_QUEUES];
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
    float                       rundivtab[MEM_RUN_QUEUES];
    volatile struct membuf     *blkhdrq;
    volatile struct membuf     *runhdrq;
    volatile struct membuf     *bufhdrq;
    void                       *hash;
    mtmtx                       mtx;
    m_atomic_t                  flg;
};

void  * memgetblk(struct membufq *queue, long qid);
void  * memgetrun(struct membufq *queue, long qid);
void  * memmapbig(size_t size);

#include <vnd/bits/mem.h>

#endif /* __VND_MEM_H__ */

