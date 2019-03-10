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
#define V0_UNIT_MASK    0x0f00  // coprocessor ID
#define V0_OP_MASK      0x00ff  // instruction ID
#define V0_FLAG_BITS    4
#define V0_UNIT_BITS    4
#define V0_OP_BITS      8
/* parm-member bits; low 12 bits are for register IDs (6-bit each) */
#define V0_VAL_BIT      0x8000  // 13-bit signed argument present in opcode
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
    uint16_t      code;         // flag-bits, unit, instruction
    uint16_t      parm;         // 4 address-bits + 2 6-bit register IDs
    union v0arg32 arg[VLA];     // immediate if (ins->parm & V0_IMM_BIT)
};

#endif /* #ifndef __V0_VM_H__ */

