#define not(dest)        (dest = ~dest)
#define and(src, dest)   (dest &= src)
#define ior(src, dest)   (dest |= src)
#define xor(src, dest)   (dest ^= src)
#define shl(cnt, dest)   (dest <<= cnt)
#define shr(cnt, dest)   (dest >>= cnt)  // guaranteed to zero-fill unsigned
#define sar(cnt, dest)   (dest >>>= cnt) // arithmetic, i.e. sign-fill
#define inc(dest)        (dest++)        // increment by one
#define dec(dest)        (dest--)        // decrement by one
#define add(src, dest)   (dest += src)   // signed addition
#define adu(usrc, udest) (udest += usrc) // unsigned addition
#define adc(usrc, udest) (udest += usrc) // addition with carry-bit
#define sub(src, dest)   (dest -= src)   // signed subraction
#define sbu(usrc, udest) (udest -= usrc) // unsigned subtraction
#define sbc(src, dest)   (dest -= src)   // subraction with borrow-bit
#define cmp(src, dest)   (dest -= src)   // sets MSW-flags
