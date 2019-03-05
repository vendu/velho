#include <stdio.h>
#include <stdint.h>
#include <valhalla/util.h>

int
main(int argc, char *argv[])
{
    uint32_t val = ceil2u32(0x00000001);
    uint32_t tmp;

    fprintf(stderr, "val: %u\n", val);
    tmp = crp24(27);
    fprintf(stderr, "crp24: %x\n", tmp);
    fprintf(stderr, "16384 / 27: %x\n", 16384 / 27);
    fprintf(stderr, "16384 / 27: %x\n", divu32(16384, 27));
    fprintf(stderr, "16384 / 17: %x\n", 16384 / 17);
    fprintf(stderr, "16384 / 17: %x\n", divu17(16384));
    fprintf(stderr, "1048576 / 17: %x\n", 1048576 / 17);
    fprintf(stderr, "1048576 / 17: %x\n", divu17(1048576));
    fprintf(stderr, "256 / 17: %x\n", 256 / 17);
    fprintf(stderr, "256 / 17: %x\n", divu17(256));

    return 0;
}

