#ifndef __V0_ALU_H__
#define __V0_ALU_H__

#define sex(x, max)     (((x) ^ (max)) - (max))
#define sexb(b)         (((b) ^ 128) - 128)
#define sexh(b)         (((b) ^ 65536) - 128)

/* sys */
#define V0_HLT        0 // halt
#define V0_RST       -1 // reset
#define V0_INT       -2 // interrupt
#define V0_MPC       -3 // multi-processor mode
#define V0_IMM       -4 // initialize memory management
#define V0_IPC       -5 // initialize page- and cache-management
#define V0_DS6       -6 // deep six

#define V0_INC        1 // increment by one
#define V0_DEC        2 // decrement by one
#define V0_NEG        3 // arithmetic negation
#define V0_ADD        4 // addition
#define V0_SUB        5 // subtraction
/* */
#define V0_RPC        6 // inverse reciprocal for replacing DIV with MUL
#define V0_MUL        7 // multiplication

/* logic */
#define V0_NOT        8 // NOT
#define V0_XOR        9 // XOR
#define V0_IOR       10 // OR
#define V0_AND       11 // AND
#define V0_NAD       12 // NAND

/* shift */
#define V0_SHL       13 // left-shift
#define V0_SHR       14 // right-shift
#define V0_SAR       15 // arithmetic right-shift
#define V0_ROL       16 // rotate left
#define V0_ROR       17 // rotate right
#define V0_SLA       18 // shift left and add
#define V0_SRA       19 // shift right and add
#define V0_SLM       20 // shift left and mask
#define V0_SRM       21 // shift right and mask

/* bits */
#define V0_CLZ       22 // count leading zero-bits
#define V0_HAM       23 // hamming-weight/bit-population/count of one-bits
#define V0_BCL       24 // clear bit
#define V0_BST       25 // set bit
#define V0_BFL       26 // flip/toggle bit

/* load-store-memory-stack */
#define V0_LDR       27 // load register
#define V0_STR       27 // store register
#define V0_LDX       29 // load special-register
#define V0_STX       30 // store special-register
#define V0_BRD       31 // memory read-barrier
#define V0_BWR       32 // memory write-barrier
#define V0_BAR       33 // full memory-barrier
#define V0_PSH       34 // push register
#define V0_PSR       35 // push register-range
#define V0_POR       36 // pop register-range

/* atomic operations */
#define V0_LCL       37 // lock cache-line
#define V0_LDC       38 // load conditional
#define V0_STC       39 // store conditional
#define V0_CAB       40 // compare-and-swap bit
#define V0_CSB       41 // compare-and-set bit
#define V0_CCB       42 // compare-and-clear bit
#define V0_CFB       43 // compare-and-flip/toggle bit
#define V0_CAB       44 // compare-and-swap-bit
#define V0_CAS       45 // compare-and-swap word

/* input/output */
#define V0_IPS       46 // set port-permission
#define V0_IPR       47 // port-read
#define V0_IPW       48 // port-write
#define V0_IOC       49 // i/o command

#define V0_UNIT_BITS  6
#define V0_INS_BITS   6
#define V0_FLAG_BITS  4

/* NOTES
 * -----
 * ZF together with non-zero result indicates unsigned operation
 */
#define V0_FLAG_ZF    (1L << 0) // zero/clear/on/off
#define V0_FLAG_CB    (1L << 2) // carry/borrow
#define V0_FLAG_VF    (1L << 3) // underflow/overflow
#define V0_FLAG_XF    (1L << 3) // auxiliary/second parcel/immediate

#endif /* __V0_ALU_H__ */

