/* REFERENCE: http://locklessinc.com/articles/barriers/ */

#include <mach/asm.h>
#include <mt/mtx.h>
#include <mt/cond.h>
#include <mt/bar.h>
#include <zero/sys.h>

#if (MTFMTX)
#define barinitlk(lp) (*(lp) = FMTXINITVAL)
#define barlk(lp)     fmtxlk(lp)
#define barunlk(lp)   fmtxunlk(lp)
#else
#define barinitlk(lp) (*(lp) = MTXINITVAL)
#define barlk(lp)     mtxlk(lp)
#endif

void
barfree(mtbar *bar)
{
    barlk(&bar->lk);
    while (bar->num > BARFLAGBIT) {
        condwait(&bar->cond, &bar->lk);
    }
    barunlk(&bar->lk);
//    condfree(&bar->cond);
}

void
barinit(mtbar *bar, unsigned long cnt)
{
    barinitlk(&bar->lk);
    condinit(&bar->cond);
    bar->cnt = cnt;
    bar->num = BARFLAGBIT;
}

long
barwait(mtbar *bar)
{
    barlk(&bar->lk);
    while (bar->num > BARFLAGBIT) {
        condwait(&bar->cond, &bar->lk);
    }
    if (bar->num == BARFLAGBIT) {
        bar->num = 0;
    }
    bar->num++;
    if (bar->num == bar->cnt) {
        bar->num += BARFLAGBIT - 1;
        condbcast(&bar->cond);
        barunlk(&bar->lk);

        return BARSERIALTHR;
    } else {
        while (bar->num < BARFLAGBIT) {
            condwait(&bar->cond, &bar->lk);
        }
        bar->num--;
        if (bar->num == BARFLAGBIT) {
            condbcast(&bar->cond);
        }
        barunlk(&bar->lk);

        return 0;
    }
}

void
barinitpool(mtbarpool *pool, long cnt)
{
    cnt--;
    pool->cnt.vals.seq = 0;
    pool->cnt.vals.cnt = 0;
    pool->nref = 1;
    pool->num = cnt;

    return;
}

void
barfreepool(mtbarpool *pool)
{
    m_fetchadd32(&pool->nref, -1);
    do {
        volatile long nref = m_atomread(&pool->nref);

        if (!nref) {

            return;
        }
        syswait(&pool->nref, nref);
    } while (1);

    return;
}

long
barwaitpool(mtbarpool *pool)
{
    long ret;

    do {
        unsigned long seq = m_atomread(&pool->cnt.vals.seq);
        long          cnt = m_fetchaddu32(&pool->cnt.vals.cnt, 1);

        if (cnt < pool->num) {
            while (m_atomread(&pool->cnt.vals.seq) == seq) {
                syswait(&pool->cnt.vals.seq, seq);
            }
            ret = 0;

            break;
        }
        if (cnt == pool->num) {
            /* mt pool->cnt.vals.cnt, increment pool->cnt.vals.seq */
            m_membar();
            pool->cnt.rst = pool->cnt.vals.seq + 1;
            m_membar();
            syswake(&pool->cnt.vals.seq);
            ret = BARSERIALTHR;

            break;
        }
        /* we were too slow, so wait for barrier to be released */
        syswait(&pool->cnt.vals.seq, seq);
    } while (1);
    if (m_fetchadd32(&pool->nref, -1) == 1) {
        /* last one to wake up, wake destroying thread */
        syswait(&pool->nref, 1);
    }

    return ret;
}

