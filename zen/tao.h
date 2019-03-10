#ifndef __ZEN_TAO_H__
#define __ZEN_TAO_H__

#include <mach/param.h>
#include <mt/mtx.h>

/*
 * NOTES
 * -----
 * - realtime priorities are negative
 */
#define TAO_PRIO_MIN      (-32)
#define TAO_SCHED_PRIOS   32
#define TAO_SCHED_SYS     0
#define TAO_SCHED_USER    1
#define TAO_SCHED_BATCH   2
#define TAO_SCHED_IDLE    3
#define TAO_SCHED_CLASSES 4

struct taothr {
    zenmtx  mtx;
    void   *tcb;
    long    sched;
    long    prio;
    long    slice;
    long    ntick;
};

struct taoschedq {
    mtmtx               mtx;
    struct taothr      *head;
    struct taothr      *tail;
    uint8_t            _pad[CLSIZE - 3 * WORDSIZE];
};

static __inline__ void
taoinitqueue(struct taoschedq *queue)
{
    queue->head = &queue->dummy;
    queue->tail = &queue->dummy;

    return;
}

static __inline__ struct taothr *
taodequeuethr(long sched)
{
    struct taoschedq *queue = &taoschedq[sched][0];
    long              prio = 0;
    long              lk;
    struct taothr    *thr;
    struct taothr    *head;
    uintptr_t         hptr;
    uintptr_t         tptr;

    while (prio < TAO_SCHED_PRIOS) {
        taolkbit(&queue->head);
        hptr = queue->head;
        hptr &= ~TAO_QUEUE_LK_BIT;
        head = NULL;
        thr = (void *)hptr;
        lk = 0;
        if (thr) {
            head = thr->next;
            if (thr == queue->tail) {
                /* single-item queue, lock tail */
                taolkbit(&queue->tail);
                tptr = queue->tail;
                if (tptr == hptr) {
                    /* still single-item queue */
                    lk = 1;
                } else {
                    /* queue changed, head != tail */
                    taounlkbit(&queue->tail);
                }
            }
        } else {
            /* empty queue */
            taounlkbit(&queue->head);
            prio++;
            queue++;

            continue;
        }
        m_atomwrite(&queue->head, head);
        if (lk) {
            /* removing only item from queue */
            m_atomwrite(&queue->tail, NULL);
        }

        return thr;
    }

    return NULL;
}

static __inline__ void
taoenqueuethr(struct taothr *thr, long sched, long prio)
{
    struct taoschedq *queue = &taoschedq[sched][prio];
    uintptr_t         uptr;

    thr->next = NULL;
    taolkbit(&queue->tail);
    uptr = queue->tail;
    uptr &= ~TAO_QUEUE_LK_BIT;
    queue->prev = (void *)uptr;
    m_atomwrite(&queue->tail, thr);

    return;
}

#endif /* __ZEN_TAO_H__ */

