#ifndef __MT_TKTLK_H__
#define __MT_TKTLK_H__

/* velho ticket locks */

/* REFERENCE: http://locklessinc.com/articles/locks/ */

#include <stdint.h>
#include <endian.h>
#include <zero/cdefs.h>
#include <mach/asm.h>
#include <mach/param.h>
#include <mt/thr.h>

#define MTTKTLKSPINS 16384
#define MTTKLKTSIZE  (2 * WORDSIZE)

#if (WORDSIZE == 4)

union mttktlk {
    m_atomicu32_t uval;
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
    struct {
        uint16_t  val;
        uint16_t  cnt;
    } s;
#else
    struct {
        uint16_t  cnt;
        uint16_t  val;
    } s;
#endif
};

#elif (WORDSIZE == 8)

union mttktlk {
    m_atomicu64_t uval;
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
    struct {
        uint32_t  val;
        uint32_t  cnt;
    } s;
#else
    struct {
        uint32_t  cnt;
        uint32_t  val;
    } s;
#endif
};

#endif /* MTTKTLKSIZE */

#define MTTKTBKTITEMS (CLSIZE / MTTKTSIZE)
struct mttktbkt {
    union mttktlk tab[MTTKTBKTITEMS];
};

typedef volatile union mttktlk mttktlk;

#if (WORDSIZE == 4)

static INLINE void
mtlktkt(mttktlk *tp)
{
    uint16_t val = m_fetchaddu16(&tp->s.cnt, 1);

    while (tp->s.val != val) {
        m_waitspin();
    }

    return;
}

static INLINE void
mtunlktkt(mttktlk *tp)
{
    m_membar();
    tp->s.val++;
    m_endspin();

    return;
}

/* return 1 if lock succeeds, 0 otherwise */
static INLINE long
mttrytktlk(union mttktlk *tp)
{
    uint16_t val = tp->s.cnt;
    uint16_t cnt = val + 1;
    uint32_t cmp = ((uint32_t)val << 16) | val;
    uint32_t cmpnew = ((uint32_t)cnt << 16) | val;
    long     res = 0;

    if (m_cmpswapu32(&tp->uval, cmp, cmpnew)) {
        res++;
    }

    return res;
}

#elif (WORDSIZE == 8)

static INLINE void
mtlktkt(union mttktlk *tp)
{
    uint32_t val = m_fetchaddu32(&tp->s.cnt, 1);

    while (tp->s.val != val) {
        m_waitspin();
    }

    return;
}

static INLINE void
mtunlktkt(union mttktlk *tp)
{
    m_membar();
    tp->s.val++;
    m_endspin();

    return;
}

/* return 1 if lock succeeds, 0 otherwise */
static INLINE long
mttrytktlk(union mttktlk *tp)
{
    uint32_t val = tp->s.cnt;
    uint32_t cnt = val + 1;
    uint64_t cmp = ((uint64_t)val << 32) | val;
    uint64_t cmpnew = ((uint64_t)cnt << 32) | val;
    long     res = 0;

    if (m_cmpswapu64(&tp->uval, cmp, cmpnew)) {
        res++;
    }

    return res;
}

#endif /* WORDSIZE */

#if 0
static INLINE long
tktmaylk(union mttktlk *tp)
{
    union mttktlk tu = *tp;

    m_membar();

    return (tu.s.val == tu.s.cnt);
}
#endif

#endif /* __MT_TKTLK_H__ */

