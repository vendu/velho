#ifndef __ZEN_VM_H__
#define __ZEN_VM_H__

/* zen kernel virtual memory interface */
#include <v0/conf.h>
#include <v0/mem.h>

struct taomemseg {
    const char *name;
    uintptr_t   adr;
    size_t      size;
    uintptr_t   flg;
};

#endif /* __ZEN_VM_H__ */

