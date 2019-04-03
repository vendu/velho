#ifndef __SYS_V0_BIT_H__
#define __SYS_V0_BIT_H__

#include <stdint.h>

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

#endif /* __SYS_V0_BIT_H__ */

