#ifndef __SYS_ZEN_MEM_H__
#define __SYS_ZEN_MEM_H__

#if defined(__v0__)
#include <zen/bsp/v0.h>
#endif

struct membuf {
    m_atomic_t  nref;
    int8_t     *ptr;
    size_t      size;
    m_word_t    flg;
};

#endif /* __SYS_ZEN_MEM_H__ */

