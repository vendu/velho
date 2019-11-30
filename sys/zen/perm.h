#ifndef __ZEN_PERM_H__
#define __ZEN_PERM_H__

/* user credentials */

#define ZEN_ROOT_UID    INT32_C(0)
#define ZEN_ROOT_GID    INT32_C(0)
#define ZEN_NO_GID      (~INT32_C(0))
struct zencred {
    zenuid_t            uid;
    zengid_t            gid;
};

#define ZEN_EXEC_PERM   1
#define ZEN_WRITE_PERM  2
#define ZEN_READ_PERM   3

/* I/O permissions */

/* standard permission check from Unix;
 * - allow all access to root user (uid == 0)
 * - check for user permission
 * - check for group permission
 * - check for world permission
 */
#define _zenchkperm(uid, grp, flg, type)                                \
    (!(usr)                                                             \
     ? 1                                                                \
     : (((uid) == (usr) && ((flg) & ((type) << 6)))                     \
        ? 1                                                             \
        : (((gid) == (grp) && ((flg) & ((type) << 3)))                  \
           ? 1                                                          \
           : ((flg) && ((flg) & type)))))

struct zenperm {
    struct zencred     *owner;
    zenperm_t           flg;
};

/* I/O permission-bits */
#define V0_IO_UR_PERM   	(1L << 8)       // user read permission bit
#define V0_IO_UW_PERM   	(1L << 7)       // user write
#define V0_IO_UX_PERM           (1L << 6)       // user execute
#define V0_IO_GR_PERM   	(1L << 5)       // group read
#define V0_IO_GW_PERM   	(1L << 4)       // group write
#define V0_IO_GX_PERM   	(1L << 3)       // group execute
#define V0_IO_AR_PERM   	(1L << 2)       // world read
#define V0_IO_AW_PERM   	(1L << 1)       // world write
#define V0_IO_AX_PERM   	(1L << 0)       // world execute

extern zenchkperm(struct zenperm *perm, struct zencred *cred, zenlong type);

#endif __ZEN_PERM_H__

