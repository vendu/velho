#ifndef __MT_THR_H__
#define __MT_THR_H__

#include <mt/conf.h>
#include <stddef.h>
#include <stdint.h>
#include <mt/mtx.h>

#if defined(PTHREAD) && !defined(__KERNEL__)
#define thrid() ((uintptr_t)pthread_self())
#endif

#if defined(ZERO_THREAD) || (POSIX_THREAD)
#define thryield() pthread_yield()
#elif defined(_WIN64) || defined(_WIN32)
#define thryield() kYieldProcessor()
#elif defined(__linux__) && !defined(__KERNEL__)
#define thryield() sched_yield()
#elif defined(__KERNEL__)
#define thryield() schedyield()
//#elif defined(ZEROTHR)
//#define thryield() /* FIXME */
#endif

#if defined(ZERO_THREAD)

#if defined(ZERO_SCHED)
#include <sched.h>
#endif

typedef uintptr_t zerothrid;

#define ZEROTHRATR_INIT          (1 << 0)       // attributes initialised
#define ZEROTHRATR_DETACHED      (1 << 1)       // detach thread
#define ZEROTHRATR_INHERITSCHED  (1 << 2)       // inherit scheduler parameters
#define ZEROTHRATR_EXPLICITSCHED (1 << 3)       // specify scheduler parameters
#define ZEROTHRATR_SCHED_PARAM   (1 << 4)       // scheduler parameters
#define ZEROTHRATR_SCHEDPOLICY   (1 << 5)       // scheduler policy
#define ZEROTHRATR_SCOPE         (1 << 6)       // scheduling scope
#define ZEROTHRATR_STKATR        (1 << 7)       // stack address and size
#define ZEROTHRATR_GUARDSIZE     (1 << 8)       // stack guard size
#define ZEROTHRATR_AFFINITY      (1 << 9)       // affinity configuration
typedef struct __zerothratr {
    long                flg;
    void               *stkadr;
    size_t              stksize;
    size_t              guardsize;
#if defined(_GNU_SOURCE)
    size_t              ncpu;
    void               *cpuset;
#endif
#if defined(ZERO_SCHED)
    struct sched_param  schedparm;
#endif
    uint8_t             _res[4 * CLSIZE
#if defined(ZERO_SCHED)
                             - sizeof(struct sched_param)
#endif
#if defined(_GNU_SOURCE)
                             - sizeof(size_t) - sizeof(void *)
#endif
                             - 2 * sizeof(size_t) - sizeof(void *)
                             - sizeof(long)];
} zerothratr;

#define ZEROTHR_NOID   (~(zerothrid)0)
#define ZEROTHR_ASLEEP 1
#define ZEROTHR_AWAKE  0
typedef struct __zerothr {
    zerothrid         id;
    long              sleep;
    zerothratr       *atr;
    struct __zerothr *prev;
    struct __zerothr *next;
} zerothr;

#define ZEROTHRQUEUE_INITIALIZER { MTXINITVAL, NULL, NULL }
typedef struct {
    zerofmtx  lk;
    zerothr  *head;
    zerothr  *tail;
} zerothrqueue;

extern void      thrwait1(zerothrqueue *queue);
extern long      thrsleep2(zerothrqueue *queue, const struct timespec *absts);
extern zerothr * thrwake(zerothrqueue *queue);
extern void thrwakeall(zerothrqueue *queue);

#define thrwait()    thrwait1(NULL)
#define thrwake()    thrwake1(NULL)
#define thrwakeall() thrwakeall1(NULL)

#endif /* defined(ZERO_THREAD) */

#endif /* __MT_THR_H__ */

