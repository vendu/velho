#ifndef __ZEN_TASK_H__
#define __ZEN_TASK_H__

#include <zen/conf.h>
#include <zen/types.h>

/* system tasks */
#define ZEN_KERNEL_TASK         0       // kernel
#define ZEN_INIT_TASK           1       // init
#define ZEN_PAGE_TASK           2       // page-daemon
#define ZEN_BUF_TASK            3       // buf-daemon
#define ZEN_SYS_TASKS           4       // # of predefined system tasks
#define ZEN_USR_PID_MIN         ZEN_SYS_TASKS
#define ZEN_USR_PID_MAX         (ZEN_TASK_MAX - 1)

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
    struct zentask     *tasktab[ZEN_PROC_TASK_MAX];
};

struct zentask {
    union m_task        tcb;    // task control block
    zenpid_t            pgrp;   // parent/group process ID
    m_word_t            state;  // task state
    m_word_t            flags;  // task flags
    union taoparm       sched;  // task scheduler parameters
};

#endif /* __ZEN_TASK_H__ */

