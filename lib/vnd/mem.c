#include <errno.h>
#include <mt/mtx.h>
#include <vnd/mem.h>
#include <vnd/unix.h>
#include <vnd/bitop.h>

struct tabhashtab              *g_memhashtab[MEM_HASH_SLOTS];
struct memglob                  g_mem ALIGNED(PAGESIZE);
THREADLOCAL struct memtls      *t_memtls;

static void
meminitblktabs(struct memglob *mem)
{
    float       slabsz = MEM_BLK_SLAB;
    float       blksz;
    float       nblk;
    float       multi;
    long        qid;

    for (qid = 0 ; qid < MEM_BLK_QUEUES ; qid++) {
        blksz = MEM_BLK_MIN * (float)(qid + 1);
        nblk = slabsz / blksz;
        multi = 1.0 / blksz;
        mem->nblktab[qid] = (long)nblk;
        mem->blkdivtab[qid] = multi;
    }

    return;
}

#define memrunslabsz(qid)                                               \
    (((qid) <= MEM_RUN_MID_QUEUE_MIN)                                   \
     ? (MEM_RUN_MAX >> 1)                                               \
     : (((qid) <= MEM_RUN_BIG_QUEUE_MIN)                                \
        ? MEM_RUN_MAX                                                   \
        : (2 * MEM_RUN_MAX)))
