#ifndef __V0_TYPES_H__
#define __V0_TYPES_H__

#include <v0/conf.h>
#include <stdint.h>

typedef int32_t    v0reg;     	// signed register value
typedef uint32_t   v0ureg;    	// unsigned register value
typedef int64_t    v0wreg;    	// signed wide-register value
typedef uint64_t   v0uwreg;   	// unsigned wide-register value
typedef v0ureg     v0adr;     	// machine memory address
typedef char      *v0ptr8;    	// 8-bit pointer
typedef uint8_t   *v0ptru8;   	// 8-bit unsigned pointer
typedef void      *v0ptr;     	// generic pointer
typedef intptr_t   v0memofs;  	// signed pointer value
typedef uintptr_t  v0memadr;    // unsigned pointer value

typedef uint32_t   v0pte;       // page-table entry
typedef uint32_t   v0trapdesc;  // interrupt/exception/abort/fault spec
typedef uint32_t   v0pagedesc;  // [virtual] memory page descriptor
#define V0_IO_SX    (1 << 0)    // system-mappable system device
#define V0_IO_SW    (1 << 1)    // system-writable device
#define V0_IO_SR    (1 << 2)    // system-readable device
#define v0_IO_GX    (1 << 3)    // group-mappable device
#define v0_IO_GW    (1 << 4)    // group-writable device
#define v0_IO_GR    (1 << 5)    // group-readable device
#define V0_IO_UX    (1 << 6)    // user-mappable device
#define V0_IO_UW    (1 << 7)    // user-writable device
#define V0_IO_UR    (1 << 8)    // user-readable device
#define V0_IO_DMA   (1 << 9)
#define V0_IO_BUF   (1 << 10)
#define V0_IO_BURST (1 << 11)
typedef uint32_t   v0ioperm;    // I/O-permission bits
typedef uint32_t   v0iodesc;    // I/O-device page address + flags
struct v0iodesc {
    void      *base;            // I/O-memory map address, e.g. framebuffer
    void      *buf;             // I/O double-buffer address; e.g. graphics
    size_t     bufsz;           // base/buf region size
    v0ioperm   flg;             // I/O-permissions + flags
};

typedef long v0vmopfunc_t(struct vm *vm, void *op); // virtual machine operation

union v0imm16 {
    int16_t  i16;
    uint16_t u16;
    int16_t  ofs;
};

/*
 * 32-bit little-endian argument parcel
 * - declared as union, 32-bit aligned
 */
union v0imm32 {
    v0ureg   adr;
    v0ureg   uval;
    v0reg    val;
    v0reg    ofs;
    int32_t  i32;
    uint32_t u32;
    int16_t  i16;
    uint16_t u16;
    int8_t   i8;
    uint8_t  u8;
};

/* parm-member bits */
#define V0_IMM_BIT  0x8000      // immediate argument follows opcode
/* addressing modes */
#define V0_PIC_ADR  0x4000      // PC-relative, i.e. x(%pc) for shared objects
#define V0_NDX_ADR  0x2000      // indexed, i.e. %r1(%r2) or $c1(%r2)
#define V0_REG_ADR  0x1000      // base address in register
#define V0_ADR_MASK 0x7000      // mask of addressing mode bits
#define v0getreg1(vm, ins)                                              \
    ((vm)->regs[(ins)->regs & V0_INS_REG_MASK])
#define v0getreg2(vm, ins)                                              \
    ((vm)->regs[((ins)->regs >> V0_INS_REG_BITS) & V0_INS_REG_MASK])
#define v0getadr1(vm, ins)                                              \
    (((ins)->parm & V0_ADR_MASK == V0_REG_ADR)                          \
     ? (v0getreg1(vm, ins))                                             \
     : ((v0getflg(ins) & V0_NDX_ADR)                                    \
        ? (v0getreg1(vm, ins) + v0getofs(vm, ins))                      \
        : ((vm)->regs[V0_PC_REG] + sizeof(struct v0ins) * v0getval(ins))))
#define v0getadr2(vm, ins)                                              \
    (((ins)->parm & V0_ADR_MASK == V0_REG_ADR)                          \
     ? (v0getreg2(vm, ins))                                             \
     : ((v0getflg(ins) & V0_NDX_ADR)                                    \
        ? (v0getreg2(vm, ins) + v0getofs(vm, ins))                      \
        : ((vm)->regs[V0_PC_REG] + sizeof(struct v0ins) * v0getval(ins))))
#define v0getcode(ins) ((ins)->code)
#define v0getunit(ins) ((ins)->code >> 4)
#define v0getop(ins)   ((ins)->code & 0x0f)
#define v0getflg(ins)  ((ins)->parm 0xf000)
#define v0getval(ins)  (((ins)->parm & 0x0800)                          \
                        ? (-((ins)->parm & 0x07ff) - 1)                 \
                        : ((ins)->parm & 0x07ff))
#define v0getofs(ins)  ((ins)->imm[0].ofs)
:(ins)->parm & 0x0fff)
struct v0ins {
    uint8_t       code;         // unit + instruction IDs
    uint8_t       regs;         // register IDs
    int16_t       parm;         // flag-bits + 12-bit immediate value
    union v0imm32 imm[VLA];     // immediate argument if present
};

struct v0callctx {
    v0reg fp; // copy of FP (frame pointer)
    v0reg r1; // registers R1 through R7
    v0reg r2;
    v0reg r3;
    v0reg r4;
    v0reg r5;
    v0reg r6;
    v0reg r7;
    v0reg ln; // return address back to caller
};

#endif /* __V0_TYPES_H__ */

