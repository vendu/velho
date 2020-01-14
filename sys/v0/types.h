#ifndef __V0_TYPES_H__
#define __V0_TYPES_H__

#include <stdint.h>
#include <mach/param.h>
#include <v0/v0.h>

/* basic types */

typedef int32_t                 v0word;         // signed register value
typedef uint32_t                v0uword;        // unsigned register value
typedef int64_t                 v0wide;    	// signed wide-register value
typedef uint64_t                v0uwide;   	// unsigned wide-register value

typedef uint32_t                v0adr;
typedef int32_t                 v0ofs;

#if (PTRBITS == 32)
typedef uint32_t                v0trapfunc;     // trap function address + flags
#elif (PTRBITS == 64)
typedef uint64_t                v0trapfunc;
#else
#error v0trapfunc not defined in <v0/types.h>
#endif

typedef uint32_t                v0pagedesc;     // page address + flags
typedef uint32_t                v0ioperm;       // I/O-permission bits
typedef uint32_t                v0iodesc;

/*
 * 32-bit little-endian argument parcel
 * - declared as union, 32-bit aligned
 */
struct v0arg {
    union {
        int32_t                 i32;
        uint32_t                u32;
        int16_t                 i16;
        uint16_t                u16;
        int8_t                  i8;
        uint8_t                 u8;
        struct {
            unsigned int        r1   : V0_REG_BITS;     // first register ID
            unsigned int        r2   : V0_REG_BITS;     // last register ID
            unsigned int        op   : 1;               // push/pop
        } stkop;
        struct {
            uint16_t            port;                   // port ID
        } iop;
        struct {
            uint16_t            cmd;                    // device command
            uint16_t            desc;                   // I/O-descriptor ID
        } ioc;
    } data;
};

#define V0_INST_UNIT_OFS        0
#define V0_INST_OP_OFS          (V0_INST_UNIT_OFS + V0_UNIT_BITS)
#define V0_INST_VAL_OFS         (V0_INST_OP_OFS + V0_OP_BITS)
#define V0_INST_ABIT_OFS        (V0_UNIT_BITS + V0_OP_BITS + V0_VAL_BITS)
struct v0inst {
    //    unsigned int unit : V0_UNIT_BITS;
    //    unsigned int op   : V0_OP_BITS;
    unsigned int code : V0_UNIT_BITS + V0_OP_BITS;
    unsigned int val  : V0_VAL_BITS;
    unsigned int abit : 1;
    struct v0arg arg[VLA];
};

/* callee-save structure */
struct v0callctx {
    /* r0..r3 may be used as subroutine arguments */
};

/* thread structures */

struct v0thr {
    v0word      msw0;                           // ring #0 machine status word
    v0word      sp0;                            // ring #0 stack-pointer
    v0word      intrmask;                       // current interrupt mask
    v0word      rescl;                          // ID of reserved cacheline
    v0adr       wtchan;                         // wait channel (system struct)
    v0wide      genregs[V0_GENERAL_REGISTERS];  // [wide] general registers
    v0word      sysregs[V0_SYSTEM_REGISTERS];   // system registers
};

//#define V0_MACH_THREADS         V0_MAX_THREADS
#if 0
#define v0initsys(sys, ramsz, numcl, numtlb, numpg, numio)              \
    ((vm)->atr.nbram = (ramsz),                                         \
     (vm)->atr.clshift = (clsft),                                       \
     (vm)->atr.ntlb = (numtlb),                                         \
     (vm)->atr.pgshift = (pgsft),                                       \
     (vm)->atr.niomap = (numio),                                        \
     (((vm)->atr.ram = calloc((nbram), sizeof(int8_t)))                 \
      && ((vm)->atr.pagetab = calloc((ramsz) >> (pgsft), sizeof(v0pagedesc))) \
      && ((vm)->atr.clbits = calloc((ramsz) >> (clsft + 3), sizeof(int8_t))) \
      && ((vm)->atr.iomap = calloc((numio), sizeof(v0iodesc *)))))
#endif

#endif /* __V0_TYPES_H__ */

