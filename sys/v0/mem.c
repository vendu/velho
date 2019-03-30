/*
 * MEM	0x07	0x00	LEA		A, R	load effective address
 * MEM	0x07	0x01	LDR	RD	RIA, R	load into register
 * MEM	0x07	0x02	STR	WR	RI, A	store to memory
 * MEM	0x07	0x03	IPG	  	A	invalidate TLB-entry
 * MEM	0x07	0x04	CLR	CL	A	mark cacheline clear
 * MEM	0x07	0x05	CPF	RD|CL	A	prefetch cacheline
 * MEM	0x07	0x06	CFL	WR|CL	A	flush cacheline
 * MEM	0x07	0x08	BAR	BAR		full memory barrier
 * MEM	0x07	0x09	BRD	RD|BAR		memory read barrier
 * MEM	0x07	0x0a	BWR	WR|BAR		memory write barrier
 * MEM	0x07	0x0b	RFL	RD|MSW	R	read machine status word/flags
 * MEM	0x07	0x0c	WFL	WR|MSW	RI	write machine status word/flags
 */

#define v0lea1(vm, ins)   (v0getadr1(vm, ins))
#define v0lea2(vm, ins)   (v0getadr2(vm, ins))
#define v0clid(adr)       ((adr)  & ~(CLSZ - 1))
#define v0pgid(adr)       ((adr) & ~(PGSZ - 1))
#define v0ldr(reg1, reg2) ((reg2) = (reg1))
#define v0lda(adr, reg)   ((reg) = *(adr))
#define v0str(ins, adr)   (*(adr) =  regimm(ins))
#define v0ipg(pga)        (v0flstlb(pga))
#define v0clr(cla)        (v0clrbit(clbits, cla))
#define v0cpf(cla)        (ldr(cla, tmp))
#define v0cfl(cla)        (str(cla, adr1))
#define v0brd(vm)         (v0synrd(vm))
#define v0bwr(vm)         (v0synwr(vm))
#define v0bar(vm)         (v0synwr(vm), v0synrd(vm))

static __inline__ void
v0memop(struct v0 *vm, struct v0ins *ins)
{
    v0reg reg1 = v0getreg1(ins);
    v0reg reg2 = v0getreg2(ins);
    v0reg adr1 = lea1(vm, ins);
    v0reg adr2 = lea2(vm, ins);
    v0reg imm = v0getimm(ins);
    v0reg cl = cla(adr1);
    v0reg pg = pga(adr1);

    switch (ins->op) {
        case LEA:
            v0setreg(vm, reg2, adr1);

            break;
        case LDR:
            if (ins->flg & V0_NO_ADR) {
                v0setreg(vm, reg2, reg1);
            } else if (ins->flg & V0_IMM_BIT) {
                v0setreg(vm, reg2, imm);
            } else {
                reg2 = v0getmem(vm, adr1, v0reg);
            }

            break;
        case STR:
            if (ins->flg & V0_NO_ADR) {
                v0setmem(vm, adr2, reg1, v0reg, 0xffffffff);
            } else if (ins->flg & V0_IMM_BIT) {
                v0setmem(vm, adr2, imm, v0reg, 0xffffffff);
            }

            break;
        case IPG:
            v0invtlb(vm, pg);

            break;
    }

    return;
}
