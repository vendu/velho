/* zero assembler [virtual] machine interface */

#include <vas/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdint.h>
#include <vas/vas.h>
#include <v0/vm/conf.h>
#include <v0/vm/isa.h>
#include <v0/vm/vm.h>
#include <v0/vm/op.h>

extern struct v0 *v0vm;
struct vasop     *v0optab[V0_HASH_SIZE];
static long       vasopbits[V0_INSTS_MAX];
static long       vasinitflg;

#define vasopisnop(ins)    (*((uint16_t *)(ins)) == V0_NOP_CODE)
//#define vasophasval(code) (bitset(v0valbits, code))

#define vassetnop(adr)    (*((uint16_t *)(adr)) = V0_NOP_CODE)
#define vassetop(op, id, n, scl)                                        \
    do {                                                                \
        (op)->code = (id);                                              \
        (op)->narg = (n);                                               \
        (op)->sft = (scl);                                              \
    } while (0)

long
vasaddop(const char *str, uint8_t code, uint8_t narg)
{
    char         *cptr = (char *)str;
    long          key = 0;
    struct vasop *op;
    uint8_t       len;

    fprintf(stderr, "%x (%d)\t%s\n", code, narg, str);
    op = calloc(1, sizeof(struct vasop));
    if (!op) {

        return 0;
    }
    len = 0;
    while ((*cptr) && !isspace(*cptr) && len < V0_MNEMO_LEN - 2) {
        op->name[len] = *cptr;
        key += *cptr++;
        len++;
    }
    op->name[len] = '\0';
    key = tmhash32(key);
    key &= V0_HASH_SIZE - 1;
    vassetop(op, code, narg, V0_DEF_SHIFT);
    op->next = v0optab[key];
    v0optab[key] = op;

    return key;
}

static struct vasop *
vasfindop(char *str, vasword *retsft, char **retptr)
{
    char         *cptr = (char *)str;
    struct vasop *op = NULL;
    long          key = 0;
    vasword       sft = 0;
    size_t        len = 0;
    long          l;

    for (l = 0 ; l < 2 ; l++) {
        while ((*cptr) && !isspace(*cptr) && len < V0_MNEMO_LEN - 1) {
            key += *cptr++;
            len++;
        }
        if (key) {
            key = tmhash32(key);
            key &= V0_HASH_SIZE - 1;
            *cptr = '\0';
            op = v0optab[key];
            cptr++;
            while ((op) && strcmp(op->name, str)) {
                op = op->next;
            }
            if (op) {
                sft = op->sft;

                break;
            } else if (cptr > str + 1
                       && (cptr[-1] == 'b'
                           || cptr[-1] == 'w'
                           || cptr[-1] == 'l')) {
                key = 0;
                cptr[-1] = '\0';
                cptr = str;
            } else {

                break;
            }
        }
    }
    *retsft = sft;
    *retptr = cptr;

    return op;
}

struct vasop *
vasgetop(char *str, vasword *retsft, char **retptr)
{
    struct vasop *op = NULL;
    vasword       sft = 0;

#if (VASDEBUG)
    fprintf(stderr, "getop: %s\n", str);
#endif
    op = vasfindop(str, &sft, &str);
    if (op) {
        fprintf(stderr, "found!\n");
        *retsft = sft;
        *retptr = str;
    } else {
        fprintf(stderr, "NOT found!\n");
    }

    return op;
}

vasword
vasgetreg(char *str, vasword *retsft, char **retptr)
{
    vasword reg = -1;
    vasword sft = 0;

#if (VASDEBUG)
    fprintf(stderr, "getreg: %s\n", str);
#endif
    if (*str == 'r') {
        str++;
        sft = 2;
    } else if (*str == 'b') {
        str++;
        sft = 0;
    } else if (*str == 'w') {
        str++;
        sft = 1;
    } else if (*str == 'l') {
        str++;
        sft = 2;
    }
    if ((sft) && (*str) && isdigit(*str)) {
        reg = 0;
        while ((*str) && isdigit(*str)) {
            reg *= 10;
            reg += *str - '0';
            str++;
        }
        if ((*str) && (*str == ',' || isspace(*str))) {
            str[0] = '\0';
            str++;
            *retsft = sft;
            *retptr = str;
        } else if ((*str) && *str == ')') {
            *retsft = sft;
            *retptr = str;
        } else {
            fprintf(stderr, "invalid register name %s\n", str);

            exit(1);
        }
    } else {
        fprintf(stderr, "invalid register name %s\n", str);

        exit(1);
    }

    return reg;
}

