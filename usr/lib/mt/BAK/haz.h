/* REFERENCE
 * ---------
 * - http://ticki.github.io/blog/fearless-concurrency-with-hazard-pointers/
 */

#ifndef __MT_HAZ_H__
#define __MT_HAZ_H__

#include <stdint.h>
#include <zero/hash.h>
#include <mach/asm.h>

#define HAZ_NODE_TAB_ITEMS                                              \
    ((128 * sizeof(void *) - 2 * sizeof(m_atomic_t) - sizeof(struct hazqueue *)) / sizeof(m_atomicptr_t))
#define HAZ_NODE_HIWATER                                                \
    (HAZ_NODE_TAB_ITEMS / 2)

#define HAZ_TAB_ITEMS                                                   \
    ((32 * sizeof(void *) - sizeof(m_atomic_t) - sizeof(struct haztab *)) \
     / sizeof(void *))
#define HAZ_TAB_CHAINS 1024
#if (PTRSIZE == 4)
#define hazhashptr(ptr) (tmhash32((uint32_t)(ptr)) & (HAZ_TAB_CHAINS - 1))
#elif (PTRSIZE == 8)
#define hazhashptr(ptr) (tmhash64((uint64_t)(ptr)) & (HAZ_TAB_CHAINS - 1))
#endif

#define HAZ_FREE      0x00
#define HAZ_PROT      0x01
#define HAZ_BLOCK     0x02
#define HAZ_DEAD      0x03

struct haznode {
    m_atomic_t      lk;
    struct haznode *next;
    long            ndx;
    void           *tab[HAZ_NODE_TAB_ITEMS];
};

#endif /* __MT_HAZ_H__ */

