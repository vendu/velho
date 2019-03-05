#include <v0/vm/vm.h>
#include <v0/vm/types.h>

/*
 * NOT   0x01    bitwise inverse
 * AND   0x02    logical AND
 * IOR   0x03    logical inclusive OR
 * XOR   0x04    logical exclusive OR
 *
 * SHL   0x05    shift left logical
 * SHR   0x06    shift right logical (fill with zero)
 * SAR   0x07    shift right arithmetic (fill with sign-bit)
 *
 * INC	 0x08    increment by one
 * DEC   0x09    decrement by one
 * ADD   0x0a    addition (ignore over- and underflows)
 * ADU   0x0b    unsigned addition
 * ADC   0x0c    addition with carry-flag
 * SUB   0x0d    subtraction; ignore underflow
 * SBU   0x0e    unsigned subtraction
 * SBC   0x0f    subtract with carry-flag
 * CMP   0x10    compare (subtract + set MSW-flags)
 *
 * CRP   0x11    calculate reciprocal
 * MUL   0x12    multiplication, returns low word
 * MLU   0x13    unsigned multiplication, returns low word
 * MLH   0x14    multiplication, returns high word
 * MHU   0x15    unsigned multiplication, returns high word
 * DIV   0x16    division
 * DVU   0x17    unsigned division
 */

long
v0not(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);
    v0reg reg;

    reg = ~src;
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0and(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);
    v0reg dest = v0getdest(vm, op);
    v0reg reg;

    reg = src & dest;
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0ior(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);
    v0reg dest = v0getdest(vm, op);
    v0reg reg;

    reg = src | dest;
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0xor(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);
    v0reg dest = v0getdest(vm, op);
    v0reg reg;

    reg = src ^ dest;
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0shl(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);
    v0reg dest = v0getdest(vm, op);
    v0ureg reg;

    reg = dest << src;
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0shr(struct vm *vm, void *op)
{
    v0reg  src = v0getsrc(vm, op);
    v0ureg dest = v0getdest(vm, op);
    v0ureg  reg;

    reg = dest >> src;
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0sar(struct vm *vm, void *op)
{
    v0reg  src = v0getsrc(vm, op);
    v0ureg dest = v0getdest(vm, op);
    v0reg mask = dest & V0_SIGN_BIT;
    v0ureg reg;

    mask--;
    mask <<= 32 - src;
    reg = dest >> src;
    reg |= mask;
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0inc(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);
    v0reg reg;

    reg = ++src;
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0dec(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);
    v0reg reg;

    reg = --src;
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0add(struct vm *vm, void *op)
{
    v0reg dest = v0getdest(vm, op);
    v0reg src = v0getsrc(vm, op);

    dest += src;
    v0setreg(vm, op, dest);

    return vm->pc;
}

long
v0adu(struct vm *vm, void *op)
{
    v0ureg dest = v0getdest(vm, op);
    v0ureg src = v0getsrc(vm, op);

    dest += src;
    v0setreg(vm, op, dest);

    return vm->pc;
}

long
v0adc(struct vm *vm, void *op)
{
    v0reg dest = v0getdest(vm, op);
    v0reg src = v0getsrc(vm, op);

    dest += src;
    v0setreg(vm, op, dest);

    return vm->pc;
}

long
v0sub(struct vm *vm, void *op)
{
    v0reg dest = v0getdest(vm, op);
    v0reg src = v0getsrc(vm, op);

    dest -= src;
    v0setreg(vm, op, dest);

    return vm->pc;
}

long
v0sbu(struct vm *vm, void *op)
{
    v0ureg dest = v0getdest(vm, op);
    v0ureg src = v0getsrc(vm, op);

    dest -= src;
    v0setreg(vm, op, dest);

    return vm->pc;
}

long
v0sbc(struct vm *vm, void *op)
{
    v0ureg dest = v0getdest(vm, op);
    v0ureg src = v0getsrc(vm, op);
    v0reg  flg = 0;

    dest -= src;
    v0setreg(vm, op, dest);

    return vm->pc;
}

long
v0cmp(struct vm *vm, void *op)
{
    ;
}

long
v0sbu(struct vm *vm, void *op)
{
    ;
}

/*
 * CRP   0x11    calculate reciprocal
 * MUL   0x12    multiplication, returns low word
 * MLU   0x13    unsigned multiplication, returns low word
 * MLH   0x14    multiplication, returns high word
 * MHU   0x15    unsigned multiplication, returns high word
 * DIV   0x16    division
 * DVU   0x17    unsigned division
 */

long
v0crp(struct vm *vm, void *op)
{
    double src = v0getwsrc(vm, op);

    src = 1.0 / src;
    v0setwdest(vm, op, src);
}

long
v0mul(struct vm *vm, void *op)
{
    v0reg dest = v0getdest(vm, op);
    v0reg src = v0getsrc(vm, op);

    dest *= src;
    v0setdest(vm, op, dest);
}

long
v0mlu(struct vm *vm, void *op)
{
    ;
}

long
v0mhu(struct vm *vm, void *op)
{
    ;
}

long
v0div(struct vm *vm, void *op)
{
    ;
}

long
v0dvu(struct vm *vm, void *op)
{
    ;
}

