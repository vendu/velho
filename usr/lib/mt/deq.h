#ifndef __MT_DEQ_H__
#define __MT_DEQ_H__

#endif /* __MT_DEQ_H__ */

#if !defined(DEQLISTPREV)
#define DEQLISTPREV     prev
#endif
#if !defined(DEQLISTNEXT)
#define DEQLISTNEXT     next
#endif
#if !defined(DEQLIST_QUEUE)
#define DEQLIST_QUEUE(dummy)
#endif
#if !defined(DEQLIST_DEQUEUE)
#define DEQLIST_DEQUEUE(dummy)
#endif
#if !defined(DEQLIST_RM_COND)
#define DEQLIST_RM_COND(dummy) 1
#endif

/*
 * assumptions
 * -----------
 * - DEQLIST_TYPE (struct/union) has members prev and next of DEQLIST_TYPE *
 * - DEQLIST_QTYPE has members head and tail of DEQLIST_TYPE *
 * - if _REENTRANT is defined DEQLIST_QTYPE has a volatile lk-member
 */

/* #define DEQLIST_TYPE  */
/* #define DEQLIST_QTYPE */

#define deqlistnotempty(queue) (queue->head)

/* get item from the head of queue */
#define deqlistpop(queue, retpp)                                        \
    do {                                                                \
        DEQLIST_TYPE *_item1;                                           \
        DEQLIST_TYPE *_item2 = NULL;                                    \
                                                                        \
        _item1 = (queue)->head;                                         \
        if (_item1) {                                                   \
            _item2 = _item1->DEQLISTNEXT;                               \
            if (_item2) {                                               \
                _item2->DEQLISTPREV = NULL;                             \
            } else {                                                    \
                (queue)->tail = NULL;                                   \
            }                                                           \
            (queue)->head = _item2;                                     \
            DEQLIST_DEQUEUE(_item1);                                    \
        }                                                               \
        *(retpp) = _item1;                                              \
    } while (0)

/* queue item to the head of queue */
#define deqlistpush(queue, item)                                        \
    do {                                                                \
        DEQLIST_TYPE *_item;                                            \
                                                                        \
        (item)->DEQLISTPREV = NULL;                                     \
        _item = (queue)->head;                                          \
        if (_item) {                                                    \
            (_item)->DEQLISTPREV = (item);                              \
        } else {                                                        \
            (queue)->tail = (item);                                     \
        }                                                               \
        (item)->DEQLISTNEXT = _item;                                    \
        (queue)->head = item;                                           \
        DEQLIST_QUEUE(item);                                            \
    } while (0)

/* get item from the tail of queue */
#define deqlistdequeue(queue, retpp)                                    \
    do {                                                                \
        DEQLIST_TYPE *_item1;                                           \
        DEQLIST_TYPE *_item2;                                           \
                                                                        \
        _item1 = (queue)->tail;                                         \
        if (_item1) {                                                   \
            _item2 = _item1->DEQLISTPREV;                               \
            if (!_item2) {                                              \
                (queue)->head = NULL;                                   \
                (queue)->tail = NULL;                                   \
            } else {                                                    \
                _item2->DEQLISTNEXT = NULL;                             \
                (queue)->tail = _item2;                                 \
            }                                                           \
            DEQLIST_DEQUEUE(_item1);                                    \
        }                                                               \
        *(retpp) = _item1;                                              \
    } while (0)

/* add block to the tail of queue */
#define deqlistqueue(queue, item)                                       \
    do {                                                                \
        deqlist_TYPE *_tail;                                            \
                                                                        \
        (item)->DEQLISTNEXT = NULL;                                     \
        _tail  = (queue)->tail;                                         \
        (item)->DEQLISTPREV = _tail;                                    \
        if (_tail) {                                                    \
            (queue)->tail = (item);                                     \
        } else {                                                        \
            (item)->DEQLISTPREV = NULL;                                 \
            (queue)->head = (item);                                     \
        }                                                               \
        DEQLIST_QUEUE(item);                                            \
    } while (0)

/* remove item from queue */
#define deqlistrm(queue, item)                                          \
    do {                                                                \
        DEQLIST_TYPE *_tmp;                                             \
                                                                        \
        if (DEQLIST_RM_COND(item)) {                                    \
            _tmp = (item)->DEQLISTPREV;                                 \
            if (_tmp) {                                                 \
                _tmp->DEQLISTNEXT = (item)->DEQLISTNEXT;                \
            } else {                                                    \
                _tmp = (item)->DEQLISTNEXT;                             \
                if (_tmp) {                                             \
                    _tmp->DEQLISTPREV = (item)->DEQLISTPREV;            \
                } else {                                                \
                    (queue)->tail = NULL;                               \
                }                                                       \
                (queue)->head = _tmp;                                   \
            }                                                           \
            _tmp = (item)->DEQLISTNEXT;                                 \
            if (_tmp) {                                                 \
                _tmp->prev = (item)->DEQLISTPREV;                       \
            } else {                                                    \
                _tmp = (item)->DEQLISTPREV;                             \
                if (_tmp) {                                             \
                    _tmp->DEQLISTNEXT = NULL;                           \
                } else {                                                \
                    (queue)->head = NULL;                               \
                }                                                       \
                (queue)->tail = _tmp;                                   \
            }                                                           \
            DEQLIST_DEQUEUE(item);                                      \
        }                                                               \
    } while (0)

