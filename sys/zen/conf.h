#ifndef __ZEN_CONF_H__
#define __ZEN_CONF_H__

#define ZEN_DEF_TASK_SCHED      ZEN_ULE_TASK_SCHED
#define ZEN_ULE_TASK_SCHED      1
#define ZEN_BVT_TASK_SCHED      2
#define ZEN_IP4_NET             1

#if defined(__v0__)
#define ZEN_PROC_MAX            ZEN_TASK_MAX
#define ZEN_PROC_TASKS          8
#define ZEN_TASK_MAX            64
#endif

/* system parameters */
#define ZEN_USR_STK_SIZE        (2 * SYS_PAGE_SIZE)
#define ZEN_SYS_STK_SIZE        (SYS_PAGE_SIZE)

#endif /* __ZEN_CONF_H__ */

