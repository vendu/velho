#ifndef __V0_VM_H__
#define __V0_VM_H__

struct vm {
	int64_t  wregs[V0_MAX_REGS];
	int32_t  xregs[VO_MAX_REGS];
	size_t   memsize;
	void     *mem;
	v0page_t *pagedir[1024];
	v0page_t *tlb[128][8];
	int8_t   *iop;
};

#endif /* __V0_VM_H__ */

