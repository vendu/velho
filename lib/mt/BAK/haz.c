#include <stddef.h>
#include <zero/cdefs.h>
#include <zero/haz.h>

THREADLOCAL struct haznode *t_hazprivtab[HAZ_TAB_CHAINS];
THREADLOCAL struct haznode  t_hazretqueue;
THREADLOCAL long            t_hazretcnt;
THREADLOCAL struct haznode  t_hazfreequeue;
struct haznode             *t_hazptrqueue;

static void hazscan(struct haznode *queue, struct haznode **ret);

static void
hazretire(struct haznode *node)
{
    long ndx = node->ndx;

    t_hazretqueue.tab[ndx] = node;
    ndx++;
    if (ndx >= HAZ_NODE_HIWATER) {
        hazscan(t_hazptrqueue, &t_hazretqueue);
    } else {
        node->ndx = ndx;
    }

    return;
}

static void
hazaddptr(void *ptr)
{
    long            key = hazhashptr(ptr);
    struct haznode *tab = t_hazprivtab[key];
    struct haznode *item;
    long            ndx;

    if (!tab) {
        tab = calloc(1, sizeof(struct haznode));
    }
    if (tab) {
        ndx = tab->ndx;
        if (ndx < HAZ_TAB_ITEMS) {
            tab->tab[ndx] = ptr;
            ndx++;
            tab->ndx = ndx;
        } else {
            item = calloc(1, sizeof(struct haznode));
            item->ndx = 1;
            item->tab[0] = ptr;
            item->next = tab;
            t_hazprivtab[key] = item;
        }
    }

    return;
}

static void *
hazfindptr(void *ptr)
{
    long            key = hazhashptr(ptr);
    struct haznode *item = t_hazprivtab[key];
    long            ndx;

    while (item) {
        for (ndx = item->ndx ; ndx > 0 ; ndx--) {
            if (item->tab[ndx] == ptr) {

                return ptr;
            }
        }
        item = item->next;
    }

    return NULL;
}

static void
hazqueueptr(struct haznode **list, void *ptr)
{
    struct haznode *queue = *list;
    long            ndx = queue->ndx;
    struct haznode *item;

    if (ndx < HAZ_NODE_TAB_ITEMS) {
        queue->tab[ndx] = ptr;
        ndx++;
        queue->ndx = ndx;
    } else {
        item = calloc(1, sizeof(struct haznode));
        item->ndx = 1;
        item->tab[0] = ptr;
        item->next = list;
        *list = item;
    }

    return;
}

static void
hazrelptr(struct haznode *list, void *ptr)
{
    long  ndx = list->ndx;

    if (ndx < HAZ_NODE_TAB_ITEMS) {
        list->tab[ndx] = ptr;
        ndx++;
        list->ndx = ndx;
    } else {
        while (ndx-- > 0) {
            ptr = list->tab[ndx];
            free(ptr);
            list->tab[ndx] = NULL;
        }
        list->tab[ndx] = 0;
    }

    return;
}

static void
hazscan(struct haznode *queue, struct haznode **ret)
{
    struct haznode *item = queue;
    struct haznode *list = NULL;
    struct haznode *node;
    void           *ptr;
    long            ndx;

    while (item) {
        for (ndx = item->ndx ; ndx >= 0 ; ndx--) {
            ptr = item->tab[ndx];
            if (ptr) {
                hazaddptr(ptr);
            }
        }
        item = item->next;
    }
    node = *ret;
    *ret = NULL;
    t_hazretcnt = 0;
    list = calloc(1, sizeof(struct haznode));
    while (node) {
        for (ndx = item->ndx ; ndx >= 0 ; ndx--) {
            ptr = node->tab[ndx];
            if (hazfindptr(ptr)) {
                t_hazretcnt++;
                hazqueueptr(&list, ptr);
            } else {
                hazrelptr(&t_hazfreequeue, ptr);
            }
        }
        node = node->next;
    }
    *ret = list;

    return;
}

