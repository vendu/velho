#ifndef __SYS_V0_OP_H__
#define __SYS_V0_OP_H__

#include <stdint.h>

#define sex(x, max)     (((x) ^ (max)) - (max))
#define sexb(b)         (((b) ^ 128) - 128)
#define sexh(b)         (((b) ^ 65536) - 128)

#define haszerob(u)     (((u) - 0x01010101U) & ~(u) & 0x80808080U)

extern const uint8_t g_clztab4bit[16];
extern const uint8_t g_clztab16bit[16];

static __inline__ long
m_clz32(uint32_t u32)
{
    uint32_t    ones = 32;

    if (u32) {
        if (!(u32 & V0_CONST_HIGH_16)) {
            ones += 16; u32 <<= 16;
        }
        if (!(u32 & V0_CONST_HIGH_8)) {
            ones += 8; u32 <<= 8;
        }
        if (!(u32 & V0_CONST_HIGH_4)) {
            ones += 4; u32 <<= 4;
        }
        u32 >>= sizeof(uint32_t) - 4;
        ones += g_clztab4bit[u32];
    }

    return ones;
}

static __inline__ long
m_clz32a(uint32_t u32)
{
    uint32_t    cnt = 32;
    uint32_t    val;

    if (u32) {
        if (u32 > V0_CONST_LOW_16) {
            cnt = 24;
        } else {
            cnt = 16;
        }
    } else {
        if (u32 > V0_CONST_LOW_8) {
            cnt = 8;
        } else {
            cnt = 0;
        }
    }
    u32 >>= cnt;
    val = g_clztab16bit[u32];
    val -= cnt;

    return val;
}

static __inline__ long
m_ctz(uint32_t u32)
{
    uint32_t val = u32 & (-u32);
    uint32_t magic = V0_CONST_CTZ;

    val *= V0_CONST_CTZ;
    val >>= 27;
    val = g_ctztab4bit[val];

    return val;
}

static __inline__ long
bytepar(uint8_t b)
{
    unsigned long ret;
    unsigned long val1;
    unsigned long val2;

    val1 = b;
    val2 = b;
    val2 >>= 4;
    val1 ^= val2;
    ret = (0x6996 >> (val1 & 0x0f)) & 0x01;

    return ret;
}

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

#endif /* __SYS_V0_OP_H__ */

