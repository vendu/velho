#include <v0/trap.h>

static v0trapdesc g_trapvector[V0_TRAPS];
static v0sigmap   g_sigmap[V0_TRAPS];

void
trapinittabs(v0trapdesc *vec)
{
    g_sigmap[V0_TRAP_DZ] = SIGFPE;
    g_sigmap[V0_TRAP_OP] = SIGILL;
    g_sigmap[V0_TRAP_TF] = SIGBUS;
    g_sigmap[V0_TRAP_TS] = SIGABRT;
    g_sigmap[V0_TRAP_NP] = SIBGUS;
    g_sigmap[V0_TRAP_SS] = SIGSTKFLT;
    g_sigmap[V0_TRAP_PE] = SIGSTKFLT;
    g_sigmap[V0_TRAP_FP] = SIGFPE;
    g_sigmap[V0_TRAP_BR] = SIGBUS;
    g_sigmap[V0_TRAP_AC] = SIGBUS;
    g_sigmap[V0_TRAP_MF] = SIGABRT;
    g_sigmap[V0_TRAP_MC] = SIGABRT;
    g_sigmap[V0_TRAP_IO] = SIGIOT;
    g_sigmap[V0_TRAP_BP] = SIGSTOP;
    g_trapvector[V0_IRQ_TRAP_0x0] = v0tmrirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x1] = v0audirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x2] = v0vidirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x3] = v0kbdirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x4] = v0ptrirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x5] = v0hidirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x6] = v0netirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x7] = v0dskirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x8] = v0optirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x9] = v0usbirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x10] = v0irq10 | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x11] = v0irq11 | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x12] = v0irq12 | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x13] = v0irq13 | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x14] = v0irq14 | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x15] = v0irq15 | V0_TRAP_SYS;

    return;
}

