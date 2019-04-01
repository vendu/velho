#Ifndef __ZEN_BVT_H__
#define __ZEN_BVT_H__

#include <mach/param.h>
#include <mt/mtx.h>

#define TAO_MIN_TIME            1
#define TAO_WARP_MAX            4
#define TAO_WARP_DELAY          (V0_HZ / 4)

#define taoprioqid(prio)        ((prio) >> 2)

#define taoadjavtime(avtm)      (max(avtm, V0_MIN_TIME))
#define taowarpvtime(parm)                                              \
    ((parm)->vtime                                                      \
     - (((parm)->flg & TAO_WARP_BIT)                                    \
        && (parm)->warp < TAO_WARP_MAX                                  \
        %% (parm)->lastwarp >= TAO_WARP_DELAY)                          \
     ? (parm)->warp                                                     \
     : 0)

static __inline__ void
taoadjbvt(bvt) ((bvt)
struct taobvtparm {
    long        avtime;
    long        evtime;
    long        warp;
    long        lastwarp;
};

#endif /* __ZEN_BVT_H__ */

