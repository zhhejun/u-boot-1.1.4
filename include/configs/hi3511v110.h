/*
 * (C) Copyright 2006, HISILICON
 * Configuation settings for the HI3510 board.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <asm/arch/platform.h>

#ifndef __CONFIG_HI3560V100_H__
#define __CONFIG_HI3560V100_H__

/*-----------------------------------------------------------------
 *  System Configuration
 */

/* #define CONFIG_FPGA_VERIFY	1 */
#undef CONFIG_FPGA_VERIFY

#define CONFIG_NO_LOWLEVEL_REGLDR

#define ROM_TEXT_ADRS 		(MEM_BASE_FLASH)	
#define CFG_BOOT_PARAMS		(MEM_BASE_DDR+0x0100)

#define CONFIG_ARM926EJS	1		/* This is an arm926ejs CPU core  */
#define CONFIG_HISILICON	1		/* Hisilicon Platform Board	*/

#ifndef CFG_CLK_MPLLIN 
#define CFG_CLK_MPLLIN		(27*MHZ)
#endif

#define CFG_CLK_SLOW		(45*KHZ)

#ifndef  CFG_CLK_BUS
#define CFG_CLK_BUS		(135*MHZ)
#endif

#define CFG_CPUCLK_SCALE	2
#define CFG_CLK_CPU		(CFG_CLK_BUS*CFG_CPUCLK_SCALE)
#define CFG_DEFAULT_CLK_CPU	CFG_CLK_CPU
#define CFG_DEFAULT_CLK_BUS CFG_CLK_BUS

#define CFG_MSEC_XTALSTABLE	10
#define CFG_USEC_MPLLSTABLE	600

#ifndef REG_CONF_PLLFCTRL
#define REG_CONF_PLLFCTRL	((1<<31) | 0x1228)
#endif

#ifndef REG_CONF_SC_PERCLKEN	/* Only enable SMI/DDRC/ETH/UART1/UART0/DDRC_HCLK clocks */
#define REG_CONF_SC_PERCLKEN	0xFFFFFFFF
#endif
#define REG_CONF_SC_PERCLKDIS	(~REG_CONF_SC_PERCLKEN)

/* DDRC Config */

#define CFG_DDRC_BUSWITH	DDRC_BUSWITH_32BITS
#define CFG_DDRC_CHIPCAP	DDRC_CHIPCAP_1Gb
#define CFG_DDRC_CHIPBITS	DDRC_CHIP_16BITS
#define CFG_DDRC_CHIPBANK	DDRC_CHIP_8BANK	
#define CFG_DDRC_READDELAY_CL	DDRC_READDELAY_4	/* CAS */

/* You Must Include <asm/arch/ddrc.h> Here! */
#include <asm/arch/ddrc.h>


#define CFG_TIMERBASE           REG_BASE_TIMER12
#define CFG_HZ                  (CFG_CLK_BUS / 256)
#define CFG_TIMER_CTRL          0x8A
#define READ_TIMER		(*(volatile unsigned long *)(CFG_TIMERBASE + REG_TIMER_VALUE))

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs  */
#define CONFIG_INITRD_TAG		1	/* support initrd */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_MISC_INIT_R		1	/* call misc_init_r during start up */

#define CFG_MALLOC_LEN		(CFG_ENV_SIZE + 128*1024)
#define CFG_GBL_DATA_SIZE	128		/* size in bytes reserved for initial data */

#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(4*1024)	/* FIQ stack */
#endif

#define CFG_PL011_SERIAL
#define CONFIG_PL011_CLOCK	CFG_CLK_BUS 
#define CONFIG_PL01x_PORTS	{(void *)CFG_SERIAL2}
#define CONFIG_CONS_INDEX	0

#define CONFIG_BAUDRATE         115200
#define CFG_BAUDRATE_TABLE      { 9600, 19200, 38400, 57600, 115200 }
#define CFG_SERIAL0		REG_BASE_UART0
#define CFG_SERIAL1		REG_BASE_UART1
#define CFG_SERIAL2     REG_BASE_UART2

#define CONFIG_AUTO_COMPLETE	1
#define CFG_CMDLINE_HISTORYS    8

#define CONFIG_SILENT_CONSOLE   1

