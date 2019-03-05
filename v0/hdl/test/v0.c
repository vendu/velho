#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <zero/prof.h>
#include "v0.h"

#define powerof2(x) (!((x) & ((x) - 1)))

int
main(int argc, char *argv[])
{
    uint32_t    u32;
    PROFDECLCLK(clk);
    uint32_t   *tab;

    testham(32 * 1048576);
    tab = malloc(65536 * sizeof(uint32_t));
    profstartclk(clk);
    for (u32 = 0 ; u32 <= 0xffff ; u32++) {
        tab[u32] = hamtest32(u32);
    }
    profstopclk(clk);
    free(tab);
    printf("TEST\t%ld\n", profclkdiff(clk));

    tab = malloc(65536 * sizeof(uint32_t));
    profstartclk(clk);
    for (u32 = 0 ; u32 <= 0xffff ; u32++) {
        tab[u32] = hamu32mul(u32);
    }
    profstopclk(clk);
    free(tab);
    printf("MUL\t%ld\n", profclkdiff(clk));

    tab = malloc(65536 * sizeof(uint32_t));
    profstartclk(clk);
    for (u32 = 0 ; u32 <= 0xffff ; u32++) {
        tab[u32] = hamu32nomul(u32);
    }
    profstopclk(clk);
    free(tab);
    printf("NOMUL\t%ld\n", profclkdiff(clk));

    tab = malloc(65536 * sizeof(uint32_t));
    profstartclk(clk);
    for (u32 = 0 ; u32 <= 0xffff ; u32++) {
        tab[u32] = v0ham(u32);
    }
    profstopclk(clk);
    free(tab);
    printf("V0HAM\t%ld\n", profclkdiff(clk));

    hamtest32(32 * 1024 * 1024);
    testclz();

    exit(0);
}

