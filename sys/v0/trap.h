#ifndef __V0_TRAP_H__
#define __V0_TRAP_H__

#define v0trapirq(t)     ((t) & V0_TRAP_IRQ_BIT)
#define v0trapirqid(t)   ((t) & ~V0_TRAP_IRQ_BIT)

/* hardware exceptions */
#define V0_TRAP_DZ       0x00 // divizion by zero       - code is PC
#define V0_TRAP_OP       0x01 // invalid opcode         - code is PC
#define V0_TRAP_PF       0x02 // page-fault             - code is ADR + flags
#define V0_TRAP_DF       0x03 // double-fault
#define V0_TRAP_TF       0x04 // triple-fault
#define V0_TRAP_TS       0x05 // invalid task state     - code is ADR
#define V0_TRAP_NP       0x06 // segment not present    - code is ID
#define V0_TRAP_SS       0x07 // stack-segment full     - code is SP
#define V0_TRAP_PE       0x08 // protection error       - code is ADR
#define V0_TRAP_FP       0x09 // floating-point error   - code is PC
#define V0_TRAP_OF       0x0a // overflow               - code is PC
#define V0_TRAP_BR       0x0b // bound-range exceeded   - code is ADR
#define V0_TRAP_AC       0x0c // alignment check        - code is ADR
#define V0_TRAP_MF       0x0d // memory failure (ECC)   - code is ADR
#define V0_TRAP_MC       0x0e // machine check          - code is ERROR
#define V0_TRAP_IO       0x0f // I/O error              - code is ADR + flags
#define V0_TRAP_DB       0x10 // debug trap             - code is PC
#define V0_TRAP_BP       0x11 // breakpoint             - code is PC
#define V0_TRAP_NMI      0x12 // non-maskable interrupt
#define V0_TRAP_LAST_SYS 0x1f
#define V0_TRAP_IRQ_BIT  0x20
#define V0_TRAP_IRQ(i)   (V0_TRAP_IRQ_BIT | (i))
#define V0_TRAP_IRQ0     V0_TRAP_IRQ(0)	// device interrupt 0   - TMR
#define V0_TRAP_IRQ1     V0_TRAP_IRQ(1)	// device interrupt 1   - AUD
#define V0_TRAP_IRQ2     V0_TRAP_IRQ(2)	// device interrupt 2	- VID
#define V0_TRAP_IRQ3     V0_TRAP_IRQ(3)	// device interrupt 3	- KBD
#define V0_TRAP_IRQ4     V0_TRAP_IRQ(4)	// device interrupt 4	- PTR
#define V0_TRAP_IRQ5     V0_TRAP_IRQ(5)	// device interrupt 5	- HID
#define V0_TRAP_IRQ6     V0_TRAP_IRQ(6)	// device interrupt 6	- NET
#define V0_TRAP_IRQ7     V0_TRAP_IRQ(7)	// device interrupt 7	- DSK
#define V0_TRAP_IRQ8     V0_TRAP_IRQ(8)	// device interrupt 8   - OPT
#define V0_TRAP_IRQ9     V0_TRAP_IRQ(9)	// device interrupt 9   - USB
#define V0_TRAP_IRQ10    V0_TRAP_IRQ(a)	// device interrupt 10
#define V0_TRAP_IRQ11    V0_TRAP_IRQ(b)	// device interrupt 11
#define V0_TRAP_IRQ12    V0_TRAP_IRQ(c)	// device interrupt 12
#define V0_TRAP_IRQ13    V0_TRAP_IRQ(d)	// device interrupt 13
#define V0_TRAP_IRQ14    V0_TRAP_IRQ(e)	// device interrupt 14
#define V0_TRAP_IRQ15    V0_TRAP_IRQ(f) // device interrupt 15

/* IV-register contents */

/* v0trapdesc */
#define V0_TRAP_SYS      (1 << 0)
#define V0_TRAP_PHYS     (1 << 1)
#define V0_TRAP_ADR_MASK 0xfffffffc

/* PF (page-fault) trap error code bits */
#define V0_PF_EX         (1 << 0)       // page-fault during execution
#define V0_PF_WR         (1 << 1)       // page-fault during write
#define V0_PF_RD         (1 << 2)       // page-fault during read
#define V0_PF_R0         (1 << 3)       // page-fault with system page
/* IO (input/output) trap error code bits */
#define V0_IO_SP         (1 << 0)       // invalid seek position
#define V0_IO_WR         (1 << 1)       // write error
#define V0_IO_RD         (1 << 2)       // read error
#define V0_IO_PF         (1 << 3)       // insufficient I/O permission

struct v0trapframe {
    void  *retadr;
    v0reg  msw;
    v0reg  sp;
    v0reg  fp;
};

struct v0trapdesc {
    long  id;
    void *func(void *);
};

#endif /* __V0_TRAP_H__ */

