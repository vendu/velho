#ifndef __V0_ISA_H__
#define __V0_ISA_H__

#include <v0/conf.h>
#include <stdint.h>
#include <v0/ins.h>

#define V0_INSTS_MAX 256

/*
 * V0 INSTRUCTION SET ARCHITECTURE
 * -------------------------------
 */

/* instruction opcodes */
#define V0_LOGIC_UNIT 0x01 // NOT, AND, IOR, XOR
#define V0_SHIFT_UNIT 0x02 // SHL, SHR, SAR, ROL, ROR
#define V0_ADD_UNIT   0x03 // ADD, ADU, ADC, AUC, SUB, SBU, SBC, SUC, CMP
#define V0_MUL_UNIT   0x04 // MUL, MLU, MLH, MHU, DIV, DVU, DIR, DUR, RPS, RPU
#define V0_SIGN_UNIT  0x05
#define V0_MISC_UNIT  0x06
#define V0_MEM_UNIT   0x07
#define V0_STK_UNIT   0x08
#define V0_BIT_UNIT   0x09
#define V0_ATOM_UNIT  0x0a
#define V0_JMP_UNIT   0x0b
#define V0_SUB_UNIT   0x0c
#define V0_SYS_UNIT   0x0d
#define V0_IO_UNIT    0x0e
#define V0_CTL_UNIT   0x0f

/* integer limits */
#define V0_IMM_MIN    (-0x7fff - 1)
#define V0_IMM_MAX    0x7fff

/* bits for MFR feature register and CR control register */
#define V0_THR_BIT    (1 << 31) // multitasking and multicore support
#define V0_VM_BIT     (1 << 30) // [virtual] memory management/paging support
#define V0_FPU_BIT    (1 << 29) // floating-point instructions present
#define V0_FPM_BIT    (1 << 28) // fixed-point instructions present
#define V0_DSP_BIT    (1 << 27) // standard signal processing support
#define V0_SIMD_BIT   (1 << 26) // single instruction, multiple data vectors
#define V0_VEC_BIT    (1 << 25) // vector operation

#endif /* __V0_ISA_H__ */

