
#include <asm/arch/platform.h>

#ifndef __ASM_ARCH_GPIO_H
#define __ASM_ARCH_GPIO_H

/* @g: gpio group, range 0-7 
 * return gpio group's base address in virtual.
 * */
#define HISILICON_GPIO_GBASE(g) IO_ADDRESS(((g)>3)?(HISILICON_GPIO4_7_BASE+0x1000*((g)-4)):(HISILICON_GPIO0_3_BASE+0x1000*(g)))

/* return gpio group's direction mask, ouput when certain bit is '1' */
#define HISILICON_GPIO_GET_DIR(g) (*(volatile unsigned long*)(HISILICON_GPIO_GBASE(g)+0x400))

/* @dir: group's direction, set to output if the bit is '1' */
#define HISILICON_GPIO_SET_DIR(dir,g) do{ *(volatile unsigned long*)(HISILICON_GPIO_GBASE(g)+0x400) = dir; }while(0)

/* @b: bits, range 0-7 
 * return '1' if specific bit is output 
 * */
#define HISILICON_GPIO_GET_BITDIR(g,b) ((HISILICON_GPIO_GET_DIR(g)>>(b))&0x01)

/* set bit direction
 * set the direction of gpio group 'g', bit 'b'
 * */
#define HISILICON_GPIO_SET_BITDIR(dir,g,b) HISILICON_GPIO_SET_DIR(((HISILICON_GPIO_GET_DIR(g)&(~(1<<(b)))) | ((dir)<<(b))), g)

/* read specific bits 
 * @mask: specific the bits you want to read 
 * */
#define HISILICON_GPIO_READ(g,mask) (*(volatile unsigned long*)(HISILICON_GPIO_GBASE(g)+((unsigned long)(mask)<<2)))

/* read specific bit */
#define HISILICON_GPIO_RDBIT(g,b) HISILICON_GPIO_READ(g,1<<(b))

/* write specific bits */
/* write (v & mask) to a group */
#define HISILICON_GPIO_WRITE(v,g,mask) do{ *(volatile unsigned long*)(HISILICON_GPIO_GBASE(g)+((unsigned long)(mask)<<2)) = v; }while(0)

/* write specific bit */
#define HISILICON_GPIO_WRBIT(v,g,b) HISILICON_GPIO_WRITE((v)<<(b),g,1<<(b))

#endif

