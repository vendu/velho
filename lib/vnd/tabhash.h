#ifndef __VND_TABHASH_H__
#define __VND_TABHASH_H__

#include <string.h>
#include <stdlib.h>

#define TABHASH_HDR_WORDS       4
#define TABHASH_TAB_ITEMS       ((64 - TABHASH_HDR_WORDS) / TABHASH_ITEM_WORDS)

#define tabhashfind(hash, key)  tabhashget(hash, key, 0)
#define tabhashrm(hash, key)    tabhashget(hash, key, 1)
#define tabhashdel(hash, key)   TABHASH_FREE(tabhashrm(hash, key))
#define tabhashcpy(src, dest)   memcpy(dest, src, sizeof(TABHASH_ITEM_T))
#define tabhashclr(ptr)         memset(ptr, 0, sizeof(TABHASH_ITEM_T))

struct tabhashtab {
    long                ncur;
    long                nmax;
    TABHASH_TAB_T      *prev;
    TABHASH_TAB_T      *next;
    TABHASH_ITEM_T      items[TABHASH_TAB_ITEMS];
};

static __inline__ long
tabhashadd(TABHASH_TAB_T **hashtab, TABHASH_ITEM_T *item)
{
    TABHASH_TAB_T      *tab;
    TABHASH_TAB_T      *prev;
    long                ndx;
    long                loop;
    long                alloc;

    ndx = TABHASH_HASH(item);
    prev = NULL;
    tab = hashtab[ndx];
    alloc = 0;
    do {
        if (!tab) {
            tab = calloc(1, sizeof(TABHASH_TAB_T));
            if (!tab) {

                return -1;
            }
            alloc = 1;
            tab->nmax = 20;
        }
        if (tab->ncur < tab->nmax) {
            ndx = tab->ncur;
            tabhashcpy(item, &tab[ndx]);
            ndx++;
            tab->ncur = ndx;
            if (alloc) {
                if (prev) {
                    prev->next = tab;
                    tab->prev = prev;
                } else {
                    hashtab[ndx] = tab;
                }
            }

            loop = 0;
        } else {
            prev = tab;
            tab = tab->next;
        }
    } while (loop);
    if (!tab) {

        return -1;
    }

    return 0;
}

static __inline__ TABHASH_ITEM_T *
tabhashget(TABHASH_TAB_T **hashtab, const uintptr_t key, long remove)
{
    TABHASH_TAB_T      *tab;
    TABHASH_TAB_T      *prev;
    TABHASH_ITEM_T     *item;
    long                cur;
    long                n;
    long                lim;

    tab = hashtab[key];
    prev = NULL;
    while (tab) {
        lim = tab->nmax;
        for (cur = 0 ; cur < lim ; cur++) {
            item = &tab->items[cur];
            if (!TABHASH_CMP(item, key)) {
                if (remove) {
                    /* remove item from hash */
                    n = tab->ncur;
                    n--;
                    if (cur != n) {
                        /* copy last item in table over removed one */
                        tabhashcpy(&tab->items[n], &tab->items[cur]);
                    }
#if defined(TABHASH_FREE)
                    TABHASH_FREE(tab->items[n]);
#endif
                    tabhashclr(&tab->items[n]);
                    tab->ncur = n;
                    if (!n) {
                        /* free empty table */
                        if (prev) {
                            prev->next = tab->next;
                        } else {
                            hashtab[key] = tab->next;
                        }
                        free(tab);
                    }
                } else if (prev) {
                    /* insert result table into front of queue */
                    prev->next = tab->next;
                    tab->next = hashtab[key];
                    hashtab[key] = tab;
                }

                return item;
            }
        }
        prev = tab;
        tab = tab->next;
    }

    return NULL;
}

#endif /* __VND_TABHASH_H__ */

