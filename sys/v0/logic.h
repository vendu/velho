#ifndef __SYS_V0_LOGIC_H__
#define __SYS_V0_LOGIC_H__

#include <v0/v0.h>

static __inline__ v0word
v0not(v0word word)
{
    word = ~word;

    return word;
}

static __inline__ v0word
v0and(v0word word1, v0word word2)
{
    word2 &= word1;

    return word2;
}

static __inline__ v0word
v0ior(v0word word1, v0word word2)
{
    word2 |= word1;

    return word2;
}

static __inline__ v0word
v0xor(v0word word1, v0word word2)
{
    word2 ^= word1;

    return word2;
}

static __inline__ void
v0logic(struct vm *vm)
{
    v0word      pc = vm->regs[V0_PC_REG];
    v0ins      *ins = (struct v0ins *)&vm->mem[pc];
    long        reg1 = v0getreg1(
}

#endif /* __SYS_V0_LOGIC_H__ */

