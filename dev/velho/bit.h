#ifndef __V0_BIT_H__
#define __V0_BIT_H__

#include <stdint.h>
#include <velho/cdefs.h>
#include <v0/const.h>

extern const uint8_t    bitparu8lut[256];
extern const uint8_t    bitclzu32lut[256];
extern const uint8_t    bitctzu32lut[256];
extern const uint8_t    bitctzu32tab[32];
extern const uint8_t    bithamu32lut[32];

/* TESTED OK */
static INLINE long
clzu32(uint32_t u32)
{
    uint32_t    tmp;
    uint32_t    val;
    long        cnt;

    if (u32) {
        tmp = u32 >> 24;
        val = bitclzu32lut[tmp];
        cnt = val;
        if (!tmp) {
            tmp = (u32 >> 16) & V0_CONST_LOW_8;
            val = bitclzu32lut[tmp];
            cnt += val;
            if (!tmp) {
                tmp = (u32 >> 8) & V0_CONST_LOW_8;
                val = bitclzu32lut[tmp];
                cnt += val;
                if (!tmp) {
                    tmp = u32 & V0_CONST_LOW_8;
                    val = bitclzu32lut[tmp];
                    cnt += val;
                }
            }
        }
    } else {
        cnt = 32;
    }

    return cnt;
}

/* TESTED OK */
static INLINE long
ctzu32lut(uint32_t u32)
{
    uint32_t    tmp;
    uint32_t    val;
    long        cnt;

    if (u32) {
        tmp = u32 & V0_CONST_LOW_8;
        val = bitctzu32lut[tmp];
        cnt = val;
        if (!tmp) {
            tmp = (u32 >> 8) & V0_CONST_LOW_8;
            val = bitctzu32lut[tmp];
            cnt += val;
            if (!tmp) {
                tmp = (u32 >> 16) & V0_CONST_LOW_8;
                val = bitctzu32lut[tmp];
                cnt += val;
                if (!tmp) {
                    tmp = u32 >> 24;
                    val = bitctzu32lut[tmp];
                    cnt += val;
                }
            }
        }
    } else {
        cnt = 32;
    }

    return cnt;
}

/* TESTED OK */
static INLINE long
ctzu32(uint32_t u32)
{
    uint32_t    val = u32 & -u32;

    val *= V0_CONST_CTZ32;
    val >>= 27;
    val = bitctzu32tab[val];
    val += !u32;

    return (long)val;
}

/* TESTED OK */
static INLINE long
hamu32lut(uint32_t u32)
{
    uint32_t    tmp;
    uint32_t    val;
    long        cnt = 0;

    if (u32) {
        tmp = u32 >> 24;
        val = bithamu32lut[tmp];
        cnt += val;
        tmp = (u32 >> 16) & V0_CONST_LOW_8;
        val = bithamu32lut[tmp];
        cnt += val;
        tmp = (u32 >> 8) & V0_CONST_LOW_8;
        val = bithamu32lut[tmp];
        cnt += val;
        tmp = u32 & V0_CONST_LOW_8;
        val = bithamu32lut[tmp];
        cnt += val;
    }

    return cnt;
}

/* TESTED OK */
static INLINE long
hamu32(uint32_t u32)
{
    u32 = u32 - ((u32 >> 1) & 0x55555555);
    u32 = (u32 & 0x33333333) + ((u32 >> 2) & 0x33333333);
    u32 = (u32 & 0x0f0f0f0f) + ((u32 >> 4) & 0x0f0f0f0f);
    u32 = (u32 & 0x00ff00ff) + ((u32 >> 8) & 0x00ff00ff);
    u32 = (u32 & 0x0000ffff) + ((u32 >> 16) & 0x0000ffff);

    return (long)u32;
}

static INLINE long
paru8(uint8_t b)
{
    uint32_t    ret = 0x6996;
    uint32_t    val1;
    uint32_t    val2;

    val2 = b;
    val1 = b;
    val2 >>= 4;
    val1 ^= val2;
    val1 &= 0x0f;
    ret >>= val1;
    ret &= 0x01;

    return ret;
}

static INLINE long
paru8lut(uint8_t b)
{
    long        val = bitparu8lut[b];

    return val;
}

#endif /* __V0_BIT_H__ */

