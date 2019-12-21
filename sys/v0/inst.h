#ifndef __V0_INST_H__
#define __V0_INST_H__

#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <v0/v0.h>
#if defined(V0_LITERAL_DOCS)
#include <v0/doc/sys.txt>
#include <v0/doc/alu.txt>
#include <v0/doc/multi.txt>
#include <v0/doc/shift.txt>
#include <v0/doc/bit.txt>
#include <v0/doc/atom.txt>
#include <v0/doc/flow.txt>
#include <v0/doc/xfer.txt>
#endif /* V0_LITERAL_DOCS */

/* instruction format */

/* unit-member */
#define V0_OP_UNIT_MASK         0x000f
#define V0_OP_OP_MASK           0x00f0
#define V0_OP_FLAG_MASK         0xf000
/* flag-member bits */
#define V0_UNIT_BITS            4
#define V0_OP_BITS              4
#define V0_VAL_BITS             15
/* val-member values */
#define V0_VAL_BIT              (1 << 15)       // 7-bit value present
/* v0-size operations if both V0_HALF_BIT and V0_BYTE_BIT are clear */
#define V0_HALF_BIT             (1 << 14)       // halfword (16-bit) operation
#define V0_BYTE_BIT             (1 << 13)       // byte (8-bit) operation
#define V0_SIGN_BIT             (1 << 12)       // signed operation
#define V0_VAL_MASK             0x7f00
#define V0_FLAG_MASK            V0_VAL_MASK
#define V0_FLAG1_BIT            0x4000  // operation flag-bit #1
#define V0_FLAG2_BIT            0x2000  // operation flag-bit #2
#define V0_SIGN_MASK            0x1000
#define V0_NO_ADR       	0x0000  // register-only operands
#define V0_NDX_ADR      	0x4000  // indexed, e.g. $4(%sp) or direct
#define V0_REG_ADR      	0x2000  // base register, e.g. *%r1 or *%pc
#define V0_ADR_MASK     	0x6000  // addressing-mode mask
#define V0_ADR_MODE_BITS        2       // address-mode for load-store
#define V0_REG_BITS             4       // bits per register ID
#define V0_REG_MASK             0x000f
#define V0_STK_FLAG_BITS        3       // stack operation flags
#define V0_STK_PROT_BIT         0x0100  // stack protector
#define V0_STK_RED_BIT          0x0200  // stack protector red-zone
#define V0_STK_CTL_BIT          0x0300  // control register range
#define V0_IO_MAP_MASK          0x7fff
#define V0_IO_FLAG_MASK         0x6000
#define V0_IO_FLAG_BIT          0x8000
#define V0_IO_MAP_BITS          16
#define V0_IO_FLAG_BITS         2

/* NOP is declared as all 1-bits in code */
#define V0_NOP_CODE             0xffff

/* unit-names */
#define V0_SYS_UNIT             0x00
#define V0_ALU_UNIT             0x01
#define V0_MULTI_UNIT           0x02
#define V0_SHIFT_UNIT           0x03
#define V0_BIT_UNIT             0x04
#define V0_ATOM_UNIT            0x05
#define V0_FLOW_UNIT            0x06
#define V0_XFER_UNIT            0x07
#define V0_STD_UNITS            8
#define V0_FPU_UNIT             0x08
#define V0_GPU_UNIT             0x09
#define V0_DSP_UNIT             0x0a
#define V0_EXTRA_UNITS          8
#define V0_UNITS                16

/* SYS-unit */
#define V0_STOP_OP              0x00
#define V0_TIMER_OP             0x01
#define V0_INTR_OP              0x02
#define V0_CL_OP                0x03
#define V0_EVENT_OP             0x04
#define V0_XREG_OP              0x05
#define V0_PAGE_OP              0x06
#define V0_CACHE_OP             0x07
#define V0_MEM_OP               0x08
#define V0_HLT_OP               V0_STOP_OP      // FLAG1-bit clear
#define V0_RST_OP               V0_STOP_OP      // FLAG1-bit set
#define V0_INT_OP               V0_INTR_OP      // VAL-bit + 8-bit interrupt ID
#define V0_STI_OP               V0_INTR_OP      // FLAG1-bit set
#define V0_CLI_OP               V0_INTR_OP      // FLAG2-bit set
#define V0_CLT_OP               V0_TIMER_OP     // no flag-bits, ID in imm
#define V0_RDT_OP               V0_TIMER_OP     // VAL-bit set, ID in bits
#define V0_RCL_OP               V0_CL_OP        // no flag-bits
#define V0_WFE_OP               V0_EVENT_OP     // FLAG1-bit set
#define V0_SEV_OP               V0_EVENT_OP     // FLAG2-bit set, ID in bits
#define V0_LDX_OP               V0_XREG_OP      // FLAG1-bit set
#define V0_STX_OP               V0_XREG_OP      // FLAG2-bit set
#define V0_IPG_OP               V0_PAGE_OP      // FLAG1-bit clear
#define V0_PFC_OP               V0_CACHE_OP     // FLAG1-bit clear
#define V0_FLC_OP               V0_CACHE_OP     // FLAG1-bit set
#define V0_PLK_OP               V0_MEM_OP       // no flag-bits
#define V0_BRD_OP               V0_MEM_OP       // FLAG1-bit set
#define V0_BWR_OP               V0_MEM_OP       // FLAG2-bit set
#define V0_BAR_OP               V0_MEM_OP       // FLAG1-bit and FLAG2-bit set

