#ifndef __V0_STK_H__
#define __V0_STK_H__

#include <v0/v0.h>

struct v0callframe {
    v0wide      retlr;
    v0wide      retfp;
};

#define v0precall(adr, nb)
    do {                                                                \
        v0loadreg(V0_TR_REG, (nb));                                     \
        v0pshreg(V0_TR_REG);                                            \
        v0add(V0_SP_REG, V0_TR_REG);                                    \
        v0pshreg(V0_LR_REG);                                            \
        v0pshreg(V0_FP_REG);                                            \
        v0loadreg(V0_TR_REG, (adr));                                    \
        v0cpyreg(V0_SP_REG, V0_FP_REG);                                 \
        v0cpytreg(V0_PC_REG, V0_LR_REG);                                \
        v0cpyreg(V0_TR_REG, V0_PC_REG);                                 \
    } while (0)
#define v0postcall(nb)                                                  \
    do {                                                                \
        v0loadreg(V0_TR_REG, (nb));                                     \
        v0pop(V0_FP_REG);                                               \
        v0sub(V0_TR_REG, V0_TR_REG);                                    \
        v0cpyreg(V0_LR_REG, V0_PC_REG);                                 \
    } while (0)
#define v0call(fp, sp)

static __inline__ void
v0psh(v0word val)
{
}

#endif /* __V0_STK_H__ */

