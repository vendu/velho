/*
 * - 8 machine words in size
 */
struct memmap {
    mtmtx       mtx;            // reference count
    void       *adr;            // allocation [page] address
    long        size;           // size of map in bytes
    long        pid;            // process ID
    long        uid;            // user ID
    long        gid;            // group ID
    long        perm;           // permission bits
    void       *acl;            // access control list
};
