#ifndef __V0_REGS_H__
#define __V0_REGS_H__

#include <sys/v0/v0.h>
#if defined (V0_LITERAL_DOCS)
#include <sys/v0/docs/regs.txt>
#endif

#define V0_MAX_REGISTERS        32

//#define V0_REG_SIZE             32
#define V0_RET_REG      	V0_R0_REG // function return value register
#define V0_REG_ARGS             5 // R1..R5, function register arguments
#define V0_CALLEE_REGISTERS     10 // R6..R11, FP, SP, callee save arguments
#define V0_R0_REG               0               // zero register
#define V0_R1_REG               1               // function argument
#define V0_R2_REG               2               // function argument
#define V0_R3_REG               3               // function argument
#define V0_R4_REG               4               // function argument
#define V0_R5_REG               5               // scratch register
#define V0_R6_REG               6               // scratch register
#define V0_R7_REG               7               // scratch register
#define V0_R8_REG               8               // scratch register
#define V0_R9_REG               9               // scratch register
#define V0_R10_REG              10              // scratch register
#define V0_R11_REG              11              // scratch register
#define V0_R12_REG      	12              // frame-pointer
#define V0_R13_REG      	13              // stack-pointer
#define V0_R14_REG      	14              // link-register
#define V0_R15_REG      	15              // program-counter
#define V0_ZR_REG               0x00            // zero register
#define V0_FP_REG               V0_R12_REG      // store stack frame address
#define V0_SP_REG               V0_R13_REG      // store current stack pointer
#define V0_LR_REG               V0_R14_REG      // link register/return address
#define V0_PC_REG               V0_R15_REG      // program counter
#define V0_GENERAL_REGISTERS    16              // # of general registers

/* SYSTEM REGISTERS */
#define V0_MSW_REG      	0x00 // machine status word
#define V0_TCB_REG      	0x01 // thread control block base address
#define V0_MFR_REG      	0x02 // machine feature word, read-only
#define V0_CR_REG               0x03 // control register
#define V0_IM_REG       	0x04 // interrupt-mask (1-bit enabled)
#define V0_IV_REG       	0x05 // interrupt vector address
#define V0_PD_REG       	0x06 // page directory address + flags
#define V0_IOM_REG              0x07 // I/O descriptor map base address
#define V0_BLO_REG      	0x08 // bound-range low limit
#define V0_BHI_REG      	0x09 // bound-range high limit (base + size)
#define V0_TLO_REG      	0x0a // timestamp low 32 bits
#define V0_THI_REG      	0x0b // timestamp high 32 bits
#define V0_TLS_REG      	0x0c // thread-local storage base address
#define V0_TWC_REG       	0x0d // thread wait channel
#define V0_SP0_REG              0x0e // ring #0 (system-mode) stack-pointer
#define V0_SYSTEM_REGISTERS     16

/* values for MSW */
#define V0_MSW_DEF_BITS 	(V0_IF_BIT)
#define V0_MSW_THR_MASK         ((1 << V0_THR_ID_BITS) - 1)
#define V0_MSW_FLAG_SHIFT       V0_THR_ID_BITS
#define V0_MSW_ZF_BIT   	(1 << (V0_MSW_FLAG_SHIFT + 0)) // zero
#define V0_MSW_CF_BIT   	(1 << (V0_MSW_FLAG_SHIFT + 1)) // carry, return

/* bits for BTR, BTS, BTC */
#define V0_MSW_OF_BIT   	(1 << (V0_MSW_FLAG_SHIFT + 2)) // overflow
#define V0_MSW_LT_BIT   	(1 << (V0_MSW_FLAG_SHIFT + 3)) // less than
#define V0_MSW_SF_BIT   	(1 << (V0_MSW_FLAG_SHIFT + 4)) // signed result
#define V0_MSW_ST_BIT           (1 << (V0_MSW_FLAG_SHIFT + 5)) // saturation
#define V0_MSW_IF_BIT           (1 << 28)
#define V0_MSW_RF_BIT           (1 << 29)
#define V0_MSW_MP_BIT           (1 << 30)
#define V0_MSW_FP_BIT           (1 << 31)

/* values for MFW- and CR-registers */
#define V0_SYS_MP_BIT   	(1 << 31) // multiprocessor support
#define V0_SYS_PG_BIT   	(1 << 30) // paging/virtual memory enabled
#define V0_SYS_IO_BIT   	(1 << 29) // I/O permission control
#define V0_SYS_FP_BIT   	(1 << 28) // floating-point processor present
#define V0_SYS_FM_BIT   	(1 << 27) // fixed-point support
#define V0_SYS_GP_BIT   	(1 << 26) // graphics processor present
#define V0_SYS_DP_BIT   	(1 << 25) // DSP support
#define V0_SYS_MD_BIT   	(1 << 24) // SIMD support
#define V0_SYS_VP_BIT   	(1 << 23) // vector processor present

/* values for CR */
#define V0_CR_RING_BIT  	(1 << 0) // system (0) or user (1) mode
#define V0_CR_IOP_BIT      	(1 << 1) // I/O permission checks

/* TR-register */
#define V0_TR_ID_MASK           0x000000ff // 256 unique thread IDs
#define V0_TR_STAT_MASK         0x0000ff00 // thread status
#define V0_TR_INTR_MASK         0x00ff0000 // thread interrupt-mask
#define V0_TR_FLAG_MASK         0xff000000 // thread-flags for system use

/* IOM-register */
#define V0_IOM_BASE_MASK        0xfffffc00 // page-aligned
#define V0_IOM_NUM_MASK         0x000003ff // number of descriptors in map

/* BR-register */
#define V0_BR_LOW_LIM           (1 << 31)
#define V0_BR_ADR_MASK          (~V0_BR_LOW_LIM)

#endif /* __V0_REGS_H__ */