#if 0
static void
vasinitops(void)
{
    if (!vasinitflg) {
        v0initopbits(vasopbits);
        vasinitflg = 1;
    }

    return;
}
#endif

#if 0
void
v0disasm(struct v0 *vm, struct v0ins *ins, v0ureg pc)
{
    fprintf(stderr, "%p:%p\t%s @ 0x%x\t", vm, op, v0opnametab[ins->code], pc);
    fprintf(stderr, "r1 = %x, r2 = %x, a = %x, p = %x, v = %x\n",
            op->reg1, op->reg2, op->adr, op->parm, op->val);

    return;
}
#endif

struct vastoken *
vasprocinst(struct vastoken *token, v0memadr adr,
            v0memadr *retadr)
{
    struct v0ins     *ins = (struct v0ins *)&v0vm->mem[adr];
    union  v0insarg  *arg;
    struct vastoken  *token1 = NULL;
    struct vastoken  *token2 = NULL;
    struct vastoken  *retval = NULL;
    struct vassymrec *sym;
    long              havearg = 0;
    vasword           val;
    vasword           flg;
    uint8_t           narg = token->data.inst.narg;
    uint8_t           sft = token->data.inst.sft;

#if 0
    if (!vasinitflg) {
        v0initops();
        vasinitflg = 1;
    }
#endif
#if (VASDB)
    vasaddline(adr, token->data.inst.data, token->file, token->line);
#endif
    ins->code = token->data.inst.code;
    if (vasopisnop(ins)) {
        retval = token->next;
        adr += sizeof(struct v0ins);
    } else if (!narg) {
        retval = token->next;
        ins->code = token->data.inst.code;
        adr += sizeof(struct v0ins);
        vasfreetoken(token);
    } else {
        token1 = token->next;
        vasfreetoken(token);
        if (token1) {
            switch(token1->type) {
                case VASTOKENIMMED:
                case VASTOKENVALUE:
                    val = token1->data.value.val;
                    if (val >= 0 && val <= V0_IMM_VAL_MAX) {
                        ins->arg[0].data.u16 = val & 0xffff;
                        adr += sizeof(struct v0ins);
                        ins++;
                    } else if (val >= V0_IMM_VAL_MIN
                               && val <= V0_IMM_VAL_MAX / 2) {
                        ins->arg[0].data.i16 = val & 0xffff;
                        adr += sizeof(struct v0ins);
                        ins++;
                    } else {
                        arg = memalign(ins, 4);
                        if (arg != (union v0insarg *)ins) {
                            ins->arg[0].data.u16 = 0xffff;
                            adr += sizeof(struct v0ins);;
                        }
                        arg->uval = val & 0xffffffff;
                        adr += sizeof(union v0insarg);
                        ins = (void *)&arg[1];
                    }

                    break;
                case VASTOKENREG:
                    v0setinsreg(ins, token1->data.reg, 0);

                    break;
                case VASTOKENSYM:
                case VASTOKENADR:
                    arg = memalign(ins, 4);
                    if (arg != (union v0insarg *)ins) {
                        ins->arg[0].data.u16 = 0xffff;
                        adr += sizeof(struct v0ins);
                    }
                    sym = malloc(sizeof(struct vassymrec));
                    sym->name = strdup((char *)token1->data.sym.name);
                    sym->adr = (uintptr_t)arg;
                    vasqueuesym(sym);
                    arg++;
                    adr += sizeof(union v0insarg);
                    ins = (void *)arg;

                    break;
                case VASTOKENINDEX:
                    token2 = token1->next;
                    if (token2->type == VASTOKENREG) {
                        val = token1->data.ndx.val;
                        v0setinsreg(ins, token2->data.ndx.reg, 0);
                        if (val >= 0 && val <= V0_IMM_VAL_MAX) {
                            ins->arg[0].data.u16 = (uint16_t)val;
                            adr += sizeof(struct v0ins);
                            ins++;
                        } else if (val >= V0_IMM_VAL_MIN
                                   && val <= V0_IMM_VAL_MAX / 2) {
                            ins->arg[0].data.i16 = (int16_t)val;
                            adr += sizeof(struct v0ins);
                            ins++;
                        } else {
                            arg = memalign(ins, 4);
                            if (arg != (union v0insarg *)ins) {
                                ins->arg[0].data.u16 = 0xffff;
                            }
                            arg->uval = val & 0xffffffff;
                            arg++;
                            adr += sizeof(union v0insarg);
                            ins = (void *)arg;
                        }
                    }
                    token1 = token2;

                    break;
                default:
                    fprintf(stderr,
                            "invalid argument 1 of type %lx\n", token1->type);
                    vasprinttoken(token1);

                    exit(1);

                    break;
            }
            if (!v0insreg(ins, 0)) {
                havearg = 1;
            }
            token2 = token1->next;
            vasfreetoken(token1);
            retval = token2;
            if (narg == 2) {
                if (token2->type == VASTOKENREG) {
                    v0setinsreg(ins, token2->data.reg, 1);
                } else if (havearg) {
                    fprintf(stderr,
                            "too many non-register arguments\n");

                    exit(1);
                } else {
                    switch(token2->type) {
                        case VASTOKENVALUE:
                        case VASTOKENIMMED:
                            val = token2->data.value.val;
                            if (val >= 0
                                && val <= V0_IMM_VAL_MAX) {
                                ins->arg[0].data.u16 = val & 0xffff;
                                adr += sizeof(struct v0ins);
                                ins++;
                            } else if (val >= V0_IMM_VAL_MIN
                                       && val <= V0_IMM_VAL_MAX / 2) {
                                ins->arg[0].data.i16 = val & 0xffff;
                                adr += sizeof(struct v0ins);
                                ins++;
                            } else {
                                arg = memalign(ins, 4);
                                if (arg != (union v0insarg *)ins) {
                                    ins->arg[0].data.u16 = 0xffff;
                                    adr += sizeof(struct v0ins);
                                }
                                arg->uval = val & 0xffffffff;
                                arg++;
                                adr += sizeof(union v0insarg);
                                ins = (void *)arg;
                            }

                            break;
                        case VASTOKENSYM:
                        case VASTOKENADR:
                            arg = memalign(ins, 4);
                            if (arg != (union v0insarg *)ins) {
                                ins->arg[0].data.u16 = 0xffff;
                            }
                            sym = malloc(sizeof(struct vassymrec));
                            sym->name = strdup((char *)token1->data.sym.name);
                            sym->adr = (uintptr_t)arg;
                            vasqueuesym(sym);
                            arg++;
                            adr += sizeof(union v0insarg);
                            ins = (void *)arg;

                            break;
                        case VASTOKENINDEX:
                            token1 = token2->next;
                            if (token1->type == VASTOKENREG) {
                                val = token2->data.ndx.val;
                                v0setinsreg(ins, token1->data.ndx.reg, 0);
                                if (val >= 0 && val <= V0_IMM_VAL_MAX) {
                                    ins->arg[0].data.u16 = (uint16_t)val;
                                    adr += sizeof(struct v0ins);
                                    ins++;
                                } else if (val >= V0_IMM_VAL_MIN
                                           && val <= V0_IMM_VAL_MAX / 2) {
                                    ins->arg[0].data.i16 = (int16_t)val;
                                    adr += sizeof(struct v0ins);
                                    ins++;
                                } else {
                                    arg = memalign(ins, 4);
                                    if (arg != (union v0insarg *)ins) {
                                        ins->arg[0].data.u16 = 0xffff;
                                        adr += sizeof(struct v0ins);
                                    }
                                    arg->uval = val & 0xffffffff;
                                    arg++;
                                    adr += sizeof(union v0insarg);
                                    ins = (void *)arg;
                                }
                            }

                            break;
                    }
                    token1 = token2;
                }
            }
            retval = token2->next;
            vasfreetoken(token2);
        }
    }
    *retadr = adr;

    return retval;
}

