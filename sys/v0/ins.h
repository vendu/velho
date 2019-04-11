#Ifndef __V0_INS_H__
#define __V0_INS_H__

#include <stdint.h>
#include <valhalla/cdefs.h>
#include <valhalla/param.h>
#include <v0/types.h>

/* NOP is declared as all 1-bits in code */
#define V0_NOP_CODE             0xffff

#define V0_SYS_UNIT             0x00
#define V0_ALU_UNIT             0x01
#define V0_MULTI_UNIT           0x02
#define V0_SHIFT_UNIT           0x03
#define V0_BIT_UNIT             0x04
#define V0_ATOM_UNIT            0x05
#define V0_FLOW_UNIT            0x06
#define V0_XFER_UNIT            0x07

/* SYS-unit */
/*
 * Instruction  Arguments       Brief
 * -----------  ---------       -----
 * HLT          ri1             halt until interrupt, ri1 is interrupt mask
 * RST          N/A             reset system
 * INT          ri1             raise interrupt, ri1 is number
 * STI          N/A             enable interrupts
 * CLI          N/A             disable interrupts
 * WFE          ri1             wait for event on cacheline
 * SEV          ri1             signal event on cacheline
 * RPC          ri1, r2         read performance counter
 * LDX          ria1, r2        load system register
 * STX          r1, adr         store system register
 * IPG          ri1             invalidate TLB page entry
 * PFC          ri1             prefetch cacheline
 * FLC          ri1             flush/invalidate caches
 * BLK          ri1             lock page (V0_MEM_LOCK_BIT in page-entry)
 * BRD          N/A             memory read barrier
 * BWR          N/A             memory write barrier
 * BAR          N/A             full memory barrier
 */

#define V0_STOP_OP              0x00
#define V0_INTR_OP              0x01
#define V0_EVENT_OP             0x02
#define V0_XREG_OP              0x03
#define V0_PAGE_OP              0x04
#define V0_CACHE_OP             0x05
#define V0_MEM_OP               0x06
#define V0_HLT_OP       	V0_STOP_OP      // FLAG1-bit clear
#define V0_RST_OP       	V0_STOP_OP      // FLAG1-bit set
#define V0_INT_OP               V0_INTR_OP      // VAL-bit + 8-bit interrupt ID
#define V0_STI_OP       	V0_INTR_OP      // FLAG1-bit set
#define V0_CLI_OP       	V0_INTR_OP      // FLAG2-bit set
#define V0_WFE_OP       	V0_EVENT_OP     // FLAG1-bit set
#define V0_SEV_OP       	V0_EVENT_OP     // FLAG2-bit set
#define V0_REV_OP               V0_EVENT_OP     // VAL-bit + counter ID in code
#define V0_LDX_OP               V0_XREG_OP      // FLAG1-bit set
#define V0_STX_OP               V0_XREG_OP      // FLAG2-bit set
#define V0_IPG_OP       	V0_PAGE_OP      // FLAG1-bit clear
#define V0_PFC_OP       	V0_CACHE_OP     // FLAG1-bit clear
#define V0_FLC_OP       	V0_CACHE_OP     // FLAG1-bit set
#define V0_BLK_OP               V0_MEM_OP       // no flag-bits
#define V0_BRD_OP               V0_MEM_OP       // FLAG1-bit set
#define V0_BWR_OP               V0_MEM_OP       // FLAG2-bit set
#define V0_BAR_OP               V0_MEM_OP       // FLAG1-bit and FLAG2-bit set

/* ALU-unit */
/*
 * Instruction  Arguments       Brief
 * -----------  ---------       -----
 * NOT          ri1, r2         reverse all bits
 * NEG          ri1, r2         arithmetic negation
 * IOR          ri1, r2         logical [inclusive] OR
 * XOR          ri1, r2         logical exclusive OR
 * AND          ri1, r2         logical AND
 * INC          r1              increment by one
 * DEC          r1              decrement by one
 * ADD          ri1, r2         addition
 * ADC          ri1, r2         addition with carry
 * SUB          ri1, r2         subtraction
 * SBB          ri1, r2         subtraction with borrow
 * CMP          ri1, r2         comparison, set flags
 * ZEX          ri1, r2         zero-extend
 * SEX          ri1, r2         sign-extend
 */
