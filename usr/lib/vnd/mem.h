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
#define MEM_BLK_BIG_SLAB        (16L * PAGESIZE)
#define MEM_BLK_MID_SLAB        (8L * PAGESIZE)
#define MEM_BLK_MIN_SLAB        (4L * PAGESIZE)
#define MEM_BLK_UNIT            CLSIZE
#define MEM_BLK_MIN     	MEM_BLK_UNIT
#define MEM_BLK_MAX     	PAGESIZE
#define MEM_BLK_QUEUES          (MEM_BLK_MAX / MEM_BLK_UNIT)
#define MEM_BLK_MID             memblksize(MEM_BLK_MID_QUEUE_MIN)
#define MEM_BLK_BIG             memblksize(MEM_BLK_BIG_QUEUE_MIN)
#define MEM_BLK_MID_QUEUE_MIN   (MEM_BLK_QUEUES >> 2)
#define MEM_BLK_BIG_QUEUE_MIN   (MEM_BLK_QUEUES >> 1)
#define MEM_BLK_MAX_BLOCKS      max(MEM_BLK_MIN_SLAB / MEM_BLK_MIN,     \
                                    max(MEM_BLK_MID_SLAB / MEM_BLK_MID, \
                                        MEM_BLK_BIG_SLAB / MEM_BLK_BIG))
#define MEM_RUN_BIG_SLAB        (2 * MEM_RUN_QUEUES * PAGESIZE)
#define MEM_RUN_MID_SLAB        (MEM_RUN_QUEUES * PAGESIZE)
#define MEM_RUN_MIN_SLAB        (MEM_RUN_QUEUES * PAGESIZE >> 1)
#define MEM_RUN_UNIT            PAGESIZE
#define MEM_RUN_MIN             MEM_RUN_UNIT
#define MEM_RUN_MAX             (MEM_RUN_QUEUES * MEM_RUN_UNIT)
/* fixed for run allocation bitmap of a single long */
#define MEM_RUN_QUEUES          MEM_BUF_MAP_BITS
#define MEM_RUN_MID_QUEUE_MIN   (MEM_RUN_QUEUES >> 2)
#define MEM_RUN_BIG_QUEUE_MIN   (MEM_RUN_QUEUES >> 1)
#define MEM_BIG_UNIT            (2 * MEM_RUN_MAX)
#define MEM_BIG_QUEUES          16
#if (MEM_BIG_QUEUES == 16)
#define MEM_BIG_SHIFT           5
#elif (MEM_BIG_QUEUES == 32)
#define MEM_BIG_SHIFT           6
#elif (MEM_BIG_QUEUES == 64)
#define MEM_BIG_SHIFT           7
#endif
#define membuftype(sz)                                                  \
    (((sz) <= MEM_BLK_MAX)                                              \
     ? MEM_BLK                                                          \
     : (((sz) <= MEM_RUN_MAX)                                           \
        ? MEM_RUN                                                       \
        : MEM_BIG))
#define memblkqid(sz)           (((sz) + MEM_BLK_MIN - 1) >> CLSIZELOG2)
#define memblksize(qid)         (CLSIZE * (qid))
#define memrunqid(sz)           (((sz) + PAGESIZE - 1) >> PAGESIZELOG2)
#define memrunsize(qid)         (PAGESIZE * (qid))
#define membigqid(sz)           (((sz) + MEM_BIG_UNIT - 1)              \
                                 >> (PAGESIZELOG2 + MEM_BIG_SHIFT))
#define membigsize(qid)         (MEM_BIG_UNIT * (qid))
#define memnblk(qid)            (g_mem.nblktab[(qid)])
#define memblknum(qid, buf, ptr)                                        \
    ((long)(((uintptr_t)(ptr) - (uintptr_t)((buf)->adr))                \
            * g_mem.blkdivtab[(qid)]))
#define memnrun(qid)            (g_mem.nruntab[(qid)])
#define memrunnum(qid, buf, ptr)                                        \
    ((long)(((uintptr_t)(ptr) - (uintptr_t)((buf)->adr))                \
            * g_mem.rundivtab[(qid)]))
#define memnblktlsbuf(qid)                                              \
    (((qid) <= MEM_BLK_MID_QUEUE_MIN)                                   \
     ? 4                                                                \
     : (((qid) <= MEM_BLK_BIG_QUEUE_MIN)                                \
        ? 2                                                             \
        : 1))
#define memnblkbuf(qid)                                                 \
    (((qid) <= MEM_BLK_MID_QUEUE_MIN)                                   \
     ? 8                                                                \
     : (((qid) <= MEM_BLK_BIG_QUEUE_MIN)                                \
        ? 4                                                             \
        : 2))
#define memnruntlsbuf(qid)                                              \
    (((qid) <= MEM_RUN_MID_QUEUE_MIN)                                   \
     ? 2                                                                \
     : (((qid) <= MEM_RUN_BIG_QUEUE_MIN)                                \
        ? 1                                                             \
        : 0))
#define memnrunbuf(qid)                                                 \
    (((qid) <= MEM_RUN_MID_QUEUE_MIN)                                   \
     ? 4                                                                \
     : (((qid) <= MEM_RUN_BIG_QUEUE_MIN)                                \
        ? 2                                                             \
        : 1))

