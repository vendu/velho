#define V0_SEX 0x18
#define V0_ZEX 0x19
#define V0_CLZ 0x1a
#define V0_HAM 0x1b
#define V0_BSW 0x1c

long
v0sex(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);

    v0setreg(vm, op, pc);

    return vm->pc;
}

long
v0zex(struct vm *vm, void *op)
{
    v0ureg src = v0getsrc(vm, op);

    v0setreg(vm, op, pc);

    return vm->pc;
}

long
v0clz(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);
    v0reg reg;

    v0clzop(src, reg);
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0ham(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);
    v0reg reg;

    v0hamop(src, reg);
    v0setreg(vm, op, reg);

    return vm->pc;
}

long
v0bsw(struct vm *vm, void *op)
{
    v0reg src = v0getsrc(vm, op);
    v0reg parm = v0insparm(vm, op);
    v0reg reg;

    if (parm == 2) {
        reg = ((src >> 8) & 0xff) | ((src & 0xff) << 8);
    } else {
        reg = ((src >> 24) & 0x000000ff) | ((src >> 8) & 0x0000ff00)
              | ((src & 0x0000ff00) << 8) | ((src & 0xff) << 24);
    }
    v0setreg(vm, op, reg);

    return vm->pc;
}

