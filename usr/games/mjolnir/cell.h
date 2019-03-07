#ifndef __MJOLNIR_CELL_H__
#define __MJOLNIR_CELL_H__

/* REFERENCE: http://www.evilscience.co.uk/a-c-algorithm-to-build-roguelike-cave-systems-part-1/ */

#define DNG_OBJMAPS 1

#include <mjolnir/conf.h>
#include <mjolnir/dng.h>
#if (DNG_RANDMT32)
#include <zero/randmt32.h>
#else
#include <stdlib.h>
#endif
#include <zero/trix.h>

#if (DNG_MJOLNIR)
#include <mjolnir/mjol.h>
#include <mjolnir/obj.h>
#endif

#define DNG_WIDTH     80
#define DNG_HEIGHT    24

/* define pseudo-random number interface */
#if (DNG_RANDMT32)
#define dngsrand(val) srandmt32(val)
#define dngrand()     (randmt32() & 0x7fffffff)
#else
#define dngsrand(val) srand(val)
#define dngrand()     (rand() & 0x7fffffff)
#endif

/* macro to calculate percentual probabilities */
static __inline__ long
dngprobpct(void)
{
    long val = dngrand();
    long ret = modu100(val);

    return ret;
}

/* directions for building and navigating dungeons */
#define DNG_NODIR     -1
#define DNG_NORTH      0
#define DNG_SOUTH      1
#define DNG_EAST       2
#define DNG_WEST       3
#define DNG_NDIR2      4
#define DNG_NORTHEAST  4
#define DNG_NORTHWEST  5
#define DNG_SOUTHWEST  6
#define DNG_SOUTHEAST  7
#define DNG_NDIR       8

/* value for uninitialised dungeon cells */
#define DNG_NOCAVE    -1L

struct cellcoord {
    long xval;          // X-coordinate
    long yval;          // Y-coordinate
};

struct cellcor {
    long              ncell;     // number of cells in corridor
    long              ncellmax;
    struct cellcoord *celltab;   // table of points (X,Y-coordinate pairs)
};

#define DNG_CAVE_CONNECTED (1 << 0)
struct cellcave {
    long              id;
//    long              flg;
    long              connid;
    long              nconn;
    long              ncell;
    long              ncellmax;
    struct cellcoord *celltab;
    long              nconnmax;
    unsigned char    *connmap;
};

/* user-settable cave parameter structure */
struct cellcaveparm {
    long rndval;        // random value
    long niter;         // # of times to visit cells
    long minsize;       // minimum # of cells in cave
    long maxsize;       // maximum # of cells in cave
    long closeprob;     // probability of closing a cell in %
    long nlimnbor;      // cells with <= this neighbors get closed
    long nrmnbor;       // cells with >= this empty neighbors get closed
    long nfillnbor;     // empty cells with >= this neighbors get opened
};

/* user-settable corridor parameter structure */
struct cellcorparm {
    long brkout;        // maximum # of tries to connect caves
    long spacing;       // minimum distance from a closed cell
    long minlen;        // minimum corridor length
    long maxlen;        // maximum corridor length
    long maxturn;       // maximum number of turns
};

#define CELL_GENPARM_INIT 0x00000001L
struct cellgenparm {
    long                flg;
    struct cellcaveparm caveparm;
    struct cellcorparm  corparm;
};

struct cellgenparm;
struct celldng {
    long              width;            // dungeon width
    long              height;           // dungeon height
    char             *map;              // dungeon cell-bitmap
    char             *cormap;           // dungeon corridor-bitmap
#if (DNG_MJOLNIR)
    struct mjolobj  **objmap;
#endif
    long              ncave;            // # of caves
    long              ncavemax;         // maximum # of (allocated) caves
    struct cellcave **cavetab;          // cave structures
    long             *caveidtab;        // map of cell-owner caves
    struct cellcave **conntab;          // connected cave structures
    long              ncor;             // # of corridors
    long              ncormax;          // maximum # of (allocated) corridors
    struct cellcor  **cortab;           // corridor structures
};

void cellsetdefparm(struct cellgenparm *parm);
void cellinitdng(struct celldng *dng, long width, long height);
void cellbuilddng(struct celldng *dng);

#if (DNG_MJOLNIR)
static __inline__ void
dngpushobj(struct celldng *dng, struct mjolobj *obj, long x, long y)
{
    if (obj) {
        obj->next = dng->objmap[y * dng->width + x];
        dng->objmap[y * dng->width + x] = obj;
    }

    return;
}

static __inline__ struct mjolobj *
dngpopobj(struct celldng *dng, long x, long y)
{
    struct mjolobj *obj = dng->objmap[y * dng->width + x];

    if (obj) {
        dng->objmap[y * dng->width + x] = obj->next;
    }

    return obj;
}

#endif /* DNG_MJOLNIR */

#endif /* __MJOLNIR_CELL_H__ */

