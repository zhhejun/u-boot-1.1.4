/************************************************************************
 *                                                                      *
 *                          ETH_TypeDef.H										*
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
 ************************************************************************/
/*Define the static data*/

#ifndef __ETH_H_
#define __ETH_H_
#define	Success	0x00000001
#define	Failure	0x00000000

#define  	In
#define  	Out
#define  	IO


//#define ETHTest
//#define Vxworks
#define Linux

#ifdef Vxworks
#define ETH_Print printf
#define ETH_Free free
#define ETH_Malloc malloc
#define HIGH_PRI_DIV_AGE 70
#define HIGH_PRI_ARP_AGE 75
#undef ETHTest

#endif

#ifdef WinCE
#undef ETHTest
#endif


#ifdef Linux
#define dbg_info(...)
#endif


#define ETH_EndianMode

/*Big or little endian mode change define*/
#ifdef ETH_EndianMode
#undef  BYTE_SWAP_32
#define BYTE_SWAP_32(data) \
((((data) & 0x000000ff) << 24) | (((data) & 0x0000ff00) << 8) | (((data) & 0x00ff0000) >> 8) | (((data) & 0xff000000) >> 24))
#undef  BYTE_SWAP_16
#define BYTE_SWAP_16(data) \
   ((((data) & 0x00FF) << 8) | (((data) & 0xFF00) >> 8))
#else
#undef  BYTE_SWAP_32
#define BYTE_SWAP_32(data) (data)
#undef  BYTE_SWAP_16
#define BYTE_SWAP_16(data) (data)
#endif


/*=========================================================*/
//    typedef unsigned char   UCHAR ;	/*8 bits*/
//    typedef unsigned short  USHORT ;	/*16 bits*/
//    typedef unsigned int    UINT ;	/*16 bits*/
//    typedef unsigned long   ULONG ;	/*32 bits*/

/*=========================================================*/
#ifndef UINT32
#define UINT32 unsigned long
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

//#ifndef UINT8
//#define UINT8 unsigned char
//#endif

/*=========================================================*/


#endif