static void
meminitruntab(struct memglob *mem)
{
    float       slabsz;
    float       runsz;
    float       nrun;
    float       multi;
    long        qid;

    for (qid = 0 ; qid < MEM_RUN_QUEUES ; qid++) {
        slabsz = (float)memrunslabsz(qid);
        runsz = PAGESIZE * (float)(qid + 1);
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
memallocblkbuf(struct membufq *queue)
{
    long                qid = queue->qid;
    struct membuf      *buf = memgetbuf(queue, MEM_BLK);
    long                ntry = 32;
    size_t              slabsz = MEM_BLK_SLAB;
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
memallocrunbuf(struct membufq *queue)
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
    long                lim = memnblkbuf(qid);
    void               *adr;
    long                ndx;
    long                max;
    long                nbuf;

    if (!tls) {
        mtlkfmtx(&queue->mtx);
    }
    adr = memgetptr(ptr);
    ndx = buf->cur;
    max = buf->max;
    ndx--;
    nbuf = queue->nbuf;
    max--;
    buf->tab[ndx] = (uintptr_t)adr;
    buf->cur = ndx;
    if (ndx == max) {
        memqueuebuf(queue, buf);
    } else if (!ndx && nbuf >= lim) {
        if (tls) {
            tls = NULL;
            mtlkfmtx(&queue->mtx);
            memqueuebuf(queue, buf);
        }
    }
    if (!tls) {
        mtunlkfmtx(&queue->mtx);
    }

    return;
}

/* acquire allocation block */
void *
memgetblk(struct membufq *queue, size_t align)
{
    long                qid = queue->qid;
    void               *ptr = NULL;
    void               *ret = NULL;
    long                lim;
    long                ndx;
    long                ofs;
    struct membuf      *buf;
    long                bits;
    long                pgbit;
    long                lock;
    uintptr_t           num;
    TABHASH_ITEM_T      item;

    /* try thread-local buffer */
    buf = queue->head;
    lock = 0;
    if (!buf) {
        /* try global buffer */
        queue = &g_mem.blktab[qid];
        lock = 1;
        mtlkfmtx(&queue->mtx);
        buf = queue->head;
    }
    if (!buf) {
        /* allocate new buffer */
        lock = 1;
        mtlkfmtx(&queue->mtx);
        buf = memallocrunbuf(queue);
    }
    bits = buf->bits;                   // allocation page-bitmap
    ndx = buf->cur;                     // top of allocation stack
    lim = buf->max;                     // # of blocks in buffer
    ofs = ndx + lim;                    // offset of book-keeping word
    ptr = (void *)buf->tab[ndx];        // allocation pointer
    lim--;                              // adjust stack limit
    ret = memalignptr(ptr, align);      // align allocation address
    ofs = membufblknum(qid, buf, ret);  // ID of aligned allocation in buffer
    num = membufpagenum(buf, ptr);      // ID of unaligned allocation in buffer
    pgbit = 1L << num;                  // page-bit
    buf->tab[ofs] = (uintptr_t)ptr;     // store unaligned allocation address
    ndx++;                              // adjust allocation stack top
    if (!(bits & pgbit)) {
        /* no record for the allocation page, hash one */
        item.page = mempageadr(ret);
        item.buf = memsethashbuf(buf, qid);
        bits |= pgbit;
        tabhashadd(g_memhashtab, &item);
        buf->bits = bits;
    }
    buf->cur = ndx;
    if (ndx == lim) {
        /* was fully-allocated buffer, queue in back */
        memqueuebuf(queue, buf);
    } else if (ndx == 1) {
        /* first allocation from buffer, queue in front */
        mempushbuf(queue, buf);
    }
    if (lock) {
        mtunlkfmtx(&queue->mtx);
    }

    return ret;
}

/* acquire allocation run */
void *
memgetrun(struct membufq *queue, size_t align)
{
    long                qid = queue->qid;
    void               *ptr = NULL;
    long                ntry = 32;
    long                nrun;
    long                ndx;
    long                bits;
    long                mask;
    struct membuf      *buf;
    struct membuf      *head;
    long                lock;
    long                alloc;
    TABHASH_ITEM_T      item;

    buf = queue->head;
    lock = 0;
    alloc = 0;
    if (!buf) {
        queue = &g_mem.runtab[qid];
        lock = 1;
        mtlkfmtx(&queue->mtx);
        buf = queue->head;
    }
    if (!buf) {
        buf = memallocrunbuf(queue);
        alloc = 1;
    }
    do {
        bits = buf->bits;
        if (!bits) {
            m_waitspin();
        }
    } while (!bits && --ntry);
    nrun = buf->max;
    if (bits) {
        ndx = lo1bit(bits);
        mask = 1L << ndx;
        ptr = (void *)(buf->adr + qid * (ndx << PAGESIZELOG2));
        mask = ~mask;
        bits &= mask;
        buf->bits = bits;
    }
    head = queue->head;
    if (m_ham(bits) == nrun) {
        if (head) {
            head->prev = NULL;
        }
        queue->head = buf->next;
    } else if (alloc) {
        buf->next = head;
        if (head) {
            head->prev = buf;
        }
        queue->head = buf;
    }
    if (lock) {
        mtunlkfmtx(&queue->mtx);
    }
    if (ptr) {
        item.page = mempageadr(ptr);
        item.buf = memhashsetbuf(buf, qid);
    }

    return ptr;
}

/* release allocation run */
static void
memputrun(struct membuf *buf, void *adr)
{
    struct membufq     *queue = buf->queue;
    long                qid = queue->qid;
    long                ndx = memrunnum(qid, buf, adr);
    int32_t             bits = buf->bits;
    long                mask = 1L << ndx;
    long                nrun = buf->max;
    struct membuf      *head;
    struct membuf      *prev;
    struct membuf      *next;

    mtlkfmtx(&queue->mtx);
    if (!(bits & mask)) {
        /* deal with duplicate free */
        ;
    }
    nrun--;
    prev = buf->prev;
    next = buf->next;
    if (!bits) {
        head = queue->head;
        if (head) {
            head->prev = buf;
        } else {
            queue->tail = prev;
        }
        queue->head = buf;
    } else if (m_ham(bits) == nrun) {
        if (prev) {
            prev->next = buf->next;
        } else {
            queue->head = next;
        }
        if (next) {
            next->prev = buf->next;
        } else {
            queue->tail = buf;
        }
    }
    bits |= mask;
    buf->bits = bits;
    mtunlkfmtx(&queue->mtx);

    return;
}

/* acquire big allocation */
void *
memgetbig(size_t size, size_t align)
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
    void               *ptr;
    size_t              alnsz = memalnsize(size, aln);
    long                type = memalloctype(alnsz);
    size_t              blksz;
    long                qid;

    switch (type) {
        case MEM_BLK:
            blksz = roundup2(alnsz, MEM_BLK_MIN);
            qid = memblkqid(blksz);
            ptr = memgetblk(&t_memtls->blktab[qid], aln);

            break;
        case MEM_RUN:
            blksz = roundup2(alnsz, PAGESIZE);
            qid = memrunqid(blksz);
            ptr = memgetrun(&t_memtls->runtab[qid], aln);

            break;
        case MEM_BIG:
            blksz = roundup2(alnsz, PAGESIZE);
            ptr = memgetbig(blksz, aln);

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
            memrel(ptr);
        }
    }

    return ret;
}

/* release allocation */
void
memrel(void *ptr)
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

