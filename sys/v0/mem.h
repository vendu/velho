#ifndef __V0_MEM_H__
#define __V0_MEM_H__

#include <stddef.h>
#include <stdint.h>

#define V0_PAGE_TAB_SHIFT       V0_PAGE_SHIFT
#define V0_PAGE_TAB_BITS        10
#define V0_PAGE_TAB_ITEMS       ((1 << V0_PAGE_TAB_BITS) - 1)
#define V0_PAGE_DIR_SHIFT       (V0_PAGE_TAB_SHIFT + V0_PAGE_SHIFT)
#define V0_PAGE_DIR_BITS        (32 - V0_PAGE_TAB_BITS - V0_PAGE_SHIFT)
#define V0_PAGE_DIR_ITEMS       ((1 << V0_PAGE_DIR_BITS) - 1)

#define V0_PAGE_TLB_ITEMS       (V0_PAGE_SIZE / sizeof(v0pagedesc))

/* page-table entry flag-bits */
/* STM: !CORE && WIRE */
#define V0_MEM_LOCK_BIT         (1 << 0)
#define V0_MEM_CORE_BIT         (1 << 1)
#define V0_MEM_WIRE_BIT         (1 << 2)
#define V0_MEM_DIRTY_BIT        (1 << 3)
#define V0_MEM_SYS_BIT          (1 << 4)
#define V0_MEM_GLOBAL_BIT       (1 << 5)
#define V0_MEM_EXEC_BIT         (1 << 6)
#define V0_MEM_WRITE_BIT        (1 << 7)
#define V0_MEM_READ_BIT         (1 << 8)
#define V0_MEM_GROW_DOWN_BIT    (1 << 9)
#define V0_MEM_FLAG1_BIT        (1 << 10)
#define V0_MEM_FLAG2_BIT        (1 << 11)
#define V0_PAGE_BITS   		(UINT32_C(V0_PAGE_SIZE - 1))
#define V0_PAGE_FRAME  		(~UINT32_C(V0_PAGE_SIZE - 1))

static __inline__ v0word
v0bufload(struct vm *vm, v0adr adr)
{
    v0word      page = adr & V0_PAGE_FRAME;
    v0uword     ndx = tmhash32(adr);
    v0pagedesc *tlb = g_v0tlb;
    v0word      word;
    v0uword     desc;
    v0uword     base;
    v0uword     val;

    ndx &= V0_PAGE_TLB_ITEMS - 1;
    val = adr >> V0_PAGE_DIR_SHIFT;
    desc = g_v0pagedir[val];
    word = v0getword(vm, adr);
    val = adr & V0_PAGE_FRAME_MASK;
    base = desc & V0_PAGE_FRAME_MASK;
    if (base != val) {
        ndx = tmhash32(ndx);
        ndx &= V0_PAGE_TLB_ITEMS - 1;
        tlb[ndx] = desc;
    }

    return word;
}

static __inline__ w0word
v0bufstore(struct vm *vm, w0adr adr, v0uword word)
{
    v0word      page = adr & V0_PAGE_FRAME;
    v0uword     ndx = tmhash32(adr);
    v0pagedesc *tlb = g_v0tlb;
    v0word      word;
    v0uword     desc;
    v0uword     base;
    v0uword     val;

    ndx &= V0_PAGE_TLB_ITEMS - 1;
    val = adr >> V0_PAGE_DIR_SHIFT;
    desc = g_v0pagedir[val];
    word = v0getword(vm, adr);
    val = adr & V0_PAGE_FRAME_MASK;
    base = desc & V0_PAGE_FRAME_MASK;
    if (base != val) {
        ndx = tmhash32(ndx);
        ndx &= V0_PAGE_TLB_ITEMS - 1;
        tlb[ndx] = desc;
    }

    return word;
}

#endif /* __V0_MEM_H__ */

