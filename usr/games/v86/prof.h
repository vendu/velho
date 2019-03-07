#ifndef __V86_PROF_H__
#define __V86_PROF_H__

/* INSTRUCTION TIMINGS (prof) */

/* REFERENCE: http://www.agner.org/optimize/instruction_tables.pdf */
/* instruction timings in cycles (as listed for pentium [mmx] in REFERENCE */

/*
 * MEMORY
 * ------
 * - cacheline fetch: 4 + CLSIZE clock cycles (size in 32- or 64-bit words)
 * - instruction fetch: 1 clock cycle
 * - instruction decode: 1 clock cycle
 * - immediate operand fetch: 2 clock cycles
 * - memory read/write: 4 clock cycles
 *
 */
/* NOP
 * ---
 * - 1 clock cycle
 */
/* NOT
 * ---
 * - 2 clock cycles
 */
/* AND, OR, XOR
 * ------------
 * - 2 clock cycles+ operands
 */
/* SHR, SHL, SAR, ROR, ROL
 * -----------------------
 * - 3 clock cycles + 1 for immediate operand
 */
/* INC, DEC
 * --------
 * - 2 clock cycles
 */
/* ADD, SUB
 * --------
 * - 2 clock cycles if both are registers
 * - 3 clock cycles with immediate operand
 */
/* CMP
 * ---
 * - 2 clock cycles if both are registers
 * - 3 clock cycles with immediate operand
 */
/* MUL
 * ---
 * - 8 clock cycles
 */
/* DIV
 * ---
 * - 32 clock cycles
 */
/* LDA, STA
 * --------
 * - src and dest are registers: 2
 * - src/dest is immediate operand: 3
 * - src/dest is a memory operand: 4
 */
/* PUSH, POP
 * ---------
 * - 4 cycles if src/dest is a register
 */
/* PUSHA, POPA
 * - 4 cycles + 1 cycle per register
 */
/*
 * JMP, branches
 * -------------
 * - 8 clock cycles for the jump
 */
/*
 * IN, OUT
 * - 8 clock cycles
 */

/* operation timings in clock cycles */

/* jmp + branches may lead to additional code fetches into cache */
#define V86_JMP_CYCLES    4     // local jump; possibly within code cache
#define V86_BRANCH_CYCLES 8     // control transfer
/* memory access timings */
#define V86_MEM_CYCLES    4     // [uncached] memory access
#define V86_FETCH_CYCLES  (V86_MEM_CYCLES * V86_CACHELINE_WORDS) // cacheline
#define V86_CACHE_CYCLES  1     // cached memory access
/* I/O operation timings */
#define V86_IO_CYCLES     8     // IN, OUT

#define V86_NOP_CYCLES    1
#define V86_NOT_CYCLES    2
#define V86_AND_CYCLES    2
#define V86_OR_CYCLES     2
#define V86_XOR_CYCLES    2
#define V86_SHL_CYCLES    2
#define V86_SHR_CYCLES    2
#define V86_SAR_CYCLES    2
#define V86_ADD_CYCLES    2
#define V86_SUB_CYCLES    2
#define V86_CMP_CYCLES    2
#define V86_MUL_CYCLES    8
#define V86_DIV_CYCLES    32
#define V86_JE_CYCLES     V86_BRANCH_CYCLES
#define V86_JNE_CYCLES    V86_BRANCH_CYCLES
#define V86_JLT_CYCLES    V86_BRANCH_CYCLES
#define V86_JLE_CYCLES    V86_BRANCH_CYCLES
#define V86_JGT_CYCLES    V86_BRANCH_CYCLES
#define V86_JGE_CYCLES    V86_BRANCH_CYCLES
#define V86_JC_CYCLES     V86_BRANCH_CYCLES
#define V86_JNC_CYCLES    V86_BRANCH_CYCLES
#define V86_JO_CYCLES     V86_BRANCH_CYCLES
#define V86_JNO_CYCLES    V86_BRANCH_CYCLES
#define V86_CALL_CYCLES   (V86_PUSHA_CYCLES                             \
                           + V86_BRANCH_CYCLES)
#define V86_RET_CYCLES    (2 * V86_CACHE_CYCLES                         \
                           + V86_BRANCH_CYCLES                          \
                           + V86_POPA_CYCLES)
#define V86_LDR_CYCLES    2
#define V86_STR_CYCLES    2
#define V86_PUSH_CYCLES   4
#define V86_PUSHA_CYCLES  (V86_PUSH_CYCLES                              \
                           + V86_USER_REGISTERS * V86_CACHE_CYCLES)
#define V86_POP_CYCLES    4
#define V86_POPA_CYCLES   (V86_POP_CYCLES                               \
                           + V86_USER_REGISTERS * V86_CACHE_CYCLES)
#define V86_IN_CYCLES     8
#define V86_OUT_CYCLES    8
#define V86_HLT_CYCLES

#endif /* __V86_PROF_H__ */

