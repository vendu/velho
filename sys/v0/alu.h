#ifndef __V0_ALU_H__
#define __V0_ALU_H__

/* NOTES
 * -----
 * ZF together with non-zero result indicates unsigned operation
 */
#define V0_FLAG_ZF   (1L << 0)  // zero/clear/on/off
#define V0_FLAG_CB   (1L << 2)  // carry/borrow
#define V0_FLAG_VF   (1L << 3)  // underflow/overflow
#define V0_FLAG_XF   (1L << 3)  // auxiliary/second parcel/immediate

#endif /* __V0_ALU_H__ */

