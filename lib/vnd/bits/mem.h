#ifndef __VND_BITS_MEM_H__
#define __VND_BITS_MEM_H__

#define MEM_HASH_SLOTS          16384
#define TABHASH_INVALID         { 0, 0 }
#define TABHASH_TAB_T           struct tabhashtab
#define TABHASH_ITEM_T          struct memhash
#define TABHASH_ITEM_WORDS      2
#define TABHASH_SLOTS           MEM_HASH_SLOTS
#define TABHASH_KEY(item)       ((item)->page)
#if (WORDSIZE == 8)
#define TABHASH_HASH(item)      tmhash64((item)->page)
#else
#define TABHASH_HASH(item)      tmhash32((item)->page
#endif
#define TABHASH_CMP(item, key)  ((item)->page == key)
#include <vnd/hash.h>
#include <vnd/tabhash.h>

#endif /* __VND_BITS_MEM_H__ */

