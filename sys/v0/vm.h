#ifndef __V0_VM_H__
#define __V0_VM_H__

#include <stddef.h>
#include <stdint.h>
#include <mach/atomic.h>
#include <v0/v0.h>

extern v0pagedesc       g_v0pagedir[V0_PAGE_DIR_ITEMS];
extern v0pagedesc       g_v0tlb[V0_PAGE_TLB_ENTRIES];

typedef char           *v0byteptr;      // 8-bit pointer
typedef uint8_t        *v0ubyteptr;   	// 8-bit unsigned pointer
typedef int16_t        *v0halfptr;      // 16-bit pointer
typedef uint16_t       *v0uhalfptr;   	// 8-bit unsigned pointer
typedef void           *v0ptr;     	// generic pointer
typedef intptr_t        v0memofs;  	// signed pointer value
typedef uintptr_t       v0memadr;       // unsigned pointer value

#define v0getpc(vm)     v0getreg(V0_PC_REG)
#define v0getfp(vm)     v0getreg(V0_FP_REG)
#define v0getsp(vm)     v0getreg(V0_SP_REG)
#define v0getmsw(vm)    v0getsysreg(V0_MSW_REG)
struct v0sys {
    struct v0thr                thrbuf[V0_MAX_THREADS];
    v0pagedesc                  tlb[V0_PAGE_TLB_ENTRIES];
    v0wide                      genregs[V0_GENERAL_REGISTERS];
    v0word                      sysregs[V0_SYSTEM_REGISTERS];
    v0word                      intrmask;
    uint8_t                    *mem;
    size_t                      memsize;
    volatile m_atomic_t         buflkmap;
    v0pagedesc                 *buftab;
    size_t                      bufsize;
    v0uword                     ustk;
    size_t                      ustksize;
    v0uword                     sstk;
    size_t                      sstksize;
};

static __inline__ v0word
v0getadr1(struct v0sys *vm, struct v0inst *inst)
{
    v0adr       adr = 0;
    v0word      val = 0;
    v0word      reg = (inst->val & V0_REG_ADR) ? v0getregid1(inst) : -1;
    v0word      ndx = (inst->val & V0_NDX_ADR) ? v0getimm32(inst) : 0;

    if (reg >= 0) {
        val = *(v0word *)&vm->genregs[reg];
    }
    adr += ndx;
    adr += val;

    return adr;
}

static __inline__ v0word
v0getadr2(struct v0sys *vm, struct v0inst *inst)
{
    v0adr       adr = 0;
    v0word      val = 0;
    v0word      reg = (inst->val & V0_REG_ADR) ? v0getregid2(inst) : -1;
    v0word      ndx = (inst->val & V0_NDX_ADR) ? v0getimm32(inst) : 0;

    if (reg >= 0) {
        val = *(v0word *)&vm->genregs[reg];
    }
    adr += ndx;
    adr += val;

    return adr;
}

static __inline__ v0word
v0bufload(struct v0sys *vm, v0adr adr)
{
    v0word      page = adr & V0_PAGE_FRAME_MASK;
    v0uword     ndx = tmhash32(adr);
    v0pagedesc *tlb = g_v0tlb;
    v0word      word;
    v0uword     desc;
    v0uword     base;
    v0uword     val;

    ndx &= V0_PAGE_TLB_ENTRIES - 1;
    val = adr >> V0_PAGE_DIR_SHIFT;
    desc = g_v0pagedir[val];
    word = v0getword(vm, adr);
    val = adr & V0_PAGE_FRAME_MASK;
    base = desc & V0_PAGE_FRAME_MASK;
    if (base != val) {
        ndx = tmhash32(ndx);
        ndx &= V0_PAGE_TLB_ENTRIES - 1;
        tlb[ndx] = desc;
    }

    return word;
}

static __inline__ v0w0rd
v0bufstore(struct v0sys *vm, v0adr adr, v0uword word)
{
    v0word      page = adr & V0_PAGE_FRAME_MASK;
    v0uword     ndx = tmhash32(adr);
    v0pagedesc *tlb = g_v0tlb;
    v0uword     desc;
    v0uword     base;
    v0uword     val;

    ndx &= V0_PAGE_TLB_ENTRIES - 1;
    val = adr >> V0_PAGE_DIR_SHIFT;
    desc = g_v0pagedir[val];
    v0putword(vm, adr, rword);
    val = adr & V0_PAGE_FRAME_MASK;
    base = desc & V0_PAGE_FRAME_MASK;
    if (base != val) {
        ndx = tmhash32(ndx);
        ndx &= V0_PAGE_TLB_ENTRIES - 1;
        tlb[ndx] = desc;
    }

    return word;
}

// virtual machine operation
typedef long v0vmopfunc_t(struct v0sys *sys, void *op);

#endif /* #ifndef __V0_VM_H__ */

