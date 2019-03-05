#include <stdint.h>

// REFERENCE: http://bisqwit.iki.fi/source/misc/bitcounting/
// - added tmp
// determine the Hamming weight aka bit population, i.e. number of 1-bits in u32
uint32_t
v0ham(uint32_t u32)
{
    /* dX == (~0) / X */
    uint32_t d3 = 0x55555555;
    uint32_t d5 = 0x33333333;
    uint32_t d17 = 0x0f0f0f0f;
    uint32_t tmp;

    tmp = u32 >> 1;
    u32 -= tmp & d3;
    tmp = u32 >> 2;
    u32 = (u32 & d5) + (tmp & d5);
    tmp = u32 >> 4;
    u32 = (u32 + tmp) & d17;
    tmp = u32 >> 8;
    u32 += tmp;
    tmp = u32 >> 16;
    u32 += tmp;
    u32 &= 0x7f;

    return u32;
}

// determine number of leading (high-order) 0-bits in u32
uint32_t
v0clz(uint32_t u32)
{
    uint32_t res;
    uint32_t tmp;
    uint32_t mask;

    if (!u32) {
        res = 32;
    } else {
        res = 0;
        tmp = u32;
        mask = 0x80000000;
        if (!(tmp & mask)) {
            mask = 0xffff0000;
            if (!(tmp & mask)) {
                tmp <<= 16;
                res += 16;
            }
            mask <<= 8;
            if (!(tmp & mask)) {
                tmp <<= 8;
                res += 8;
            }
            mask <<= 4;
            if (!(tmp & mask)) {
                tmp <<= 4;
                res += 4;
            }
            mask <<= 2;
            if (!(tmp & mask)) {
                tmp <<= 2;
                res += 2;
            }
            mask <<= 1;
            if (!(tmp & mask)) {
                res++;
            }
        }
    }

    return res;
}

