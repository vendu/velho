/* REFERENCE:  http://corewar.co.uk/cwg.txt */
/* REFERENCE: http://seblog.cs.uni-kassel.de/fileadmin/se/courses/SE1/WS0708/redcode-icws-88-2.pdf */

#include <corewar/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#if defined(CWRANDMT32)
#include <zero/randmt32.h>
#endif
#include <corewar/cw.h>
#include <corewar/rc.h>
#if defined(ZEUS)
#include <unistd.h>
#include <corewar/zeus.h>
#endif

extern long    rcnargtab[CWNOP];

struct cwmars  cwmars ALIGNED(PAGESIZE);        // virtual machine structure
const char    *cwopnametab[CWNOP]               // instruction name table
= {
    "DAT",
    "MOV",
    "ADD",
    "SUB",
    "JMP",
    "JMZ",
    "JMN",
    "CMP",
    "SLT",
    "DJN",
    "SPL",
};

/* disassembe instruction */
void
cwdisasm(struct cwinstr *op, FILE *fp)
{
    char ch;

    if (op) {
        fprintf(fp, "\t%s\t", cwopnametab[op->op]);
        if  (rcnargtab[op->op] == 2) {
            ch = '\0';
            if (op->aflg & CWIMMBIT) {
                ch = '#';
            } else if (op->aflg & CWINDIRBIT) {
                ch = '@';
            } else if (op->aflg & CWPREDECBIT) {
                ch = '<';
            }
            if (ch) {
                fprintf(fp, "%c", ch);
            }
            if (op->aflg & CWSIGNBIT) {
                fprintf(fp, "%d", op->a - CWCORESIZE);
            } else {
                fprintf(fp, "%d", op->a);
            }
        }
        ch = '\0';
        if (op->bflg & CWIMMBIT) {
            ch = '#';
        } else if (op->bflg & CWINDIRBIT) {
            ch = '@';
        } else if (op->aflg & CWPREDECBIT) {
            ch = '<';
        }
        if (ch) {
            fprintf(fp, "\t%c", ch);
        } else {
                fprintf(fp, "\t");
        }
        if (op->bflg & CWSIGNBIT) {
            fprintf(fp, "%d\n", op->b - CWCORESIZE);
        } else {
            fprintf(stderr, "%d\n", op->b);
        }
    }

    return;
}

/* read instruction operands */
static void
cwgetargs(struct cwinstr *op, long pc, long *argp1, long *argp2)
{
    long arg1 = 0;
    long arg2 = 0;
    long tmp;

    if (op->aflg & CWIMMBIT) {
        arg1 = op->a;
    } else {
        tmp = pc + op->a;
        tmp %= CWCORESIZE;
        if (op->aflg & (CWINDIRBIT | CWPREDECBIT)) {
            struct cwinstr *ptr;

            ptr = &cwmars.optab[tmp];
            tmp = ptr->b;
            if (op->aflg & CWPREDECBIT) {
                tmp--;
                ptr->b = tmp;
            }
            arg1 = tmp;
            arg1 += pc;
        } else {
            arg1 = tmp;
        }
    }
    arg1 %= CWCORESIZE;
    if (op->bflg & CWIMMBIT) {
        arg2 = op->b;
    } else {
        tmp = pc + op->b;
        tmp %= CWCORESIZE;
        if (op->bflg & (CWINDIRBIT | CWPREDECBIT)) {
            struct cwinstr *ptr;

            ptr = &cwmars.optab[tmp];
            tmp = ptr->b;
            if (op->bflg & CWPREDECBIT) {
                tmp--;
                ptr->b = tmp;
            }
            arg2 = tmp;
            arg2 += pc;
        } else {
            arg2 = tmp;
        }
    }
    arg2 %= CWCORESIZE;
    *argp1 = arg1;
    *argp2 = arg2;

    return;
}

/* instruction handler for DAT */
static long
cwdatop(long pid, long pc)
{
#if defined(ZEUS)
    zeusdrawsim(&cwmars.zeusx11);
#endif
    if (!pid) {
        fprintf(stderr, "program #2 (%s) won (%ld)\n", cwmars.prog2name, pc);
    } else {
        fprintf(stderr, "program #1 (%s) won (%ld)\n", cwmars.prog1name, pc);
    }
#if defined(ZEUS)
    sleep(5);
#endif
    exit(0);

    /* NOTREACHED */
    return CWNONE;
}

