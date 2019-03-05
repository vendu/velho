#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// verified routine
uint32_t
clz32(uint32_t u32)
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

int
testclz(void)
{
    uint32_t x;
    int      i;
    int      n;
    int      n1;
    int      n2;

    x = 1;
    for (i = 0 ; i < 32 ; i++) {
        n1 = clz32(x);
        n2 = v0clz(x);
        printf("CLZ\t%d (%08lx)\n", n1, x);
        printf("_CLZ\t%d (%08lx)\n", n2, x);
        x <<= 1;
    }

    x = 0;
    n = __builtin_clz(x);
    n1 = clz32(x);
    if (n != n1) {
        printf("CLZ32\t%d != %d (%08lx)\n", n1, n, x);

        exit(1);
    }
    x = 0xffffffff;
    n = __builtin_clz(x);
    n1 = clz32(x);
    if (n != n1) {
        printf("CLZ32\t%d != %d (%08lx)\n", n1, n, x);

        exit(1);
    }
    x = 0x00000001;
    for (i = 0 ; i < 32 ; i++) {
        n = __builtin_clz(x);
        n1 = clz32(x);
        if (n != n1) {
            printf("CLZ32\t%d != %d (%08lx)\n", n1, n, x);

            exit(1);
        }
        x <<= 1;
    }
    for (i = 1 ; i < 1048576 ; i++) {
        x = rand() & 0x7fffffff;
        n = __builtin_clz(x);
        n1 = clz32(x);
        if (n != n1) {
            printf("CLZ32\t%d != %d (%08lx)\n", n1, n, x);

            exit(1);
        }
    }

    exit(0);
}

#if 0
int
main(int argc, char *argv[])
{
    testclz();

    exit(0);
}
#endif

