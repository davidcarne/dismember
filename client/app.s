.global _start

_start:
	mov	r0, #1
1:	cmp	r0, #0
	blne	other
	b	1b
	mov r0, r3
other:
	mov	r0, #0
	mov	pc, lr
