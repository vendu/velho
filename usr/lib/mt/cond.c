#include <stddef.h>
#include <time.h>
#include <errno.h>
#if defined(PTHREAD)
#include <pthread.h>
#endif
#include <mt/mtx.h>
#include <mt/cond.h>
#include <mt/thr.h>

void
condinit(mtcond *cond)
{
#if (MTFMTX)
    mtinitfmtx(&cond->queue.mtx);
#endif
    cond->queue.head = NULL;
    cond->queue.tail = NULL;

    return;
}

long
condsigone(mtcond *cond)
{
    if (!cond) {

        return -EINVAL;
    }
    thrwake1(&cond->queue);

    return 0;
}

long
condsigmany(mtcond *cond, long nthr)
{
    if (!cond || !nthr) {

        return -EINVAL;
    }
    while (nthr--) {
        if (!thrwake1(&cond->queue)) {

            return 0;
        }
    }

    return 0;
}

long
condsigall(mtcond *cond)
{
    if (!cond) {

        return EINVAL;
    }
    thrwakeall(&cond->queue);

    return 0;
}

long
condwait(mtcond *cond, mtfmtx *fmtx)
{
    if (!cond || !fmtx) {

        return -EINVAL;
    } else if (!mttryfmtx(fmtx)) {

        return -EPERM;
    } else {
        thrwait1(&cond->queue);
    }
    mtlkfmtx(fmtx);

    return 0;
}

long
condwaitabstime(mtcond *cond, mtfmtx *fmtx, const struct timespec *absts)
{
    if (!cond || !fmtx || !absts || absts->tv_sec < 0
        || absts->tv_nsec < 0 || absts->tv_nsec >= 1000000000) {

        return -EINVAL;
    } else if (!mttryfmtx(fmtx)) {

        return -EPERM;
    } else if (thrsleep2(&cond->queue, absts) < 0) {

        return -EINVAL;
    }
    mtlkfmtx(fmtx);

    return 0;
}

