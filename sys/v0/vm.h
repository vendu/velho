#ifndef __V0_VM_H__
#define __V0_VM_H__

#include <stddef.h>
#include <stdint.h>
#include <mach/atomic.h>
#include <v0/v0.h>

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
    v0pagedesc                  tlb[V0_TLB_ENTRIES];
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

 // virtual machine operation
typedef long v0vmopfunc_t(struct v0sys *sys, void *op);

#endif /* #ifndef __V0_VM_H__ */