#define V0_INV_OP               0x00
#define V0_OR_OP                0x01
#define V0_AND_OP               0x02
#define V0_UNADD_OP             0x03
#define V0_ADDER_OP             0x04
#define V0_EXT_OP               0x05
#define V0_NOT_OP               V0_INV_OP       // FLAG1-bit clear
#define V0_NEG_OP               V0_INV_OP       // FLAG1-bit set
#define V0_IOR_OP               V0_OR_OP        // FLAG1-bit clear
#define V0_XOR_OP               V0_OR_OP        // FLAG1-bit set
#define V0_AND_OP               V0_AND_OP       // FLAG1-bit set
#define V0_INC_OP               V0_UNADD_OP     // FLAG1-bit clear
#define V0_DEC_OP               V0_UNADD_OP     // FLAG1-bit set
#define V0_ADD_OP               V0_ADDER_OP     // no flag bits
#define V0_ADC_OP               V0_ADDER_OP     // FLAG1-bit set
#define V0_SUB_OP               V0_ADDER_OP     // FLAG2-bit set
#define V0_SBB_OP               V0_ADDER_OP     // FLAG1- and FLAG2-bits set
#define V0_CMP_OP               V0_ADDER_OP     // VAL-bit set
#define V0_ZEX_OP               V0_EXT_OP       // FLAG1-bit clear
#define V0_SEX_OP               V0_EXT_OP       // FLAG1-bit set

/* MULTI-unit */
/*
 * Instruction  Arguments       Brief
 * -----------  ---------       -----
 * MUL          ri1, r2         multiplication
 * MLH          ri1, r2         multiplication, return high result word
 * DIV          ri1, r2         division
 * REM          ri1, r2         remainder
 * MAD          ri1, r2         multiply and add
 * MSL          ri1, r2         multiply and shift left
 * MSR          ri1, r2         multiply and shift right
 * IRP          ri1, r2         calculate inverse reciprocal 1/D
 */
#define V0_MULTI_OP             0x00
#define V0_DIVIDE_OP            0x01
#define V0_MULADD_OP            0x02
#define V0_MULSH_OP             0x03
#define V0_RECIP_OP             0x04
#define V0_MUL_OP       	V0_MULTI_OP
#define V0_MLH_OP               V0_MULTI_OP     // FLAG1-bit set
#define V0_DIV_OP       	V0_DIVIDE_OP    // no flag-bits
#define V0_REM_OP       	V0_DIVIDE_OP    // FLAG1-bit set
#define V0_MAD_OP               V0_MULADD_OP    // no flag-bits
#define V0_MSL_OP               V0_MULSH_OP     // FLAG1-bit clear
#define V0_MSR_OP               V0_MULSH_OP     // FLAG1-bit set
#define V0_IRP_OP       	V0_RECIP_OP     // FLAG1-bit clear

/* SHIFT-unit */
/*
 * Instruction  Arguments       Brief
 * -----------  ---------       -----
 * SHL          rv1, r2         shift left
 * SHR          rv1, r2         shift right
 * SAR          rv1, r2         shift right, arithmetic (fill with sign)
 * ROL          rv1, r2         rotate left
 * ROR          rv1, r2         rotate right
 * SLA          rv1, r2         shift left and add
 * SRA          rv1, r2         shift right and add
 * SLM          v, r1, r2       shift left and mask
 * SRM          v, r1, r2       shift right and mask
 */
#define V0_SHIFT_OP             0x00
#define V0_SHADD_OP             0x01
#define V0_SHMASK_OP            0x02
#define V0_SHL_OP       	V0_SHIFT_OP
#define V0_SHR_OP       	V0_SHIFT_OP     // FLAG1-bit set
#define V0_SAR_OP       	V0_SHIFT_OP     // FLAG1- and FLAG2-bits set
#define V0_ROL_OP       	V0_SHIFT_OP     // VAL-bit set
#define V0_ROR_OP       	V0_SHIFT_OP     // VAL- and FLAG1-bits set
#define V0_SLA_OP               V0_SHADD_OP     // FLAG1-bit clear
#define V0_SRA_OP               V0_SHADD_OP     // FLAG1-bit set
#define V0_SLM_OP       	V0_SHMASK_OP    // FLAG1-bit clear
#define V0_SRM_OP       	V0_SHMASK_OP    // FLAG1-bit set