/* instruction handler for MOV */
static long
cwmovop(long pid, long pc)
{
    struct cwinstr *op = &cwmars.optab[pc];
    long            arg1;
    long            arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    if (op->aflg & CWIMMBIT) {
        if (op->bflg & CWIMMBIT) {
            cwmars.optab[arg2] = cwmars.optab[arg1];
        } else {
            cwmars.optab[arg2].b = arg1;
        }
    } else {
        cwmars.optab[arg2] = cwmars.optab[arg1];
    }
    pc++;
    pc %= CWCORESIZE;

    return pc;
}

/* instruction handler for ADD */
static long
cwaddop(long pid, long pc)
{
    struct cwinstr *op = &cwmars.optab[pc];
    long            arg1;
    long            arg2;
    long            a;
    long            b;

    cwgetargs(op, pc, &arg1, &arg2);
    if (op->aflg & CWIMMBIT) {
        a = arg1;
        if (op->bflg & CWIMMBIT) {
            b = arg2;
        } else {
            b = cwmars.optab[arg2].b;
        }
        b += a;
        b %= CWCORESIZE;
        if (op->bflg & CWIMMBIT) {
            op->bflg &= ~CWSIGNBIT;
            op->b = b;
        } else {
            cwmars.optab[arg2].bflg &= ~CWSIGNBIT;
            cwmars.optab[arg2].b = b;
        }
    } else if (op->bflg & CWIMMBIT) {
        a = arg1;
        b = arg2;
        b += a;
        b %= CWCORESIZE;
        op->b = b;
    } else {
        a = cwmars.optab[arg1].a;
        b = cwmars.optab[arg1].b;
        a += cwmars.optab[arg2].a;
        b += cwmars.optab[arg2].b;
        a %= CWCORESIZE;
        b %= CWCORESIZE;
        cwmars.optab[arg2].a = a;
        cwmars.optab[arg2].b = b;
    }
    pc++;
    pc %= CWCORESIZE;

    return pc;
}

/* instruction handler for SUB */
static long
cwsubop(long pid, long pc)
{
    struct cwinstr *op = &cwmars.optab[pc];
    long            arg1;
    long            arg2;
    long            a;
    long            b;

    cwgetargs(op, pc, &arg1, &arg2);
    if (op->aflg & CWIMMBIT) {
        a = arg1;
        b = cwmars.optab[arg2].b;
        b -= a;
        if (b < 0) {
            b += CWCORESIZE;
        }
        cwmars.optab[arg2].b = b;
    } else {
        a = cwmars.optab[arg1].a;
        b = cwmars.optab[arg1].b;
        a -= cwmars.optab[arg2].a;
        b -= cwmars.optab[arg2].b;
        if (a < 0) {
            a += CWCORESIZE;
        }
        if (b < 0) {
            b += CWCORESIZE;
        }
        cwmars.optab[arg2].a = a;
        cwmars.optab[arg2].b = b;
    }
    pc++;
    pc %= CWCORESIZE;

    return pc;
}

/* instruction handler for JMP */
static long
cwjmpop(long pid, long pc)
{
    struct cwinstr *op = &cwmars.optab[pc];
    long            cnt;
    long            arg1;
    long            arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    cnt = cwmars.proccnt[pid];
    if (cnt < CWNPROC) {
        pc = arg2;
        cwmars.runqtab[pid][cnt - 1] = pc;
    }

    return pc;
}

/* instruction handler for JMZ */
static long
cwjmzop(long pid, long pc)
{
    struct cwinstr *op = &cwmars.optab[pc];
    long            cnt;
    long            arg1;
    long            arg2;
    long            b;

    cwgetargs(op, pc, &arg1, &arg2);
    b = cwmars.optab[arg2].b;
    if (!b) {
        cnt = cwmars.proccnt[pid];
        pc = arg1;
        cwmars.runqtab[pid][cnt - 1] = pc;
    } else {
        pc++;
        pc %= CWCORESIZE;
    }

    return pc;
}

/* instruction handler for JMN */
static long
cwjmnop(long pid, long pc)
{
    struct cwinstr *op = &cwmars.optab[pc];
    long            cnt;
    long            arg1;
    long            arg2;
    long            b;

    cwgetargs(op, pc, &arg1, &arg2);
    b = cwmars.optab[arg2].b;
    if (b) {
        cnt = cwmars.proccnt[pid];
        pc = arg1;
        cwmars.runqtab[pid][cnt - 1] = pc;
    } else {
        pc++;
        pc %= CWCORESIZE;
    }

    return pc;
}

