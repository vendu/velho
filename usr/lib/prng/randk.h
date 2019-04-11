#ifndef __ZERO_RANDK_H__
#define __ZERO_RANDK_H__

void srandk(long seed);
long randk(void);

#define RANDKMBIG  1000000000L
#define RANDKMSEED 161803398L
#define RANDKMZ    0
#define RANDKNTAB  56

struct _randkglobal {
#if 0
    long seed;
#endif
    int initialized;
    int inext;
    int inextp;
};

#endif /* __ZERO_RANDK_H__ */