/* BIT-unit */
/*
 * Instruction  Arguments       Brief
 * -----------  ---------       -----
 * CLZ          ri1, r2         count leading zero-bits
 * HAM          ri1, r2         Hamming weight, count one-bits
 * PAR          ri1, r2         calculate parity
 * SWP          ri1, r2         swap byte-order
 * EBD          ri1, r2         encode binary-coded decimal
 * DBD          ri1, r2         decode binary-coded decimal
 * CRC          ri1, r2         compute 16-bit IPv4 checksum
 * ECC          ri1, r2         compute 32-bit ECC checksum
 * HSH          ri1, r2         hash value
 * UNH          ri1, r2         unhash value
 */
#define V0_BITCNT_OP            0x00
#define V0_BSWAP_OP             0x01
#define V0_BCD_OP               0x02
#define V0_CHK_OP               0x03
#define V0_HASH_OP              0x04
#define V0_CLZ_OP       	V0_BITCNT_OP    // FLAG1-bit clear
#define V0_HAM_OP       	V0_BITCNT_OP    // FLAG1-bit set
#define V0_PAR_OP       	V0_BITCNT_OP    // FLAG2-bit set
#define V0_SWP_OP       	V0_BSWAP_OP
#define V0_EBD_OP       	V0_BCD_OP       // FLAG1-bit clear
#define V0_DBD_OP       	V0_BCD_OP       // FLAG1-bit set
#define V0_CRC_OP       	V0_CHK_OP       // compute 16-bit IPv4 checksum
#define V0_ECC_OP               V0_CHK_OP       // FLAG1-bit set
#define V0_HSH_OP       	V0_HASH_OP      // hash using tmhash32()
#define V0_HUN_OP       	V0_HASH_OP      // FLAG1-bit set, unhash

/* ATOM-unit */
/*
 * Instruction  Arguments       Brief
 * -----------  ---------       -----
 * LDL          adr, r2         load linked
 * STL          r1, adr         store linked/conditional
 * BTC          rv1, adr        bit test and clear
 * BTS          rv1, adr        bit test and set
 * XADD         i, adr, r2      fetch and add
 * XINC         adr, r2         fetch and increment
 * XDEC         adr, r2         fetch and decrement
 * CAS          i, r1, adr      compare and swap
 */
#define V0_LLSC_OP              0x00
#define V0_BITXCHG_OP           0x01
#define V0_FETCHADD_OP          0x02
#define V0_CAS_OP               0x03
#define V0_LDL_OP       	V0_LLSC_OP      // FLAG1-bit clear
#define V0_STL_OP       	V0_LLSC_OP      // FLAG2-bit set
#define V0_BTC_OP       	V0_BITXCHG_OP   // FLAG1-bit clear
#define V0_BTS_OP               V0_BITXCHG_OP   // set if clear, old in CF
#define V0_XADD_OP      	V0_FETCHADD_OP  // no flag-bits
#define V0_XINC_OP              V0_FETCHADD_OP  // FLAG1-bit set
#define V0_XDEC_OP      	V0_FETCHADD_OP  // FLAG2-bit set

/*
 * Code	MSW-Bits	Description
 * ----	--------	-----------
 * EQ	ZF		equal
 * NE	!ZF		not equal
 * LT	!ZF && LT	less than
 * LO	!CF    		lower (unsigned < 0)
 * LE	ZF || LT	less than or equal
 * LS	!CF || ZF	lower or same (unsigned <=)
 * GT	!ZF && !LT	greater than
 * HI	CF && !ZF	higher (unsigned > 0)
 * GE	ZF || !LT	greater than or equal
 * HS	CF   		higher or same (unsigned >=)
 * CF	CF		carry set
 * NC	!CF		carry not set
 * OF	OF		overflow set
 * NO	!OF		overflow not set
 * SF   SF              sign-flag set
 * NS   !SF             sign-flag not set
 */

/* FLOW-unit */
/*
 * Instruction  Arguments       Brief
 * -----------  ---------       -----
 * JMP                          jump absolute
 * JMR                          jump relative
 * BEQ                          branch if equal
 * BZF                          branch if zero
 * BNE                          branch if not equal
 * BNZ                          branch if not zero
 * BLT
 * BLE
 * BGT
 * BGE
 * BCF
 * BNC
 * BOF
 * BNO
 * BSF
 * BNS
 * BEG                          subroutine prologue
 * FIN                          subroutine epilogue
 * MKF                          construct call stack-frame
 * CSR                          call subroutine
 * SYS                          call system routine
 * THR                          start thread
 * THC                          thread command/control operation
 * THW                          thread wait
 * THY                          thread yield
 * RET                          return from subroutine
 * SRT                          return from system routine
 * THX                          exit thread
 * IRT                          return from interrupt routine
 */
