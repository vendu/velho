#ifndef __V0_ALU_H__
#define __V0_ALU_H__

#define sex(x, max)     (((x) ^ (max)) - (max))
#define sexb(b)         (((b) ^ 128) - 128)
#define sexh(b)         (((b) ^ 65536) - 128)

#define V0_UNIT_SYS     0
#define V0_UNIT_THR     1
#define V0_UNIT_ADD     2
#define V0_UNIT_MUL     3
#define V0_UNIT_BOOL    4
#define V0_UNIT_SHIFT   5
#define V0_UNIT_BIT     6
#define V0_UNIT_MEM     7
#define V0_UNIT_ATOM    8
#define V0_UNIT_IO      9

/* sys */
#define V0_HLT  0x00       // halt
#define V0_RST  0x01       // reset
#define V0_INT  0x02       // interrupt
#define V0_MPC  0x03       // multi-processor mode
#define V0_IMM  0x04       // initialize memory management
#define V0_IPC  0x05       // initialize page- and cache-management
#define V0_DS6  0x06       // deep six (shutdown)

/* thread control */
#define V0_THR  0x00 // launch thread
#define V0_TAT  0x01 // attach to ('join') thread
#define V0_TEX  0x02 // exit thread
#define V0_TWT  0x03 // wait/pause on cacheline event
#define V0_TRL  0x04 // release ('yield') thread

/* adder */
#define V0_INC  0x05 // increment by one
#define V0_DEC  0x06 // decrement by one
#define V0_NEG  0x07 // arithmetic negation
#define V0_ADD  0x08 // addition
#define V0_SUB  0x09 // subtraction

/* multiplication */
#define V0_RPC  0x00 // inverse reciprocal for replacing DIV with MUL
#define V0_MUL  0x01 // multiplication

/* boolean logic */
#define V0_NOT  0x00 // NOT
#define V0_XOR  0x01 // XOR
#define V0_IOR  0x02 // OR
#define V0_AND  0x03 // AND
#define V0_NAD  0x04 // NAND

/* shift */
#define V0_SHL  0x00 // left-shift
#define V0_SHR  0x01 // right-shift
#define V0_SAR  0x02 // arithmetic right-shift
#define V0_ROL  0x03 // rotate left
#define V0_ROR  0x04 // rotate right
#define V0_SLA  0x05 // shift left and add
#define V0_SRA  0x06 // shift right and add
#define V0_SLM  0x07 // shift left and mask
#define V0_SRM  0x08 // shift right and mask

/* bits */
#define V0_CLZ  0x00 // count leading zero-bits
#define V0_HAM  0x01 // hamming-weight/bit-population/count of one-bits
#define V0_BCL  0x02 // clear bit
#define V0_BST  0x03 // set bit
#define V0_BFL  0x04 // flip/toggle bit

/* load-store-memory-stack */
#define V0_LDR  0x00 // load register
#define V0_STR  0x01 // store register
#define V0_LDX  0x02 // load special-register
#define V0_STX  0x03 // store special-register
#define V0_BRD  0x04 // memory read-barrier
#define V0_BWR  0x05 // memory write-barrier
#define V0_BAR  0x06 // full memory-barrier
#define V0_PSH  0x07 // push register
#define V0_PSR  0x08 // push register-range
#define V0_POR  0x09 // pop register-range

/* atomic operations */
#define V0_LCL  0x00 // lock cache-line
#define V0_LDC  0x01 // load conditional
#define V0_STC  0x02 // store conditional
#define V0_CAB  0x03 // compare-and-swap bit
#define V0_CSB  0x04 // compare-and-set bit
#define V0_CCB  0x05 // compare-and-clear bit
#define V0_CFB  0x06 // compare-and-flip/toggle bit
#define V0_CAB  0x07 // compare-and-swap-bit
#define V0_CAS  0x08 // compare-and-swap word

/* input/output */
#define V0_IPS  0x00 // port-permission set
#define V0_IPR  0x01 // port-read
#define V0_IPW  0x02 // port-write
#define V0_IOC  0x03  // i/o command

/* NOTES
 * -----
 * ZF together with non-zero result indicates unsigned operation
 */
#define V0_FLAG_ZF   (1L << 0)  // zero/clear/on/off
#define V0_FLAG_CB   (1L << 2)  // carry/borrow
#define V0_FLAG_VF   (1L << 3)  // underflow/overflow
#define V0_FLAG_XF   (1L << 3)  // auxiliary/second parcel/immediate

#endif /* __V0_ALU_H__ */

