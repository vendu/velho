#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <valhalla/util.h>
#include <v0/vm/math.h>

uint32_t
ham32(uint32_t u32)
{
    uint32_t res = 0;
    uint32_t mask32;

    for (mask32 = 1 ; (mask32) ; mask32 <<= 1) {
        if (u32 & mask32) {
            res++;
        }
    }

    return res;
}

int
main(int argc, char *argv[])
{
    uint32_t val = 0;
    uint32_t tmp = 0;
    uint32_t rnd;

    v0clzop(val, tmp);
    fprintf(stderr, "CLZ(0) == %u\n", tmp);

    for (val = 1 ; (val) ; val <<= 1) {
        v0clzop(val, tmp);
        fprintf(stderr, "CLZ(%x) == %u\n", val, tmp);
    }
    tmp = ham32(0);
    fprintf(stderr, "CLZ(0) == %u\n", tmp);
    for (val = 1 ; (val) ; val++) {
        fprintf(stderr, "%d\n", val);
        rnd = rand();
        _v0hamop1(rnd, tmp);
        if (ham32(rnd) != tmp) {
            fprintf(stderr, "HAM(%x): %d != %d!\n", rnd, tmp, ham32(rnd));

            exit(1);
        }
    }

    return 0;
}

