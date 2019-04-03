#ifndef __V0_ALU_H__
#define __V0_ALU_H__

#define sex(x, max)     (((x) ^ (max)) - (max))
#define sexb(b)         (((b) ^ 128) - 128)
#define sexh(b)         (((b) ^ 65536) - 128)

#endif /* __V0_ALU_H__ */

