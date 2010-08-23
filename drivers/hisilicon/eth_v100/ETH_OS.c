/************************************************************************
 *                                                                      *
 *                          ETH_OS.C                                  	*
 *                                                                      *
 *  Project Code:       Hi3560 ETH                                       	*
 *  Create Date:        2005/1/5                                      	*
 *  Author:             	tangbenbing                                     *
 *  Modify Date:        2005/5/5                                                *
 *  Document:                                                           *
 *  Function:                                                           *
 *  Others:                                                             *
 *----------------------------------------------------------------------*
 *                                                                      *
 * Copyright 2005-20** Hi3560 Team HuaWei Tech, Inc.  					*
 *                     ALL RIGHTS RESERVED                              *
 *                                                                      *
 *----------------------------------------------------------------------*
 ************************************************************************/

#include <common.h>

#ifdef CONFIG_HISILICON_ETH_V100

#include "ETH_TypeDef.h"
#include "ETH_Reg.h"
#include "ETH_Struct.h"
#include "ETH_FunctionList.h"
#include "Hisilicon-ETH.h"


#if 0
#include "vxWorks.h"
#include "config.h"
#include "intLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "logLib.h"
#endif

int SendTask;
int RecvTask;
int TESTTask;

//int SearchIPMACFailTask;
#define	HIGH_PRI		95		/* priority of high priority task */
#define HIGH_PRI_DIV  	90		/*priority of priority task of recv div*/
//#define HIGH_PRI_IPMAC  	98		/*priority of priority task of IPMAC form*/
#define LOW_PRI			200		/* priority of low priority task */

void ETH_RecvPacketTask (void);
void ETH_TestFrameTask (void);
void ETH_SendPacketTask (void);



static UINT32 temp_num = 1;
static UINT32 a_num = 0;

//static UINT32 c_num = 0;

UINT32 e_num = 32;
UINT32 h_num = 32;

static UINT32 g_num = 0;
static UINT32 d_queue[64];



//extern STATUS	ambaIntLvlEnable (int);
extern int sysClkRateGet (void);

extern void ETH_SetPHYExternAddress(In UINT32 u32PHYAddr)   ;
//extern void ETH_SetQueueAddress (In UINT32 u32InputQueueAddr,In UINT32 u32OutputQueueAddr) ;
extern void ETH_SetPortStatusMode(In UINT32 u32PortStatMode);
extern void ETH_SetSpeedLinkDuplexMode(In UINT32 u32Speed, In UINT32 u32Link, In UINT32 u32Duplex);
extern void ETH_SetFrameConfiguration (In UINT32 u32PreCodNo, In UINT32 u32FraCle, In UINT32 u32StrFraDist);
extern void ETH_SetInterruptEnable(In UINT32 u32InterruptEn);
extern void ETH_SetNormalInterruptStyle(In UINT32 u32InterruptNorChoi);

extern void ETH_SetPortPostFrameSend(In UINT32 u32PostFraSenEn);
extern UINT32 ETH_PhyWrite(In UINT32 u32PhyRegIndex , In UINT32 u32PhyData );
extern void ETH_SetEndian (In UINT32 u32InEndian, In UINT32 u32OutEndian);
extern void ETH_SetioutQueuelen(In UINT32 ip_len,In UINT32 ep_len);
extern void ETH_SetMACFiltCtrl(UINT32 value);
extern void eth_soft_reset(void);

//extern void ETH_changcontrol(UINT32 value1,UINT32 value2);
extern void ETH_changcontrol__(UINT32 value1);
/*========================================*/

