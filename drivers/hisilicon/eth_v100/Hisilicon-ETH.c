/*
 * Cirrus Logic CS8900A Ethernet
 *
 * (C) 2003 Wolfgang Denk, wd@denx.de
 *     Extension to synchronize ethaddr environment variable
 *     against value in EEPROM
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * Copyright (C) 1999 Ben Williamson <benw@pobox.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is loaded into SRAM in bootstrap mode, where it waits
 * for commands on UART1 to read and write memory, jump to code etc.
 * A design goal for this program is to be entirely independent of the
 * target board.  Anything with a CL-PS7111 or EP7211 should be able to run
 * this code in bootstrap mode.  All the board specifics can be handled on
 * the host.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <config.h>
#include <common.h>
#include <command.h>

#ifdef CONFIG_HISILICON_ETH_V100

#include "ETH_TypeDef.h"
#include "ETH_Reg.h"
#include "ETH_Struct.h"
#include "Hisilicon-ETH.h"
#include <net.h>
#include <linux-adapter.h>
extern UINT32 ETH_FrameTransmit (In UINT16 u16FrameLen, In UINT8 *pu8FrameTransmitData) ;
extern UINT32 ETH_FrameReceive (Out UINT8 *pu8FrameReceiveData ,Out UINT16 *pu16Length) ;
extern void ETH_SET_MAC(unsigned char*mac);
extern UINT32 ETH_Init(void);

#if (CONFIG_COMMANDS & CFG_CMD_NET)

#undef DEBUG

#define MAC_LEN 6

#define print_mac(mac) 	do{ int i;\
	printf("MAC:  ");\
	for (i = 0; i < MAC_LEN; i++)\
		printf("%c%02X", i ? '-' : ' ', *(((unsigned char*)mac)+i));\
	printf("\n");\
	}while(0)

int string_to_mac(unsigned char *mac, char* s)
{
	int i;
	char *e;
	int ret = 0;

	if(s == NULL)
		return 1;

	for (i=0; i<MAC_LEN; ++i) {
		mac[i] = simple_strtoul(s, &e, 16);
		if (s) {
			s = (*e) ? e+1 : e;
		}

		if((*(((unsigned char*)mac)+i) == 0)){
			ret += 1;
		}
	}

	return (ret == MAC_LEN?1:0);
}

#ifdef CONFIG_RANDOM_ETHADDR
void set_random_mac(unsigned char *mac)
{
	unsigned char ethaddr[20];
	union{
		unsigned long rand;
		unsigned char mac[4];
	}rand;

	rand.rand = random();
	sprintf (ethaddr, "%02X:%02X:%02X:%02X:%02X:%02X",
		0x00,0x50,rand.mac[0],rand.mac[1],rand.mac[2],rand.mac[3]);

	printf("RANDOM MAC %s\n",ethaddr);
	setenv("ethaddr",ethaddr);
	* mac++  = 0x00;
	* mac++  = 0x50;
	memcpy((void *)mac,(void *) rand.mac, MAC_LEN - 2);
}
#endif

void init_eth_mac_env(void)
{
	unsigned char mac[MAC_LEN];
	char* s=getenv("ethaddr");

	memset(mac,0,MAC_LEN);

	if(string_to_mac(mac,s)){
#ifdef CONFIG_RANDOM_ETHADDR
		set_random_mac(mac);
#endif
	}

	print_mac(mac);
	ETH_SET_MAC(mac);
}




	 

void ETH_SoftResetPort(In UINT32 u32PortReset)
{
}

/* Hi3511 write 0 reset, Hi3560 write 1 reset ...faint */
static void eth_reset (void)
{
	writel( readl(REG_BASE_SC+REG_SC_PERCTRL3) & (~(1<<12)), REG_BASE_SC+REG_SC_PERCTRL3);
	udelay(100);
	writel( readl(REG_BASE_SC+0x01C) | (1<<12), REG_BASE_SC+0x01C);
}

static void eth_reginit (void)
{
     UINT32 ulIntEn = 0;

		/*disable the interrupt*/
	ulIntEn = *((volatile UINT32 *) ETH_GLB_REG(1));
	*((volatile UINT32 *) ETH_GLB_REG(1)) = (ulIntEn & (0x0000));
	

}


void eth_halt (void)
{
	writel(1, ETH_Reg_Base_Addr + 0x01A4);
	writel(0, ETH_Reg_Base_Addr + 0x01AC);
}

extern void	hisf_set_mac_addr( char* addr );

extern unsigned long _bss_end;
unsigned long ETH_FRAM_ADDR=0;

int eth_init (bd_t * bd)
{
	ETH_FRAM_ADDR = (_bss_end + 0x1000)&(~0xFFF);

	dbg_info("%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);

	printf("BUF:   0x%08lX\n", ETH_FRAM_ADDR);

	eth_reset();

	ETH_Init();
	init_eth_mac_env();

	return 0;
}

/* Get a data block via Ethernet */
extern int eth_rx (void)
{
	UINT16 rxlen=0;

	dbg_info("%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);

       if( ETH_FrameReceive((UINT8 *)NetRxPackets[0],&rxlen) ==1 )
	/* Pass the packet up to the protocol layers. */
	NetReceive (NetRxPackets[0], rxlen);

	return rxlen;
}

/* Send a data block via Ethernet. */
extern int eth_send (volatile void *packet, int length)
{
//	int i =0;
	
	dbg_info("%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);

	ETH_FrameTransmit((UINT16)length+4,(UINT8 *)packet);	//add 4 as crc check
	return 0;
}

#endif	/* COMMANDS & CFG_NET */

#endif

