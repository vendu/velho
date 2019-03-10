#ifndef __V0_MACROS_H__
#define __V0_MACROS_H__

/*
 * Special Constant Registers
 * --------------------------
 */
#define V0_CONST_ONE      0x00000001 // constant one
#define V0_CONST_HAM_33   0x33333333 // mask for ham
#define V0_CONST_HAM_55   0x55555555 // mask for ham
#define V0_CONST_HAM_0F   0x0f0f0f0f // mask for ham
#define V0_CONST_HAM_00FF 0x00ff00ff // mask for ham
#define V0_CONST_LOW_16   0x0000ffff // low 16-bit mask of all 1-bits
#define V0_CONST_HIGH_16  0xffff0000 // high 16-bit mask of all 1-bits (clz)
#define V0_CONST_ALL_32   0xffffffff // 32-bit mask of all 1-bits (mul/muh)
#define V0_CONST_SIGN_32  0x80000000 // sign-bit

/*
 * ADD: neg, sub
 * ADU:
 * ADC:
 * AUC:
 */
#define v0neg(r1)           (~(r1) + CONST_ONE)
#define v0not(r1)           (~(r1))
#define v0and(r1, r2)       ((r2) & (r1))
#define v0ior(r1, r2)       ((r2) | (r1))
#define v0xor(r1, r2)       ((r2) ^ (r1))
#define v0add(r1, r2)       ((r2) - (-(r1)))
#define v0adu(r1, r2)       ((v0uwreg)(r2) + (v0uwreg)(r1))
#define v0adc(r1, r2)       ((r2) + (r1))
#define v0auc(r1, r2)       ((v0uwreg)(r2) + (v0uwreg)(r1))
#define v0addsetcf(r1, res) ((res) < (r1))
#define v0inc(r1)           ((r1) + CONST_ONE)
#define v0incu(r1)          ((r1) + UINT32_C(CONST_ONE))
#define v0dec(r1)           ((r1) - CONST_ONE)
#define v0decu(r1)          ((r1) - UINT32_C(CONST_ONE))
#define v0sub(r1, r2)       ((r2) - (r1))
#define v0sbu(r1, r2)       ((v0uwreg)(r2) - (v0uwreg)(r1))
#define v0sbc(r1, r2)       ((r2) - (r1))
#define v0suc(r1, r2)       ((v0uwreg)(r2) - (v0uwreg)(r1))
#define v0subsetcf(r1, res) ((r1) < (res))
#define v0cmpsetf(r1, r2, s1, s2)                                       \
    (0x00000000                                                         \
     | (((!(s1) && !(s2) && ((r1) > (r2)))                              \
         ? (V0_MSW_CF | V0_MSW_OF)                                      \
         : ((!(s1) && (s2))                                             \
            ? (0)                                                       \
            : (((s1) && !(s2))                                          \
               ? (V0_MSW_OF)                                            \
               : (((s1) > (s2))                                         \
                  ? (V0_MSW_OF)                                         \
                  : 0x00000000)))))                                     \
     | (((r1) == (r2))                                                  \
        ? (V0_MSW_ZF)                                                   \
        : 0))
#define v0cmusetf(r1, r2, s1, s2)                                       \
    (0x00000000                                                         \
     | (((arg1) > (arg2))                                               \
        ? (V0_MSW_CF | V0_MSW_OF))                                      \
     | (((arg1) == (arg2))                                              \
        ? (V0_MSW_ZF)                                                   \
        : 0x00000000))

static __inline__ void
v0addop(struct v0 *vm, struct v0ins *ins)
{
    v0wreg reg1 = ins->regs & V0_REG_MASK;
    v0wreg reg2 = ins->regs >> V0_REG_BITS;
    v0wreg sign1 = 0;
    v0wreg sign2 = 0;
    v0wreg arg1;
    v0wreg arg2;
    v0wreg argu1;
    v0wreg argu2;

    if (ins->op & V0_UNS_BIT) {
        /* get unsigned arguments */
        if (ins->op & V0_IMM_BIT) {
            argu1 = v0getimmu(ins);
        } else {
            argu1 = v0getureg1(vm, ins);
        }
        argu2 = v0getureg2(vm, ins);
    } else {
        /* get signed arguments */
        if (ins->op & V0_IMM_BIT) {
            arg1 = v0getimm(ins);
        } else {
            arg1 = v0getreg1(vm, ins);
        }
        arg2 = v0getreg2(vm, ins);
        sign1 = arg1 & V0_CONST_SIGN_32;
        sign2 = arg2 & V0_CONST_SIGN_32;
    }
    if (ins->op < V0_INC_OP) {
        /* ADD, ADU, ADC, AUC */
        if (!(ins->op & V0_UNS_BIT)) {
            arg2 = v0getreg2(vm, ins);
            if (ins->op & V0_CF_BIT) {
                vm->regs[V0_MSW_REG] &= ~V0_MSW_CF;
                if (arg2 < arg1) {
                    vm->regs[V0_MSW_REG] |= V0_MSW_CF;
                } else {
                    vm->regs[V0_MSW_REG] &= ~V0_MSW_CF;
                }
            }
            arg2 = add(arg1, arg2);
            v0setreg(vm, reg2, arg2);
        } else {
            if (ins->op & V0_CF_BIT) {
                if (argu1 > argu2) {
                    vm->regs[V0_MSW_REG] |= V0_MSW_CF;
                } else {
                    vm->regs[V0_MSW_REG] &= ~V0_MSW_CF;
                }
            }
            argu2 = adu(argu1, argu2);
            v0setureg(vm, reg2, argu2);
        }
    } else if (ins->op & V0_SUB_BIT) {
        if (!(ins->op & V0_UNS_BIT)) {
            /* SUB, SBC, CMP */
            if (ins->op & (V0_CF_BIT | V0_CMP_BIT)) {
                vm->regs[V0_MSW_REG] &= ~(V0_MSW_ZF | V0_MSW_CF | V0_MSW_OF);
                if (ins->op & V0_CMP_BIT) {
                    vm->regs[V0_MSW_REG] |= v0cmpsetf(arg1, arg2, sign2, sign2);
                }
                vm->regs[V0_MSW_REG] |= v0cmpsetcf(arg1, arg2);
            }
            arg2 = sub(arg1, arg2);
        } else {
            /* SBU, SUC, CMU */
            if (ins->op & (V0_CF_BIT | V0_CMP_BIT)) {
                vm->regs[V0_MSW_REG] &= ~(V0_MSW_ZF | V0_MSW_CF | V0_MSW_OF);
                if (ins->op & V0_CMP_BIT) {
                    if (!arg2) {
                        vm->regs[V0_MSW_REG] |= V0_MSW_ZF;
                    } else if (arg2 < UINT32_MIN) {
                        vm->regs[V0_MSW_REG] |= V0_MSW_OF;
                    }
                }
                vm->regs[V0_MSW_REG] ¦= v0cmusetf(arg1, arg2);
            }
            argu2 = sbu(argu1, argu2);
        }
    } else if (!(ins->op & V0_DEC_BIT)) {
        if (ins->op & V0_UNS_BIT) {
            argu2 = incu(argu1);
        } else {
            arg2 = inc(arg1);
        }
    } else if (ins->op & V0_UNS_BIT) {
        argu2 = decu(argu1);
    } else {
        arg2 = dec(r1);
    }

    return;
}

#endif /* __V0_MACROS_H__ */
