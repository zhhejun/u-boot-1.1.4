/************************************************************************
 *                                                                      *
 *                          ETH_reg.H	                                *
 *                                                                      *
 *  Project Code:       Hi3560 ETH                                     *
 *  Create Date:        2005/01/03                                      *
 *  Author:             TANGBENBING                                     *
 *  Modify Date:                                                        *
 *  Document:                                                 			*
 *  Function:                                                           *
 *  Others:                                                             *
 *----------------------------------------------------------------------*
 *                                                                      *
 * Copyright 2004-2005 Hi3560 Team Shenzhen  HuaWei Tech, Inc.   		*
 *                     ALL RIGHTS RESERVED                              *
 *                                                                      *
 *----------------------------------------------------------------------*
 *                                                                      *
 *    1 define the register address			                            *
 *    2	this file is used by the ETH_API.c file                          *
 *                                                                      *
 ************************************************************************/

#ifndef ETH_REG_H
#define ETH_REG_H


 #ifdef Vxworks
#include "vxWorks.h"
#include "config.h"
#include "intLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "logLib.h"


SEM_ID	semId_Nor;
SEM_ID semId_Test;
SEM_ID semId_Transmit;	
#endif
/* semaphore ID */
//SEM_ID 		semId_IPMAC;
/*define cpu receive packets save address*/

 /*=========define for task==========*/

#define ETH_ReceiveNoOnce	400000

extern unsigned long ETH_FRAM_ADDR;

/*Define the opposite addr. for the switch fabric registers*/

#define ETH_Reg_Base_Addr	0x90030000

#define	ETH_Statis	(0x0300 + ETH_Reg_Base_Addr)	/*0x300~0x3FF*/

#define	ETH_FLT_TAB( n ) 	(0x0000 + ETH_Reg_Base_Addr + n*4*3)	/*The range of n is 0 to 20:x000бл0x0FC*/
#if 0
#define	ETH_FLT_TAB0 	(0x0000 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB1 	(0x0004 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB2 	(0x000c + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB3 	(0x0010 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB4 	(0x0018 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB5 	(0x001c + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB6 	(0x0024 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB7 	(0x0028 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB8 	(0x0030 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB9 	(0x0034 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB10 	(0x003c + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB11 	(0x0040 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB12 	(0x0048 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB13 	(0x004c + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB14 	(0x0054 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB15 	(0x0058 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB16 	(0x0060 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB17 	(0x0064 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB18 	(0x006c + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB19 	(0x0070 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#define	ETH_FLT_TAB20 	(0x0078 + ETH_Reg_Base_Addr )	/*The range of n is 0 to 20:x000бл0x0FC*/
#endif

 #define	ETH_GLB_REG( n )	(0x0200 + ETH_Reg_Base_Addr + n*4)	/*The range of n is 0 to 18*/

 #define	ETH_MAC0_REG( n )	(0x01C0 + ETH_Reg_Base_Addr + n*4)	/*The range of n is 0 to 3:0x1C0~0x1CC*/

#define	ETH_MAC_REG( n )	(0x01A0 + ETH_Reg_Base_Addr + n*4)	/*The range of n is 0 to 5:0x1A0~0x1BC*/

#define	ETH_MDIO_REG( n )	(0x0180 + ETH_Reg_Base_Addr + n*4)	/*The range of n is 0 to 4:0x180~0x19C*/
 
#endif
