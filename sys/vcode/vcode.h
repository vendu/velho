#ifndef __SYS_VCODE_H__
#define __SYS_VCODE_H__

#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <zero/trix.h>

/* INSTRUCTIONS */
#define V_NOP           0x00    // no operation done
/* logical operations */
#define V_NOT           0x01    // bitwise negation; 2's complement
#define V_AND           0x02    // bitwise AND
#define V_OR            0x03    // bitwise OR
#define V_XOR           0x04    // bitwise exclusive-OR
#define V_NAD           0x05    // NAND
#define V_SHL           0x10    // left shift
#define V_SHR           0x11    // logical right shift; fill with zero
#define V_SAR           0x12    // arithmetic right shift; fill with sign-bit
#define V_INC           0x20    // arg1++;
#define V_DEC           0x21    // arg1--;
#define V_ADD           0x22    // arg2 += arg1;
#define V_SUB           0x23    // arg2 -= arg1;
#define V_MUL           0x30    // arg2 *= arg1;
#define V_RPC           0x31    // compute 1/D so that X * 1/D replaces X/D
#define V_MOD           0x32    // arg2 %= arg1;
/* LOAD-STORE INSTRUCTIONS */
#define V_MOV           0x40    // move between registers or register and memory
#define V_PSH           0x41    // push item or items to stack
#define V_POP           0x42    // pop item or items from stack
#define V_POM           0x43    // pop register range
#define V_PSM           0x44    // push register range
/* BRANCH INSTRUCTIONS */
#define V_ULT           0x50    // unsigned arg1 < arg2
#define V_LT            0x51    // arg1 < arg2;
#define V_ULTE          0x52    // unsigned arg1 <= arg2
#define V_LTE           0x53    // arg1 <= arg2;
#define V_UGT           0x54    // unsigned arg1 > arg2
#define V_GT            0x55    // arg1 > arg2;
#define V_UGTE          0x56    // unsigned arg1 >= arg2
#define V_GTE           0x57    // arg1 >= arg2;
#define V_EQ            0x58    // arg1 == arg2;
#define V_NEQ           0x59    // arg1 != arg2;
/* MISCELLANEOUS INSTRUCTIONS */
#define V_SEL           0x60    // select
#define V_RAND          0x61    // arg2 = (c) ? arg1 : arg2;
/* FLOATING POINT INSTRUCTIONS */
#define V_FLOOR         0x70
#define V_CEIL          0x71
#define V_TRUNC         0x72
#define V_ITOB          0x73
#define V_ITOF          0x74
#define V_ITOD          0x75
#define V_BTOI          0x76
#define V_FTOI          0x77
#define V_DTOI          0x77
/* logarithms, roots and exponents */
#define V_LOG           0x80
#define V_SQRT          0x81
#define V_CBRT          0x82
#define V_EXP           0x83
/* trigonometry */
#define V_SIN           0x90
#define V_COS           0x91
#define V_TAN           0x92
#define V_ASIN          0x93
#define V_ACOS          0x94
#define V_ATAN          0x95
#define V_SINH          0x96
#define V_COSH          0x97
#define V_TANH          0x98

/* PROCESSOR CONTROL BITS */
#define V_INTR_BIT      (1 << 0)
#define V_ZERO_BIT      (1 << 1)
#define V_UNDERFLOW_BIT (1 << 2)
#define V_OVERFLOW_BIT  (1 << 3)
#define V_CARRY_BIT     (1 << 4)
#define V_BORROW_BIT    (1 << 5)
#define V_PARITY_BIT    (1 << 6)
#define V_ALIGN_BIT     (1 << 7)
#define V_FAIL_BIT      (1 << 8)

/* INSTRUCTION FLAGS */
#define V_BOOLEAN_BIT   (1 << 0)    // boolean
#define V_ADDRESS_BIT   (1 << 1)    // address (pointer value)
#define V_FLOAT_BIT     (1 << 2)    // floating-point
#define V_COMPLEX_BIT   (1 << 3)    // complex number
#define V_FIXED_BIT     (1 << 4)    // fixed point
#define V_SIMD_BIT      (1 << 5)    // SIMD vector-operation
#define V_DSP_BIT       (1 << 6)    // DSP-operation

/* QUALIFIER FLAGS */
#define V_LITERAL_BIT   (1 << 0)
#define V_CONST_BIT     (1 << 1)
#define V_AUTOMATIC_BIT (1 << 2)
#define V_REGISTER_BIT  (1 << 3)
#define V_STATIC_BIT    (1 << 4)
#define V_VOLATILE_BIT  (1 << 5)
#define V_IMMUTABLE_BIT (1 << 6)

/* SPECIAL TYPE FLAGS */
#define V_STRING_BIT    (1 << 0)
#define V_VECTOR_BIT    (1 << 1)    // [SIMD] vector
#define V_REGMAP_BIT    (1 << 2)

union varg {
    void                *ptr;
    char                *cptr;
    uintptr_t            uptr;
    intptr_t             iptr;
    ptrdiff_t            pdiff;
    unsigned long long   ull;
    long long            ll;
    unsigned long        ul;
    long                 l;
    double               d64;
    float                f32;
#if (V_WORDSIZE == 64)
    int64_t              i64;
    uint64_t             u64;
#endif
    int32_t              i32;
    uint32_t             u32;
    int16_t              i16;
    uint16_t             u16;
    int8_t               i8;
    uint8_t              u8;
    unsigned char        uc;
    signed char          c;
};

struct vregmap {
    uint8_t             bits[V_WORDSIZE];
};

struct vop {
    struct varg         arg1;
    struct varg         arg2;
    struct vregmap      rmap;
    unsigned long       flg;
#if (V_WORDBITS == 64)
    uint64_t            code;
#else
    uint32_t            code;
#endif
    uint16_t            cflg;   // machine control/status flags
    uint8_t             iflg;   // instruction flags
    uint8_t             qflg;   // qualifier flags
    uint8_t             tflg;   // type flags
    size_t              size;
};

#endif /* __SYS_VCODE_H__ */

