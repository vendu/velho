#ifndef __V0_MEM_H__
#define __V0_MEM_H__

#include <stddef.h>

/* page-table entry flag-bits */
#define V0_PAGE_EXEC   (1 << 0)
#define V0_PAGE_WRITE  (1 << 1)
#define V0_PAGE_READ   (1 << 2)
#define V0_PAGE_CORE   (1 << 3)
#define V0_PAGE_DIRTY  (1 << 4)
#define V0_PAGE_WIRE   (1 << 5)
#define V0_PAGE_BUF    (1 << 6)
#define V0_PAGE_BURST  (1 << 7)
#define V0_PAGE_DMA    (1 << 8)
#define V0_PAGE_SYS    (1 << 9)
#define V0_PAGE_GLOBAL (1 << 10)
#define V0_PAGE_DIRECT (1 << 11)

static __inline__ v0word
v0bufload(struct vm *vm, v0adr adr)
{
    v0word              ndx = tmhash32(adr);
    v0word              word;
    v0word              badr;
    struct v0membuf    *buf;

    ndx &= vm->memsize - 1;
    buf = &vm->buf[ndx];
    badr = buf->adr;
    if (badr == adr) {
        word = buf->val;
    } else {
        ndx = tmhash32(ndx);
        ndx &= vm->memsize - 1;
        buf = &vm->buf[ndx];
        badr = buf->adr;
        if (badr == adr) {
            word = buf->val;
        } else {
            word = *(v0reg *)(&g_v0vm.mem[adr]);
        }
    }

    return word;
}

static __inline__ w0word
v0bufstore(struct vm *vm, w0adr adr, v0word word)
{
    v0word              ndx = tmhash32(adr);
    v0word              val;
    v0word              badr;
    struct v0membuf    *buf;

    ndx &= vm->memsize - 1;
    buf = &vm->buf[ndx];
    badr = buf->adr;
    val = buf->val;
    if ((badr) && badr != adr) {
        val = buf->val;
        ndx = tmhash32(ndx);
        buf->adr = adr;
        ndx &= vm->memsize - 1;
        buf->val = word;
        buf = &vm->buf[ndx];
        buf->adr = adr;
        buf->val = val;
    }

    return word;
}

#endif /* __V0_MEM_H__ */

