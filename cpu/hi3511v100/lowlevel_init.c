#include <config.h>
#include <common.h>
#include <linux-adapter.h>
#include <asm/arch/platform.h>

#define SYS_REGS_ADDR   REG_BASE_SCTL
#define HI3511_VER_V100 0x35110100

static unsigned int hi_getchipversion(void)
{
    unsigned int  v = 0;
    v |= 0xff & __raw_readl(0xEE0 + SYS_REGS_ADDR);
    v |= (0x0f & __raw_readl(0xEE4 + SYS_REGS_ADDR)) << 8;
    v |= (0xff & __raw_readl(0xEE8 + SYS_REGS_ADDR)) << 16;
    v |= (0xff & __raw_readl(0xEEC + SYS_REGS_ADDR)) << 24;
    return v;
}

static void _delay_cl(unsigned long cl)
{
	for(; cl!=0; cl--)
		__asm__ __volatile__("nop");
}

void cpu_init_sysctrl(void)
{
	unsigned long base = REG_BASE_SCTL;

	__raw_writel(REG_VALUE_SC_NOLOCK, base + REG_SC_PERLOCK);
#ifndef CONFIG_FPGA_VERIFY
	__raw_writel( (65536 - CFG_MSEC_XTALSTABLE*(CFG_CLK_SLOW/KHZ)) << 3, base + REG_SC_XTALCTRL);
	__raw_writel( (33554432 - (CFG_USEC_MPLLSTABLE*(CFG_CLK_MPLLIN/KHZ))/KHZ) << 3, base + REG_SC_PLLCTRL);
	__raw_writel(REG_CONF_PLLFCTRL, base + REG_SC_PLLFCTRL);

	__raw_writel(__raw_readl(base + REG_SC_CTRL) | (1<<16) | 0x7, base + REG_SC_CTRL);

	/* LED0 ON */
#if HISILICON_DEMO_BOARD > 0
	__raw_writel(0x0080, 0x101E5400);
	__raw_writel(__raw_readl(0x101E5200) & (~(0x00FF)), 0x101E5200);
#endif
	while( ((__raw_readl(base + REG_SC_CTRL)>>3)&0xF) != 0x04);
#endif

#ifdef REG_CONF_SC_PERCLKEN
	__raw_writel(REG_CONF_SC_PERCLKEN, base + REG_SC_PEREN);
#endif
#ifdef REG_CONF_SC_PERCLKDIS
	__raw_writel(REG_CONF_SC_PERCLKDIS, base + REG_SC_PERDIS);
#endif
	/*
		Hi3511 slave bug , REG_SC_PERCTRL2 should set ARM:HCLK =1:1 (bit 7 set 1)before  cancel reset.
		When ARM Entering Normal,REG_SC_PERCTRL2 should set ARM:HCLK =2:1(bit 7 set 0)
		No use to hi3511 host
	*/
	__raw_writel((__raw_readl(base + REG_SC_PERCTRL2) & (~(1 << 7)) ),base + REG_SC_PERCTRL2);
	
}

void cpu_init_smi(void)
{
#ifdef CFG_FLASH_TIMING_OPT
	unsigned long base = REG_BASE_SMI;	
	__raw_writel(CFG_CLK_BUS/((1000/FLASH_READ_TIMING_TRC)*MHZ), base + REG_SMI_BWSTRDR0);
	__raw_writel(CFG_CLK_BUS/((1000/(FLASH_READ_TIMING_TCE-FLASH_READ_TIMING_TOE))*MHZ), base + REG_SMI_BWSTOENR0);	
#endif
	/* LED1 ON */
#if HISILICON_DEMO_BOARD > 0
	__raw_writel(0x0001, 0x101E6400);
	__raw_writel(__raw_readl(0x101E6004) & (~(0x00FF)), 0x101E6004);
#endif
}

void lowlevel_memzero_8burst(unsigned long start, unsigned long end); 
#define memzero_8burst(start, end) do{ \
		__asm__ __volatile__("stmdb sp!, {r4-r12, lr}"); \
		lowlevel_memzero_8burst(start, end); \
		__asm__ __volatile__("ldmia sp!, {r4-r12, lr}"); \
	} while(0)

