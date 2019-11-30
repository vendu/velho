#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <v0/v0.h>

/* internal type-definitions */
typedef void            * (*v0instfunc(struct v0inst *inst));

/* thread-local data */
THREADLOCAL struct v0thr * t_thrctx;

/* global data */
static v0instfunc       * g_v0unittab[V0_MAX_UNITS];
static v0instfunc       * g_v0insttab[V0_MAX_UNITS][V0_MAX_INSTS];

