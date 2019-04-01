#ifndef __V0_VM_INS_H__
#define __V0_VM_INS_H__

#include <stdint.h>
#include <valhalla/cdefs.h>
#include <valhalla/param.h>
#include <v0/types.h>

#define v0alnptr(adr, aln)      (((adr) + (aln) - 1) & ((aln) - 1))
#define v0alnstk(adr, aln)      ((adr) & ((aln) - 1))

/* NOP is declared as all 0-bits in code */
#define V0_NOP_CODE             0x0000
/* predefined coprocessor IDs in middle 8 parm-bits */
#define V0_COPROC_FPM         	(0x20 << V0_OP_BITS) // fixed-point
#define V0_COPROC_FPU         	(0x21 << V0_OP_BITS) // floating-point unit
#define V0_COPROC_SIMD        	(0x22 << V0_OP_BITS) // SIMD-unit
#define V0_COPROC_VEC         	(0x23 << V0_OP_BITS) // vector processor
#define V0_COPROC_DSP         	(0x24 << V0_OP_BITS) // digital signal processor
#define V0_COPROC_GPU         	(0x25 << V0_OP_BITS) // graphics processor unit
#define v0isnop(ins)            ((ins)->code == V0_NOP_CODE)
#define v0getflg(ins)           ((ins)->code & V0_INS_FLG_MASK)
#define v0getimm(ins)           ((ins)->imm[0].val)
#define v0getimmu(ins)          ((ins)->imm[0].uval)
#define v0getop(ins)    	((ins)->code & V0_OP_MASK)
#define v0getunit(ins)  	(((ins)->code & V0_UNIT_MASK) >> V0_UNIT_SHIFT)
#define v0getregid1(ins)        ((ins)->parm & V0_REG_MASK)
#define v0getregid2(ins)        (((ins)->parm >> V0_REG_BITS) & V0_REG_MASK)
#define v0getadr(ins)           (((ins)->parm & V0_ADR_MASK) >> 2 * V0_REG_BITS)
#define v0getval(ins)           (((ins)->parm & V0_VAL_BIT)             \
                                 ? ((ins)->parm & ~V0_VAL_BIT)          \
                                 : ((ins)->arg[0].val))
#define v0getword(vm, adr)      (*(v0reg *)(&(vm)->mem[adr]))
#define v0gethalf(vm, adr)      (*(int16_t *)(&(vm)->mem[adr]))
#define v0getbyte(vm, adr)      (*(int8_t *)(&(vm)->mem[adr]))
#define v0getflg(ins)           ((ins)->code & V0_INS_FLG_MASK)
#define v0getimm8(ins)          ((imm)->arg[0].i8)
#define v0getimmu8(ins)         ((imm)->arg[0].u8)
#define v0getimm16(ins)         ((imm)->arg[0].i16)
#define v0getimmu16(ins)        ((imm)->arg[0].u16)
#define v0getimm32(ins)         ((imm)->arg[0].i32)
#define v0getimmu32(ins)        ((imm)->arg[0].u32)
#define v0getimm(ins)           ((imm)->arg[0].val)
#define v0getimmu(ins)          ((imm)->arg[0].uval)

#define v0getwreg(reg)          (*(v0wreg *)(&g_vm.intregs[(reg)]))
#define v0getreg(reg)           (*(v0reg *)(&g_vm.intregs[(reg)]))
#define v0gethreg(reg)          (*(int16_t *)(&g_vm.intregs[(reg)]))
#define v0getbreg(reg)          (*(int8_t *)(&g_vm.intregs[(reg)]))
#define v0getsysreg(reg)        (*(v0reg *)(&g_vm.sysregs[(reg)]))
#define v0loadwreg(reg, adr)    (*(v0wreg *)(&g_vm.intregs[(reg)])    \
                                 = *(v0reg *)&g_vm.mem[(adr)])
#define v0loadreg(reg, adr)     (*(v0word *)(&g_vm.mem[(reg)])         \
                                 = *(v0reg *)&g_vm.mem[(adr)])
#define v0loadhreg(reg, adr)    (*(v0word *)(&g_vm.mem[(reg)])       \
                                 = *(int16_t *)&g_vm.mem[(adr)])
#define v0loadbreg(reg, adr)    (*(v0word *)(&g_vm.mem[(reg)])       \
                                 = *(int32_t *)&g_vm.mem[(adr)])
