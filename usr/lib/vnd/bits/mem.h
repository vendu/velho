#ifndef __VND_BITS_MEM_H__
#define __VND_BITS_MEM_H__

#define TABHASH_SLOTS           16384
#define TABHASH_INVALID         { 0, 0 }
#define TABHASH_TAB_T           struct tabhashtab
#define TABHASH_ITEM_T          struct memhash
#define TABHASH_ITEM_WORDS      2
#define TABHASH_HDR_WORDS       4
#define TABHASH_ITEM_WORDS      2
#define TABHASH_TAB_ITEMS       ((64 - TABHASH_HDR_WORDS) / TABHASH_ITEM_WORDS)
#define TABHASH_KEY(item)       ((item)->page)
#if (WORDSIZE == 8)
#define TABHASH_HASH(key)       tmhash64(key)
#define TABHASH_HASH_ITEM(item) TABHASH_HASH((item)->page)
#else
#define TABHASH_HASH_ITEM(item) tmhash32((item)->page)
#endif
#define TABHASH_CMP(item, key)  ((item)->page == key)
#include <mach/param.h>
#include <vnd/hash.h>
#include <vnd/tabhash.h>
#include <vnd/unix.h>

extern struct memglob   g_mem ALIGNED(PAGESIZE);

static __inline__ void *
memgethashtab(void)
{
    volatile struct tabhashtab  **tabq;
    long                          ntry = 32;
    size_t                        tabsz;
    size_t                        ntab;
    struct tabhashtab            *ret;
    int8_t                       *ptr;
    struct tabhashtab            *tab;
    struct tabhashtab            *head;
    struct tabhashtab            *prev;

    ret = NULL;
    ptr = MAP_FAILED;
    tabsz = sizeof(struct tabhashtab);
    ntab = 2 * PAGESIZE / sizeof(struct tabhashtab);
    tabq = &g_mem.hashtabq;
    do {
        mtlkbit((m_atomic_t *)tabq, MEM_LK_BIT_OFS);
        ret = (void *)((uintptr_t)*tabq & ~MEM_LK_BIT);
        if (!ret) {
            if (ptr == MAP_FAILED) {
                /* map header region */
                ptr = mapanon(-1, ntab * tabsz, 0);
                if (ptr != MAP_FAILED) {
                    tab = (struct tabhashtab *)ptr;
                    ret = (struct tabhashtab *)ptr;
                    head = (struct tabhashtab *)((int8_t *)tab + tabsz);
                    head->prev = NULL;
                    prev = head;
                    while (--ntab) {
                        tab = (struct tabhashtab *)((int8_t *)tab + tabsz);
                        tab->prev = prev;
                        prev->next = tab;
                        prev = tab;
                    }
                    tab->next = NULL;
                }
            }
            if (ptr != MAP_FAILED) {
                ret = (struct tabhashtab *)ptr;
            }
        }
        if (ret) {
            head = ret->next;
            if (head) {
                head->prev = NULL;
            }
        }
        m_atomwrite((m_atomic_t *)tabq, (m_atomic_t *)head);
        if (!ret) {
            m_waitspin();
        }
    } while (!ret && --ntry);

    return ret;
}

#endif /* __VND_BITS_MEM_H__ */

