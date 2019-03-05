/*
 * This file aims to develop a set of 32-bit mathematics routines to be
 * implemented in [FPGA] hardware.
 */

#include <v0/vm/types.h>

/*
 * Object Names
 * ------------
 * - sig        synchronisation and such signals
 * - src32      32-bit source argument
 * - dest32     32-bit destination argument
 * - out32      32-bit output word
 */
/* signal-bits */
#define V0_MRDLK_SIG 0x01       // memory is read-locked when 1
#define V0_MWRLK_SIG 0x02       // memory is write-locked when 1
#define V0_MEMLK_SIG 0x03       // memory is read-write-locked when 1
#define V0_BUSLK_SIG 0x40       // memory and data buses are locked when 1
#define V0_RESET_SIG 0x80       // reset is signaled by 1
/*
 * Units
 * -----
 * mem
 *- dec                 address decoder/calculations
 *  - lea(r, p, ofs, n) register r = &p[ofs << n]; // p + (ofs << n)
 *    - LEA %p, %r      %r = %p + (direct ofs << parm n)
 *                      (i->arg + (i)->reg1 << i->parm)
 *    - shl             shifter for scaling offset (in bytes)
 *    - add             unsigned addition, wrap-around on overflow
 * - arg1(r, op, nb)    load second [register or memory] operand of 8 * n bits
 *                      into register r
 * - arg2(r, op, nb)    store destination [register or memory] operand of 8 * n
 *                      bits into register r
 * - ldr(r, rim)        load register, immediate, or memory operand into r
 * - str(r, rm)         store r into another register or memory at address m
 *
 * ARITHMETIC-LOGICAL PIPELINE
 * ---------------------------
 * - clz                count leading 0-bits
 * - add                addition
 * - crp                compute inverse/reciprocal
 * - mul                multiplication
 * - neg                arithmetic negation
 * - not                2's complement
 * - and                logical AND
 * - xor                logical exclusive OR
 * - lor                logical OR
 */
/*
 * Buses
 * -----
 * mem          access and synchronisation of cache and random-access memory
 * map          access and control of memory-mapped devices
 */

/*
 * V0 Operations
 * -------------
 *
 * processor operations
 * --------------------
 * lkrw(m)              write-lock register
 * lkrd(m)              read-lock register
 * cas(m, a, b)         swap b with a in address m if a is the specified value
 * cas2(m, p1, p2)      swap *p1 with *p2 if *p1 == *p2
 *
 * memory/cache operations
 * -----------------------
 * mrdbar()     memory read-barrier
 * mwrbar()     memory write-barrier
 * mbar()       full memory barrier
 * let(o, a)    o <- a, synchronised
 *              - set cacheline dirty-bit if memory
 *              - release write-lock on registers
 * peek(m, n)   fetch bytes from memory address m
 * poke(m, n)   stores bytes to memory address m; set cacheline dirty-bit
 * - n = 0 -> 8-bit, 1 -> 16-bit, 3 -> 32-bit, ... 7 - 256-bit access
 * ldc(m)       load cacheline; clear dirty-bit
 * rdc(m)       read word from memory address m; set cacheline dirty-bit
 * stc(m, w)    store word if cacheline not dirty
 * stc2(m, d)   store doubleword if cacheline not dirty
 * mlk()        lock memory- and data-buses for atomic access
 * mrel()       unlock memory- and data-buses
 * msyn(m)      synchronize writeback cacheline
 *
 * arithmetic and logical operations
 * --------------------------------
 * neg(a)    (-(a))       arithmetic negation
 * sar(a, n) ((a) >> (n)) arithmetic shift by n bits (fill-with-sign)
 * clz(a)    evaluates to leading-zero count
 * ham(a)    evaluates to hamming weight
 * mul(a, b) (b64 = (a) * (b), and((b64), V0_CONST_ALL_32))
 * muh(a, b) (b64 = (a) * (b), shr((b64), 32))
 * crp(a)    reciprocal R so that B/A <=> B*R
 * div(a, b) ((b) * crp(a))
 *
 * not(a)    (~(a))
 * and(a, b) ((a) & (b))  AND
 * or(a, b)  ((a) | (b))  OR
 * xor(a, b) ((a) ^ (b))  XOR
 *
 * shr(a, n) ((a) >> (n)) logical shift for unsigned operand
 * shl(a, n) ((a) << (n)) logical shift for any operand
 *
 * inc(a)    (++(a))      increment by one
 * dec(a)    (--(a))      decrement by one
 *
 * add(a, b) ((a) + (b))  addition
 * sub(a, b) ((a) - (b))  subtraction
 *
 * sex(a, b) b <= a sign-extended
 */

