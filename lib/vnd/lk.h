#ifndef __VND_LK_H__
#define __VND_LK_H__

#include <mach/asm.h>

#define vndlkbit(adr, ofs)                                              \
    do {                                                                \
        while (*(m_atomic_t *)(adr) & ((m_atomic_t)1L << (ofs))) {      \
            m_waitspin();                                               \
        }                                                               \
        if (!m_cmpsetbitl(((adr), (ofs)))) {                            \
                                                                        \
            return;                                                     \
        }                                                               \
    } while (1)

#define vndunlkbit(adr, ofs)                                            \
    m_clrbitl((adr), (ofs))

#endif /* __VND_LK_H__ */

