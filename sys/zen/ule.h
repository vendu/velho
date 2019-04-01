#Ifndef __ZEN_ULE_H__
#define __ZEN_ULE_H__

#include <mach/param.h>
#include <mt/mtx.h>

/* thread priority parameters */
#define TAO_SCHED_PRIO_MAX      128
#define TAO_SCHED_PRIOS         32
#define TAO_SCHED_FAST          0
#define TAO_SCHED_SYS           1
#define TAO_SCHED_USER          2
#define TAO_SCHED_BATCH         3
#define TAO_SCHED_CLASSES       4

/* thread priority parameters */
#define TAO_SCHED_PRIO_MAX      128
#define TAO_SCHED_PRIOS         32
#define TAO_SCHED_FAST          0
#define TAO_SCHED_SYS           1
#define TAO_SCHED_USER          2
#define TAO_SCHED_BATCH         3
#define TAO_SCHED_CLASSES       4

/* thread states */
#define TAO_THR_NEW             0
#define TAO_THR_RUN             1
#define TAO_THR_SLEEP           2
#define TAO_THR_STOP            3
#define TAO_THR_ZOMBIE          4

#define taoprioqid(prio)        ((prio) >> 2)
struct taouleparm {
    uintptr_t   wtchan;
    m_word_t    type;
    m_word_t    prio;
    m_word_t    slice;
    m_word_t    ticks;
};

struct taoproc {
    struct taouleparm   sched;
};

struct taothr {
    void               *tcb;
    struct taoparm      sched;
};

struct taoqueue {
    mtmtx               mtx;
    struct taothr      *head;
    struct taothr      *tail;
    uint8_t            _pad[CLSIZE - sizeof(mtmtx) - 2 * sizeof(void *)];
};

struct taouleparm {
    m_word_t    type;
    m_word_t    prio;
    m_word_t    slice;
    m_word_t    ticks;
};

struct taoproc {
    struct taoparm      sched;
};

struct taothr {
    void               *tcb;
    struct taoparm      sched;
};

struct taoqueue {
    mtmtx               mtx;
    struct taothr      *head;
    struct taothr      *tail;
    uint8_t            _pad[CLSIZE - sizeof(mtmtx) - 2 * sizeof(void *)];
};

#if 0
static __inline__ void
taoinitqueue(struct taoqueue *queue)
{
    queue->head = NULL;
    queue->tail = NULL;

    return;
}
#endif

static __inline__ struct taothr *
taopickthr(m_word_t type)
{
    struct taoqueue    *queue = &taoprioq[type][0];
    m_word_t            prio = 0;
    m_word_t            one;
    struct taothr      *thr;
    struct taothr      *head;
    uintptr_t           hptr;
    uintptr_t           tptr;

    while (prio < TAO_SCHED_PRIOS) {
        mtlkbit(&queue->head);
        hptr = queue->head;
        thr = (void *)(hptr & ~TAO_QUEUE_LK_BIT);
        one = 0;
        tptr = queue->tail;
        if ((thr) && hptr == tptr) {
            /* single-item queue, lock tail */
            mtlkbit(&queue->tail);
            tptr = queue->tail;
            head = thr->next;
            if (tptr == hptr) {
                /* still single-item queue */
                one = 1;
            } else {
                /* queue changed, head != tail */
                mtunlkbit(&queue->tail);
            }
        } else if (thr) {
            head = thr->next;
        } else {
            /* empty queue */
            mtunlkbit(&queue->head);
            prio++;
            queue++;

            continue;
        }
        m_atomwrite(&queue->head, head);
        if (one) {
            /* removing only item from queue */
            m_atomwrite(&queue->tail, NULL);
        }

        return thr;
    }

    return NULL;
}

static __inline__ void
taopushthr(struct taothr *thr, m_word_t type, m_word_t prio)
{
    struct taoqueue    *queue = &taoprioq[type][0];
    m_word_t            one;
    struct taothr      *thr;
    struct taothr      *head;
    uintptr_t           hptr;
    uintptr_t           tptr;

    thr->prev = NULL;
    mtlkbit(&queue->head);
    hptr = queue->head;
    head = (void *)(hptr & ~TAO_QUEUE_LK_BIT);
    thr->next = head;
    one = 0;
    if (head) {
        /* single-item queue, lock tail */
        mtlkbit(&queue->tail);
        tptr = queue->tail;
        if (tptr == hptr) {
            /* still single-item queue */
            one = 1;
        } else {
            /* queue changed, head != tail */
            mtunlkbit(&queue->tail);
        }
    } else {
        mtunlkbit(&queue->head);
        prio++;
        queue++;

        continue;
    }
    if (one) {
            /* removing only item from queue */
        m_atomwrite(&queue->tail, thr);
    }
    m_atomwrite(&queue->head, thr);

    return;
}

static __inline__ void
taoqueuethr(struct taothr *thr, m_word_t type, m_word_t prio)
{
    struct taoqueue    *queue = &taoprioq[type][prio];
    m_word_t            one;
    struct taoqueue    *head;
    struct taoqueue    *tail;
    uintptr_t           hptr;
    uintptr_t           tptr;

    thr->next = NULL;
    mtlkbit(&queue->tail);
    tptr = (uintptr_t)queue->tail;
    one = 0;
    tail = (void *)(tptr & ~TAO_QUEUE_LK_BIT);
    thr->prev = tail;
    if (tail) {
        tail->next = tail;
    }
    hptr = queue->head;
    if ((tail) && tptr == hptr) {
        mtlkbit(&queue->head);
        hptr = queue->head;
        head = (void *)(hptr & ~TAO_QUEUE_LK_BIT);
        if (hptr == tptr) {
            /* single-item queue */
            one = 1;
        } else {
            mtunlkbit(&queue->head);
        }
    }
    thr->prev = tail;
    thr->next = NULL;
    if (one) {
        m_atomwrite(&queue->head, thr);
    }
    m_atomwrite(&queue->tail, thr);

    return;
}

#endif /* __ZEN_ULE_H__ */

