#ifndef __MT_SPINRW_H__
#define __MT_SPINRW_H__

/* REFERENCE: https://jfdube.wordpress.com/2014/01/03/implementing-a-recursive-read-write-spinlock/ */

/*
 * NOTES
 * -----
 * - initialise spinrw-locks with spininit() from <zero/spin.h>
 */

#include <assert.h>
#include <stdint.h>
#include <zero/thr.h>
#include <zero/param.h>
#include <zero/asm.h>
#include <zero/spin.h>

#if (LONGSIZE == 8)
#define SPINRWWRBIT   (UINT64_C(1) << 63)
#elif (LONGSIZE == 4)
#define SPINRWWRBIT   (UINT32_C(1) << 31)
#endif
#define SPINRWCNTMASK (~SPINWRWRBIT)

#define ZEROSPINRW_FREE      (-1L)
#define ZEROSPINRWREC_DEFVAL { ZEROSPININITVAL, ZEROSPINRW_FREE, 0 }
/* structure for recursive locks */
typedef struct spinrw {
    volatile long lk;   // lock value
    long          thr;  // writer thread ID
    long          rec;  // recursion depth
} zerospinrwrec;

static __inline__ void
spinrwlkrd(volatile long *sp)
{
    volatile long old;
    volatile long val;

    do {
        while (*sp & SPINRWWRBIT) {
            m_waitspin();
        }
        old = *sp & SPINRWCNTMASK;
        val = old + 1;
        if (m_cmpswap(sp, old, val)) {

            return;
        }
    } while (1);

    return;
}

static __inline__ void
spinrwunlkrd(volatile long *sp)
{
    m_atominc(*sp);

    return;
}

static __inline__ void
spinrwlkwr(volatile long *sp)
{
    volatile long old;
    volatile long val;

    do {
        while (*sp & SPINRWWRBIT) {
            thryield();
        }
        old = *sp & SPINRWCNTMASK;
        val = old | SPINRWWRBIT;

        if (m_cmpswap(sp, old, val) == old) {
            while (*sp & SPINRWCNTMASK) {
                m_waitspin();
            }
        }
    } while (1);

    return;
}

static __inline__ void
spinrwunlkwr(volatile long *sp)
{
    assert(*sp == SPINRWWRBIT);
    *sp = ZEROSPININITVAL;
    m_endspin();

    return;
}

static __inline__ void
zerolkspinrwrecwr(zerospinrwrec *spin)
{
    long thr = thrid();

    if (spin->thr == thr) {
        spinrwlkwr(&spin->lk);
        assert(!spin->rec);
        spin->thr = thr;
    }
    spin->rec++;
}

static __inline__ void
zerounlkspinrwrecwr(zerospinrwrec *spin)
{
    long thr = thrid();

    assert(thr == spin->thr);
    spin->rec--;
    if (!spin->rec) {
        spin->thr = ZEROSPINRW_FREE;
        spinrwunlkwr(&spin->lk);
    }
}

#endif /* __MT_SPINRW_H__ */