/* instruction handler for CMP */
static long
cwcmpop(long pid, long pc)
{
    struct cwinstr *op = &cwmars.optab[pc];
    long            arg1;
    long            arg2;
    long            a;
    long            b;

    cwgetargs(op, pc, &arg1, &arg2);
    if (op->aflg & CWIMMBIT) {
        b = cwmars.optab[arg2].b;
        if (arg1 == b) {
            pc++;
        }
    } else {
        a = arg1;
        b = arg2;
        if (cwmars.optab[a].a == cwmars.optab[b].a && cwmars.optab[a].b == cwmars.optab[b].b) {
            pc++;
        }
    }
    pc++;
    pc %= CWCORESIZE;

    return pc;
}

/* instruction handler for SLT */
static long
cwsltop(long pid, long pc)
{
    struct cwinstr *op = &cwmars.optab[pc];
    long            arg1;
    long            arg2;
    long            b;

    cwgetargs(op, pc, &arg1, &arg2);
    b = cwmars.optab[arg2].b;
    if (op->aflg & CWIMMBIT) {
        if (arg1 < b) {
            pc++;
        }
    } else if (cwmars.optab[arg2].b < b) {
        pc++;
    }
    pc++;
    pc %= CWCORESIZE;

    return pc;
}

/* instruction handler for DJN */
static long
cwdjnop(long pid, long pc)
{
    struct cwinstr *op = &cwmars.optab[pc];
    long            cnt;
    long            arg1;
    long            arg2;
    long            b;

    cwgetargs(op, pc, &arg1, &arg2);
    if (op->bflg & CWIMMBIT) {
        b = cwmars.optab[arg1].b;
        b--;
        if (b < 0) {
            b += CWCORESIZE;
        }
        cwmars.optab[arg1].b = b;
    } else {
        b = cwmars.optab[arg2].b;
        b--;
        if (b < 0) {
            b += CWCORESIZE;
        }
        cwmars.optab[arg2].b = b;
    }
    if (b) {
        cnt = cwmars.proccnt[pid];
        pc = arg1;
        cwmars.runqtab[pid][cnt - 1] = pc;
    }

    return pc;
}

/* instruction handler for SPL */
static long
cwsplop(long pid, long pc)
{
    struct cwinstr *op = &cwmars.optab[pc];
    long            cnt;
    long            cur;
    long            arg1;
    long            arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    pc++;
    pc %= CWCORESIZE;
    cnt = cwmars.proccnt[pid];
    cur = cwmars.curproc[pid];
    cwmars.runqtab[pid][cur] = pc;
    if (cnt < CWNPROC) {
        cwmars.runqtab[pid][cnt] = arg2;
        cnt++;
        cwmars.proccnt[pid] = cnt;
    }

    return pc;
}

/* initialise instruction handling */
static void
cwinitop(void)
{
    cwmars.opnames = cwopnametab;
    cwmars.functab[CWOPDAT] = cwdatop;
    cwmars.functab[CWOPMOV] = cwmovop;
    cwmars.functab[CWOPADD] = cwaddop;
    cwmars.functab[CWOPSUB] = cwsubop;
    cwmars.functab[CWOPJMP] = cwjmpop;
    cwmars.functab[CWOPJMZ] = cwjmzop;
    cwmars.functab[CWOPJMN] = cwjmnop;
    cwmars.functab[CWOPCMP] = cwcmpop;
    cwmars.functab[CWOPSLT] = cwsltop;
    cwmars.functab[CWOPDJN] = cwdjnop;
    cwmars.functab[CWOPSPL] = cwsplop;
    rcaddop("DAT", CWOPDAT);
    rcaddop("MOV", CWOPMOV);
    rcaddop("ADD", CWOPADD);
    rcaddop("SUB", CWOPSUB);
    rcaddop("JMP", CWOPJMP);
    rcaddop("JMZ", CWOPJMZ);
    rcaddop("JMN", CWOPJMN);
    rcaddop("CMP", CWOPCMP);
    rcaddop("SLT", CWOPSLT);
    rcaddop("DJN", CWOPDJN);
    rcaddop("SPL", CWOPSPL);
}