#define v0sewtreg(reg, val)     (*(v0wreg *)(&g_vm.mem[(reg)])        \
                                 = (val) & 0xffffffff)
#define v0setreg(reg, val)      (*(int32_t *)(&g_vm.mem[(reg)])       \
                                 = (val) & 0xffffffff)
#define v0sethreg(reg, val)     (*(int16_t *)(&g_vm.mem[(reg)])       \
                                 = (val) & 0xffff)
#define v0setbreg(reg, val)     (*(int8_t *)(&g_vm.mem[(reg)])        \
                                 = (val))
#define v0storehi(reg, adr)     ((v0reg *)&g_vm.mem[(adr)]              \
                                 = v0getwreg(reg) >> V0_REG_BITS)
#define v0storelo(reg, adr)     ((v0reg *)&g_vm.mem[(adr)]              \
                                 = v0getwreg(reg) & 0xffffffff)
#define v0storereg(reg, adr)    ((v0reg *)&g_vm.mem[(adr)] = v0getreg(reg))
#define v0storehreg(reg, adr)   ((int16_t *)&g_vm.mem[(adr)] = v0gethreg(reg))
#define v0storebreg(reg, adr)   ((int8_t *)&g_vm.mem[(adr)] = v0getbreg(reg))
#define v0store(val, adr)       ((v0reg *)&g_vm.mem[(adr)] = (val))
#define v0storew(val, adr)      ((v0wreg *)&g_vm.mem[(adr)] = (val))

static __inline__ v0word
v0getadr1(struct v0 *vm, struct v0ins *ins)
{
    v0adr       adr = 0;
    v0word      val = 0;
    v0word      reg = ((ins)->flg & V0_REG_ADR) ? v0getregid1(ins) : -1;
    v0word      ndx = ((ins)->flg & V0_NDX_ADR) ? v0getimm(ins) : 0;

    if (reg >= 0) {
        val = *(v0word *)&vm->regs[reg];
    }
    adr += ndx;
    adr += val;

    return adr;
}

static __inline__ v0word
v0getadr2(struct v0 *vm, struct v0ins *ins)
{
    v0adr       adr = 0;
    v0word      val = 0;
    v0word      reg = ((ins)->flg & V0_REG_ADR) ? v0getregid2(ins) : -1;
    v0word      ndx = ((ins)->flg & V0_NDX_ADR) ? v0getimm(ins) : 0;

    if (reg >= 0) {
        val = *(v0word *)&vm->regs[reg];
    }
    adr += ndx;
    adr += val;

    return adr;
}

/* common flags */
#define V0_UNS_BIT    (1 << 0)  // unsigned operands
#define V0_RD_BIT     (1 << 0)  // read operation
#define V0_WR_BIT     (1 << 1)  // write operation
/* LOGIC-unit instructions */
#define V0_LOGIC_UNIT 0x01
#define V0_AND_BIT    (1 << 0)  // logical AND operation: R2 &= R1
#define V0_EXC_BIT    (1 << 0)  // exclusive [OR] operation: R2 ^= R1
#define V0_OR_BIT     (1 << 1)  // logical OR operation: R2 |= R1
#define V0_NOT_OP     0x00
#define V0_AND_OP     V0_AND_BIT
#define V0_IOR_OP     V0_OR_BIT
#define V0_XOR_OP     (V0_EXC_BIT | V0_OR_BIT)
/* SHIFT-unit */
#define V0_SHIFT_UNIT 0x02
#define V0_DIR_BIT    (1 << 0)  // right shift operations: SHR, SAR, ROR
#define V0_ARI_BIT    (1 << 1)  // arithmetic [right] shift: SAR
#define V0_SAM_BIT    (1 << 2)  // shift and mask operation
#define V0_ROT_BIT    (1 << 2)  // bitwise rotation: ROL, ROR
#define V0_SFS_BIT    (1 << 3)  // fused shift operation
#define V0_SHL_OP     0x00
#define V0_SHR_OP     V0_DIR_BIT
#define V0_SAR_OP     (V0_ARI_BIT
#define V0_ROL_OP     V0_ROT_BIT
#define V0_ROR_OP     (V0_DIR_BIT | V0_ROT_BIT)
#define V0_SLA_OP     V0_SFS_BIT
#define V0_SRA_OP     (V0_DIR_BIT | V0_SFS_BIT)
#define V0_SLM_OP     (V0_SAM_BIT | V0_SFS_BIT)
#define V0_SRM_OP     (V0_DIR_BIT | V0_SAM_BIT | V0_SFS_BIT)
/* ADDER-unit */
#define V0_ADDER_UNIT 0x03
#define V0_DEC_BIT    (1 << 0)
#define V0_CF_BIT     (1 << 0)
#define V0_ADD_BIT    (1 << 1)
#define V0_SUB_BIT    (1 << 2)
#define V0_CMP_MASK   0x06