/* ALU-unit */
#define V0_INV_OP               0x00
#define V0_OR_OP                0x01
#define V0_UNADD_OP             0x03
#define V0_ADDER_OP             0x04
#define V0_EXT_OP               0x05
#define V0_NOT_OP               V0_INV_OP       // FLAG1-bit clear
#define V0_NEG_OP               V0_INV_OP       // FLAG1-bit set
#define V0_AND_OP               0x02            // no flag-bits
#define V0_IOR_OP               V0_OR_OP        // no flag-bits
#define V0_XOR_OP               V0_OR_OP        // FLAG1-bit set
#define V0_INC_OP               V0_UNADD_OP     // FLAG1-bit clear
#define V0_DEC_OP               V0_UNADD_OP     // FLAG1-bit set
#define V0_ADD_OP               V0_ADDER_OP     // no flag bits
#define V0_ADC_OP               V0_ADDER_OP     // FLAG1-bit set
#define V0_SUB_OP               V0_ADDER_OP     // FLAG2-bit set
#define V0_SBB_OP               V0_ADDER_OP     // FLAG1- and FLAG2-bits set
#define V0_CMP_OP               V0_ADDER_OP     // VAL-bit set, value after op
#define V0_ZEX_OP               V0_EXT_OP       // FLAG1-bit clear
#define V0_SEX_OP               V0_EXT_OP       // FLAG1-bit set

/* MULTI-unit */
#define V0_MULTI_OP             0x00
#define V0_DIVIDE_OP            0x01
#define V0_MULSFT_OP            0x02
#define V0_RECIP_OP             0x03
#define V0_MUL_OP               V0_MULTI_OP
#define V0_MLH_OP               V0_MULTI_OP     // FLAG1-bit set
#define V0_DIV_OP               V0_DIVIDE_OP    // no flag-bits
#define V0_REM_OP               V0_DIVIDE_OP    // FLAG1-bit set
#define V0_MSL_OP               V0_MULSFT_OP     // FLAG1-bit clear
#define V0_MSR_OP               V0_MULSFT_OP     // FLAG1-bit set
#define V0_IRP_OP               V0_RECIP_OP     // FLAG1-bit clear

/* SHIFT-unit */
#define V0_SHIFT_OP             0x00
#define V0_SHADD_OP             0x01
#define V0_SHMASK_OP            0x02
#define V0_SHL_OP               V0_SHIFT_OP
#define V0_SHR_OP               V0_SHIFT_OP     // FLAG1-bit set
#define V0_SAR_OP               V0_SHIFT_OP     // FLAG1- and FLAG2-bits set
#define V0_ROL_OP               V0_SHIFT_OP     // VAL-bit set
#define V0_ROR_OP               V0_SHIFT_OP     // VAL- and FLAG1-bits set
#define V0_SLA_OP               V0_SHADD_OP     // FLAG1-bit clear
#define V0_SRA_OP               V0_SHADD_OP     // FLAG1-bit set
#define V0_SLM_OP               V0_SHMASK_OP    // FLAG1-bit clear
#define V0_SRM_OP               V0_SHMASK_OP    // FLAG1-bit set

