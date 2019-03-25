#define V0_SAVE_REGS    8
#define V0_R0_REG       0
#define V0_R1_REG       1
#define V0_R2_REG       2
#define V0_R3_REG       3
#define V0_R4_REG       4
#define V0_R5_REG       5
#define V0_R6_REG       6
#define V0_R7_REG       7
#define V0_R8_REG       8
#define V0_R9_REG       9
#define V0_R10_REG      10
#define V0_R11_REG      11
#define V0_FP_REG       12
#define V0_SP_REG       13
#define V0_LR_REG       14
#define V0_PC_REG       15

static __inline__ void
v0csr(v0adr adr)
{
    v0psh(V0_PC_REG);
    v0csr(adr);
}

static __inline__ void
v0beg(v0reg nb)
{
    v0psh(V0_FP_REG);
    v0setreg(V0_FP_REG, V0_SP_REG);
    v0psm(V0_R4_REG, V0_R11_REG);
    v0addreg(V0_SP_REG, nb);
}

static __inline__ void
v0fin(void)
{
    v0setreg(V0_SP_REG, V0_FP_REG);
    v0addreg(-V0_SAVE_REGS * V0_REG_SIZE);
    v0pom(V0_R4_REG, V0_R11_REG);
    v0pop(V0_FP_REG);
    v0pop(V0_PC_REG);
}

