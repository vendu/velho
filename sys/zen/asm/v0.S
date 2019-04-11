#include <zen/asm/v0.h>

.org	0x00000000

.org	$V0_PAGE_SIZE

.globl	bootstrap, boothdr

	jmp	bootstrap

boothdr:
_bootflg:		.word	(BOOT_MEM_BIT | BOOT_FB_BIT)
_bootmemsize:	.word	0x00000000
_bootfbhdr:
_bootfbadr:		.word	0x00000000
_bootfbw:		.half	640
_bootfbh:		.half	360
_bootfbfmt:		.half	0x0000
_bootfbflg:		.half	0x0000
_bootfbmode:	.word	0x00000000

.align	4

bootstrap:
	ldr			$V0_PROBE_FB
	ldr			$_bootfbhdr, %r1
	v0probefb
	ldr			$_boothdr, %r0
	ldr			$(V0_MP_BIT | V0_VM_BIT), %r1
	v0sysmode

