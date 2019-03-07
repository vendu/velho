#include <mjolnir/conf.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#if (DNG_RANDMT32)
#include <zero/randmt32.h>
#endif
#include <zero/trix.h>
#include <mjolnir/cell.h>

/*
 * TODO
 * ----
 * - weed out circular connections to the same cave
 * - try to make every cave have at least N connections (2, 3?)
 */

/* internal function prototypes */
static long dngcountnbors1(struct celldng *dng, long x, long y, long lim);
static long dngcountnbors0(struct celldng *dng, long x, long y, long lim);
static void dnggencaves(struct celldng *dng);
static long dngfindcave(struct celldng *dng, long caveid, long x, long y);
static void dngbuildcaves(struct celldng *dng);
static void dngrmcave(struct celldng *dng, long caveid, long clear);
static long dngconncaves(struct celldng *dng);
static long dngturncor(long dir);
static long dngturncornorev(long dir1, long dir2);
static void dngfindedge(struct celldng *dng, long caveid,
                        long *retx, long *rety, long *retdir);
static long dngfindcoredge(struct celldng *dng,
                           long *retx, long *rety, long *retdir);
static long dngchkcorcell(struct celldng *dng,
                          long x, long y, long dir);
static struct cellcor * dngtrycor(struct celldng *dng,
                                  long caveid,
                                  long x, long y, long dir,
                                  long backtrack);

/* macros */
#define dngiscell(x, y, w, h)                                           \
    ((x) >= 0 && (x) < (w) && (y) >= 0 && (y) < h)
#define dnggetcellbit(dng, x, y)                                        \
    (bitset((dng)->map, (y) * (dng)->width + (x)))
#define dngsetcellbit(dng, x, y)                                        \
    (setbit((dng)->map, (y) * (dng)->width + x))
#define dngclrcellbit(dng, x, y)                                        \
    (clrbit((dng)->map, (y) * (dng)->width + x))
#define dnggetcorbit(dng, x, y)                                         \
    (bitset((dng)->cormap, (y) * (dng)->width + (x)))
#define dngsetcorbit(dng, x, y)                                         \
    (setbit((dng)->cormap, (y) * (dng)->width + x))
#define dngclrcorbit(dng, x, y)                                         \
    (clrbit((dng)->cormap, (y) * (dng)->width + x))
#define dngrevdir(dir) (dngrevdirtab[(dir)])
#define dngmovedir(x, y, dir)                                           \
    ((x) += dngdirofstab[(dir)].xval, (y) += dngdirofstab[(dir)].yval)
#define dngsetcaveid(dng, x, y, cid)                                    \
    ((dng)->caveidtab[(y) * (dng)->width + (x)] = (cid))
#define dnggetcaveid(dng, x, y)                                         \
    ((dng)->caveidtab[(y) * (dng)->width + (x)])

/* lookup tables */
static long dngdirtab[DNG_NDIR]
= {
    DNG_NORTH,
    DNG_SOUTH,
    DNG_EAST,
    DNG_WEST,
    DNG_NORTHEAST,
    DNG_NORTHWEST,
    DNG_SOUTHWEST,
    DNG_SOUTHEAST,
};
static long dngrevdirtab[DNG_NDIR]
= {
    DNG_SOUTH,
    DNG_NORTH,
    DNG_WEST,
    DNG_EAST,
    DNG_SOUTHWEST,
    DNG_SOUTHEAST,
    DNG_NORTHEAST,
    DNG_NORTHWEST
};
static struct cellcoord dngdirofstab[DNG_NDIR]
= {
    { 0, -1 },  // DNG_NORTH
    { 0, 1 },   // DNG_SOUTH
    { 1, 0 },   // DNG_EAST
    { -1, 0 },  // DNG_WEST
    { 1, -1 },  // DNG_NORTHEAST
    { -1, -1 }, // DNG_NORTHWEST
    { -1, -1 }, // DNG_SOUTHWEST
    { 1, 1 }    // DNG_SOUTHEAST
};

