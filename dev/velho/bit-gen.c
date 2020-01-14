#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void
genparu8(void)
{
    uint32_t    num;

    printf("const uint8_t bitparu8lut[256] = {\n");
    for (num = 0 ; num <= UINT8_MAX ; num++) {
        uint32_t    cnt = __builtin_parity(num);

        if (!num) {
            printf("    %d, ", cnt);
        } else if (num == UINT8_MAX) {
            printf("%d\n};", cnt);
        } else if (!(num & 0x07)) {
            printf("\n    %d, ", cnt);
        } else {
            printf("%d, ", cnt);
        }
    }
    printf("\n\n");

    return;
}

void
genclzu32(void)
{
    uint32_t    num;

    printf("#include <stdint.h>\n\n");
    printf("const uint8_t bitclzu32lut[256] = {\n");
    printf("    8, ");
    for (num = 1 ; num <= UINT8_MAX ; ) {
        uint32_t    cnt = __builtin_clz(num) - 24;

        num++;
        if (num & 0x07) {
            printf("%d, ", cnt);
        } else if (num <= UINT8_MAX) {
            printf("%d,\n    ", cnt);
        } else {
            printf("%d\n};", cnt);
        }
    }
    printf("\n\n");

    return;
}

void
genctzu32(void)
{
    uint32_t    num;

    printf("#include <stdint.h>\n\n");
    printf("const uint8_t bitctzu32lut[256] = {\n");
    printf("    8, ");
    for (num = 1 ; num <= UINT8_MAX ; ) {
        uint32_t    cnt = __builtin_ctz(num);

        num++;
        if (num & 0x07) {
            printf("%d, ", cnt);
        } else if (num <= UINT8_MAX) {
            printf("%d,\n    ", cnt);
        } else {
            printf("%d\n};", cnt);
        }
    }
    printf("\n\n");

    return;
}

void
genhamu32(void)
{
    uint32_t    num;

    printf("const uint8_t bithamu32lut[256] = {\n");
    for (num = 0 ; num <= UINT8_MAX ; num++) {
        uint32_t    cnt = __builtin_popcount(num);

        if (!num) {
            printf("    %d, ", cnt);
        } else if (num == UINT8_MAX) {
            printf("%d\n};", cnt);
        } else if (!(num & 0x07)) {
            printf("\n    %d, ", cnt);
        } else {
            printf("%d, ", cnt);
        }
    }
    printf("\n\n");

    return;
}

int
main(int argc, char *argv[])
{
    printf("#include <stdint.h>\n\n");
    genparu8();
    genclzu32();
    genctzu32();
    genhamu32();

    exit(0);
}

