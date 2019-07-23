#ifndef __V0_TRAP_H__
#define __V0_TRAP_H__

#define v0trapirq(t)     ((t) & V0_TRAP_IRQ_BIT)
#define v0trapirqid(t)   ((t) & ~V0_TRAP_IRQ_BIT)

/* hardware exceptions */
#define V0_TRAP_DZ       0x00 // divizion by zero       - code is PC
#define V0_TRAP_OP       0x01 // invalid opcode         - code is PC
#define V0_TRAP_PF       0x02 // page-fault             - code is ADR + flags
#define V0_TRAP_DF       0x03 // double-fault           - code is ADR
#define V0_TRAP_TF       0x04 // triple-fault           - code is ADR
#define V0_TRAP_TS       0x05 // invalid task state     - code is ADR
#define V0_TRAP_NP       0x06 // segment not present    - code is ADR

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
#define V0_IRQ_TRAP_BIT  0x20
#define V0_IRQ_TRAP(i)   (V0_IRQ_TRAP_BIT | (i))
#define V0_IRQ_TRAP0     V0_IRQ_TRAP(0)		// device interrupt 0   - TMR
#define V0_IRQ_TRAP1     V0_IRQ_TRAP(1)		// device interrupt 1   - AUD
#define V0_IRQ_TRAP2     V0_IRQ_TRAP(2)		// device interrupt 2	- VID
#define V0_IRQ_TRAP3     V0_IRQ_TRAP(3)		// device interrupt 3	- KBD
#define V0_IRQ_TRAP4     V0_IRQ_TRAP(4)		// device interrupt 4	- PTR
#define V0_IRQ_TRAP5     V0_IRQ_TRAP(5)		// device interrupt 5	- HID
#define V0_IRQ_TRAP6     V0_IRQ_TRAP(6)		// device interrupt 6	- NET
#define V0_IRQ_TRAP7     V0_IRQ_TRAP(7)		// device interrupt 7	- DSK
#define V0_IRQ_TRAP8     V0_IRQ_TRAP(8)		// device interrupt 8   - OPT
#define V0_IRQ_TRAP9     V0_IRQ_TRAP(9)         // device interrupt 9   - USB
#define V0_IRQ_TRAP10    V0_IRQ_TRAP(10)	// device interrupt 10
#define V0_IRQ_TRAP11    V0_IRQ_TRAP(11)	// device interrupt 11
#define V0_IRQ_TRAP12    V0_IRQ_TRAP(12)	// device interrupt 12
#define V0_IRQ_TRAP13    V0_IRQ_TRAP(13)	// device interrupt 13
#define V0_IRQ_TRAP14    V0_IRQ_TRAP(14)	// device interrupt 14
#define V0_IRQ_TRAP15    V0_IRQ_TRAP(15)        // device interrupt 15
#define V0_TRAPS         32                     // total # of traps for system

/* IV-register contents */

/* v0trapdesc */
#define V0_TRAP_SYS             (1 << 0)       	// system-mode trap execution
#define V0_TRAP_STK             (1 << 1)        // traps on separate stack
#define V0_TRAP_ADR_MASK        0xfffffffc     	// trap-handler function pointer

/* PF (page-fault) trap error code bits */
#define V0_PF_EXEC         	(1 << 0)       	// page-fault during execution
#define V0_PF_WRITE         	(1 << 1)       	// page-fault during write
#define V0_PF_READ         	(1 << 2)       	// page-fault during read
#define V0_PF_RING0         	(1 << 3)       	// page-fault with system page
#define V0_PF_STACK         	(1 << 4)       	// stack-fault
#define V0_PF_ADR_MASK   	(~(V0_PAGE_SIZE - 1)) // page-fault address mask
/* IO (input/output) trap error code bits */
#define V0_IO_EXEC         	(1 << 0)       	// execute error
#define V0_IO_WRITE         	(1 << 1)       	// write error
#define V0_IO_READ              (1 << 2)        // read error
#define V0_IO_PERM         	(1 << 3)       	// no I/O permission
#define V0_IO_SEEK         	(1 << 4)       	// invalid seek position
#define V0_IO_DEV_ERROR      	(1 << 5)        // device I/O error
#define V0_IO_CHK         	(1 << 6)        // check if given I/O allowed

/* trap stack-frame */
/*
 * top-to-bottom
 * -------------
 * - ret
 * - ufp
 * - usp
 * - msw
 * - code
 */
struct v0trapframe {
    struct v0arg code;                          // error code
    v0uword      msw;                   	// machine status-word
    v0uword      usp;                   	// user-mode stack-pointer
    v0uword      ufp;                   	// user-mode frame-pointer
    v0uword      retp;                          // return address
};

#endif /* __V0_TRAP_H__ */