/* global generator parameters */
static struct cellgenparm  *genparm;
static struct cellcave    **dngcavetab;

void
cellsetdefparm(struct cellgenparm *parm)
{
    parm->caveparm.rndval = dngrand();
#if (DNG_PIXELCELLS)
    parm->caveparm.niter = 500000;
#else
    parm->caveparm.niter = 10000;
#endif
//    parm->caveparm.closeprob = 45;
    parm->caveparm.closeprob = 45;
    parm->caveparm.nlimnbor = 4;
    parm->caveparm.nrmnbor = 3;
    parm->caveparm.nfillnbor = 2;
    genparm = parm;
}

void
cellsetgenparm(struct celldng *dng, struct cellgenparm *parm)
{
    if (!parm) {
        /* invoke global default configuration if one not supplied */
        parm = genparm;
    } else {
        genparm = parm;
    }
    /* set cave parameters */
    parm->caveparm.minsize = 16;
    parm->caveparm.maxsize = 500;
    /* set corridor parameters */
    parm->corparm.brkout = 10000;
    parm->corparm.spacing = 5;
    parm->corparm.minlen = 2;
    parm->corparm.maxlen = max(dng->width, dng->height);
    parm->corparm.maxturn = 8;
    parm->flg |= CELL_GENPARM_INIT;

    return;
}

/* initialise dungeon generator */
void
cellinitdng(struct celldng *dng, long width, long height)
{
    long              num = width * height;
    long              ncavemax = 16;
    long              ncormax = 16;
    long              n = roundup2(num, CHAR_BIT);
    char             *map = calloc(n / CHAR_BIT, sizeof(char));
    char             *cormap = calloc(n / CHAR_BIT, sizeof(char));
    struct cellcave **cavetab = calloc(ncavemax, sizeof(struct cellcave *));
    long             *idtab = malloc(num * sizeof(long));
    struct cellcor  **cortab = calloc(ncormax, sizeof(struct cellcor *));
    long              ndx;

    dngcavetab = calloc(width * height, sizeof(struct cellcave **));
    if (!dngcavetab) {
        fprintf(stderr, "CELL: failed to allocate global cave table\n");

        exit(1);
    }
    if (!map) {
        fprintf(stderr, "CELL: failed to allocate cave bitmap\n");

        exit(1);
    }
    if (!cormap) {
        fprintf(stderr, "CELL: failed to allocate corridor bitmap\n");

        exit(1);
    }
    if (!cavetab) {
        fprintf(stderr, "CELL: failed to allocate cave table\n");

        exit(1);
    }
    if (!idtab) {
        fprintf(stderr, "CELL: failed to allocate cave ID table\n");

        exit(1);
    }
    /* set random seed */
//    dngsrand(~0L);
    dngsrand(0x55555555L);
    /* set cell owner-IDs (caves) to uninitialised */
    for (ndx = 0 ; ndx < num ; ndx++) {
        idtab[ndx] = DNG_NOCAVE;
    }
    dng->map = map;
    dng->cormap = cormap;
    dng->caveidtab = idtab;
    /* initialise dungeon structure */
    dng->width = width;
    dng->height = height;
//    dng->map = map;
    dng->ncormax = ncormax;
    dng->cortab = cortab;
    dng->ncavemax = ncavemax;
    dng->cavetab = cavetab;
    cellsetgenparm(dng, NULL);

    return;
}

/* API function for building a dungeon */
void
cellbuilddng(struct celldng *dng)
{
    dnggencaves(dng);
    dngbuildcaves(dng);
    dngconncaves(dng);

    return;
}

