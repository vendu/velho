#include <stddef.h>
#include <v0/v0.h>

static v0pagedesc *g_v0pagedir[V0_PAGE_DIR_ITEMS];
static v0pagedesc  g_v0tlb[V0_PAGE_TLB_ITEMS];

void
v0bzerow(void *ptr, size_t nw)
{
    v0word     *dest = ptr;
    v0word      zero = 0;
    size_t      val = (uintptr_t)ptr & (V0_CL_SIZE - 1);

    if (nw >= 2 * V0_CL_SIZE) {
        if (val) {
            val = V0_CLSIZE - val;
            nw -= val;
            while (val--) {
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
            src += 8;
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
    size_t      val = (uintptr_t)ptr & (V0_CL_SIZE - 1);

    if (nw >= 2 * V0_CL_SIZE) {
        if (val) {
            val = V0_CLSIZE - val;
            nw -= val;
            while (val--) {
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
            src += 8;
        }
    }
    while (nw--) {
        *dest++ = src++;
    }

    return;
}

