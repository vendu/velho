#ifndef __VC_MATH_H__
#define __VC_MATH_H__

/* FIXME: hack this for non-little endian systems */

#include <stdint.h>
#include <endian.h>

/*
 * IEEE 32-bit
 * 0..22  - mantissa
 * 23..30 - exponent
 * 31     - sign
 */
union _ieee754f { uint32_t u32; float f; };

#define f_fgetmant(f)       (((union _ieee754f *)&(f))->u32 & 0x007fffff)
#define f_fgetexp(f)        ((((union _ieee754f *)&(f))->u32 & 0x7ff00000) >> 23)
#define f_fgetsign(f)       (((union _ieee754f *)&(f))->u32 & 0x80000000)
#define f_fsetmant(f, mant) (((union _ieee754f *)&(f))->u32 |= (mant) & 0x7fffff)
#define f_fsetexp(f, exp)   (((union _ieee754f *)&(f))->u32 |= (exp) << 23)
#define f_fsetsign(f, sign)                                             \
    ((sign)                                                             \
     ? (((union _ieee754f *)&(f))->u32 |= 0x80000000)                   \
     : (((union _ieee754f *)&(f))->u32 &= 0x7fffffff))
#define f_fsetnan(f)                                                    \
    (*(uint32_t *)&(f) = 0x7fffffffU)
#define f_fsetsnan(f)                                                   \
    (*(uint32_t *)&(f) = 0xffffffffU)

/*
 * IEEE 64-bit
 * 0..51  - mantissa
 * 52..62 - exponent
 * 63     - sign
 */
    union _ieee754d { uint64_t u64; double d; };

#define f_dgetmant(d)       (((union _ieee754d *)(&(d)))->u64 & UINT64_C(0x000fffffffffffff))
#define f_dgetexp(d)        ((((union _ieee754d *)&(d))->u64 & UINT64_C(0x7ff0000000000000)) >> 52)
#define f_dgetsign(d)       (((union _ieee754d *)(&(d)))->u64 & UINT64_C(0x8000000000000000))
#define f_dsetmant(d, mant) (((union _ieee754d *)(&(d)))->u64 |= (mant))
#define f_dsetexp(d, exp)   (((union _ieee754d *)(&(d)))->u64 |= (uint64_t)(exp) << 52)
#define f_dsetsign(d, sign)                                             \
    ((sign)                                                             \
     ? (((union _ieee754d *)(&(d)))->u64 |= UINT64_C(0x8000000000000000)) \
     : (((union _ieee754d *)(&(d)))->u64 &= UINT64_C(0x7fffffffffffffff)))
#define f_dsetnan(d)                                                    \
    (*(uint64_t *)(&(d)) = UINT64_C(0x7fffffffffffffff))
#define f_dsetsnan(d)                                                   \
    (*(uint64_t *)(&(d)) = UINT64_C(0xffffffffffffffff))

/*
 * IEEE 80-bit
 * 0..63  - mantissa
 * 64..78 - exponent
 * 79     - sign
 */
#define f_ldgetmant(ld)       (*((uint64_t *)&(ld)))
#define f_ldgetexp(ld)        (*((uint32_t *)&(ld) + 2) & 0x7fff)
#define f_ldgetsign(ld)       (*((uint32_t *)&(ld) + 3) & 0x8000)
#define f_ldsetmant(ld, mant) (*((uint64_t *)&(ld)) = (mant))
#define f_ldsetexp(ld, exp)   (*((uint32_t *)&(ld) + 2) |= (exp) & 0x7fff)
#define f_ldsetsign(ld, sign)                                           \
    ((sign)                                                             \
     ? (*((uint32_t *)&ld + 3) |= 0x8000)                               \
     : (*((uint32_t *)&ld + 3) &= 0x7fff))
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define f_ldsetnan(ld)                                                  \
        do {                                                            \
            uint32_t *_u32p = (uint32_t *)&(ld);                        \
                                                                        \
            _u32p[0] = 0xffffffffU;                                     \
            _u32p[1] = 0xffffffffU;                                     \
            _u32p[2] = 0x7fffU;                                         \
        } while (0)
#define f_ldsetsnan(ld)                                                 \
    do {                                                                \
        uint32_t *_u32p = (uint32_t *)&(ld);                            \
                                                                        \
        _u32p[0] = 0xffffffffU;                                         \
        _u32p[1] = 0xffffffffU;                                         \
        _u32p[2] = 0xffffU;                                             \
    } while (0)
#elif (__BYTE_ORDER == __LITTLE_ENDIAN)
#define f_ldsetnan(ld)                                                  \
    do {                                                                \
        uint32_t *_u32p = (uint32_t *)&(ld);                            \
                                                                        \
        _u32p[0] = 0x7fffU;                                             \
        _u32p[1] = 0xffffffffU;                                         \
        _u32p[2] = 0xffffffffU;                                         \
    } while (0)
#define f_ldsetsnan(ld)                                                 \
    do {                                                                \
        uint32_t *_u32p = (uint32_t *)&(ld);                            \
                                                                        \
        _u32p[0] = 0xffffU;                                             \
        _u32p[1] = 0xffffffffU;                                         \
        _u32p[2] = 0xffffffffU;                                         \
    } while (0)
#endif

/* internal macros. */
#define _ftoi32(f)     (*((int32_t *)&(f)))
#define _ftou32(f)     (*((uint32_t *)&(f)))
#define _dtoi64(d)     (*((int64_t *)&(d)))
#define _dtou64(d)     (*((uint64_t *)&(d)))
/* FIXME: little-endian. */
#define _dtohi32(d)    (*(((uint32_t *)&(d)) + 1))
/* sign bit 0x8000000000000000. */
#define ifabs(d)                                                        \
    (_dtou64(d) & UINT64_C(0x7fffffffffffffff))
#define fabs2(d, t64)                                                   \
    (*((uint64_t *)&(t64)) = ifabs(d))
/* sign bit 0x80000000. */
#define ifabsf(f)                                                       \
    (_ftou32(f) & 0x7fffffff)

/*
 * TODO: IEEE 128-bit
 * - 0..112   - mantissa
 * - 113..126 - exponent
 * - 127      - sign
 */

#if defined(HUGE_VAL_F128)

#define f_f128getexp(f128)      (((uint16_t *)(f128))[7] & 0x7fff)
#define f_f128getsign(f128)     (((int32_t *)(f128))[3] & INT32_C(0x80000000))
#define f_f128setexp(f128, exp) (((uint16_t *)(f128))[7] = (exp) & 0x7fff)
#define f_f128setsign(f128, sign)                                       \
    ((sign)                                                             \
     ? (((uint16_t *)(f128))[7] |= 0x8000)                              \
     : (((uint16_t *)(f129))[7] &= 0x7fff))
#endif /* 128-bit float */

int f_fpclassify(float x);
int f_isfinite(float x);
int f_isinf(float x);
int f_isnan(float x);
int f_isnormal(float x);
int f_signbit(float x);
int f_isgreater(float x, float y);
int f_isgreaterequal(float x, float y);
int f_isless(float x, float y);
int f_islessequal(float x, float y);
int f_islessgreater(float x, float y);
int f_isunordered(float x, float y);
int d_fpclassify(float x);
int d_isfinite(double x);
int d_isinf(double x);
int d_isnan(double x);
int d_isnormal(double x);
int d_signbit(double x);
int d_isgreater(double x, double y);
int d_isgreaterequal(double x, double y);
int d_isless(double x, double y);
int d_islessequal(double x, double y);
int d_islessgreater(double x, double y);
int d_isunordered(double x, double y);

#endif /* __VC_MATH_H__ */