#define V0_JUMP_OP              0x00
#define V0_BRANCH_OP            0x01
#define V0_FRAME_OP             0x02
#define V0_CALL_OP              0x03
#define V0_RETURN_OP            0x04
#define V0_JMP_OP               V0_JMP_OP       // no flag-bits
#define V0_JMR_OP               V0_JMP_OP       // FLAG1-bit set
#define V0_BEQ_OP               V0_BRANCH_OP
#define V0_BZF_OP               V0_BRANCH_OP
#define V0_BNE_OP               V0_BRANCH_OP
#define V0_BNZ_OP               V0_BRANCH_OP
#define V0_BLT_OP               V0_BRANCH_OP
#define V0_BLE_OP               V0_BRANCH_OP
#define V0_BGT_OP               V0_BRANCH_OP
#define V0_BGE_OP               V0_BRANCH_OP
#define V0_BCF_OP               V0_BRANCH_OP
#define V0_BNC_OP               V0_BRANCH_OP
#define V0_BOF_OP               V0_BRANCH_OP
#define V0_BNO_OP               V0_BRANCH_OP
#define V0_BSF_OP               V0_BRANCH_OP
#define V0_BNS_OP               V0_BRANCH_OP
#define V0_BEG_OP               V0_FRAME_OP      // FLAG1-bit set
#define V0_FIN_OP               V0_FRAME_OP     // FLAG1-bit set
#define V0_MKF_OP               V0_FRAME_OP     // VAL-bit set, # of args in val
#define V0_CSR_OP               V0_CALL_OP      // no flag-bits
#define V0_SYS_OP               V0_CALL_OP      // FLAG1-bit set
#define V0_THR_OP               V0_THREAD_OP    // no flag-bits
#define V0_THC_OP               V0_THREAD_OP    // VAL-bit set + command in val
#define V0_THW_OP               V0_THREAD_OP    // FLAG1-bit set
#define V0_THY_OP               V0_THREAD_OP    // FLAG2-bit set
#define V0_RET_OP               V0_RETURN_OP    // no flag-bits
#define V0_SRT_OP               V0_RETURN_OP    // FLAG1-bit set
#define V0_THX_OP               V0_RETURN_OP    // FLAG2-bit set
#define V0_IRT_OP               V0_RETURN_OP    // FLAG1- and FLAG2-bits set

/* XFER-unit */
/*
 * Instruction  Arguments       Brief
 * -----------  ---------       -----
 * LDR                          load register
 * LDN                          load non-temporal
 * LDC                          load conditional
 * STR                          store register
 * STN                          store non-temporal
 * STC                          store conditional
 * PSH                          push register
 * PSM                          push register-range
 * POP                          pop register
 * POM                          pop register-range
 * IOC                          I/O command
 * IOP                          I/O permission
 * IOR                          read I/O port
 * IOW                          write I/O port
 */
#define V0_LOAD_OP              0x00
#define V0_STORE_OP             0x01
#define V0_STACK_OP             0x02
#define V0_IO_OP                0x03
#define V0_LDR_OP               V0_LOAD_OP      // no flag-bits
#define V0_LDN_OP               V0_LOAD_OP      // FLAG1-bit set
#define V0_LDC_OP               V0_LOAD_OP      // VAL-bit + condition bits
#define V0_STR_OP               V0_STORE_OP     // no flag-bits
#define V0_STN_OP               V0_STORE_OP     // FLAG1-bit set
#define V0_STC_OP               V0_STORE_OP     // VAL-bit + condition bits
#define V0_PSH_OP               V0_STACK_OP     // FLAG1-bit clear
#define V0_PSM_OP       	V0_STACK_OP     // FLAG1-bit set
#define V0_POP_OP       	V0_STACK_OP     // FLAG2-bit set
#define V0_POM_OP       	V0_STACK_OP     // FLAG1- and FLAG2-bits set
#define V0_IOC_OP               V0_IO_OP        // VAL-bit + command in val
#define V0_IOP_OP               V0_IO_OP        // no flag-bits
#define V0_IOR_OP               V0_IO_OP        // FLAG1-bit set
#define V0_IOW_OP               V0_IO_OP        // FLAG2-bit set

#endif /* __V0_INS_H__ */

