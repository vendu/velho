#include <errno.h>
#include <mt/mtx.h>
#include <vnd/mem.h>
#include <vnd/unix.h>
#include <vnd/bitop.h>

struct tabhashtab              *g_memhashtab[TABHASH_SLOTS];
struct memglob                  g_mem ALIGNED(PAGESIZE);
THREADLOCAL struct memtls      *t_memtls;
struct tabhashtab              *g_memhashbuf;

#define memblkslabsize(qid)                                             \
    (((qid) <= MEM_BLK_MID_QUEUE_MIN)                                   \
     ? (MEM_BLK_MIN_SLAB >> 2)                                          \
     : (((qid) <= MEM_BLK_BIG_QUEUE_MIN)                                \
        ? (MEM_BLK_MID_SLAB >> 1)                                       \
        : MEM_BLK_BIG_SLAB))
static void
meminitblktabs(struct memglob *mem)
{
    double      slabsz;
    double      blksz;
    double      nblk;
    double      multi;
    long        qid;

    for (qid = 0 ; qid < MEM_BLK_QUEUES ; qid++) {
        slabsz = memblkslabsize(qid);
        blksz = MEM_BLK_MIN * (double)(qid + 1);
        nblk = slabsz / blksz;
        multi = 1.0 / blksz;
        mem->nblktab[qid] = (long)nblk;
        mem->blkdivtab[qid] = multi;
    }

    return;
}

#define memrunslabsize(qid)                                             \
    (((qid) < MEM_RUN_MID_QUEUE_MIN)                                    \
     ? MEM_RUN_MIN_SLAB                                                 \
     : (((qid) < MEM_RUN_BIG_QUEUE_MIN)                                 \
        ? MEM_RUN_MID_SLAB                                              \
        : MEM_RUN_BIG_SLAB))
static void
meminitruntab(struct memglob *mem)
{
    double      slabsz;
    double      runsz;
    double      nrun;
    double      multi;
    long        qid;

    for (qid = 0 ; qid < MEM_RUN_QUEUES ; qid++) {
        slabsz = (double)memrunslabsize(qid);
        runsz = PAGESIZE * (double)(qid + 1);
        nrun = slabsz / runsz;
        multi = 1.0 / runsz;
        mem->nruntab[qid] = (long)nrun;
        mem->rundivtab[qid] = multi;
    }

    return;
}

/* acquire buffer header */
static struct membuf *
memgetbuf(struct membufq *queue, long type)
{
    volatile struct membuf      **hdrq;
    long                          ntry = 32;
    size_t                        bufsz;
    size_t                        nbuf;
    struct membuf                *ret;
    int8_t                       *ptr;
    struct membuf                *buf;
    struct membuf                *head;
    struct membuf                *prev;

    ret = NULL;
    ptr = MAP_FAILED;
    if (type == MEM_BLK) {
        bufsz = memblkbufsize();
        nbuf = MEM_CACHE_BLK_BUFS;
        hdrq = &g_mem.blkhdrq;
    } else {
        bufsz = membufsize();
        nbuf = MEM_CACHE_BUFS;
        hdrq = &g_mem.bufhdrq;
    }
    do {
        mtlkbit((m_atomic_t *)hdrq, MEM_LK_BIT_OFS);
        ret = (void *)((uintptr_t)*hdrq & ~MEM_LK_BIT);
        if (!ret) {
            if (ptr == MAP_FAILED) {
                /* map header region */
                ptr = mapanon(-1, nbuf * bufsz, 0);
                if (ptr != MAP_FAILED) {
                    buf = (struct membuf *)ptr;
                    ret = (struct membuf *)ptr;
                    buf->queue = queue;
                    head = (struct membuf *)((int8_t *)buf + bufsz);
                    head->queue = queue;
                    head->prev = NULL;
                    prev = head;
                    while (--nbuf) {
                        buf = (struct membuf *)((int8_t *)buf + bufsz);
                        buf->queue = queue;
                        buf->prev = prev;
                        prev->next = buf;
                        prev = buf;
                    }
                    buf->next = NULL;
                }
            }
            if (ptr != MAP_FAILED) {
                ret = (struct membuf *)ptr;
            }
        }
        if (ret) {
            head = ret->next;
            if (head) {
                head->prev = NULL;
            }
        }
        m_atomwrite((m_atomic_t *)hdrq, (m_atomic_t *)head);
        if (!ret) {
            m_waitspin();
        }
    } while (!ret && --ntry);

    return ret;
}

