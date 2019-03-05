#include <v0/vm/conf.h>
#include <vas/conf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
//#include <zero/fastudiv.h>
#include <v0/vm/types.h>
#include <v0/vm/isa.h>
#include <v0/vm/ins.h>
#include <v0/vm/vm.h>
#include <v0/vm/io.h>
#include <v0/vm/op.h>
#include <vas/vas.h>

extern void   vasinit(void);
extern long * vasgetinst(char *str);
extern long   vasaddop(const char *str, uint8_t code, uint8_t narg);

#if !defined(__GNUC__) && 0
#define _V0INSFUNC_T __inline__ uint32_t
#else
#define _V0INSFUNC_T uint32_t
#endif

extern struct vasop     *v0optab[256];
extern char             *vaslinebuf;
extern long              vasreadbufcur;
extern vasuword          _startadr;
extern vasuword          _startset;
#if defined(V0_DEBUG_TABS)
static struct v0insinfo  v0insinfotab[V0_INSTS_MAX];
#endif
char                    *v0insnametab[V0_INSTS_MAX];
#if defined(__GNUC__) && 0
#define v0entry(x)                                                      \
    v0ins##x:                                                           \
    pc = v0##x(vm, pc);                                                 \
    insjmp(vm, pc)
#define _v0insadr(x) &&v0##xins
#define _V0INSTAB_T  void *
#else
#define _v0insadr(x) v0##x##ins
typedef v0reg        v0insfunc(struct v0 *vm, v0ureg pc);
#define _V0INSTAB_T  v0insfunc *
#endif
//static  _V0INSTAB_T *v0jmptab[V0_INSTS_MAX];
struct v0           *v0vm;
#if defined(V0_GAME)
static long long     v0speedcnt;
#endif
static v0insfunc    *v0jmptab[V0_INSTS_MAX];

static _V0INSFUNC_T
v0nop(struct v0 *vm, v0ureg pc)
{
    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0not(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *sptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = *sptr;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    src = ~src;
    vm->regs[V0_PC_REG] = pc;
    *sptr = src;

    return pc;
}

static _V0INSFUNC_T
v0and(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest &= src;
    vm->regs[V0_PC_REG] = pc;
    *dptr = dest;

    return pc;
}

static _V0INSFUNC_T
v0ior(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest |= src;
    vm->regs[V0_PC_REG] = pc;
    *dptr = dest;

    return pc;
}

static _V0INSFUNC_T
v0xor(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest ^= src;
    vm->regs[V0_PC_REG] = pc;
    *dptr = dest;

    return pc;
}

static _V0INSFUNC_T
v0shl(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest <<= src;
    vm->regs[V0_PC_REG] = pc;
    *dptr = dest;

    return pc;
}

static _V0INSFUNC_T
v0shr(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;
    v0reg         fill = ~((v0reg)0) >> src;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest >>= src;
    dest &= fill;
    vm->regs[V0_PC_REG] = pc;
    *dptr = dest;

    return pc;
}

static _V0INSFUNC_T
v0sar(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;
    v0reg         mask = ~(v0reg)0;
#if (V0_WORD_SIZE == 8)
    v0reg        fill = (((dest) & (INT64_C(1) << 63))
                         ? (mask >> (64 - src))
                         : 0);
#else
    v0reg        fill = (((dest) & (INT32_C(1) << 31))
                         ? (mask >> (32 - src))
                         : 0);
#endif

    dest >>= src;
    fill = -fill << (8 * sizeof(v0reg) - src);
    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest &= fill;
    vm->regs[V0_PC_REG] = pc;
    *dptr = dest;

    return pc;
}

static _V0INSFUNC_T
v0inc(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *sptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = *sptr;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    src++;
    vm->regs[V0_PC_REG] = pc;
    *sptr = src;

    return pc;
}

static _V0INSFUNC_T
v0dec(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *sptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = *sptr;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    src--;
    vm->regs[V0_PC_REG] = pc;
    *sptr = src;

    return pc;
}

static _V0INSFUNC_T
v0add(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;
    v0reg         res = src;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    res += dest;
    if (res < dest) {
        v0setof(vm);
    }
    vm->regs[V0_PC_REG] = pc;
    *dptr = dest;

    return pc;
}

/* FIXME: set carry-bit */
static _V0INSFUNC_T
v0adc(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;
    v0reg         res = src;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    res += dest;
    if (res < dest) {
        v0setof(vm);
    }
    vm->regs[V0_PC_REG] = pc;
    *dptr = dest;

    return pc;
}

static _V0INSFUNC_T
v0sub(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest -= src;
    vm->regs[V0_PC_REG] = pc;
    *dptr = dest;

    return pc;
}

static _V0INSFUNC_T
v0sbc(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest -= src;
    vm->regs[V0_PC_REG] = pc;
    *dptr = dest;

    return pc;
}

static _V0INSFUNC_T
v0cmp(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 1,  ptr);
    v0reg         src = v0getarg1(vm, ins, v0reg, ptr);
    v0reg         dest = *dptr;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest -= src;
    v0clrmsw(vm);
    if (!dest) {
        v0setzf(vm);
    } else if (dest < 0) {
        v0setcf(vm);
    }
    *dptr = dest;
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0crp(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0wreg        src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg        res;
}

#if 0
static _V0INSFUNC_T
v0crp(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc)
    struct v0ins *ins = (struct v0ins *)ptr;
    v0wreg        src = v0getarg1(vm, ins, v0reg, ptr);
    double       *dptr = (double *)v0getadr(vm, ins, 2, ptr);
    double        res = 1.0 / src;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    *dptr = res;

    return pc;
}
#endif

