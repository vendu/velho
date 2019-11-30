#include <stdio.h>
#include <stdlib.h>
#include <zero/irp.h>

int
main(int argc, char *argv[])
{
    fprintf(stderr, "%x (%x)\n", 3975 / 7, (UINT64_C(3975) * irp32(7)) >> 2);

    exit(0);
}

