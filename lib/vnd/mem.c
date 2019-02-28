#include <vnd/mem.h>

#define MEM_HASH_SLOTS  32768
struct memhashtab      *g_memhashtab[MEM_HASH_SLOTS];
struct memglob          g_mem ALIGNED(PAGESIZE);

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
static void
meminitruntab(struct memglob *mem)
{
    float       slabsz;
    float       runsz;
    float       nrun;
    float       multi;
    long        qid;

    for (qid = 0 ; qid < MEM_RUN_QUEUES ; qid++) {
        slabsz = memrunslabsz(qid);
        runsz = PAGESIZE * (float)(qid + 1);
        nrun = slabsz / runsz;
        multi = 1.0 / runsz;
        mem->nruntab[qid] = (long)nrun;
        mem->rundivtab[qid] = multi;
    }

    return;
}

static struct membuf *
memgetbuf(long qid, long type)
{
    struct membuf     **hdrq;
    long                ntry = 32;
    size_t              bufsz;
    size_t              nbuf;
    struct membuf      *ret;
    int8_t             *ptr;
    struct membuf      *buf;
    struct membuf      *head;
    struct membuf      *prev;

    ret = NULL;
    ptr = MAP_FAILED;
    if (type == MEM_BLK) {
        bufsz = memblkbufsize();
        nbuf = MEM_CACHE_BLK_BUFS;
        hdrq = &g_mem.blkhdrq;
    } else if (type == MEM_RUN) {
        bufsz = memrunbufsize();
        nbuf = MEM_CACHE_RUN_BUFS;
        hdrq = &g_mem.runhdrq;
    } else {
        bufsz = membufsize();
        nbuf = MEM_CACHE_BUFS;
        hdrq = &g_mem.bufhdrq;
    }
    do {
        mtlkbit(hdrq, MEM_LK_BIT_OFS);
        ret = (void *)((uintptr_t)*hdrq & ~MEM_LK_BIT);
        if (!ret) {
            if (ptr == MAP_FAILED) {
                /* map header region */
                ptr = mapanon(-1, nbuf * bufsz);
                if (ptr != MAP_FAILED) {
                    buf = (struct membuf *)ptr;
                    ret = (struct membuf *)ptr;
                    buf->type = type;
                    n = MEM_CACHE_BUFS - 1;
                    head = (struct membuf *)((int8_t *)buf + bufsz);
                    head->prev = NULL;
                    prev = head;
                    while (--n) {
                        buf = (struct membuf *)((int8_t *)buf + bufsz);
                        buf->type = type;
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
        m_atomwrite(hdrq, (m_atomic_t *)head);
    } while (!ret && --ntry);

    return ret;
}

static void
memputbuf(struct membuf *buf, long type)
{
    struct membuf     **hdrq;
    struct membuf      *head;

    buf->base = NULL;
    buf->ptr = NULL;
    buf->prev = NULL;
    if (type == MEM_BLK) {
        hdrq = &g_mem.blkhdrq;
    } else if (type == MEM_RUN) {
        hdrq = &g_mem.runhdrq;
    } else {
        hdrq = &g_mem.bufhdrq;
    }
    mtlkbit(hdrq, MEM_LK_BIT_OFS);
    head = (void *)((uintptr_t)*hdrq & ~MEM_LK_BIT);
    buf->next = head;
    if (head) {
        head->prev = buf;
    }
    m_atomwrite(hdrq, buf);

    return;
}

static struct membuf *
memallocblkbuf(struct membufq *queue, long qid)
{
    struct membuf      *buf = memgetbuf(qid, MEM_BLK);
    long                ntry = 32;
    size_t              slabsz = MEM_BLK_SLAB;
    size_t              blksz = memblksize(qid);
    int8_t             *ptr;
    struct memblkhdr   *hdr;
    void              **stk;
    long                nblk;
    long                n;

    if (!buf) {

        return NULL;
    }
    do {
        ptr = mapanon(-1, slabsz);
        if (ptr == MAP_FAILED) {
            m_waitspin();
        }
    } while (ptr == MAP_FAILED && --ntry);
    if (ptr == MAP_FAILED) {
        memputbuf(buf, MEM_BLK);
        buf = NULL;
    } else {
        hdr = &buf->hdr.blk;
        nblk = memnblk(qid);
        stk = &buf->stk;
        hdr->adr = ptr;
        hdr->nblk = nblk;
        do {
            n = min(8, nblk);
            switch (n) {
                case 8:
                    stk[7].adr = (uintptr_t)(ptr + 7 * blksz);
                case 7:
                    stk[6].adr = (uintptr_t)(ptr + 6 * blksz);
                case 6:
                    stk[5].adr = (uintptr_t)(ptr + 5 * blksz);
                case 5:
                    stk[4].adr = (uintptr_t)(ptr + 4 * blksz);
                case 4:
                    stk[3].adr = (uintptr_t)(ptr + 3 * blksz);
                case 3:
                    stk[2].adr = (uintptr_t)(ptr + 2 * blksz);
                case 2:
                    stk[1].adr = (uintptr_t)(ptr + blksz);
                case 1:
                    stk[0].adr = (uintptr_t)ptr;
                case 0:

                    break;
            }
            nblk -= n;
            ptr += 8 * blksz;
            stk += 8;
        } while (nblk);
    }

    return buf;
}

static struct membuf *
memallocrunbuf(struct membufq *queue, long qid)
{
    struct membuf      *buf = memgetbuf();
    long                ntry = 32;
    size_t              blksz = memrunsize(qid);
    size_t              nblk = memnrun(qid);
    size_t              slabsz = blksz * memnrun(qid);
    void               *ptr;
    struct memrun      *run;
    int32_t            *i32p;

    if (!buf) {

        return NULL;
    }
    do {
        ptr = mapanon(-1, slabsz);
        if (ptr == MAP_FAILED) {
            m_waitspin();
        }
    } while (ptr == MAP_FAILED && --ntry);
    if (ptr == MAP_FAILED) {
        memputbuf(buf, MEM_RUN_BUF);
        buf = NULL;
    } else {
        run = buf->buf.run;
        run->adr = ptr;
        run->size = slabsz;
        run->nblk = nblk;
    }

    return buf;
}

static void
mempushbuf(struct membufq *queue, struct membuf *buf)
{
    struct memtls       *tls = queue->tls;
    struct membuf       *next = queue->head;

    buf->prev = NULL;
    buf->next = next;
    if (!next) {
        queue->tail = buf;
    }
    queue->head = buf;

    return;
}

static void
memqueuebuf(struct membufq *queue, struct membuf *buf)
{
    struct memtls      *tls = queue->tls;
    struct membuf      *prev = queue->tail;

    buf->next = NULL;
    buf->prev = prev;
    if (prev) {
        prev->next = buf;
    } else {
        queue->head = buf;
    }
    queue->tail = buf;

    return;
}

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

static void
memputblk(struct membufq *queue, long qid, void *adr)
{
    struct memtls      *tls = buf->tls;
    long                lim = memnmagbuf(qid);
    struct membuf      *buf;
    struct membuf      *tail;
    struct memmag      *mag;
    void               *next;

    if (!tls) {
        mtlkmtx(&queue->mtx);
    }
    buf = buf->prev;
    mag = buf->buf.mag;
    ndx = mag->ndx;
    ndx--;
    mag->stk[ndx] = adr;
    mag->ndx = ndx;
    if (ndx == mag->nblk - 1 && memnblk(qid) > 1) {
        memqueuebuf(queue, buf);
    } else if (!ndx) {
        if (tls) {
            tls = NULL;
            mtlkmtx(&buf->mtx);
            memqueuebuf(buf, mag);
        }
    }
    if (!tls) {
        mtunlkmtx(&queue->mtx);
    }

    return;
}

static void *
memgetblk(struct membufq *queue, long qid, struct membuf **retbuf)
{
    void               *ptr = NULL;
    long                ndx = 0;
    long                ntry = 32;
    struct memblkhdr   *hdr;
    long               *bmap;
    struct membuf      *next;
    long                lock;
    long                ndx;
    uintptr_t           pgnum;
    struct memhash      hash;

    buf = queue->head;
    lock = 0;
    if (!buf) {
        queue = &g_mem.blktab[qid];
        lock = 1;
        mtlkmtx(&queue->mtx);
        buf = queue->head;
    }
    if (!buf) {
        lock = 1;
        mtlkmtx(&queue->mtx);
        buf = memallocrunbuf(qid);
    }
    hdr = &buf->hdr.blk;
    ndx = hdr->ndx;
    ptr = (void *)ptr->stk[ndx].adr;
    if (!membufpagebit(buf, membufpagenum(ptr))) {
        hash.page = mempageadr(ptr);
        hash.buf = memsethashbuf(buf, qid);
        tabhashadd(&g_memhashtab, &hash);
    }
    ndx++;
    next = queue->head;
    hdr->ndx = ndx;
    if (ndx == lim - 1) {
        memqueuebuf(queue, buf);
    } else if (ndx == 1) {
        mempushbuf(queue, buf);
    }
    if (lock) {
        mtunlkmtx(&queue->mtx);
    }
    pgnum = membufpagenum(buf, ptr);
    if ((ptr) {
        hash.page = mempageadr(ptr);
        hash.buf = memhashsetbuf(buf, qid);
    }

    return ptr;
}

static void *
memgetrun(struct membufq *queue, long qid, struct membuf **retbuf)
{
    void               *ptr = NULL;
    long                ndx = 0;
    long                ntry = 32;
    long                bit = 1L << ndx;
    struct membuf      *head;
    struct memrunhdr   *hdr;
    long               *bmap;
    long                lock;

    buf = queue->head;
    lock = 0;
    if (!buf) {
        queue = &g_mem.runtab[qid];
        lock = 1;
        mtlkmtx(&queue->mtx);
        buf = queue->head;
    }
    if (!buf) {
        buf = memallocrunbuf(qid);
    }
    hdr = &buf->hdr.run;
    bmap = &hdr->bmap;
    do {
        ndx = clz(*bmap);
    } while (!ndx && --ntry);
    if (ndx) {
        ptr = (void *)(hdr->base + qid * (ndx << PAGESIZELOG2));
        *bmap |= bit;
    }
    if (ham(*bmap) == nrun) {
        head = queue->head;
        if (head) {
            head->prev = NULL;
        }
        queue->head = buf->next;
    } else if (*bmap == bit) {
        buf->next = next;
        if (head) {
            head->prev = buf;
        }
        queue->head = buf;
    }
    if (lock) {
        mtunlkmtx(&queue->mtx);
    }
    if (ptr) {
        hash.page = mempageadr(ptr);
        hash.buf = memhashsetbuf(buf, qid);
    }

    return ptr;
}

static void
memqueuerunbuf(struct membuf *pool, struct membuf *buf)
{
    struct membuf      *tail;

    buf->next = NULL;
    tail = pool->next;
    if (tail) {
        buf->prev = tail;
        tail->next = buf;
    } else {
        buf->prev = NULL;
        pool->prev = buf;
        pool->next = buf;
    }

    return;
}

static void
memdequerunbuf(struct membuf *pool, struct membuf *buf)
{
    struct membuf      *prev;
    struct membuf      *next;

    prev = buf->prev;
    next = buf->next;
    if (prev) {
        prev->next = run->next;
    } else {
        pool->prev = next;
    }
    if (next) {
        next->prev = next;
    } else {
        pool->next = prev;
    }

    return;
}

static void
memputrun(struct membuf *buf, long qid, void *adr)
{
    struct memrun      *pool = &g_mem.runtab[qid];
    long                ndx = memrunnum(run, adr);
    int32_t            *lptr = &run->bmap;
    long                bit = 1L << ndx;
    struct membuf      *head;
    struct membuf      *prev;
    struct memrun      *next;

    mtlkmtx(&buf->mtx);
    if (!(*lptr & bit)) {
        /* deal with duplicate free */
        ;
    }
    if (!*lptr) {
        head = pool->prev;
        if (head) {
            head->prev = buf;
        } else {
            pool->prev = buf;
            pool->next = prev;
        }
    } else if (ham(*lptr) == MEM_RUN_QUEUES - 1) {
        prev = buf->prev;
        next = buf->next;
        if (prev) {
            prev->next = buf->next;
        } else {
            pool->prev = next;
        }
        if (next) {
            next->prev = buf->next;
        } else {
            pool->next = buf;
        }
    }
    *lptr |= bit;
    mtunlkmtx(&buf->mtx);

    return;
}

static void *
memmapbig(size_t size, long flg, struct membuf **retbuf)
{
    struct membuf      *buf = memgetbuf();
    void               *ptr;
    struct memblk       blk;

    if (!buf) {

        return NULL;
    }
    ptr = mapanon(-1, mapsz);
    if (ptr == MAP_FAILED) {
        memputbuf(buf, MEM_BIG);

        ptr = NULL;
    } else {
        blk.page = (uintptr_t)ptr & ~(PAGESIZE - 1);
        blk.info = MEM_BIG_BIT;
        tabhashadd(&g_memhashtab, &blk);
    }
    if (ptr) {
        hash.page = mempageadr(ptr);
        hash.buf = memhashsetbuf(buf, qid);
    }

    return ptr;
}

static void
memfreebig(void *ptr, size_t size)
{
    tabhashdel(&t_memhashtab, ptr);
    unmapanon(ptr, size);

    return;
}

