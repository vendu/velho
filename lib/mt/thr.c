#include <time.h>
#include <sys/time.h>
#include <zero/cdefs.h>
#include <mach/asm.h>
#include <zero/time.h>
#include <zero/thr.h>
#include <zero/cond.h>
#include <zero/prof.h>
#define HTLIST_TYPE         zerothr
#define HTLIST_QTYPE        zerothrqueue
#define HTLIST_RM_COND(thr) ((thr)->sleep)
#define HTLIST_QUEUE(thr)   ((thr)->sleep = ZEROTHR_ASLEEP)
#define HTLIST_DEQUEUE(thr) ((thr)->sleep = ZEROTHR_AWAKE)
#include <zero/htlist.h>

static zerothrqueue thrsleepqueue;
THREADLOCAL zerothr thrself;

void
thrwait1(zerothrqueue *queue)
{
    zerothr *thr = &thrself;

    if (!queue) {
        queue = &thrsleepqueue;
    }
    htlistpush(queue, thr);
    while (m_atomread(&thr->sleep)) {
        thryield();
    }

    return;
}

long
thrsleep2(zerothrqueue *queue, const struct timespec *absts)
{
    zerothr        *thr = &thrself;
#if defined(USECLOCKNANOSLEEP)
    struct timespec tsrem;
#else
    struct timeval  tvout;
    struct timeval  tvcur;
#endif

    if (!queue) {
        queue = &thrsleepqueue;
    }
    htlistpush(queue, thr);
#if defined(USECLOCKNANOSLEEP)
    while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, absts, &tsrem)) {
        if (errno == EINTR) {

            continue;
        } else {
            if (m_atomread(&thr->sleep)) {
                htlistrm(queue, thr);
            }

            return -1;
        }
    }
    if (m_atomread(&thr->sleep)) {
        htlistrm(queue, thr);
    }
#else
    gettimeofday(&tvcur, NULL);
    TIMESPEC_TO_TIMEVAL(&tvout, absts);
    do {
        if (timevalcmp(&tvout, &tvcur) > 0) {
            if (m_atomread(&thr->sleep)) {
                htlistrm(queue, thr);
            }

            return 0;
        }
        thryield();
        gettimeofday(&tvcur, NULL);
    } while (1);
#endif

    return 0;
}

zerothr *
thrwake1(zerothrqueue *queue)
{
    zerothr *thr = NULL;

    if (!queue) {
        queue = &thrsleepqueue;
    }
    htlistdequeue(queue, &thr);

    return thr;
}

void
thrwakeall(zerothrqueue *queue)
{
    zerothr *thr = NULL;

    if (!queue) {
        queue = &thrsleepqueue;
    }
    do {
        htlistdequeue(queue, &thr);
    } while (thr);

    return;
}

