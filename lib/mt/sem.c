#include <errno.h>
#if defined(PTHREAD) && !defined(ZEROPTHREAD)
#include <pthread.h>
#else
#include <zero/mtx.h>
#endif
#include <mach/asm.h>
#include <zero/sem.h>
//#include <zero/thr.h>

long
semwait(zerosem *sem)
{
    do {
#if defined(PTHREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            thryield();
        }
#elif defined(ZEROFMTX)
        while (!fmtxtrylk(&sem->lk)) {
            thryield();
        }
#endif
        if (sem->val > 0) {
            sem->val--;
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(ZEROFMTX)
            fmtxunlk(&sem->lk);
#endif

            return 0;
        } else {
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(ZEROFMTX)
            fmtxunlk(&sem->lk);
#endif
            thryield();
        }
    } while (1);

    return 0;
}

long
semtrywait(zerosem *sem)
{
    do {
#if defined(PTHREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            thryield();
        }
#elif defined(ZEROfMTX)
        while (!fmtxtrylk(&sem->lk)) {
            thryield();
        }
#endif
        if (sem->val > 0) {
            sem->val--;
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(ZEROFMTX)
            fmtxunlk(&sem->lk);
#endif

            return 0;
        } else {
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(ZEROFMTX)
            fmtxunlk(&sem->lk);
#endif
            errno = EAGAIN;

            return -1;
        }
    } while (1);

    return 0;
}

long
sempost(zerosem *sem)
{
    do {
#if defined(PTHREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            thryield();
        }
#elif defined(ZEROFMTX)
        while (fmtxtrylk(&sem->lk)) {
            thryield();
        }
#endif
        if (!sem->val) {
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(ZEROFMTX)
            fmtxunlk(&sem->lk);
#endif
            thryield();
        } else if (sem->val != ZEROSEM_MAXVAL) {
            sem->val++;
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(ZEROFMTX)
            fmtxunlk(&sem->lk);
#endif

            return 0;
        } else {
#if defined(EOVERFLOW)
            errno = EOVERFLOW;
#endif

            return -1;
        }
    } while (1);

    return 0;
}