void cpu_init_ddrc(void)
{
	unsigned long base = REG_BASE_DDRC;

	/* For 135MHZ 200us =  28000 cycle */
	//udelay(1000);
	_delay_cl(10000);
	__raw_writel(0x00000000,base + REG_DDRC_CTRL);
	_delay_cl(10000);
	//udelay(1000);
	/* 400ns =  55 <= 60*/
	
	__raw_writel(REG_DDRC_EMRS01_CONFIG_VALUE,base + REG_DDRC_EMRS01);_delay_cl(100);
	
	/* NOCHANGE FOR DEMO */
	//__raw_writel(0x00000000,base + REG_DDRC_EMRS23);

	__raw_writel(REG_DDRC_CONF_VALUE,base + REG_DDRC_CONFIG);_delay_cl(100);

	/* DDRC_TIMING0 0x22330806 */
	__raw_writel(REG_DDR_TIMING0_VALUE,base + REG_DDRC_TIMING0);_delay_cl(100);
	
	/* DDRC_TIMING1 0xc8023412 */
	__raw_writel(REG_DDR_TIMING1_VALUE,base + REG_DDRC_TIMING1);_delay_cl(100);

	/* DDRC_TIMING2 0x3230703a */
	__raw_writel(REG_DDR_TIMING2_VALUE,base + REG_DDRC_TIMING2);_delay_cl(100);

	/* MUST KEEP THIS LINE FOR SECRET REASON FROM IP SUPPORT */
	__raw_writel(0xf200fff6, base + 0x0044);_delay_cl(100);
	
	/* DDRC_TIMING3 default value */
	//__raw_writel(REG_DDR_TIMING3_VALUE,base + REG_DDRC_TIMING3);

	/* DDRC_ODT_CONFIG */
	/* datasheet suggestion: using default value*/
	//__raw_writel(DDR_ODT_CONFIG,base + REG_DDRC_ODT_CONFIG);

	/* DDRC_DLL_CONFIG */
	/* datasheet suggestion: using default value*/
	//__raw_writel(DDR_DLL_CONFIG,base + REG_DDRC_DLL_CONFIG);

	/* QOS CH0-7 QOS */
	__raw_writel(0x000101A0, base + REG_DDRC_CH0_QOS); _delay_cl(100);
	__raw_writel(0x00010301, base + REG_DDRC_CH1_QOS); _delay_cl(100);
	__raw_writel(0x02, base + REG_DDRC_CH2_QOS); _delay_cl(100);
	__raw_writel(0x03, base + REG_DDRC_CH3_QOS); _delay_cl(100);
	__raw_writel(0x06, base + REG_DDRC_CH4_QOS); _delay_cl(100);
	__raw_writel(0x07, base + REG_DDRC_CH5_QOS); _delay_cl(100);
	__raw_writel(0x05, base + REG_DDRC_CH6_QOS); _delay_cl(100);
	__raw_writel(0x04, base + REG_DDRC_CH7_QOS); _delay_cl(100);

	/* start initial */
	__raw_writel(0x2,base + REG_DDRC_CTRL);

	while((__raw_readl(base + REG_DDRC_STATUS) & (1 << 3)) != (1 << 3));

	/* check Hi3511V100 version */
	if(HI3511_VER_V100 == hi_getchipversion()){
		 __raw_writel(0x00010405, base + REG_DDRC_CH6_QOS); _delay_cl(100);	
		 __raw_writel(0x00010702, base + REG_DDRC_CH2_QOS); _delay_cl(100);	
		 __raw_writel(0x00010703, base + REG_DDRC_CH3_QOS); _delay_cl(100);	
	}

	/* LED0 OFF */
#if HISILICON_DEMO_BOARD > 0
	__raw_writel(0x0080, 0x101E5400);
	__raw_writel(0x00FF, 0x101E5200);
#endif

#ifdef CONFIG_BOOTUP_CLEAN_RAM
	memzero_8burst(CFG_DDR_PHYS_OFFSET, CFG_DDR_PHYS_OFFSET + CFG_DDR_SIZE);
#endif

	/* LED1 OFF */
#if HISILICON_DEMO_BOARD > 0
	__raw_writel(0x0001, 0x101E6400);
	__raw_writel(0x00FF, 0x101E6004);
#endif

}

#ifdef CONFIG_RANDOM_ETHADDR
int random_init_r(void)
{
	unsigned long seed;
	unsigned long *base = MEM_BASE_ITCM;
	unsigned int size = ((MEM_BASE_ITCM + MEM_SIZE_ITCM) >> 2);
	for(; base < size; base++){
		seed ^= *base;
	}
	srandom(seed);
	return 0;
}
#endif