#define V0_INC_OP     0x00                      // INC, INCU
#define V0_DEC_OP     V0_DEC_BIT                // DEC, DECU
#define V0_ADD_OP     V0_ADD_BIT                // ADD, ADDU
#define V0_ADC_OP     (V0_CF_BIT | V0_ADD_BIT)  // ADC, ADCU
#define V0_SUB_OP     V0_SUB_BIT                // SUB, SUBU
#define V0_SBC_OP     (V0_CF_BIT | V0_SUB_BIT)  // SBC, SBCU
#define V0_CMP_OP     V0_CMP_MASK               // CMP, CMPU
/* MULTI-unit */
#define V0_MULTI_UNIT 0x04
#define V0_DIV_BIT    (1 << 0)  // division
#define V0_REM_BIT    (1 << 1)  // remainder
#define V0_RPC_BIT    (1 << 1)  // inverse reciprocal
#define V0_MUL_BIT    (1 << 2)
#define V0_MUL_OP     0x00
#define V0_DIV_OP     V0_DIV_BIT                // DIV, DIVU
#define V0_REM_OP     V0_REM_BIT                // REM, REMU
#define V0_RPC_OP     (V0_DIV_BIT | V0_RPC_BIT) // RPC, RPCU
#define V0_MLH_OP     V0_MUL_BIT                // MLH, MLHU
/* BIT-unit */
#define V0_BIT_UNIT   0x05
#define V0_BDD_BIT    (1 << 0)
#define V0_HUN_BIT    (1 << 0)
#define V0_ONE_BIT    (1 << 1)
#define V0_NEG_BIT    (1 << 1)  // arithmetic negation: R2 = -R1
#define V0_CNT_BIT    (1 << 2)
#define V0_BCD_BIT    (1 << 3)
#define V0_SWP_MASK   0x03      // swap byte order
#define V0_CNT_MASK   0x06
#define V0_CRC_MASK   0x0a
#define V0_HASH_MASK  0x0c
#define V0_SEX_OP     0x00
#define V0_ZEX_OP     V0_UNS_BIT
#define V0_NEG_OP     V0_NEG_BIT
#define V0_SWP_OP     V0_SWP_MASK
#define V0_PAR_OP     V0_CNT_BIT
#define V0_CLZ_OP     (V0_UNS_BIT | V0_CNT_BIT)
#define V0_HAM_OP     (V0_ONE_BIT | V0_CNT_BIT)
#define V0_BCD_OP     V0_BCD_BIT
#define V0_BDD_OP     (V0_BDD_BIT | V0_BCD_BIT)
#define V0_CRC_OP     V0_CRC_MASK
#define V0_HSH_OP     V0_HASH_MASK
#define V0_HUN_OP     (V0_HUN_BIT | V0_HASH_MASK)
/* MEM-unit */
#define V0_MEM_UNIT   0x06
#define V0_CST_BIT    (1 << 0)
#define V0_CL_BIT     (1 << 2)  // cacheline operation
#define V0_BAR_BIT    (1 << 3)  // memory barrier
#define V0_IPG_MASK   0x03
#define V0_COND_MASK  0x0c
#define V0_STN_MASK   0x0e
#define V0_LEA_OP     0x00
#define V0_LDR_OP     V0_RD_BIT
#define V0_STR_OP     V0_WR_BIT
#define V0_IPG_OP     V0_IPG_MASK
#define V0_CLD_OP     V0_CL_BIT
#define V0_CPF_OP     (V0_RD_BIT | V0_CL_BIT)
#define V0_CFL_OP     (V0_WR_BIT | V0_CL_BIT)
#define V0_BAR_OP     V0_BAR_BIT
#define V0_BRD_OP     (V0_RD_BIT | V0_BAR_BIT)
#define V0_BWR_OP     (V0_WR_BIT | V0_BAR_BIT)
#define V0_CLD_OP     V0_COND_MASK
#define V0_CST_OP     (V0_CST_BIT | V0_COND_MASK)
#define V0_STN_OP     V0_STN_MASK
/* STK-unit */
#define V0_STK_UNIT   0x07
#define V0_POP_BIT    (1 << 0)
#define V0_RNG_BIT    (1 << 1)  // range operation
#define V0_PSH_OP     0x00
#define V0_POP_OP     V0_POP_BIT
#define V0_PSM_OP     V0_RNG_BIT
#define V0_POM_OP     (V0_POP_BIT | V0_RNG_BIT)

