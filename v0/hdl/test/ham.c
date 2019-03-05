#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "v0.h"

uint32_t
hamu32mul(uint32_t a)
{
    /* dX == (~0) / X */
    uint32_t d3 = 0x55555555;
    uint32_t d5 = 0x33333333;
    uint32_t d17 = 0x0f0f0f0f;
    uint32_t d255 = 0x01010101;

    a -= (a >> 1) & d3;
    a = (a & d5) + ((a >> 2) & d5);
    a = (a + (a >> 4)) & d17;
    a = (a * d255) >> 24;

    return a;
}

uint32_t
hamu32nomul(uint32_t a)
{
    /* dX == (~0) / X */
    uint32_t d3 = 0x55555555;
    uint32_t d5 = 0x33333333;
    uint32_t d17 = 0x0f0f0f0f;

    a -= (a >> 1) & d3;
    a = (a & d5) + ((a >> 2) & d5);
    a = (a + (a >> 4)) & d17;
    a += a >> 8;
    a += a >> 16;
    a &= 0x7f;

    return a;
}

int
hamtest32(uint32_t a)
{
    uint32_t bit = UINT32_C(0x00000001);
    int      n = 0;
    int      i;

    for (i = 0 ; i < 32 ; i++) {
        if (a & bit) {
            n++;
        }
        bit <<= 1;
    }

    return n;
}

int
_v0ham(uint32_t u32)
{
    int      out;
    uint32_t res;
    uint32_t tmp1;
    uint32_t tmp2;
    uint32_t tmp3;
    uint32_t tmp4;
    uint32_t val1;
    uint32_t val2;
    uint32_t val3;
    uint32_t val4;
    uint32_t val5;

    val1 = 0x55555555;
    val2 = 0x33333333;
    val3 = 0x0f0f0f0f;
    val4 = 0x00ff00ff;
    val5 = 0x0000ffff;
    tmp1 = u32 >> 1;
    tmp2 = u32 & val1;
    tmp3 = tmp1 & val1;
    tmp4 = tmp2 + tmp3;
    tmp1 = tmp4;
    tmp2 = tmp4 >> 2;
    tmp3 = tmp4 & val2;
    tmp1 = tmp2 & val2;
    tmp4 = tmp1 + tmp3;
    tmp2 = tmp4;
    tmp1 = tmp4 >> 4;
    tmp3 = tmp2 & val3;
    tmp4 = tmp1 & val3;
    tmp3 = tmp3 + tmp4;
    tmp2 = tmp3;
    tmp1 = tmp3 >> 8;
    tmp4 = tmp3 & val4;
    tmp2 = tmp1 & val4;
    tmp3 = tmp2 + tmp4;
    tmp1 = tmp3;
    tmp2 = tmp3 >> 16;
    tmp4 = tmp1 & val5;
    tmp3 = tmp2 & val5;
    res = tmp3 + tmp4;

    return res;
}

int
testham(uint32_t ntest)
{
    uint32_t x;
    uint32_t res;
    uint32_t n;
    uint32_t n1;
    uint32_t n2;
    uint32_t n3;
    uint32_t n4;

    for (n = 0 ; n < ntest ; n++) {
        x = rand();
        res = hamtest32(x);
        n1 = hamu32mul(x);
        n2 = hamu32nomul(x);
        n3 = v0ham(x);
        n4 = _v0ham(x);
        if (n1 != res) {
            printf("MUL: %d != %d (%08lx)\n", n1, n, x);

            exit(1);
        }
        if (n2 != res) {
            printf("NOMUL: %d != %d (%08lx)\n", n1, n, x);

            exit(1);
        }
        if (n3 != res) {
            printf("HAM: %d != %d (%08lx)\n", n3, n, x);

            exit(1);
        }
        if (n4 != res) {
            printf("_HAM: %d != %d (%08lx)\n", n4, n, x);

            exit(1);
        }
    }

    return 0;
}

