/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * (C) Copyright 2003
 * Texas Instruments, <www.ti.com>
 * Kshitij Gupta <Kshitij@ti.com>
 *
 * (C) Copyright 2004
 * ARM Ltd.
 * Philippe Robin, <philippe.robin@arm.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <linux-adapter.h>
#include <asm/arch/platform.h>

void peripheral_power_enable (void);

#if defined(CONFIG_SHOW_BOOT_PROGRESS)
void show_boot_progress(int progress)
{
    printf("Boot reached stage %d\n", progress);
}
#endif

#define COMP_MODE_ENABLE ((unsigned int)0x0000EAEF)

static inline void delay (unsigned long loops)
{
	__asm__ volatile ("1:\n"
		"subs %0, %1, #1\n"
		"bne 1b":"=r" (loops):"0" (loops));
}

/*
* GPIO Initial for Hi3511v100 DMEB
* GPIO configure for different board
	GPIO6_0				SPI			I
	GPIO6_1				SPI			O
	GPIO7_6				SPI			I
	GPIO7_5				SPI			I
	GPIO1_7/GPIO2_0		PCI_REQ/GNT	O
	GPIO0_5/GPIO0_6		UART2		I
	GPIO2_7				UCTSN1		O 
	GPIO2_5/GPIO2_4		UART1		I
	GPIO1_1				EBICS1		O
	GPIO3_4				IIC_CLK		O
	GPIO3_3				IIC_SDA		IO
*/
void gpio_init(void)
{
	unsigned int gpio_ctrl = 0;
	unsigned int reg = 0;
/*
	sys control register gpio
*/
/* HI3511 DEMO BOARD
Offset Address
0x020	Register Name
SC_PERCTRL1	Total Reset Value
[31]	RW	pinmuxctrl31	SIO0输入的发送帧指示复用控制。0：从管脚SIO0XFS。
[30]	RW	pinmuxctrl30	SIO0输入的发送时钟复用控制。0：从管脚SIO0XCK。
[29:25]	-	reserved	保留。
[24]	RW	pinmuxctrl24	以太网半双工管脚ERXERR/ECRS/ECOL复用控制。0：以太网接口管脚。
[23]	RW	pinmuxctrl23	VI1接口和GPIO复用控制。0：VI1接口。
[22]	-	reserved	保留
[21]	RW	pinmuxctrl21	控制EBI接口中片选信号EBIADR24和GPIO的复用控制。0：EBI接口的地址线。
[20]	RW	pinmuxctrl20	控制EBI接口中片选信号EBICS1N和GPIO的复用控制。0：GPIO1_1。
[19]	RW	pinmuxctrl19	芯片输出可编程时钟管脚ACKOUT管脚（可用于音频芯片工作时钟）复用控制。1：可编程时钟输出。
[18]	RW	pinmuxctrl18	SIO0的SIO0XFS管脚和GPIO的管脚复用控制。1：SIO0XFS。
[17]	RW	pinmuxctrl17	IRRCV和GPIO的管脚复用控制。0：IRRCV。
[16]	RW	pinmuxctrl16	UART1接口复用控制。1：UART1 URXD1/UTXD1/URTSN1/UCTSN1信号。
[15]	RW	pinmuxctrl15	DDR的绕线信号的管脚复用控制。1：DDR的绕线信号DDRMRCVI和DDRMRCVO。
[14]	RW	pinmuxctrl14	控制I2C信号和GPIO复用控制。1：GPIO3_3和GPIO3_4。
[13]	RW	pinmuxctrl13	PCI的PCIREQ4和PCIGRANT4与GPIO复用控制。0：GPIO1_7和GPIO2_0。
[12]	RW	pinmuxctrl12	SPI接口中片选的译码控制。0：SPICSN0。
[11]	RW	pinmuxctrl11	1：SIO0XCK。
[10]	RW	pinmuxctrl10	GPIO和SPI的管脚复用控制。0：GPIO6_0、GPIO7_6、GPIO6_1和GPIO7_5。
[9]	RW	pinmuxctrl9	1：MMC。
[8:7]	RW	pinmuxctrl8-7	VI2接口的同步信号、GPIO、和高清模式下的VI输入的管脚复用控制。10: UART。//00：GPIO0_5和GPIO0_6。
[6]	RW	pinmuxctrl6	0：VI3接口。
[5]	RW	pinmuxctrl5	1
[4]	RW	pinmuxctrl4	0
[3]	RW	pinmuxctrl3	0
[2]	RW	pinmuxctrl2	0
[1:0]	RW	pinmuxctrl1-0	01
*/
	gpio_ctrl = 0x000DCB21;
 /* 	gpio_ctrl = 0x000DCB01;
	gpio_ctrl &= ~(1 << 0);
	gpio_ctrl &= ~(1 << 9);
	gpio_ctrl &= ~(1 << 10);
	gpio_ctrl &= ~(1 << 11);
	gpio_ctrl &= ~(1 << 14);
	gpio_ctrl |=  (1 << 17);
	gpio_ctrl &= ~(1 << 18);
*/
	gpio_ctrl &= ~(1 << 19);
//	gpio_ctrl |=  (1 << 23);
	

	/*added by  daoguang Wu*/
	__raw_writel(gpio_ctrl, REG_BASE_SCTL + REG_SC_PERCTRL1);
	reg = __raw_readl(REG_BASE_SCTL + REG_SC_PERCTRL3);
	reg = (reg & 0xFF000000) | (0x24000000);
	reg |= ((4096000ULL * (1 << 27)) / (CFG_CLK_BUS * 4));/* K = Fsio * (1 << 27) / (Fpll)   */
	__raw_writel(reg , REG_BASE_SCTL + REG_SC_PERCTRL3);

	reg = __raw_readl(REG_BASE_SCTL + REG_SC_PERCTRL4);
	reg |= 0x80;
	__raw_writel(reg , REG_BASE_SCTL + REG_SC_PERCTRL4);

}

