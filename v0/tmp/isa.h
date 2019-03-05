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

/* special values */
#define V0_NOP      0x00      // dummy operation
#define V0_COP      0xff      // coprocessor prefix opcode
#define V0_ATOM     0xf4      // atomic instruction follows

/* instruction opcodes */
#define V0_LOGIC_OP 0x01 // NOT, AND, IOR, XOR
#define V0_SHIFT_OP 0x02 // SHL, SHR, SAR, ROL, ROR
#define V0_ADD_OP   0x03 // ADD, ADU, ADC, AUC, SUB, SBU, SBC, SUC, CMP
#define V0_MUL_OP   0x04 // MUL, MLU, MLH, MHU, DIV, DVU, DIR, DUR, RPS, RPU
#define V0_SIGN_OP  0x05
#define V0_MISC_OP  0x06
#define V0_MEM_OP   0x07
#define V0_STK_OP   0x08
#define V0_BIT_OP   0x09
#define V0_ATOM_OP  0x0a
#define V0_JMP_OP   0x0b
#define V0_SUB_OP   0x0c
#define V0_SYS_OP   0x0d
#define V0_IO_OP    0x0e
#define V0_CTL_OP   0x0f

/* integer limits */
#define V0_IMM_MIN  (-0x7fff - 1)
#define V0_IMM_MAX  0x7fff
#define V0_IMMU_MAX 0xffffU

* bits for MFR feature register and CR control register */
#define V0_MFR_THR  (1U << 31) // multitasking and multicore support
#define V0_MFR_VMEM (1U << 30) // [virtual] memory management/paging support
#define V0_MFR_FPU  (1U << 29) // floating-point instructions present
#define V0_MFR_FPM  (1U << 28) // fixed-point instructions present
#define V0_MFR_DSP  (1U << 27) // standard signal processing support
#define V0_MFR_SIMD (1U << 26) // single instruction, multiple data vectors
#define V0_MFR_VEC  (1U << 25) // vector operation

/* bits for CR control register */
/*
 * V0 REGISTER FILE
 * ----------------
 *
 * General-Purpose Registers
 * -------------------------
 * %r0          zero register; reads always return zero, writes are NOPs
 * %zr          synonymous with %r0
 * %r1          function return value, first caller save register
 * %r2..%7      function argument registers (the rest on stack)
 * %r7..%r15    scratch registers; callee-save
 *
 * System Registers
 * ----------------
 * %pc          program counter i.e. instruction pointer
 * %fp          frame pointer
 * %sp          stack pointer
 * %msw         machine status word (carry, overflow, interrupts, etc.)
 * %ln          function return address back to caller
 * %tr          thread-local storage segment info
 * %iv          interrupt vector [handler table] base address
 * %im          interrupt mask register; 1-bit denote enabled interrupts
 * %pd          page-directory base address for virtual memory
 * %io          I/O-map descriptor with paramters
 * %fp0         system-mode execution stack frame-pointer
 * %sp0         system-mode stack pointer
 * %iv0         system-mode interrupt vector pointer
 * %im0         system-mode interrupt mask register
 * %cr         control register (for system control)
 * %mfw         machine feature word, e.g. indication of optional hardware
 */
