#ifndef __V0_TYPES_H__
#define __V0_TYPES_H__

#include <stdint.h>
#include <v0/conf.h>
#include <v0/gpu.h>

/* basic types */

typedef int32_t         v0word;         // signed register value
typedef uint32_t        v0uword;        // unsigned register value
typedef int64_t         v0wide;    	// signed wide-register value
typedef uint64_t        v0uwide;   	// unsigned wide-register value

typedef uint32_t        v0pagedesc;     // page address + flags
typedef uint32_t        v0trapdesc;     // trap function address + flags

typedef uint32_t        v0iodesc;       // I/O-device page address + flags
typedef uint32_t        v0ioperm;       // I/O-permission bits

/* instruction format */

/*
 * 32-bit little-endian argument parcel
 * - declared as union, 32-bit aligned
 */
union v0arg {
    v0uword     adr;    // memory address
    v0uword     uval;   // unsigned register value
    v0word      val;    // signed register value
    v0word      ofs;    // signed offset
    int32_t     i32;    // 32-bit signed integer
    uint32_t    u32;    // 32-bit unsigned integer
    int16_t     i16;    // 16-bit signed integer
    uint16_t    u16;    // 16-bit unsigned integer
    int8_t      i8;     // 8-bit signed integer
    uint8_t     u8;     // 8-bit unsigned integer
};

/* unit-member */
#define V0_OP_UNIT_MASK         0x000f
#define V0_OP_OP_MASK           0x00f0
#define V0_OP_FLAG_MASK         0xff00
/* flag-member bits */
#define V0_FLAG1_BIT     	(1 << 0)
#define V0_FLAG2_BIT            (1 << 1)
#define V0_FLAG_BITS            2
/* bits-member bits */
#define V0_VAL_BIT              (1 << 5)        // 5-bit value present
#define V0_SIGN_BIT             (1 << 4)        // value sign-bit
#define V0_INTR_BIT             (1 << 3)        // interrupts pending
#define V0_HIGHER_BIT           (1 << 2)        // higher-than result
#define V0_LOWER_BIT            (1 << 1)        // less-than result
#define V0_ZERO_BIT             (1 << 0)        // zero/equal result
#define V0_BITS_CNT             6               // # of members in bits-field
#define V0_UNIT_BITS            4
#define V0_OP_BITS              4
/* parm-member values */
#define V0_IMM_BIT              (1 << 15)       // 15-bit value present
#define V0_HALF_BIT             (1 << 14)       // halfword (16-bit) operation
#define V0_BYTE_BIT             (1 << 13)       // byte (8-bit) operation
#define V0_ARG_BIT_MASK         0xc000
#define V0_REG_MASK             0xe000
#define V0_NO_ADR       	0x0000  // register-only operands
#define V0_NDX_ADR      	0x4000  // indexed, e.g. $4(%sp) or direct
#define V0_REG_ADR      	0x2000  // base register, e.g. *%r1 or *%pc
#define V0_STD_REG              0x0000  // standard unit register
#define V0_CTL_REG              0x1000  // control register
#define V0_ADR_MASK     	0x3000  // addressing-mode mask
#define V0_REG2_MASK    	0x1f00  // register operand #2 ID
#define V0_REG1_MASK    	0x00f8  // register operand #1 ID
#define V0_ADR_MODE_BITS        2       // address-mode for load-store
#define V0_REG_BITS             5       // bits per register ID
#define V0_OP_FLAG_BITS         3
#define V0_OP_OFS_BITS          3
#define V0_STK_FLAG_BITS        3       // stack operation flags
#define V0_STK_PROT_BIT         0x0001  // stack protector
#define V0_STK_RED_BIT          0x0002  // stack protector red-zone
#define V0_STK_CTL_BIT          0x0003  // control register range
#define V0_IO_MAP_MASK          0x7fff
#define V0_IO_FLAG_MASK         0x6000
#define V0_IO_FLAG_BIT          0x8000
#define V0_IO_MAP_BITS          15
#define V0_IO_FLAG_BITS         2
#define

struct v0arg {
    int32_t     i32;
    uint32_t    u32;
    int16_t     i16;
    uint16_t    u16;
    int8_t      i8;
    uint8_t     u8;
        union {
            unsigned int ra1  : V0_REG_BITS;
            unsigned int ra2  : V0_REG_BITS;
            unsigned int rt   : 1;
            unsigned int adr  : V0_OP_ADR_BITS;
            unsigned int flg  : V0_OP_FLAG_BITS;
        } adr;
        union {
            unsigned int r1   : V0_REG_BITS;    // first register ID
            unsigned int r2   : V0_REG_BITS;    // last register
            unsigned int ofs  : V0_REG_BITS;    //
        } stk;
        union {
            int16_t      map  : V0_IO_MAP_BITS; // port-I/O bitmap
            unsigned int flg  : V0_IO_FLAG_BITS; // port-IO operation flag
            unsigned int bit  : 1;              // port-I/O bit value
        } iop;
        union {
            unsigned int inst : 2;
            unsigned int ofs  : 12;     // 1-megabyte aligned address or port
            unsigned int flg  : 2;      // COW, DMA
        } ioc;
};

struct v0inst {
    unsigned int unit : 4;
    unsigned int op:  : 4;
    unsigned int flg  : 2;
    unsigned int bits : 6;
};

struct v0arg {
    union {
        int8_t   i8;
        uint8_t  u8;
        int16_t  i16;
        uint16_t u16;
        int32_t  i32;
        uint32_t u32;
    } data;
};

/* caller-save structure */
struct v0callerctx {
    /* r0..r3 may be used as subroutine arguments */
    v0word      r0;
    v0word      r1;
    v0word      r2;
    v0word      r3;
};

/* thread structures */

struct v0tcb {
    v0word      msw0;                   // ring #0 machine status word
    v0word      sp0;                    // ring #0 stack-pointer
    v0wide      genregs[V0_GEN_REGS];   // wide registers
    v0word      sysregs[V0_SYS_REGS];   // system registers
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

#define V0_MACH_THREADS   V0_MAX_THREADS
#define V0_TLB_ENTRIES   (V0_PAGE_SIZE / sizeof(v0pagedesc))
#define V0_IOMAP_ENTRIES (V0_PAGE_SIZE / sizeof(v0iodesc *))
#define v0initvm(vm, ramsz, clsft, numtlb, pgsft, numio)                \
    ((vm)->atr.nbram = (ramsz),                                         \
     (vm)->atr.clshift = (clsft),                                       \
     (vm)->atr.ntlb = (numtlb),                                         \
     (vm)->atr.pgshift = (pgsft),                                       \
     (vm)->atr.niomap = (numio),                                        \
     (((vm)->atr.ram = calloc((nbram), sizeof(int8_t)))                 \
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

#endif /* __V0_TYPES_H__ */

