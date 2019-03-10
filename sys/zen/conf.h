#ifndef __ZEN_CONF_H__
#define __ZEN_CONF_H__

#if defined(__v0__)
#define ZEN_TASK_MAX            64
#define ZEN_THR_MAX             64
#endif

/* system parameters */
#define ZEN_USR_STK_SIZE        (2 * SYS_PAGE_SIZE)
#define ZEN_SYS_STK_SIZE        (SYS_PAGE_SIZE)

#endif /* __ZEN_CONF_H__ */