/*=======================================================================
 *  函数名称:UINT32 ETH_InterruptHandler() 
 *  初稿完成:2005/1/18   
 *  作    者:  
 *  函数功能:中断服务程序
 *  输入参数:无
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_InterruptHandler(void)
{
	UINT32 u32InterrMod;
	UINT32 U32Interstates;
	queue_Recv_Tran_uses rtuses;
       
	
	/*disable the interrupt*/
	u32InterrMod = *((volatile UINT32 *) ETH_GLB_REG(1));
	*((volatile UINT32 *) ETH_GLB_REG(1)) = (u32InterrMod & (0x0000));
	/*clear the interrupt*/
	U32Interstates = *(volatile UINT32 *) ETH_GLB_REG(0);
	 *((volatile UINT32 *) ETH_GLB_REG(0)) = (U32Interstates  & (0xFFFF));
	/*printf("intr states  0x%x\n",U32Interstates);*/
	/*interrupt print*/
    if((U32Interstates  & 0x1)==0x1)
   	{
   	if ((*(volatile UINT32 *) ETH_GLB_REG(13)&0x01) == 0x1)
   	   {
           	Frame_Receive_Describe ReadDescribEnInf;
           	
           	ReadDescribEnInf.w32 = *(volatile UINT32 *) ETH_GLB_REG(12);       	
           	a_num= ReadDescribEnInf.bits.reseive_index;
           	g_num = (a_num+(!temp_num)*e_num);
           	d_queue[g_num] = ReadDescribEnInf.bits.fm_len;
           	*(volatile UINT32 *) ETH_GLB_REG(13) = 0;
           	if(a_num == (e_num-1))
           	  {
           	  temp_num = !temp_num;
           	  }
           	if(g_num >= e_num)
              {
                 *(volatile UINT32 *) ETH_GLB_REG(14) = (ETH_FRAM_ADDR + ((g_num-e_num)<<11));
           	  }
              else 
           	   {
           	      *(volatile UINT32 *)ETH_GLB_REG(14) = (ETH_FRAM_ADDR + ((g_num+e_num)<<11));
           	   } 
           	
           	*(UINT32 *)(0xf3f0f000) += 1;
           
           
            if(*(volatile UINT32 *)(0xf3f0f010) == 1)
              {
                *((volatile UINT32 *) ETH_GLB_REG(1)) = (u32InterrMod |0x1) ;
                return ;
              }
               //printf("queue packet use a_num is %d\n",a_num); 
             //  printf("queue packet use g_num is %d\n",g_num);

                
              rtuses.w32=(*(volatile UINT32 *) ETH_GLB_REG(18));

              while(rtuses.bits.eq_in_rdy == 0)
                {
               
              rtuses.w32=(*(volatile UINT32 *) ETH_GLB_REG(18));
                }
          
                    

                    (*(volatile UINT32 *) ETH_GLB_REG(8)) = (ETH_FRAM_ADDR + (g_num<<11));
                   	(*(volatile UINT32 *) ETH_GLB_REG(9)) = d_queue[g_num];
                   	*((volatile UINT32 *) ETH_GLB_REG(1)) = (u32InterrMod |0x1) ;
             
              
           }
        }
       	
}
    
	



/*=======================================================================
 *  函数名称:UINT32 ETH_Interrupt_Init() 
 *  初稿完成:2005/1/18   
 *  作    者:  
 *  函数功能:定义ETH中断服务入口，以及可以打开中断使能
 *  输入参数:无
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
UINT32 ETH_Init(void) 
{
        UINT32 k = 0;
#ifdef HI3560_V100
//	ETH_changcontrol(27,7);
	ETH_changcontrol__(27);
#endif
        eth_soft_reset();
	/*set the phy chip extern address as 1*/
#if defined(HI3511_DEMO)||defined(HI3560_V100) 
	ETH_SetPHYExternAddress(0x1);
#endif
#ifdef HI3511_UMAP
	ETH_SetPHYExternAddress(0x7);
#endif
	/*set input and output queue extern address(input addr, output addr)*/
	//ETH_SetQueueAddress (0xf2800000,0xf2900000) ;

	/*set little and big endian*/
        ETH_SetEndian (1, 1);
	/*set post frame enable*/
	ETH_SetPortPostFrameSend(1);
	
	/*set the port mdio fetch from which root*/

	/*set port work mode as 10mbps, link enable,full duplex*/
	ETH_SetSpeedLinkDuplexMode(0, 1, 0);
	
	/*half duplex work mode*/
	ETH_SetFrameConfiguration(0x0,92,0x1FFF);

		/*set port mode fetch from cpu*/
	ETH_SetPortStatusMode(0x1);

	/*enable the receive frame interrupt*/
	/*include the retry interrupt and ahba error*/
	//ETH_SetInterruptEnable(0x181);

	//interrupt testfunction
	ETH_SetInterruptEnable(0x0);
	ETH_SetNormalInterruptStyle(0x0);
        
       ETH_SetFrameQueueLength(h_num,e_num); 

	/*set Phy information*/
	ETH_PhyWrite(0 , 0x0);
        udelay(1000);	
        //printf("Set Phy information!!\r\n");
    /*输入输出队列使用长度配置*/
       ETH_SetMACFiltCtrl(0x120);//add mac file control


 /*启动发包*/
for(k =0;k<h_num;k++)
 *(UINT32 *) ETH_GLB_REG(14)=((UINT32)(ETH_FRAM_ADDR+(k << 11)));
 
	return Success;
}

#endif

