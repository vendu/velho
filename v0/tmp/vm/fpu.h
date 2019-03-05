#ifndef __V0_VM_FPU_H__
#define __V0_VM_FPU_H__

/* adder instructions */
#define V0_FADD 0x01
#define V0_FSUB 0x02
/* multiplication and division instructions */
#define V0_FCRP 0x03
#define V0_FMUL 0x04
#define V0_FDIV 0x05
/* exponential and logarithm instructions */
#define V0_FPOW 0x06
#define V0_FEXP 0x07
#define V0_FLOG 0x08
/* root instructions */
#define V0_FSQR 0x07
#define V0_FCRT 0x08
/* min and max */
#define V0_FMIN 0x09
#define V0_FMAX 0x0a

#endif /* __V0_VM_FPU_H__ */

