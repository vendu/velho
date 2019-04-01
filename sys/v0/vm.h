#ifndef __V0_VM_H__
#define __V0_VM_H__

#include <stdint.h>
#include <v0/types.h>

typedef char           *v0bptr;         // 8-bit pointer
typedef uint8_t        *v0ubptr;   	// 8-bit unsigned pointer
typedef void           *v0ptr;     	// generic pointer
typedef intptr_t        v0memofs;  	// signed pointer value
typedef uintptr_t       v0memadr;       // unsigned pointer value

 // virtual machine operation
typedef long v0vmopfunc_t(struct vm *vm, void *op);

#define v0getop(ins)    	((ins)->code & V0_OP_MASK)
#define v0getunit(ins)  	(((ins)->code & V0_UNIT_MASK) >> V0_UNIT_SHIFT)
#define v0getreg1(ins)          ((ins)->parm & V0_REG_MASK)
#define v0getreg2(ins)          ((ins)->parm >> V0_REG_BITS)
#define V0getadr(ins)           (((ins)->parm & V0_ADR_MASK) >> 2 * V0_REG_BITS)
#define v0getval(ins)           (((ins)->parm & V0_VAL_BIT)             \
                                 ? ((ins)->parm & ~V0_VAL_BIT)          \
                                 : -1)
#define v0getflg(ins)           ((ins)->code & V0_INS_FLG_MASK)
#define v0getword(vm, adr)      (*(v0reg *)(&(vm)->mem[adr]))
#define v0gethalf(vm, adr)      (*(int16_t *)(&(vm)->mem[adr]))
#define v0getbyte(vm, adr)      (*(int8_t *)(&(vm)->mem[adr]))
#define v0getimm8(ins)          ((imm)->arg[0].i8)
#define v0getimmu8(ins)         ((imm)->arg[0].u8)
#define v0getimm16(ins)         ((imm)->arg[0].i16)
#define v0getimmu16(ins)        ((imm)->arg[0].u16)
#define v0getimm32(ins)         ((imm)->arg[0].i32)
#define v0getimmu32(ins)        ((imm)->arg[0].u32)
#define v0getimm(ins)           ((imm)->arg[0].val)
#define v0getimmu(ins)          ((imm)->arg[0].uval)
#define v0getimmadr(ins)        ((imm)->arg[0].adr)

#define v0gewtreg(reg)          (*(v0wreg *)(&g_v0vm.intregs[(reg)]))
#define v0getreg(reg)           (*(v0reg *)(&g_v0vm.intregs[(reg)]))
#define v0gethreg(reg)          (*(int16_t *)(&g_v0vm.intregs[(reg)]))
#define v0getbreg(reg)          (*(int8_t *)(&g_v0vm.intregs[(reg)]))
#define v0getsysreg(reg)        (*(v0reg *)(&g_v0vm.sysregs[(reg)]))
#define v0loadwreg(reg, adr)    (*(v0wreg *)(&g_v0vm.intregs[(reg)]) \
                                 = *(v0reg *)(adr))
#define v0loadreg(reg, adr)     (*(v0reg *)(&g_v0vm.mem[(reg)]) \
                                 = *(v0reg *)(adr))
#define v0loadreg(reg, adr)     (*(int16_t *)(&g_v0vm.mem[(reg)])  \
                                 = *(int16_t *)(adr))
#define v0loadreg(reg, adr)     (*(int32_t *)(&g_v0vm.mem[(reg)])       \
                                 = *(int32_t *)(adr))
#define v0sewtreg(reg, val)     (*(v0wreg *)(&g_v0vm.mem[(reg)])        \
                                 = (val) & 0xffffffff)
#define v0setreg(reg, val)      (*(int32_t *)(&g_v0vm.mem[(reg)])        \
                                 = (val) & 0xffffffff)
#define v0sethreg(reg, val)     (*(int16_t *)(&g_v0vm.mem[(reg)])       \
                                 = (val) & 0xffff)
