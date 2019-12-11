#ifndef __V0_MACROS_H__
#define __V0_MACROS_H__

#include <stdint.h>
#include <v0/v0.h>

#define v0alnptr(adr, aln)      (((adr) + (aln) - 1) & ((aln) - 1))
#define v0alnstk(adr, aln)      ((adr) & ((aln) - 1))

#define v0isnop(inst)           ((inst)->code == V0_NOP_CODE)
#define v0getflg(inst)          ((inst)->code & V0_INST_FLG_MASK)
//#define v0getimm(inst)          ((inst)->imm[0].val)
//#define v0getimmu(inst)         ((inst)->imm[0].uval)
#define v0getop(inst)    	((inst)->code & V0_OP_MASK)
#define v0getunit(inst)  	(((inst)->code & V0_UNIT_MASK) >> V0_UNIT_SHIFT)
#define v0getregid1(inst)       ((inst)->val & V0_REG_MASK)
#define v0getregid2(inst)       (((inst)->val >> V0_REG_BITS) & V0_REG_MASK)
#define v0getadr(inst)          (((inst)->val & V0_ADR_MASK) >> 2 * V0_REG_BITS)
#define v0getval(inst)          (((inst)->val & V0_VAL_BIT)             \
                                 ? ((inst)->val & ~V0_VAL_BIT)          \
                                 : ((inst)->arg[0].val))
#define v0getword(vm, adr)      (*(v0word *)(&(vm)->mem[adr]))
#define v0gethalf(vm, adr)      (*(int16_t *)(&(vm)->mem[adr]))
#define v0getbyte(vm, adr)      (*(int8_t *)(&(vm)->mem[adr]))
#define v0getuword(vm, adr)     (*(v0uword *)(&(vm)->mem[adr]))
#define v0getuhalf(vm, adr)     (*(uint16_t *)(&(vm)->mem[adr]))
#define v0getubyte(vm, adr)     (*(uint8_t *)(&(vm)->mem[adr]))
#define v0getflg(inst)          ((inst)->code & V0_INST_FLG_MASK)
#define v0getimm8(inst)         ((inst)->arg[0].data.i8)
#define v0getimmu8(inst)        ((inst)->arg[0].data.u8)
#define v0getimm16(inst)        ((inst)->arg[0].data.i16)
#define v0getimmu16(inst)       ((inst)->arg[0].data.u16)
#define v0getimm32(inst)        ((inst)->arg[0].data.i32)
#define v0getimmu32(inst)       ((inst)->arg[0].data.u32)

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

#endif /* __V0_MACROS_H__ */
