/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
 *
 * Be sure to mark tests to be run before relocation as such with the
 * CFG_POST_PREREL flag so that logging is done correctly if the
 * logbuffer support is enabled.
 */

#include <common.h>

#ifdef CONFIG_POST

#include <post.h>

extern int cache_post_test (int flags);
extern int watchdog_post_test (int flags);
extern int i2c_post_test (int flags);
extern int rtc_post_test (int flags);
extern int memory_post_test (int flags);
extern int cpu_post_test (int flags);
extern int uart_post_test (int flags);
extern int ether_post_test (int flags);
extern int spi_post_test (int flags);
extern int usb_post_test (int flags);
extern int spr_post_test (int flags);
extern int sysmon_post_test (int flags);
extern int dsp_post_test (int flags);
extern int codec_post_test (int flags);
extern int flash_post_test(int flags);
extern int test_e2prom(int flags);
extern int nand_post_test(int flags);

extern int sysmon_init_f (void);

extern void sysmon_reloc (void);


struct post_test post_list[] =
{
#if CONFIG_POST & CFG_POST_CACHE
    {
	"Cache test",
	"cache",
	"This test verifies the CPU cache operation.",
	POST_RAM | POST_ALWAYS,
	&cache_post_test,
	NULL,
	NULL,
	CFG_POST_CACHE
    },
#endif
#if CONFIG_POST & CFG_POST_WATCHDOG
    {
	"Watchdog timer test",
	"watchdog",
	"This test checks the watchdog timer.",
	POST_RAM | POST_POWERON | POST_SLOWTEST | POST_MANUAL | POST_REBOOT,
	&watchdog_post_test,
	NULL,
	NULL,
	CFG_POST_WATCHDOG
    },
#endif
#if CONFIG_POST & CFG_POST_I2C
    {
	"I2C test",
	"i2c",
	"This test verifies the I2C operation.",
	POST_RAM | POST_ALWAYS,
	&i2c_post_test,
	NULL,
	NULL,
	CFG_POST_I2C
    },
#endif
#if CONFIG_POST & CFG_POST_RTC
    {
	"RTC test",
	"rtc",
	"This test verifies the RTC operation.",
	POST_RAM | POST_SLOWTEST | POST_MANUAL,
	&rtc_post_test,
	NULL,
	NULL,
	CFG_POST_RTC
    },
#endif
#if CONFIG_POST & CFG_POST_MEMORY
    {
	"Memory test",
	"memory",
	"This test checks RAM.",
	POST_RAM |POST_ROM | POST_POWERON | POST_SLOWTEST | POST_PREREL | POST_MANUAL,
	&memory_post_test,
	NULL,
	NULL,
	CFG_POST_MEMORY
    },
#endif
#if CONFIG_POST & CFG_POST_CPU
    {
	"CPU test",
	"cpu",
	"This test verifies the arithmetic logic unit of"
	" CPU.",
	POST_RAM | POST_ALWAYS,
	&cpu_post_test,
	NULL,
	NULL,
	CFG_POST_CPU
    },
#endif
#if CONFIG_POST & CFG_POST_UART
    {
	"UART test",
	"uart",
	"This test verifies the UART operation.",
	POST_RAM | POST_SLOWTEST | POST_MANUAL,
	&uart_post_test,
	NULL,
	NULL,
	CFG_POST_UART
    },
#endif
#if CONFIG_POST & CFG_POST_ETHER
    {
	"ETHERNET test",
	"ethernet",
	"This test verifies the ETHERNET operation.",
	POST_RAM | POST_ALWAYS | POST_MANUAL,
	&ether_post_test,
	NULL,
	NULL,
	CFG_POST_ETHER
    },
#endif
#if CONFIG_POST & CFG_POST_SPI
    {
	"SPI test",
	"spi",
	"This test verifies the SPI operation.",
	POST_RAM | POST_ALWAYS | POST_MANUAL,
	&spi_post_test,
	NULL,
	NULL,
	CFG_POST_SPI
    },
#endif
#if CONFIG_POST & CFG_POST_USB
    {
	"USB test",
	"usb",
	"This test verifies the USB operation.",
	POST_RAM | POST_ALWAYS | POST_MANUAL,
	&usb_post_test,
	NULL,
	NULL,
	CFG_POST_USB
    },
#endif
#if CONFIG_POST & CFG_POST_SPR
    {
	"SPR test",
	"spr",
	"This test checks SPR contents.",
	POST_ROM | POST_ALWAYS | POST_PREREL,
	&spr_post_test,
	NULL,
	NULL,
	CFG_POST_SPR
    },
#endif
#if CONFIG_POST & CFG_POST_SYSMON
    {
	"SYSMON test",
	"sysmon",
	"This test monitors system hardware.",
	POST_RAM | POST_ALWAYS,
	&sysmon_post_test,
	&sysmon_init_f,
	&sysmon_reloc,
	CFG_POST_SYSMON
    },
#endif
#if CONFIG_POST & CFG_POST_DSP
    {
	"DSP test",
	"dsp",
	"This test checks any connected DSP(s).",
	POST_RAM | POST_MANUAL,
	&dsp_post_test,
	NULL,
	NULL,
	CFG_POST_DSP
    },
#endif
#if CONFIG_POST & CFG_POST_DSP
    {
	"CODEC test",
	"codec",
	"This test checks any connected codec(s).",
	POST_RAM | POST_MANUAL,
	&codec_post_test,
	NULL,
	NULL,
	CFG_POST_CODEC
    },
#endif
#if CONFIG_POST & CFG_POST_FLASH
	{
	 "FLASH test",
	 "flash",
	 "This test checks nor flash.",
	 POST_RAM | POST_ALWAYS | POST_MANUAL,
	 &flash_post_test,
	 NULL,
	 NULL,
	 CFG_POST_FLASH},
#endif
#if CONFIG_POST & CFG_POST_E2PROM
	{
	 "E2PROM test",
	 "e2prom",
	 "This test checks e2prom.",
	 POST_RAM | POST_ALWAYS | POST_MANUAL,
	 &test_e2prom,
	 NULL,
	 NULL,
	 CFG_POST_E2PROM},
#endif
#if CONFIG_POST & CFG_POST_NAND
	{
	 "NAND flash test",
	 "nand",
	 "This test checks nand flash.",
	 POST_RAM | POST_ALWAYS | POST_MANUAL,
	 &nand_post_test,
	 NULL,
	 NULL,
	 CFG_POST_NAND},
#endif

};

unsigned int post_list_size = sizeof (post_list) / sizeof (struct post_test);

#endif /* CONFIG_POST */