/* count surrounding populated neighbors of the cell at (x, y) */
static long
dngcountnbors1(struct celldng *dng, long x, long y, long lim)
{
    long              cnt = 0;
    struct cellcoord *cell;
    long              dir;
    long              w = dng->width;
    long              h = dng->height;
    long              x1;
    long              y1;

    cell = dngdirofstab;
    for (dir = 0 ; dir < lim ; dir++) {
        x1 = x + cell->xval;
        y1 = y + cell->yval;
        if (dngiscell(x1, y1, w, h)
            && dnggetcellbit(dng, x1, y1)) {
            cnt++;
        }
        cell++;
    }

    return cnt;
}

/* count surrounding empty neighbors of the cell at (x, y) */
static long
dngcountnbors0(struct celldng *dng, long x, long y, long lim)
{
    long              cnt = 0;
    struct cellcoord *cell;
    long              dir;
    long              w = dng->width;
    long              h = dng->height;
    long              x1;
    long              y1;

    cell = dngdirofstab;
    for (dir = 0 ; dir < lim ; dir++) {
        x1 = x + cell->xval;
        y1 = y + cell->yval;
        if (dngiscell(x1, y1, w, h)
            && !dnggetcellbit(dng, x1, y1)) {
            cnt++;
        }
        cell++;
    }

    return cnt;
}

/* generate a cave */
static void
dnggencaves(struct celldng *dng)
{
    long             w = dng->width;
    long             h = dng->height;
//    char            *map = calloc(h, w / CHAR_BIT);
    char            *map = dng->map;
    long             closeprob = genparm->caveparm.closeprob;
    long             ndx;
    long             n = w * h;
    long             x;
    long             y;
    long             lim;

    /* initialise cell-bitmap */
    for (ndx = 0; ndx < n ; ndx++) {
        if (dngprobpct() < closeprob) {
            /* close a cell (set bit to 1) */
            setbit(map, ndx);
        }
    }
    /*
     * iterate over the map, picking cells at random; if the cell has > lim
     * neighbors close it, otherwise open it
     */
    n = genparm->caveparm.niter;
    lim = genparm->caveparm.nlimnbor;
    for (ndx = 0 ; ndx < n ; ndx++) {
        x = dngrand() % w;
        y = dngrand() % h;
        if (dngcountnbors1(dng, x, y, DNG_NDIR) > lim) {
            setbit(map, y * w + x);
        } else {
            clrbit(map, y * w + x);
        }
    }
    /*
     * smooth cave edges and single blocks by removing cells with
     * >= lim empty neighbors
     */
    lim = genparm->caveparm.nrmnbor;
//    for (ndx = 0 ; ndx < 5 ; ndx++) {
    for (ndx = 0 ; ndx < 3 ; ndx++) {
        for (y = 0 ; y < h ; y++) {
            for (x = 0 ; x < w ; x++) {
                if (dnggetcellbit(dng, x, y)
                    && dngcountnbors0(dng, x, y, DNG_NDIR2) >= lim) {
                    clrbit(map, y * w + x);
                }
            }
        }
    }
    /* fill in empty cells with at least lim neighbors to get rid of holes */
    lim = genparm->caveparm.nfillnbor;
    for (y = 0 ; y < h ; y++) {
        for (x = 0 ; x < w ; x++) {
            if (!dnggetcellbit(dng, x, y)
                && dngcountnbors1(dng, x, y, DNG_NDIR2) >= lim) {
                setbit(map, y * w + x);
            }
        }
    }

    return;
}

/*
 * locate all caves within a map
 * NOTE: this function calls itself recursively
 */
