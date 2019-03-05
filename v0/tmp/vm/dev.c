#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <valhalla/util.h>
#include <v0/vm/isa.h>
#include <v0/vm/vm.h>
#include <v0/vm/io.h>
#include <v0/vm/dev.h>

static struct v0tmr v0tmr;

void
v0readkbd(struct v0 *vm, uint8_t port, v0reg reg)
{
    v0reg *dest = v0regtoptr(vm, reg);
    v0reg  key = getchar();

    if (key == EOF) {
        key = 0;
    }
    *dest = key;

    return;
}

void
v0writetty(struct v0 *vm, uint8_t port, v0reg val)
{
    printf("%uc", val & 0xff);

    return;
}

void
v0writeerr(struct v0 *vm, uint8_t port, v0reg val)
{
    fprintf(stderr, "%uc", val & 0xff);

    return;
}

void
v0readrtc(struct v0 *vm, uint8_t port, v0reg reg)
{
    v0reg  *dest = v0regtoptr(vm, reg);
    time_t  tm = time(NULL);
    v0wreg  val = (v0wreg)tm;

    *dest = val;

    return;
}

void
v0readtmr(struct v0 *vm, uint8_t port, v0reg reg)
{
    v0wreg  w = vm->regs[reg];
    v0reg  *dest = v0regtoptr(vm, reg);
    v0wreg  val = ~INT64_C(0);

    if (!(w & V0_HIRES_TMR)) {
        struct timeval  tv = { 0 };

        gettimeofday(&tv, NULL);
        val = tv.tv_sec * 1000000;
        val += tv.tv_usec;
        *dest = val;
    } else {
        struct timespec tsz = { 0 };
        struct timespec ts;

#if defined(CLOCK_MONOTONIC)
        clock_gettime(CLOCK_MONOTONIC, &ts);
#else
        clock_gettime(CLOCK_REALTIME, &ts);
#endif
        val = tscmp(tsz, ts);
        *dest = (v0reg)val;
    }

    return;
}

void
v0conftmr(struct v0 *vm, uint8_t port, v0reg val)
{
    if (val & V0_HIRES_TMR) {
        /*
         * submicrosecond timer facilities
         * - TODO: support HPET on X86
         */
        v0tmr.mode |= V0_HIRES_TMR;
    } else {
        v0tmr.mode &= ~V0_HIRES_TMR;
    }
    if (val & V0_INTERVAL_TMR) {
        v0tmr.mode |= V0_INTERVAL_TMR;
        v0tmr.freq = val & 0xff;
    }

    return;
}

void
v0writevtd(struct v0 *vm, uint8_t port, v0reg val)
{
    FILE *fp = vm->vtdfp;
    int   ret;

    if (fp) {
        ret = fputc(val & 0xff, fp);
        if (ret != EOF) {

            return;
        }
        fprintf(stderr, "V0: failed to print to VTD-file %s\n", vm->vtdpath);
        fprintf(stderr, "closing VTD-file %s\n", vm->vtdpath);
        fclose(fp);
        vm->vtdfp = NULL;
    }

    return;
}

