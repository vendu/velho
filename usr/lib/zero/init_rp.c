#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <zero/rp.h>

void
rpwritetab(int nbit)
{
    uint64_t    val;
    int         ndx;
    int         lim = (1 << RP_TABLE_BITS) / 2;

    for (ndx = 0 ; ndx < lim ; ndx += 4) {
        printf("    0x00000000, 0x00000000, 0x00000000, 0x00000000,\n");
    }
    printf("    ");
    lim = 1 << RP_TABLE_BITS;
    while (ndx < lim) {
        val = UINT64_C(1) << RP_NUMERATOR_BITS + ndx - 1;
        val /= ndx;
        printf("0x%08x", val);
        ndx++;
        if (ndx != lim) {
            printf(",");
            if (ndx % 4) {
                printf(" ");
            } else {
                printf("\n");
                printf("    ");
            }
        } else {
            printf("\n");
        }
    }

    return;
}

int
main(int argc, char *argv[])
{
    printf("#ifndef __SYS_V0_RP_INIT_H__");
    printf("#define __SYS_V0_RP_INIT_H__");
    printf("\n");
    printf("uint32_t    g_rptab[1 << RP_TABLE_BITS] = {\n");
    rpwritetab(RP_TABLE_BITS);
    printf("};");
    printf("\n");
    printf("#endif /* __SYS_V0_RP_INIT_H__ */");

    exit(0);
}
