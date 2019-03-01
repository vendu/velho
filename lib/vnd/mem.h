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
#define MEM_TYPE_MASK           0x03
#define MEM_BLK_SLAB            (8 * PAGESIZE)
#define MEM_BLK_MIN     	CLSIZE
#define MEM_BLK_MAX     	PAGESIZE
#define MEM_BLK_QUEUES          (MEM_BLK_MAX / MEM_BLK_MIN)
#define MEM_BLK_MID_QUEUE_MIN   (MEM_BLK_QUEUES >> 2)
#define MEM_BLK_BIG_QUEUE_MIN   (MEM_BLK_QUEUES >> 1)
#define MEM_BLK_MAX_BLOCKS      (MEM_BLK_SLAB / MEM_BLK_MIN)
#define MEM_RUN_MAX             (MEM_RUN_QUEUES * PAGESIZE)
/* fixed for run allocation bitmap of a single long */
#define MEM_RUN_QUEUES          (CHAR_BIT * sizeof(long))
#define MEM_RUN_MID_QUEUE_MIN   (MEM_RUN_QUEUES >> 2)
#define MEM_RUN_BIG_QUEUE_MIN   (MEM_RUN_QUEUES >> 1)
#define memblkqid(sz)           (((sz) + MEM_BLK_MIN - 1) >> CLSIZELOG2)
#define memblksize(q)           (((q) + 1) << CLSIZELOG2)
#define memrunqid(sz)           (((sz) + PAGESIZE - 1) >> PAGESIZELOG2)
#define memrunsize(q)           (((q) + 1) << PAGESIZELOG2)
#define memalloctype(sz)                                                \
    (((sz) <= MEM_BLK_MAX)                                              \
     ? MEM_BLK                                                          \
     : (((sz) <= MEM_RUN_MAX)                                           \
        ? MEM_RUN                                                       \
        : MEM_BIG))
#define memalnalloctype(sz, aln)                                        \
    memalloctype(memalnsize(sz, aln))

#define memnblk(qid)            (g_mem.nblktab[(qid)])
#define memblknum(qid, buf, ptr)                                        \
    ((long)(((uintptr_t)(ptr) - (uintptr_t)((buf)->adr))                \
            * g_mem.blkdivtab[(qid)]))

#define memnrun(qid)            (g_mem.nruntab[(qid)])
#define memrunnum(qid, buf, ptr)                                        \
    ((long)(((uintptr_t)(ptr) - (uintptr_t)((buf)->adr))                \
            * g_mem.rundivtab[(qid)]))

#define memnblktlsbuf(q)                                                \
    (((q) <= MEM_BLK_SMALL_MAX)                                         \
     ? 4                                                                \
     : (((q) <= MEM_BLK_BIG_MIN)                                        \
        ? 2                                                             \
        : 1))
#define memnblkbuf(q)                                                   \
    (((q) <= MEM_BLK_MID_QUEUE_MIN)                                     \
     ? 8                                                                \
     : (((q) <= MEM_BLK_BIG_QUEUE_MIN)                                  \
        ? 4                                                             \
        : 2))
#define memnrunbuf(q)                                                   \
    (((q) <= MEM_RUN_MID_QUEUE_MIN)                                     \
     ? 4                                                                \
     : (((q) <= MEM_RUN_BIG_QUEUE_MIN)                                  \
        ? 2                                                             \
        : 1))

#define MEM_HASH_ADR_MASK       (~(((uintptr_t)1 << MEM_BUF_QUEUE_BITS) - 1))
#define MEM_HASH_QUEUE_MASK     (((uintptr_t)1 << MEM_BUF_QUEUE_BITS) - 1)
#define memhashtype(hash)       ((hash)->page & MEM_TYPE_MASK)
#define memhashbuf(hash)        ((void *)((hash)->buf & MEM_HASH_ADR_MASK))
#define memhashqid(hash)        ((hash)->buf & MEM_HASH_QUEUE_MASK)
#define memsethashbuf(buf, qid) ((uintptr_t)(buf) | (qid))
struct memhash {
    uintptr_t   page;           // block page address +
    uintptr_t   buf;            // queue ID + allocation buffer address + flags
};

