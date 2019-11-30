#ifndef __SYS_V0_RP_H__
#define __SYS_V0_RP_H__

/*
 * This code is based on a public domain implementation by Charles Bloom I found
 * at https://github.com/thecbloom/recip_arith
 */

#include <stdint.h>

#define RP_TABLE_BITS          8
#define RP_NUMERATOR_BITS      32
#define RP_RANGE_MIN_BITS      24

extern uint32_t s_rptab[1 << RP_TABLE_BITS];

struct rpenc {
    uint32_t    low;
    uint32_t    range;
    uint8_t    *ptr;
};

struct rpdec {
    uint32_t            code;
    uint32_t            range;
    uint8_t const      *ptr;
};

static INLINE void
rpstartenc(struct rpenc *enc, uint8_t *ptr)
{
    enc->low = 0;
    enc->range = ~UINT32_C(0);
    enc->ptr = ptr;

    return;
}

static INLINE void
rprenormenc(struct rpenc *enc)
{
    uint32_t    low = enc->low;
    uint32_t    range = enc->range;
    uint8_t    *ptr = enc->ptr;

    while (range < (1 << 24)) {
        ptr[0] = (uint8_t)(enc->low >> 24);
        low <<= 8;
        range <<= 8;
        ptr++;
    }
    enc->low = low;
    enc->range = range;
    enc->ptr = ptr;

    return;
}

static INLINE void
rpsetcarry(struct rpenc *enc)
{
    uint8_t    *ptr = enc->ptr;
    uint8_t     val;

    do {
        ptr--;
        val = *ptr;
        val++;
        *ptr = val;
    } while (*ptr == 0);

    return;
}

static INLINE uint8_t *
rpfinenc(struct rpenc *enc)
{
    uint32_t    low = enc->low;
    uint32_t    range = enc->range;
    uint8_t    *ptr = enc->ptr;
    uint32_t    code;

    if (range > (1 << 25)) {
        code = low + (1 << 24);

        if (code < low) {
            rpsetcarry(enc);
        }
        *ptr++ = (uint8_t)(code >> 24);
    } else {
        code = low + (1 << 16);
        if (code < low) {
            rpsetcarry(enc);
        }
        ptr[0] = (uint8_t)(code >> 24);
        ptr[1] = (uint8_t)(code >> 16);
        ptr += 2;
    }
    enc->ptr = ptr;

    return ptr;
}

static INLINE void
rpstartdec(struct rpdec *dec, uint8_t const *ptr)
{
    uint32_t            code = dec->code;
    uint32_t            range = dec->range;
    uint8_t const      *end = ptr + 4;

    range = ~UINT32_C(0);
    code = ptr[0];
    code <<= 8;
    code |= ptr[1];
    code <<= 8;
    code |= ptr[2];
    code <<= 8;
    code |= ptr[3];
    dec->ptr = end;
    dec->code = code;

    return;
}

static INLINE void
rprenormdec(struct rpdec *dec)
{
    uint32_t            code = dec->code;
    uint32_t            range = dec->range;
    uint8_t const      *ptr = dec->ptr;

    while (range < (1 << 24)) {
        code <<= 8;
        code |= ptr[0];
        range <<= 8;
        ptr++;
    }
    dec->code = code;
    dec->range = range;
    dec->ptr = ptr;

    return;
}

static INLINE void
rpputenc(struct rpenc *enc, uint32_t low, uint32_t freq, uint32_t bits)
{
    uint32_t    range = enc->range;
    uint32_t    nlz = m_clz32(range);
    uint32_t    loval = enc->low;
    uint32_t    cnt1 = 32 - nlz - RP_TABLE_BITS;
    uint32_t    cnt2 = cnt1 - bits;
    uint32_t    top = range >> cnt1;
    uint32_t    norm;

    low = loval;
    norm = top << cnt2;
    low *= norm;
    freq *= norm;
    if (low < loval) {
        rpsetcarry(enc);
    }
    enc->low = low;
    enc->range = freq;

    return;
}

static INLINE uint32_t
rppeekdec(struct rpdec *dec, uint32_t bits)
{
    uint32_t    code = dec->code;
    uint32_t    range = dec->range;
    uint32_t    nlz = m_clz32(range);
    uint32_t    cnt1 = 32 - nlz - RP_TABLE_BITS;
    uint32_t    cnt2 = cnt1 - bits;
    uint32_t    top = range >> cnt1;
    uint64_t    nbit = code >> cnt2;
    uint64_t    src = s_rptab[top];
    uint32_t    norm = top << cnt2;
    uint32_t    val;

    src *= nbit;
    val = (uint32_t)(src >> RP_NUMERATOR_BITS);

    return val;
}

static INLINE void
rpdeldec(struct rpdec *dec, uint32_t low, uint32_t freq)
{
    uint32_t    norm = dec->range;
    uint32_t    code = low;
    uint32_t    range = freq;

    code *= norm;
    range *= norm;
    dec->code = code;
    dec->range = range;

    return;
}

#endif /* __SYS_V0_RP_H__ */