/* release buffer header */
static void
memputbuf(struct membuf *buf, long type)
{
    volatile struct membuf    **hdrq;
    struct membuf              *head;

    buf->adr = NULL;
    buf->prev = NULL;
    if (type == MEM_BLK) {
        hdrq = &g_mem.blkhdrq;
    } else {
        hdrq = &g_mem.bufhdrq;
    }
    mtlkbit((m_atomic_t *)hdrq, MEM_LK_BIT_OFS);
    head = (void *)((uintptr_t)*hdrq & ~MEM_LK_BIT);
    buf->next = head;
    if (head) {
        head->prev = buf;
    }
    m_atomwrite((m_atomic_t *)hdrq, buf);

    return;
}

/* allocate new block buffer */
static struct membuf *
memgetblkbuf(struct membufq *queue)
{
    long                qid = queue->qid;
    struct membuf      *buf = memgetbuf(queue, MEM_BLK);
    long                ntry = 32;
    size_t              slabsz = memblkslabsize(qid);
    size_t              blksz = memblksize(qid);
    int8_t             *ptr;
    uintptr_t          *tab;
    long                nblk;
    long                n;

    if (!buf) {

        return NULL;
    }
    do {
        ptr = mapanon(-1, slabsz, 0);
        if (ptr == MAP_FAILED) {
            m_waitspin();
        }
    } while (ptr == MAP_FAILED && --ntry);
    if (ptr == MAP_FAILED) {
        memputbuf(buf, MEM_BLK);
        buf = NULL;
    } else {
        /* initialise buffer */
        nblk = memnblk(qid);
        buf->adr = ptr;
        buf->size = slabsz;
        buf->queue = queue;
        tab = buf->tab;
        buf->max = nblk;
        do {
            /* initialise allocation stack */
            n = min(8, nblk);
            switch (n) {
                case 8:
                    tab[7] = (uintptr_t)(ptr + 7 * blksz);
                case 7:
                    tab[6] = (uintptr_t)(ptr + 6 * blksz);
                case 6:
                    tab[5] = (uintptr_t)(ptr + 5 * blksz);
                case 5:
                    tab[4] = (uintptr_t)(ptr + 4 * blksz);
                case 4:
                    tab[3] = (uintptr_t)(ptr + 3 * blksz);
                case 3:
                    tab[2] = (uintptr_t)(ptr + 2 * blksz);
                case 2:
                    tab[1] = (uintptr_t)(ptr + blksz);
                case 1:
                    tab[0] = (uintptr_t)ptr;
                case 0:

                    break;
            }
            nblk -= n;
            ptr += 8 * blksz;
            tab += 8;
        } while (nblk);
    }

    return buf;
}

static struct membuf *
memgetrunbuf(struct membufq *queue)
{
    long                qid = queue->qid;
    struct membuf      *buf = memgetbuf(queue, MEM_RUN);
    long                ntry = 32;
    size_t              blksz = memrunsize(qid);
    size_t              nrun = memnrun(qid);
    size_t              slabsz = blksz * memnrun(qid);
    long                bits;
    void               *ptr;

    if (!buf) {

        return NULL;
    }
    do {
        ptr = mapanon(-1, slabsz, 0);
        if (ptr == MAP_FAILED) {
            m_waitspin();
        }
    } while (ptr == MAP_FAILED && --ntry);
    if (ptr == MAP_FAILED) {
        memputbuf(buf, MEM_RUN);
        buf = NULL;
    } else {
        /* initialise run buffer */
        bits = 1L << nrun;              // shift by run count
        buf->adr = ptr;                 // buffer base address
        bits--;                         // free-bitmap for runs
        buf->size = slabsz;             // buffer total size
        buf->bits = bits;               // set free-bitmap
        buf->max = nrun;                // set # of runs in buffer
    }

    return buf;
}