#define isneg(i)    (-(i))
#define iseq(a, b)  (!((b) - (a)))
#define islt(a, b)  ((a) < (b))
#define islte(a, b) ((a) <= (b))
#define isgt(a, b)  ((a) > (b))
#define isgte(a, b) ((a) >= (b))
#define not(i)      (~(i))

/* REFERENCE: https://bisqwit.iki.fi/story/howto/bitmath/ */

#define v0addop(src32, dest32, out32) v0addop1(src32, dest32, out32)

// out32 = _tmp32 + 1
// OPS: add
#define v0incop1(src32, out32)                                          \
    do {                                                                \
        v0ureg _tmp32 = (src32);                                        \
        v0ureg _res32 = _tmp32 + V0_CONST_ONE;                          \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// out32 = _tmp32 - V0_CONST_ALL_32 (-1)
// OPS: sub
#define v0incop2(src32, out32)                                          \
    do {                                                                \
        v0ureg _tmp32 = (src32);                                        \
        v0ureg _res32 = _tmp32 - V0_CONST_ALL_32;                       \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// out32 = _tmp32 + V0_CONST_ALL_32 (-1)
// OPS: add
#define v0decop1(src32, out32)                                          \
    do {                                                                \
        v0ureg _tmp32 = (src32);                                        \
        v0ureg _res32 = _tmp32 + V0_CONST_ALL_32;                       \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

/* TODO: v0cmpop() */

// out32 = _tmp32 - 1
// OPS: sub
#define v0decop2(src32, out32)                                          \
    do {                                                                \
        v0ureg _tmp32 = (src32);                                        \
        v0ureg _res32 = _tmp32 - V0_CONST_ONE;                          \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

#define v0addop1(src32, dest32, out32)                                  \
    do {                                                                \
        v0ureg _tmp32 = (src32);                                        \
        v0ureg _res32 = (dest32) + _tmp32;                              \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// out32 -= -src32, ignore carry-bit
// OPS: neg, sub
#define v0addop2(src32, dest32, out32)                                  \
    do {                                                                \
        v0ureg _tmp32 = neg(src32);                                     \
        v0ureg _res32 = (dest32) - _tmp32;                              \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// out32 += src32, set carry-bit
// OPS: add, lt
#define v0adcop1(src32, dest32, out32, cf)                              \
    do {                                                                \
        v0ureg _tmp32 = (src32);                                        \
        v0ureg _res32 = (dest32) + _tmp32;                              \
                                                                        \
        if (_res32 < _tmp32) {                                          \
            _cf |= V0_MSW_CF_BIT;                                       \
        }                                                               \
        setcf(_cf);                                                     \
        (out32) = _res32;                                               \
    } while (0)

        // out32 -= -src32, set carry-bit
// OPS: sub, lt
#define v0adcop2(src32, dest32, out32, cf)                              \
    do {                                                                \
        v0ureg _tmp32 = neg(src32);                                     \
        v0ureg _res32 = (dest32) - _tmp32;                              \
                                                                        \
        if (_res32 < _tmp32) {                                          \
            _cf |= V0_MSW_CF_BIT;                                       \
        }                                                               \
        setcf(_cf);                                                     \
        (out32) = _res32;                                               \
    } while (0)

        // out32 += -src32, ignore carry-bit
// OPS: neg, add
#define v0subop1(src32, dest32, out32)                                  \
    do {                                                                \
        v0ureg _tmp32 = neg(src32);                                     \
        v0ureg _res32 = (dest32) + _tmp32;                              \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// out32 -= src32, ignore carry-bit
// OPS: sub
#define v0subop2(src32, dest32, out32)                                  \
    do {                                                                \
        v0ureg _tmp32 = (src32);                                        \
        v0ureg _res32 = (dest32) - _tmp32;                              \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// out32 += -src32, set carry-bit
// OPS: neg, add, lt
#define v0sbcop1(src32, dest32, out32, cf)                              \
    do {                                                                \
        v0ureg _tmp32 = neg(src32);                                     \
        v0ureg _res32 = (dest32) + _tmp32;                              \
                                                                        \
        if (_res32 < _tmp32) {                                          \
            (cf) |= V0_MSW_CF_BIT;                                      \
        }                                                               \
        (out32) = _res32;                                               \
    } while (0)

// out32 -= src32, set carry-bit
// OPS: sub, lt
#define v0sbcop2(src32, dest32, out32, cf)                              \
    do {                                                                \
        v0ureg _tmp32 = (src32);                                        \
        v0ureg _res32 = (dest32) - _tmp32;                              \
                                                                        \
        if (_res32 < _tmp32) {                                          \
            (cf) |= V0_MSW_CF_BIT;                                      \
        }                                                               \
        (out32) = _res32;                                               \
    } while (0)

// out32 = ~dest32 + 1, arithmetic negation
// OPS: not, inc
#define v0negop1(src32, out32)                                          \
    do {                                                                \
        v0reg _tmp32 = not(src32);                                      \
                                                                        \
        _tmp32++;                                                       \
        (out32) = _tmp32;                                               \
    } while (0)
// out32 = 0 - src32
// OPS: sub
#define v0negop2(src32, out32)                                          \
    do {                                                                \
        v0reg _zero32 = V0_v0rdzero();                                  \
        v0reg _res32 = _zero32 - (src32);                               \
                                                                        \
        (out32) = (_res32);                                             \
    } while (0)

// out32 = V0_CONST_ALL_32 ^ (src32), logical negation
// OPS: xor
#define v0notop(src32, out32)                                           \
    do {                                                                \
        v0reg _res32 = (src32) ^ V0_CONST_ALL_32;                       \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// out32 = V0_CONST_ALL_32 - (src32), logical negation
// OPS: sub
#define v0notop2(src32, out32)                                          \
    do {                                                                \
        v0reg _tmp32 = V0_CONST_ALL_32;                                 \
        v0reg _res32 = _tmp32 - (src32);                                \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// out32 = -(src32) - 1, logical negation
#define v0notop3(src32, out32)                                          \
    do {                                                                \
        v0reg _tmp32 = neg(src32);                                      \
                                                                        \
        _tmp32--;                                                       \
        (out32) = _tmp32;                                               \
    } while (0)

// a & b = ~(~a | ~b);
#define v0andop(src32, dest32, out32)                                   \
    do {                                                                \
        v0reg _tmp32a = not(src32);                                     \
        v0reg _tmp32b = not(dest32);                                    \
        v0reg _val32 = _tmp32a | _tmp32b;                               \
        v0reg _res32 = not(_val32);                                     \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// a | b = not(~a & ~b)
#define v0xorop(src32, dest32, out32)                                   \
    do {                                                                \
        v0reg _tmp32a = not(src32);                                     \
        v0reg _tmp32b = not(dest32);                                    \
        v0reg _res32 = _tmp32a & _tmp32b;                               \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// a | b = ~(~a & ~b)
#define v0lorop(src32, dest32, out32)                                   \
    do {                                                                \
        v0reg _tmp32a = not(src32);                                     \
        v0reg _tmp32b = not(dest32);                                    \
        v0reg _val32 = _tmp32a & _tmp32b;                               \
        v0reg _res32 = not(_val32);                                     \
                                                                        \
        (out32) = _res32;                                               \
    } while (0)

// count leading zero bits in src32
#define v0clzop(src32, out32)                                           \
    do {                                                                \
        uint32_t _res32 = 0;                                            \
        uint32_t _tmp32 = (src32);                                      \
        uint32_t _mask32 = V0_CONST_HIGH_16;                            \
                                                                        \
        if (_tmp32) {                                                   \
            if (!(_tmp32 & _mask32)) {                                  \
                _res32 += 16;                                           \
                _tmp32 <<= 16;                                          \
            }                                                           \
            _mask32 <<= 8;                                              \
            if (!(_tmp32 & _mask32)) {                                  \
                _res32 += 8;                                            \
                _tmp32 <<= 8;                                           \
            }                                                           \
            _mask32 <<= 4;                                              \
            if (!(_tmp32 & _mask32)) {                                  \
                _res32 += 4;                                            \
                _tmp32 <<= 4;                                           \
            }                                                           \
            _mask32 <<= 2;                                              \
            if (!(_tmp32 & _mask32)) {                                  \
                _res32 += 2;                                            \
                _tmp32 <<= 2;                                           \
            }                                                           \
            _mask32 <<= 1;                                              \
            if (!(_tmp32 & _mask32)) {                                  \
                _res32++;                                               \
            }                                                           \
        } else {                                                        \
            _res32 = 32;                                                \
        }                                                               \
        out32 = _res32;                                                 \
    } while (0)

/* compute the Hamming weight, i.e. the number of 1-bits in a */

/* #L1: each 2-bit chunk sums 2 bits */
/* #L2-3: each 4-bit chunk sums 4 bits */
/* #L4: each 8-bit chunk sums 8 bits */
/* #L5: each 16-bit chunk sums 16 bits */
#define _v0hamop1(src32, out32)                                         \
    do {                                                                \
        v0reg _res32 = (src32);                                         \
                                                                        \
        (_res32) = (((_res32) >> 1) & V0_CONST_HAM_55) + ((_res32) & V0_CONST_HAM_55); \
        (_res32) = (((_res32) >> 2) & V0_CONST_HAM_33) + ((_res32) & V0_CONST_HAM_33); \
        (_res32) = (((_res32) >> 4) & V0_CONST_HAM_0F) + ((_res32) & V0_CONST_HAM_0F); \
        (_res32) = (((_res32) >> 8) & V0_CONST_HAM_00FF) + ((_res32) & V0_CONST_HAM_00FF); \
        (out32) = ((_res32) >> 16) + ((_res32) & V0_CONST_LOW_16);      \
    } while (0)

#define v0hamop(src32, dest32, out32)                                   \
    do {                                                                \
        v0reg _tmp32 = (dest32);                                        \
        v0reg _res32;                                                   \
                                                                        \
        _v0hamop1(_tmp32, _res32);                                      \
        (out32) = _tmp32;                                               \
    } while (0)

// sign-extend src32
#define v0sexop(src32, out32, sft)                                      \
    do {                                                                \
        v0reg _tmp32 = (src32);                                         \
        v0reg _sign = (8 << (sft)) - 1;                                 \
        v0reg _sign32 = 0;                                              \
        v0reg _clz32;                                                   \
        v0reg _cnt32;                                                   \
                                                                        \
        if (_sign) {                                                    \
            _sign32 = V0_CONST_SIGN;                                    \
        }                                                               \
        v0clzop(src32, (out32), _clz32);                                \
        if (_clz32) {                                                   \
            _sign32--;                                                  \
            _cnt32 -= 32 - clz;                                         \
            _sign32 <<= _cnt32;                                         \
            _tmp32 |= _sign32;                                          \
        }                                                               \
        (out32) = _tmp32;                                               \
    } while (0)

