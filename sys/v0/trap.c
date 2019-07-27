#include <v0/trap.h>

static v0trapdesc g_trapvector[V0_MAX_TRAPS];
static v0sigmap   g_sigmap[V0_MAX_TRAPS];

static void
v0initsigmap(void)
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

    return;
}

static void
v0inittrapvector(void)
{
    g_trapvector[V0_IRQ_TRAP_0x00] = (v0trapdesc)v0tmrirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x01] = (v0trapdesc)v0audirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x02] = (v0trapdesc)v0vidirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x03] = (v0trapdesc)v0kbdirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x04] = (v0trapdesc)v0ptrirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x05] = (v0trapdesc)v0hidirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x06] = (v0trapdesc)v0netirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x07] = (v0trapdesc)v0dskirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x08] = (v0trapdesc)v0optirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x09] = (v0trapdesc)v0usbirq | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x0a] = (v0trapdesc)v0irq0x0a | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x0b] = (v0trapdesc)v0irq0x0b | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x0c] = (v0trapdesc)v0irq0x0c | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x0d] = (v0trapdesc)v0irq0x0d | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x0e] = (v0trapdesc)v0irq0x0e | V0_TRAP_SYS;
    g_trapvector[V0_IRQ_TRAP_0x0f] = (v0trapdesc)v0irq0x0f | V0_TRAP_SYS;

    return;
}

static void
v0inittraps(void)
{
    v0initsigmap();
    v0inittrapvector();

    return;
}