/* ATOM-unit */
#define V0_ATOM_UNIT  0x08

#define V0_STR_BIT    (1 << 0)  // atomic operation
#define V0_XINC_BIT   (1 << 0)  // atomic fetch and increment
#define V0_DUAL_BIT   (1 << 0)  // atomic dual-word/pointer compare and swap
#define V0_BT_BIT     (1 << 1)  // atomic bit-test operation (BTC, BTS)
#define V0_XDEC_BIT   (1 << 1)  // atomic fetch and decrement
#define V0_XADD_BIT   (1 << 2)  // atomic fetch and add
#define V0_CAS_BIT    (1 << 3)  // atomic compare and swap

#define V0_LNK_OP     0x00
#define V0_STC_OP     V0_STR_BIT
#define V0_BTC_OP     V0_BT_BIT
#define V0_BTS_BIT    (V0_STR_BIT | V0_BT_BIT)
#define V0_XADD_OP    V0_XADD_BIT
#define V0_XINC_OP    (V0_XINC_BIT | V0_ADD_BIT)
#define V0_XDEC_OP    (V0_XDEC_BIT | V0_ADD_BIT)
#define V0_CAS_OP     V0_CAS_BIT
#define V0_CAS2_OP    (V0_DUAL_BIT | V0_CAS_BIT)
/* FLOW-unit */
#define V0_FLOW_UNIT  0x09
#define V0_NOT_BIT    (1 << 0)
#define V0_REL_BIT    (1 << 0)  // PC-relative addressing
#define V0_OF_BIT     (1 << 0)  // branch on overflow
#define V0_EQ_BIT     (1 << 1)  // branch on equal
#define V0_LT_BIT     (1 << 2)  // branch if less than
#define V0_CF_BIT     (1 << 3)  // branch if greater than
#define V0_OF_MASK    0x0a
#define V0_SF_MASK    0x0e
#define V0_JMP_OP     0x00
#define V0_JMR_OP     V0_REL_BIT
#define V0_BEQ_OP     V0_EQ_BIT
#define V0_BZF_OP     V0_BEQ_OP
#define V0_BNE_OP     (V0_NOT_BIT ¦ V0_EQ_BIT)
#define V0_BNZ_OP     V0_BNE_OP
#define V0_BLT_OP     V0_LT_BIT
#define V0_BGE_OP     (V0_NOT_BIT | V0_LT_BIT)
#define V0_BLE_OP     (V0_EQ_BIT | V0_LT_BIT)
#define V0_BGT_OP     (V0_NOT_BIT | V0_EQ_BIT | V0_LT_BIT)
#define V0_BCF_OP     V0_CF_BIT
#define V0_BNC_OP     (V0_NOT_BIT | V0_CF_BIT)
#define V0_BOF_OP     V0_OF_MASK
#define V0_BNO_OP     (V0_NOT_BIT | V0_OF_MASK)
#define V0_BSF_OP     V0_SF_MASK
#define V0_NSF_OP     (V0_NOT_BIT | V0_SF_MASK)
/* SUBR-unit */
#define V0_SUBR_UNIT  0x0a
#define V0_FIN_BIT    (1 << 0)
#define V0_RET_BIT    (1 << 0)
#define V0_YIELD_BIT  (1 << 0)
#define V0_SUBR_BIT   (1 << 1)
#define V0_WAIT_BIT   (1 << 1)
#define V0_SYS_BIT    (1 << 2)
#define V0_INTR_BIT   (1 << 2)
#define V0_CONF_BIT   (1 << 2)
#define V0_THR_BIT    (1 << 3)
#define V0_BEG_OP     0x00
#define V0_FIN_OP     V0_FIN_BIT
#define V0_CSR_OP     V0_SUBR_BIT
#define V0_RET_OP     (V0_SUBR_BIT | V0_FIN_BIT)
#define V0_SYS_OP     V0_SYS_BIT
#define V0_SRT_OP     (V0_SYS_BIT | V0_RET_BIT)
#define V0_IRT_OP     (V0_INTR_BIT | V0_RET_BIT)
#define V0_THR_OP     V0_THR_BIT
#define V0_THX_OP     (V0_THR_BIT | V0_RET_BIT)
#define V0_THW_OP     (V0_THR_BIT | V0_WAIT_BIT)
#define V0_THY_OP     (V0_THR_BIT | V0_WAIT_BIT | V0_YIELD_BIT)
#define V0_THC_OP     (V0_THR_BIT | V0_CONF_BIT)
/* SYS-unit */
#define V0_SYS_UNIT   0x0b
#define V0_ON_BIT     (1 << 0)  // enable operation
#define V0_EV_BIT     (1 << 1)  // event operation
#define V0_INT_BIT    (1 << 2)  // software interrupt
#define V0_TRAP_BIT   (1 << 3)
#define V0_HLT_OP     0x00
#define V0_RST_OP     V0_ON_BIT
#define V0_WFE_OP     V0_EV_BIT
#define V0_SEV_OP     (V0_ON_BIT | V0_EV_BIT)
#define V0_CLI_OP     V0_INT_BIT
#define V0_STI_OP     (V0_ON_BIT | V0_INT_BIT)
#define V0_INT_OP     V0_TRAP_BIT
/* IO-unit */
#define V0_IO_UNIT    0x0c
#define V0_IOC_OP     0x00
#define V0_IRD_OP     V0_RD_BIT
#define V0_IWD_OP     V0_WR_BIT
#define V0_ILD_OP     0x03
#define V0_IST_OP     0x04

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
#define V0_UNSG_BIT     0x8000  // unsigned operation
#define V0_IMM_BIT      0x4000  // immediate argument follows opcode
#define V0_HALF_BIT     0x2000  // halfword (16-bit) operation
#define V0_BYTE_BIT     0x1000  // byte (8-bit) operation
#define V0_ATOM_BIT     0x0800  // atomic operation / bus-lock
#define V0_FLAG_MASK    0xf800  // code-member flag-bits
#define V0_UNIT_MASK    0x07c0  // coprocessor ID
#define V0_OP_MASK      0x003f  // instruction ID
#define V0_FLAG_BITS    5
#define V0_UNIT_BITS    5
#define V0_OP_BITS      6
#define V0_UNIT_SHIFT   V0_OP_BITS
#define V0_FLAG_SHIFT   (V0_OP_BIITS + V0_UNIT_BITS)
/* parm-member bits; low 12 bits are for register IDs (6-bit each) */
#define V0_VAL_BIT      0x8000  // 15-bit signed argument present in opcode
#define V0_PRIV_BIT     0x4000  // privileged operation [system mode/ring #0]
#define V0_PARM_MASK    0x6000  // flag-bit mask
#define V0_NO_ADR       0x0000  // register-only operands
#define V0_NDX_ADR      0x2000  // indexed, e.g. $4(%sp) or $128(%pc) or direct
#define V0_REG_ADR      0x1000  // base register, e.g. *%r1 or *%pc
#define V0_ADR_MASK     0x3000  // addressing-mode mask
#define V0_REG2_MASK    0x0fc0  // register operand #2 ID
#define V0_REG1_MASK    0x003f  // register operand #1 ID
#define V0_PARM_BITS    2       // execution flag-bits VAL, PRIV
#define V0_ADR_BITS     2       // address-mode for load-store instructions
#define V0_REG_BITS     6       // bits per register ID
struct v0ins {
    uint16_t            code;         // flag-bits, unit, instruction
    uint16_t            parm;         // 4 address-bits + 2 6-bit register IDs
    union v0arg         arg[VLA];     // immediate if (ins->parm & V0_IMM_BIT)
};

#endif /* __V0_VM_INS_H__ */

