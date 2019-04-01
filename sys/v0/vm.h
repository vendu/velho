#ifndef __V0_VM_H__
#define __V0_VM_H__

#include <v0/types.h>
#include <stdint.h>
#include <v0/types.h>

typedef char           *v0byteptr;      // 8-bit pointer
typedef uint8_t        *v0ubyteptr;   	// 8-bit unsigned pointer
typedef int16_t        *v0halfptr;      // 16-bit pointer
typedef uint16_t       *v0uhalfptr;   	// 8-bit unsigned pointer
typedef void           *v0ptr;     	// generic pointer
typedef intptr_t        v0memofs;  	// signed pointer value
typedef uintptr_t       v0memadr;       // unsigned pointer value

 // virtual machine operation
typedef long v0vmopfunc_t(struct vm *vm, void *op);

#define v0getpc(vm)     v0getreg(V0_PC_REG)
#define v0getfp(vm)     v0getreg(V0_FP_REG)
#define v0getsp(vm)     v0getreg(V0_SP_REG)
#define v0getmsw(vm)    v0getsysreg(V0_MSW_REG)
struct vm {
    uint8_t            *mem;
    size_t              memsize;
    m_atomic_t          buflkmap;
    struct v0membuf    *buftab;
    size_t              bufsize;
};

#endif /* #ifndef __V0_VM_H__ */

