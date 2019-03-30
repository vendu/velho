#ifndef __V0_TYPES_H__
#define __V0_TYPES_H__

#include <stdint.h>
#include <v0/conf.h>
#include <v0/gpu.h>

typedef int32_t         v0word;          // signed register value
typedef uint32_t        v0ureg;         // unsigned register value
typedef int64_t         v0intreg;    	// signed wide-register value
typedef uint64_t        v0uwreg;   	// unsigned wide-register value
typedef v0ureg          v0adr;     	// machine memory address

typedef uint32_t        v0pagedesc;     // [virtual] memory page descriptor
typedef uint32_t        v0iodesc;       // I/O-device page address + flags
typedef uint32_t        v0ioperm;       // I/O-permission bits

struct v0sysctx {
    v0word      msw;
    v0word      sp;
};

struct v0tcb {
    v0word      msw0;                   // ring #0 machine status word
    v0word      sp0;                    // ring #0 stack-pointer
    v0intreg    intregs[V0_INT_REGS];   // wide registers
    v0word      sysregs[V0_SYS_REGS];   // system registers
};

/* callee-save structure */
struct v0calleectx {
    /* v0word varsize; (local variables in bytes) goes here */
    /* local variables go here */
    /* stored registers r4..r11 go here */
    v0word      fp;             // caller frame pointer         <- FP after BEG
    v0word      lr;             // caller return address        <- SP after CSR
    v0word      args[VLA];      // stack parameters to callee   <- SP after CSR
};

/* caller-save structure */
struct v0callerctx {
    /* r0..r3 may be used as subroutine arguments */
    v0word      r0;
    v0word      r1;
    v0word      r2;
    v0word      r3;
};

#endif /* __V0_TYPES_H__ */