/* BIT-unit */
#define V0_BCNT_OP              0x00            // bit-count
#define V0_BSWAP_OP             0x01            // byte-swap
#define V0_BCD_OP               0x02            // binary coded decimal
#define V0_CHK_OP               0x04            // cyclic-redundancy check
#define V0_HASH_OP              0x05            // hash routines
#define V0_CLZ_OP               V0_BCNT_OP      // FLAG1-bit clear
#define V0_HAM_OP               V0_BCNT_OP      // FLAG1-bit set
#define V0_PAR_OP               V0_BCNT_OP      // FLAG2-bit set
#define V0_SWP_OP               V0_BSWAP_OP     // 8/16-bit byteswap (FLAG1 0/1)
#define V0_BEC_OP               V0_BCD_OP       // BCD encode (FLAG1-bit 0)
#define V0_BDC_OP               V0_BCD_OP       // BCD decode (FLAG1-bit 1)
#define V0_CRC_OP               V0_CHK_OP       // 16/32-bit CRC (FLAG1-bit 0/1)
#define V0_ECC_OP               V0_CHK_OP       // FLAG1-bit set
#define V0_HSH_OP               V0_HASH_OP      // hash using tmhash32()
#define V0_HUN_OP               V0_HASH_OP      // FLAG1-bit set, unhash

/* ATOM-unit */
#define V0_LLSC_OP              0x00
#define V0_BITXCHG_OP           0x01
#define V0_FETCHADD_OP          0x02
#define V0_CAS_OP               0x03            // no bits set
#define V0_LDL_OP               V0_LLSC_OP      // FLAG1-bit clear
#define V0_STL_OP               V0_LLSC_OP      // FLAG2-bit set
#define V0_BTC_OP               V0_BITXCHG_OP   // FLAG1-bit clear
#define V0_BTS_OP               V0_BITXCHG_OP   // set if clear, old in CF
#define V0_XADD_OP              V0_FETCHADD_OP  // no flag-bits
#define V0_XINC_OP              V0_FETCHADD_OP  // FLAG1-bit set
#define V0_XDEC_OP              V0_FETCHADD_OP  // FLAG2-bit set

/*
 * V0_BRANCH_OPs have VAL-bit set in opcode and one of the following codes in
 * the val-field.
 */
#define V0_EQ_VAL               0x00
#define V0_ZF_VAL               V0_EQ_VAL
#define V0_BEQ_VAL              0x01
#define V0_BNE_VAL              0x02
#define V0_BNZ_VAL              V0_BNE_VAL
#define V0_BLT_VAL              0x03
#define V0_BLE_VAL              0x04
#define V0_BGT_VAL              0x05
#define V0_BGE_VAL              0x06
#define V0_BCF_VAL              0x07
#define V0_BNC_VAL              0x08
#define V0_BOF_VAL              0x09
#define V0_BNO_VAL              0x0a
#define V0_BSF_VAL              0x0b
#define V0_BNS_VAL              0x0c

/* FLOW-unit */
#define V0_JUMP_OP              0x00
#define V0_BRANCH_OP            0x01
#define V0_FRAME_OP             0x02
#define V0_CALL_OP              0x03
#define V0_RETURN_OP            0x04
#define V0_JMP_OP               V0_JMP_OP       // no flag-bits
#define V0_JMR_OP               V0_JMP_OP       // FLAG1-bit set
/* branch operations, see V0_EQ_VAL etc. above for the bits-value */
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
#define V0_AXS_OP               0x00
#define V0_LOAD_OP              0x01
#define V0_STORE_OP             0x02
#define V0_STACK_OP             0x03
#define V0_IO_CMD_OP            0x04
#define V0_IO_PORT_OP           0x05
#define V0_CKR_OP               V0_AXS_OP       // no flag-bits
#define V0_CKW_OP               V0_AXS_OP       // FLAG1-bit set
#define V0_CKX_OP               V0_AXS_OP       // FLAG2-bit set
#define V0_LDR_OP               V0_LOAD_OP      // no flag-bits
#define V0_LDN_OP               V0_LOAD_OP      // FLAG1-bit set
#define V0_LDC_OP               V0_LOAD_OP      // VAL-bit + condition bits
#define V0_STR_OP               V0_STORE_OP     // no flag-bits
#define V0_STN_OP               V0_STORE_OP     // FLAG1-bit set
#define V0_STC_OP               V0_STORE_OP     // VAL-bit + condition bits
#define V0_PSH_OP               V0_STACK_OP     // FLAG1-bit clear
#define V0_PSM_OP               V0_STACK_OP     // FLAG1-bit set
#define V0_POP_OP               V0_STACK_OP     // FLAG2-bit set
#define V0_POM_OP               V0_STACK_OP     // FLAG1- and FLAG2-bits set
#define V0_IOC_OP               V0_IO_CMD_OP    // VAL-bit + command in val
#define V0_IOP_OP               V0_IO_PORT_OP   // VAL-bit + command in val

#endif /* __V0_INST_H__ */