#define CONFIG_COMMANDS	(CFG_CMD_DHCP | CFG_CMD_NET | CFG_CMD_PING | CFG_CMD_BDI | CFG_CMD_MEMORY | CFG_CMD_FLASH | CFG_CMD_ENV | CFG_CMD_LOADB )
#define CONFIG_BOOTP_MASK	CONFIG_BOOTP_DEFAULT

#include <cmd_confdefs.h>

#define CONFIG_BOOTDELAY	2
#define CONFIG_ZERO_BOOTDELAY_CHECK 1

#define CONFIG_BOOTARGS "mem=128M console=ttyAMA0,115200 root=/dev/mtdblock/2 rootfstype=jffs2 mmz=ddr,1,0x680M,128M mtdparts=phys_mapped_flash:1M(boot),2M(kernel),13M(rootfs),5M(update),3M(blk),7M(app),1M(para) boardip=192.168.0.100"
#define CONFIG_BOOTCOMMAND "bootm 0x34100000"
#define CONFIG_NETMASK	255.255.255.0		/* talk on MY local net */
#define CONFIG_IPADDR	192.168.0.93		/* static IP I currently own */
#define CONFIG_SERVERIP	192.168.0.1		/* current IP of tftp server ip */
#define CONFIG_ETHADDR  00:19:21:68:11:55 
#define CONFIG_BOOTFILE	"uImage" 		/* file to load */

#define CFG_LONGHELP	
#define CFG_PROMPT	"hilinux # "		/* Monitor Command Prompt   */
#define CFG_CBSIZE	1024			/* Console I/O Buffer Size  */
#define CFG_PBSIZE	(CFG_CBSIZE+sizeof(CFG_PROMPT)+16)
#define CFG_MAXARGS	16			/* max number of command args   */
#define CFG_BARGSIZE	CFG_CBSIZE		/* Boot Argument Buffer Size    */
#undef	CFG_CLKS_IN_HZ				/* everything, incl board info, in Hz */
#define CFG_LOAD_ADDR	(CFG_DDR_PHYS_OFFSET + 0x08000)		/* default load address */


/*============ ATTENTION! YOU MAY MODIFY BELOW CONFIGS WHEN PORTING ========== */


/*-----------------------------------------------------------------------
 * Physical Memory Configuration
 */
#define CONFIG_NR_DRAM_BANKS    1		/* we have 1 bank of DRAM */
#define CFG_DDR_PHYS_OFFSET	MEM_BASE_DDR
#define CFG_DDR_SIZE		((0x800000<<CFG_DDRC_CHIPCAP) * ((16<<CFG_DDRC_BUSWITH)/(8<<CFG_DDRC_CHIPBITS)))

#define CONFIG_BOOTUP_CLEAN_RAM

#define CFG_ALT_MEMTEST 1
#define CFG_MEMTEST_START       (CFG_DDR_PHYS_OFFSET + sizeof(unsigned long))
#define CFG_MEMTEST_END         (CFG_DDR_PHYS_OFFSET + CFG_DDR_SIZE)
#define CFG_MEMTEST_SCRATCH     CFG_DDR_PHYS_OFFSET 

#ifndef REG_CONF_SC_PEREN	/* Only enable SMI/DDRC/ETH/UART1/UART0/DDRC_HCLK clocks */
#define REG_CONF_SC_PEREN	0xFFFFFFFF
#endif
#define REG_CONF_SC_PERDIS	(~REG_CONF_SC_PEREN)


/*-----------------------------------------------------------------------
 * FLASH and Environment Configuration
 */
#define CFG_FLASH_CFI
#define CFG_FLASH_CFI_DRIVER
#define CFG_FLASH_PROTECTION
#define CFG_FLASH_USE_BUFFER_WRITE

#define CFG_FLASH_BASE          MEM_BASE_FLASH

#define CFG_ENV_IS_IN_FLASH     1            /* env in flash instead of CFG_ENV_IS_NOWHERE */
#define CFG_ENV_SIZE            0x20000      /* Total Size of Environment Sector */
#define CFG_ENV_OFFSET          0x40000      /* environment starts here  */
#define CFG_ENV_ADDR            (CFG_FLASH_BASE + CFG_ENV_OFFSET)
#define CONFIG_ENV_OVERWRITE

#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	512   

/*-----------------------------------------------------------------------
 * Net Driver Configuration
 */

#define CONFIG_HISILICON_ETH_V100
/*#define CONFIG_HI3560V100_LOGO*/

#define CONFIG_CLEANUP_MEMORY	1

#define __mem_pci(c) (c)

