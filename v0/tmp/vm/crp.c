/* HAZARD: this file is seriously broken at the moment... =) */

/* compute reciprocals accurate to 24 bits */

#include <stdio.h>
#include <stdint.h>
#include <valhalla/util.h>

static uint8_t crp24tab[256];

void
crp24filltab(uint8_t *tab)
{
    long   ndx;
    double d;
    double e;

    for (ndx = 1 ; ndx <= 0xff ; ndx++) {
        d = (double)ndx + 0.5;
        d /= 256.0;
        d += 1.0;
        e = 1.0 / d;
        e *= 256.0;
        e *= 512.0;
        e += 0.5;
        e -= 256.0;
        tab[ndx] = (uint8_t)e;
    }

    return;
}

#if 0
uint32_t
crp24(uint32_t num, uint32_t div)
{
    uint64_t r = clz32(div);
    uint64_t e = 1;
    uint64_t t = r * div;

    e -= t;
    /* e = fma(e, e, e); */
    t = e;
    e *= e;
    e += t;
    t = r;
    r *= e;
    r += t;

    return (uint32_t)r;
}
#endif

/* r = clz32(div), e = r * div - 1, t = (1 - e) * r */
uint32_t
crp24(uint32_t div)
{
    uint32_t r = clz32(div);
    uint32_t t = r * div;
    uint32_t e = t - 1;

    t = 1;
    r *= e;
    t -= r;

    return t;
}

uint32_t
fcrp24(uint32_t z)
{
    uint32_t x;
    uint32_t y;
    uint32_t sign;
    uint32_t exp;

    sign = z & 0x80000000;
    exp = (z >> 23) & 0xff;
    x = exp - 1;
    if (x > 0xfd) {
        x = z << 1;
        if (x <= 0x00400000) {
            /* zero or small denorm, return infinity with same sign */
            return sign | 0x7f800000;
        } else if (x > 0xff000000) {
            /* SNaNs -> QNaNs */
            return z | 0x00400000;
        } else if (x != 0xff000000) {
            /* large denormal, normalize */
            y = z < 0x00800000;
            z = x << y;
            exp = exp - y;
        } else {
            /* infinity return zero with proper sign */
            return sign;
        }
    }
    y = z & 0x007fffff;
    z = 0x100 | crp24tab[y >> 15];
    y = y | 0x00800000;
    /* cubically convergent reciprocal approximation */
    x = (uint32_t)-(int32_t)(y * z); /* x = 1 - arg * approx */
    x = mulhu32(x, x) | x; /* x = x * x + x */
    z = z << 15;
    z = mulhu32(x, z) + z; /* approx = x * approx + approx */
    exp = 252 - exp;
    if (exp >= 0) {
        x = y * z;
        x += y;
        z = (exp << 23) + z;
    } else {
        exp = -exp;
        z = z >> exp;
        x = y * z;
        x += y;
    }
    z = z | sign;
    /* round */
    if ((int32_t)x <= (int32_t)(y >> 1)) {
        z++;
    }

    return z;
}

#if 0
/* r = clz32(div), e = r * div - 1, t = (1 - e) * r */
uint32_t
divu32(uint32_t num, uint32_t div)
{
    uint32_t val;
    uint32_t log2;
    uint64_t res;
    uint64_t tmp1;
    uint64_t tmp2;

    val = normu32(num, div, &log2);
    tmp1 = val;
    tmp1 <<= 5; /* tmp1 = val * 32 */
    tmp2 = tmp1 + 48; /* tmp2 = val + 32 / 17 */
    tmp1 = divu17(tmp2); /* tmp1 = 32/17 * val + 48/17 */
    tmp2 = tmp1 * (2 - val * res);
    tmp1 = tmp2 * (2 - val * tmp2);
    tmp2 = tmp1 * (2 - val * tmp1);
    res = tmp2 * num;
    //    res >>= log2;

    return (uint32_t)res;
}
#endif

static INLINE uint64_t
norm64u32(uint32_t val, uint32_t *retlog2)
{
    uint64_t res = val;
    uint32_t cnt = 64 - clz32(val);

    if (res) {
        res <<= cnt;
        *retlog2 = cnt;
    }

    return res;
}

uint32_t
divu32(uint32_t num, uint32_t div)
{
    uint64_t val = div;
    uint32_t log2;
    uint64_t res;
    uint64_t tmp1;
    uint64_t tmp2;

    val = norm64u32(val, &log2);
    fprintf(stderr, "DIV: %x - LOG2: %u - NORM32: %x\n", div, log2, val);
    tmp1 = val;
    tmp1 <<= 5; /* tmp1 = val * 32 */
    tmp2 += tmp1 + 48; /* tmp2 = val + 32 / 17 */
    tmp1 = divu17(tmp2); /* tmp1 = 32/17 * val + 48/17 */
    tmp2 = tmp1 * (2 - val * res);
    tmp1 = tmp2 * (2 - val * tmp2);
    tmp2 = tmp1 * (2 - val * tmp1);
    res = tmp2 * num;
    //    res >>= log2;

    return (uint32_t)res;
}