static long
dngfindcave(struct celldng *dng, long caveid, long x, long y)
{
    long              ret;
    long              dir = DNG_NODIR;
    long              id = (caveid != DNG_NOCAVE) ? caveid : dng->ncave;
    struct cellcave  *cave = (caveid != DNG_NOCAVE) ? dng->cavetab[caveid] : NULL;
    long              ncavemax = dng->ncavemax;
    long              recur = 1;
    long              w = dng->width;
    long              h = dng->height;
    long              ncellmax = (cave) ? cave->ncellmax : 16;
    struct cellcoord *celltab;
    long              x1;
    long              y1;
    long              ndx;
    long              lim;

    if (caveid == DNG_NOCAVE) {
        cave = calloc(1, sizeof(struct cellcave));
        if (!cave) {
            fprintf(stderr, "CELL: failed to allocate cave\n");

            exit(1);
        }
        celltab = calloc(ncellmax, sizeof(struct cellcoord));
        if (!celltab) {
            fprintf(stderr, "CELL: failed to allocate cell table\n");

            exit(1);
        }
        cave->ncellmax = ncellmax;
        cave->celltab = celltab;
        if (dng->ncave == ncavemax) {
            ncavemax <<= 1;
            dng->cavetab = realloc(dng->cavetab,
                                   ncavemax * sizeof(struct cellcave **));
            if (!dng->cavetab) {
                fprintf(stderr, "CELL: failed to allocate cave table\n");

                exit(1);
            }
            dng->ncavemax = ncavemax;
        }
        dng->cavetab[id] = cave;
//        dng->ncave++;
//        dng->ncave = id + 1;
        recur = 0;
    }
    lim = DNG_NDIR2;
    cave = dng->cavetab[id];
    celltab = cave->celltab;
    for (ndx =  0 ; ndx < lim ; ndx++) {
        x1 = x;
        y1 = y;
        dir = dngdirtab[ndx];
        dngmovedir(x1, y1, dir);
        if (dngiscell(x1, y1, w, h)
            && dnggetcellbit(dng, x1, y1)
            && dnggetcaveid(dng, x1, y1) == DNG_NOCAVE) {
//            && !dnggetcellbit(dng, x1, y1)) {
            dngsetcellbit(dng, x1, y1);
            dngsetcaveid(dng, x1, y1, id);
            if (cave->ncell == ncellmax) {
                ncellmax <<= 1;
                cave->celltab = realloc(cave->celltab,
                                        ncellmax * sizeof(struct cellcoord));
                if (!cave->celltab) {
                    fprintf(stderr, "CELL: failed to reallocate cell table\n");

                    exit(1);
                }
                cave->ncellmax = ncellmax;
            }
            cave->celltab[cave->ncell].xval = x1;
            cave->celltab[cave->ncell].yval = y1;
            cave->ncell++;
            dngfindcave(dng, id, x1, y1);
        }
    }
    ret = id;
    if (!recur) {
        if (!cave->ncell) {
            free(cave->celltab);
            free(cave);

            ret = DNG_NOCAVE;
        } else {
            dng->ncave++;
        }
    } else {

        ret = DNG_NOCAVE;
    }

    return ret;
}

static void
dngbuildcaves(struct celldng *dng)
{
    struct cellcave *cave;
    long             w = dng->width;
    long             h = dng->height;
    long             min = genparm->caveparm.minsize;
    long             max = genparm->caveparm.maxsize;
    long             x;
    long             y;
    long             id = DNG_NOCAVE;
    long             ncell;

    for (y = 0 ; y < h ; y++) {
        for (x = 0 ; x < w ; x++) {
            if (dnggetcellbit(dng, x, y)
                && (dnggetcaveid(dng, x, y) == DNG_NOCAVE)) {
//                dngsetcaveid(dng, x, y, id);
                id = dngfindcave(dng, DNG_NOCAVE, x, y);
                if (id != DNG_NOCAVE) {
                    cave = dng->cavetab[id];
                    ncell = cave->ncell;
                    if ((ncell <= min || ncell > max) && id != DNG_NOCAVE) {
                        dngrmcave(dng, id, 1);
                    }
                }
            }
        }
    }

    return;
}

