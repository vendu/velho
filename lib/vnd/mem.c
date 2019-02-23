#include <mt/lk.h>
#include <mt/mtx.h>
#include <vnd/mem.h>
#include <vnd/unix.h>
#include <vnd/bitop.h>

#define TABHASH_TAB_T           struct tabhashtab
#define TABHASH_ITEM_T          struct memblk
#define TABHASH_ITEM_WORDS      2
#define TABHASH_SLOTS           MEM_HASH_SLOTS
#define TABHASH_KEY(item)       ((item)->page)
#if (WORDSIZE == 8)
#define TABHASH_HASH(item)      tmhash64((item)->page)
#else
#define TABHASH_HASH(item)      tmhash32((item)->page
#endif
#define TABHASH_CMP(item, key)  ((item)->page == key)
#include <vnd/hash.h>
#include <vnd/tabhash.h>

#define MEM_HASH_SLOTS  32768
struct memglob          g_mem ALIGNED(PAGESIZE);

static void
meminithash(struct memglob *mem, size_t nslot)
{
    void       *ptr;

    ptr = mapanon(-1, nslot * sizeof(TABHASH_TAB_T *), 0);
    if (ptr == MAP_FAILED) {

        exit(1);
    }
    mem->hash = ptr;
}

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

static void
meminitruntabs(struct memglob *mem)
{
    float       slabsz = MEM_RUN_SLAB;
    float       runsz;
    float       nrun;
    float       multi;
    long        qid;

    for (qid = 0 ; qid < MEM_RUN_QUEUES ; qid++) {
        runsz = MEM_BLK_MIN * (float)(qid + 1);
        nrun = slabsz / runsz;
        multi = 1.0 / runsz;
        mem->nruntab[qid] = (long)nrun;
        mem->rundivtab[qid] = multi;
    }

    return;
}

static void
meminit(struct memglob *mem)
{
    if (!(g_mem.flg & MEM_INIT_BIT)) {
        mtlkfmtx(&g_mem.mtx);
        if (g_mem.flg & MEM_INIT_BIT) {
            mtunlkfmtx(&g_mem.mtx);

            return;
        }
    }
    meminithash(mem, MEM_HASH_SLOTS);
    meminitblktabs(mem);
    meminitruntabs(mem);
    g_mem.flg |= MEM_INIT_BIT;
    mtunlkfmtx(&g_mem.mtx);

    return;
}

static struct membuf *
memgetbuf(long qid, long type)
{
    long                ntry = 32;
    size_t              bufsz = MEM_CACHE_BUFS * membufsize();
    struct membuf      *ret;
    int8_t             *ptr;
    struct membuf      *buf;
    struct membuf      *head;
    struct membuf      *prev;
    long                nblk;

    ret = NULL;
    ptr = MAP_FAILED;
    do {
        mtlkbit((m_atomic_t *)&g_mem.bufhdrq, MEM_LK_BIT_OFS);
        head = (struct membuf *)g_mem.bufhdrq;
        ret = (void *)((uintptr_t)head & ~MEM_LK_BIT);
        if (!ret) {
            if (ptr == MAP_FAILED) {
                /* map header region */
                ptr = mapanon(-1, bufsz, 0);
                if (ptr != MAP_FAILED) {
                    buf = (struct membuf *)ptr;
                    ret = (struct membuf *)ptr;
                    nblk = MEM_CACHE_BUFS - 1;
                    head = (struct membuf *)((int8_t *)buf + membufsize());
                    head->prev = NULL;
                    prev = head;
                    while (--nblk) {
                        buf = (struct membuf *)((int8_t *)buf + membufsize());
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
            head = (struct membuf *)ret->next;
            if (head) {
                head->prev = NULL;
            }
        }
        m_atomwrite(&g_mem.bufhdrq, (m_atomic_t *)head);
    } while (!ret && --ntry);

    return ret;
}

static void
memputbuf(struct membuf *buf)
{
    struct membuf      *head;

    buf->adr = NULL;
    buf->prev = NULL;
    mtlkbit((m_atomic_t *)&g_mem.bufhdrq, MEM_LK_BIT_OFS);
    head = (void *)((uintptr_t)g_mem.bufhdrq & ~MEM_LK_BIT);
    buf->next = head;
    if (head) {
        head->prev = buf;
    }
    m_atomwrite(&g_mem.bufhdrq, buf);

    return;
}

static struct membuf *
memallocblkbuf(struct membufq *queue, long qid)
{
    struct membuf      *buf = memgetbuf(qid, MEM_SMALL_BLK);
    long                ntry = 32;
    size_t              slabsz = MEM_BLK_SLAB;
    size_t              blksz = memblksize(qid);
    struct memmag      *mag;
    int8_t             *ptr;
    void              **stk;
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
        memputbuf(buf);
        buf = NULL;
    } else {
        mag = &buf->hdr.mag;
        nblk = memnblk(qid);
        stk = mag->stk;
        buf->adr = ptr;
        mag->nblk = nblk;
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
            buf->qid = qid;
            buf->prev = NULL;
            buf->next = NULL;
            queue->head = buf;
            queue->tail = buf;
        } while (nblk);
    }

    return buf;
}

static void
mempushblkbuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf *next;

    buf->prev = NULL;
    next = queue->head;
    if (next) {
        next->prev = buf;
    } else {
        queue->tail = NULL;
    }
    buf->next = next;
    queue->head = buf;

    return;
}

static void
memqueueblkbuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf      *tail;

    tail = queue->tail;
    buf->next = NULL;
    if (tail) {
        buf->prev = tail;
    } else {
        queue->head = buf;
    }
    queue->tail = buf;

    return;
}

