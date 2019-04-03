#ifndef __V0_MACROS_H__
#define __V0_MACROS_H__

/*
 * Special Constant Registers
 * --------------------------
 */
#define V0_CONST_ZERO     	0x00000000 // constant zero
#define V0_CONST_ONE      	0x00000001 // constant one
#define V0_CONST_HIGH_4   	0xf0000000 // high 4-bit mask of 1-bits (clz)
#define V0_CONST_LOW_8          0x000000ff // low 8-bit mask of 1-bits
#define V0_CONST_HIGH_8   	0xff000000 // high 8-bit mask of 1-bits (clz)
#define V0_CONST_LOW_16   	0x0000ffff // low 16-bit mask of 1-bits
#define V0_CONST_HIGH_16  	0xffff0000 // high 16-bit mask of 1-bits (clz)
#define V0_CONST_LOW_24   	0x00ffffff // low 24-bit mask of 1-bits
#define V0_CONST_ALL_32   	0xffffffff // 32-bit mask of 1-bits (mul/muh)
#define V0_CONST_CTZ            0x077cb531 // constant for CTZ
#define V0_CONST_HAM_33   	0x33333333 // mask for ham
#define V0_CONST_HAM_55   	0x55555555 // mask for ham
#define V0_CONST_HAM_0F   	0x0f0f0f0f // mask for ham
#define V0_CONST_HAM_00FF 	0x00ff00ff // mask for ham
)
#define V0_CONST_SIGN_32  	0x80000000 // sign-bit

#endif /* __V0_MACROS_H__ */
