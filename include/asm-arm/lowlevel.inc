

#include <asm/lowlevel.h>

#ifndef __REGISTER_LOADER_INC__
#define __REGISTER_LOADER_INC__

#define INITDATA_BEGIN(name, base) __INITDATA_BEGIN__##name: .word (base)

.macro	__initdata_end, name
	.word ~0x00
.endm

#define INITDATA_END(name) __initdata_end name

.macro load_initdata, name
	adr r0, __INITDATA_BEGIN__\name
	bl  lowlevel_regldr
.endm

.macro initdata1, tag
	.word (\tag)
.endm

.macro initdata2, tag, val
	.word (\tag)
	.word (\val)
.endm

.macro initdata3, tag, va, vb
	initdata2 (\tag), (\va)
	.word (\vb)
.endm

.macro initdata4, tag, va, vb, vc
	initdata3 (\tag), (\va), (\vb)
	.word (\vc)
.endm


#define REGLDR_OFS_TAG(ofs, flags, cmd)	(((ofs)<<16) | (((flags)&(0x0FFFF &(~REGLDR_CMDTAG_MASK))) & (~__INTR_REGLDR_FLAG_ABS)) | ((cmd)&REGLDR_CMDTAG_MASK))
#define REGLDR_ABS_TAG(flags, cmd)	(REGLDR_OFS_TAG(0, flags, cmd) | __INTR_REGLDR_FLAG_ABS)

#define REGLDR_OFS_RO(ofs, flags)	initdata1 REGLDR_OFS_TAG(ofs, (flags) | RLF_PRERD, REGLDR_CMD_NOP)
#define REGLDR_OFS_WR(ofs, data, flags)	initdata2 REGLDR_OFS_TAG(ofs, flags, REGLDR_CMD_WRITE), (data)
#define REGLDR_OFS_BITSET(ofs, data, flags)	initdata2 REGLDR_OFS_TAG(ofs, flags, REGLDR_CMD_BIT_SET), (data)
#define REGLDR_OFS_BITCLR(ofs, data, flags)	initdata2 REGLDR_OFS_TAG(ofs, flags, REGLDR_CMD_BIT_CLR), (data)
#define REGLDR_OFS_BITTST(ofs, data, flags)	initdata2 REGLDR_OFS_TAG(ofs, flags, REGLDR_CMD_BIT_TST), (data)
#define REGLDR_OFS_COMPAR(ofs, data, flags)	initdata2 REGLDR_OFS_TAG(ofs, flags, REGLDR_CMD_BIT_COMPAR), (data)
#define REGLDR_OFS_MASKEQ(ofs, mask, data, flags)	initdata3 REGLDR_OFS_TAG(ofs, flags, REGLDR_CMD_MASK_EQ), (mask), (data)

#define REGLDR_ABS_RO(abs, flags)	initdata2 REGLDR_ABS_TAG((flags) | RLF_PRERD, REGLDR_CMD_NOP), (abs)
#define REGLDR_ABS_WR(abs, data, flags)	initdata3 REGLDR_ABS_TAG(flags, REGLDR_CMD_WRITE), (abs), (data)
#define REGLDR_ABS_BITSET(abs, data, flags)	initdata3 REGLDR_ABS_TAG(flags, REGLDR_CMD_SET), (abs), (data)
#define REGLDR_ABS_BITCLR(abs, data, flags)	initdata3 REGLDR_ABS_TAG(flags, REGLDR_CMD_CLR), (abs), (data)
#define REGLDR_ABS_BITTST(abs, data, flags)	initdata3 REGLDR_ABS_TAG(flags, REGLDR_CMD_TST), (abs), (data)
#define REGLDR_ABS_COMPAR(abs, data, flags)	initdata3 REGLDR_ABS_TAG(flags, REGLDR_CMD_COMPAR), (abs), (data)
#define REGLDR_ABS_MASKEQ(abs, mask, data, flags)	initdata4 REGLDR_ABS_TAG(flags, REGLDR_CMD_MASK_EQ), (abs), (mask), (data)

#endif

