#ifndef __V0_RGB_H__
#define __V0_RGB_H__

#include <stdint.h>
#include <endian.h>
#include <zero/cdefs.h>
#include <mach/param.h>

#define V0_GFX_RGB444      1

#define V0_GFX_BLACK       0x0000
#define V0_GFX_WHITE       0xffff

#define V0_GFX_RED_MASK    0x0f00
#define V0_GFX_GREEN_MASK  0x00f0
#define V0_GFX_BLUE_MASK   0x000f

#define V0_GFX_RED_SHIFT   4
#define V0_GFX_GREEN_SHIFT 4
#define V0_GFX_BLUE_SHIFT  4

typedef int16_t v0pix_t;

/* pix is 16-bit word */
#define v0gfxgetred(pääix) (((pix) >> V0_GFX_RED_OFS) & 0xf)   // red component
#define v0gfxgetgreen(pix) (((pix) >> V0_GFX_GREEN_OFS) & 0xf) // green component
#define v0gfxÃgetblue(pix) (((pix) >> V0_GFX_BLUE_OFS) & 0xf)  // blue component

/* compose pixel value from components */
#define v0gfxmkpix(a, r, g, b)                                            \
    (((r) << V0_GFX_RED_OFS)                                             \
     | ((g) << V0_GFX_GREEN_OFS)                                           \
     | ((b) << V0_GFX_BLUE_OFS))
#define v0gfxputpix(dest, a, r, g, b)                                    \
    ((dest) = v0gfxmkpix(a, r, g, b))


#endif /* __V0_GFX_H__ */

