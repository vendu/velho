.include <stdio.def>

msg:	.asciz	"hello, world\n"

.align	4

_start:
	ldr		$msg,   %r0     ; r0 = message-string address
	ldrb	*%r0, %r1   	; r1 = first character
	ldr		$STDOUT, %r2    ; r2 = file descriptor for stdout
	ldr		$NUL, %r3		; r3 = NUL-terminator
	cmp		%r3,  %r1       ; check if first character is NUL
	beq	    done        	; yes
loop:
	inc	    %r0         	; increment address
	iwrb	%r1, %r2    	; write character to stdout
	ldrb	*%r0, %r1		; read new character
	cmp		%r3, %r1        ; check if character is NUL
	bne		loop            ; no
done:
	hlt

