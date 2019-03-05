#ifndef __V0__INS_H__
#define __V0__INS_H__

#include <stdint.h>
#include <valhalla/cdefs.h>
#include <valhalla/param.h>
#include <v0/types.h>

/* INSTRUCTION FORMAT */
/*
 * - the shortest instructions (2 register operands, no special functionality)
 *   are 16-bit
 *   - the high bit (0x80) of the opcode is used to denote there's another 16-
 *     bit instruction parcel to follow
 * - instructions are always fetched as 64-bit or 32-bit chunks
 * - immediate operands following 32-bit instructions are always 32-bit
 */

/* imm16- and imu16- immediate fields */
#define V0_IMM12_VAL_MAX   0x07ff
#define V0_IMM12_VAL_MIN   (-0x07ff - 1)

#define V0_INS_CODE_BITS   8
#define V0_INS_REG_BITS    4
#define V0_INS_REG_MASK    ((1 << V0_INS_REG_BITS) - 1)
#define vogetinsval(ins)   ((ins)->arg.parm.val & 0x2f)
/* NOP is declared as all 0-bits */
#define V0_NOP_CODE        (UINT16_C(0))
#define V0_COP_CODE        0xff
/* instruction prefixes have all 1-bits in code */
#define V0_SYS_CODE        (UINT16_C(~0))
/* predefined coprocessor IDs in val-field after 0xff-opcode */
#define V0_COPROC_FPM      0x01 // fixed-point
#define V0_COPROC_FPU      0x02 // floating-point unit
#define V0_COPROC_SIMD     0x03 // SIMD-unit
#define V0_COPROC_VEC      0x04 // vector processor
#define V0_COPROC_DSP      0x05 // digital signal processor
#define v0isnop(ins)       (*(uint8_t *)(ins) == V0_NOP_CODE)
#define v0coprocid(ins)    (((uint8_t *)(ins))[1])
#define v0getcode(ins)     ((ins)->code)
#define v0getxreg(ins)     ((ins)->arg[0].op.val)
#define v0getval(ins)      ((ins)->arg[0].op.val)
#define v0setreg(ins, reg, id)                                          \
    ((ins)->parm |= (reg) << (4 * (id)))
#define v0setxreg(ins, reg)                                             \
    ((ins)->u.arg16[0].op.val = (reg))

/* values for the val-field; GEN */
/* common flags */
#define V0_ZERO_BIT (1U << 0)
#define V0_UNS_BIT  (1U << 0)
#define V0_FLG_BIT  (1U << 1)
#define V0_MSW_BIT  (1U << 2)
/* logical operations */
#define V0_AND_BIT  (1U << 0)
#define V0_OR_BIT   (1U << 1)
#define V0_EXC_BIT  (1U << 2)
/* shift operations */
#define V0_DIR_BIT  (1U << 0)
#define V0_ARI_BIT  (1U << 1)
#define V0_ROT_BIT  (1U << 2)
/* add operations */
#define V0_INC_BIT  (1U << 2)
#define V0_SUB_BIT  (1U << 3)
/* multiplication operations */
#define V0_REM_BIT  (1U << 1)
#define V0_DIV_BIT  (1U << 2)
#define V0_RPC_BIT  (1U << 3)
/* bit operations */
#define V0_CNT_BIT  (1U << 0)
#define V0_ONE_BIT  (1U << 1)
/* memory operations */
#define V0_RD_BIT   (1U << 0)
#define V0_REG_BIT  (1U << 1)
#define V0_WR_BIT   (1U << 1)
#define V0_N_BIT    (1U << 1)
#define V0_MEM_BIT  (1U << 2)
#define V0_CL_BIT   (1U << 2)
#define V0_BAR_BIT  (1U << 3)
#define V0_MSW_MASK 0x0a
//#define V0_PG_BIT   (1U << 4)
/* atomic operations */
#define V0_CLR_BIT  (1U << 0)
#define V0_DUAL_BIT (1U << 0)
#define V0_CAS_BIT  (1U << 1)
#define V0_SYN_MASK 0x06
/* values for the branch unit  (BRA) */
#define V0_EQ_BIT   (1U << 0)
#define V0_NE_BIT   (1U << 1)
#define V0_LT_BIT   (1U << 2)
#define V0_GT_BIT   (1U << 3)
#define V0_BC_BIT   (1U << 4)
#define V0_BO_BIT   (1U << 5)
#define V0_BCS_MASK 0x06
#define V0_BOS_MASK 0x07
/* subroutines */
#define V0_TERM_BIT (1U << 0)
#define V0_SYS_BIT  (1U << 1)
#define V0_RET_BIT  (1U << 2)
#define V0_THR_BIT  (1U << 3)
/* interrupt management */
#define V0_ON_BIT   (1U << 0)
#define V0_RST_BIT  (1U << 0)
#define V0_MSK_BIT  (1U << 1)
#define V0_INT_BIT  (1U << 2)
#define V0_EV_BIT   (1U << 3)
#define V0_HLT_MASK 0x06
/* I/O control */
#define V0_CMD_BIT  (1U << 2)
#define V0_RM_CMD   1
#define V0_CP_CMD   2

#endif /* __V0__INS_H__ */

