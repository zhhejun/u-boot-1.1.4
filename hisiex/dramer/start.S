
.global _start
_start:
	mov	sp, #0x800
	mvn	lr, #0
	ldr	r3, _main
	bx	r3

_main:
.word main