static _V0INSFUNC_T
v0mul(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 2, ptr);
    double        src = (double)v0getarg1(vm, ins, v0reg, ptr);
    double        res = *(double *)dptr;
    v0wreg        dest;

    res *= src;
    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest = (v0wreg)res;
    vm->regs[V0_PC_REG] = pc;
    dest &= 0xffffffff;
    *dptr = (v0reg)dest;

    return pc;
}

static _V0INSFUNC_T
v0muh(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg        *dptr = v0getadr(vm, ins, 2, ptr);
    double        src = (double)v0getarg1(vm, ins, v0reg, ptr);
    double        res = *(double *)dptr;
    v0wreg        dest;

    res *= src;
    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    dest = (v0wreg)res;
    vm->regs[V0_PC_REG] = pc;
    dest >>= 32;
    *dptr = (v0reg)dest;

    return pc;
}

static _V0INSFUNC_T
v0div(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0wreg        src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg        res;
}

static _V0INSFUNC_T
v0sex(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0wreg        src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg        res;
}

static _V0INSFUNC_T
v0lea(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0wreg        src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg        res;
}

static _V0INSFUNC_T
v0clz(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0wreg        src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg        res;
}

static _V0INSFUNC_T
v0ham(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0wreg        src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg        res;
}

static _V0INSFUNC_T
v0bsw(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0wreg        src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg        res;
}

static _V0INSFUNC_T
v0bts(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0wreg        src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg        res;
}

static _V0INSFUNC_T
v0btc(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0wreg        src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg        res;
}

