#ifndef __MT_SPINWT_H__
#define __MT_SPINWT_H__

/*
 * NOTES
 * -----
 * - initialise spin-wait locks with spininit() from <zero/spin.h>
 */

#include <zero/asm.h>
#include <zero/spin.h>

/*
 * try to acquire spin-wait lock
 * - return non-zero on success, zero otherwise
 */
static __inline__ long
spinwttrylk(volatile long *sp, long val, long niter)
{
    volatile long res = 0;

    while ((niter) && !m_cmpswap(sp, ZEROSPININITVAL, val)) {
        niter--;
        m_waitspin();
    }
    if (!niter) {
        res = m_cmpswap(sp, ZEROSPININITVAL, val);
    } else {
        res++;
    }

    return res;
}

/*
 * lock spin-wait lock
 */
static __inline__ void
spinwtlk(volatile long *sp, long val, long niter)
{
    volatile long res = val;

    while ((niter) && !m_cmpswap(sp, ZEROSPININITVAL, val)) {
        niter--;
        m_waitspin();
    }
    if (!niter) {
        mtxlk(sp, val);
    }

    return;
}

/*
 * release spin-wait lock
 */
static __inline__ void
spinwtunlk(volatile long *sp)
{
    m_membar();
    *sp = ZEROSPININITVAL;
    m_endspin();

    return;
}

#endif /* __MT_SPINWT_H__ */

