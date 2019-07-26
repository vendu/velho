#ifndef __V0_GPU_H__
#define __V0_GPU_H__

#define V0_GPU_8_PIX       0
#define V0_GPU_444_PIX     1
#define V0_GPU_555_PIX     2
#define V0_GPU_565_PIX     3
#define V0_GPU_888_PIX     4
#define V0_GPU_ARGB32_PIX  5
#define V0_GPU_PIX_FORMAT  V0_GPU_444_PIX

#define V0_GPU_NOP_OP      0    // (void)0
#define V0_GPU_COPY_OP     1    // dest = src
#define V0_GPU_COPY_INV_OP 2    // dest = ~src
#define V0_GPU_CLEAR_OP    3    // dest = 0
#define V0_GPU_SET_OP      4    // dest = ~0
#define V0_GPU_INV_OP      5    // dest = ~dest
#define V0_GPU_AND_OP      6    // dest = src & dest
#define V0_GPU_AND_REV_OP  7    // dest = src & (~dest)
#define V0_GPU_AND_INV_OP  8    // dest = (~src) & dest
#define V0_GPU_OR_OP       9    // dest = src | dest
#define V0_GPU_OR_INV_OP   10   // dest = (~src) | dest
#define V0_GPU_OR_REV_OP   11   // dest = src | (~dest)
#define V0_GPU_XOR_OP      12   // dest = src ^ dest
#define V0_GPU_NOR_OP      13   // dest = (~src) & (~dest)
#define V0_GPU_EQ_OP       14   // dest = (~src) ^ dest
#define V0_GPU_NAND_OP     15   // dest = ~(src & dest)

struct v0sysgpu {
    v0wreg  regs[V0_GPU_REGS];
    v0reg   w;
    v0reg   h;
    v0pix  *fb;
};

#endif /* __V0_GPU_H__ */

