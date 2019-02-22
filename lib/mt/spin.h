#ifndef __MT_SPIN_H__
#define __MT_SPIN_H__

#include <mach/asm.h>

#if !defined(ZERONEWSPIN)
#define ZERONEWSPIN 1
#endif

typedef volatile m_atomic_t zerospin;

#define ZEROSPININITVAL 0L
#define ZEROSPINLKVAL   1L

#define spininit(sp) (*(sp) = ZEROSPININITVAL)

/*
 * try to acquire spin-lock
 * - return non-zero on success, zero if already locked
 */
static __inline__ long
spintrylk(m_atomic_t *sp)
{
    m_atomic_t res = *sp;

    if (res == ZEROSPININITVAL) {
        res = m_cmpswap(sp, ZEROSPININITVAL, ZEROSPINLKVAL);
    }

    return res;
}

/*
 * lock spin-lock
 */
static __inline__ void
spinlk(m_atomic_t *sp)
{
    m_atomic_t res = 0;

    do {
        if (*sp == ZEROSPININITVAL) {
            res = m_cmpswap(sp, ZEROSPININITVAL, ZEROSPINLKVAL);
            if (res) {

                return;
            }
        }
        m_waitspin();
    } while (!res);

    return;
}

/*
 * release spin-lock
 */
static __inline__ void
spinunlk(m_atomic_t *sp)
{
    m_membar();
    *sp = ZEROSPININITVAL;
    m_endspin();

    return;
}

#endif /* __MT_SPIN_H__ */

