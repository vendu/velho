#ifndef __MT_COND_H__
#define __MT_COND_H__

#if defined(PTHREAD)
#include <stddef.h>
#include <time.h>
#include <pthread.h>
#endif
#include <mt/conf.h>
#include <mt/mtx.h>
#include <mt/thr.h>

#if 0 && defined(PTHREAD) && !defined(MTCOND)

typedef pthread_cond_t mtcond;

#define condinit(cp)               pthread_cond_init(cp, NULL)
#define condwait(cp, mp)           pthread_cond_wait(cp, mp)
#define condwaittimed(cp, mp, tsp) pthread_cond_timedwait(cp, mp, tsp)
#define condsigone(cp)             pthread_cond_signal(cp)
#define condsigall(cp)             pthread_cond_broadcast(cp)
#define condfree(cp)               pthread_cond_destroy(cp)

#elif defined(MTCOND)

#define MTCOND_INITIALIZER { MTTHRQUEUE_INITIALIZER }
typedef struct __mtcond {
    mtthrqueue queue;
} mtcond;

#endif

#define condsignal(cp) condsigmany(cp, 8)
void condinit(mtcond *cond);
long condsigone(mtcond *cond);
long condsigmany(mtcond *cond, long nthr);
long condsigall(mtcond *cond);;
#if (MTFMTX)
long condwait(mtcond *cond, mtfmtx *mtx);
long condwaitabstime(mtcond *cond, mtfmtx *mtx,
                     const struct timespec *absts);
#elif (MTMTX)
long condwait(mtcond *cond, mtmtx *mtx);
long condwaitabstime(mtcond *cond, mtmtx *mtx,
                     const struct timespec *absts);
#endif

#endif /* __MT_COND_H__ */

