# %r1	- src
# %r2	- dest
# %r3	- npix
_dith444:
	psh	%fp				// save frame pointer
	ldr	%sp, %fp		   // copy stack pointer to frame pointer
	ldr	*%r1, %r5		  // fetch source pixel
	ldr	$0x00f0f0f0, %r7   // set pixel mask for RGB24
_loop:
	ldr	%r1, %r5		   // fetch source pixel
	dec	%r3				// # of pixels left
	inc	%r1				// next source address
	cmp	$0, %r3			// 0 pixels left?
#if (HAVE_SLM)
	msr	$4, %r7, %r5	   // r5 = r6 & r5;
#else
	and	%r7, %r5		   // mask high nibbles
	inc	%r2				// next destination address
	shr	$4, %r5			// shift out low nibbles
#endif
	jne	_loop			  // pixels left
	ret