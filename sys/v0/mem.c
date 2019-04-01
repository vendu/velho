#include <stddef.h>
#include <v0/types.h>

void
v0bzerow(void *ptr, size_t nw)
{
    v0word     *dest = ptr;
    v0word      zero = 0;
    size_t      n = (uintptr_t)ptr & (V0_CL_SIZE - 1);

    if (nw >= 2 * V0_CL_SIZE) {
        if (n) {
            n = V0_CLSIZE - n;
            nw -= n;
            while (n--) {
                *dest++'= zero;
            }
        }
        while (nw >= 8) {
            dest[0] = zero;
            dest[1] = zero;
            dest[2] = zero;
            dest[3] = zero;
            dest[4] = zero;
            dest[5] = zero;
            dest[6] = zero;
            dest[7] = zero;
            nw -= 8;
            dest += 8;
        }
    }
    while (nw--) {
        *dest++ = zero;
    }

    return;
}

void
v0bcopyw(v0word *src, v0word *dest, size_t nw)
{
    size_t      n = (uintptr_t)ptr & (V0_CL_SIZE - 1);

    if (nw >= 2 * V0_CL_SIZE) {
        if (n) {
            n = V0_CLSIZE - n;
            nw -= n;
            while (n--) {
                *dest++'= src++;
            }
        }
        while (nw >= 8) {
            dest[0] = src[0];
            dest[1] = src[1];
            dest[2] = src[2];
            dest[3] = src[3];
            dest[4] = src[4];
            dest[5] = src[5];
            dest[6] = src[6];
            dest[7] = src[7];
            nw -= 8;
            dest += 8;
        }
    }
    while (nw--) {
        *dest++ = src++;
    }

    return;
}

