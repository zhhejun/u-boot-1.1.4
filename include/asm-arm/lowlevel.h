
#ifndef __REGISTER_LOADER_H__
#define __REGISTER_LOADER_H__

#define __INTR_REGLDR_FLAG_ABS	(1<<7)

#define RLF_PRERD	(1<<6)
#define RLF_DELAY_10X	(1<<5)
#define RLF_DELAY_100X	(1<<4)
#define RLF_DELAY_1000X	(REGLDR_FLAG_DELAY10X | REGLDR_FLAG_DELAY100X)
#define RLF_DELAY(us) (((us)&0x0FF)<<8)

#define RLF_TAG2_DELAY(tag) (((tag)>>8)&0x0FF)
#define RLF_TAG2_OFSET(tag) ((tag)>>16)

#define REGLDR_CMDTAG_MASK	0x0F

#define REGLDR_CMD_NOP	15
#define REGLDR_CMD_WRITE	0
#define REGLDR_CMD_BIT_SET	1
#define REGLDR_CMD_BIT_CLR	2
#define REGLDR_CMD_BIT_TST	3
#define REGLDR_CMD_COMPAR	4

#define REGLDR_CMD_MASK_EQ	5

#define REGLDR_CMD_NR		6

#define io_write32(v, c) (*(volatile unsigned long *)(c) = v)
#define io_read32(c)	(*(volatile unsigned long *)(c))

#endif

