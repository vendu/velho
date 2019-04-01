#ifndef __V0_MACH_H__
#define __V0_MACH_H__

#include <limits.h>
#include <v0/conf.h>
#include <v0/fpu.h>
#include <v0/types.h>

struct v0machatr {
    size_t       nbbram;        // size of BRAM-memory in bytes
    uint8_t     *bram;          // block RAM
    size_t       nbram;         // size of RAM-memory in bytes
    uint8_t     *ram;           // pointer to virtual machine "RAM"
    size_t       clshift;       // cacheline-size log2
    int8_t      *clbits;        // cacheline lock-bits
    size_t       pgshift;       // page-size log2
    v0pagedesc  *pagetab;       // page-table of v0pagedesc-entries
    size_t       niomap;        // number of I/O-descriptors
    v0iodesc   **iomap;         // v0iodesc *iomap[niomap]
};

struct v0machintctx {
    v0wreg regs[V0_STD_REGS];   // scalar registers
};

struct v0machfpuctx {
    v0dbl regs[V0_FPU_REGS];    // floating-point registers
};

struct v0machthr {
    v0machintctx        ictx;   // integer-unit context
    v0machfpuctx        fctx;   // floating-point unit context
};

struct v0tlbitem {
    v0word      hash;
    v0adr       page;
};

#define V0_MACH_THREADS   V0_MAX_THREADS
#define V0_TLB_ENTRIES   (V0_PAGE_SIZE / sizeof(v0pagedesc))
#define V0_IOMAP_ENTRIES (V0_PAGE_SIZE / sizeof(v0iodesc *))
#define v0initvm(vm, ramsz, clsft, numtlb, pgsft, numio)               \
    ((vm)->atr.nbram = (ramsz),                                        \
     (vm)->atr.clshift = (clsft),                                      \
     (vm)->atr.ntlb = (numtlb),                                        \
     (vm)->atr.pgshift = (pgsft),                                      \
     (vm)->atr.niomap = (numio),                                       \
     (((vm)->atr.ram = calloc((nbram), sizeof(int8_t)))                \
      && ((vm)->atr.pagetab = calloc((ramsz) >> (pgsft), sizeof(v0pagedesc))) \
      && ((vm)->atr.clbits = calloc((ramsz) >> (clsft + 3), sizeof(int8_t))) \
      && ((vm)->atr.iomap = calloc((numio), sizeof(v0iodesc *)))))
struct v0 {
    struct v0machthr thrbuf[V0_SYS_THREADS];
    v0pagedesc       tlb[V0_TLB_ENTRIES];
    v0wreg           regs[V0_STD_REGS];
    v0flt            fpuregs[V0_FPU_REGS];
    struct v0sysatr  atr;
};

struct v0memrom {
    long *hashpage(v0pagedesc pte);
    long *bufpage(void *tlb, v0pagedesc pte);
    long *chkpage(v0pagedesc pte, v0pagedesc flg);
    void *mappage(void *tlb, v0pagedesc pte, v0pagedesc flg);
    void *holdcl(void *adr);
    void *relcl(void *adr);
};

/* flg-member bits */
#define V0_IO_AX    (1 << 0)    // world-mappable system device
#define V0_IO_AW    (1 << 1)    // world-writable device
#define V0_IO_AR    (1 << 2)    // world-readable device
#define v0_IO_GX    (1 << 3)    // group-mappable device
#define v0_IO_GW    (1 << 4)    // group-writable device
#define v0_IO_GR    (1 << 5)    // group-readable device
#define V0_IO_UX    (1 << 6)    // user-mappable device
#define V0_IO_UW    (1 << 7)    // user-writable device
#define V0_IO_UR    (1 << 8)    // user-readable device
#define V0_IO_DMA   (1 << 9)    // direct memory access
#define V0_IO_BURST (1 << 11)   // burst mode bus I/O
#define V0_IO_BUF   (1 << 12)   // buffered I/O
#define V0_IO_RAW   (1 << 13)   // raw [unbuffered] I/O
#define V0_IO_SYNC  (1 << 14)   // synchronous I/O
#define V0_IO_RST   (1 << 15)   // restart I/O system calls on descriptor
#define V0_IO_MAP   (1 << 16)   // I/O buffer may be shared
struct v0iocred {
    long pid;                   // ID of owning process
    long uid;                   // ID of owner user
    long gid;                   // ID of owner group
    long flg;                   // access permissions
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

/* values for the flg-member to denote presence of parameters */
#define SYS_IO_DEV_BIT   (1U << 0)
#define SYS_IO_BUS_BIT   (1U << 1)
#define SYS_IO_PERM_BIT  (1U << 2)
#define SYS_IO_BASE_BIT  (1U << 3)
#define SYS_IO_BUF_BIT   (1U << 4)
#define SYS_IO_SIZE_BIT  (1U << 5)
#define SYS_IO_DESC_SIZE (8 * V0_WORD_SIZE)
struct v0sysiodesc {
    v0reg  flg;         // flags listing parameters present
    v0reg  dev;         // device ID
    v0reg  bus;         // bus ID
    v0reg  perm;        // I/O-permission bits
    v0ureg base;        // I/O-map such as framebuffer base address
    v0ureg buf;         // I/O read buffer base address
    v0ureg size;        // I/O-map size in bytes
    v0reg  _pad;        // pad to boundary of 8 v0regs
};

struct v0sysiofuncs {
    int     *creat(const char *path, mode_t mode);
    int     *open(const char *path, int flg, ...);
    int     *close(int fd);
    off_t   *lseek(int fd, off_t ofs, int whence);
    int     *link(const char *src, const char *dest);
    int     *symlink(const char *src, const char *dest);
    int     *unlink(const char *path);
    ssize_t *read(int fd, void *buf, size_t nb);
    ssize_t *write(int fd, void *buf, size_t nb);
    void    *mmap(void *adr, size_t nb, int prot, int flg, int fd, off_t ofs);
    int     *mkdir(const char *path, mode_t mode);
    int     *rmdir(const char *path);

};

#endif /* __V0_SYS_H__ */

