
#include <asm/sizes.h>

#define P15_CTRL_RR	(1<<14)
#define P15_CTRL_IC	(1<<12)
#define P15_CTRL_DC	(1<<2)
#define P15_CTRL_MMU	(1<<0)

.global flush_cache_off
flush_cache_off:
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #P15_CTRL_IC
	bic	r0, r0, #P15_CTRL_DC
	mcr	p15, 0, r0, c1, c0, 0

	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0
	mcr	p15, 0, r0, c7, c10, 4	@ drain write buffer
	mcr	p15, 0, r0, c8, c7, 0	@ flush I,D TLBs

	bx	lr


#define MMU_SEC_DESC	( (3<<10) | (0<<5) | (1<<4) | (2<<0) )
#define MMU_SEC_CB	( (1<<3) | (1<<2) )	/* cached, write back */
#define MMU_SEC_NCB	~( (1<<3) | (1<<2) )	/* no cached */

.global mmu_turnoff
mmu_turnoff:	
	1: mrc	p15, 0, pc, c7, c14, 3
	bne	1b			@ test and clean D-cache
	
	mrc	p15, 0, r3, c1, c0, 0	@ read control reg
	bic	r3, r3, #P15_CTRL_DC
	bic	r3, r3, #P15_CTRL_MMU
	bic	r3, r3, #P15_CTRL_IC
	mcr	p15, 0, r3, c1, c0, 0	@ load control register
	
	bx	lr

@r0: page directory address, must align to 16KB
.global mmu_pagedir_init
mmu_pagedir_init:
	add	r3, r0, #SZ_16K
	ldr	r2, =MMU_SEC_DESC
1:
	str	r2, [r0], #4		@ 1:1 mapping
	add	r2, r2, #SZ_1M
	teq	r0, r3
	bne	1b

	bx	lr

@r0: page directory address, must align to 16KB
@r1: start of cached MEM,    must align to 1MB
@r2:  size of cached MEM,    must align to 1MB
.global mmu_pagedir_cached_range
mmu_pagedir_cached_range:
	add	r1, r0, r1, lsr #18
	add	r2, r1, r2, lsr #18

1:
	ldr	r0, [r1]
	orr	r0, #MMU_SEC_CB
	str	r0, [r1], #4

	cmp	r1, r2
	blt	1b

	bx	lr
	
@r0: page directory address, must align to 16KB
@r1: start of cached MEM,    must align to 1MB
@r2:  size of cached MEM,    must align to 1MB
.global mmu_pagedir_nocached_range
mmu_pagedir_nocached_range:		
clean_loop:
	mrc	p15, 0, r15, c7, c14, 3
	bne	clean_loop		@ test and clean D-cache
	
	mov	r3, #0
	mcr	p15, 0, r3, c8, c7, 0	@ flush I,D TLBs

	add	r1, r0, r1, lsr #18
	add	r2, r1, r2, lsr #18

1:
	ldr	r0, [r1]
	and	r0, #MMU_SEC_NCB
	str	r0, [r1], #4

	cmp	r1, r2
	blt	1b
	
	bx	lr	

@r0: page directory address, must align to 16KB
.global mmu_startup
mmu_startup:
	stmdb	sp!, {r0, lr}
	bl	flush_cache_off
	ldmia	sp!, {r0, lr}

	mrc	p15, 0, r3, c1, c0, 0	@ read control reg
	orr	r3, r3, #P15_CTRL_IC		@ I-cache enable
	bic	r3, r3, #P15_CTRL_RR
	orr	r3, r3, #(P15_CTRL_DC | P15_CTRL_MMU)

	mov	r2, #0
	mov	r1, #-1

	mcr	p15, 0, r0, c2, c0, 0	@ load page table pointer
	mcr	p15, 0, r1, c3, c0, 0	@ load domain access control
	mcr	p15, 0, r3, c1, c0, 0	@ load control register
	mcr	p15, 0, r2, c8, c7, 0	@ flush I,D TLBs

	bx	lr



.global dcache_stop
dcache_stop:
	
	1: mrc	p15, 0, pc, c7, c14, 3
	bne	1b			@ test and clean D-cache
	
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #P15_CTRL_DC
	mcr	p15, 0, r0, c1, c0, 0
	
	bx	lr
	
.global dcache_start
dcache_start:	
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #P15_CTRL_DC
	mcr	p15, 0, r0, c1, c0, 0
	
	bx	lr
	
.global dcache_stop_noflush
dcache_stop_noflush:
	
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #P15_CTRL_DC
	mcr	p15, 0, r0, c1, c0, 0
	
	bx	lr	
	


