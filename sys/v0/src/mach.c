#include <stdlib.h>
#include <v0/types.h>

struct v0sys *
v0confsys(struct v0sys *sys, struct v0sysconf *conf)
{
    size_t nbram = V0_RAM_SIZE;
    size_t ncl = nbram >> V0_CL_SHIFT;
    size_t ntlb = V0_TLB_ENTRIES;
    size_t npg = nbram >> V0_PAGE_SHIFT;
    size_t nio = V0_IOMAP_ENTRIES;

    if (!sys) {
        sys = calloc(1, sizeof(struct v0sys));
    } else {
        memset(sys, sizeof(struct v0sys));
    }
    if (sys) {
        if (!v0initsys(sys, nbram, ncl, ntlb, npg, nio)) {
            if (sys->ram) {
                free(sys->ram);
            }
            if (sys->pagetab) {
                free(sys->pagetab);
            }
            if (sys->clbits) {
                free(sys->clbits);
            }
            if (sys->iomap) {
                free(sys->iomap);
            }

            return NULL;
        }
    }

    return sys;
}