/* push buffer in front of queue */
static void
mempushbuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf       *head = queue->head;

    buf->prev = NULL;
    buf->next = head;
    queue->nbuf++;
    if (head) {
        head->prev = buf;
    } else {
        queue->tail = buf;
    }
    queue->head = buf;

    return;
}

/* queue buffer in back of queue */
static void
memqueuebuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf      *tail = queue->tail;

    buf->next = NULL;
    buf->prev = tail;
    queue->nbuf++;
    if (tail) {
        tail->next = buf;
    } else {
        queue->head = buf;
    }
    queue->tail = buf;

    return;
}

/* remove buffer from queue */
static void
memdequebuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf      *prev;
    struct membuf      *next;

    prev = buf->prev;
    next = buf->next;
    queue->nbuf--;
    if (prev) {
        prev->next = buf->next;
    } else {
        queue->head = next;
    }
    if (next) {
        next->prev = buf->prev;
    } else {
        queue->tail = prev;
    }

    return;
}

/* release allocation block */
static void
memputblk(struct membuf *buf, void *ptr)
{
    struct membufq     *queue = buf->queue;
    long                qid = queue->qid;
    struct memtls      *tls = queue->tls;
    long                lim = (tls) ? memnblktlsbuf(qid) : memnblkbuf(qid);
    struct membufq     *glob;
    void               *adr;
    long                ndx;
    long                max;
    long                nbuf;

    if (!tls) {
        mtlkfmtx(&queue->mtx);
    }
    adr = memgetptr(ptr);
    ndx = buf->ofs;
    max = buf->max;
    ndx--;
    nbuf = queue->nbuf;
    max--;
    buf->tab[ndx] = (uintptr_t)adr;
    buf->ofs = ndx;
    if (ndx == max) {
        memqueuebuf(queue, buf);
    } else if (!ndx && nbuf > lim) {
        memdequebuf(queue, buf);
        if (!tls) {
            memfreebuf(buf);
        } else {
            glob = &g_mem.blktab[qid];
            nbuf = glob->nbuf;
            lim = memnblkbuf(qid);
            if (nbuf > lim) {
                memfreebuf(buf);
            } else {
                mempushbuf(glob, buf);
            }
        }
    }
    if (!tls) {
        mtunlkfmtx(&queue->mtx);
    }

    return;
}

/* acquire allocation block */
void *
memgetblk(long qid, size_t align, struct memtls *tls)
{

    struct membufq     *queue = ((tls)
                                 ? &tls->blktab[qid]
                                 : &g_mem.blktab[qid]);
    void               *ptr = NULL;
    void               *ret = NULL;
    long                lim;
    long                ndx;
    long                ofs;
    struct membuf      *buf;
    long                bits;
    long                pgbit;
    long                alloc;
    uintptr_t           num;
    TABHASH_ITEM_T      item;

    alloc = 0;
    if (!tls) {
        mtlkfmtx(&queue->mtx);
    }
    buf = queue->head;
    if (!buf) {
        /* allocate new buffer */
        buf = memgetblkbuf(queue);
        alloc = 1;
    }
    ndx = buf->ofs;                     // top of allocation stack
    lim = buf->max;                     // # of blocks in buffer
    ofs = ndx;
    bits = buf->bits;                   // allocation page-bitmap
    ofs += lim;                         // offset of book-keeping word
    ptr = (void *)buf->tab[ndx];        // allocation pointer
    ret = memalignptr(ptr, align);      // align allocation address
    pgbit = 1L << ndx;                  // page-bit
    buf->tab[ofs] = (uintptr_t)ptr;     // store unaligned allocation address
    ndx++;                              // adjust allocation stack top
    if (!(bits & pgbit)) {
        /* no record for the allocation page, hash one */
        item.page = memsethashpage(mempageadr(ret), MEM_BLK, 0);
        item.buf = memsethashbuf(buf, qid);
        bits |= pgbit;
        tabhashadd(g_memhashtab, &item);
        buf->bits = bits;
    }
    buf->ofs = ndx;
    if (ndx == lim) {
        /* fully-allocated buffer, dequeue */
        memdequebuf(queue, buf);
    } else if (alloc) {
        /* first allocation from buffer, queue in front */
        mempushbuf(queue, buf);
    }
    if (!tls) {
        mtunlkfmtx(&queue->mtx);
    }

    return ret;
}