static void
memdequeblkbuf(struct membufq *queue, struct membuf *buf)
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
    struct memtls      *tls = queue->tls;
    long                lim = memnblk(qid);
    struct membuf      *buf = queue->head;
    struct memmag      *mag;
    long                ndx;

    if (!tls) {
        mtlkfmtx(&queue->mtx);
    }
    mag = &buf->hdr.mag;
    ndx = mag->ndx;
    ndx--;
    mag->stk[ndx] = adr;
    mag->ndx = ndx;
    if (ndx == lim - 1 && lim > 1) {
        memqueueblkbuf(queue, buf);
    }
    if (!tls) {
        mtunlkfmtx(&queue->mtx);
    }

    return;
}

static struct membuf *
memallocrunbuf(struct membufq *queue, long qid)
{
    struct membuf      *buf = memgetbuf(qid, MEM_RUN_BLK);
    long                ntry = 32;
    size_t              blksz = memrunblksize(qid);
    size_t              nblk = memnrun(qid);
    size_t              slabsz = blksz * nblk;
    struct memrun      *run;
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
        memputbuf(buf);
        buf = NULL;
    } else {
        run = &buf->hdr.run;
        buf->adr = ptr;
        run->size = slabsz;
        run->nblk = nblk;
        run->bits = (1L << nblk) - 1;
        buf->qid = qid;
        buf->prev = NULL;
        buf->prev = NULL;
        queue->head = buf;
        queue->tail = buf;
    }

    return buf;
}

static void
memqueuerunbuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf      *tail;

    buf->next = NULL;
    tail = queue->tail;
    if (tail) {
        buf->prev = tail;
    } else {
        queue->head = buf;
    }
    queue->tail = buf;

    return;
}

static void
memdequerunbuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf      *prev;
    struct membuf      *next;

    prev = buf->prev;
    next = buf->next;
    if (prev) {
        prev->next = next;
    } else {
        queue->head = next;
    }
    if (next) {
        next->prev = next;
    } else {
        queue->tail = prev;
    }

    return;
}

static void
memputrun(struct membuf *buf, long qid, void *adr)
{
    long                ndx = memrunblknum(buf, adr);
    struct memrun      *run = &buf->hdr.run;
    long                bit = 1L << ndx;
    long               *bmap = &run->bits;
    struct membufq     *queue = &g_mem.runtab[qid];

    mtlkfmtx(&queue->mtx);
    if (!(*bmap & bit)) {
        /* deal with duplicate free */
        ;
    }
    *bmap |= bit;
    if (m_ham(*bmap) == 1) {
        memqueuerunbuf(queue, buf);
    }
    mtunlkfmtx(&queue->mtx);

    return;
}

void *
memgetblk(struct membufq *queue, long qid)
{
    void               *ptr = NULL;
    long                lim = memnblk(qid);
    struct membuf      *buf;
    struct memmag      *mag;
    long                ndx;
    long                lock;
    struct memblk       blk;

    if (!(g_mem.flg & MEM_INIT_BIT)) {
        meminit(&g_mem);
    }
    buf = queue->head;
    lock = 0;
    if (!buf) {
        queue = &g_mem.blktab[qid];
        mtlkfmtx(&queue->mtx);
        buf = queue->head;
        lock = 1;
    }
    if (!buf) {
        buf = memallocblkbuf(queue, qid);
    }
    if (buf) {
        mag = &buf->hdr.mag;
        ndx = mag->ndx;
        ptr = mag->stk[ndx];
        ndx++;
        mag->ndx = ndx;
        if (ndx == 1 && lim > 1) {
            mempushblkbuf(queue, buf);
        } else if (ndx == mag->nblk) {
            memdequeblkbuf(queue, buf);
        }
    }
    if (lock) {
        mtunlkfmtx(&queue->mtx);
    }
    blk.page = (uintptr_t)ptr;
    blk.info = (uintptr_t)buf;
    tabhashadd(g_mem.hash, &blk);

    return ptr;
}

void *
memgetrunblk(struct membufq *queue, long qid)
{
    void               *ptr = NULL;
    long                ndx;
    long                bit;
    struct membuf      *buf;
    struct memrun      *run;
    struct membuf      *next;
    long               *bmap;
    struct memblk       blk;

    mtlkfmtx(&queue->mtx);
    buf = queue->head;
    if (!buf) {
        buf = memallocrunbuf(queue, qid);
    }
    run = &buf->hdr.run;
    bmap = &run->bits;
    ndx = m_clz(*bmap);
    bit = 1L << ndx;
    if (ndx) {
        ptr = (void *)(buf->adr + qid * (ndx << PAGESIZELOG2));
        *bmap &= ~bit;
    }
    if (!m_ham(*bmap)) {
        next = buf->next;
        if (next) {
            next->prev = NULL;
        } else {
            queue->tail = NULL;
        }
        queue->head = next;
    }
    mtunlkfmtx(&queue->mtx);
    blk.page = (uintptr_t)ptr;
    blk.info = (uintptr_t)buf | MEM_RUN_BLK_BIT;
    tabhashadd(g_mem.hash, &blk);

    return ptr;
}

void *
memmapbig(size_t size)
{
    void               *ptr;
    size_t              mapsz = roundup2(size, PAGESIZE);
    struct memblk       blk;

    ptr = mapanon(-1, size, 0);
    if (ptr == MAP_FAILED) {
        ptr = NULL;
    } else {
        blk.page = (uintptr_t)ptr;
        blk.info = mapsz | MEM_BIG_BLK_BIT;
        tabhashadd(g_mem.hash, &blk);
    }

    return ptr;
}

static void
memfreebig(void *ptr, size_t size)
{
    unmapanon(ptr, size);

    return;
}

