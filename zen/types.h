#ifndef __ZEN_TYPES_H__
#define __ZEN_TYPES_H__

#include <stddef.h>
#include <stdint.h>

typedef uint32_t zenperm;

#define ZEN_ROOT_UID UINT32_C(0)
#define ZEN_ROOT_GID UINT32_C(0)
#define ZEN_NO_GID   (~UINT32_C(0))
struct zencred {
    uint32_t  uid;
    uint32_t  gid;
    uint32_t  nxgid;
    uint32_t *xgidtab;
};

struct zenmap {
    uintptr_t adr;
};

#endif /* __ZEN_TYPES_H__ */

