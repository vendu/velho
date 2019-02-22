#ifndef __MT_COND_H__
#define __MT_COND_H__

#include <mt/conf.h>
#include <mt/mtx.h>
#include <mt/thr.h>
#if defined(PTHREAD)
#include <stddef.h>
#include <time.h>
#include <pthread.h>
#endif

#if 0 && defined(PTHREAD) && !defined(ZEROCOND)

typedef pthread_cond_t zerocond;

#define condinit(cp)               pthread_cond_init(cp, NULL)
#define condwait(cp, mp)           pthread_cond_wait(cp, mp)
#define condwaittimed(cp, mp, tsp) pthread_cond_timedwait(cp, mp, tsp)
#define condsigone(cp)             pthread_cond_signal(cp)
#define condsigall(cp)             pthread_cond_broadcast(cp)
#define condfree(cp)               pthread_cond_destroy(cp)

#elif defined(ZEROCOND)

#define ZEROCOND_INITIALIZER { ZEROTHRQUEUE_INITIALIZER }
typedef struct __zerocond {
    zerothrqueue queue;
} zerocond;

#endif

#define condsignal(cp) condsigmany(cp, 8)
void condinit(zerocond *cond);
long condsigone(zerocond *cond);
long condsigmany(zerocond *cond, long nthr);
long condsigall(zerocond *cond);;
#if (ZEROFMTX)
long condwait(zerocond *cond, zerofmtx *mtx);
long condwaitabstime(zerocond *cond, zerofmtx *mtx,
                     const struct timespec *absts);
#elif (ZEROMTX)
long condwait(zerocond *cond, zeromtx *mtx);
long condwaitabstime(zerocond *cond, zeromtx *mtx,
                     const struct timespec *absts);
#endif

#endif /* __MT_COND_H__ */