#define v0setbreg(reg, val)     (*(int8_t *)(&g_v0vm.mem[(reg)])        \
                                 = (val))
#define v0storehi(reg, adr)     ((v0reg *)(adr) = v0getwreg(reg) >> V0_REG_SIZE)
#define v0storelo(reg, adr)     ((v0reg *)(adr) = v0getwreg(reg) & 0xffffffff)
#define v0storereg(reg, adr)    ((v0reg *)(adr) = v0getreg(reg))
#define v0storehreg(reg, adr)   ((int16_t *)(adr) = v0gethreg(reg))
#define v0storebreg(reg, adr)   ((int8_t *)(adr) = v0getbreg(reg))

/*
 * 32-bit little-endian argument parcel
 * - declared as union, 32-bit aligned
 */
union v0arg32 {
    v0ureg      adr;    // memory address
    v0ureg      uval;   // unsigned register value
    v0reg       val;    // signed register value
    v0reg       ofs;    // signed offset
    int32_t     i32;    // 32-bit signed integer
    uint32_t    u32;    // 32-bit unsigned integer
    int16_t     i16;    // 16-bit signed integer
    uint16_t    u16;    // 16-bit unsigned integer
    int8_t      i8;     // 8-bit signed integer
    uint8_t     u8;     // 8-bit unsigned integer
};

/* code-member bits */
#define V0_UNSG_BIT     0x8000  // unsigned operation
#define V0_IMM_BIT      0x4000  // immediate argument follows opcode
#define V0_HALF_BIT     0x2000  // halfword (16-bit) operation
#define V0_BYTE_BIT     0x1000  // byte (8-bit) operation
#define V0_FLAG_MASK    0xf000  // code-member flag-bits
#define V0_UNIT_MASK    0x07c0  // coprocessor ID
#define V0_OP_MASK      0x003f  // instruction ID
#define V0_FLAG_BITS    4
#define V0_UNIT_BITS    5
#define V0_OP_BITS      6
#define V0_UNIT_SHIFT   V0_OP_BITS
/* parm-member bits; low 12 bits are for register IDs (6-bit each) */
#define V0_VAL_BIT      0x8000  // 15-bit signed argument present in opcode
#define V0_ATOM_BIT     0x4000  // atomic operation
#define V0_PRIV_BIT     0x2000  // privileged operation [system mode/ring #0]
#define V0_NO_ADR       0x0000  // register-only operands
#define V0_NDX_ADR      0x1000  // indexed, e.g. $4(%sp) or $128(%pc) or direct
#define V0_REG_ADR      0x0800  // base register, e.g. *%r1 or *%pc
#define V0_ADR_MASK     0x1800  // addressing-mode mask
#define V0_REG2_MASK    0x07c0  // register operand #2 ID
#define V0_REG1_MASK    0x003f  // register operand #1 ID
#define V0_ADR_BITS     2
#define V0_RES_BITS     2
#define V0_REG_BITS     6
struct v0ins {
    uint16_t            code;         // flag-bits, unit, instruction
    uint16_t            parm;         // 4 address-bits + 2 6-bit register IDs
    union v0arg32       arg[VLA];     // immediate if (ins->parm & V0_IMM_BIT)
};

struct v0membuf {
    v0reg       adr;
    v0reg       val;
};

#define v0getpc(vm)     v0getreg(V0_PC_REG)
#define v0getfp(vm)     v0getreg(V0_FP_REG)
#define v0getsp(vm)     v0getreg(V0_SP_REG)
#define v0getmsw(vm)    v0getsysreg(V0_MSW_REG)
struct vm {
    uint8_t            *mem;
    size_t              memsize;
    m_atomic_t          buflkmap;
    struct v0membuf    *buftab;
    size_t              bufsize;
    v0wreg              intregs[V0_INT_REGS];
    v0reg               sysregs[V0_SYS_REGS];
};

#endif /* #ifndef __V0_VM_H__ */

