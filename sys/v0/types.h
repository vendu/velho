#ifndef __V0_TYPES_H__
#define __V0_TYPES_H__

#include <stdint.h>
#include <v0/conf.h>
#include <v0/gpu.h>

typedef int32_t         v0reg;          // signed register value
typedef uint32_t        v0ureg;         // unsigned register value
typedef int64_t         v0wreg;    	// signed wide-register value
typedef uint64_t        v0uwreg;   	// unsigned wide-register value
typedef v0ureg          v0adr;     	// machine memory address

typedef uint32_t        v0pagedesc;     // [virtual] memory page descriptor
typedef uint32_t        v0iodesc;       // I/O-device page address + flags
typedef uint32_t        v0ioperm;       // I/O-permission bits

struct v0sysctx {
    v0reg       msw;
    v0reg       sp;
};

struct v0tcb {
    v0wreg      regs[V0_STD_REGS];
    v0reg       fp0;                    // ring #0 (system mode) frame-pointer
    v0reg       sp0;                    // ring #0 stack-pointer
};

/* callee-save structure */
struct v0calleectx {
    /* local variables are allocated here */
    /* stored registers r4..r11 go here */                       <- SP
    v0reg       fp;             // caller frame pointer         <- FP after BEG
    v0reg       lr;             // caller return address        <- FP after CSR
    v0reg       args[VLA];      // stack parameters to callee   <- SP after CSR
};

/* caller-save structure */
struct v0callerctx {
    /* r0..r3 may be used as subroutine arguments */
    v0reg       r0;
    v0reg       r1;
    v0reg       r2;
    v0reg       r3;
};

/*
 * CALL STACK
 * ----------
 *

/*
 * NOTES
 * ------
 * - return address is stored in the link register %lr (%r15)
 * - number of stack arguments is stored in %ac
 */
struct v0calleectx {
    v0reg             fp;               // copy of FP (frame pointer)
    struct v0callregs regs;             // caller-save register
    v0reg             args[VLA];        // args followed by struct v0calleeregs
};

#endif /* __V0_TYPES_H__ */

