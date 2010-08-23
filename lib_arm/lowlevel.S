
#include <config.h>

#ifndef CONFIG_NO_LOWLEVEL_REGLDR

#include <asm/lowlevel.h>

.macro	delay_1us, r
	mov	\r,	#(CFG_DEFAULT_CLK_CPU/1000000/4)
	1: subs	\r,	#1
	bcs	1b
.endm

.macro M_lowlevel_regldr, data, base, addr, delay, tag, tmp, d0, d1, ret
	ldr	\base,	[\data], #4	@load register base address

_main_loop:
	ldr	\tag,	[\data], #4	@load section tag
	cmn	\tag,	#1		@if tag==~0x0, means the end
	moveq	pc,	\ret

	mov	\delay,	\tag, lsr #8	@get delay
	and	\delay,	\delay,	#0xFF
	
	tst	\tag,	#__INTR_REGLDR_FLAG_ABS	@check address type
	ldrne	\addr,	[\data], #4
	addeq	\addr,	\base,	\tag, lsr #16

	tst	\tag,	#RLF_PRERD	@do pre-read
	ldrne	\tmp,	[\addr]

	mov	\tmp,	#10
	tst	\tag,	#RLF_DELAY_10X	@delay*10
	mulne	\delay,	\tmp,	\delay

	tst	\tag,	#RLF_DELAY_100X	@delay*100
	mulne	\delay,	\tmp,	\delay
	mulne	\delay,	\tmp,	\delay

	and	\tag,	#0xF	@tag&0xF, get the command

	cmp	\tag,	#REGLDR_CMD_NR		@check if command is valid
	bge	_loop_delay	@for nop options

	ldr	\d0,	[\data], #4	@pre-load data

	add	pc,	pc,	\tag, lsl #2
	nop	@for stuff

_switch_table:
	bl	_do_write
	bl	_do_bitset
	bl	_do_bitclr
	bl	_do_bittst
	bl	_do_compar
	bl	_do_maskeq

_do_bitset:
	ldr	\tmp,	[\addr]
	orr	\d0,	\tmp
	b	_do_write

_do_bitclr:
	ldr	\tmp,	[\addr]
	mvn	\d0,	\d0
	and	\d0,	\tmp
	b	_do_write

_do_bittst:
	ldr	\tmp,	[\addr]
	and	\tmp,	\d0
	cmp	\tmp,	\d0
	b	_do_tst_wait

_do_compar:
	ldr	\tmp,	[\addr]
	cmp	\tmp,	\d0
	b	_do_tst_wait


_do_maskeq:
	ldr	\d1,	[\data], #4
__do_maskeq:
	ldr	\tmp,	[\addr]
	and	\tmp,	\tmp,	\d0
	cmp	\tmp,	\d1
	adr	lr,	__do_maskeq
	b	_do_tst_wait

_do_tst_wait:
	beq	_main_loop
	delay_1us \tmp
	subs	\delay,	#1
	mov	pc,	lr
	b	_main_loop


_do_write:
	str	\d0,	[\addr]


_loop_delay:
	subs	\delay,	#1
	bcc	_main_loop
	delay_1us \tmp
	b	_loop_delay

.endm


@r0: initdata address
.global lowlevel_regldr
lowlevel_regldr:
	mov	r8,	lr
	M_lowlevel_regldr r0, r1, r2, r3, r4, r5, r6, r7, r8

#endif

@r0: start
@r1: end
.global lowlevel_memzero_8burst
lowlevel_memzero_8burst:
	mov	r3, #0
	mov	r4, #0
	mov	r5, #0
	mov	r6, #0
	mov	r7, #0
	mov	r8, #0
	mov	r9, #0
	mov	r10, #0

	1:
	stmia	r0!, {r3-r10}
	cmp	r0, r1
	blt	1b

	mov	pc, lr

@r0: start
@r1: dest
@r2: length
.global lowlevel_memcpy_8burst
lowlevel_memcpy_8burst:
	add	r2,  r0
	1:
	ldmia	r0!, {r3-r10}
	stmia	r1!, {r3-r10}
	cmp	r0,  r2
	blt	1b

	mov	pc, lr


