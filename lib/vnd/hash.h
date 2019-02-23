#ifndef __VND_HASH_H__
#define __VND_HASH_H__

#include <stdint.h>
#include <zero/cdefs.h>

CONST uint32_t  tmhash32(uint32_t u);
CONST uint32_t  tmunhash32(uint32_t u);
CONST uint32_t  tmhash64(uint32_t u);
CONST uint32_t  tmunhash64(uint32_t u);

#endif /* __VND_HASH_H__ */

