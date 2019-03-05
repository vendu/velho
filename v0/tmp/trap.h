#ifndef __V0_TRAP_H__
#define __V0_TRAP_H__

struct v0trapframe {
    void  *fp;
    void  *retadr;
    v0reg  msw;
    v0reg  sp;
};

struct v0trapdesc {
    long  id;
    void *func(void *);
};

#endif /* __V0_TRAP_H__ */

