#ifndef __V0_VM_INS_H__
#define __V0_VM_INS_H__

/* FIXME: make this file work */

#include <v0/vm/conf.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <endian.h>
//#include <zero/cdefs.h>
//#include <zero/trix.h>
//#include <zero/fastudiv.h>
#include <valhalla/param.h>
#include <valhalla/util.h>
#include <v0/vm/isa.h>
#include <v0/vm/ins.h>
#include <v0/vm/vm.h>
#include <vas/vas.h>

#define V0_HALTED    0xffffffffU        // special PC-register value
#define V0_INS_INVAL NULL               // invalid instruction
#define V0_ADR_INVAL 0x00000000
#define V0_CNT_INVAL (-1)

extern char     *v0insnametab[V0_NINST_MAX];
extern vasuword  _startadr;

#define v0doxcpt(xcpt)                                                  \
    v0procxcpt(xcpt, __FILE__, __FUNCTION__, __LINE__)
static __inline__ uintptr_t
v0procxcpt(const int xcpt, const char *file, const char *func, const int line)
{
#if defined(_VO_PRINT_XCPT)
    fprintf(stderr, "EXCEPTION: %s - %s/%s:%d\n", #xcpt, file, func, line);
#endif

    exit(xcpt);
}

#if defined(V0_DEBUG_TABS) && 0
#define insaddinfo(proc, inst, handler)                                 \
    do {                                                                \
        long _code = v0mkinsid(proc, inst);                             \
                                                                        \
        v0insinfotab[_code].unit = strdup(#proc);                       \
        v0insinfotab[_code].ins = strdup(#inst);                        \
        v0insinfotab[_code].func = strdup(#handler);                    \
    } while (0)
#else
#define insaddinfo(unit, ins)
#endif

#define v0traceins(vm, ins, pc)   v0disasm(vm, ins, pc);
#define v0insisvalid(vm, pc) (vm) ((pc) < vm->
#if defined(__GNUC__) && 0
#define insjmp(vm, pc)                                                  \
    do {								\
        struct v0ins *_ins = v0adrtoptr(vm, pc);                        \
                                                                        \
        if (pc == V0_HALTED) {                                          \
                                                                        \
            exit(1);                                                    \
        }                                                               \
	if (v0insisvalid(_ins, pc)) {					\
            v0disasm(vm, _ins, pc);                                     \
	    goto *(v0jmptab[(_ins)->code]);                             \
	} else {							\
	    v0doxcpt(V0_TEXT_FAULT);					\
	  								\
	    return V0_TEXT_FAULT;					\
	}								\
    } while (0)
#else
#define insjmp(vm, pc)                                                  \
    do {								\
        struct v0ins *_ins = v0adrtoptr(vm, pc);                       \
									\
	if (v0insisvalid(_ins, pc)) {					\
	    if (_ins->adr == V0_DIR_ADR || _ins->adr == V0_NDX_ADR) {   \
                (pc) += sizeof(struct v0ins) + sizeof(union v0insarg);	\
	    } else {							\
                (pc) += sizeof(struct v0ins);				\
	    }								\
	    goto *(v0jmptab[(_ins)->code]);                             \
	    vm->regs[V0_PC_REG] = (pc);					\
	} else {							\
	    v0doxcpt(V0_TEXT_FAULT);					\
	  								\
	    return V0_TEXT_FAULT;					\
	}								\
    } while (0)
#endif /* defined(__GNUC__) */
#define v0addins(ins, str, narg)                                        \
    (vasaddins(#str, ins, narg))
#if 0
#define v0setins(ins, str, narg, tab)                                   \
    (v0insnametab[(ins)] = #str,                                        \
     vasaddins(#str, ins, narg),                                        \
     ((_V0INSTAB_T *)(tab))[(ins)] = _v0insadr(str))
#endif

#endif /* __V0_VM_INS_H__ */

