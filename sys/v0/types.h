#ifndef __V0_TYPES_H__
#define __V0_TYPES_H__

#include <stdint.h>
#include <v0/conf.h>
#include <v0/gpu.h>

typedef int32_t         v0word;         // signed register value
typedef uint32_t        v0uword;        // unsigned register value
typedef int64_t         v0wide;    	// signed wide-register value
typedef uint64_t        v0uwide;   	// unsigned wide-register value

typedef uint32_t        v0pagedesc;     // page address + flags
typedef uint32_t        v0trapdesc;     // trap function address + flags

typedef uint32_t        v0iodesc;       // I/O-device page address + flags
typedef uint32_t        v0ioperm;       // I/O-permission bits

struct v0tcb {
    v0word      msw0;                   // ring #0 machine status word
    v0word      sp0;                    // ring #0 stack-pointer
    v0intreg    genregs[V0_INT_REGS];   // wide registers
    v0word      sysregs[V0_SYS_REGS];   // system registers
};

/* caller-save structure */
struct v0callerctx {
    /* r0..r3 may be used as subroutine arguments */
    v0word      r0;
    v0word      r1;
    v0word      r2;
    v0word      r3;
};

struct v0thr {
    v0wide              genregs[V0_INT_REGS];
    v0word              sysregs[V0_SYS_REGS];
    m_atomic_t          mtx;
};

#endif /* __V0_TYPES_H__ */

