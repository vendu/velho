#include <errno.h>
#include <mach/asm.h>
#if defined(PTHREAD) && !defined(MTPTHREAD)
#include <pthread.h>
#else
#include <mt/mtx.h>
#endif
#include <mt/sem.h>
//#include <mt/thr.h>

long
semwait(mtsem *sem)
{
    do {
#if defined(PTHREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            thryield();
        }
#elif defined(MTFMTX)
        while (!fmtxtrylk(&sem->lk)) {
            thryield();
        }
#endif
        if (sem->val > 0) {
            sem->val--;
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            fmtxunlk(&sem->lk);
#endif

            return 0;
        } else {
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            fmtxunlk(&sem->lk);
#endif
            thryield();
        }
    } while (1);

    return 0;
}

long
semtrywait(mtsem *sem)
{
    do {
#if defined(PTHREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            thryield();
        }
#elif defined(MTfMTX)
        while (!fmtxtrylk(&sem->lk)) {
            thryield();
        }
#endif
        if (sem->val > 0) {
            sem->val--;
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            fmtxunlk(&sem->lk);
#endif

            return 0;
        } else {
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            fmtxunlk(&sem->lk);
#endif
            errno = EAGAIN;

            return -1;
        }
    } while (1);

    return 0;
}

long
sempost(mtsem *sem)
{
    do {
#if defined(PTHREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            thryield();
        }
#elif defined(MTFMTX)
        while (fmtxtrylk(&sem->lk)) {
            thryield();
        }
#endif
        if (!sem->val) {
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            fmtxunlk(&sem->lk);
#endif
            thryield();
        } else if (sem->val != MTSEM_MAXVAL) {
            sem->val++;
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
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

