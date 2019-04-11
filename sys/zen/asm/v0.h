#ifndef __ZEN_ASM_V0_H__
#define __ZEN_ASM_V0_H__

#define V0_PAGE_SIZE    4096

#define BOOT_MEM_BIT    (1 << 0)
#define BOOT_FB_BIT     (1 << 1)

#if !defined(__ASSEMBLER__)

struct zenboothdr {
    uint32_t    flags;
    uint32_t    memsize;
};

struct zenbootfbhdr {
    uint32_t    adr;
    uint16_t    w;
    uint16_t    h;
    uint16_t    fmt;
    uint16_t    flg;
    uint32_t    mode;
};

#endif /* !defined(__ASSEMBLER__) */

#endif /* __ZEN_ASM_V0_H__ */