static _V0INSFUNC_T
v0ldr(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0reg         reg;
    v0reg         parm = ins->parm;
    v0reg        *dptr = v0regtoptr(vm, v0insreg(ins, 1));
    v0reg        *sptr = NULL;
    v0reg         src = 0;
    v0reg         mask;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    if (v0insreg(ins, 0)) {
        reg = v0insreg(ins, 0);
        mask = 8 << parm;
        sptr = v0regtoptr(vm, reg);
        mask--;
        src = *sptr;
        src &= mask;
        *dptr |= src;
    } else {
        if (v0insissigned(ins)) {
            switch (parm) {
                case 0:
                    src = *(int8_t *)sptr;

                    break;
                case 1:
                    src = *(int16_t *)sptr;

                    break;
                case 2:
                    src = *(int32_t *)sptr;

                    break;
#if 0
                case 3:
                    wsrc = *(int64_t *)sptr;

                    break;
#endif
            }
            *dptr = src;
        } else {
            switch (ins->parm) {
                case 0:
                    *(v0ureg *)dptr = *(uint8_t *)sptr;

                    break;
                case 1:
                    *(v0ureg *)dptr = *(uint16_t *)sptr;

                    break;
                case 2:
                    *(v0ureg *)dptr = *(uint32_t *)sptr;

                    break;
#if 0
                case 3:
                    uwsrc = *(uint64_t *)sptr;

                    break;
#endif
            }
        }
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0str(struct v0 *vm, v0ureg pc)
{
    long          ptr = v0adrtoptr(vm, pc)
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg         reg;
    v0reg        *adr = v0getadr(vm, ins, 2, ptr);
    v0reg         src;
    v0ureg        usrc;
    v0reg         parm = ins->parm;
    v0reg        *dptr;
    v0reg        *sptr = v0adrtoptr(vm, v0insreg(ins, 0));
    v0reg         mask;

    if (!adr) {
        v0doxcpt(V0_INV_MEM_ADR);
    }
    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    if (v0insreg(ins, 0)) {
        reg = v0insreg(ins, 1);
        mask = 1 << (parm + 8);
        src = *sptr;
        mask--;
        dptr = v0regtoptr(vm, reg);
        src &= mask;
        *dptr |= src;
    } else {
        if (v0insissigned(ins)) {
            switch (ins->parm) {
                case 0:
                    src = *(int8_t *)adr;
                    *(int8_t *)adr = (int8_t)src;

                    break;
                case 1:
                    src = *(int16_t *)adr;
                    *(int16_t *)adr = (int16_t)src;

                    break;
                case 2:
                    src = *(int32_t *)adr;
                    *(int32_t *)adr = (v0reg)src;

                    break;
                case 3:
                    v0doxcpt(V0_INV_MEM_WRITE);

                    break;
            }
        } else {
            switch (ins->parm) {
                case 0:
                    usrc = *(uint8_t *)adr;
                    *(uint8_t *)adr = (uint8_t)usrc;

                    break;
                case 1:
                    usrc = *(uint16_t *)adr;
                    *(uint16_t *)adr = (uint16_t)usrc;

                    break;
                case 2:
                    usrc = *(int32_t *)adr;
                    *(uint32_t *)adr = (uint32_t)usrc;

                    break;
                case 3:
                    v0doxcpt(V0_INV_MEM_WRITE);

                    break;
            }
        }
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0rsr(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0reg        reg;
    v0reg        parm = ins->parm;
    v0reg       *dptr = v0regtoptr(vm, v0insreg(ins, 1));
    v0reg       *sptr = NULL;
    v0reg        src = 0;
    v0reg        mask;

    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    if (v0insreg(ins, 0)) {
        reg = v0insreg(ins, 0) + V0_INT_REGS;
        mask = 8 << parm;
        sptr = v0regtoptr(vm, reg);
        mask--;
        src = *sptr;
        src &= mask;
        *dptr |= src;
    } else {
        if (v0insissigned(ins)) {
            switch (parm) {
                case 0:
                    src = *(int8_t *)sptr;

                    break;
                case 1:
                    src = *(int16_t *)sptr;

                    break;
                case 2:
                    src = *(int32_t *)sptr;

                    break;
#if 0
                case 3:
                    wsrc = *(int64_t *)sptr;

                    break;
#endif
            }
            *dptr = src;
        } else {
            switch (ins->parm) {
                case 0:
                    *(v0ureg *)dptr = *(uint8_t *)sptr;

                    break;
                case 1:
                    *(v0ureg *)dptr = *(uint16_t *)sptr;

                    break;
                case 2:
                    *(v0ureg *)dptr = *(uint32_t *)sptr;

                    break;
#if 0
                case 3:
                    uwsrc = *(uint64_t *)sptr;

                    break;
#endif
            }
        }
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0wsr(struct v0 *vm, v0ureg pc)
{
    long          ptr = v0adrtoptr(vm, pc)
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg         reg;
    v0reg        *adr = v0getadr(vm, ins, 2, ptr);
    v0reg         src;
    v0ureg        usrc;
    v0reg         parm = ins->parm;
    v0reg        *dptr;
    v0reg        *sptr = v0adrtoptr(vm, v0insreg(ins, 0));
    v0reg         mask;

    if (!adr) {
        v0doxcpt(V0_INV_MEM_ADR);
    }
    if (v0insreg(ins, 0) || v0getadrmode(ins) == V0_IMM_ADR) {
        pc += sizeof(struct v0ins);
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
    }
    if (v0insreg(ins, 0)) {
        reg = v0insreg(ins, 1) + V0_INT_REGS;
        mask = 1 << (parm + 8);
        src = *sptr;
        mask--;
        dptr = v0regtoptr(vm, reg);
        src &= mask;
        *dptr |= src;
    } else {
        if (v0insissigned(ins)) {
            switch (ins->parm) {
                case 0:
                    src = *(int8_t *)adr;
                    *(int8_t *)adr = (int8_t)src;

                    break;
                case 1:
                    src = *(int16_t *)adr;
                    *(int16_t *)adr = (int16_t)src;

                    break;
                case 2:
                    src = *(int32_t *)adr;
                    *(int32_t *)adr = (int32_t)src;

                    break;
                case 3:
                    v0doxcpt(V0_INV_MEM_WRITE);

                    break;
            }
        } else {
            switch (ins->parm) {
                case 0:
                    usrc = *(uint8_t *)adr;
                    *(uint8_t *)adr = (uint8_t)usrc;

                    break;
                case 1:
                    usrc = *(uint16_t *)adr;
                    *(uint16_t *)adr = (uint16_t)usrc;

                    break;
                case 2:
                    usrc = *(int32_t *)adr;
                    *(uint32_t *)adr = (uint32_t)usrc;

                    break;
                case 3:
                    v0doxcpt(V0_INV_MEM_WRITE);

                    break;
            }
        }
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

#if defined(V0_MULTICORE)

static _V0INSFUNC_T
v0ldl(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0wreg       src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg       res;
}

static _V0INSFUNC_T
v0stc(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0wreg       src = v0getarg1(vm, ins, v0reg, ptr);
    v0wreg       res;
}

#else /* !defined(V0_MULTICORE) */

static _V0INSFUNC_T
v0ldl(struct v0 *vm, v0ureg pc)
{
    pc = v0ldr(vm, pc);

    return pc;
}

static _V0INSFUNC_T
v0stc(struct v0 *vm, v0ureg pc)
{
    pc = v0str(vm, pc);

    return pc;
}

#endif /* V0_MULTICORE */

static _V0INSFUNC_T
v0jmp(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr;
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc = v0getjmpofs(vm, ins, ptr);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0beq(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr;
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0reg         dest = v0getjmpofs(vm, ins, ptr);

    if (v0zfset(vm)) {
        pc = dest;
    } else {
        pc += sizeof(struct v0ins);
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0bne(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr;
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0reg         dest = v0getjmpofs(vm, ins, ptr);

    if (!v0zfset(vm)) {
        pc = dest;
    } else {
        pc += sizeof(struct v0ins);
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0blt(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr;
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0reg        dest = v0getjmpofs(vm, ins, ptr);

    if (!v0ofset(vm)) {
        pc = dest;
    } else {
        pc += sizeof(struct v0ins);
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0bul(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr;
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0ureg        dest = v0getjmpofs(vm, ins, ptr);

    if (!v0ofset(vm)) {
        pc = dest;
    } else {
        pc += sizeof(struct v0ins);
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0bgt(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr;
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0reg         dest = v0getjmpofs(vm, ins, ptr);

    if (v0ofset(vm)) {
        pc = dest;
    } else {
        pc += sizeof(struct v0ins);
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0bug(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr;
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0reg         dest = v0getjmpofs(vm, ins, ptr);

    if (v0ofset(vm) || v0zfset(vm)) {
        pc = dest;
    } else {
        pc += sizeof(struct v0ins);
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0baf(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0reg         dest = v0getjmpofs(vm, ins, ptr);

    if (v0ofset(vm) || v0zfset(vm)) {
        pc = dest;
    } else {
        pc += sizeof(struct v0ins);
    }
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

/*
 * call
 * ----
 * - push return address
 *
 * stack frame after call - CREATED BY COMPILER
 * ----------------------
 * r7
 * r6
 * r5
 * r4
 * r3
 * r2
 * r1
 * r0
 * argN
 * ...
 * arg0 <- after compiler has generated stack frame
 * retadr <- sp
 */
static _V0INSFUNC_T
v0csr(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr=  v0adrtoptr(vm, pc);
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0ureg        sp = vm->regs[V0_SP_REG];
    v0ureg        dest = v0getarg1(vm, ins, v0reg, ptr);
    v0reg        *sptr;

    pc += sizeof(struct v0ins);
    sp -= sizeof(v0reg);
    sptr = v0adrtoptr(vm, sp);
    *sptr = pc;
    vm->regs[V0_PC_REG] = dest;
    vm->regs[V0_SP_REG] = sp;

    return dest;
}

/* create subroutine stack-frame;
 * - push frame pointer
 * - cinsy stack pointer to frame pointer
 * - push callee-save registers r8..r15
 * - allocate room for local variables on stack
 */
/*
 * stack after enter
 * -----------------
 * retadr
 * oldfp <- fp
 * r15
 * ...
 * r8
 * var0
 * ...
 * varN <- sp
 */
static _V0INSFUNC_T
v0beg(struct v0 *vm, v0ureg pc)
{
    v0reg        *ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = (struct v0ins *)ptr;
    v0ureg        fp = vm->regs[V0_SP_REG];
    v0ureg        sp = vm->regs[V0_SP_REG];
    v0reg        *rptr = v0regtoptr(vm, V0_R0_REG);
    v0reg         nb = v0getarg1(vm, ins, v0reg, ptr);
    v0reg        *sptr;

    /* set stack frame up */
    sp -= sizeof(v0reg);
    sptr = v0adrtoptr(vm, sp);
    vm->regs[V0_FP_REG] = sp;
    *sptr = fp;
    pc += sizeof(struct v0ins);
    sptr -= V0_SAVE_REGS;
    sp -= V0_SAVE_REGS * sizeof(v0reg);
    sptr[V0_R8_REG] = rptr[V0_R8_REG];
    sptr[V0_R9_REG] = rptr[V0_R9_REG];
    sptr[V0_R10_REG] = rptr[V0_R10_REG];
    sptr[V0_R11_REG] = rptr[V0_R11_REG];
    sp -= nb;
    sptr[V0_R12_REG] = rptr[V0_R12_REG];
    sptr[V0_R13_REG] = rptr[V0_R13_REG];
    sptr[V0_R14_REG] = rptr[V0_R14_REG];
    sptr[V0_R15_REG] = rptr[V0_R15_REG];
    vm->regs[V0_SP_REG] = sp;
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

/* destroy subroutine stack-frame
 * - return value is in r0
 * - deallocate local variables
 * - pins callee save registers r8..r15
 * - pins caller frame pointer
 */
/*
 * stack after fin
 * -----------------
 * retadr <- sp
 * oldfp
 * callee save registers r8..r15
 * local variables
 */
static _V0INSFUNC_T
v0fin(struct v0 *vm, v0ureg pc)
{
    v0ureg  sp = vm->regs[V0_FP_REG];
    v0ureg  fp;
    v0reg  *rptr = v0regtoptr(vm, V0_R0_REG);
    v0reg  *sptr = v0adrtoptr(vm, sp);

    pc += sizeof(struct v0ins);
    sptr -= V0_SAVE_REGS + 1;
    rptr[V0_R15_REG] = sptr[0];
    rptr[V0_R14_REG] = sptr[1];
    sp += sizeof(v0reg);
    rptr[V0_R13_REG] = sptr[2];
    rptr[V0_R12_REG] = sptr[3];
    rptr[V0_R11_REG] = sptr[4];
    rptr[V0_R10_REG] = sptr[5];
    sptr = v0adrtoptr(vm, sp);
    rptr[V0_R9_REG] = sptr[6];
    rptr[V0_R8_REG] = sptr[7];
    sptr = v0adrtoptr(vm, sp);
    vm->regs[V0_PC_REG] = pc;
    fp = *sptr;
    vm->regs[V0_SP_REG] = sp;
    vm->regs[V0_FP_REG] = fp;

    return pc;
}

/* return from subroutine;
 * - pins return address
 */
/*
 * stack after ret
 * ---------------
 * r7
 * ...
 * r0
 * argN
 * ...
 * arg0 <- sp
 * retadr
 */
static _V0INSFUNC_T
v0ret(struct v0 *vm, v0ureg pc)
{
    v0ureg  sp = vm->regs[V0_SP_REG];
    v0reg  *sptr = v0adrtoptr(vm, sp);

    sp -= sizeof(v0reg);
    pc = *sptr;
    vm->regs[V0_SP_REG] = sp;
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0sys(struct v0 *vm, v0ureg pc)
{
    v0ureg  sp = vm->regs[V0_SP_REG];
    v0reg  *sptr = v0adrtoptr(vm, sp);

    return pc;
}

static _V0INSFUNC_T
v0srt(struct v0 *vm, v0ureg pc)
{
    v0ureg  sp = vm->regs[V0_SP_REG];
    v0reg  *sptr = v0adrtoptr(vm, sp);

    return pc;
}

static _V0INSFUNC_T
v0thr(struct v0 *vm, v0ureg pc)
{
    v0ureg  sp = vm->regs[V0_SP_REG];
    v0reg  *sptr = v0adrtoptr(vm, sp);

    return pc;
}

static _V0INSFUNC_T
v0thx(struct v0 *vm, v0ureg pc)
{
    v0ureg  sp = vm->regs[V0_SP_REG];
    v0reg  *sptr = v0adrtoptr(vm, sp);

    return pc;
}

#if 0
void
v0mkframe(struct v0 *vm, size_t narg, v0reg *tab)
{
    v0ureg  pc = vm->regs[V0_PC_REG];
    v0ureg  sp = vm->regs[V0_SP_REG];
    v0reg  *rptr = v0regtoptr(vm, V0_R0_REG);
    v0reg  *sptr;
    v0reg   n;

    sp -= V0_SAVE_REGS;
    pc += sizeof(struct v0ins);
    sptr = v0adrtoptr(vm, sp);
    sptr[V0_R0_REG] = rptr[V0_R0_REG];
    sptr[V0_R1_REG] = rptr[V0_R1_REG];
    sptr[V0_R2_REG] = rptr[V0_R2_REG];
    sptr[V0_R3_REG] = rptr[V0_R3_REG];
    n = narg;
    sptr[V0_R4_REG] = rptr[V0_R4_REG];
    sptr[V0_R5_REG] = rptr[V0_R5_REG];
    sptr[V0_R6_REG] = rptr[V0_R6_REG];
    sptr[V0_R7_REG] = rptr[V0_R7_REG];
    tab += narg;
    while (n > 8) {
        n = min(narg, 4);
        sptr -= n;
        tab -= n;
        sp -= n;
        switch (narg) {
            case 4:
                sptr[3] = tab[3];
            case 3:
                sptr[2] = tab[2];
            case 2:
                sptr[1] = tab[1];
            case 1:
                sptr[0] = tab[0];
            case 0:

                break;
        }
    }
    vm->regs[V0_SP_REG] = sp;
    if (n) {
        tab -= narg;
        switch (n) {
            case 8:
                rptr[7] = tab[7];
            case 7:
                rptr[6] = tab[6];
            case 6:
                rptr[5] = tab[5];
            case 5:
                rptr[4] = tab[4];
            case 4:
                rptr[3] = tab[3];
            case 3:
                rptr[2] = tab[2];
            case 2:
                rptr[1] = tab[1];
            case 1:
                rptr[0] = tab[0];

                break;
        }
    }

    return;
}
#endif

/* call epilogue;
 * - number of return values (0, 1, 2) in ins->parm
 * - get possible return value from r0
 * - deallocate stack arguments
 * - restore caller-save registers r0..r7
 * - set r0 to return value
 */
static _V0INSFUNC_T
v0rmframe(struct v0 *vm, size_t narg)
{
    v0reg  *rptr = v0regtoptr(vm, V0_R0_REG);
    v0ureg  pc = vm->regs[V0_PC_REG];
    v0ureg  sp = vm->regs[V0_SP_REG];
    v0reg  *sptr;

    /* adjust SP past stack arguments */
    sp += narg;
    pc += sizeof(struct v0ins);
    /* store r0 and r1 in ret and rethi */
    sptr = v0adrtoptr(vm, sp);
    sp += V0_SAVE_REGS * sizeof(struct v0ins);
    /* restore caller-save registers */
    rptr[V0_R0_REG] = sptr[V0_R0_REG];
    rptr[V0_R1_REG] = sptr[V0_R1_REG];
    rptr[V0_R2_REG] = sptr[V0_R2_REG];
    rptr[V0_R3_REG] = sptr[V0_R3_REG];
    rptr[V0_R4_REG] = sptr[V0_R4_REG];
    rptr[V0_R5_REG] = sptr[V0_R5_REG];
    rptr[V0_R6_REG] = sptr[V0_R6_REG];
    rptr[V0_R7_REG] = sptr[V0_R7_REG];
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0psh(struct v0 *vm, v0ureg pc)
{
    long          ptr = v0adrtoptr(vm, pc);
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0ureg        sp = vm->regs[V0_SP_REG];
    v0reg        *sptr = v0getadr(vm, ins, 1, ptr);
    v0reg        *dptr;

    sp -= sizeof(v0reg);
    pc += sizeof(struct v0ins);
    dptr = v0adrtoptr(vm, sp);
    if (!v0insreg(ins, 0)) {
        pc += sizeof(union v0insarg);
    }
    *dptr = *sptr;
    vm->regs[V0_SP_REG] = sp;
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

/*
 * PSM takes the lowest register ID to be pushed and shift count
 * for number of registers in ins->parm
 */
static _V0INSFUNC_T
v0psm(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0ureg       sp = vm->regs[V0_SP_REG];
    v0reg        lo = v0getarg1(vm, ins, v0reg, ptr);
    v0reg        cnt = 4 << ins->parm;
    v0reg       *sptr = v0regtoptr(vm, lo);
    v0reg       *dptr = v0adrtoptr(vm, sp);
    v0reg        num;

    pc += sizeof(struct v0ins);
    dptr -= cnt;
    while (cnt) {
        num = min(8, cnt);
        switch (num) {
            case 8:
                dptr[7] = sptr[7];
            case 7:
                dptr[6] = sptr[6];
            case 6:
                dptr[5] = sptr[5];
            case 5:
                dptr[4] = sptr[4];
            case 4:
                dptr[3] = sptr[3];
            case 3:
                dptr[2] = sptr[2];
            case 2:
                dptr[1] = sptr[1];
            case 1:
                dptr[0] = sptr[0];
            case 0:

                break;
        }
        sptr += num;
        dptr += num;
        cnt -= num;
    }
    vm->regs[V0_SP_REG] = sp;
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0pins(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0ureg       sp = vm->regs[V0_SP_REG];
    v0reg        reg = ins->val;
    v0reg       *sptr = v0adrtoptr(vm, sp);
    v0reg       *dest = v0regtoptr(vm, reg);

    pc += sizeof(struct v0ins);
    sp += sizeof(v0reg);
    *dest = *sptr;
    vm->regs[V0_SP_REG] = sp;
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0pom(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0ureg       sp = vm->regs[V0_SP_REG];
    v0reg        lo = ins->val;
    v0reg        cnt = 4 << ins->parm;
    v0reg       *sptr = v0adrtoptr(vm, sp);
    v0reg       *dptr = v0regtoptr(vm, lo);
    v0reg        ndx = lo;
    v0reg        num;

    pc += sizeof(struct v0ins);
    sptr -= cnt;
    sp -= cnt;
    while (cnt) {
        num = min(8, cnt);
        switch (num) {
            case 8:
                dptr[7] = sptr[7];
            case 7:
                dptr[6] = sptr[6];
            case 6:
                dptr[5] = sptr[5];
            case 5:
                dptr[4] = sptr[4];
            case 4:
                dptr[3] = sptr[3];
            case 3:
                dptr[2] = sptr[2];
            case 2:
                dptr[1] = sptr[1];
            case 1:
                dptr[0] = sptr[0];
            case 0:

                break;
        }
        cnt -= num;
        sptr += num;
        dptr += num;
    }
    vm->regs[V0_SP_REG] = sp;
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0cpf(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    return pc;
}

#if defined(V0_MULTICORE)

static _V0INSFUNC_T
v0bar(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    return pc;
}

static _V0INSFUNC_T
v0bwr(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    return pc;
}

static _V0INSFUNC_T
v0brd(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    return pc;
}

static _V0INSFUNC_T
v0wfe(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0sev(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

#else /* !defined(V0_MULTICORE) */

static _V0INSFUNC_T
v0bar(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0bwr(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0brd(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0wfe(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0sev(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

#endif /* V0_MULTICORE */

static _V0INSFUNC_T
v0icd(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    return pc;
}

static _V0INSFUNC_T
v0imm(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    return pc;
}

static _V0INSFUNC_T
v0ird(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    return pc;
}

static _V0INSFUNC_T
v0iwr(struct v0 *vm, v0ureg pc)
{
    struct v0ins      *ins = v0adrtoptr(vm, pc);
    uint8_t           port = ins->val;
    v0reg             parm = ins->parm;
    struct v0iofuncs *funcs = vm->iovec;
    v0reg             val;

    if v0insreg(ins, 0) {
        pc += sizeof(struct v0ins);
        val = vm->regs[v0insreg(ins, 1)];
    } else {
        pc += sizeof(struct v0ins) + sizeof(union v0insarg);
        val = v0getarg1(vm, ins, v0reg);
    }
    if (funcs[port].wrfunc) {
        funcs[port].wrfunc(vm, port, val);
    }

    return pc;
}

static _V0INSFUNC_T
v0hlt(struct v0 *vm, v0ureg pc)
{

    return V0_HALTED;
}

static _V0INSFUNC_T
v0rst(struct v0 *vm, v0ureg pc)
{

    return _startadr;
}

// disable _all_ interrupts
static _V0INSFUNC_T
v0cli(struct v0 *vm, v0ureg pc)
{
    long          ptr = v0adrtoptr(vm, pc)
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0reg         imr = vm->regs[V0_IMR_REG];
    v0reg        *dptr = v0getadr(vm, ins, 1, ptr);

    pc += sizeof(struct v0ins);
    if (v0getadrmode(ins) == V0_DIR_ADR || v0getadrmode(ins) == V0_NDX_ADR) {
        *dptr = imr;
    }
    vm->regs[V0_IMR_REG] = 0;
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

// add bits to IMR in order to enable interrupts
// instional memory-argument is mask to be restored
static _V0INSFUNC_T
v0sti(struct v0 *vm, v0ureg pc)
{
    long          ptr = v0adrtoptr(vm, pc)
    struct v0ins *ins = v0adrtoptr(vm, pc);
    v0ureg        imr = vm->regs[V0_IMR_REG];
    v0ureg        mask = vm->regs[v0insreg(ins, 0)];
    v0reg        *sptr = v0getadr(vm, ins, 1, ptr);

    pc += sizeof(struct v0ins);
    if (v0getadrmode(ins) == V0_DIR_ADR || v0getadrmode(ins) == V0_NDX_ADR) {
        imr = *sptr;
    } else {
        imr |= mask;
    }
    vm->regs[V0_PC_REG] = pc;
    vm->regs[V0_IMR_REG] = imr;

    return pc;
}

static _V0INSFUNC_T
v0slp(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

#if defined(V0_VIRTMEM)

static _V0INSFUNC_T
v0fpg(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0fls(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

#else /* !defined(V0_VIRTMEM) */

static _V0INSFUNC_T
v0fpg(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

static _V0INSFUNC_T
v0fls(struct v0 *vm, v0ureg pc)
{
    struct v0ins *ins = v0adrtoptr(vm, pc);

    pc += sizeof(struct v0ins);
    vm->regs[V0_PC_REG] = pc;

    return pc;
}

#endif /* defined(V0_VIRTMEM) */

#define v0setins(ins, str, narg, tab)                                   \
    (v0insnametab[(ins)] = #str,                                        \
     vasaddins(#str, ins, narg),                                        \
     (tab)[(ins)] = _v0insadr(str))
#define v0initins(tab)                                                  \
    do {                                                                \
        v0setins(V0_NOP, nop, 0, tab);                                  \
        v0setins(V0_NOT, not, 1, tab);                                  \
        v0setins(V0_AND, and, 2, tab);                                  \
        v0setins(V0_IOR, ior, 2, tab);                                  \
        v0setins(V0_XOR, xor, 2, tab);                                  \
        v0setins(V0_SHL, shl, 2, tab);                                  \
        v0setins(V0_SHR, shr, 2, tab);                                  \
        v0setins(V0_SAR, sar, 2, tab);                                  \
        v0setins(V0_INC, inc, 1, tab);                                  \
        v0setins(V0_DEC, dec, 1, tab);                                  \
        v0setins(V0_ADD, add, 2, tab);                                  \
        v0setins(V0_ADC, adc, 2, tab);                                  \
        v0setins(V0_SUB, sub, 2, tab);                                  \
        v0setins(V0_SBC, sbc, 2, tab);                                  \
        v0setins(V0_CMP, cmp, 2, tab);                                  \
        v0setins(V0_CRP, crp, 1, tab);                                  \
        v0setins(V0_MUL, mul, 2, tab);                                  \
        v0setins(V0_MUL, muh, 2, tab);                                  \
        v0setins(V0_DIV, div, 2, tab);                                  \
        v0setins(V0_SEX, sex, 2, tab);                                  \
        v0setins(V0_LEA, lea, 2, tab);                                  \
        v0setins(V0_CLZ, clz, 2, tab);                                  \
        v0setins(V0_HAM, ham, 2, tab);                                  \
        v0setins(V0_BSW, bsw, 2, tab);                                  \
        v0setins(V0_BTS, bts, 2, tab);                                  \
        v0setins(V0_BTC, btc, 2, tab);                                  \
        v0setins(V0_LDR, ldr, 2, tab);                                  \
        v0setins(V0_STR, str, 2, tab);                                  \
        v0setins(V0_RSR, rsr, 2, tab);                                  \
        v0setins(V0_WSR, wsr, 2, tab);                                  \
        v0setins(V0_PSH, psh, 1, tab);                                  \
        v0setins(V0_PSM, psm, 1, tab);                                  \
        v0setins(V0_PINS, pins, 1, tab);                                \
        v0setins(V0_POM, pom, 1, tab);                                  \
        v0setins(V0_JMP, jmp, 1, tab);                                  \
        v0setins(V0_BEQ, beq, 1, tab);                                  \
        v0setins(V0_BNE, bne, 1, tab);                                  \
        v0setins(V0_BLT, blt, 1, tab);                                  \
        v0setins(V0_BUL, bul, 1, tab);                                  \
        v0setins(V0_BGT, bgt, 1, tab);                                  \
        v0setins(V0_BUG, bug, 1, tab);                                  \
        v0setins(V0_CSR, csr, 1, tab);                                  \
        v0setins(V0_BEG, beg, 2, tab);                                  \
        v0setins(V0_FIN, fin, 2, tab);                                  \
        v0setins(V0_RET, ret, 1, tab);                                  \
        v0setins(V0_SYS, sys, 2, tab);                                  \
        v0setins(V0_SRT, srt, 2, tab);                                  \
        v0setins(V0_THR, thr, 1, tab);                                  \
        v0setins(V0_THX, thx, 2, tab);                                  \
        v0setins(V0_ICD, icd, 2, tab);                                  \
        v0setins(V0_IMM, imm, 2, tab);                                  \
        v0setins(V0_IRD, ird, 2, tab);                                  \
        v0setins(V0_IWR, iwr, 2, tab);                                  \
        v0setins(V0_HLT, hlt, 0, tab);                                  \
        v0setins(V0_RST, rst, 0, tab);                                  \
        v0setins(V0_CLI, cli, 1, tab);                                  \
        v0setins(V0_STI, sti, 1, tab);                                  \
        v0setins(V0_SLP, slp, 0, tab);                                  \
        v0setins(V0_WFE, wfe, 1, tab);                                  \
        v0setins(V0_SEV, sev, 1, tab);                                  \
        v0setins(V0_FPG, cli, 1, tab);                                  \
        v0setins(V0_FLS, sti, 1, tab);                                  \
    } while (0)

void
v0printop(struct v0ins *ins)
{
    int val = ins->code;

    fprintf(stderr, "code\t%x - reg1 == %x, reg2 == %x\n",
            val, v0insreg(ins, 0), v0insreg(ins, 1));

    return;
}

void
v0initseg(struct v0 *vm, v0memadr base, size_t npage, v0pagedesc flg)
{
    v0pagedesc *pgtab = vm->membits;
    size_t      ndx = base / V0_PAGE_SIZE;

    /* text, rodata, data, bss, stk */
    for (ndx = 0 ; ndx < npage ; ndx++) {
        pgtab[ndx] = flg;
    }

    return;
}

void
v0initio(struct v0 *vm)
{
    struct v0iofuncs *vec = vm->iovec;
    char             *vtd = vm->vtdpath;
    FILE             *fp;

    if (!vtd) {
        vtd = strdup(V0_VTD_PATH);
        if (!vtd) {
            fprintf(stderr, "V0: failed to duplicate VTD-path\n");

            exit(1);
        }
    }
    fp = fopen(vtd, "a+");
    if (!fp) {
        perror("V0: failed to open VTD-file");

        exit(errno);
    }
    vec[V0_STDIN_PORT].rdfunc = v0readkbd;
    vec[V0_STDOUT_PORT].wrfunc = v0writetty;
    vec[V0_STDERR_PORT].wrfunc = v0writeerr;
    vec[V0_RTC_PORT].rdfunc = v0readrtc;
    vec[V0_TMR_PORT].rdfunc = v0readtmr;

    return;
}

#define V0_TRAP_PERMS  (V0_MEM_PRESENT | V0_MEM_EXEC                    \
                        | V0_MEM_MAP | V0_MEM_TRAP | V0_MEM_SYS)
#define V0_CODE_PERMS  (V0_MEM_PRESENT | V0_MEM_READ | V0_MEM_EXEC)
#define V0_DATA_PERMS  (V0_MEM_READ | V0_MEM_WRITE | V0_MEM_MAP)
#define V0_KERN_PERMS  (V0_MEM_PRESENT | V0_MEM_EXEC | V0_MEM_SYS | V0_MEM_MAP)
#define V0_TLS_PERMS   (V0_DATA_PERMS | V0_MEM_TLS)
#define V0_STACK_PERMS (V0_MEM_PRESENT | V0_MEM_READ | V0_MEM_WRITE     \
                        | V0_MEM_STACK)

struct v0 *
v0init(struct v0 *vm)
{
    void   *mem = calloc(1, V0_RAM_SIZE);
    void   *ptr;
    long    newvm = 0;
    size_t  vmnpg = V0_RAM_SIZE / V0_PAGE_SIZE;

    if (!mem) {

        return NULL;
    }
    ptr = calloc(V0_MAX_IOPORTS, sizeof(struct v0iofuncs));
    if (ptr) {
        if (!vm) {
            vm = calloc(1, sizeof(struct v0));
            if (!vm) {
                free(mem);
                free(ptr);

                return NULL;
            }
            newvm = 1;
        }
        memset(vm, 0, sizeof(struct v0));
        vm->iovec = ptr;
#if 0
        ptr = calloc(65536, sizeof(struct divuf16));
        if (!ptr) {
            free(mem);
            free(vm->iovec);
            if (newvm) {
                free(vm);
            }

            return NULL;
        }
        fastuf16divuf16gentab(ptr, 0xffff);
        vm->divu16tab = ptr;
#endif
        ptr = calloc(vmnpg, sizeof(v0pagedesc));
        if (!ptr) {
            free(mem);
            free(vm->iovec);
            //      free(vm->divu16tab);
            if (newvm) {
                free(vm);
            }
        }
        vm->mem = mem;
        vm->membits = ptr;
        v0initseg(vm, 0, 1, V0_TRAP_PERMS);
        v0initseg(vm, V0_PAGE_SIZE, vmnpg / 8 - 1, V0_CODE_PERMS);
        v0initseg(vm, vmnpg * V0_PAGE_SIZE / 4, vmnpg / 2, V0_DATA_PERMS);
        v0initseg(vm, vmnpg * V0_PAGE_SIZE * 3 / 4, vmnpg / 4 - 8, V0_KERN_PERMS);
        v0initseg(vm, vmnpg * (V0_PAGE_SIZE - 16), 8, V0_TLS_PERMS);
        v0initseg(vm, vmnpg * (V0_PAGE_SIZE - 8), 8, V0_STACK_PERMS);
        v0initio(vm);
        vm->regs[V0_FP_REG] = 0x00000000;
        vm->regs[V0_SP_REG] = V0_RAM_SIZE - 1;
        vm->memsize = V0_RAM_SIZE;
    }
    v0vm = vm;

    return vm;
}

int
v0loop(struct v0 *vm, v0ureg pc)
{
    //    static _V0INSTAB_T  jmptab[V0_MAX_INSTS];
    struct v0ins *ins = v0adrtoptr(vm, pc);

#if defined(__GNUC__) && 0
    do {
        v0reg code = ins->code;

        {
            /* ALU */
            v0nopins:
            v0notins:
            v0andins:
            v0iorins:
            v0xorins:
            v0shlins:
            v0shrins:
            v0sarins:
            v0incins:
            v0decins:
            v0addins:
            v0aduins:
            v0adcins:
            v0subins:
            v0sbuins:
            v0sbcins:
            v0cmpins:
            v0crpins:
            v0mulins:
            v0mluins:
            v0muhins:
            v0mhuins:
            v0divins:
            v0dvuins:
            /* bit operations */
            v0sexins:
            v0zexins:
            v0leains:
            v0clzins:
            v0hamins:
            v0bswins:
            v0btsins:
            v0btcins:
            v0bclins:
            /* load-store unit */
            v0ldrins:
            v0strins:
            v0barins:
            v0brdins:
            v0bwrins:
            v0pshins:
            v0psmins:
            v0popins:
            v0pomins:
            v0cpfins:
            v0fpgins:
            v0flsins:
            v0ldlins:
            v0stcins:
            /* branches and control flow */
            v0jmpins:
            v0beqins:
            v0bneins:
            v0bltins:
            v0bulins:
            v0bgtins:
            v0bugins:
            v0bafins:
            v0csrins:
            v0begins:
            v0finins:
            v0retins:
            v0sysins:
            v0srtins:
            v0thrins:
            v0thxins:
            /* interrupts and system control */
            v0hltins:
            v0rstins:
            v0cliins:
            v0stiins:
            v0intins:
            v0slpins:
            v0wfeins:
            v0sevins:
            /* I/= operations */
            v0icdins:
            v0ircins:
            v0iwcins:
            v0ilmins:
            v0iocins:

            goto *(v0jmptab[code]);
        }
    } while (1);

#else /* !defined(__GNUC__) */

    v0initins(v0jmptab);
    while (v0insisvalid(vm, ins)) {
        struct v0ins *ins = v0adrtoptr(vm, pc);
        uint8_t       code = ins->code;
        _V0INSFUNC_T *func = v0jmptab[code];

        pc = func(vm, pc);
    }

#endif

    return EXIT_SUCCESS;
}

void
v0getopt(struct v0 *vm, int argc, char *argv[])
{
    return;
}

int
main(int argc, char *argv[])
{
    struct v0 *vm = v0init(NULL);
    vasuword   adr = V0_TEXT_ADR;
    int        ret = EXIT_FAILURE;
    long       ndx;

    if (vm) {
        v0getopt(vm, argc, argv);
        v0initops(v0jmptab);
        vasinit();
        for (ndx = 1 ; ndx < argc ; ndx++) {
#if (VASBUF) && !(VASMMAP)
            vasreadfile(argv[ndx], adr, ++vasreadbufcur);
#else
            vasreadfile(argv[ndx], adr);
#endif
            adr = vastranslate(adr);
            vasresolve();
            vasfreesyms();
            if (!vm->regs[V0_PC_REG]) {
                vm->regs[V0_PC_REG] = _startadr;
            }
            if (_startset) {
                ret = v0loop(vm, _startadr);
            }
        }

        exit(ret);
    }

    exit(0);
}

