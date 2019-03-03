#ifndef __MT_MTX_H__
#define __MT_MTX_H__

#define MTMTX     1
#if !defined(MTFMTX)
#define MTFMTX    1
#endif

//#include <mt/conf.h>
#include <stddef.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <mach/asm.h>

typedef volatile m_atomic_t mtmtx;
typedef volatile m_atomic_t mtfmtx;

/*
 * Special thanks to Matthew 'kinetik'
 * Gregan for help with the mutex code.:)
 */

#define fmtxinit(lp)    (*(lp) = MTMTXINITVAL)
#define fmtxfree(lp) /* no-op */

#define MTMTXINITVAL    0
#define MTMTXLKVAL      1
#define MTMTXCONTVAL    2
#define MTFMTXINITVAL   0
#define MTFMTXLKVAL     1

static INLINE void
mtinitftmx(volatile m_atomic_t *lp)
{
    m_membar();         // full memory barrier
    *lp = MTMTXINITVAL;  // lazy-write

    return;
}

/*
 * try to acquire fast mutex lock
 * - return non-zero on success, zero if already locked
 */
static INLINE long
mttryfmtx(volatile m_atomic_t *lp)
{
    m_atomic_t res = 0;

    if (*lp == MTMTXINITVAL) {
        res = m_cmpswap(lp, MTMTXINITVAL, MTMTXLKVAL);
    }

    return res;
}

/*
 * - acquire fast mutex lock
 * - spin on volatile lock to avoid excess lock-operations
 */
static INLINE void
mtlkfmtx(volatile m_atomic_t *lp)
{
    m_atomic_t res = 0;

    do {
        while (*lp != MTMTXINITVAL) {
            m_waitspin();
        }
        res = m_cmpswap(lp, MTMTXINITVAL, MTMTXLKVAL);
    } while (!res);

    return;
}

/*
 * unlock fast mutex
 * - must use full memory barrier to guarantee proper write-ordering
 */
static INLINE void
mtunlkfmtx(volatile m_atomic_t *lp)
{
    m_membar();         // full memory barrier
    *lp = MTMTXINITVAL;  // lazy-write
    m_endspin();        // signal wakeup-event

    return;
}

#endif /* __MT_MTX_H__ */