#define V0_ZERO_REG      V0_R0_REG // always returns zero, writes equal to NOP
#define V0_RET_REG       V0_R1_REG // function return value register
#define V0_REG_ARGS      7         // R1..R7, function register arguments
/* CALLER-SAVE REGISTERS */
#define V0_R0_REG        0x00 // zero register
#define V0_ZR_REG        V0_R0_REG
#define V0_R1_REG        0x01 // function return value, first function argument
#define V0_R2_REG        0x02 // extra return value, second function argument
#define V0_R3_REG        0x03 // third function argument
#define V0_R4_REG        0x04 // fourth function argument
#define V0_R5_REG        0x05 // fifth function argument
#define V0_R6_REG        0x06 // sixth function argument
#define V0_R7_REG        0x07 // seventh function argument
/* CALLEE-SAVE REGISTERS */
#define V0_R8_REG        0x08 // scratch register #1
#define V0_R9_REG        0x09 // scratch register #2
#define V0_R10_REG       0x0a // scratch register #3
#define V0_R11_REG       0x0b // scratch register #4
#define V0_R12_REG       0x0c // scratch register #5
#define V0_R13_REG       0x0d // scratch register #6
#define V0_R14_REG       0x0e // scratch register #7
#define V0_R15_REG       0x0f // stratch register #8
#define V0_LN_REG        V0_R15_REG // link register, i.e. return address
#define V0_INT_REGS      16   // # of integer/scalar registers
#define V0_SAVE_REG_MIN  V0_R1_REG
#define V0_SAVE_REG_MAX  V0_R7_REG
#define V0_SAVE_REGS     7    // caller saves r1..r7, callee r8..r15
/* SYSTEM REGISTERS */
#define V0_PC_ID         0x00 // program counter
#define V0_FP_ID         0x01 // frame pointer
#define V0_SP_ID         0x02 // stack pointer
#define V0_MSW_ID        0x03 // machine status word
#define V0_LN_ID         0x04 // link register (return address)
#define V0_TR_ID         0x05 // thread-local storage region
#define V0_IV_ID         0x06 // interrupt vector address
#define V0_IM_ID         0x07 // interrupt-mask (1-bit enabled)
#define V0_PD_ID         0x08 // page directory address + flags
#define V0_IOM_ID        0x09 // I/O descriptor map address + flags
#define V0_FP0_ID        0x0a // system frame-pointer (ring 0)
#define V0_SP0_ID        0x0b // system stack-pointer (ring 0)
#define V0_IV0_ID        0x0c // system-mode interrupt vector
#define V0_IM0_ID        0x0d // system-mode interrupt mask
#define V0_CR_ID         0x0e // control-register
#define V0_MFW_ID        0x0f // machine feature word
#define V0_SYS_REGS      16
#define V0_PC_REG        V0_SYSREG(V0_PC_ID)
#define V0_FP_REG        V0_SYSREG(V0_FP_ID)
#define V0_SP_REG        V0_SYSREG(V0_SP_ID)
#define V0_MSW_REG       V0_SYSREG(V0_MSW_ID)
#define V0_LN_REG        V0_SYSREG(V0_LN_ID)
#define V0_TR_REG        V0_SYSREG(V0_TR_ID)
#define V0_IV_REG        V0_SYSREG(V0_IV_ID)
#define V0_IM_REG        V0_SYSREG(V0_IM_ID)
#define V0_PD_REG        V0_SYSREG(V0_PD_ID)
#define V0_IOM_REG       V0_SYSREG(V0_IOM_ID)
#define V0_FP0_REG       V0_SYSREG(V0_FP0_ID)
#define V0_SP0_REG       V0_SYSREG(V0_SP0_ID)
#define V0_IV0_REG       V0_SYSREG(V0_IV0_ID)
#define V0_IM0_REG       V0_SYSREG(V0_IM0_ID)
#define V0_CR_REG        V0_SYSREG(V0_CTR_ID)
#define V0_MFW_REG       V0_SYSREG(V0_MFW_ID)

/* system register IDs */
#define V0_STD_REGS      (V0_INT_REGS + V0_SYS_REGS) // total # of registers
#define V0_FPU_REGS      (V0_INT_REGS)
#define V0_SYSREG(x)     (V0_INT_REGS + (x))         // system register indices
/* values for regs[V0_MSW_REG] */
#define V0_MSW_DEF_BITS  (V0_IF_BIT)
#define V0_MSW_ZF_BIT    (1U << 0)  // zero
#define V0_MSW_OF_BIT    (1U << 1)  // overflow
#define V0_MSW_CF_BIT    (1U << 2)  // carry-flag, return bit for BTR, BTS, BTC
#define V0_MSW_IF_BIT    (1U << 3)  // interrupts enabled
#define V0_MSW_FX_BIT    (1U << 4)  // floating-point exceptions pending
/* values for MFW- and MSW-registers */
#define V0_FP_BIT        (1U << 25) // fixed-point support
#define V0_FC_BIT        (1U << 26) // floating-point coprocessor present
#define V0_VC_BIT        (1U << 27) // vector coprocessor present
/* coprocessor flags */
#define V0_SP_BIT        (1U << 28) // SIMD support
#define V0_MP_BIT        (1U << 29) // multiprocessor [synchronous] mode
#define V0_PE_BIT        (1U << 30) // paging enabled (virtual memory)
/* system-ring bit */
#define V0_PM_BIT        (1U << 31) // privileged/system mode

#endif /* __V0_ISA_H__ */

