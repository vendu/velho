#ifndef __V0_VM_IO_H__
#define __V0_VM_IO_H__

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <v0/vm/vm.h>

void v0readkbd(struct v0 *vm, uint8_t port, v0reg reg);
void v0writetty(struct v0 *vm, uint8_t port, v0reg val);
void v0writeerr(struct v0 *vm, uint8_t port, v0reg val);
void v0readrtc(struct v0 *vm, uint8_t port, v0reg reg);
void v0readtmr(struct v0 *vm, uint8_t port, v0reg reg);
void v0conftmr(struct v0 *vm, uint8_t port, v0reg val);
void v0writevtd(struct v0 *vm, uint8_t port, v0reg val);

#endif /* __V0_VM_IO_H__ */

