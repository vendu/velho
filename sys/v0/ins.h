#ifndef __V0_INS2_H__
#define __V0_INS2_H__

#include <stdint.h>
#include <valhalla/cdefs.h>
#include <valhalla/param.h>
#include <v0/types.h>

/* NOP is declared as all 1-bits in code */
#define V0_NOP_CODE             0xffff

#define V0_SYS_UNIT     	0x00
#define V0_ALU_UNIT     	0x01
#define V0_MULTI_UNIT     	0x02
#define V0_SHIFT_UNIT   	0x03
#define V0_BIT_UNIT     	0x04
#define V0_ATOM_UNIT    	0x05
#define V0_FLOW_UNIT            0x06
#define V0_XFER_UNIT            0x07

/* SYS-unit */
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
#define V0_PFN_OP       	V0_CACHE_OP     // FLAG1-bit set
#define V0_FLC_OP       	V0_CACHE_OP     // FLAG2-bit set
#define V0_WBC_OP       	V0_CACHE_OP     // FLAG1- and FLAG2-bits set
#define V0_BLK_OP               V0_MEM_OP       // no flag-bits
#define V0_BRD_OP               V0_MEM_OP       // FLAG1-bit set
#define V0_BWR_OP               V0_MEM_OP       // FLAG2-bit set
#define V0_BAR_OP               V0_MEM_OP       // FLAG1-bit and FLAG2-bit set

/* ALU-unit */
#define V0_INV_OP               0x00
#define V0_OR_OP                0x01
#define V0_UNADD_OP             0x02
#define V0_ADDER_OP             0x03
#define V0_EXT_OP               0x04
#define V0_NOT_OP       	V0_INV_OP       // FLAG1-bit clear
#define V0_NEG_OP       	V0_INV_OP       // FLAG1-bit set
#define V0_AND_OP       	V0_INV_BIT      // FLAG1-bit set
#define V0_IOR_OP       	V0_OR_OP        // FLAG1-bit clear
#define V0_XOR_OP               V0_OR_OP        // FLAG1-bit set
#define V0_INC_OP       	V0_UNADD_OP     // FLAG1-bit clear
#define V0_DEC_OP               V0_UNADD_OP     // FLAG1-bit set
#define V0_ADD_OP       	V0_ADDER_OP
#define V0_ADC_OP       	V0_ADDER_OP     // FLAG1-bit set
#define V0_SUB_OP       	V0_ADDER_OP     // FLAG2-bit set
#define V0_SBB_OP       	V0_ADDER_OP     // FLAG1- and FLAG2-bits set
#define V0_CMP_OP       	V0_ADDER_OP     // VAL- and FLAG1-bits set
#define V0_ZEX_OP       	V0_EXT_OP       // FLAG1-bit clear
#define V0_SEX_OP       	V0_EXT_OP       // FLAG1-bit set

/* MULTI-unit */
#define V0_MULTI_OP             0x00
#define V0_DIVIDE_OP            0x01
#define V0_MULADD_OP            0x02
#define V0_MULSH_OP             0x03
#define V0_RECIP_OP             0x04
#define V0_MUL_OP       	V0_MULTI_OP
#define V0_UMUL_OP              V0_MULTI_OP     // FLAG1-bit set
#define V0_MLH_OP               V0_MULTI_OP     // FLAG2-bit set
#define V0_UMLH_OP              V0_MULTI_OP     // FLAG1- and FLAG2-bits set
#define V0_DIV_OP       	V0_DIVIDE_OP    // no flag-bits
#define V0_REM_OP       	V0_DIVIDE_OP    // FLAG1-bit set
#define V0_MAD_OP               V0_MULADD_OP    // no flag-bits
#define V0_MSL_OP               V0_MULSH_OP     // FLAG1-bit clear
#define V0_MSR_OP               V0_MULSH_OP     // FLAG1-bit set
#define V0_IRP_OP       	V0_RECIP_OP     // FLAG1-bit clear
#define V0_UIRP_OP              V0_RECIP_OP     // FLAG1-bit set

/* SHIFT-unit */
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
#define V0_CAS_OP       	V0_CAS_OP       // FLAG1-bit clear
#define V0_CAS2_OP      	V0_CAS_OP       // FLAG1-bit set

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
#define V0_BGE_OP               V0_BRANCH_OP
#define V0_BLE_OP               V0_BRANCH_OP
#define V0_BGT_OP               V0_BRANCH_OP
#define V0_BCF_OP               V0_BRANCH_OP
#define V0_BNC_OP               V0_BRANCH_OP
#define V0_BOF_OP               V0_BRANCH_OP
#define V0_BNO_OP               V0_BRANCH_OP
#define V0_BSF_OP               V0_BRANCH_OP
#define V0_BNS_OP               V0_BRANCH_OP
#define V0_CSR_OP               V0_SUBR_OP      // no flag-bits
#define V0_BEG_OP               V0_SUBR_OP      // FLAG1-bit set
#define V0_BEG_OP               V0_FRAME_OP     // no flag-bits
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

#endif /* __V0_INS2_H__ */

