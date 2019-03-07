#include <stdlib.h>
#include <stdio.h>
#include <mjolnir/conf.h>
#include <mjolnir/api.h>
#include <mjolnir/cell.h>
#include <corewar/cw.h>
#include <corewar/zeus.h>

struct celldng     celldng;
struct cellgenparm genparm;
struct cwmars      cwmars;

static void
cellinitparm(struct cellgenparm *parm)
{
    cellsetdefparm(parm);
#if 0
    parm->caveparm.minsize = 64;
    parm->caveparm.maxsize = 1024;
#endif
    parm->caveparm.minsize = 16;
    parm->caveparm.maxsize = 1024;

//    parm->corparm.brkout = 1024 * 768;
//    parm->corparm.brkout = 10000;

    return;
}

#if 0
static void
cellplotdng(struct celldng *dng)
{
    /* plot caves */
    cellplotcave(dng);
    /* plot corridors */
    cellplotcor(dng);
}
#endif

int
main(int argc, char *argv[])
{
#if 0
    FILE *fp;
    long  base;
    long  lim;
    long  pc1;
    long  pc2;
#endif

    cellinitparm(&genparm);
#if (DNG_PIXELCELLS)
    cellinitdng(&celldng, 1024, 768);
#else
    cellinitdng(&celldng, 1024 / 8, 768 / 8);
#endif
    cellbuilddng(&celldng);
//    cellplotdng(&celldng);
    cellx11loop(&celldng);
#if 0
    if (argc != 3) {
        fprintf(stderr, "usage: %s prog1.rc prog2.rc\n", argv[0]);

        exit(1);
    }
    cwinit();
#if (CWRANDMT32)
    base = randmt32() % CWCORESIZE;
#else
    base = rand() % CWCORESIZE;
#endif
    fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "failed to open %s\n", argv[1]);

        exit(1);
    }
    pc1 = rcxlate(fp, 0, base, &base, &lim);
    if (pc1 < 0) {
        fprintf(stderr, "failed to translate %s\n", argv[1]);

        exit(1);
    }
    fclose(fp);
#if (CWRANDMT32)
    base = randmt32() % CWCORESIZE;
#else
    base = rand() % CWCORESIZE;
#endif
    fp = fopen(argv[2], "r");
    if (!fp) {
        fprintf(stderr, "failed to open %s\n", argv[2]);

        exit(1);
    }
    pc2 = rcxlate(fp, 1, base, &base, &lim);
    if (pc2 < 0) {
        fprintf(stderr, "failed to translate %s\n", argv[1]);

        exit(1);
    }
    fclose(fp);
    cwmars.proccnt[0] = 1;
    cwmars.proccnt[1] = 1;
    cwmars.curproc[0] = 0;
    cwmars.curproc[1] = 0;
    cwmars.nturn[0] = CWNTURN;
    cwmars.nturn[1] = CWNTURN;
    cwmars.runqtab[0][0] = pc1;
    cwmars.runqtab[1][0] = pc2;
#if (CWRANDMT32)
    cwmars.curpid = randmt32() & 0x01;
#else
    cwmars.curpid = rand() & 0x01;
#endif
#if (ZEUS)
    zeusdrawsim(&cwmars.zeusx11);
    while (1) {
        zeusprocev(&cwmars.zeusx11);
    }
#endif
#endif /* 0 */
    /* NOTREACHED */
    exit(0);
}