/* acquire allocation run */
void *
memgetrun(long qid, size_t align, struct memtls *tls)
{
    struct membufq     *queue = ((tls)
                                 ? &tls->blktab[qid]
                                 : &g_mem.blktab[qid]);
    void               *ptr = NULL;
    long                ntry = 32;
    long                nrun;
    long                ndx;
    long                bits;
    long                mask;
    struct membuf      *buf;
    struct membuf      *head;
    long                alloc;
    TABHASH_ITEM_T      item;

    alloc = 0;
    if (!tls) {
        mtmtxlk(&queue->mtx);
    }
    buf = queue->head;
    if (buf) {
        do {
            bits = buf->bits;
            if (!bits) {
                m_waitspin();
            }
        } while (!bits && --ntry);
    } else {
        buf = memgetrunbuf(queue);
        alloc = 1;
        bits = buf->bits;
    }
    nrun = buf->max;
    if (bits) {
        ndx = m_ctz(bits);
        mask = 1L << ndx;
        ndx <<= PAGESIZELOG2;
        ptr = buf->adr + qid * ndx;
        mask = ~mask;
        bits &= mask;
        buf->bits = bits;
    }
    head = queue->head;
    if (!bits) {
        if (head) {
            head->prev = NULL;
        }
        queue->head = buf->next;
    } else if (alloc) {
        mempushbuf(queue, buf);
    }
    if (!tls) {
        mtunlkfmtx(&queue->mtx);
    }
    if (ptr) {
        item.page = memsethashpage(mempageadr(ptr), MEM_RUN, 0);
        item.buf = memsethashbuf(buf, qid);
    }

    return ptr;
}

/* release allocation run */
static struct membuf *
memputrun(struct membuf *buf, void *adr)
{
    struct membufq     *queue = buf->queue;
    long                qid = queue->qid;
    struct memtls      *tls = queue->tls;
    long                ndx = memrunnum(qid, buf, adr);
    uintptr_t           bits = buf->bits;
    uintptr_t           mask = (uintptr_t)1 << ndx;
    long                nrun = buf->max;
    long                nbuf = queue->nbuf;
    long                lim = 2;
    struct membufq     *glob;

    if (!tls) {
        mtlkfmtx(&queue->mtx);
    }
    if (bits & mask) {
        /* deal with duplicate free */
        ;
    }
    bits |= mask;
    buf->bits = bits;
    if (bits == mask) {
        memqueuebuf(queue, buf);
    } else {
        mask = (uintptr_t)1 << nrun;
        mask--;
        if (bits == mask && nbuf > lim) {
            memdequebuf(queue, buf);
            if (!tls) {
                memfreebuf(buf);
            } else {
                glob = &g_mem.runtab[qid];
                nrun = glob->nbuf;
                nbuf = glob->nbuf;
                mask = (uintptr_t)1 << nrun;
                lim = memnrunbuf(qid);
                mask--;
                if (bits == mask && nbuf > lim) {
                    memfreebuf(buf);
                } else {
                    mempushbuf(glob, buf);
                }
            }
        }
    }
    if (!tls) {
        mtunlkfmtx(&queue->mtx);
    }

    return buf;
}

