#ifndef __SYS_V0_OP_H__
#define __SYS_V0_OP_H__

#include <stdint.h>
#include <v0/v0.h>

#define sex(x, max)     (((x) ^ (max)) - (max))
#define sexb(b)         (((b) ^ 128) - 128)
#define sexh(b)         (((b) ^ 65536) - 128)

#define haszerob(u)     (((u) - 0x01010101U) & ~(u) & 0x80808080U)

#endif /* __SYS_V0_OP_H__ */

