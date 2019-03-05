#define dprintf(fmt, ...)                                               \
    fprintf(stderr, "%s:%d: " ## fmt, __FILE__, __LINE__, __VA_ARGS__)
#define dprintfnl(fmt, ...)                                             \
    fprintf(stderr, "%s:%d: " ## fmt ## "\n", __FILE__, __LINE__, __VA_ARGS__)
#define dprintnl()                                                      \
    fprintf(stderr, "\n")

/* LOGIC Unit */
/* NOT, AND, IOR, XOR */
void
vmlogicop(struct v0 *vm, struct v0ins *ins)
{
    long   op = v0getop(ins);
    long   pc;
    long   incr = 4;
    v0wreg arg1;
    long   reg2 = (ins->regs >> 4) & 0x0f;
    v0wreg arg2 = v0getreg2(vm, ins);

    if (ins->xtra & V0_IMM_BIT) {
        incr += 4;
        arg1 = v0getimm(ins);
    } else {
        arg1 = v0getreg1(vm, ins),
    }
    pc = m_atomadd((m_atomic_t *)&vm->regs[VM_PC_REG], (m_atomic_t)incr);
    switch (op) {
        case V0_NOT:                    // bitwise inverse
            /* V0_NOT */
            arg2 = ~arg1;

            break;
        case V0_AND:                    // bitwise AND
            /* V0_AND */
            arg2 &= arg1;

            break;
        case V0_IOR:                    // bitwise inclusive OR
            /* V0_IOR */
            arg2 |= arg1;

            break;
        case V0_XOR:                    // bitwise exclusive OR
            /* V0_XOR */
            arg2 ^= arg1;

            break;
        case V0_NEG:                    // negation
            /* V0_XOR */
            arg2 = -arg1;

            break;
        default:
            abort();

            break;
    }
    v0setreg(vm, reg2, arg2);

    return;
}

void
v0shiftop(struct v0vm *vm, struct v0ins *ins)
{
    long    op = v0getop(ins);
    long    pc;
    long    incr = 4;
    v0wreg  arg1;
    long    reg1 = ins->regs & 0x0f;
    long    reg2 = (ins->regs >> 4) & 0x0f;
    v0uwreg arg2 = v0getureg2(vm, reg2);
    v0uwreg uval;

    if (ins->xtra & V0_IMM_BIT) {
        incr += 4;
        arg1 = v0getimm(ins);
    } else {
        arg1 = v0getreg1(vm, ins),
    }
    pc = m_atomadd((m_atomic_t *)&vm->regs[VM_PC_REG], (m_atomic_t)incr);
    if (op & V0_ROT_BIT) {
        if (!(op & V0_DIR_BIT)) {
            /* SLA - sla I, R1, R2 -> R2 >>= I, R2 += R1 */
            arg2 <<= arg1;
            uval >>= 32 - arg1;         // extract high bits
            arg2 |= uval;               // set rotated bits
        } else {
            /* SRA - sra I, R1, R2 -> R2 <<= I, R2 += R1 */
            arg2 >>= arg1;
            uval <<= 32 - arg1;         // move low bits
            arg2 >>= arg1;              // logical right shift
            arg2 |= uval;               // set rotated bits
        }
    } else if (op & V0_SHA_BIT) {
        if (!(op & V0_DIR_BIT)) {
            /* SLA - sla I, R1, R2 -> R2 >>= I, R2 += R1 */
            arg2 <<= arg1;
            if (ins->xtra & V0_IMM_BIT) {
                arg1 = v0getreg1(vm, ins);
                arg2 += arg1;
            }
        } else {
            /* SRA - sra I, R1, R2 -> R2 <<= I, R2 += R1 */
            arg2 >>= arg1;
            if (ins->xtra & V0_IMM_BIT) {
                arg1 = v0getreg1(vm, ins);
                arg2 += arg1;
            }
        }
    } else if ((op & V0_SAM_MASK) == V0_SAM_MASK) {
        if (!(op & V0_DIR_BIT)) {
            /* SLM - slm I, R1, R2 -> R2 >>= I, R2 &= R1 */
            arg2 <<= arg1;
            if (ins->xtra & V0_IMM_BIT) {
                arg1 = v0getreg1(vm, ins);
                arg2 &= arg1;
            }
        } else {
            /* SRM - srm I, R1, R2 -> R2 <<= I, R2 &= R1 */
            arg2 >>= arg1;
            if (ins->xtra & V0_IMM_BIT) {
                arg1 = v0getreg1(vm, ins);
                arg2 &= arg1;
            }
        }
   } else if (op & V0_DIR_BIT) {
        /* SHR, SAR */
        /* SHR - shr RI1, R2 -> R2 >>= RI1 */
        uval = arg2;                    // duplicate destination value
        arg2 >>= arg1;                  // right shift
        if  (op & V0_ARI_BIT) {         // arithmetic/fill-with-sign
            /* SAR - sar RI1, R2 -> R2 >>>= RI1 */
            uval &= 0x80000000;         // extract sign bit
            uval--;                     // 0x7fffffff for signed
            uval <<= 32 - src;          // construct sign mask
            arg2 |= uval;               // set high bits
        }
    } else {
        /* SHL - shl RI1, R2 -> R2 <<= RI1 */
        arg2 <<= arg1;                  // left shift
    }
    v0setureg(vm, reg2, arg2);

    return;
}

void
v0addop(struct v0 *vm, struct v0ins *ins)
{
    long    op = v0getop(ins);
    long    pc;
    long    incr = 4;
    v0wreg  arg1;
    v0uwreg argu1;
    long    reg2 = (ins->regs >> 4) & 0x0f;
    v0wreg  arg2;
    v0uwreg argu2;

    if (ins->xtra & V0_IMM_BIT) {
        incr += 4;
        arg1 = v0getimm(ins);
    } else {
        arg1 = v0getreg1(vm, ins),
    }
    pc = m_atomadd((m_atomic_t *)&vm->regs[VM_PC_REG], (m_atomic_t)incr);
    if (op & V0_UNS_BIT) {
        /* ADU, AUC, SBU, SUC */
        argu2 = v0getureg2(vm, ins);
        argu1 = v0getureg1(vm, ins);
        if (op & V0_SUB_BIT) {
            /* SBU, SUC */
            if (op & V0_FLG_BIT) {
                /* SUC */
                if (argu1 > argu2) {
                    v0setcf(vm);
                }
            }
            argu2 -= argu1;
        } else {
            /* ADU, AUC */
            argu2 += argu1;
            if (op & V0_FLG_BIT) {
                if (argu2 > UINT32_MAX) {
                    v0setcf(vm);
                }
            }
        }
        v0setureg(vm, reg1, argu2);
    } else if (op & V0_INC_BIT) {
        /* INC, DEC */
        argu1 = v0getreg1(vm, ins);
        /* INC, DEC */
        if (op & V0_DEC_BIT) {
            /* DEC */
            argu1--;
        } else {
            /* INC */
            argu1++;
        }
        v0setureg(vm, reg1, argu1);
    } else {
        /* ADD, ADC, SUB, SBC */
        arg2 = v0getreg2(vm, ins);
        arg1 = v0getreg1(vm, ins);
        if (op & V0_SUB_BIT) {
            /* SUB, SBC */
            if (op & V0_FLG_BIT) {
                if (arg1 > arg2) {
                    v0setcf(vm);
                }
            }
            arg2 -= arg1;
        } else {
            /* ADD, ADC */
            arg2 += arg1;
            if (op & V0_FLG_BIT) {
                if (arg2 > INT32_MAX) {
                    v0setcf(vm);
                }
            }
        }
        v0setreg(vm, reg1, arg2);
    }

    return;
}

void
v0mulop(struct v0 *vm, struct v0ins *ins)
{
    long    op = v0getop(ins);
    long    pc;
    long    incr = 4;
    v0wreg  arg1;
    v0uwreg argu1;
    long    reg2 = (ins->regs >> 4) & 0x0f;
    v0wreg  arg1;
    v0wreg  arg2;
    v0uwreg argu2;

    if (ins->xtra & V0_IMM_BIT) {
        incr += 4;
        arg1 = v0getimm(ins);
    } else {
        arg1 = v0getreg1(vm, ins),
    }
    pc = m_atomadd((m_atomic_t *)&vm->regs[VM_PC_REG], (m_atomic_t)incr);
    if (op & V0_UNS_BIT) {
        /* MLU, MHU, DVU, RMU, RPU */
        argu2 = v0getureg2(vm, ins);
        argu1 = v0getureg1(vm, ins);
        if (op & V0_HI_BIT) {
            /* MHU */
            argu2 *= argu1;
            argu2 >>= 32;
        } else if (op & v0_DIV_BIT) {
            /* DVU */
            argu2 /= argu1;
        } else if (op & v0_REM_BIT) {
            /* RMU */
            argu2 %= argu1;
        } else if (op & v0_RPC_BIT) {
            ;
        } else {
            /* MLU */
            argu2 *= argu1;
        }
        v0setureg(vm, reg2, argu2);
    } else {
        /* MUL, MLH, DIV, REM, RPS */
        arg2 = v0getreg2(vm, ins);
        arg1 = v0getreg1(vm, ins);
        if (op & V0_HI_BIT) {
            /* MHU */
            arg2 *= arg1;
            arg2 >>= 32;
        } else if (op & v0_DIV_BIT) {
            /* DIV */
            arg2 /= arg1;
        } else if (op & v0_REM_BIT) {
            /* REM */
            arg2 %= arg1;
        } else if (op & v0_RPC_BIT) {
            ;
        } else {
            /* MUL */
            arg2 *= arg1;
        }
        v0setreg(vm, reg2, arg2);
    }

    return;
}

void
v0signop(struct v0 *vm, struct v0ins *ins)
{
    long    op = v0getop(ins);
    long    pc;
    long    incr = 4;
    v0wreg  arg1;
    long    reg1 = ins->regs & 0x0f;
    long    reg2 = (ins->regs >> 4) & 0x0f;
    v0wreg  arg2;
    long    sign;

    if (ins->xtra & V0_IMM_BIT) {
        incr += 4;
        arg1 = v0getimm(ins);
    } else {
        arg1 = v0getreg(vm, reg1);
    }
    pc = m_atomadd((m_atomic_t *)&vm->regs[VM_PC_REG], (m_atomic_t)incr);
    if (op & V0_UNS_BIT) {
        if (op & V0_BYTE_BIT) {
            arg2 &= 0xff;
        } else if (op & V0_HALF_BIT) {
            arg2 &= 0xffff;
        }
    } else if (op & V0_BYTE_BIT) {
        sign = arg1 & 0x80;
        if (sign) {
            arg2 = arg1 | 0xffffff00;
        }
    } else if (op & V0_HALF_BIT) {
        sign = arg1 & 0x8000;
        if (sign) {
            arg2 = arg1 | 0xffff0000;
        }
    }
    v0setreg(vm, reg2, arg2);

    return;
}

// count leading zero bits in src32
#define v0clz(src32, out32)                                             \
    do {                                                                \
        uint32_t _res32 = 0;                                            \
        uint32_t _tmp32 = (src32);                                      \
        uint32_t _mask32 = V0_CONST_HIGH_16;                            \
                                                                        \
        if (_tmp32) {                                                   \
            if (!(_tmp32 & _mask32)) {                                  \
                _res32 += 16;                                           \
                _tmp32 <<= 16;                                          \
            }                                                           \
            _mask32 <<= 8;                                              \
            if (!(_tmp32 & _mask32)) {                                  \
                _res32 += 8;                                            \
                _tmp32 <<= 8;                                           \
            }                                                           \
            _mask32 <<= 4;                                              \
            if (!(_tmp32 & _mask32)) {                                  \
                _res32 += 4;                                            \
                _tmp32 <<= 4;                                           \
            }                                                           \
            _mask32 <<= 2;                                              \
            if (!(_tmp32 & _mask32)) {                                  \
                _res32 += 2;                                            \
                _tmp32 <<= 2;                                           \
            }                                                           \
            _mask32 <<= 1;                                              \
            if (!(_tmp32 & _mask32)) {                                  \
                _res32++;                                               \
            }                                                           \
        } else {                                                        \
            _res32 = 32;                                                \
        }                                                               \
        out32 = _res32;                                                 \
    } while (0)

/* compute the Hamming weight, i.e. the number of 1-bits in a */

/* #L1: each 2-bit chunk sums 2 bits */
/* #L2-3: each 4-bit chunk sums 4 bits */
/* #L4: each 8-bit chunk sums 8 bits */
/* #L5: each 16-bit chunk sums 16 bits */
#define v0ham(src32, out32)                                             \
    do {                                                                \
        v0reg _res32 = (src32);                                         \
                                                                        \
        (_res32) = (((_res32) >> 1) & V0_CONST_HAM_55) + ((_res32) & V0_CONST_HAM_55); \
        (_res32) = (((_res32) >> 2) & V0_CONST_HAM_33) + ((_res32) & V0_CONST_HAM_33); \
        (_res32) = (((_res32) >> 4) & V0_CONST_HAM_0F) + ((_res32) & V0_CONST_HAM_0F); \
        (_res32) = (((_res32) >> 8) & V0_CONST_HAM_00FF) + ((_res32) & V0_CONST_HAM_00FF); \
        (out32) = ((_res32) >> 16) + ((_res32) & V0_CONST_LOW_16);      \
    } while (0)

/* 1 if parity is odd, 0 if even */
#define v0bytepar(b) ((0x6996 >> (((b) ^ ((b) >> 4)) & 0x0f)) & 0x01)

void
v0bitop(struct v0 *vm, struct v0ins *ins)
{
    long    op = v0getop(ins);
    long   pc;
    long   incr = 4;
    v0wreg arg1;
    long   reg1 = ins->regs & 0x0f;
    long   reg2 = (ins->regs >> 4) & 0x0f;
    v0wreg arg2;
    v0wreg adr1;
    long   val;

    if (ins->xtra & V0_IMM_BIT) {
        incr += 4;
        arg1 = v0getimm(ins);
    } else {
        arg1 = v0getreg(vm, reg1);
    }
    pc = m_atomadd((m_atomic_t *)&vm->regs[VM_PC_REG], (m_atomic_t)incr);
    if (op == V0_CNT_BIT) {
        /* CLZ */
        v0clz(arg1, arg2);
    } else if ((op & V0_CNT_BIT) && (op & V0_ONE_BIT)) {
        /* HAM */
        v0ham(arg1, arg2);
    } else if (op & V0_PAR_BIT) {
        arg2 = v0bytepar(arg1);
    } else if (op & V0_BCD_BIT) {
        if (op & V0_DCD_BIT) {
            /* DCD */
            arg2 = (arg1 & 0xff) - '\0';
            arg1 >>= 8;
            arg2 <<= 8;
            arg2 |= (arg1 & 0xff) - '\0';
            arg1 >>= 8;
            arg2 <<= 8;
            arg2 |= (arg1 & 0xff) - '\0';
            arg1 >>= 8;
            arg2 <<= 8;
            arg2 |= (arg1 & 0xff) - '\0';
        } else {
            /* BCD */
        }
    } else if (op & V0_COND_BIT) {
        if (op & V0_WR_BIT) {
            /* STC */
        } else {
            /* LNK */
        }
    } else if (op & V0_HSH_MASK) {
        if (op & V0_RD_BIT) {
            /* UNH */
            arg2 = tmunhash32(arg1);
        } else {
            /* HSH */
            arg2 = tmhash32(arg1);
        }
    } else if (op & V0_HALF_BIT) {
        /* SWBH */
        arg2 = arg1;            // arg2 = 0x78563412
        val = arg1 >> 8;        // val  = 0x00000034
        arg2 &= 0xff;           // arg2 = 0x00000012
        arg2 <<= 8;             // arg2 = 0x00001200
        arg2 |= val;            // arg2 = 0x00001234
    } else {
        /* SWBW */
        arg2 = arg1;            // arg2 = 0x78563412
        val = arg1 >> 24;       // val  = 0x00000078
        arg2 >>= 8;             // arg2 = 0x00785634
        arg2 &= 0x0000ff00;     // arg2 = 0x00005600
        arg2 |= val;            // arg2 = 0x00005678
        val = arg1 << 8;        // val  = 0x56341200
        val &= 0x00ff0000;      // val  = 0x00340000
        arg2 |= val;            // arg2 = 0x00345678
        val = arg1 << 24;       // val  = 0x12000000
        arg2 |= val;            // arg2 = 0x12345678
    }
    vm->regs[reg2] = arg2;

    return;
}

#define v0getmem(vm, adr, type)                                         \
    (*((type *)&vm->mem[(adr)]))
#define v0putmem(vm, adr, val, type, mask)                              \
    (*((type *)&vm->mem[(adr)]) = (val) & (mask))
#define v0flstlb(vm, adr)
#define v0flscl(vm, adr)
#define v0membar(vm, flg)                                               \
    (((flg) == V0_RD_BIT)                                               \
     ? v0synrd(vm)                                                      \
     : (((flg) == V0_WR_BIT)                                            \
        ? v0synwr(vm)                                                   \
        : (v0synrd(vm), v0synwr(vm))))

void
v0memop(struct v0 *vm, struct v0ins *ins)
{
    long    op = v0getop(ins);
    long    pc;
    long    incr = 4;
    v0wreg  arg1;
    long    reg1 = ins->regs & 0x0f;
    long    reg2 = (ins->regs >> 4) & 0x0f;
    v0wreg  arg2;
    v0wreg  val;

    if (ins->xtra & V0_IMM_BIT) {
        incr += 4;
        arg1 = v0getimm(ins);
    } else {
        arg1 = v0getreg(vm, reg1);
    }
    pc = m_atomadd((m_atomic_t *)&vm->regs[VM_PC_REG], (m_atomic_t)incr);
    if (!op) {
        val = v0decadr1(vm, ins);
        v0setreg(vm, reg2, val);
    } else if (op == V0_RD_BIT) {
        /* LDR */
        arg2 = v0getreg2(vm, ins);
        if (op & V0_NO_ADR) {
            arg1 = v0getreg1(vm, ins);
        } else if (op & V0_IMM_BIT) {
            arg1 = v0getimm(ins);
        } else {
            val = v0decadr1(vm, ins);
            if (op & V0_BYTE_BIT) {
                arg1 = v0getmem(vm, val, int8_t);
            } else if (op & V0_HALF_BIT) {
                arg1 = v0getmem(vm, val, int16_t);
            } else {
                arg1 = v0getmem(vm, val, int32_t);
            }
        }
        v0setreg(vm, reg2, arg1);
    } else if (op == V0_WR_BIT) {
        /* STR */
        arg2 = v0decadr2(vm, ins);
        if (op & V0_NO_ADR) {
            arg1 = v0getreg1(vm, ins);
        } else if (op & V0_IMM_BIT) {
            arg1 = v0getimm(ins);
        }
        if (op & V0_BYTE_BIT) {
            v0putmem(vm, arg1, arg2, int8_t, 0xff);
        } else if (op & V0_HALF_BIT) {
            v0putmem(vm, arg1, arg2, int16_t, 0xffff);
        } else {
            v0putmem(vm, arg1, arg2, int32_t, 0xffffffff);
        }
    } else if (op == V0_IPG) {
        val = v0decadr1(vm, ins);
        v0flstlb(vm, val);
    } else if (op & V0_CL_BIT) {
        val = v0decadr1(vm, ins);
        if (op & V0_RD_BIT) {
            arg1 = v0getmem(vm, val, int32_t);
        } else {
            v0flscl(vm, val);
        }
    } else if (op & V0_BAR_BIT) {
        val = op & (V0_RD_BIT | V0_WR_BIT);
        v0membar(val);
    } else if (op & V0_MSW_BIT) {
        if (op & V0_RD_BIT) {
            val = vm->regs[V0_MSW_REG];
        } else {
            v0setreg(vm, V0_MSW_REG, arg1);
        }
    }

    return;
}

void
v0stkop(struct v0 *vm, struct v0ins *ins)
{
    long    op = v0getop(ins);
    long    pc;
    long    sp;
    long    incr = -4;
    v0wreg  arg1;
    long    reg1 = ins->regs & 0x0f;
    long    reg2 = (ins->regs >> 4) & 0x0f;
    v0wreg  arg2;
    v0wreg  val;

    val = ins->xtra;
    if (val & V0_IMM_BIT) {
        arg1 = v0getimm(ins);
    } else {
        arg1 = v0getreg(vm, reg1);
    }
    pc = m_atomadd((m_atomic_t *)&vm->regs[VM_PC_REG], (m_atomic_t)incr);
    if (!op) {
        sp = m_atomadd((m_atomic_t *)&vm->regs[VM_SP_REG], (m_atomic_t)incr);
        sp += incr;
        if (ins->flg & V0_BYTE_BIT) {
            v0putmem(vm, sp, arg1, int8_t, 0xff);
        } else if (op & V0_HALF_BIT) {
            v0putmem(vm, sp, arg1, int16_t, 0xffff);
        } else {
            v0putmem(vm, sp, arg1, int32_t, 0xffffffff);
        }
    } else if (op == V0_RD_BIT) {
        incr =  -incr;
        sp = m_atomadd((m_atomic_t *)&vm->regs[VM_SP_REG], (m_atomic_t)incr);
        val = v0getmem(vm, sp, int32_t);
    } else if (op & V0_N_BIT) {
        incr *= reg2 - reg1;
        if (op == V0_N_BIT) {
            sp = m_atomadd((m_atomic_t *)&vm->regs[VM_SP_REG],
                           (m_atomic_t)incr);
            while (reg2 >= reg1) {
                sp -= 4;
                val = v0getreg(vm, reg2);
                v0putmem(vm, sp, val, int32_t, 0xffffffff);
                reg2--;
            }
        } else {
            incr = -incr;
            sp = m_atomadd((m_atomic_t *)&vm->regs[VM_SP_REG],
                           (m_atomic_t)incr);
            while (reg1 <= reg2) {
                val = v0getmem(vm, sp, int32_t);
                v0setreg(vm, reg1, val);
                reg1++;
                sp += 4;
            }
        }
    } else if (op & V0_MSW_BIT) {
        val = ins->xtra & V0_RD_BIT;
        if (val == V0_RD_BIT) {
            incr = -incr;
            sp = m_atomadd((m_atomic_t *)&vm->regs[VM_SP_REG],
                           (m_atomic_t)incr);
            val = v0getmem(vm, sp, int32_t);
            v0setreg(vm, V0_MSW_REG, val);
        } else {
            sp = m_atomadd((m_atomic_t *)&vm->regs[VM_SP_REG],
                           (m_atomic_t)incr);
            val = v0getreg(vm, V0_MSW_REG);
            sp += incr;
            v0putmem(vm, sp, val, int32_t, 0xffffffff);
        }
    }

    return;
}

#define v0clid(adr)         ((uintptr_t)(adr) & ~(V0_CL_SIZE - 1))
#define v0clrcl(vm, adr)    clrbit((vm)->clbits, v0clid(adr))
#define v0markcl(vm, adr)   setbit((vm)->clbits, v0clid(adr))

long
v0trylkcl(struct vm *vm, v0ureg adr)
{
    long        cl = v0clid(adr);
    long        byte = cl >> 3;
    m_atomic_t *bit = &vm->clkbits[byte];
    long        ndx = cl - byte * 8;
    long        val;

    val = !m_cmpsetbit(bit, ndx);

    return val;
}

long
v0lkcl(struct vm *vm, v0ureg adr)
{
    long        cl = v0clid(adr);
    long        byte = cl >> 3;
    m_atomic_t *ptr = &vm->clkbits[byte];
    long        ndx = cl - byte * 8;
    long        bit = 1L << ndx;
    long        val;

    do {
        while (*ptr & bit) {
            ;
        }
        val = !m_cmpsetbit(ptr, ndx);
    } while (!val);

    return val;
}

long
v0unlkcl(struct vm *vm, v0ureg adr)
{
    long        cl = v0clid(adr);
    long        byte = cl >> 3;
    m_atomic_t *bit = &vm->clkbits[byte];
    long        ndx = cl - byte * 8;

    m_clrbit(bit, ndx);

    return val;
}

void
v0atomop(struct v0 *vm, struct v0ins *ins)
{
    long    op = v0getop(ins);
    long    pc;
    long    incr = 4;
    v0wreg  arg1;
    long    reg1 = ins->regs & 0x0f;
    long    reg2 = (ins->regs >> 4) & 0x0f;
    v0wreg  arg2;
    v0ureg  adr;
    v0reg   bit;
    v0wreg  val;

    if (ins->xtra & V0_IMM_BIT) {
        incr += 4;
        arg1 = v0getimm(ins);
    } else {
        arg1 = v0getreg(vm, reg1);
    }
    pc = m_atomadd((m_atomic_t *)&vm->regs[VM_PC_REG], (m_atomic_t)incr);
    if (ins->xtra == V0_WR_BIT) {
        /* STC */
        adr = v0decadr2(vm, ins);
        if (v0trylkcl(vm, adr)) {
            while (reg1 <= reg2) {
                val = vm->regs[reg1];
                v0putmem(vm, adr, val, int32_t, 0xffffffff);
                reg1++;
            }
            v0unlkcl(vm, adr);
        }
    } else {
        adr = v0decadr2(vm, ins);
        v0lkcl(vm, adr);
        val = v0getmem(vm, adr, int32_t);
        if (!ins->xtra) {
            /* LDL */
            v0setreg(vm, reg2, val);
        } else if (ins->xtra & V0_BT_BIT) {
            bit = 1L << arg1;
            if (val & bit) {
                vm->regs[VM_MSW_REG] |= V0_MSW_CF;
            } else {
                vm->regs[VM_MSW_REG] &= ~V0_MSW_CF;
            }
            if (ins->xtra & V0_CLR_BIT) {
                val &= ~bit;
            } else {
                val |= bit;
            }
            v0putmem(vm, adr, val, int32_t, 0xffffffff);
        } else if (ins->xtra & V0_SYN_BIT) {
            bit = 1L << arg1;
            if (ins->xtra & V0_CLR_BIT) {
                val &= ~bit;
            } else {
                val |= bit;
            }
            v0putmem(vm, adr, val, int32_t, 0xffffffff);
        }
        v0unlkcl(vm, adr);
    }

    return;
}

void
v0flowop(struct v0 *vm, struct v0ins *ins)
{
    long    op = v0getop(ins);
    long    pc;
    long    incr = 4;
    v0wureg argu1;
    long    reg1 = ins->regs & 0x0f;
    v0wreg  arg2;
    v0ureg  adr;
    v0wreg  val;

    if (ins->xtra & V0_IMM_BIT) {
        incr += 4;
        argu1 = v0getimmu(ins);
    } else if (!(ins->xtra & V0_NO_ADR)) {
        adr = v0decadr1(vm, ins);
        argu1 = v0getmem(vm, adr, uint32_t);
    } else {
        argu1 = v0getureg(vm, reg1);
    }
    pc = m_atomadd((m_atomic_t *)&vm->regs[VM_PC_REG], (m_atomic_t)incr);
    pc += incr;
    if (!ins->xtra) {
        pc = argu1;
    } else if (ins->xtra == V0_EQ_BIT) {
        if (vm->regs[VM_MSW_REQ] & V0_ZF_BIT) {
            pc = argu1;
        }
    } else if (ins->xtra & V0_NE_BIT) {
        if (!(vm->regs[VM_MSW_REQ] & V0_ZF_BIT)) {
            pc = argu1;
        }
    } else if (ins->xtra & V0_LT_BIT) {
        if (!(vm->regs[VM_MSW_REG] & V0_ZF_BIT)
            && !(vm->regs[VM_MSW_REG] & V0_CF_BIT)) {
            pc = argu1;
        }
    } else if (ins->xtra & V0_GT_BIT) {
        if (!(vm->regs[VM_MSW_REG] & V0_ZF_BIT)
            && (vm->regs[VM_MSW_REG] & V0_CF_BIT)) {
            pc = argu1;
        }
    } else if (ins->xtra & V0_BFL_BIT) {
        if (ins->xtra & V0_BOF_BIT) {
            if (vm->regs[VM_MSW_REG & V0_OF_BIT)) {
                pc = argu1;
            }
        } else if (vm->regs[VM_MSW_REG & V0_CF_BIT)) {
            pc = argu1;
        }
    }
    m_atomwrite((m_atomic_t *)&vm->regs[VM_PC_REG], pc);

    return;
}

void
v0loop(struct v0 *vm, v0ureg pc)
{
    do {
        void          *op = &vm->mem[pc];
        unsigned long  code = ((struct v0op *)op)->code;
        v0vmopfunc_t  *func = v0vmoptab[code];

        if (func) {
            func(vm, op);
        }
    } while (1);
}

