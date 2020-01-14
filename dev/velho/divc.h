#ifndef __VELHO_DIVC_H__
#define __VELHO_DIVC_H__

#include <stdint.h>

/* these routines were generated with https://sourceforge.net/projects/kdiv/ */

#define DIVCU10_MAGIC_U32	UINT32_C(0xcccccccd)
#define DIVCU60_MAGIC_U32	UINT32_C(0x88888889)
#define DIVCU100_MAGIC_U32	UINT32_C(0x51eb851f)
#define DIVCU400_MAGIC_U32	UINT32_C(0x51eb851f)
#define DIVCU1000_MAGIC_U32	UINT32_C(0x10624dd3)

/* TESTED OK */
static INLINE CONST uint32_t
divcu10(uint32_t num)
{
    uint32_t    q;
    uint64_t    m64 = DIVCU10_MAGIC_U32;
    uint64_t    tmp = num;

    tmp *= m64;
    q = tmp >> 32;
    q = q >> 3;

    return q;
}

/* TESTED OK */
static INLINE CONST uint32_t
divcu60(uint32_t num)
{
    uint32_t    q;
    uint64_t    m64 = DIVCU60_MAGIC_U32;
    uint64_t    tmp = num;

    tmp *= m64;
    q = tmp >> 32;
    q = q >> 5;

    return q;
}

/* TESTED OK */
static INLINE CONST uint32_t
modcu60(uint32_t num)
{
    uint32_t    q;
    uint64_t    m64 = DIVCU60_MAGIC_U32;
    uint64_t    tmp = num;

    tmp *= m64;
    q = tmp >> 32;
    q = q >> 5;
    q *= 60;
    num -= q;

    return num;
}

/* TESTED OK */
static INLINE CONST uint32_t
divcu100(uint32_t num)
{
    uint32_t    q;
    uint64_t    m64 = DIVCU100_MAGIC_U32;
    uint64_t    tmp = num;

    tmp *= m64;
    q = tmp >> 32;
    q = q >> 5;

    return q;
}

/* TESTED OK */
static INLINE CONST uint32_t
modcu400(uint32_t num)
{
    uint32_t    q;
    uint64_t    m64 = DIVCU400_MAGIC_U32;
    uint64_t    tmp = num;

    tmp *= m64;
    q = tmp >> 32;
    q = q >> 7;
    q *= 400;
    num -= q;

    return num;
}

/* TESTED OK */
static INLINE CONST uint32_t
divcu1000(uint32_t num)
{
    uint32_t    q;
    uint64_t    m64 = DIVCU1000_MAGIC_U32;
    uint64_t    tmp = num;

    tmp *= m64;
    q = tmp >> 32;
    q = q >> 6;

    return q;
}

#endif /* __VELHO_DIVC_H__ */