/*
 * Miscellaneous platform dependent initialisations
 */
int board_init (void)
{
	DECLARE_GLOBAL_DATA_PTR;
	unsigned long t;

	gd->bus_clk = CFG_CLK_BUS;
	gd->cpu_clk = CFG_CLK_CPU;

	/* enable timer 0_1 */
	t = __raw_readl(REG_BASE_SCTL) | (1<<16);
	__raw_writel(t, REG_BASE_SCTL);

	gd->bd->bi_arch_number = MACH_TYPE_VERSATILE_PB;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = CFG_BOOT_PARAMS;

	gd->flags = 0;

	icache_enable ();

	__raw_writel(0, CFG_TIMERBASE + REG_TIMER_CONTROL);
	__raw_writel(~0, CFG_TIMERBASE + REG_TIMER_RELOAD);
	__raw_writel(CFG_TIMER_CTRL, CFG_TIMERBASE + REG_TIMER_CONTROL);
	__raw_writel(__raw_readl(REG_BASE_SCTL + REG_SC_CTRL) | (1<<16), REG_BASE_SCTL + REG_SC_CTRL);

	t = __raw_readl(REG_GPIO_DIR+REG_GPIO7_BASE) | (1<<4);
	__raw_writel(t, REG_GPIO_DIR +REG_GPIO7_BASE);

	__raw_writel(0, (1<<6) +REG_GPIO7_BASE);
	/* gpio init */
	gpio_init();

	return 0;
}

void init_eth_mac_env(void);

int misc_init_r (void)
{
#ifdef CONFIG_RANDOM_ETHADDR
	random_init_r();
#endif
	init_eth_mac_env();

	setenv("verify", "n");

	return (0);
}

int dram_init (void)
{
	  DECLARE_GLOBAL_DATA_PTR;

          gd->bd->bi_dram[0].start = CFG_DDR_PHYS_OFFSET;
          gd->bd->bi_dram[0].size = CFG_DDR_SIZE;

	return 0;
}

