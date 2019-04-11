#ifndef __ZEN_TYPES_H__
#define __ZEN_TYPES_H__

#include <stddef.h>
#include <stdint.h>
#if defined(__v0__)
#include <zen/bsp/v0.h>
#endif
#include <zen/types.h>

typedef intmax_t        zenlong;        // system maximum
typedef uintmax_t       zenulong;       // unsigned system maximum
typedef int32_t         zenid_t;        // generic object ID
typedef uint32_t        zendev_t;       // device type
typedef int64_t         zenoff_t;       // file-system offset
typedef int32_t         zenuid_t;       // user ID
typedef int32_t         zengid_t;       // group ID

#define ZEN_ROOT_UID    INT32_C(0)
#define ZEN_ROOT_GID    INT32_C(0)
#define ZEN_NO_GID      (~INT32_C(0))
struct zencred {
    zenuid_t            uid;
    zengid_t            gid;
    m_uword_t           nxgid;
    zengid_t           *xgidtab;
};

struct zenacl {
    void               *data;
    m_word_t          (*chk)(zenuid_t uid, zenuid_t gid, struct zenacl *acl);
};

struct zenperm {
    m_uword_t           flags;
};

struct zenmap {
    m_adr_t             adr;
    m_uword_t           size;
    m_uword_t           flags;
    struct zencred     *cred;
};

#endif /* __ZEN_TYPES_H__ */

