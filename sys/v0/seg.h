#ifndef __V0_SEG_H__
#define __V0_SEG_H__

#define V0_HI_SEG_OFS    0xc0000000
#define V0_STK_SEG_OFS   V0_HI_SEG_OFS
#define V0_STK_SIZE      (3 * V0_PAGE_SIZE)
#define V0_STK_PROT_SIZE V0_PAGE_SIZE
#define V0_NULL_SEG      0x00   // NULL-segment
#define V0_TRAP_SEG      0x01   // trap-vector
#define V0_CODE_SEG      0x02   // code-segment
#define V0_RODATA_SEG    0x03   // read-only data segment
#define V0_HEAP_SEG      0x04   // heap segment
#define V0_MAP_SEG       0x05   // mapped memory
#define V0_STK_SEG       0x06   // user- and system-mode stacks
#define V0_HI_SEG        0x07   // I/O-buffers, device memory maps

#endif /* __V0_SEG_H__ */

