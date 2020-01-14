#ifndef __V0_V0_H__
#define __V0_V0_H__

#define OP_ARG8_MASK    0
#define OP_ARG16_MASK   OP_FLG1_BIT
#define OP_ARG32_MASK   OP_FLG2_BIT
#define OP_ARG64_MASK   (OP_FLG1_BIT | OP_FLG2_BIT)
#define OP_FLG1_BIT	(1 << 8)
#define OP_FLG2_BIT	(1 << 9)

#define OP_SYS_UNIT     0x0
#define OP_ALU_UNIT	0x1
#define OP_SFT_UNIT	0x2
#define OP_XFER_UNIT    0x3
#define OP_THR_UNIT	0x4
#define OP_FLOW_UNIT    0x5
#define OP_COP_UNIT     0x7

#define SYS_HLT_OP 	0x0 // HLT, RST, SIX
#define SYS_RST_OP	(SYS_HLT_OP | OP_FLG1_BIT)
#define SYS_SIX_OP	(SYS_HLT_OP | OP_FLG2_BIT)
#define SYS_DEV_OP	0x1 // BUS, DEV, 
#define SYS_IOP_OP      0x2 // SET, CLR
#define SYS_IOC_OP      0x3 // CTL, DMA, BUF

#define ALU_NOT_OP	0x0
#define ALU_NEG_OP      (ALU_NOT_OP | OP_FLG1_BIT
#define ALU_BIT_OP      0x1
#define ALU_AND_OP	ALU_BIT_OP
#define ALU_IOR_OP      (ALU_BIT_OP | OP_FLG1_BIT)
#define ALU_XOR_OP	(ALU_BIT_OP | OP_FLG2_BIT)
#define ALU_ADD_OP      0x2
#define ALU_ADC_OP      (ALU_ADD_OP | OP_FLG1_BIT)
#define ALU_SUB_OP      0x3
#define ALU_SBB_OP      (ALU_SUB_OP | OP_FLG1_BIT)
#define ALU_CMP_OP      (ALU_SUB_OP | OP_FLG2_BIT)
#define ALU_EXT_OP      0x04 // ZEX, SEX
#define ALU_CNT_OP      0x05 // PAR, CLZ, HAM

#define SFT_SHIFT_OP    0x0
#define SFT_ROT_OP	0x1
#define SFT_SHA_OP 	0x2	// shift-and-mask       
#define SFT_SHM_OP	0x3	// shift-and-multiply
#define SFT_SHL_OP      SFT_SHIFT_OP
#define SFT_SHR_OP      (SFT_SHIFT_OP | OP_FLG1_BIT)
#define SFT_SAR_OP      (SFT_SHIFT_OP | OP_FLG2_BIT)    // shift-and-add
#define SFT_ROL_OP      SFT_ROT_OP
#define SFT_ROR_OP	(SFT_ROT_OP | OP_FLG1_BIT)
#define SFT_RCL_OP	(SFT_ROT_OP | OP_FLG2_BIT
#define SFT_RCR_OP	(SFT_ROT_OP | OP_FLG1_BIT | OP_FLG2_BIT)
#define SFT_MSL_OP	SFT_SHA_0P
#define SFT_MSR_OP	(SFT_SHA_OP | OP_FLG1_BIT)
#define SFT_SML_OP      SFT_SHM_OP
#define SFT_SMR_OP      (SFT_SHM_OP | OP_FLG1_BIT)
#define SFT_SAD_OP	0x4
#define SFT_SRA_OP      (SFT_SAD_OP | OP_FLG1_BIT)
#define SFT_SLA_OP      (SFT_SAD_OP | OP_FLG2_BIT)

#define /*  __V0_V0_H__ */

