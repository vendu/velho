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

/* code-member bits */
#define V0_VAL_BIT              (1 << 15)
#define V0_IMM_BIT              (1 << 14)
#define V0_FLAG1_BIT     	(1 << 13)
#define V0_FLAG2_BIT            (1 << 12)
#define V0_CODE_FLAG_BITS       6
#define V0_CODE_UNIT_BITS       6
#define V0_CODE_OP_BITS         4
#define V0_CODE_FLAG_MASK       0xfc00
#define V0_CODE_UNIT_MASK       0x03f0
#define V0_CODE_OP_MASK         0x000f
/* parm-member values */
#define V0_HALF_BIT             (1 << 15)       // halfword (16-bit) operation
#define V0_BYTE_BIT             (1 << 14)       // byte (8-bit) operation
#define V0_REG_MASK             ((1 << V0_REG_BIT) - 1)
#define V0_PARM_FLAG_MASK       0xc000
#define V0_NO_ADR       	0x0000  // register-only operands
#define V0_NDX_ADR      	0x2000  // indexed, e.g. $4(%sp) or $128(%pc) or direct
#define V0_REG_ADR      	0x1000  // base register, e.g. *%r1 or *%pc
#define V0_ADR_MASK     	0x3000  // addressing-mode mask
#define V0_REG2_MASK    	0x0fc0  // register operand #2 ID
#define V0_REG1_MASK    	0x003f  // register operand #1 ID
#define V0_PARM_FLAG_BITS       2
#define V0_PARM_ADR_BITS        2       // address-mode for load-store
#define V0_REG_BITS             6       // bits per register ID
struct v0ins {
    uint16_t            code;         // flag-bits, unit, instruction
    uint16_t            parm;         // flag-bits, address-mode, 2 register IDs
    union v0arg         arg[VLA];     // immediate if (ins->parm & V0_IMM_BIT)
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
    v0intreg    genregs[V0_GEN_REGS];   // wide registers
    v0word      sysregs[V0_SYS_REGS];   // system registers
};

struct v0thr {
    v0wide      genregs[V0_GEN_REGS];
    v0word      sysregs[V0_SYS_REGS];
    m_atomic_t  mtx;
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

#endif /* __V0_TYPES_H__ */

