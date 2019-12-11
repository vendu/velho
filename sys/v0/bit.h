#ifndef __SYS_V0_BIT_H__
#define __SYS_V0_BIT_H__

#include <stdint.h>
#include <v0/v0.h>

#define V0_CONST_CTZ32  0x0ef96a62

extern uint8_t          bitctztab4bit[16];
extern const uint8_t    bitclztab4bit[16];
extern const uint8_t    bitclztab16bit[16];

static __inline__ long
m_clz32(uint32_t u32)
{
    uint32_t    nlz = 32;

    if (u32) {
        if (!(u32 & V0_CONST_HIGH_16)) {
            nlz += 16; u32 <<= 16;
        }
        if (!(u32 & V0_CONST_HIGH_8)) {
            nlz += 8; u32 <<= 8;
        }
        if (!(u32 & V0_CONST_HIGH_4)) {
            nlz += 4; u32 >>= sizeof(uint32_t) - 8;
        }
        nlz += bitclztab4bit[u32];
    }

    return nlz;
}

static __inline__ long
m_ctz32(uint32_t u32)
{
    uint32_t    val = u32 & (-u32);

    val *= V0_CONST_CTZ;
    val >>= 27;
    val = bitctztab4bit[val];
    val += !u32;

    return val;
}

static __inline__ long
bytepar(uint8_t b)
{
    uint32_t    ret;
    uint32_t    val1;
    uint32_t    val2;

    val1 = b;
    val2 = b;
    val2 >>= 4;
    val1 ^= val2;
    ret = (0x6996 >> (val1 & 0x0f)) & 0x01;

    return ret;
}

#endif /* __SYS_V0_BIT_H__ */

