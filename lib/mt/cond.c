#include <stddef.h>
#include <time.h>
#include <errno.h>
#if defined(PTHREAD)
#include <pthread.h>
#endif
#include <mt/mtx.h>
#include <zero/cond.h>
#include <zero/thr.h>

void
condinit(zerocond *cond)
{
#if (ZEROFMTX)
    fmtxinit(&cond->queue.lk);
#endif
    cond->queue.head = NULL;
    cond->queue.tail = NULL;
}

long
condsigone(zerocond *cond)
{
    if (!cond) {

        return -EINVAL;
    }
    thrwake1(&cond->queue);

    return 0;
}

long
condsigmany(zerocond *cond, long nthr)
{
    if (!cond || !nthr) {

        return -EINVAL;
    }
    while (nthr--) {
        if (!thrwake(&cond->queue)) {

            return 0;
        }
    }

    return 0;
}

long
condsigall(zerocond *cond)
{
    if (!cond) {

        return EINVAL;
    }
    thrwakeall(&cond->queue);

    return 0;
}

long
condwait(zerocond *cond, zerofmtx *fmtx)
{
    if (!cond || !fmtx) {

        return -EINVAL;
    } else if (!fmtxtrylk(fmtx)) {

        return -EPERM;
    } else {
        thrwait1(&cond->queue);
    }
    zerolkfmtx(fmtx);

    return 0;
}

long
condwaitabstime(zerocond *cond, zerofmtx *fmtx, const struct timespec *absts)
{
    if (!cond || !fmtx || !absts || absts->tv_sec < 0
        || absts->tv_nsec < 0 || absts->tv_nsec >= 1000000000) {

        return -EINVAL;
    } else if (!fmtxtrylk(fmtx)) {

        return -EPERM;
    } else if (thrsleep2(&cond->queue, absts) < 0) {

        return -EINVAL;
    }
    zerolkfmtx(fmtx);

    return 0;
}