static void
dngrmcave(struct celldng *dng, long caveid, long clear)
{
    struct cellcave  *cave = dng->cavetab[caveid];
    struct cellcoord *cell;
    long              x;
    long              y;
    long              ndx1;
    long              ndx2;
    long              lim;

    if (clear) {
        lim = cave->ncell;
        cell = cave->celltab;
        for (ndx1 = 0 ; ndx1 < lim ; ndx1++) {
            x = cell->xval;
            y = cell->yval;
            dngclrcellbit(dng, x, y);
            dngsetcaveid(dng, x, y, DNG_NOCAVE);
            cell++;
        }
    }
    ndx1 = caveid;
    for (ndx2 = ndx1 + 1 ; ndx2 < dng->ncave - 1; ndx2++) {
        dng->cavetab[ndx1] = dng->cavetab[ndx2];
        ndx1++;
    }
    dng->cavetab[ndx2] = NULL;
    dng->ncave--;

    return;
}

static long
dngconncaves(struct celldng *dng)
{
    long               w;
    long               h;
    long               ncave = dng->ncave;
    long               ncor = 0;
    long               dir = DNG_NODIR;
    struct cellcor    *cor;
    long               nconn = 0;
    long               nconnmax = ncave;
    struct cellcoord  *coord1;
    struct cellcave  **conntab = calloc(nconnmax, sizeof(struct cellcave *));
    struct cellcave   *cave;
    struct cellcave   *dest;
    long               id = 0;
    long               corx;
    long               cory;
    long               x1;
    long               y1;
    long               num;
    long               ndx;
    long               lim;
    long               ncell;
    long               brkcnt;

    if (ncave) {
        id = dngrand() % ncave;
    } else {

        return 0;
    }
    cave = dng->cavetab[id];
    w = dng->width;
    h = dng->height;
    conntab[nconn] = cave;
    nconn++;
    lim = genparm->corparm.brkout;
    brkcnt = 0;
    do {
        fprintf(stderr, "NCAVE: %ld\n", ncave);
        if ((!ncor
             || dngprobpct() > 50)
            && (nconn)) {
            ndx = dngrand() % nconn;
            cave = conntab[ndx];
            id = cave->id;
            dngfindedge(dng, id, &corx, &cory, &dir);
        } else {
            cave = NULL;
//            id = DNG_NOCAVE;
            id = dngfindcoredge(dng, &corx, &cory, &dir);
        }
        cor = dngtrycor(dng, id, corx, cory, dir, 0);
        if (cor) {
            ncave = dng->ncave;
            for (ndx = 0 ; ndx < ncave ; ndx++) {
//                cave = dng->cavetab[ndx];
                ncell = cor->ncell;
                x1 = cor->celltab[ncell - 1].xval;
                y1 = cor->celltab[ncell - 1].yval;
                if (dngiscell(x1, y1, w, h)) {
                    id = dnggetcaveid(dng, x1, y1);
                    if (id == cave->id) {
                        free(cor);

                        continue;
                    }
                    dest = dngcavetab[id];
                    coord1 = cor->celltab;
//                        ncell--;
                    for (num = 0 ; num < ncell ; num++) {
                        x1 = coord1->xval;
                        y1 = coord1->yval;
//                           dngsetcellbit(dng, x1, y1);
                        dngsetcorbit(dng, x1, y1);
                        coord1++;
                    }
                    cave->nconn++;
                    if (cave->nconn > 2) {
                        if (nconn == nconnmax) {
                            nconnmax <<= 1;
                            conntab = realloc(conntab,
                                              nconnmax * sizeof(struct cellcave *));
                            if (!conntab) {
                                fprintf(stderr,
                                        "CELL: failed to allocate connection table\n");

                                exit(1);
                            }
                        }
                        conntab[nconn] = cave;
                        nconn++;
                        dngrmcave(dng, ndx, 0);
                        ncave--;

                        break;
                    }
                    if (dest) {
                        if (dest->nconn > 2) {
                            if (nconn == nconnmax) {
                                nconnmax <<= 1;
                                conntab = realloc(conntab,
                                                  nconnmax * sizeof(struct cellcave *));
                                if (!conntab) {
                                    fprintf(stderr,
                                            "CELL: failed to allocate connection table\n");

                                    exit(1);
                                }
                            }
                            conntab[nconn] = dest;
                            nconn++;
                            dngrmcave(dng, ndx, 0);
                            ncave--;
                        }
                    } else {
                        free(cor);

                        continue;
                    }
                }
            }
            brkcnt++;
            if (brkcnt >= lim) {
                free(cor);

                return 0;
            }
        }
    } while (ncave);
    free(dng->cavetab);
    dng->ncave = nconn;
    dng->ncavemax = nconnmax;
    dng->cavetab = conntab;
    dng->ncor = ncor;

    return 1;
}

