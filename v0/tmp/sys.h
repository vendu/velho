#ifndef __V0_SYS_H__
#define __V0_SYS_H__

#include <limits.h>
#include <v0/conf.h>
#include <v0/types.h>

#define V0_SYS_THREADS  (V0_PAGE_SIZE / sizeof(struct v0systhr))
#define V0_SYS_RAM_SIZE (1 << 0)
#define V0_SYS_NCL      (1 << 1)
#define V0_SYS_NTLB     (1 << 2)
#define V0_SYS_NPAGE    (1 << 3)
#define V0_SYS_NIODESC  (1 << 4)
#define v0initsys(sys, ramsz, numcl, numtlb, numpg, numio)              \
    ((sys)->atr.nbram = (ramsz),                                        \
     (sys)->atr.ncl = (numcl),                                          \
     (sys)->atr.ntlb = (numtlb),                                        \
     (sys)->atr.npage = (numpg),                                        \
     (sys)->atr.niomap = (numio),                                       \
     (((sys)->atr.ram = calloc((nbram), sizeof(int8_t)))                \
      && ((sys)->atr.pagetab = calloc((numpg), sizeof(v0pte)))          \
      && ((sys)->atr.clbits = calloc((ncl) >> 3, sizeof(int8_t)))       \
      && ((sys)->atr.iomap = calloc((numio), sizeof(v0iodesc *)))))

struct v0sysatr {
    size_t     nbram;           // size of RAM-memory in bytes
    uint8_t   *ram;             // pointer to virtual machine "RAM"
    size_t     ncl;             // number of physical cachelines present
    int8_t    *clbits;          // cacheline lock-bits
    size_t     npage;           // number of physical pages present
    v0pte     *pagetab;         // page-table of v0pte-entries
    size_t     niomap;          // number of I/O-descriptors
    v0iodesc **iomap;           // v0iodesc *iomap[niomap]
};

struct v0sysintctx {
    v0wreg regs[V0_STD_REGS];
};

typedef double v0flt;
struct v0sysfpuctx {
    v0flt regs[V0_FPU_REGS];
};

struct v0systhr {
    v0sysintctx ictx;           // integer-unit context
    v0sysfpuctx fctx;           // floating-point unit context
};

#define V0_CONF_NBRAM (1 << 0)
#define V0_CONF_NIO   (1 << 1)
struct v0sysconf {
    long            flg;        // flags to denote parameter presence
    struct v0sysatr atr;        // system attributes
};

#define V0_TLB_ENTRIES   (V0_PAGE_SIZE / sizeof(v0pte))
#define V0_IOMAP_ENTRIES (V0_PAGE_SIZE / sizeof(v0iodesc *))
struct v0sys {
    struct v0systhr  thrbuf[V0_SYS_THREADS];
    v0pte            tlb[V0_TLB_ENTRIES];
    v0wreg           regs[V0_STD_REGS];
    v0flt            fpuregs[V0_FPU_REGS];
    struct v0sysatr  atr;
};

struct v0sys {
    v0wreg  regs[V0_REGS];
    size_t    ramsize;  // sizeof of physical memory in bytes
    size_t    ncl;      // ramsize >> V0_CACHE_LINE_SHIFT
    int8_t   *clbits;
    size_t    ntlb;     // V0_PAGE_SIZE / sizeof(v0pte)
    v0pte    *tlb;
    size_t    npg;      // ramsize >> V0_PAGE_SHIFT
    v0pte    *ptab;
    size_t    nio;      // V0_PAGE_SIZE / sizeof(v0iodesc *)
    v0iodesc *iomap;
};

struct v0memrom {
    long *hashpage(v0pte pte);
    long *bufpage(void *tlb, v0pte pte);
    long *chkpage(v0pte pte, v0pte flg);
    void *mappage(void *tlb, v0pte pte, v0pte flg);
    void *holdcl(void *adr);
    void *relcl(void *adr);
};

#define V0_BUS_MAX_DEVS 16
#define V0_BUS_ALL_DEVS (-1)
struct v0iorom {
    long *probe(long bus, long dev, void *atr, size_t n);
    void *map(long bus, long dev, void *atr, size_t nb);
    long  reset(long bus, long dev);
    long  start(long bus, long dev);
    long  stop(long bus, long dev);
};

struct v0sysrom {
    struct v0memrom mem;
};

#endif /* __V0_SYS_H__ */