/* page-member */
#define MEM_HASH_NREF_BITS              8
#define MEM_HASH_TYPE_SHIFT             MEM_HASH_NREF_BITS
#define MEM_HASH_TYPE_MASK              (MEM_TYPE_MASK << MEM_HASH_TYPE_SHIFT)
#define MEM_HASH_TYPE_BLK               (MEM_BLK << MEM_HASH_TYPE_SHIFT)
#define MEM_HASH_TYPE_RUN               (MEM_RUN << MEM_HASH_TYPE_SHIFT)
#define MEM_HASH_TYPE_BIG               (MEM_BIG << MEM_HASH_TYPE_SHIFT)
#define MEM_HASH_PAGE_MASK              (PAGESIZE - 1)
/* buf-member */
#define MEM_BUF_ADR_MASK                (~MEM_BUF_QUEUE_MASK)
#define MEM_BUF_QUEUE_MASK              (((uintptr_t)1 << MEM_BUF_QUEUE_BITS) \
                                         - 1)
#define MEM_BUF_QUEUE_BITS              8
#define MEM_BUF_TLS_SHIFT               MEM_QUEUE_BITS
#define MEM_BUF_TLS_BIT                 ((uintptr_t)1 << MEM_BUF_TLS_SHIFT)
#define memhashtype(hash)               ((hash)->buf & MEM_TYPE_MASK)
#define memhashbuf(hash)                ((void *)((hash)->buf           \
                                                  & MEM_BUF_ADR_MASK))
#define memhashqid(hash)                ((hash)->buf & MEM_BUF_QUEUE_MASK)
#define memsethashpage(adr, type, n)    ((uintptr_t)(adr) \
                                         | ((type) << MEM_HASH_TYPE_SHIFT) \
                                         | (n))
#define memsethashbuf(buf, qid)         ((uintptr_t)(buf) | (qid))
/*
 * illustrative little-endian bitfield
 * -----------------------------------
 * struct memhash {
 *     unsigned nref    : MEM_HASH_NREF_BITS;
 *     unsigned page    : PTRBITS - MEM_HASH_NREF_BITS;
 *     unsigned qid     : MEM_BUF_QUEUE_BITS;
 *     unsigned buf     : PTRBITS;
 * };
 */

struct memhash {
    uintptr_t   page;           // block page address + reference count
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
#define MEM_BUF_MAP_BITS        (CHAR_BIT * sizeof(uintptr_t))
#define membufpagenum(buf, ptr)                                         \
    (((uintptr_t)(ptr) - (uintptr_t)(buf)->adr) >> PAGESIZELOG2)
#define membufpagebit(buf, num)                                         \
    ((buf)->info & (1L << (MEM_BUF_QUEUE_BITS + (num))))
#define membufsetpagebit(buf, num)                                      \
    ((buf)->info |= (1L << (MEM_BUF_QUEUE_BITS + (num))))
#define membufclrpagebit(buf, num)                                      \
    ((buf)->info &= ~(1L << (MEM_BUF_QUEUE_BITS + (num))))
struct membuf {
    uint8_t                    *adr;    // address of first allocation
    size_t                      size;   // mapped buffer size
    size_t                      ofs;    // current stack offset
    size_t                      max;    // available # of allocations
    uintptr_t                   bits;   // allocation page- or run-bitmap
    struct membufq             *queue;  // queue the buffer is on
    struct membuf              *prev;   // previous magazine in queue
    struct membuf              *next;   // next magazine in queue
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
    double                      blkdivtab[MEM_BLK_QUEUES];
    struct membufq              runtab[MEM_RUN_QUEUES];
    long                        nruntab[MEM_RUN_QUEUES];
    double                      rundivtab[MEM_RUN_QUEUES];
    volatile struct membuf     *blkhdrq;
    volatile struct membuf     *bufhdrq;
    volatile struct tabhashtab *hashtabq;
    mtmtx                       mtx;
    m_atomic_t                  flg;
};

#define MEM_CONF_MIN_SIZE       1
#define MEM_CONF_MAX_BLK        2
#define MEM_CONF_MAX_RUN        3

#define MEM_CONF_ALIGN  (1L << 0)
#define MEM_CONF_SIZE   (1L << 1)
struct memconf {
    long        flg;
    size_t      align;
    size_t      size;
};

#define MEM_FREE_ON_FAILURE (1 << 0)
#define MEM_ALLOC_ON_NULL   (1 << 1)
#define MEM_ERRNO_ON_RESIZE (1 << 2)

void  * memgetblk(long qid, size_t align, struct memtls *tls);
void  * memgetrun(long qid, size_t align, struct memtls *tls);
void  * memgetbig(size_t size, size_t align);
void  * memget(size_t size, size_t align);
void    memput(void *ptr);
void  * memresize(void *ptr, size_t size, size_t align, long flg);
void    memrel(void *adr);

#include <vnd/bits/mem.h>

extern struct tabhashtab        *g_memhashtab[TABHASH_SLOTS];
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

