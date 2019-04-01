#ifndef __SYS_V0_SHIFT_H__
#define __SYS_V0_SHIFT_H__

static __inline__ v0word
v0shl(cnt, word)
{
    word <<= cnt;

    return word;
}

static __inline__ v0word
v0shr(v0word cnt, v0word word)
{
    v0word mask = 0xffffffff;

    word >>= cnt;
    mask >>= cnt;
    word &= mask;

    return word;
}

static __inline__ v0word
v0sar(v0word cnt, v0word word)
{
    v0word sign = word & 0x80000000;
    v0word mask = 0xffffffff;

    word >>= cnt;
    if (sign) {
        mask <<= 32 - cnt;
        word |= mask;
    } else {
        mask >> cnt;
        word &= mask;
    }

    return word;
}

static __inline__ v0word
v0rol(v0word cnt, v0word word)
{
    v0word mask = 0xffffffff;

    mask >>= 32 - cnt;
    word <<= cnt;
    word |= mask;

    return word;
}

static __inline__ v0word
v0ror(v0word cnt, v0word word)
{
    v0word mask = 0xffffffff;

    mask <<= 32 - cnt;
    word >>= cnt;
    word |= mask;

    return word;
}

#endif /* __SYS_V0_SHIFT_H__ */