static long
dngturncor(long dir)
{
    long ret;

    do {
        ret = dngdirtab[dngrand() % DNG_NDIR2];
    } while (ret == dngrevdir(dir));

    return ret;
}

static long
dngturncornorev(long dir1, long dir2)
{
    long ret;

    do {
        ret = dngdirtab[dngrand() % DNG_NDIR2];
    } while (ret == dngrevdir(dir1) || ret == dir2);

    return ret;
}

static void
dngfindedge(struct celldng *dng, long caveid,
            long *retx, long *rety, long *retdir)
{
    struct cellcave  *cave = dng->cavetab[caveid];
    struct cellcoord *cell;
    long              w = dng->width;
    long              h = dng->height;
    long              x1;
    long              y1;
    long              dir = *retdir;
    long              ndx;

    if (dir == DNG_NODIR) {
        dir = dngrand() % DNG_NDIR;
    }
    if (cave->ncell) {
        do {
            ndx = dngrand() % cave->ncell;
            cell = &cave->celltab[ndx];
            x1 = cell->xval;
            y1 = cell->yval;
            dir = dngturncor(dir);
            do {
                dngmovedir(x1, y1, dir);
                if (!dngiscell(x1, y1, w, h)) {

                    break;
                } else if (!dnggetcellbit(dng, x1, y1)) {
                    *retx = x1;
                    *rety = y1;
                    *retdir = dir;

                    return;
                }
            } while (1);
        } while (1);
    }

    return;
}

static long
dngfindcoredge(struct celldng *dng,
               long *retx, long *rety, long *retdir)
{
    struct cellcor    *cor;
    long               ncor = dng->ncor;
    struct cellcoord  *ofs;
    long               x;
    long               y;
    long               dir = *retdir;
    long               w = dng->width;
    long               h = dng->height;
    long               id;
    long               x1;
    long               y1;
    long               ndx;
    long               val;
    long               ndir = 0;
    long               dirstk[DNG_NDIR];

    if (ncor) {
        do {
            ndx = dngrand() % ncor;
            cor = dng->cortab[ndx];
            val = dngrand() % cor->ncell;
            x = cor->celltab[val].xval;
            y = cor->celltab[val].yval;
            for (ndx = 0 ; ndx < DNG_NDIR ; ndx++) {
                ofs = &dngdirofstab[ndx];
                x1 = x + ofs->xval;
                y1 = y + ofs->yval;
                id = dnggetcaveid(dng, x1, y1);
                if (dngiscell(x1, y1, w, h)
                    && !dnggetcellbit(dng, x1, y1)) {
                    dir = dngdirtab[ndx];
                    dirstk[ndir] = dir;
                    ndir++;
                }
            }
        } while (!ndir);
        ndx = dngrand() % ndir;
        *retdir = dirstk[ndx];
        *retx = x1;
        *rety = y1;
    }

    return id;
}

static long
dngchkcorcell(struct celldng *dng, long x, long y, long dir)
{
    long             xofs = dngdirofstab[dir].xval;
    long             yofs = dngdirofstab[dir].yval;
    long             space = genparm->corparm.spacing;
    long             w = dng->width;
    long             h = dng->height;
    long             x1;
    long             y1;
    long             ofs;

    for (ofs = -space ; ofs <= space ; ofs++) {
        if (xofs == 0) {
            x1 = x + space;
            if (dngiscell(x1, y, w, h)
                && dnggetcellbit(dng, x1, y)) {

                return 0;
            }
        } else if (yofs == 0) {
            y1 = y + space;
            if (dngiscell(x, y1, w, h)
                && dnggetcellbit(dng, x, y1)) {

                return 0;
            }
        }
    }

    return 1;
}

