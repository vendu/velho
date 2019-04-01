#ifndef __ZEN_BSP_V0_H__
#define __ZEN_BSP_V0_H__

typedef int32_t         	v0word;         // machine register
typedef uint32_t        	v0uword;        // unsigned register/word
typedef uint32_t        	v0adr;          // memory address
typedef uint32_t        	v0pagedesc;     // page-structure descriptor

/* memory parameters */
#define V0_CL_SIZE              32              // memory cacheline size
#define V0_STRIDE_SIZE          128             // cache stride size
#define V0_PAGE_SIZE            4096            // page size
#define V0_JUMBO_SIZE           131072          // jumbo-page size
#define V0_BANK_SIZE            2048            // RAM-bank size

/* page-descriptor flags */
#define V0_MEM_EXEC             (1 << 0)        // executable page
#define V0_MEM_WRITE            (1 << 1)        // write permission bit
#define V0_MEM_READ             (1 << 2)        // read permission bit
#define V0_MEM_CORE             (1 << 3)        // in-core memory (in RAM)
#define V0_MEM_NONTEMP   	(1 << 4)        // non-temporal (uncached) hint
#define V0_MEM_BUSY     	(1 << 5)        // memory accessed bit
#define V0_MEM_MOD      	(1 << 6)        // modified (written) bit
#define V0_MEM_GLOBAL     	(1 << 7)        // global memory
#define V0_MEM_SYSTEM      	(1 << 8)        // system memory
#define V0_MEM_GROW_DOWN        (1 << 9)        // stack segments
#define V0_MEM_ATOMIC           (1 << 10)       // page needs atomic access
#define V0_MEM_TRANS            (1 << 11)       // STM

/* memory layout */

#define V0_TRAP_VECTOR          0x00000000
#define V0_TEXT_MEM             V0_PAGE_SIZE
#define V0_SYS_STACK            0x00000000

#endif /* __ZEN_BSP_V0_H__ */

