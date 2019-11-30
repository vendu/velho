/* NOTE: type MUST be one of ZEN_EXEC_PERM, ZEN_WRITE_PERM, ZEN_READ_PERM */

zenlong
zenchkperm(struct zenperm *perm, struct zencred *cred, zenlong type)
{
    zenuid_t    usr = zengetuid();
    zenuid_t    grp = zengetgid();
    zenuid_t    uid = perm->owner->uid;
    zenuid_t    gid = perm->owner->gid;
    zenperm_t   flg = perm->flg;
    long        ret;

    ret = _zenchkperm(uid, grp, flg, type);

    return ret;
}