static struct cellcor *
dngtrycor(struct celldng *dng,
          long caveid,
          long x, long y,
          long dir, long backtrack)
{
    long              min = genparm->corparm.minlen;
    long              max = genparm->corparm.maxlen;
    long              nturn = genparm->corparm.maxturn;
    struct cellcor   *cor = calloc(1, sizeof(struct cellcor));
//    long              ncor = dng->ncor;
//    long              ncormax = dng->ncormax;
    long              ncell = 0;
    long              ncellmax;
    struct cellcoord *celltab;
    struct cellcoord *coord;
    long              w = dng->width;
    long              h = dng->height;
    long              id;
    long              dir1;
    long              corx;
    long              cory;
    long              len;

    if (!cor) {
        fprintf(stderr, "CELL: failed to allocate corridor\n");

        exit(1);
    }
    if (caveid == DNG_NOCAVE) {
        caveid = dng->ncave;
    }
    dir1 = dir;
    ncellmax = 16;
    celltab = calloc(ncellmax, sizeof(struct cellcoord));
    if (!celltab) {
        fprintf(stderr, "CELL: failed to allocate corridor point table\n");

        exit(1);
    }
    cor->ncellmax = ncellmax;
    cor->celltab = celltab;
    coord = celltab;
    coord->xval = x;
    coord->yval = y;
    ncell = 1;
    coord++;
    corx = x;
    cory = y;
    while (nturn >= 0) {
        len = min + (dngrand() % (max - min + 1));
        while (len) {
            len--;
            corx = x + dngdirofstab[dir].xval;
            cory = y + dngdirofstab[dir].yval;
            x = corx;
            y = cory;
            if (dngiscell(corx, cory, w, h)) {
                id = dnggetcaveid(dng, corx, cory);
                if (id != DNG_NOCAVE && id != caveid) {
                    if (ncell > 1) {
                        if (ncell == ncellmax) {
                            ncellmax <<= 1;
                            celltab = realloc(cor->celltab,
                                              ncellmax * sizeof(struct cellcor));
                            if (!celltab) {
                                fprintf(stderr, "CELL: failed to reallocate corridor cell table\n");

                                exit(1);
                            }
                            cor->ncellmax = ncellmax;
                            cor->celltab = celltab;
                            coord = &celltab[ncell];
                        }
                        coord->xval = corx;
                        coord->yval = cory;
                        ncell++;
                        cor->ncell = ncell;

                        return cor;
                    } else {
                        free(cor);
                        free(celltab);

                        return NULL;
                    }
                } else if (id == caveid) {
                    free(cor);
                    free(celltab);

                    return NULL;
                } else if (!dngiscell(corx, cory, w, h)
                           || !dngchkcorcell(dng, corx, corx, dir)) {
                    free(cor);
                    free(celltab);

                    return NULL;
                }
            } else {
                free(cor);
                free(celltab);

                return NULL;
            }
            if (ncell == ncellmax) {
                ncellmax <<= 1;
                celltab = realloc(celltab,
                                  ncellmax * sizeof(struct cellcoord));
                if (!celltab) {
                    fprintf(stderr, "CELL: failed to reallocate corridor point table\n");

                    exit(1);
                }
                cor->celltab = celltab;
                coord = &celltab[ncell];
            }
            coord->xval = corx;
            coord->yval = cory;
            ncell++;
            coord++;
        }
        nturn--;
        if (nturn > 1) {
            if (backtrack) {
                dir = dngturncor(dir);
            } else {
                dir = dngturncornorev(dir, dir1);
            }
        }
    }
    free(cor);
    free(celltab);

    return NULL;
}

