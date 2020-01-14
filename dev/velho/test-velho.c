#include <limits.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <velho/bit.h>
#include <velho/divc.h>

static int g_testall = 0;
static int g_testverbose = 0;
static int g_testbit = 0;
static int g_testdivc = 0;

void
testgetopt(int argc, char *argv[])
{
    int arg;

    for (arg = 1 ; arg < argc ; arg++) {
        if (!strcmp(argv[arg], "--all")) {
            g_testall = 1;

            return;
        } else if (!strcmp(argv[arg], "--verbose")) {
            g_testverbose = 1;
        } else if (!strcmp(argv[arg], "--bit")) {
            g_testbit = 1;
        } else if (!strcmp(argv[arg], "--divc")){
            g_testdivc = 1;
        }
    }

    return;
}

void
testbit(void)
{
    uint32_t    num;

    for (num = 0 ; num <= UINT8_MAX ; num++) {
        assert(paru8(num) == __builtin_parity(num));
    }

    for (num = 1 ; num < UINT32_MAX ; num++) {
        if (g_testverbose) {
            if (!(num & 0xffff)) {
                fprintf(stderr, "bit\t%x\n", num);
            }
        }
        assert(clzu32(num) == __builtin_clz(num));
        assert(ctzu32(num) == __builtin_ctz(num));
        assert(ctzu32lut(num) == __builtin_ctz(num));
        assert(hamu32(num) == __builtin_popcount(num));
        assert(hamu32lut(num) == __builtin_popcount(num));
    }
    if (g_testverbose) {
        if (!(num & 0xffff)) {
            fprintf(stderr, "bit\t%x\n", num);
        }
    }
    assert(clzu32(num) == __builtin_clz(num));
    assert(ctzu32(num) == __builtin_ctz(num));
    assert(ctzu32lut(num) == __builtin_ctz(num));
    assert(hamu32(num) == __builtin_popcount(num));
    assert(hamu32lut(num) == __builtin_popcount(num));

         return;
}

void
testdivc(void)
{
    uint32_t    num;

    for (num = 1 ; num < UINT32_MAX ; num++) {
        if (g_testverbose) {
            if (!(num & 0xffff)) {
                fprintf(stderr, "divc\t%x\n", num);
            }
        }
        assert(divcu10(num) == num / 10);
        assert(divcu60(num) == num / 60);
        assert(modcu60(num) == num % 60);
        assert(modcu400(num) == num % 400);
        assert(divcu1000(num) == num / 1000);
    }
    if (g_testverbose) {
        fprintf(stderr, "divc\t%x\n", num);
    }
    assert(divcu10(num) == num / 10);
    assert(divcu60(num) == num / 60);
    assert(modcu60(num) == num % 60);
    assert(divcu100(num) == num / 100);
    assert(modcu400(num) == num % 400);
    assert(divcu1000(num) == num / 1000);

    return;
}

#define test(opt, func)                                                 \
    do {                                                                \
        if ((g_testall) || (opt)) {                                     \
            func();                                                     \
        }                                                               \
    } while (0)

void
testrun(void)
{
    test(g_testbit, testbit);
    test(g_testdivc, testdivc);

    return;
}

int
main(int argc, char *argv[])
{
    testgetopt(argc, argv);
    testrun();

    exit(0);
}