#define MEM_CACHE_BLK_BUFS      16
#define MEM_CACHE_BUFS          32
/* must be power of two size/alignment at least a page */
#define membufsize()            PAGESIZE
#define memblkbufsize()                                                 \
    roundup2(membufsize() + 2 * MEM_BLK_MAX_BLOCKS * sizeof(uintptr_t), \
             PAGESIZE)
#define memrunbufsize()         (membufsize())
#define membigbufsize()         (membufsize())

/* buf-word bits */
#define MEM_BUF_QUEUE_BITS      8
#define MEM_BUF_TLS_SHIFT       MEM_BUF_QUEUE_BITS
#define MEM_BUF_TLS_BIT         ((uintptr_1)1 << MEM_BUF_QUEUE_BITS)
#define MEM_BUF_PAGEMAP_SHIFT   MEM_BUF_QUEUE_BITS
#define MEM_BUF_PAGEMAP_BITS    (CHAR_BIT * sizeof(long) - MEM_BUF_QUEUE_BITS)
#define membufpagenum(buf, ptr)                                         \
    (((uintptr_t)(ptr) - (uintptr_t)(buf)->adr) >> PAGESIZELOG2)
#define membufpagebit(buf, num)                                         \
    ((buf)->info & (1L << (MEM_BUF_QUEUE_BITS + (num))))
#define membufsetpagebit(buf, num)                                      \
    ((buf)->info |= (1L << (MEM_BUF_QUEUE_BITS + (num))))
#define membufclrpagebit(buf, num)                                      \
    ((buf)->info &= ~(1L << (MEM_BUF_QUEUE_BITS + (num))))
struct membuf {
    void                       *adr;    // address of first allocation
    size_t                      size;   // mapped buffer size
    long                        bits;   // allocation page- or run-bitmap
    struct membufq             *queue;  // queue the buffer is on
    struct membuf              *prev;   // previous magazine in queue
    struct membuf              *next;   // next magazine in queue
    long                        cur;    // current # of allocations
    long                        max;    // available # of allocations
    uintptr_t                   tab[VLA];
};

struct membufq {
    mtmtx               mtx;    // buffer-queue lock mutex
    long                qid;    // queue ID
    long                nbuf;   // number of buffers in queue
    struct memtls      *tls;    // pointer to thread-local data
    struct membuf      *head;   // queue head
    struct membuf      *tail;   // queue tail
    long                pad[2]; // pad to multiple of 8 machine words
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
    volatile struct membuf     *bufhdrq;
    void                       *hash;
    mtmtx                       mtx;
    m_atomic_t                  flg;
};

#define MEM_FREE_ON_FAILURE (1 << 0)
#define MEM_ALLOC_ON_NULL   (1 << 1)
#define MEM_ERRNO_ON_RESIZE (1 << 2)

void  * memgetblk(struct membufq *queue, size_t align);
void  * memgetrun(struct membufq *queue, size_t align);
void  * memgetbig(size_t size, size_t align);
void  * memget(size_t size, size_t align);
void  * memresize(void *ptr, size_t size, size_t align, long flg);
void    memrel(void *adr);

#include <vnd/bits/mem.h>

extern struct tabhashtab        *g_memhashtab[MEM_HASH_SLOTS];
extern struct memglob            g_mem ALIGNED(PAGESIZE);

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

static __inline__ void *
memgetptr(void *ptr)
{
    struct memhash      item = tabhashfind(g_memhashtab, mempageadr(ptr));
    long                type = memhashtype(&item);
    struct membuf      *buf = memhashbuf(&item);
    long                qid = memhashqid(&item);
    uint8_t            *adr = ptr;
    long                ofs;
    long                bits;
    long                mask;
    uintptr_t           num;

    mtlkfmtx(&buf->queue->mtx);
    switch (type) {
        case MEM_BLK:
            ofs = buf->max;
            num = memblknum(qid, buf, ptr);
            ofs += num;
            adr = (uint8_t *)buf->tab[ofs];

            break;
        case MEM_RUN:
            mask = 1L;
            num = memrunnum(qid, buf, ptr);
            bits = buf->bits;
            mask <<= num;
            adr = (uint8_t *)buf->tab[num];
            bits |= mask;
            buf->bits = bits;

            break;
        case MEM_BIG:
            adr = buf->adr;

            break;
        default:

            break;
    }
    mtunlkfmtx(&buf->queue->mtx);

    return adr;
}

#endif /* __VND_MEM_H__ */

