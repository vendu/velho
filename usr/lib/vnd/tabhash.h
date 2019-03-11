#ifndef __VND_TABHASH_H__
#define __VND_TABHASH_H__

#include <stdlib.h>
#include <string.h>
#include <mach/param.h>
#include <mt/lk.h>
#include <zero/trix.h>
#include <vnd/unix.h>

extern TABHASH_TAB_T   *TABHASH_BUF;

#define tabhashfind(hash, key)  tabhashget(hash, key, 0)
#define tabhashrm(hash, key)    tabhashget(hash, key, 1)
#if defined(TABHASH_FREE)
#define tabhashdel(hash, key)   TABHASH_FREE(tabhashrm(hash, key))
#else
#define tabhashdel(hash, key)   tabhashrm(hash, key)
#endif
#if !defined(TABHASH_COPY)
#define TABHASH_COPY(src, dest) memcpy(dest, src, sizeof(TABHASH_ITEM_T))
#endif
#if !defined(TABHASH_CLEAR)
#define TABHASH_CLEAR(ptr)      memset(ptr, 0, sizeof(TABHASH_ITEM_T))
#endif

struct tabhashtab;
#define TABHASH_TAB_SIZE        (roundup2(sizeof(struct tabhashtab), CLSIZE))
#define TABHASH_BUF_SIZE        (2 * PAGESIZE)
#define TABHASH_CACHE_TABS      (TABHASH_BUF_SIZE / TABHASH_TAB_SIZE)
struct tabhashtab {
    long                ncur;
    long                nmax;
    TABHASH_TAB_T      *prev;
    TABHASH_TAB_T      *next;
    TABHASH_ITEM_T      items[TABHASH_TAB_ITEMS];
};

static __inline__ TABHASH_TAB_T *
tabhashgettab(void)
{
    TABHASH_TAB_T      *head = NULL;
    TABHASH_TAB_T      *tab;
    long                ntab;

    mtlkbit((m_atomic_t *)&TABHASH_BUF, MEM_LK_BIT_OFS);
    tab = TABHASH_BUF;
    tab = (void *)((uintptr_t)tab & ~MEM_LK_BIT);
    if (!tab) {
        uint8_t        *ptr = mapanon(-1, TABHASH_BUF_SIZE, 0);
        TABHASH_TAB_T  *cur;
        TABHASH_TAB_T  *prev;
        
        if (ptr == MAP_FAILED) {
            ptr = NULL;
            mtunlkbit((m_atomic_t *)&TABHASH_BUF, MEM_LK_BIT_OFS);
        } else {
            tab = (TABHASH_TAB_T *)ptr;
            ntab = TABHASH_CACHE_TABS - 1;
            head = (TABHASH_TAB_T *)(ptr + TABHASH_TAB_SIZE);
            prev = head;
            while (--ntab) {
                ptr += TABHASH_TAB_SIZE;
                cur = (TABHASH_TAB_T *)ptr;
                prev->next = cur;
                cur->prev = prev;
                prev = cur;
            }
            prev->next = NULL;
        }
    } else {
        head = tab->next;
    }
    m_atomwrite(&TABHASH_BUF, head);
    if (tab) {
        tab->nmax = TABHASH_TAB_ITEMS;
        ntab = TABHASH_TAB_ITEMS;
        memset(&tab->items, 0, sizeof(tab->items));
    }

    return tab;
}

static __inline__ long
tabhashadd(TABHASH_TAB_T **hashtab, TABHASH_ITEM_T *item)
{
    TABHASH_TAB_T      *head;
    TABHASH_TAB_T      *tab;
    TABHASH_TAB_T      *prev;
    long                key;
    long                ndx;
    long                lim;
    long                loop;

    key = TABHASH_HASH_ITEM(item);
    prev = NULL;
    mtlkbit((m_atomic_t *)&hashtab[key], MEM_LK_BIT_OFS);
    head = hashtab[key];
    tab = (void *)((uintptr_t)head & ~MEM_LK_BIT);
    head = tab;
    do {
        if (!tab) {
            /* allocate new subtable */
            tab = calloc(1, sizeof(TABHASH_TAB_T));
            if (!tab) {

                return -1;
            }
            /* queue new subtable in front of chain */
            tab->next = head;
            if (head) {
                head->prev = tab;
            }
            head = tab;
            tab->nmax = TABHASH_TAB_ITEMS;
        }
        ndx = tab->ncur;
        lim = tab->nmax;
        if (ndx < lim) {
            TABHASH_COPY(item, &tab[ndx]);
            ndx++;
            tab->ncur = ndx;
            loop = 0;
        } else {
            prev = tab;
            tab = tab->next;
        }
    } while (loop);
    m_atomwrite((m_atomic_t *)&hashtab[key], head);

    return 0;
}

static __inline__ TABHASH_ITEM_T
tabhashget(TABHASH_TAB_T **hashtab, const uintptr_t val, long remove)

{
    TABHASH_ITEM_T      ret = TABHASH_INVALID;
    TABHASH_TAB_T      *tab;
    TABHASH_TAB_T      *head;
    TABHASH_TAB_T      *prev;
    TABHASH_TAB_T      *next;
    TABHASH_ITEM_T     *item;
    long                key;
    long                ndx;
    long                lim;
    long                cur;

    key = TABHASH_HASH(val);
    prev = NULL;
    mtlkbit((m_atomic_t *)&hashtab[key], MEM_LK_BIT_OFS);
    head = hashtab[key];
    tab = (void *)((uintptr_t)head & ~MEM_LK_BIT);
    head = tab;
    while (tab) {
        lim = tab->nmax;
        item = &tab->items[0];
        for (cur = 0 ; cur < lim ; cur++) {
            if (!TABHASH_CMP(item, val)) {
                ret = *item;
                if (remove) {
                    /* remove item from hash */
                    ndx = tab->ncur;
                    ndx--;
                    if (cur != ndx) {
                        /* copy last item in table over removed one */
                        TABHASH_COPY(&tab->items[ndx], &tab->items[cur]);
                    }
#if defined(TABHASH_FREE)
                    TABHASH_FREE(tab->items[ndx]);
#endif
                    TABHASH_CLEAR(&tab->items[ndx]);
                    tab->ncur = ndx;
                    prev = tab->prev;
                    next = tab->next;
                    if (!ndx) {
                        /* free empty table */
                        if (prev) {
                            prev->next = next;
                        } else {
                            head = next;
                        }
                        if (next) {
                            next->prev = prev;
                        }
                        free(tab);
                    }
                } else {
                    if (prev) {
                        /* insert table into front of queue */
                        next = tab->next;
                        prev->next = next;
                        if (next) {
                            next->prev = prev;
                        }
                        tab->next = hashtab[key];
                    }
                    head = tab;
                }
                m_atomwrite((m_atomic_t *)&hashtab[key], head);

                return ret;
            }
            item++;
        }
        prev = tab;
        tab = tab->next;
    }
    m_atomwrite((m_atomic_t *)&hashtab[key], head);

    return ret;
}

#endif /* __VND_TABHASH_H__ */

