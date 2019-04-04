#ifndef __V0_MACROS_H__
#define __V0_MACROS_H__

/*
 * Special Constant Registers
 * --------------------------
 */
#define V0_CONST_ZERO     	0x00000000 // constant zero
#define V0_CONST_ONE      	0x00000001 // constant one
#define V0_CONST_HIGH_4   	0xf0000000 // high 4-bit mask of 1-bits (clz)
#define V0_CONST_LOW_8          0x000000ff // low 8-bit mask of 1-bits
#define V0_CONST_HIGH_8   	0xff000000 // high 8-bit mask of 1-bits (clz)
#define V0_CONST_LOW_16   	0x0000ffff // low 16-bit mask of 1-bits
#define V0_CONST_HIGH_16  	0xffff0000 // high 16-bit mask of 1-bits (clz)
#define V0_CONST_LOW_24   	0x00ffffff // low 24-bit mask of 1-bits
#define V0_CONST_ALL_32   	0xffffffff // 32-bit mask of 1-bits (mul/muh)
#define V0_CONST_CTZ            0x077cb531 // constant for CTZ
#define V0_CONST_HAM_33   	0x33333333 // mask for ham
#define V0_CONST_HAM_55   	0x55555555 // mask for ham
#define V0_CONST_HAM_0F   	0x0f0f0f0f // mask for ham
#define V0_CONST_HAM_00FF 	0x00ff00ff // mask for ham
)
#define V0_CONST_SIGN_32  	0x80000000 // sign-bit

#define v0alnptr(adr, aln)      (((adr) + (aln) - 1) & ((aln) - 1))
#define v0alnstk(adr, aln)      ((adr) & ((aln) - 1))

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
#define v0getword(vm, adr)      (*(v0word *)(&(vm)->mem[adr]))
#define v0gethalf(vm, adr)      (*(int16_t *)(&(vm)->mem[adr]))
#define v0getbyte(vm, adr)      (*(int8_t *)(&(vm)->mem[adr]))
#define v0getuword(vm, adr)     (*(v0uword *)(&(vm)->mem[adr]))
#define v0getuhalf(vm, adr)     (*(uint16_t *)(&(vm)->mem[adr]))
#define v0getubyte(vm, adr)     (*(uint8_t *)(&(vm)->mem[adr]))
#define v0getflg(ins)           ((ins)->code & V0_INS_FLG_MASK)
#define v0getimm8(ins)          ((imm)->arg[0].i8)
#define v0getimmu8(ins)         ((imm)->arg[0].u8)
#define v0getimm16(ins)         ((imm)->arg[0].i16)
#define v0getimmu16(ins)        ((imm)->arg[0].u16)
#define v0getimm32(ins)         ((imm)->arg[0].i32)
#define v0getimmu32(ins)        ((imm)->arg[0].u32)
#define v0getimm(ins)           ((imm)->arg[0].val)
#define v0getimmu(ins)          ((imm)->arg[0].uval)

#define v0getwreg(reg)          (*(v0wide *)(&g_vm.intregs[(reg)]))
#define v0getreg(reg)           (*(v0word *)(&g_vm.intregs[(reg)]))
#define v0gethreg(reg)          (*(int16_t *)(&g_vm.intregs[(reg)]))
#define v0getbreg(reg)          (*(int8_t *)(&g_vm.intregs[(reg)]))
#define v0getsysreg(reg)        (*(v0word *)(&g_vm.sysregs[(reg)]))
#define v0loadwreg(reg, adr)    (*(v0wide *)(&g_vm.intregs[(reg)])      \
                                 = *(v0word *)&g_vm.mem[(adr)])
#define v0loadreg(reg, adr)     (*(v0word *)(&g_vm.mem[(reg)])          \
                                 = *(v0word *)&g_vm.mem[(adr)])
#define v0loadhreg(reg, adr)    (*(v0word *)(&g_vm.mem[(reg)])          \
                                 = *(int16_t *)&g_vm.mem[(adr)])
#define v0loadbreg(reg, adr)    (*(v0word *)(&g_vm.mem[(reg)])          \
                                 = *(int32_t *)&g_vm.mem[(adr)])
#define v0setwreg(reg, val)     (*(v0wide *)(&g_vm.mem[(reg)])          \
                                 = (val) & 0xffffffff)
#define v0setreg(reg, val)      (*(int32_t *)(&g_vm.mem[(reg)])         \
                                 = (val) & 0xffffffff)
#define v0sethreg(reg, val)     (*(int16_t *)(&g_vm.mem[(reg)])         \
                                 = (val) & 0xffff)
#define v0setbreg(reg, val)     (*(int8_t *)(&g_vm.mem[(reg)])          \
                                 = (val))
#define v0storehi(reg, adr)     ((v0word *)&g_vm.mem[(adr)]             \
                                 = v0getwreg(reg) >> V0_REG_BITS)
#define v0storelo(reg, adr)     ((v0word *)&g_vm.mem[(adr)]             \
                                 = v0getwreg(reg) & 0xffffffff)
#define v0storereg(reg, adr)    ((v0word *)&g_vm.mem[(adr)] = v0getreg(reg))
#define v0storehreg(reg, adr)   ((int16_t *)&g_vm.mem[(adr)] = v0gethreg(reg))
#define v0storebreg(reg, adr)   ((int8_t *)&g_vm.mem[(adr)] = v0getbreg(reg))
#define v0store(val, adr)       ((v0word *)&g_vm.mem[(adr)] = (val))
#define v0storew(val, adr)      ((v0wide *)&g_vm.mem[(adr)] = (val))

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

#endif /* __V0_MACROS_H__ */