/* acquire big allocation */
void *
memmapbig(size_t size, size_t align)
{
    struct membuf      *buf = memgetbuf(NULL, MEM_BIG);
    size_t              mapsz = roundup2(size, PAGESIZE);
    void               *ptr;
    void               *ret;
    TABHASH_ITEM_T      item;

    if (!buf) {

        return NULL;
    }
    ptr = mapanon(-1, mapsz, 0);
    if (ptr == MAP_FAILED) {
        memputbuf(buf, MEM_BIG);

        ptr = NULL;
    } else {
        ret = memalignptr(ptr, align);
        item.page = mempageadr(ret);
        item.buf = memsethashbuf(buf, 0);
        tabhashadd(g_memhashtab, &item);
    }

    return ptr;
}

/* free (unmap) big allocation */
static void
memfreebig(void *ptr, size_t size)
{
    tabhashdel(g_memhashtab, (uintptr_t)ptr);
    unmapanon(ptr, size);

    return;
}

/* allocate size bytes, align to boundary of align bytes */
void *
memget(size_t size, size_t align)
{
    size_t              aln = max(align, MEM_ALIGN_MIN);
    size_t              alnsz = memalnsize(size, aln);
    uintptr_t           type = membuftype(alnsz);
    void               *ptr;
    size_t              blksz;
    long                qid;

    switch (type) {
        case MEM_BLK:
            blksz = roundup2(alnsz, MEM_BLK_MIN);
            qid = memblkqid(blksz);
            ptr = memgetblk(qid, aln, t_memtls);

            break;
        case MEM_RUN:
            blksz = roundup2(alnsz, PAGESIZE);
            qid = memrunqid(blksz);
            ptr = memgetrun(qid, aln, t_memtls);

            break;
        case MEM_BIG:
            blksz = roundup2(alnsz, PAGESIZE);
            qid = membigqid(blksz);
            if (qid >= MEM_BIG_QUEUES) {
                ptr = memmapbig(blksz, aln);
            } else {
                ptr = memgetbig(qid, aln);
            }

            break;
    }

    return ptr;
}

void *
memresize(void *ptr, size_t size, size_t align, long flg)
{
    size_t      alnsz = (align) ? memalnsize(size, align) : size;
    void       *adr = memmaxsize(ptr) >= alnsz ? ptr : NULL;
    size_t      cpysz = 0;
    void       *ret = NULL;

    if (!ptr && (flg & MEM_ALLOC_ON_NULL)) {
        adr = memget(size, align);
    } else if (!adr) {
        adr = memget(size, align);
    } else {
        adr = ptr;
    }
    if (adr) {
        if (!align) {
            ret = adr;
        } else {
            ret = memalignptr(adr, align);
        }
        cpysz = min(memsize(ptr), memsize(ret));
    }
    if (cpysz) {
        memcpy(ret, ptr, cpysz);
    }
    if (!ret) {
        errno = ENOMEM;
        if (flg & MEM_FREE_ON_FAILURE) {
            memput(ptr);
        }
    }

    return ret;
}

/* release allocation */
void
memput(void *ptr)
{
    uintptr_t           page = mempageadr(ptr);
    struct memhash      hash = tabhashfind(g_memhashtab, page);

    if (hash.page) {
        struct membuf  *buf = memhashbuf(&hash);
        long            type = memhashbuftype(&hash);
        size_t          size = memhashbufsize(&hash);

        switch (type) {
            case MEM_BLK:
                memputblk(buf, ptr);

                break;
            case MEM_RUN:
                memputrun(buf, ptr);

                break;
            case MEM_BIG:
                memfreebig(buf->adr, size);

                break;
        }
    }

    return;
}

