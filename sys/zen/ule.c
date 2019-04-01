#include <zen/ule.h>

static struct taoqueue  g_taoprioq[TAO_SCHED_CLASSES][TAO_SCHED_PRIOS];
static struct taoqueue  g_taortq;
static struct taoqueue  g_taoidleq;

