#include <stddef.h>
#include <string.h>
#include <zero/cdefs.h>
#include <vnd/unix.h>
#include <vnd/mem.h>

extern struct memhashtab       *g_memhashtab[MEM_HASH_SLOTS];
extern struct memglob           g_mem;
THREADLOCAL struct memtls      *t_memtls;

void *
memalloc(size_t size, size_t align, long flg)
{
    size_t              aln = max(align, MEM_ALIGN_MIN);
    void               *ptr;
    void               *ret;
    size_t              blksz = memalnsize(size, aln);
    long                type = memalloctype(blksz);
    long                qid;
    struct memhash      hash;

    switch (type) {
        case MEM_BLK:
            qid = memblkqueue(blksz);
            ptr = memgetblk(&t_memtls->blktab[qid], qid);

            break;
        case MEM_RUN:
            qid = memrunqueue(blksz);
            ptr = memgetrun(&t_memtls->runtab[qid], qid);

            break;
        case MEM_BIG:
            ptr = memmapbig(blksz);

            break;
    }
    if (flg & MEM_ZERO_BIT) {
        memset(ptr, 0, size);
    }
    ret = memalignptr(ptr, aln);

    return ret;
}

void
memfree(void *adr)
{
    uintptr_t            page = (uintptr_t)adr & ~(PAGESIZE - 1);
    struct memhash       hash = tabhashfind(g_memhashtab, page);
    struct membuf       *buf = memhashbuf(hash);
    long                 type = memhashbuftype(hash);
    size_t               size = memhashbufsize(hash);

    switch (type) {
        case MEM_BLK:
            memputblk(buf, adr);

            break;
        case MEM_RUN:
            memputrun(buf, adr);

            break;
        case MEM_BIG:
            memfreebig((void *)page, size);

            break;
    }

    return;
}

void *
malloc(size_t size)
{
    void *ptr = memalloc(size, 0, 0);

    return ptr;
}

void *
calloc(size_t n, size_t size)
{
    size_t       blksz = n * size;
    void        *ptr;

    if (!blksz) {

        return NULL;
    }
    if (blksz >= size) {
        ptr = memalloc(size, 0, 1);
    }

    return ptr;
}

void *
realloc(void *adr, size_t size)
{
    void        *ptr = memalloc(size, 0, 0);
}

void *
aligned_alloc(size_t align, size_t size)
{
    void *ptr = memalloc(size, align, 0);

    return ptr;
}

