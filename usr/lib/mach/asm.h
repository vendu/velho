#ifndef __MACH_ASM_H__
#define __MACH_ASM_H__

#include <mach/atomic.h>

/* for systems without GNU C, we define these in e.g. <mach/ia32/asm.h> */
#if defined(__GNUC__)
/* stack frames */
#define m_atomread(a)      (*((typeof(a))(a)))
#define m_atomwrite(a, v)  (*((typeof(v) *)(a)) = (v))
#define m_syncread(a, res)                                              \
    do {                                                                \
        m_memwrbar();                                                   \
        (res) = m_atomread(a);                                          \
    } while (0)
#define m_syncwrite(a, val)                                             \
    do {                                                                \
        m_memwrbar();                                                   \
        m_atomwrite(a, val);                                            \
    } while (0)
#endif

#include <mach/atomic.h>
#if defined(_WIN64)
#include <zero/msc/win64.h>
#elif defined(_WIN32)
#include <zero/msc/win32.h>
#elif (defined(__x86_64__) || defined(__amd64__))
#include <mach/x86/asm.h>
#include <mach/x86-64/asm.h>
#elif (defined(__i386__) || defined(__i486__)                           \
       || defined(__i586__) || defined(__i686__))
#include <mach/x86/asm.h>
#include <mach/ia32/asm.h>
#elif defined(__arm__)
#include <mach/arm/asm.h>
#elif defined(__ppc__)
#include <mach/ppc/asm.h>
#endif

#define m_trylkbit(p, ndx) (!m_cmpsetbit(p, ndx))
#define m_unlkbit(p, ndx)  m_clrbit(p, ndx)
#define m_lkbit(p, ndx)                                                 \
    do {                                                                \
        long _pos = 1 << (ndx);                                         \
        long _res = 0;                                                  \
                                                                        \
        do {                                                            \
            while ((uintptr_t)(p) & _pos) {                             \
                m_waitspin();                                           \
            }                                                           \
            _res = m_cmpsetbit((p), ndx);                               \
        } while (!_res);                                                \
    } while (0)

#endif /* __MACH_ASM_H__ */

