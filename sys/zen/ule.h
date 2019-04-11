#Ifndef __ZEN_ULE_H__
#define __ZEN_ULE_H__

#include <zen/conf.h>

#if defined(ZEN_ULE_THR_SCHED)

#include <zen/task.h>
#include <mach/param.h>
#include <mt/mtx.h>

/* thread priority parameters */
#define TAO_SCHED_PRIO_MAX      128
#define TAO_SCHED_PRIOS         32
#define TAO_SCHED_PRIO_SHIFT    2
#define TAO_SCHED_QUICK         0
#define TAO_SCHED_FAST          1
#define TAO_SCHED_USER          2
#define TAO_SCHED_BATCH         3
#define TAO_SCHED_CLASSES       4

#define taoprioqid(prio)        ((prio) >> TAO_SCHED_PRIO_SHIFT)
struct taouleparm {
    m_adr_t             wtchan;
    m_word_t            type;
    m_word_t            prio;
    m_word_t            slice;
    m_word_t            ticks;
};

struct taoqueue {
    mtmtx               mtx;
    struct zentask     *head;
    struct zentask     *tail;
    uint8_t            _pad[CLSIZE - sizeof(mtmtx) - 2 * sizeof(void *)];
};

struct taoqueue {
    mtmtx               mtx;
    struct zentask     *head;
    struct zentask     *tail;
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

static __inline__ struct zentask *
taopicktask(m_word_t type)
{
    struct taoqueue    *queue = &taoprioq[type][0];
    m_word_t            prio = 0;
    m_word_t            one;
    struct zentask     *task;
    struct zentask     *head;
    uintptr_t           hptr;
    uintptr_t           tptr;

    while (prio < TAO_SCHED_PRIOS) {
        mtlkbit(&queue->head);
        hptr = queue->head;
        task = (void *)(hptr & ~TAO_QUEUE_LK_BIT);
        one = 0;
        tptr = queue->tail;
        if ((task) && hptr == tptr) {
            /* single-item queue, lock tail */
            mtlkbit(&queue->tail);
            tptr = queue->tail;
            head = task->next;
            if (tptr == hptr) {
                /* still single-item queue */
                one = 1;
            } else {
                /* queue changed, head != tail */
                mtunlkbit(&queue->tail);
            }
        } else if (task) {
            head = task->next;
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

        return task;
    }

    return NULL;
}

static __inline__ void
taopushtask(struct zentask *task, m_word_t type, m_word_t prio)
{
    struct taoqueue    *queue = &taoprioq[type][0];
    m_word_t            one;
    struct zentask     *task;
    struct zentask     *head;
    uintptr_t           hptr;
    uintptr_t           tptr;

    task->prev = NULL;
    mtlkbit(&queue->head);
    hptr = queue->head;
    head = (void *)(hptr & ~TAO_QUEUE_LK_BIT);
    task->next = head;
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
        m_atomwrite(&queue->tail, task);
    }
    m_atomwrite(&queue->head, task);

    return;
}

static __inline__ void
taoqueuetask(struct zentask *task, m_word_t type, m_word_t prio)
{
    struct taoqueue    *queue = &taoprioq[type][prio];
    m_word_t            one;
    struct taoqueue    *head;
    struct taoqueue    *tail;
    uintptr_t           hptr;
    uintptr_t           tptr;

    task->next = NULL;
    mtlkbit(&queue->tail);
    tptr = (uintptr_t)queue->tail;
    one = 0;
    tail = (void *)(tptr & ~TAO_QUEUE_LK_BIT);
    task->prev = tail;
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
    task->prev = tail;
    task->next = NULL;
    if (one) {
        m_atomwrite(&queue->head, task);
    }
    m_atomwrite(&queue->tail, task);

    return;
}

#endif /* __ZEN_ULE_H__ */

