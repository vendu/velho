#include <vnd/mem/mem.h>
#include <vnd/mem/mag.h>

struct memglob g_mem ALIGNED(PAGESIZE);

static void
meminitblktab(struct g_mem *mem)
{
    float       slabsz = MEM_SMALL_SLAB;
    float       blksz;
    long        qid;

    for (qid = 0 ; qid < MEM_BLK_QUEUES ; qid++) {
        blksz = MEM_SMALL_MIN * (float)(qid + 1);
        mem->nblktab[qid] = (long)(slabsz / blksz);
    }

    return;
}

static void
meminitruntab(struct g_mem *mem)
{
    float       slabsz = MEM_RUN_SLAB;
    float       blksz;
    long        qid;

    for (qid = 0 ; qid < MEM_BLK_QUEUES ; qid++) {
        blksz = MEM_SMALL_MIN * (float)(qid + 1);
        mem->nruntab[qid] = (long)(slabsz / blksz);
    }

    return;
}

static struct membuf *
memgetbuf(long qid, long type)
{
    long                ntry = 32;
    size_t              bufsz = MEM_CACHE_BUFS * membufsize();
    struct membuf      *glob = NULL;
    struct membuf      *ret;
    int8_t             *ptr;
    struct membuf      *buf;
    struct membuf      *head;
    struct membuf      *prev;

    ret = NULL;
    ptr = MAP_FAILED;
    if (type == MEM_SMALL_BLK) {
        glob = &g_mem.blktab[qid];
    } else if (type == MEM_RUN_BLK) {
        glob = &g_mem.runtab[qid];
    }
    do {
        vndlkbit(&g_mem.bufhdrq, MEM_LK_BIT_OFS);
        head = g_mem.bufhdrq;
        ret = (void *)((uintptr_t)head & ~MEM_LK_BIT);
        if (!ret) {
            if (ptr == MAP_FAILED) {
                /* map header region */
                ptr = mapanon(-1, bufsz);
                if (ptr != MAP_FAILED) {
                    buf = (struct membuf *)ptr;
                    ret = (struct membuf *)ptr;
                    buf->type = type;
                    buf->glob = glob;
                    n = MEM_CACHE_BUFS - 1;
                    head = (struct membuf *)((int8_t *)buf + membufsize());
                    head->glob = glob;
                    head->prev = NULL;
                    prev = head;
                    while (--n) {
                        buf = (struct membuf *)((int8_t *)buf + membufsize());
                        buf->type = type;
                        buf->glob = glob;
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
        m_atomwrite(&g_mem.hdrbufq.prev, (m_atomic_t *)head);
    } while (!ret && --ntry);

    return ret;
}

static void
memputbuf(struct membuf *buf)
{
    struct membuf      *head;

    buf->base = NULL;
    buf->ptr = NULL;
    buf->prev = NULL;
    vndlkbit(&g_mem.bufhdrq, MEM_LK_BIT_OFS);
    head = (void *)((uintptr_t)g_mem.bufhdrq & ~MEM_LK_BIT);
    buf->next = head;
    if (head) {
        head->prev = buf;
    }
    m_atomwrite(&g_mem.bufhdrq, buf);

    return;
}

static struct membuf *
memallocblkbuf(long qid)
{
    struct membuf      *buf = memgetbuf();
    long                ntry = 32;
    size_t              slabsz = MEM_SMALL_SLAB;
    size_t              blksz = memblksize(qid);
    int8_t             *ptr;
    struct memmag      *mag;
    void               *stk;
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
        memputbuf(buf);
        buf = NULL;
    } else {
        mag = &buf->hdr.mag;
        nblk = memnblk(qid);
        stk = (int8_t *)buf + MEM_BUF_DATA_SIZE;
        mag->adr = ptr;
        mag->nblk = nblk;
        mag->stk = stk;
        do {
            n = min(8, nblk);
            switch (n) {
                case 8:
                    stk[7] = ptr + 7 * blksz;
                case 7:
                    stk[6] = ptr + 6 * blksz;
                case 6:
                    stk[5] = ptr + 5 * blksz;
                case 5:
                    stk[4] = ptr + 4 * blksz;
                case 4:
                    stk[3] = ptr + 3 * blksz;
                case 3:
                    stk[2] = ptr + 2 * blksz;
                case 2:
                    stk[1] = ptr + blksz;
                case 1:
                    stk[0] = ptr;
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

static void
mempushblkbuf(struct mempool *pool, struct membuf *buf)
{
    struct memtls      *tls = pool->tls;
    struct membuf      *head;

    head = pool->head;
    buf->prev = NULL;
    buf->next = head;
    if (head) {
        head->prev = buf;
        pool->head = buf;
    } else {
        pool->head = buf;
        pool->tail = buf;
    }

    return;
}

static void
memqueueblkbuf(struct mempool *pool, struct membuf *buf)
{
    struct memtls      *tls = pool->tls;
    struct membuf      *head;

    buf->next = NULL;
    head = pool->head;
    if (head) {
        buf->prev = tail;
        pool->tail = buf;
    } else {
        buf->prev = NULL;
        pool->head = buf;
        pool->tail = buf;
    }

    return;
}

static void
memdequeblkbuf(struct mempool *pool, struct membuf *buf)
{
    struct memtls      *tls = pool->tls;
    struct membuf      *head;
    struct membuf      *prev;
    struct membuf      *next;

    head = pool->head;
    prev = buf->prev;
    next = buf->next;
    if (prev) {
        prev->next = buf->next;
    } else {
        pool->head = next;
    }
    if (next) {
        next->prev = buf->prev;
    } else {
        pool->tail = prev;
    }

    return;
}

static void *
mempopblk(struct mempool *pool, long qid)
{
    struct memtls      *tls = pool->tls;
    void               *ptr = NULL;
    long                ntry = 32;
    long                qid;
    struct membuf      *head;
    struct memmag      *buf;
    long                ndx;

    if (!tls) {
        vndlkmtx(&pool->mtx);
    }
    head = pool->head;
    if (!buf && (tls)) {
        tls = NULL;
        pool = pool->glob;
        vndlkmtx(&pool->mtx);
        buf = pool->head;
    }
    if (!buf) {
        buf = memallocblkbuf(qid);
    }
    if (buf) {
        mag = &pool->buf.mag;
        ndx = mag->ndx;
        ptr = mag->stk[ndx];
        ndx++;
        mag->ndx = ndx;
        if (ndx == 1 && memnblk(qid) > 1) {
            mempushblkbuf(pool, buf);
        } else if (ndx == mag->nblk) {
            memdequeblkbuf(pool, mag);
        }
    }
    if (!tls) {
        vndunlkmtx(&pool->mtx);
    }

    return ptr;
}

static void
memputblk(struct mempool *pool, long qid, void *adr)
{
    struct memtls      *tls = pool->tls;
    long                lim = memnmagbuf(qid);
    struct membuf      *buf;
    struct membuf      *qbuf;
    struct membuf      *tail;
    struct memmag      *mag;
    void               *next;

    if (!tls) {
        vndlkmtx(&pool->mtx);
    }
    buf = pool->head;
    mag = pool->buf.mag;
    ndx = mag->ndx;
    ndx--;
    mag->stk[ndx] = adr;
    mag->ndx = ndx;
    if (ndx == mag->nblk - 1 && memnblk(qid) > 1) {
        memqueueblkbuf(pool, mag);
    } else if (!ndx) {
        if (tls) {
            tls = NULL;
            pool = buf->glob;
            vndlkmtx(&pool->mtx);
            memqueueblkbuf(pool, mag);
        }
    }
    if (!tls) {
        vndunlkmtx(&pool->mtx);
    }

    return;
}

static struct membuf *
memallocrunbuf(long qid)
{
    struct membuf      *buf = memgetbuf();
    long                ntry = 32;
    size_t              blksz = memrunblksize(qid);
    size_t              nblk = memnrunblk(qid);
    size_t              slabsz = blksz * memnrunblk(qid);
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
        memputbuf(buf);
        buf = NULL;
    } else {
        run = pool->buf.run;
        run->adr = ptr;
        run->nblk = nblk;
        run->nrunpg = MEM_RUN_MIN_PAGES * qid;
    }

    return buf;
}

static void *
memgetrunblk(struct mempool *pool, long qid)
{
    struct memtls      *tls = pool->tls;
    struct membuf      *buf;
    void               *ptr = NULL;
    long                ndx = 0;
    long                ntry = 32;
    long               *lptr;

    if (!tls) {
        vndlkmtx(&pool->mtx);
    }
    buf = pool->head;
    if (!buf && (tls)) {
        tls = NULL;
        pool = pool->glob;
        vndlkmtx(&pool->mtx);
        buf = pool->head;
    }
    if (!buf) {
        buf = memallocrunbuf(qid);
    }
    lptr = run->bits;
    do {
        ndx = m_ctz(*lptr);
    } while (!ndx && --ntry);
    if (ndx) {
        ptr = (void *)(run->base + qid * (ndx << PAGESIZELOG2));
        *lptr |= 1L << ndx;
    }
    if (*lptr == ~0L) {
        if (run->next) {
            run->next->prev = NULL;
        }
        pool->head = run->next;
    }
    if (!tls) {
        vndunlkmtx(&pool->mtx);
    }

    return ptr;
}

static void
memqueuerunbuf(struct mempool *pool, struct membuf *run)
{
    struct memtls      *tls = pool->tls;
    struct membuf      *head;
    struct membuf      *tail;

    run->next = NULL;
    head = pool->head;
    tail = pool->tail;
    if (head) {
        run->prev = tail;
        pool->tail = run;
    } else {
        run->prev = NULL;
        pool->head = run;
        pool->tail = run;
    }

    return;
}

static void
memdequerunbuf(struct mempool *pool, long glob)
{
    struct memrun      *run = &pool->buf.run;
    struct memtls      *tls = pool->tls;
    struct membuf      *prev;
    struct membuf      *next;

    prev = run->prev;
    next = run->next;
    if (prev) {
        prev->next = run->next;
    } else {
        pool->head = next;
    }
    if (next) {
        next->prev = run->prev;
    } else {
        pool->next = prev;
    }

    return;
}

static void
memputrun(struct mempool *pool, long qid, void *adr)
{
    struct memtls      *tls = pool->tls;
    struct memrun      *run = pool->head;
    struct memrun      *pool = (tls) ? &tls->runtab[qid] : &g_mem.runtab[qid];
    long                ndx = memrunblknum(run, adr);
    int32_t            *lptr = run->bits;
    struct memrun      *next;

    if (!tls) {
        vndlkmtx(&pool->mtx);
    }
    if (*lptr & (1L << ndx)) {
        /* deal with duplicate free */
        ;
    }
    if (!*lptr) {
        if (run->next) {
            run->next->prev = run->prev;
        }
        if (run->prev) {
            run->prev->next = run->next;
        }
    } else if (m_ham(*lptr) == MEM_RUN_QUEUES) {
        next = pool->head;
        run->next = next;
    }
    if (!tls) {
        vndunlkmtx(&pool->mtx);
    }

    return;
}

static void *
memgetmap(size_t size, long flg)
{
    void       *ptr;
    size_t      mapsz = roundup2(size, SYS_PAGE_SIZE);

    ptr = mapanon(-1, mapsz);
    if (ptr == MAP_FAILED) {
        ptr = NULL;
    }
    memaddmap(ptr, size);

    return ptr;
}

static void
memfreemap(void *ptr, size_t size)
{
    unmapanon(ptr, size);

    return;
}

