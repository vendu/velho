#include <time.h>
#include <sys/time.h>
#include <mach/asm.h>
#include <mt/thr.h>
#include <mt/cond.h>
#include <zero/cdefs.h>
#include <zero/time.h>
#include <zero/prof.h>

#define DEQLIST_TYPE            mtthr
#define DEQLIST_QTYPE           mtthrqueue
#define DEQLIST_RM_COND(thr)    ((thr)->sleep)
#define DEQLIST_QUEUE(thr)      ((thr)->sleep = MT_THR_ASLEEP)
#define DEQLIST_DEQUEUE(thr)    ((thr)->sleep = MT_THR_AWAKE)
#include <mt/deq.h>

static mtthrqueue               thrsleepqueue;
THREADLOCAL mtthr               thrself;

void
thrwait1(mtthrqueue *queue)
{
    mtthr      *thr = &thrself;

    if (!queue) {
        queue = &thrsleepqueue;
    }
    deqlistpush(queue, thr);
    while (m_atomread(&thr->sleep)) {
        thryield();
    }

    return;
}

long
thrsleep2(mtthrqueue *queue, const struct timespec *absts)
{
    mtthr              *thr = &thrself;
#if defined(USECLOCKNANOSLEEP)
    struct timespec     tsrem;
#else
    struct timeval      tvout;
    struct timeval      tvcur;
#endif

    if (!queue) {
        queue = &thrsleepqueue;
    }
    deqlistpush(queue, thr);
#if defined(USECLOCKNANOSLEEP)
    while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, absts, &tsrem)) {
        if (errno == EINTR) {

            continue;
        } else {
            if (m_atomread(&thr->sleep)) {
                deqlistrm(queue, thr);
            }

            return -1;
        }
    }
    if (m_atomread(&thr->sleep)) {
        deqlistrm(queue, thr);
    }
#else
    gettimeofday(&tvcur, NULL);
    TIMESPEC_TO_TIMEVAL(&tvout, absts);
    do {
        if (timevalcmp(&tvout, &tvcur) > 0) {
            if (m_atomread(&thr->sleep)) {
                deqlistrm(queue, thr);
            }

            return 0;
        }
        thryield();
        gettimeofday(&tvcur, NULL);
    } while (1);
#endif

    return 0;
}

mtthr *
thrwake1(mtthrqueue *queue)
{
    mtthr      *thr = NULL;

    if (!queue) {
        queue = &thrsleepqueue;
    }
    deqlistdequeue(queue, &thr);

    return thr;
}

void
thrwakeall(mtthrqueue *queue)
{
    mtthr      *thr = NULL;

    if (!queue) {
        queue = &thrsleepqueue;
    }
    do {
        deqlistdequeue(queue, &thr);
    } while (thr);

    return;
}