/* execute program pid for one instruction */
void
cwexec(long pid)
{
    struct cwinstr *op;
    cwinstrfunc    *func;
    long            cur;
    long            cnt;
    long            pc;
    long            l;
#if defined(ZEUS)
    static long     ref = 0;
#endif

#if defined(ZEUS)
    while (XEventsQueued(cwmars.zeusx11.disp, QueuedAfterFlush)) {
        zeusprocev(&cwmars.zeusx11);
    }
#endif
    cur = cwmars.curproc[pid];
    pc = cwmars.runqtab[pid][cur];
    op = &cwmars.optab[pc];
#if defined(ZEUS) && 0
    fprintf(stderr, "%ld\t%ld\t", pid, pc);
    cwdisasm(op, stderr);
#endif
    if (!(*((uint64_t *)op))) {
#if defined(ZEUS)
        zeusdrawsim(&cwmars.zeusx11);
#endif
        if (!pid) {
            fprintf(stderr, "program #2 (%s) won (%ld)\n", cwmars.prog2name, pc);
        } else {
            fprintf(stderr, "program #1 (%s) won (%ld)\n", cwmars.prog2name, pc);
        }
#if defined(ZEUS)
        sleep(5);
#endif

        exit(0);
    }
    func = cwmars.functab[op->op];
    pc = func(pid, pc);
    cnt = cwmars.proccnt[pid];
    if (pc == CWNONE) {
        if (cnt > 1) {
            for (l = cur ; l < cnt - 1 ; l++) {
                cwmars.runqtab[pid][l] = cwmars.runqtab[pid][l + 1];
            }
        } else {
#if defined(ZEUS)
            zeusdrawsim(&cwmars.zeusx11);
#endif
        if (!pid) {
            fprintf(stderr, "program #2 (%s) won (%ld)\n", cwmars.prog2name, pc);
        } else {
            fprintf(stderr, "program #1 (%s) won (%ld)\n", cwmars.prog2name, pc);
        }
#if defined(ZEUS)
            sleep(5);
#endif

            exit(0);
        }
        cnt--;
        cwmars.proccnt[pid] = cnt;
    } else if (op->op != CWOPSPL) {
        cwmars.runqtab[pid][cur] = pc;
        cur++;
    }
    cnt = cwmars.proccnt[pid];
    if (cur == cnt) {
        cur = 0;
    }
    cwmars.curproc[pid] = cur;
#if defined(ZEUS)
    ref++;
    if (!cwmars.running || ref == 32) {
        zeusdrawsim(&cwmars.zeusx11);
        ref = 0;
    }
#endif

    return;
}

/* virtual machine main loop */
void
cwloop(void)
{
    long pid = cwmars.curpid;

    while (cwmars.nturn[pid]--) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        cwmars.curpid = pid;
    }
    fprintf(stderr, "TIE\n");
#if defined(ZEUS)
    sleep(5);
#endif

    exit(0);
}

/* initialise virtual machine */
static void
cwinit(void)
{
    time_t seed32 = (((time(NULL) & 0xff) << 24)
                     | ((time(NULL) & 0xff) << 16)
                     | ((time(NULL) & 0xff) << 8)
                     | (time(NULL) & 0xff));

#if defined(CWRANDMT32)
    srandmt32(seed32);
#else
    srand(time(&seed32));
#endif
    cwinitop();
    rcinitop();
#if defined(CWPIDMAP)
    cwmars.pidmap = calloc(CWCORESIZE / CHAR_BIT, sizeof(char));
#endif
    cwmars.optab = calloc(CWCORESIZE, sizeof(struct cwinstr));
    if (!cwmars.optab) {
        fprintf(stderr, "failed to allocate core\n");

        exit(1);
    }

    return;
}

int
main(int argc, char *argv[])
{
    FILE *fp;
    long  base;
    long  lim;
    long  pc1;
    long  pc2;

#if defined(ZEUS)
#if defined(ZEUSWINX11)
    zeusinitx11(&cwmars.zeusx11);
#elif defined(ZEUSWINEFL)
    zeusinitefl(&cwmars.zeusefl);
#endif
#endif
    if (argc != 3) {
        fprintf(stderr, "usage: %s prog1.rc prog2.rc\n", argv[0]);

        exit(1);
    }
    cwinit();
#if defined(CWRANDMT32)
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
#if defined(CWRANDMT32)
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
#if defined(CWRANDMT32)
    cwmars.curpid = randmt32() & 0x01;
#else
    cwmars.curpid = rand() & 0x01;
#endif
    cwmars.prog1name = argv[1];
    cwmars.prog2name = argv[2];
#if defined(ZEUS)
    zeusdrawsim(&cwmars.zeusx11);
    while (1) {
        zeusprocev(&cwmars.zeusx11);
    }
#else
    cwloop();
#endif

    /* NOTREACHED */
    exit(0);
}

