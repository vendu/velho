#ifndef __ZEN_TASK_H__
#define __ZEN_TASK_H__

#include <zen/conf.h>
#include <zen/types.h>

/* task states */
#define ZEN_TASK_NEW            0
#define ZEN_TASK_READY          1
#define ZEN_TASK_RUN            2
#define ZEN_TASK_WAIT           3
#define ZEN_TASK_SLEEP          4
#define ZEN_TASK_ZOMBIE         5
#define ZEN_TASK_EXIT           6
#define ZEN_TASK_IDLE           7

struct zenproc {
    union m_task        tcb;
    m_word_t            flags;
    union taoschedparm  sched;
    m_uword_t           ntask;
    struct zentask     *tasktab;
};

struct zentask {
    union m_task        tcb;    // task control block
    m_word_t            state;  // task state
    m_word_t            flags;  // task flags
    union taoparm       sched;  // task scheduler parameters
};

#endif /* __ZEN_TASK_H__ */

