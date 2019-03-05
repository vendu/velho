`ifndef V0_MACRO_V
`define V0_MACRO_V

`define V0_R0_REG         0  // zero register
`define V0_ZERO_REG       0
`define V0_RET_REG        1  // function return value register
/* CALLER-SAVE REGISTERS */
`define V0_R1_REG         1  // function return value, first function argument
`define V0_R2_REG         2  // extra return value, second function argument
`define V0_R3_REG         3  // third function argument
`define V0_R4_REG         4  // fourth function argument
`define V0_R5_REG         5  // fifth function argument
`define V0_R6_REG         6  // sixth function argument
`define V0_R7_REG         7  // seventh function argument
`define V0_REG_ARGS       7  // R1..R7, function [caller-save] arguments
/* CALLEE-SAVE REGISTERS */
`define V0_R8_REG         8  // scratch register #1
`define V0_R9_REG         9  // scratch register #2
`define V0_R10_REG        10 // scratch register #3
`define V0_R11_REG        11 // scratch register #4
`define V0_R12_REG        12 // scratch register #5
`define V0_R13_REG        13 // scratch register #6
`define V0_R14_REG        14 // scratch register #7
`define V0_R15_REG        15 // stratch register #8
`define V0_INT_REGS       16 // # of integer/scalar registers
`define V0_CALL_REG_0     1
`define V0_CALL_REG_MAX   7
`define V0_CALL_REGS      7  // caller saves r1..r7, callee r8..r15

// SYSTEM REGISTERS
`define V0_PC             0  // program counter
`define V0_FP             1  // frame pointer
`define V0_SP             2  // stack pointer
`define V0_CNT            3  // function argument count
`define V0_LNK            4  // link register return address
`define V0_MSW            5  // machine status word
`define V0_IVR            6  // interrupt vector address
`define V0_IMR            7  // interrupt-mask 1-bit enabled
`define V0_THR            8  // thread ID + permission flags
`define V0_PDR            9  // page directory address
`define V0_IOM            10 // I/O descriptor map address
`define V0_FP0            11 // system frame-pointer ring 0
`define V0_SP0            12 // system stack-pointer ring 0
`define V0_IV0            13 // system-mode interrupt mask
`define V0_PD0            14 // system-mode page-directory
/* READ-ONLY REGISTER */
`define V0_MFW            15 // machine feature word
`define V0_SYS_REGS       16
`define V0_SYS_REG(x)     (x)
`define V0_CALLEE_REG_0   0
`define V0_CALLEE_REG_MAX 15
`define V0_CALLEE_REGS    16

`define V0_STD_REGS     (`V0_INT_REGS + `V0_SYS_REGS) // total user and system

`endif /* V0_MACRO_V */