/*-----------------------------------------------------------------------
 * NOR Flash Configuration
 */
#define CFG_FLASH_TIMING_OPT	1
#define FLASH_READ_TIMING_TOE	35	/* Output Enable to Output Delay */
#define FLASH_READ_TIMING_TCE	100	/* Chip Enable to Output Delay */
#define FLASH_READ_TIMING_TRC	100	/* Read Cycle Time */

//#define CFG_DMA_MEMCPY	1 
/*-----------------------------------------------------------------------
* DDRC TIMING
* NOTICE: YOU MAY CHANGE TIMING VALUE FOR DIFFRENT DDR CHIP 
*/

/* TIMING0 1Gb 0x22330806 */
#define  CFG_DDRC_TMRD   0x2
#define  CFG_DDRC_TRRD   0x2
#define  CFG_DDRC_TRP    0x3
#define  CFG_DDRC_TRCD   0x3
#define  CFG_DDRC_TRC    0x8
#define  CFG_DDRC_TRAS   0x6

/* TIMING1 1Gb 0xc8023412  */
#define  CFG_DDRC_TSRE   0xC8
#define  CFG_DDRC_TRL    0x2
#define  CFG_DDRC_TWL    0x3
#define  CFG_DDRC_TCL    DDR_CAS_VAL
#define  CFG_DDRC_TRFC   0x12

/* TIMING2 1Gb 0x3230703a */
#define  CFG_DDRC_TCKE   0x3
#define  CFG_DDRC_TWTR   0x2
#define  CFG_DDRC_TWR    0x3
#define  CFG_DDRC_TFAW   0x7
#define  CFG_DDRC_TAREF  0x03A

/* TIMING3 default */
#define  CFG_DDRC_TANOD_TAOFD   0
#define  CFG_DDRC_TXARD         0
#define  CFG_DDRC_TRTP          0

/* TIMING0 VALUE */
#define REG_DDR_TIMING0_VALUE  ((CFG_DDRC_TMRD << 28)		| \
								(CFG_DDRC_TRRD << 24)		| \
								(CFG_DDRC_TRP << 20)		| \
								(CFG_DDRC_TRCD << 16)		| \
								(CFG_DDRC_TRCD << 16)		| \
								(CFG_DDRC_TRC << 8)			| \
								(CFG_DDRC_TRAS << 0))
/* TIMING1 VALUE */
#define REG_DDR_TIMING1_VALUE  ((CFG_DDRC_TSRE << 24)		| \
								(CFG_DDRC_TRL << 16)		| \
								(CFG_DDRC_TWL << 12)		| \
								(DDR_CAS_VAL << 8)			| \
								(CFG_DDRC_TRFC))
/* TIMING2 VALUE */
#define REG_DDR_TIMING2_VALUE  ((CFG_DDRC_TCKE << 28)		| \
								(CFG_DDRC_TWTR << 24)		| \
								(CFG_DDRC_TWR << 20)		| \
								(CFG_DDRC_TFAW << 12)		| \
								(CFG_DDRC_TAREF))

#define REG_DDR_TIMING3_VALUE  ((CFG_DDRC_TANOD_TAOFD << 20)		| \
								(CFG_DDRC_TXARD << 8)		| \
								(CFG_DDRC_TRTP))

//// define by yaxon
#define CFG_MONITOR_BASE				    CFG_FLASH_BASE
#define CFG_FLASH_BLOCK_SIZE	            0x20000

#define HISILICON_DEMO_BOARD                0

#define HISILICON_SCTL_BASE		            REG_BASE_SCTL

#define CONFIG_HISILICON_WDT	            1  // disable watchdog by default

#if defined(CONFIG_HISILICON_WDT)
#define CONFIG_HW_WATCHDOG	                1
#endif

#define HIDOG_BOOTUP_TIMEOUT	            10
#define CONFIG_WDT_TIMEOUT	                60

#if defined(CONFIG_HISILICON_WDT)
#define CONFIG_HISILICON_WDT_EXTERN         1
#else 
#define CONFIG_HISILICON_WDT_EXTERN         0
#endif

#define HISILICON_WDT_EXT_GPIO_GROUP        3
#define HISILTCON_WDT_EXT_GPIO_BITS         5

#define CONFIG_IDENT_STRING                 " Version: uboot-110v8001-1.09" 
#define CONFIG_BOOT_RETRY_TIME	            60
#endif	/* __CONFIG_H */
