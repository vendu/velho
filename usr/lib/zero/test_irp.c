#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <zero/irp.h>

struct irp irpstate;

int
main(int argc, char *argv[])
{
    fprintf(stderr, "33/7 == %d (%d)\n", 33/7, irpdiv(33, 7));

    exit(0);
}

