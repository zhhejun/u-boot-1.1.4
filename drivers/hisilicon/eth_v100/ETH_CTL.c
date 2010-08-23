/************************************************************************
 *                                                                      *
 *                          ETH_CTL.C                                  	*
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

//#include "stdio.h"
//#include "stdlib.h"

#include <config.h>

#ifdef CONFIG_HISILICON_ETH_V100

#include <linux-adapter.h>
#include "ETH_TypeDef.h"
#include "ETH_Reg.h"
#include "ETH_Struct.h"

#define BYTE char
#define DWORD long int




extern UINT32 e_num;
extern UINT32 h_num;

/*=======================================================================*/
/*=======================================================================*/
/*=====calculate the CRC value used 32 bit width===================================*/
/*=======================================================================*/
/*=======================================================================*/

//以下是CRC32算法函数计算模式定义
const BYTE CALC_CRC_NORMAL         = 1;     //正常计算模式
const BYTE CALC_CRC_MULTI_CONTINUE = 2;     //多次计算模式，未结束标志
const BYTE CALC_CRC_MULTI_END      = 3;     //多次计算模式，结束标志

/**************************************************************************
函数名称： Calc_CRC32( BYTE * Packet, DWORD dwLength, BYTE byCalcMode = 
CALC_CRC_NORMAL )
函数说明：计算CRC32校验字
函数调用：
函数被调用：
函数输入：包地址、包长度、计算模式
函数输出：无
函数返回： CRC32校验字
其他说明：该函数的主要处理为查CRC32计算表以便得到CRC32校验字。该计算表是由
      另外一个按照CRC-32标准的计算方法和种子数的函数计算得出，函数由马剑涛
      编写，这两个函数已经充分验证，保证其正确性和符合CRC-32的标准。
              
      性能测试结果：
      在奔腾3 代677MHz计算机上实际运行，计算200M内存数据的CRC32校验字
      的时间约为2秒，即20K数据的计算时间为0.2毫秒，可以满足主机软件和单
      板软件的计算性能要求。
      同时支持多次计算生成一个CRC32校验字的计算模式，以便支持单板软件低
      内存占用的要求。如把一个20k的数据包分成10次、每次2k的数据进行计算，
      最后得到一个CRC32校验字（和一次计算得到的校验字完全一样）。
              
修改记录：  
<修改人>          <修改时间>     <问题单号>      <修改描述>
马剑涛/25593     2003年3月18日                Create First Version.
马剑涛/25593     2003年4月01日                增加多次计算的功能，以便节省内存
**************************************************************************/
DWORD Calc_CRC32( BYTE * Packet, DWORD dwLength, BYTE byCalcMode )
{
    static DWORD CRC32_TABLE[256] = {
        0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b
, 0x1a864db2, 0x1e475005, 
        0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 0x350c9b64, 0x31cd86d3
, 0x3c8ea00a, 0x384fbdbd, 
        0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b
, 0x569796c2, 0x52568b75, 
        0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3
, 0x709f7b7a, 0x745e66cd, 
        0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039, 0x8b27c03c, 0x8fe6dd8b
, 0x82a5fb52, 0x8664e6e5, 
        0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033
, 0xa4ad16ea, 0xa06c0b5d, 
        0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb
, 0xceb42022, 0xca753d95, 
        0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4, 0xe5ffeb43
, 0xe8bccd9a, 0xec7dd02d, 
        0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c
, 0x2e003dc5, 0x2ac12072, 
        0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4
, 0x0808d07d, 0x0cc9cdca, 
        0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c
, 0x6211e6b5, 0x66d0fb02, 
        0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4
, 0x44190b0d, 0x40d816ba, 
        0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc
, 0xb6238b25, 0xb2e29692, 
        0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6, 0x99a95df3, 0x9d684044
, 0x902b669d, 0x94ea7b2a, 
        0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c
, 0xfa325055, 0xfef34de2, 
        0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34
, 0xdc3abded, 0xd8fba05a, 
        0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637, 0x7a089632, 0x7ec98b85
, 0x738aad5c, 0x774bb0eb, 
        0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d
, 0x558240e4, 0x51435d53, 
        0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5
, 0x3f9b762c, 0x3b5a6b9b, 
        0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff, 0x1011a0fa, 0x14d0bd4d
, 0x19939b94, 0x1d528623, 
        0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65
, 0xeba91bbc, 0xef68060b, 
        0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd
, 0xcda1f604, 0xc960ebb3, 
        0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7, 0xae3afba2, 0xaafbe615
, 0xa7b8c0cc, 0xa379dd7b, 
        0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad
, 0x81b02d74, 0x857130c3, 
        0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2
, 0x470cdd2b, 0x43cdc09c, 
        0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a
, 0x61043093, 0x65c52d24, 
        0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082
, 0x0b1d065b, 0x0fdc1bec, 
        0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a
, 0x2d15ebe3, 0x29d4f654, 
        0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12
, 0xdf2f6bcb, 0xdbee767c, 
        0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa
, 0xf9278673, 0xfde69bc4, 
        0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662
, 0x933eb0bb, 0x97ffad0c, 
        0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668, 0xbcb4666d, 0xb8757bda
, 0xb5365d03, 0xb1f740b4, 
    };

    static DWORD dwTempSaveCRC = 0;
    DWORD i;
     DWORD CRC32 = dwTempSaveCRC;
   
    //完全是为了提高可靠性增加一个赋值处理，可以不要
    if ( CALC_CRC_NORMAL == byCalcMode )
    {
        dwTempSaveCRC = 0;
    }
    

    for ( i = 0; i < dwLength; i++ )
    {        
        CRC32 = ( ( CRC32 << 8 )|Packet[i] ) ^ ( CRC32_TABLE[ ( CRC32 >> 24 ) 
& 0xFF ] );
    }

    if ( CALC_CRC_MULTI_CONTINUE == byCalcMode )
    {
        dwTempSaveCRC = CRC32;
        return 0;
    }
    else
    {
        dwTempSaveCRC = 0;
    }
 
    for ( i = 0; i < 4; i++ )
    {
        CRC32 = ( ( CRC32 << 8 )|0x00 ) ^ ( CRC32_TABLE[ ( CRC32 >> 24 ) & 
0xFF ] );
    }
    return CRC32;
}





UINT32 caculate_crc32(UnsignInt8Bits data[], UINT32 length)
{ 

        unsigned int i;
        UnsignInt8Bits mtemp;
        UnsignInt32Bits tmpq;
        UnsignInt32Bits regq;
        UnsignInt8Bits  data_in;
	    UnsignInt32Bits_Re fcs;
        tmpq.W32 = 0;
        regq.W32 = 0;
        for (i=0;i<4;i++){
                data[i].W8 = ~data[i].W8;
		}
        for (i=0;i<length;i++)
        {
                data_in.W8 = data[i].W8;
                        mtemp.bits.i0 = data_in.bits.i0 ^ tmpq.bits.i31;
                        mtemp.bits.i1 = data_in.bits.i1 ^ tmpq.bits.i30;
                        mtemp.bits.i2 = data_in.bits.i2 ^ tmpq.bits.i29;
                        mtemp.bits.i3 = data_in.bits.i3 ^ tmpq.bits.i28;
                        mtemp.bits.i4 = data_in.bits.i4 ^ tmpq.bits.i27;
                        mtemp.bits.i5 = data_in.bits.i5 ^ tmpq.bits.i26;
                        mtemp.bits.i6 = data_in.bits.i6 ^ tmpq.bits.i25;
                        mtemp.bits.i7 = data_in.bits.i7 ^ tmpq.bits.i24;

                regq.bits.i31  = tmpq.bits.i23  ^ mtemp.bits.i2 ;
                regq.bits.i30  = tmpq.bits.i22  ^ mtemp.bits.i3  ^ mtemp.bits.i0 ;
                regq.bits.i29  = tmpq.bits.i21  ^ mtemp.bits.i4  ^ mtemp.bits.i1  ^ mtemp.bits.i0 ;
                regq.bits.i28  = tmpq.bits.i20  ^ mtemp.bits.i5  ^ mtemp.bits.i2  ^ mtemp.bits.i1 ;
                regq.bits.i27  = tmpq.bits.i19  ^ mtemp.bits.i6  ^ mtemp.bits.i3  ^ mtemp.bits.i2  ^ mtemp.bits.i0 ;
                regq.bits.i26  = tmpq.bits.i18  ^ mtemp.bits.i7  ^ mtemp.bits.i4  ^ mtemp.bits.i3  ^ mtemp.bits.i1 ;
                regq.bits.i25  = tmpq.bits.i17  ^ mtemp.bits.i5  ^ mtemp.bits.i4 ;
                regq.bits.i24  = tmpq.bits.i16  ^ mtemp.bits.i6  ^ mtemp.bits.i5  ^ mtemp.bits.i0 ;
                regq.bits.i23  = tmpq.bits.i15  ^ mtemp.bits.i7  ^ mtemp.bits.i6  ^ mtemp.bits.i1 ;
                regq.bits.i22  = tmpq.bits.i14  ^ mtemp.bits.i7 ;
                regq.bits.i21  = tmpq.bits.i13  ^ mtemp.bits.i2 ;
                regq.bits.i20  = tmpq.bits.i12  ^ mtemp.bits.i3 ;
                regq.bits.i19  = tmpq.bits.i11  ^ mtemp.bits.i4  ^ mtemp.bits.i0 ;
                regq.bits.i18  = tmpq.bits.i10  ^ mtemp.bits.i5  ^ mtemp.bits.i1  ^ mtemp.bits.i0 ;
                regq.bits.i17  = tmpq.bits.i9  ^ mtemp.bits.i6  ^ mtemp.bits.i2  ^ mtemp.bits.i1 ;
                regq.bits.i16  = tmpq.bits.i8  ^ mtemp.bits.i7  ^ mtemp.bits.i3  ^ mtemp.bits.i2 ;
                regq.bits.i15  = tmpq.bits.i7  ^ mtemp.bits.i4  ^ mtemp.bits.i3  ^ mtemp.bits.i2  ^ mtemp.bits.i0 ;
                regq.bits.i14  = tmpq.bits.i6  ^ mtemp.bits.i5  ^ mtemp.bits.i4  ^ mtemp.bits.i3  ^ mtemp.bits.i1  ^ mtemp.bits.i0 ;
                regq.bits.i13  = tmpq.bits.i5  ^ mtemp.bits.i6  ^ mtemp.bits.i5  ^ mtemp.bits.i4  ^ mtemp.bits.i2  ^ mtemp.bits.i1  ^ mtemp.bits.i0 ;
                regq.bits.i12  = tmpq.bits.i4  ^ mtemp.bits.i7  ^ mtemp.bits.i6  ^ mtemp.bits.i5  ^ mtemp.bits.i3  ^ mtemp.bits.i2  ^ mtemp.bits.i1 ;
                regq.bits.i11  = tmpq.bits.i3  ^ mtemp.bits.i7  ^ mtemp.bits.i6  ^ mtemp.bits.i4  ^ mtemp.bits.i3 ;
                regq.bits.i10  = tmpq.bits.i2  ^ mtemp.bits.i7  ^ mtemp.bits.i5  ^ mtemp.bits.i4  ^ mtemp.bits.i2 ;
                regq.bits.i9  = tmpq.bits.i1  ^ mtemp.bits.i6  ^ mtemp.bits.i5  ^ mtemp.bits.i3  ^ mtemp.bits.i2 ;
                regq.bits.i8  = tmpq.bits.i0  ^ mtemp.bits.i7  ^ mtemp.bits.i6  ^ mtemp.bits.i4  ^ mtemp.bits.i3 ;
                regq.bits.i7  = mtemp.bits.i7  ^ mtemp.bits.i5  ^ mtemp.bits.i4  ^ mtemp.bits.i2  ^ mtemp.bits.i0 ;
                regq.bits.i6  = mtemp.bits.i6  ^ mtemp.bits.i5  ^ mtemp.bits.i3  ^ mtemp.bits.i2  ^ mtemp.bits.i1  ^ mtemp.bits.i0 ;
                regq.bits.i5  = mtemp.bits.i7  ^ mtemp.bits.i6  ^ mtemp.bits.i4  ^ mtemp.bits.i3  ^ mtemp.bits.i2  ^ mtemp.bits.i1  ^ mtemp.bits.i0 ;
                regq.bits.i4  = mtemp.bits.i7  ^ mtemp.bits.i5  ^ mtemp.bits.i4  ^ mtemp.bits.i3  ^ mtemp.bits.i1 ;
                regq.bits.i3  = mtemp.bits.i6  ^ mtemp.bits.i5  ^ mtemp.bits.i4  ^ mtemp.bits.i0 ;
                regq.bits.i2  = mtemp.bits.i7  ^ mtemp.bits.i6  ^ mtemp.bits.i5  ^ mtemp.bits.i1  ^ mtemp.bits.i0 ;
                regq.bits.i1  = mtemp.bits.i7  ^ mtemp.bits.i6  ^ mtemp.bits.i1  ^ mtemp.bits.i0 ;
                regq.bits.i0  = mtemp.bits.i7  ^ mtemp.bits.i1 ;
                tmpq.W32 = regq.W32;
        }

/*        fcs = (~regq[24],~regq[25],~regq[26],~regq[27],~regq[28],~regq[29],
						~regq[30],~regq[31],~regq[16],~regq[17],~regq[18],~regq[19],~regq[20],
						~regq[21],~regq[22],~regq[23],~regq[ 8],~regq[ 9],~regq[10],~regq[11],
						~regq[12],~regq[13],~regq[14],~regq[15],~regq[ 0],~regq[ 1],~regq[ 2],
						~regq[ 3],~regq[ 4],~regq[ 5],~regq[ 6],~regq[ 7]);
*/
            fcs.bits.i31 = regq.bits.i31;
            fcs.bits.i30 = regq.bits.i30;
            fcs.bits.i29 = regq.bits.i29;
            fcs.bits.i28 = regq.bits.i28;
            fcs.bits.i27 = regq.bits.i27;
            fcs.bits.i26 = regq.bits.i26;
            fcs.bits.i25 = regq.bits.i25;
            fcs.bits.i24 = regq.bits.i24;
            fcs.bits.i23 = regq.bits.i23;
            fcs.bits.i22 = regq.bits.i22;
            fcs.bits.i21 = regq.bits.i21;
            fcs.bits.i20 = regq.bits.i20;
            fcs.bits.i19 = regq.bits.i19;
            fcs.bits.i18 = regq.bits.i18;
            fcs.bits.i17 = regq.bits.i17;
            fcs.bits.i16 = regq.bits.i16;
            fcs.bits.i15 = regq.bits.i15;
            fcs.bits.i14 = regq.bits.i14;
            fcs.bits.i13 = regq.bits.i13;
            fcs.bits.i12 = regq.bits.i12;
            fcs.bits.i11 = regq.bits.i11;
            fcs.bits.i10 = regq.bits.i10;
            fcs.bits.i9  = regq.bits.i9 ;
            fcs.bits.i8  = regq.bits.i8 ;
            fcs.bits.i7  = regq.bits.i7 ;
            fcs.bits.i6  = regq.bits.i6 ;
            fcs.bits.i5  = regq.bits.i5 ;
            fcs.bits.i4  = regq.bits.i4 ;
            fcs.bits.i3  = regq.bits.i3 ;
            fcs.bits.i2  = regq.bits.i2 ;
            fcs.bits.i1  = regq.bits.i1 ;
            fcs.bits.i0  = regq.bits.i0 ;
		
		  for (i=0;i<4;i++){
                data[i].W8 = ~data[i].W8;
		  }
		  
		  fcs.W32 = ~fcs.W32;
		  return fcs.W32;

}




/*=======================================================================
*							GLB REG
*
*
=======================================================================*/




/*=======================================================================
 *  函数名称:void ETH_SetInterruptEnable(In UINT32 u32InterruptEn)
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:通过配置寄存器，使能/禁止CPU的中断类型
 *  输入参数:中断使能/禁止控制值
 *			 每个Bit代表一种中断类型的使能控制，0－中断禁止、1－中断使能）。
 *               		bit0 :	ETH有帧等待CPU接收                          
 *               		bit1:	ETH发送完来自CPU的一帧数据指示           
 *               		bit2:	ETH有空间接收来自CPU的帧                   
 *               		bit3:	ETH连接状态变化中断                      
 *               		bit4:	ETH速度模式变化中断                        
 *              		bit5:	ETH双工模式变化中断                        
 *               		bit6:	MDIO完成CPU操作指示                       
 *               		bit7:	总线操作出错指示中断信号                       
 *               		bit8:	重传次数超限指示                       
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetInterruptEnable(In UINT32 u32InterruptEn)
{
	//CPU_Int_U SetInterruptEn;
	//SetInterruptEn.w32 = (u32InterruptEn & 0x1FF);
	//*(UINT32 *) ETH_GLB_REG(1) = SetInterruptEn.w32;
	*(UINT32 *) ETH_GLB_REG(1) = (u32InterruptEn & 0x1FF);
}

/*=======================================================================
 *  函数名称:UINT32 ETH_ReadInterruptEnable(Out UINT32 *pu32InterruptEnAddr)
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:读中断配置寄存器，获取使能/禁止CPU的中断信息
 *  输入参数:中断使能/禁止控制值
 *			 每个Bit代表一种中断类型的使能控制，0－中断禁止、1－中断使能）。
 *               		bit0 :	ETH有帧等待CPU接收                          
 *               		bit1:	ETH发送完来自CPU的一帧数据指示           
 *               		bit2:	ETH有空间接收来自CPU的帧                   
 *               		bit3:	ETH连接状态变化中断                      
 *               		bit4:	ETH速度模式变化中断                        
 *              		bit5:	ETH双工模式变化中断                        
 *               		bit6:	MDIO完成CPU操作指示                       
 *               		bit7:	总线操作出错指示中断信号                       
 *               		bit8:	重传次数超限指示                       
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
UINT32 ETH_ReadInterruptEnable(Out UINT32 *pu32InterruptEnAddr)
{
	CPU_Int_U SetInterruptEn;
	SetInterruptEn.w32 = *(UINT32 *) ETH_GLB_REG(1);
	*pu32InterruptEnAddr = SetInterruptEn.w32;
	return *pu32InterruptEnAddr;
}

/*=======================================================================
 *  函数名称:UINT32 ETH_ReadInterruptStatus(Out UINT32 *pu32InterruptMode)
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:通过读取CPU对应的中断向量表寄存器，可以判断和相应中断类型
 *  输入参数:读取中断类型寄存器
 *			 每个Bit代表一种中断类型的使能控制，0－中断禁止、1－中断使能）
 *               		bit0 :	ETH有帧等待CPU接收                          
 *               		bit1:	ETH发送完来自CPU的一帧数据指示           
 *               		bit2:	ETH有空间接收来自CPU的帧                   
 *               		bit3:	ETH连接状态变化中断                      
 *               		bit4:	ETH速度模式变化中断                        
 *              		bit5:	ETH双工模式变化中断                        
 *               		bit6:	MDIO完成CPU操作指示                       
 *               		bit7:	总线操作出错指示中断信号                       
 *               		bit8:	重传次数超限指示                       
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
UINT32 ETH_ReadInterruptStatus(Out UINT32 *pu32InterruptMode)
{
	CPU_Int_U ReadInterrupt;
	ReadInterrupt.w32 = *(UINT32 *) ETH_GLB_REG(0);
	*pu32InterruptMode = ReadInterrupt.w32;
	/*read clear the status*/
	//*(UINT32 *) ETH_GLB_REG(0) = ((ReadInterrupt.w32) & (0x1FF));
	return *pu32InterruptMode;
}


/*=======================================================================
 *  函数名称:UINT32 ETH_ClearInterruptStatus(Out UINT32 *pu32InterruptMode)
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:通过读取CPU对应的中断向量表寄存器，可以判断和相应中断类型
 *  输入参数:读取中断类型寄存器
 *			 每个Bit代表一种中断类型的使能控制，0－中断禁止、1－中断使能）
 *               		bit0 :	ETH有帧等待CPU接收                          
 *               		bit1:	ETH发送完来自CPU的一帧数据指示           
 *               		bit2:	ETH有空间接收来自CPU的帧                   
 *               		bit3:	ETH连接状态变化中断                      
 *               		bit4:	ETH速度模式变化中断                        
 *              		bit5:	ETH双工模式变化中断                        
 *               		bit6:	MDIO完成CPU操作指示                       
 *               		bit7:	总线操作出错指示中断信号                       
 *               		bit8:	重传次数超限指示                       
 *  输出参数:
 *  返回类型:  return 1->soft reset ok; return 0->soft reset fail.
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
UINT32 ETH_ClearInterruptStatus(In UINT32 u32InterruptClear)
{
	CPU_Int_U ReadInterrupt;
	ReadInterrupt.w32 = *(UINT32 *) ETH_GLB_REG(0);
	/*read clear the status*/
	*(UINT32 *) ETH_GLB_REG(0) = ((ReadInterrupt.w32) & (u32InterruptClear));
	if ((u32InterruptClear & (*(UINT32 *) ETH_GLB_REG(0))) == 0)
	{
		return 1;
	}
	return 0;
}

/*=======================================================================
 *  函数名称:void ETH_SetNormalInterruptStyle(In UINT32 u32InterruptNorChoi)
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:配置CPU必须响应的普通中断类型
 *  输入参数:配置值（每个Bit代表一种中断类型的使能控制，0－中断禁止、1－中断使能）
 *               		bit0 :	ETH有帧等待CPU接收                          
 *               		bit1:	ETH发送完来自CPU的一帧数据指示           
 *               		bit2:	ETH有空间接收来自CPU的帧                   
 *               		bit3:	ETH连接状态变化中断                      
 *               		bit4:	ETH速度模式变化中断                        
 *              		bit5:	ETH双工模式变化中断                        
 *               		bit6:	MDIO完成CPU操作指示                       
 *               		bit7:	总线操作出错指示中断信号                       
 *               		bit8:	重传次数超限指示                       
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetNormalInterruptStyle(In UINT32 u32InterruptNorChoi)
{
//	CPU_Int_U SetCPUInterrNor;
//	SetCPUInterrNor.w32 = u32InterruptNorChoi;
//	*(UINT32 *) ETH_GLB_REG(2) = SetCPUInterrNor.w32;
	*(UINT32 *) ETH_GLB_REG(2) = (u32InterruptNorChoi & 0x1FF);
}


/*=======================================================================
 *  函数名称:UINT32 ETH_ReadSoftResetFinishFlag (Out UINT32 *pu32ResetFiniInf)
 *  初稿完成:2005/1/8   
 *  作    者:  
 *  函数功能:读取ETH软复位之后硬件初始化完成的指示标志位，以便于确定是否可以做相应操作
 *  输入参数:
 *  输出参数:return 1->soft reset ok; return 0->soft reset fail.
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
/*
UINT32 ETH_ReadSoftResetFinishFlag (Out UINT32 *pu32ResetFiniInf)
{
	Init_OnResetInf_U ReadResetFin;
	ReadResetFin.w32 = *(UINT32 *) ETH_GLB_REG(3);
	*pu32ResetFiniInf = ReadResetFin.w32;
	if ((ReadResetFin.w32 ^ 0x7) != 0)
	{
		return Failure;
	}
	else
	{
		return Success;
	}
}
*/

/*=======================================================================
 *  函数名称:UINT32 ETH_SoftResetPort(In UINT32 u32PortReset)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:端口复位控制1－复位
 *  输入参数:
 *  输出参数:  
 *  返回类型:  return 0->reset fail; return 1->reset success.
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
/*UINT32 ETH_SoftResetPort(In UINT32 u32PortReset)
{
	UINT16 u16Delay = 0;
	Port_Reset_U SoftReset;
	SoftReset.w32 = *(UINT32 *) ETH_MAC_REG(2);
	SoftReset.bits.reset_port = u32PortReset;
	*(UINT32 *) ETH_MAC_REG(2) = SoftReset.w32;
	for(u16Delay = 0 ; u16Delay < 5000 ; u16Delay ++)
	{
		if ((*(UINT32 *) ETH_GLB_REG(3) ^ 0x7) == 0)
		{
			return 1;
		}
	}
	return 0;
}*/


#if 0
/*=======================================================================
 *  函数名称:void ETH_SetPortRMIIOrMII(In UINT32 u32MIIOrRMIIMode)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:设置端口与PHY之间RMII与MII接口模式选择(0-MII,1-RMII)
 *  输入参数:
 *	     	 RMII/MII模式
 *  输出参数:  
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetPortRMIIOrMII (In UINT32 u32MIIOrRMIIMode)
 
{
	Set_RMII_Endian_U SetRMIIOrMII;
	SetRMIIOrMII.w32 = *(UINT32 *) ETH_GLB_REG(4);
	SetRMIIOrMII.bits.mode_rmii = u32MIIOrRMIIMode;
	*(UINT32 *) ETH_GLB_REG(4) = SetRMIIOrMII.w32;
}
#endif


/*=======================================================================
 *  函数名称:void ETH_SetEndian (In UINT32 u32InEndian, In UINT32 u32OutEndian)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:设置SDRAM的大小端模式，1－小端，0－大端
 *  输入参数:
 *  输出参数:  
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetEndian (In UINT32 u32InEndian, In UINT32 u32OutEndian)
 
{
	Set_RMII_Endian_U SetEndian;
	SetEndian.w32 = *(UINT32 *) ETH_GLB_REG(4);
	SetEndian.bits.in_endian = u32InEndian;
	SetEndian.bits.out_endian = u32OutEndian;
	*(UINT32 *) ETH_GLB_REG(4) = SetEndian.w32;
}

/*=======================================================================
 *  函数名称:void ETH_SetFrameQueueLength (In UINT32 u32InputLength, In UINT32 u32OutputLength)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:设置输入和输出队列长度，长度总和<= 32
 *  输入参数:
 *  输出参数:  
 *  返回类型:  return 0->set fail; return 1->set success.
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
UINT32 ETH_SetFrameQueueLength (In UINT32 u32InputLength, In UINT32 u32OutputLength)
{
	Set_FrameQueueLength_U SetFrameQueue;
	Set_CPUReadSpeLinDup_U SpeLinDupMode;
	if ( (u32InputLength == 0) || (u32OutputLength == 0) )
	{
		//ETH_Print("The length can not be 0!\n");
		return 0;
	}
	if ( (u32InputLength + u32OutputLength) > 64 )
	{
		//ETH_Print("InputLength add OutputLength must smaller than 32!\n");
		return 0;
	}
	/*for the queue can be set perfect, we must unlink the port first, and clear the queue frame*/
	SpeLinDupMode.w32 = *(UINT32 *) ETH_MAC_REG(3);
	SpeLinDupMode.bits.link_stat = 0;
	*(UINT32 *) ETH_MAC_REG(3) = SpeLinDupMode.w32;
	/*clear the input queue frame*/
	//*(UINT32 *) ETH_MAC_REG(10) = 0;
	//*(UINT32 *) ETH_MAC_REG(10) = 1;
	//*(UINT32 *) ETH_MAC_REG(10) = 0;
	/*set the queue length*/
	SetFrameQueue.w32 = *(UINT32 *) ETH_GLB_REG(6);
	SetFrameQueue.bits.iq_len = u32InputLength;
	SetFrameQueue.bits.eq_len = u32OutputLength;
	*(UINT32 *) ETH_GLB_REG(6) = SetFrameQueue.w32;
	SpeLinDupMode.bits.link_stat = 1;
	*(UINT32 *) ETH_MAC_REG(3) = SpeLinDupMode.w32;
	e_num = u32InputLength;
	h_num = u32OutputLength;
//	ETH_Init();
	return 1;
}

/*=======================================================================
 *  函数名称:UINT32 ETH_SetPauseFrameLimitLevel (In UINT32 u32EnablePauseUpLimit, In UINT32 u32DisablePauseDownLimit)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:设置队列流控上限和解除流控下限，均<=32
 *  输入参数:
 *  输出参数:  
 *  返回类型:  return 0->set fail; return 1->set success.
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
UINT32 ETH_SetPauseFrameLimitLevel (In UINT32 u32EnablePauseUpLimit, In UINT32 u32DisablePauseDownLimit)
{
	Set_PauseFrameLimitLevel_U SetPauseFrameLimitLevel;
	if ((u32EnablePauseUpLimit > 32) || (u32DisablePauseDownLimit > 32))
	{
		return 0;
	}
	SetPauseFrameLimitLevel.w32 = *(UINT32 *) ETH_GLB_REG(7);
	SetPauseFrameLimitLevel.bits.qlimit_up = u32EnablePauseUpLimit;
	SetPauseFrameLimitLevel.bits.qlimit_down = u32DisablePauseDownLimit;
	*(UINT32 *) ETH_GLB_REG(7) = SetPauseFrameLimitLevel.w32;
	return 1;
}


/*=======================================================================
 *  函数名称:void ETH_ClearFrameQueue (In UINT32 u32CPUCleQue) 
 *  初稿完成:2005/1/8   
 *  作    者:  
 *  函数功能:CPU清空输出队列控制
 *  输入参数:CPU清空队列控制信息,1-clear
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
/*此寄存器已经在规格中删除*/
/*
void ETH_ClearFrameQueue (In UINT32 u32CPUCleQue) 
{
	CPU_ClearQueue_U SetCPUCleQue;
	SetCPUCleQue.w32 = *(UINT32 *) ETH_GLB_REG(10);
	SetCPUCleQue.bits.Flush_in = u32CPUCleQue;
	*(UINT32 *) ETH_GLB_REG(10) = SetCPUCleQue.w32;
}
*/
#if 0
/*=======================================================================
 *  函数名称:void ETH_ClearFrameQueue (In UINT32 u32CPUCleQue) 
 *  初稿完成:2005/1/8   
 *  作    者:  
 *  函数功能:设置输入输出队列起始地址，高21位有效，低11位无效
 *  输入参数:
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
 
void ETH_SetQueueAddress (In UINT32 u32InputQueueAddr,In UINT32 u32OutputQueueAddr) 
{
	*(UINT32 *) ETH_GLB_REG(13) = u32InputQueueAddr & 0xFFFFF800;
	*(UINT32 *) ETH_GLB_REG(14) = u32OutputQueueAddr & 0xFFFFF800;
}
#endif
#if 0
/*=======================================================================
 *  函数名称:void ETH_SetRetryTimes (In UINT32 u32RetryTimes) 
 *  初稿完成:2005/1/8   
 *  作    者:  
 *  函数功能:设置帧重传的次数上限，达到这个上限的时候，如果中断使能的时候会产生中断
 *  输入参数:
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetRetryTimes (In UINT32 u32RetryTimes) 
{
	Frame_RetryTimes_U SetRetryTimes;
	SetRetryTimes.w32 = *(UINT32 *) ETH_GLB_REG(15);
	SetRetryTimes.bits.retry_time = u32RetryTimes;
	*(UINT32 *) ETH_GLB_REG(15) = SetRetryTimes.w32;
}

#endif
/*=======================================================================
 *  函数名称:void ETH_ReadQueueUseLength (Out UINT32 *pu32InputQueue,Out UINT32 *pu32OutputQueue) 
 *  初稿完成:2005/1/8   
 *  作    者:  
 *  函数功能:读取输出输入队列使用长度
 *  输入参数:
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_ReadQueueUseLength (Out UINT32 *pu32InputQueue,Out UINT32 *pu32OutputQueue) 
{
	Queue_UseLength_U ReadQueueLength;
	ReadQueueLength.w32 = *(UINT32 *) ETH_GLB_REG(18);
	*pu32OutputQueue = ReadQueueLength.bits.eq_cnt;
	*pu32InputQueue = ReadQueueLength.bits.iq_cnt;
}

/*=======================================================================
 *  函数名称:void ETH_StatisticFiltFrameNumber (Out UINT32 *pu32FiltFrameCnt) 
 *  初稿完成:2005/1/8   
 *  作    者:  
 *  函数功能:统计过滤帧计数
 *  输入参数:
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
/*
void ETH_StatisticFiltFrameNumber (Out UINT32 *pu32FiltFrameCnt) 
{
	*pu32FiltFrameCnt = *(UINT32 *) ETH_GLB_REG(17);
}

*/
/*=======================================================================
*							MAC REG
*
*
=======================================================================*/


/*=======================================================================
 *  函数名称:void ETH_SetFrameConfiguration (In UINT32 u32PreCodNo, In UINT32 u32FraCle, In UINT32 u32StrFraDist)
 *  初稿完成:2005/1/8   
 *  作    者:  
 *  函数功能:设置连续检测到帧前导码个数
 *			 设置帧间隙值
 *			 设置输出流控帧间隔参数
 *  输入参数:u32PreCodNo->帧前导码个数
 *			u32FraCle->帧间隙值
 *			u32StrFraDist->流控帧间隔参数
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetFrameConfiguration (In UINT32 u32PreCodNo, In UINT32 u32FraCle, In UINT32 u32StrFraDist)
{
	Frame_Para_U SetFrameConfig;
	SetFrameConfig.w32 = *(UINT32 *) ETH_MAC_REG(0);
	SetFrameConfig.bits.para = u32StrFraDist;
	SetFrameConfig.bits.ipg = u32FraCle;
	SetFrameConfig.bits.pre_cnt_limit = u32PreCodNo;
	*(UINT32 *) ETH_MAC_REG(0) = SetFrameConfig.w32;
}


/*=======================================================================
 *  函数名称:void ETH_SetPortStatusMode(In UINT32 u32PortStatMode)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:设置端口状态信息获得方式
 *  输入参数:1'b1:使用CPU设定的状态信息
				0: 使用从MDIO接口获得的状态信息
 *  输出参数:  
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetPortStatusMode(In UINT32 u32PortStatMode)
{
	Port_Stat_Choice_U SetPortStatMode;
	SetPortStatMode.w32 = *(UINT32 *) ETH_MAC_REG(1);
	SetPortStatMode.bits.stat_ctrl = u32PortStatMode;
	*(UINT32 *) ETH_MAC_REG(1) = SetPortStatMode.w32;
}


/*=======================================================================
 *  函数名称:void ETH_SetSpeedLinkDuplexMode(In UINT32 u32Speed, In UINT32 u32Link, In UINT32 u32Duplex)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:CPU设置速度、链接状态、双工模式
 *  输入参数:速度(1-100M,0-10M)、链接状态(1-链接,0-无链接)、双工模式(1-双工,0-半双工)对应的指针数据
 *  输出参数:  
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetSpeedLinkDuplexMode(In UINT32 u32Speed, In UINT32 u32Link, In UINT32 u32Duplex)
{
	Set_CPUSetSpeLinDup_U SetSpeLinDupMode;
	SetSpeLinDupMode.w32 = *(UINT32 *) ETH_MAC_REG(3);
	SetSpeLinDupMode.bits.speed_stat_dio = u32Speed;
	SetSpeLinDupMode.bits.duplex_stat_dio = u32Duplex;
	SetSpeLinDupMode.bits.link_stat_dio = u32Link;
	*(UINT32 *) ETH_MAC_REG(3) = SetSpeLinDupMode.w32;
}

/*=======================================================================
 *  函数名称:void ETH_ReadSpeedLinkDuplexMode(Out UINT32 *u32Speed Out UINT32 *u32Link, Out UINT32 *u32Duplex,)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:读取端口的速度、链接状态、双工模式
 *  输入参数:
 *  输出参数:  速度(1-100M,0-10M)、链接状态(1-链接,0-无链接)、双工模式(1-双工,0-半双工)对应的指针数据，
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_ReadSpeedLinkDuplexMode(Out UINT32 *u32Speed, Out UINT32 *u32Link, Out UINT32 *u32Duplex)
 
{
	Set_CPUReadSpeLinDup_U ReadSpeLinDupMode;
	ReadSpeLinDupMode.w32 = *(UINT32 *) ETH_MAC_REG(4);
	*u32Speed = ReadSpeLinDupMode.bits.speed_stat ;
	*u32Duplex = ReadSpeLinDupMode.bits.duplex_stat;
	*u32Link = ReadSpeLinDupMode.bits.link_stat;
}

/*=======================================================================
 *  函数名称:void ETH_ClearSpeedLinkDuplexMode(In UINT8 u8PortNo, In UINT32 u32Speed ,In UINT32 u32Link, In UINT32 u32Duplex)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:清除速度、链接状态、双工模式改变指示信号
 *  输入参数:清除－1，不清除－0
 *  输出参数:  
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_ClearSpeedLinkDuplexMode(In UINT32 u32Speed, In UINT32 u32Link, In UINT32 u32Duplex)
{
	Set_CPUSetSpeLinDup_U ClearSpeLinDupMode;
	ClearSpeLinDupMode.w32 = *(UINT32 *) ETH_MAC_REG(5);
	ClearSpeLinDupMode.bits.speed_stat_dio = u32Speed;
	ClearSpeLinDupMode.bits.duplex_stat_dio = u32Duplex;
	ClearSpeLinDupMode.bits.link_stat_dio = u32Link;
	*(UINT32 *) ETH_MAC_REG(5) = ClearSpeLinDupMode.w32;
}

/*=======================================================================
*							MAC0 REG
*
*
=======================================================================*/


/*=======================================================================
 *  函数名称:void ETH_ClearPortCountInformation( Out UINT32 u32Value)
 *  初稿完成:2005/1/5   
 *  作    者:  
 *  函数功能:清除指定端口列表的计数信息
 *  输入参数: 需要清除统计信息的端口置1，否则置0
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/

void ETH_ClearPortCountInformation( In UINT32 u32Value)
{
	Port_Conf_U ClearCountInf;
	ClearCountInf.w32 = *(UINT32 *) ETH_MAC_REG(7);
	ClearCountInf.bits.cntr_clr_all = u32Value;
	*(UINT32 *) ETH_MAC_REG(7) = ClearCountInf.w32;
	/*finish clear, then disable clear.*/
	ClearCountInf.bits.cntr_clr_all = 0;
	*(UINT32 *) ETH_MAC_REG(7) = ClearCountInf.w32;
}


/*=======================================================================
 *  函数名称:void ETH_SetPortCntCleanEnable(In UINT32 u32CntAuCleEn)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:设置端口统计寄存器读清空使能/禁止
 *  输入参数:读清空设置.1:表示读清空，0:禁止读清空。
 *  输出参数:  
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetPortCntCleanEnable(In UINT32 u32CntAuCleEn)
{
	Port_Conf_U SetCntAuCleEn;

		SetCntAuCleEn.w32 = *(UINT32 *) ETH_MAC_REG(7);
		SetCntAuCleEn.bits.cntr_rdclr_en = u32CntAuCleEn;
		*(UINT32 *) ETH_MAC_REG(7) = SetCntAuCleEn.w32;
}

#if 0

/*=======================================================================
 *  函数名称:void ETH_CountPortStatistic( In UINT8 u8PortNo, Out Port_Cnt_S *pu32StaticCnt)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:统计端口的统计信息
 *  输入参数:端口号（上行口或者下行口），
 *  输出参数:  
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_CountPortStatistic( Out Port_Cnt_S *pu32StaticCnt) 
{
	UINT32 AddrForPort = 0;
	
	if (u8PortNo == 1)
	{
		AddrForPort = 0x0100;
	}
	pu32StaticCnt->drop_event	= *(UINT32 *) (SF_Statis + (AddrForPort | 0x0000));
	pu32StaticCnt->crcerr = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0004));	        
	pu32StaticCnt->undersizepkts = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0008));
	pu32StaticCnt->oversizepkts = *(UINT32 *) (SF_Statis + (AddrForPort | 0x000c));	        
	pu32StaticCnt->fragments = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0010));    
	pu32StaticCnt->jabber = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0014));	        
	pu32StaticCnt->broadcastpkts = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0018));
	pu32StaticCnt->multicastpkts = *(UINT32 *) (SF_Statis + (AddrForPort | 0x001c));	        
	pu32StaticCnt->pkts64 = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0020));		        
	pu32StaticCnt->pkts65_127 = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0024));	        
	pu32StaticCnt->pkts128_255 = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0028));	        
	pu32StaticCnt->pkts256_511 = *(UINT32 *) (SF_Statis + (AddrForPort | 0x002c));	        
	pu32StaticCnt->pkts512_1023 = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0030));	        
	pu32StaticCnt->pkts1024_1518 = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0034));	        
	pu32StaticCnt->pkts_1518 = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0038));	        
	pu32StaticCnt->ifinucastpkts = *(UINT32 *) (SF_Statis + (AddrForPort | 0x003c));	        
	pu32StaticCnt->ifinnucastpkts = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0040));	        
	pu32StaticCnt->ifindiscards = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0044));	        
	pu32StaticCnt->ifinerrors = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0048));	        
	pu32StaticCnt->ifinmulticast = *(UINT32 *) (SF_Statis + (AddrForPort | 0x004c));	        
	pu32StaticCnt->ifinbroadcast = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0050));	        
	pu32StaticCnt->dot3alignmenterr = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0054));	
	pu32StaticCnt->dot3fcserr = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0058));	        
	pu32StaticCnt->dot3internalrecerr = *(UINT32 *) (SF_Statis + (AddrForPort | 0x005c));      
	pu32StaticCnt->dot3inpause = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0060));	        
	pu32StaticCnt->dot3unkpause = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0064));	        
	pu32StaticCnt->dot3dribble = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0068));	        
	pu32StaticCnt->octets = *(UINT32 *) (SF_Statis + (AddrForPort | 0x006c));	 	        
	pu32StaticCnt->pkts = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0070));		        
	pu32StaticCnt->ifinoctets = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0074));	        
	pu32StaticCnt->broadcastpkts_tx =	*(UINT32 *) (SF_Statis + (AddrForPort | 0x0080));
	pu32StaticCnt->multicastpkts_tx =	*(UINT32 *) (SF_Statis + (AddrForPort | 0x0084));
	pu32StaticCnt->ifoutdiscards = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0088));	        
	pu32StaticCnt->ifouterrors = *(UINT32 *) (SF_Statis + (AddrForPort | 0x008c));	        
	pu32StaticCnt->ifoutucastpkts = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0090));	        
	pu32StaticCnt->ifoutnucastpkts = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0094));	 
	pu32StaticCnt->ifoutmulticast = *(UINT32 *) (SF_Statis + (AddrForPort | 0x0098));	        
	pu32StaticCnt->ifoutbroadcast = *(UINT32 *) (SF_Statis + (AddrForPort | 0x009c));	        
	pu32StaticCnt->pkts64_tx = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00a0));	        
	pu32StaticCnt->pkts65_127_tx = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00a4));	        
	pu32StaticCnt->pkts128_255_tx = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00a8));	        
	pu32StaticCnt->pkts256_511_tx = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00ac));	        
	pu32StaticCnt->pkts512_1023_tx = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00b0));	 
	pu32StaticCnt->pkts1024_1518_tx = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00b4));	
	pu32StaticCnt->pkts_1518_tx = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00b8));	        
	pu32StaticCnt->dot3singlecol = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00bc));	        
	pu32StaticCnt->dot3multiplecol = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00c0));	
	pu32StaticCnt->dot3latecol = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00c4));	        
	pu32StaticCnt->dot3excessivecol =	*(UINT32 *) (SF_Statis + (AddrForPort | 0x00c8));
	pu32StaticCnt->dot3colcnt = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00cc));	        
	pu32StaticCnt->dot3defer = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00d0));	        
	pu32StaticCnt->pkts_tx = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00d4));	 	
	pu32StaticCnt->octets_tx = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00d8));	        
	pu32StaticCnt->collisions = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00dc));	        
	pu32StaticCnt->ifoutoctets = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00e0));	        
	pu32StaticCnt->dot3outpause = *(UINT32 *) (SF_Statis + (AddrForPort | 0x00e4));	        

	#ifdef SfTest
		SF_Print("SF count information is %ld\n",pu32StaticCnt->drop_event);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->crcerr);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->undersizepkts);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->oversizepkts);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->fragments);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->jabber);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->broadcastpkts);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->multicastpkts);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts64);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts65_127);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts128_255);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts256_511);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts512_1023);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts1024_1518);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts_1518);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifinucastpkts);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifinnucastpkts);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifindiscards);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifinerrors);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifinmulticast);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifinbroadcast);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3alignmenterr);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3fcserr);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3internalrecerr);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3inpause);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3unkpause);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3dribble);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->octets);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifinoctets);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->broadcastpkts_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->multicastpkts_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifoutdiscards);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifouterrors);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifoutucastpkts);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifoutnucastpkts);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifoutmulticast);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifoutbroadcast);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts64_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts65_127_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts128_255_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts256_511_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts512_1023_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts1024_1518_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts_1518_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3singlecol);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3multiplecol);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3latecol);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3excessivecol);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3colcnt);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3defer);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->pkts_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->octets_tx);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->collisions);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->ifoutoctets);
		SF_Print("SF count information is %ld\n",pu32StaticCnt->dot3outpause);
	#endif
}

#endif

/*=======================================================================
 *  函数名称:void ETH_SetPortShortFrameReceive(In UINT32 u32ShorReceEn)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:设置端口短帧接收使能/禁止
 *  输入参数: 短帧接收使能/禁止设置
 *			u32ShorReceEn:1->使能；0->禁止
 *  输出参数:  
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetPortShortFrameReceive(In UINT32 u32ShorReceEn)
{
	Port_Conf_U SetShorReceEn;
	SetShorReceEn.w32 = *(UINT32 *) ETH_MAC_REG(7);
	SetShorReceEn.bits.rx_shframe_en = u32ShorReceEn;
	*(UINT32 *) ETH_MAC_REG(7) = SetShorReceEn.w32;
}


/*=======================================================================
 *  函数名称:void ETH_SetPortPostFrameSend(In UINT32 u32PostFraSenEn)
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:设置端口流控帧发送使能/禁止
 *  输入参数:流控帧发送使能/禁止设置
 *			u32PostFraSenEn:1->使能；0->禁止
 *  输出参数:  
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetPortPostFrameSend(In UINT32 u32PostFraSenEn)
{
	Port_Conf_U SetPostFraSenEn;
	SetPostFraSenEn.w32 = *(UINT32 *) ETH_MAC_REG(7);
	SetPostFraSenEn.bits.pause_en = u32PostFraSenEn;
	*(UINT32 *) ETH_MAC_REG(7) = SetPostFraSenEn.w32;
}


/*=======================================================================
 *  函数名称:void ETH_SetPortCountRollCtrl(  In UINT32 u32Value) 
 *  初稿完成:2005/1/6   
 *  作    者:  
 *  函数功能:设置端口的统计信息计满是否回零
 *  输入参数: 需要设置计数器满回零的端口置1，否则置0
 *  输出参数:  
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetPortCountRollCtrl( In UINT32 u32Value) 
{
	Port_Conf_U CntRollCtrl;
	CntRollCtrl.w32 = *(UINT32 *) ETH_MAC_REG(7);
	CntRollCtrl.bits.cntr_roll_dis = u32Value;
	*(UINT32 *) ETH_MAC_REG(7) = CntRollCtrl.w32;
}

#if 0
/*=======================================================================
 *  函数名称:void ETH_ReadFrameCRCErrorFlag (Out UINT32 *u32RecFraCRCErr, Out UINT32 *u32TraFraCRCErr) 
 *  初稿完成:2005/1/15   
 *  作    者:  
 *  函数功能:读取的接收帧CRC校验出错标志，读取的发送帧CRC校验出错标志
 *  输入参数:
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_ReadFrameCRCErrorFlag (Out UINT32 *u32RecFraCRCErr, Out UINT32 *u32TraFraCRCErr)
{
	Port_Conf_U FrameCRCError;
	FrameCRCError.w32 = *(UINT32 *) ETH_MAC_REG(6);
	*(UINT32 *)u32RecFraCRCErr = FrameCRCError.bits.crc_rx;
	*(UINT32 *)u32TraFraCRCErr = FrameCRCError.bits.txcrc_bad;
}
#endif

/*=======================================================================
 *  函数名称:void ETH_SetFrameCRCCtlRebuild ( In UINT32 u32CRCRebuild)
 *  初稿完成:2005/1/15   
 *  作    者:  
 *  函数功能:CRC生成时是否需要修改原来的CRC位，重新计算CRC
 *  输入参数:CRC重新生成设置位
 *			u32CRCRebuild:0->从新计算；1->不从新计算
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetFrameCRCCtlRebuild ( In UINT32 u32CRCRebuild)
{
	Port_Conf_U FrameCRCRebuild;
	FrameCRCRebuild.w32 = *(UINT32 *) ETH_MAC_REG(7);
	FrameCRCRebuild.bits.crcgen_dis= u32CRCRebuild;
	*(UINT32 *) ETH_MAC_REG(7) = FrameCRCRebuild.w32;
}



/*=======================================================================
 *  函数名称:UINT32 ETH_SetLongFrameLength(In UINT32 u32Value)
 *  初稿完成:2005/1/5   
 *  作    者:  
 *  函数功能:设置最长帧的长度
 *  输入参数: 最长帧长度
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetLongFrameLength( In UINT32 u32Value)
{
	Port_Conf_U SetLongFrame;
	SetLongFrame.w32 = *(UINT32 *) ETH_MAC_REG(7);
	SetLongFrame.bits.len_max = u32Value;
	*(UINT32 *) ETH_MAC_REG(7) = SetLongFrame.w32;
}


/*=======================================================================
 *  函数名称:UINT32 ETH_SetShortFrameLength(In UINT32 u32Value)
 *  初稿完成:2005/1/5   
 *  作    者:  
 *  函数功能:设置最短帧的长度
 *  输入参数:最短帧长度
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetShortFrameLength(In UINT32 u32Value)
{
	Port_Conf_U SetLongFrame;
	SetLongFrame.w32 = *(UINT32 *) ETH_MAC_REG(7);
	SetLongFrame.bits.rx_min_thr = u32Value;
	*(UINT32 *) ETH_MAC_REG(7) = SetLongFrame.w32;
}

/*=======================================================================
 *  函数名称:void ETH_SetLoopBackEnable(In UINT32 u32ExLoop, In UINT32 u32InLoop)
 *  初稿完成:2005/1/5   
 *  作    者:  
 *  函数功能:设置内环回和外环回使能
 *  输入参数:使能－1，否则－0
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetLoopBackEnable(In UINT32 u32ExLoop, In UINT32 u32InLoop)
{
	Port_Conf_U SetLoopBack;
	SetLoopBack.w32 = *(UINT32 *) ETH_MAC_REG(7);
	SetLoopBack.bits.ex_loop_en = u32ExLoop;
	SetLoopBack.bits.in_loop_en = u32InLoop;
	*(UINT32 *) ETH_MAC_REG(7) = SetLoopBack.w32;
}

/*=======================================================================
*							MDIO REG
*
*
=======================================================================*/


/*=======================================================================
 *  函数名称:UINT32 ETH_SetPHYExternAddress(In UINT32 u32PHYAddr)    
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:用于是设置对应端口链接的PHY外部物理地址
 *  输入参数:Phy Addr.
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetPHYExternAddress(In UINT32 u32PHYAddr)   
{
	Set_phy_addr_U SetPHYExAddr;
	SetPHYExAddr.w32 = *(UINT32 *) ETH_MDIO_REG(2);
	SetPHYExAddr.bits.phy_addr0= u32PHYAddr;
	*(UINT32 *) ETH_MDIO_REG(2) = SetPHYExAddr.w32;
}


/*=======================================================================
 *  函数名称:void ETH_SetPHYFrequency(In UINT32 u32PHYFreq)   
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:通过配置分频系数配置外部PHY进行读写操作时的速度
 *  输入参数:分频系数(2分频到16分频)
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetPHYFrequency(In UINT32 u32PHYFreq)  
{
	PHY_Conf_U SetPHYFreq;
	SetPHYFreq.w32 = *(UINT32 *) ETH_MDIO_REG(0);
	SetPHYFreq.bits.frq_dv = u32PHYFreq;
	*(UINT32 *) ETH_MDIO_REG(0) = SetPHYFreq.w32;
}


/*=======================================================================
 *  函数名称:void ETH_SetPHYChipInformationAddress(In UINT32 u32PHYCInfAddr, In UINT32 u32PHYSpeOffsAddr, 
 *									In UINT32 u32PHYLinOffsAddr, In UINT32 u32PHYDupOffsAddr)
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:设置PHY芯片存储状态信息（速度、链接和双工）的寄存器地址
 *  输入参数:PHY芯片存储状态信息（速度、链接和双工）的寄存器地址；
 *	     	 PHY芯片状态寄存器中存储速度信息的偏移地址；
 *			 PHY芯片状态寄存器中存储链接信息的偏移地址；
 *			 PHY芯片状态寄存器中存储双工信息的偏移地址
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetPHYChipInformationAddress(In UINT16 u16PortNo, 
	In UINT32 u32PHYSpeInfAddr, In UINT32 u32PHYLinInfAddr, In UINT32 u32PHYDupInfAddr, 
	In UINT32 u32PHYSpeOffsAddr, 	In UINT32 u32PHYLinOffsAddr, In UINT32 u32PHYDupOffsAddr)
{
	PHY_Stat_Addr_U SetPHYChipInfAddr;
	
	if (u16PortNo == 0)
	{
		SetPHYChipInfAddr.w32 = *( volatile UINT32 * ) ETH_MDIO_REG(4);
		SetPHYChipInfAddr.bits.duplex_index = u32PHYDupOffsAddr;
		SetPHYChipInfAddr.bits.link_index = u32PHYLinOffsAddr;
		SetPHYChipInfAddr.bits.speed_index = u32PHYSpeOffsAddr;
		SetPHYChipInfAddr.bits.internal_addr_duplex = u32PHYDupInfAddr;
		SetPHYChipInfAddr.bits.internal_addr_link = u32PHYLinInfAddr;
		SetPHYChipInfAddr.bits.internal_addr_speed = u32PHYSpeInfAddr;
		*( volatile UINT32 * ) ETH_MDIO_REG(4) = SetPHYChipInfAddr.w32;
	}
}
/*=======================================================================
 *  函数名称:void ETH_ReadPHYPortInformation(In UINT8 u8PHYPortNo, Out UINT32 *pu32PHYPortSpeed, 
 *									Out UINT32 *pu32PHYPortLink, Out UINT32 *pu32PHYPortDupMode) 
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:读取PHY的各个端口的端口速度，链接状态，双工工作状态
 *  输入参数:
 *           
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_ReadPHYPortInformation( Out UINT32 *pu32PHYPortSpeed, 
									Out UINT32 *pu32PHYPortLink, Out UINT32 *pu32PHYPortDupMode) 
{
	PHY_Read_SpeLinDup_U ReadPHYPortInf;
	ReadPHYPortInf.w32 = *(UINT32 *) ETH_MDIO_REG(3);
	*pu32PHYPortSpeed = ReadPHYPortInf.bits.speed_mdio2mac;
	*pu32PHYPortLink = ReadPHYPortInf.bits.link_mdio2mac ;
	*pu32PHYPortDupMode = ReadPHYPortInf.bits.duplex_mdio2mac ;
	*(UINT32 *) ETH_MDIO_REG(3) = ReadPHYPortInf.w32;
}

/*=======================================================================
 *  函数名称: UINT32 ETH_PhyWrite(In UINT32 u32PhyRegIndex , In UINT32 u32PhyData )
 *  初稿完成:  2004/01/07   
 *  作    者: 
 *  函数功能: 向指定PHY芯片的指定寄存器写入值
 *  输入参数: u32PhyIndex－ PHY芯片的索引号
 *            u32PhyRegIndex－PHY芯片内的寄存器号
 *            u32PhyData－要写的寄存器数据
 *  输出参数: 无
 *  返回类型:  
 *  其他说明: 
 *  调用函数:  
 *  主调函数:  
 *========================================================================*/
UINT32 ETH_PhyWrite(In UINT32 u32PhyRegIndex , In UINT32 u32PhyData )
{
	PHY_Conf_U PhyWhiteReg ;			
	UINT16 u16Count = 0;     
	while( u16Count < 1000 )
	{
		/*get the control register state*/
     		PhyWhiteReg.w32 = *(UINT32 *)ETH_MDIO_REG(0);
	    	if ( PhyWhiteReg.bits.finish == 1 )
    		{
    			PhyWhiteReg.bits.finish = 0;
    			PhyWhiteReg.bits.rw = 1;
    			PhyWhiteReg.bits.phy_exaddr = *(UINT32 *)ETH_MDIO_REG(2);/*the phy extern address register*/
    			PhyWhiteReg.bits.phy_inaddr = u32PhyRegIndex;
    			PhyWhiteReg.bits.cpu_data_in = u32PhyData;
			*( UINT32 *)ETH_MDIO_REG(0) = PhyWhiteReg.w32 ;
    			break;
    		}
    		u16Count++ ;
	}
	if( u16Count >= 1000 )
	{
		#ifdef ETHTest
			ETH_Print("ETH write content of phy failure\n");
		#endif
		return Failure;
	}
	u16Count = 0 ;
	while( u16Count < 1000 )
	{
		/*Write finish?*/
     		PhyWhiteReg.w32 = *(UINT32 *)ETH_MDIO_REG(0) ;
	    	if ( PhyWhiteReg.bits.finish == 1 )
    		{
			return Success;
    		}
    		u16Count++ ;
	}
	return Failure;
}

/*=======================================================================
 *  函数名称:UINT32 ETH_PhyRead(In UINT32 u32PhyIndex , In UINT32 u32PhyRegIndex , Out UINT32 *pu32PhyDataAddr )
 *  初稿完成:  2004/01/07   
 *  作    者: 
 *  函数功能: 从指定PHY芯片的指定寄存器读出值
 *  输入参数: u32PhyIndex－ PHY芯片的索引号
 *            u32PhyRegIndex－PHY芯片内的寄存器号
 *            *pu32PhyDataAddr－读取要输出的寄存器数据
 *  输出参数: 无
 *  返回类型:  
 *  其他说明: 
 *  调用函数:  
 *  主调函数:  
 *========================================================================*/
UINT32 ETH_PhyRead(In UINT32 u32PhyRegIndex , Out UINT32 *pu32PhyDataAddr )
{
	PHY_Conf_U PhyReadReg ;
	Read_PHY_Data_U PhyReadData;
	UINT16 u16Count = 0;     
	
	while( u16Count < 1000 )
	{
		/*get the control register state*/
     		PhyReadReg.w32 = *(UINT32 *)ETH_MDIO_REG(0) ;
    		if ( PhyReadReg.bits.finish == 1 )
    		{
           	 /*set the mdio register reading command*/
			PhyReadReg.bits.rw = 0 ;
			PhyReadReg.bits.finish = 0 ;
			PhyReadReg.bits.phy_exaddr = *(UINT32 *)ETH_MDIO_REG(2);/*the phy extern address register*/
			PhyReadReg.bits.phy_inaddr = u32PhyRegIndex ;
			PhyReadReg.bits.cpu_data_in = 0 ;
			*(UINT32 *)ETH_MDIO_REG(0) = PhyReadReg.w32;
            		break ;
    		}
    		u16Count++ ;
	}
	if( u16Count >= 1000 )
	{
		#ifdef ETHTest
			ETH_Print("ETH read content of phy failure\n");
		#endif
		return Failure;
	}
	u16Count = 0 ;
	while( u16Count < 1000 )
	{
		/*get the control register state*/
     		PhyReadReg.w32 = *(UINT32 *)ETH_MDIO_REG(0) ;
    		if ( PhyReadReg.bits.finish == 1 )
    		{
            		/*get the mdio register data*/
           	 	PhyReadData.w32 = *( UINT32 *)ETH_MDIO_REG(1) ;
            		*pu32PhyDataAddr = PhyReadData.bits.cpu_data_out;
            
            		#ifdef ETHTest
				ETH_Print("ETH_MDIO_REG(1)-ETH read content of phy is : %lx\n",*pu32PhyDataAddr);
	    		#endif

            		return Success;
    		}
    		u16Count++ ;
	}
	#ifdef ETHTest
		ETH_Print("ETH read content of phy failure\n");
	#endif
	return Failure;
}

/*=======================================================================
*							FILTER Control
*过滤优先级是:总体过滤使能>表内过滤控制>
*广播帧，组播帧，单播控制>表外过滤控制
*
=======================================================================*/

/*=======================================================================
 *  函数名称:void ETH_SetMACFiltEnable(In UINT32 u32FilterEnable)  
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:总体过滤使能。1->使能；0->禁止。
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
/*void ETH_SetMACFiltEnable(In UINT32 u32FilterEnable)  
{
	Set_FrameFilt_U MACFiltCtrl;
	MACFiltCtrl.w32 = *(UINT32 *) ETH_GLB_REG(4);
	MACFiltCtrl.bits.flt_en = u32FilterEnable;
	*(UINT32 *) ETH_GLB_REG(4) = MACFiltCtrl.w32;
}*/

#if 0
/*=======================================================================
 *  函数名称:void ETH_SetOutMACFiltEnable(In UINT32 u32OutFilterEnable)  
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:过滤表外帧过滤控制。1->接收；0->丢弃。
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetOutMACFiltEnable(In UINT32 u32OutFilterEnable)  
{
	Set_FrameFilt_U MACFiltCtrl;
	MACFiltCtrl.w32 = *(UINT32 *) ETH_GLB_REG(5);
	MACFiltCtrl.bits.outtab_ctrl = u32OutFilterEnable;
	*(UINT32 *) ETH_GLB_REG(5) = MACFiltCtrl.w32;
}
#endif
/*=======================================================================
 *  函数名称:UINT32 ETH_AddMACFiltFormContent(In UINT32 u32Index, In UINT32 u32MACLow, In UINT32 u32MACMiddle, In UINT32 u32MACHigh)  
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:写MAC过滤表内容
 *  输入参数:u32Index->添加内容索引，u32MACLow->MAC低16位，u32MACMiddle->MAC中16位，u32MACHigh->MAC高16位
 *  输出参数:
 *  返回类型:  return 0->reset fail; return 1->reset success.
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
UINT32 ETH_AddMACFiltFormContent(In UINT32 u32Index, In UINT32 u32MACLow, In UINT32 u32MACMiddle, In UINT32 u32MACHigh)  
{
	UINT32  *pu32Filter;
	#if 0
	switch (u32Index)
	{
		case 0 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB0;
			break;
		case 1 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB1;
			break;
		case 2: 
			pu32Filter = (UINT32 *) ETH_FLT_TAB2;
			break;
		case 3 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB3;
			break;
		case 4 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB4;
			break;
		case 5 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB5;
			break;
		case 6 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB6;
			break;
		case 7 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB7;
			break;
		case 8 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB8;
			break;
		case 9 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB9;
			break;
		case 10 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB10;
			break;
		case 11 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB11;
			break;
		case 12 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB12;
			break;
		case 13 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB13;
			break;
		case 14 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB14;
			break;
		case 15 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB15;
			break;
		case 16 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB16;
			break;
		case 17 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB17;
			break;
		case 18 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB18;
			break;
		case 19 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB19;
			break;
		case 20 : 
			pu32Filter = (UINT32 *) ETH_FLT_TAB20;
			break;
		default : 
			return 0;;
	}
	if ((u32Index & 0x1) != 0)
	{/*the index is odd number*/
		*pu32Filter= ((*pu32Filter & 0xFFFF) | u32MACLow << 16);
		pu32Filter ++;
		*pu32Filter = (u32MACMiddle | (u32MACHigh << 16));
	}
	else
	{
		*pu32Filter ++ = (u32MACLow | (u32MACMiddle << 16));
		*pu32Filter= (*pu32Filter & 0xFFFF0000) | u32MACHigh;
	}
	#endif
	if (u32Index > 20)
	{
		return 0;
	}
	pu32Filter = (UINT32 *) ETH_FLT_TAB(u32Index);
	*pu32Filter ++ = u32MACLow;
	*pu32Filter ++ = u32MACMiddle;
	*pu32Filter ++ = u32MACHigh;
	return 1;
}


/*=======================================================================
 *  函数名称:UINT32 ETH_ReadMACFiltFormContent(In UINT32 u32Index, In UINT32 u32MACLow, In UINT32 u32MACMiddle, In UINT32 u32MACHigh)  
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:读MAC过滤表内容
 *  输入参数:u32Index->添加内容索引，u32MACLow->MAC低16位，u32MACMiddle->MAC中16位，u32MACHigh->MAC高16位
 *  输出参数:
 *  返回类型:  return 0->reset fail; return 1->reset success.
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
UINT32 ETH_ReadMACFiltFormContent(In UINT32 u32Index, Out UINT32 *pu32MACLow, Out UINT32 *pu32MACMiddle, Out UINT32 *pu32MACHigh)  
{
	UINT32  *pu32Filter;
	if (u32Index > 20)
	{
		return 0;
	}
	pu32Filter = (UINT32 *) ETH_FLT_TAB(u32Index);
	*pu32MACLow = *pu32Filter ++;
	*pu32MACMiddle = *pu32Filter ++;
	*pu32MACHigh = *pu32Filter ;
	return 1;
}

/*=======================================================================
 *  函数名称:void ETH_SetMACInFiltCtrl(In UINT32 u32ReceiveCtrl)  
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:MAC过滤表表内过滤控制。0－20表示对应的21个表项，1表示接收，0表示丢弃
 *  输入参数:
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetInMACFiltCtrl_local(In UINT32 mac1_ctrl,In UINT32 mac0_ctrl,In UINT32 local_mac_ctrl)  
{
	Set_FrameFilt_U MACFiltCtrl;
	MACFiltCtrl.w32 = *(UINT32 *) ETH_GLB_REG(5);
	MACFiltCtrl.bits.mac1_ctrl = mac1_ctrl;
	MACFiltCtrl.bits.mac0_ctrl = mac0_ctrl;
	MACFiltCtrl.bits.local_mac_ctrl = local_mac_ctrl;
	*(UINT32 *) ETH_GLB_REG(5) = MACFiltCtrl.w32;
}


void ETH_SetInMACFiltenable(In UINT32 mac1_en,In UINT32 mac0_en,In UINT32 local_mac_en)  
{
	Set_FrameFilt_U MACFiltCtrl;
	MACFiltCtrl.w32 = *(UINT32 *) ETH_GLB_REG(5);
	MACFiltCtrl.bits.mac1_en = mac1_en;
	MACFiltCtrl.bits.mac0_en = mac0_en;
	MACFiltCtrl.bits.local_mac_en = local_mac_en;
	*(UINT32 *) ETH_GLB_REG(5) = MACFiltCtrl.w32;
}

/*=======================================================================
 *  函数名称:void ETH_SetMACFiltCtrlGroup(In UINT32 u32CtrlEnable,In UINT32 u32ReceiveCtrl)  
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:目的MAC组播帧接收控制
 *  输入参数:u32CtrlEnable:控制使能;1->控制有效，0->控制无效；u32ReceiveCtrl:1->接收、0->丢弃控制
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetMACFiltCtrl_destination(In UINT32 multi_ctrl,In UINT32 uni_ctrl,In UINT32 broad_ctrl)  
{
	Set_FrameFilt_U MACFiltCtrl;
	MACFiltCtrl.w32 = *(UINT32 *) ETH_GLB_REG(5);
	MACFiltCtrl.bits.multi_ctrl = multi_ctrl;
	MACFiltCtrl.bits.uni_ctrl = uni_ctrl;
	MACFiltCtrl.bits.broad_ctrl=broad_ctrl;
	*(UINT32 *) ETH_GLB_REG(5) = MACFiltCtrl.w32;
}

#if 0
/*=======================================================================
 *  函数名称:void ETH_SetMACFiltCtrlSingle(In UINT32 u32CtrlEnable,In UINT32 u32ReceiveCtrl)  
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:目的MAC单播帧接收控制
 *  输入参数:u32CtrlEnable:控制使能;1->控制有效，0->控制无效；u32ReceiveCtrl:1->接收、0->丢弃控制
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetMACFiltCtrlSingle(In UINT32 u32CtrlEnable,In UINT32 u32ReceiveCtrl)  
{
	Set_FrameFilt_U MACFiltCtrl;
	MACFiltCtrl.w32 = *(UINT32 *) ETH_GLB_REG(5);
	MACFiltCtrl.bits.uni_en = u32CtrlEnable;
	MACFiltCtrl.bits.uni_ctrl = u32ReceiveCtrl;
	*(UINT32 *) ETH_GLB_REG(5) = MACFiltCtrl.w32;
}
#endif
#if 0
/*=======================================================================
 *  函数名称:void ETH_SetMACFiltCtrlBroadcast(In UINT32 u32CtrlEnable,In UINT32 u32ReceiveCtrl)  
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:目的MAC广播帧接收控制
 *  输入参数:u32CtrlEnable:控制使能;1->控制有效，0->控制无效；u32ReceiveCtrl:1->接收、0->丢弃控制
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
void ETH_SetMACFiltCtrlBroadcast(In UINT32 u32CtrlEnable,In UINT32 u32ReceiveCtrl)  
{
	Set_FrameFilt_U MACFiltCtrl;
	MACFiltCtrl.w32 = *(UINT32 *) ETH_GLB_REG(5);
	MACFiltCtrl.bits.broad_en = u32CtrlEnable;
	MACFiltCtrl.bits.broad_ctrl = u32ReceiveCtrl;
	*(UINT32 *) ETH_GLB_REG(5) = MACFiltCtrl.w32;
}
#endif

/*=======================================================================
*							Receive and Transmit Frame
*
=======================================================================*/

/*=======================================================================
 *  函数名称:void ETH_ReadResendFrameDescribe(Out UINT32 *pu32ResendFrameDescribe)  
 *  初稿完成:2005/1/7   
 *  作    者:  
 *  函数功能:读取重传的帧描述子
 *  输入参数:
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
/*void ETH_ReadResendFrameDescribe(Out UINT32 *pu32ResendFrameDescribe)  
{
	*pu32ResendFrameDescribe = *(UINT32 *) ETH_GLB_REG(16);
}
*/

/*=======================================================================
 *  函数名称:UINT32 ETH_FrameReceive (Out UINT8 *pu8FrameReceiveData , Out UINT16 *pu16Length) 
 *  初稿完成:2005/1/8   
 *  作    者:  
 *  函数功能:完成的工作是CPU的收包功能。
 *  输入参数:数据保存地址，一次接收的帧的个数
 *  输出参数:返回1表示有包接收，返回0表示没有包接收
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
#if 0 
UINT32 ETH_FrameReceive (Out UINT8 *pu8FrameReceiveData ,Out UINT16 *pu16Length) 
{
	Frame_Describe_U ReadDescribEnInf;
	
	UINT8 *pu8ReceiveFrameAddr;
	UINT16 u16ReceiveFrameLength;
	UINT16 i, u32Cnt = 1;
	UINT32 CRC_Value = 0;
	/*UINT8 PackageTemp[2048] = {0};*/
	UINT16 u32FrameLenTemp = 0;
	
	for (i = u32Cnt; i > 0; i --)
	{
	  	if (*(UINT32 *) ETH_GLB_REG(12) != 0)
	    	{/*receive frame describe available*/
			ReadDescribEnInf.w32 = *(UINT32 *) ETH_GLB_REG(11);
			(UINT32)pu8ReceiveFrameAddr = ReadDescribEnInf.w32 & 0xFFFFF800;
			*pu16Length = (UINT16)ReadDescribEnInf.bits.fm_len;
			u16ReceiveFrameLength = *pu16Length;
			//ETH_Print("Enter Frame Normal Receive.\r\n");
			/*for(i=0;i<(u16ReceiveFrameLength + 3)/4;i++)
			{
				*(UINT32 *)(pu8FrameReceiveData+i*4) = *(UINT32 *)(pu8ReceiveFrameAddr+i*4);
			}*/
			u32FrameLenTemp = u16ReceiveFrameLength & 0xFFFC;
			memcpy ((UINT8 *)(pu8FrameReceiveData),(UINT8 *)(pu8ReceiveFrameAddr),(u32FrameLenTemp));

			if ((u16ReceiveFrameLength & 0x3) != 0)
			{
				 *(UINT32 *)(pu8FrameReceiveData+u32FrameLenTemp) = *(UINT32 *)(pu8ReceiveFrameAddr+u32FrameLenTemp);
			}

			/*memcpy ((UINT8 *)pu8FrameReceiveData,(UINT8 *)pu8ReceiveFrameAddr,u16ReceiveFrameLength);*/
			if (*(UINT32 *)(0xf3f0f004) == 1)
			{
			    /*memcpy (PackageTemp,pu8FrameReceiveData,u16ReceiveFrameLength);*/
                                
			    /*add the crc for received package*/
			    /*for(j = 0; j < u16ReceiveFrameLength; j ++)
			    {
			        printf("0x%x,",PackageTemp[j]);
			    }*/
			    CRC_Value =  caculate_crc32(pu8FrameReceiveData, (UINT32)(u16ReceiveFrameLength - 4));

                        /*CRC_Value = Calc_CRC32( PackageTemp, (UINT32)(u16ReceiveFrameLength - 4), 1 );*/
			    /*printf("CRC_Value:0x%08x\r\n",CRC_Value);*/
			    /*printf("length:%d\r\n",u16ReceiveFrameLength);*/
			    /*LastCRC = (((UINT32)(PackageTemp[u16ReceiveFrameLength-4])) << 24) | 
			        (((UINT32)(PackageTemp[u16ReceiveFrameLength-3])) << 16) | 
			        (((UINT32)(PackageTemp[u16ReceiveFrameLength-2])) << 8) | 
			        (((UINT32)(PackageTemp[u16ReceiveFrameLength-1])) << 0);*/
			    /*printf("LastCRC:0x%08x\r\n",LastCRC);*/
			    if (((UINT8)(CRC_Value & 0xff) != *(pu8FrameReceiveData + 
			        u16ReceiveFrameLength-1)) ||
			        ((UINT8)((CRC_Value & 0xff00) >> 8) != 
			        *(pu8FrameReceiveData+u16ReceiveFrameLength-2))||
			        ((UINT8)((CRC_Value & 0xff0000)>>16) != 
			        *(pu8FrameReceiveData+u16ReceiveFrameLength-3))||
			        ((UINT8)((CRC_Value & 0xff000000)>>24) != 
			        *(pu8FrameReceiveData+u16ReceiveFrameLength-4)))
			        {
			            printf("CRC Error!\r\n");
			            *(UINT32 *)(0xf3f0f008) += 1;
			        }

			}
			
			/*add the crc for received package*/
                    /*CRC_Value = Calc_CRC32( pu8ReceiveFrameAddr, (UINT32)(u16ReceiveFrameLength - 4), 1 );
			if (((UINT8)(CRC_Value & 0xff) != *(pu8ReceiveFrameAddr + 
			u16ReceiveFrameLength-1)) ||
			    ((UINT8)((CRC_Value & 0xff00) >> 8) != 
			    *(pu8ReceiveFrameAddr+u16ReceiveFrameLength-2))||
			    ((UINT8)((CRC_Value & 0xff0000)>>16) != 
			    *(pu8ReceiveFrameAddr+u16ReceiveFrameLength-3))||
			    ((UINT8)((CRC_Value & 0xff000000)>>24) != 
			    *(pu8ReceiveFrameAddr+u16ReceiveFrameLength-4)))
			    {
			        printf("CRC Error!\r\n");
			    }*/
			
                   /*=========================*/

                   
	       		/*receive finish, set bit 0 as 1*/
			*(UINT32 *) ETH_GLB_REG(12) = 0;
	       		/*clear the interrupt status for not enter again*/

                    if (1 == *(volatile UINT32 *)(0xf3f0f014))
                    {
                            printf("R");
                    }
			*(UINT32 *)(0xf3f0f000) += 1;
	       		
			#ifdef ETHTest
				ETH_Print("Enter Frame Normal Receive.\r\n");
			#endif
			return 1;
		}
	}
	return 0;	
}

#endif





/*=======================================================================
 *  函数名称:UINT32 ETH_FrameReceive (Out UINT8 *pu8FrameReceiveData , Out UINT16 *pu16Length) 
 *  初稿完成:2005/1/8   
 *  作    者:  
 *  函数功能:完成的工作是CPU的收包功能。
 *  输入参数:数据保存地址，一次接收的帧的个数
 *  输出参数:返回1表示有包接收，返回0表示没有包接收
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
UINT32 ETH_FrameReceive (Out UINT8 *pu8FrameReceiveData ,Out UINT16 *pu16Length) 
{
	
	UINT8 *pu8ReceiveFrameAddr;
	UINT16 u16ReceiveFrameLength;
	UINT16 i, u32Cnt = 1;
	//UINT32 u32ClearTheInterrupt = 0;
	Frame_Receive_Describe ReadDescribEnInf;
	for (i = u32Cnt; i > 0; i --)
	{
	  	if (*(UINT32 *) ETH_GLB_REG(13) != 0)
	    	{/*receive frame describe available*/
			ReadDescribEnInf.w32 = *(UINT32 *) ETH_GLB_REG(12);
//			(UINT32)pu8ReceiveFrameAddr = (ETH_FRAM_ADDR +(ReadDescribEnInf.bits.reseive_index <<11));
                pu8ReceiveFrameAddr = (UINT8 *)(ETH_FRAM_ADDR +(ReadDescribEnInf.bits.reseive_index <<11));
			*pu16Length = ReadDescribEnInf.bits.fm_len;
			u16ReceiveFrameLength = *pu16Length;
			//dbg_info("Enter Frame Normal Receive.\r\n");
			/*
			for(i=0;i<(u16ReceiveFrameLength + 3)/4;i++)
			{
				*(UINT32 *)(pu8FrameReceiveData+i*4) = *(UINT32 *)(pu8ReceiveFrameAddr+i*4);
				//dbg_info("0x%8lx, ",*(UINT32 *)(pu8FrameReceiveData+i*4));
			}
			*/
			memcpy(pu8FrameReceiveData,pu8ReceiveFrameAddr,u16ReceiveFrameLength);
	       		/*receive finish, set bit 0 as 1*/
			*(UINT32 *) ETH_GLB_REG(13) = 0;
	       	  *(volatile UINT32 *)ETH_GLB_REG(14) = (UINT32)pu8ReceiveFrameAddr;	
	       		/*clear the interrupt status for not enter again*/
			//u32ClearTheInterrupt = *(UINT32 *) ETH_GLB_REG(0);
			//*((volatile UINT32 *) ETH_GLB_REG(0)) = (u32ClearTheInterrupt & (0xFFFFFFFF));
			//printf("recv pack addr =%x\n",pu8ReceiveFrameAddr);
			#if 0
			if((*(volatile UINT32 *)(0xf3000000) ==0xffffffff)&&(u16ReceiveFrameLength >=60))
			{
				for(i =0 ;i < 60;i +=6)
				{
				printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
		              pu8FrameReceiveData[i], pu8FrameReceiveData[i+1], pu8FrameReceiveData[i+2], pu8FrameReceiveData[i+3], pu8FrameReceiveData[i+4], pu8FrameReceiveData[i+5]);
				}
				printf("recv pack len =%d\n",u16ReceiveFrameLength);	
				printf("\n\n\n\n\n");
			}
	       	#endif
	       	//printf("recv pack len =%d\n",u16ReceiveFrameLength);
			#ifdef ETHTest
				dbg_info("Enter Frame Normal Receive.\r\n");
			#endif
			return 1;
		}
	}
	return 0;	
}



UINT32 ETH_FrameTransmit (In UINT16 u16FrameLen, In UINT8 *pu8FrameTransmitData) 
{
             queue_Recv_Tran_uses rtuses;
		rtuses.w32=(*(volatile UINT32 *) ETH_GLB_REG(18));

              while(rtuses.bits.eq_in_rdy == 0)
                {
             
               
				printf("send block\n");
	
               	rtuses.w32=(*(volatile UINT32 *) ETH_GLB_REG(18));
                }
			
	 	(*(volatile UINT32 *) ETH_GLB_REG(8)) = (UINT32)pu8FrameTransmitData;
              (*(volatile UINT32 *) ETH_GLB_REG(9)) = (UINT32)u16FrameLen;
           //   printf("send packet len =%d \n",u16FrameLen);
               return 0;
}








/*=======================================================================
 *  函数名称:UINT32 ETH_FrameTransmit (In UINT32 u32FrameLen, In UINT32 *pu32FrameTransmitData) 
 *  初稿完成:2005/1/25   
 *  作    者:  
 *  函数功能:完成的工作是CPU的发包功能
 *  输入参数:等待发送的帧。
 *	 		 发送的形式，包括（发给下行口――1，上行口――2，广播帧――3）
 *			 待发送帧描述子
 *  输出参数:
 *  返回类型:  无
 *  其他说明: 
 *  调用函数:  建议 
 *  主调函数:  建议
 *========================================================================*/
 
#if 0 
UINT32 ETH_FrameTransmit (In UINT16 u16FrameLen, In UINT8 *pu8FrameTransmitData) 
{
	
	Frame_Describe_U TransFrameDescribe;
	Frame_OutAddr_U FrameOutTailAddr;
	
	UINT8 *pu8TransmitFrameAddr;
	UINT16 u16TransmitFrameLength;
	UINT32 i, u32Count = 0;

	TransFrameDescribe.bits.fm_len = u16FrameLen;
	FrameOutTailAddr.w32 = *(UINT32 *) ETH_GLB_REG(10);

	u32Count = 0;
	while (u32Count < 10000)
	{
		FrameOutTailAddr.w32 = *(UINT32 *) ETH_GLB_REG(10);
		if (FrameOutTailAddr.bits.tail_vld_out == 1)
		{
			TransFrameDescribe.bits.fm_head_addr = (*(UINT32 *) ETH_GLB_REG(14) + (FrameOutTailAddr.bits.tail_addr_out << 11)) >> 11;
			pu8TransmitFrameAddr =(UINT8 *) ((UINT32) TransFrameDescribe.bits.fm_head_addr << 11);
			u16TransmitFrameLength = u16FrameLen;	
			for (i = 0 ; i < u16TransmitFrameLength ; i++)
			{
				*(pu8TransmitFrameAddr+i) = *(pu8FrameTransmitData+ i);
			}
			//*(UINT32 *) ETH_GLB_REG( 8 ) = 1;
			//printf("the  FrameDescribe.w32 is 0x%8lx\r\n", TransFrameDescribe.w32);

			//printf("the glb reg 8 add is 0x%8x\r\n",(UINT32 *) ETH_GLB_REG( 8 ));
			*(UINT32 *) ETH_GLB_REG( 8 ) = TransFrameDescribe.w32;
			*(UINT32 *) ETH_GLB_REG( 9 ) = 1;
			//*(UINT32 *)(0xA0020220) = FrameDescribe.w32;
			//*((UINT32 *)(0xA0020224)) = 1;
			break;
		}
		u32Count++;
	}
	//printf("the transmit ok!\r\n");
	return 1;
}

#endif


    


/*test cpu continute add by tbb 20050609*/
void TestCpuSentFrame(In UINT32 u32SendNumber,In UINT16 u16FrameLen, In UINT8 *pu8FrameTransmitData)
{
	UINT32 i;
	queue_Recv_Tran_uses rtuses;
	for (i = 0; i < u32SendNumber; i++)
	{
	 rtuses.w32=(*(volatile UINT32 *) ETH_GLB_REG(18));
	while(rtuses.bits.eq_in_rdy == 0)
		                {
		           			rtuses.w32=(*(volatile UINT32 *) ETH_GLB_REG(18));
		                }
	
	(*(UINT32 *) ETH_GLB_REG(8)) = (int)(*pu8FrameTransmitData);
        (*(UINT32 *) ETH_GLB_REG(9)) = u16FrameLen;
	}
}




/*use for test at tornado tools 20050608*/
void PrintMemory(unsigned int * Addr)
{
	unsigned int *tmp;
	unsigned int i;
	
	printf("\n\n");
	for(i=0; i<8; i++)
	{
		tmp = Addr+i*4;
		printf("%8x: %8x %8x %8x %8x\n",  (unsigned int)(Addr+i*4),  *tmp, *(tmp+1), *(tmp+2), *(tmp+3));
	}
	
	printf("\n");
}


UINT32 printOneMemory(unsigned int *Addr)
{
	printf("The value is : 0x%08x=%8d\r\n",*Addr,*Addr);
	return *Addr;
}

void SetRegister(In UINT32 *pu32Addr, In UINT32 u32Value)
{
	unsigned int i;

	*pu32Addr = u32Value;

	for (i = 0 ; i < 3000 ; i ++);
	
	printf("\r\nThe Register of 0x%8lx is : 0x%8lx\r\n", (UINT32)(UINT32 *)pu32Addr, u32Value);
}

#if 0
/*use for tornado for write and read register on 20050622 by tangbenbing*/
void md(unsigned int * Addr)
{
	unsigned int *tmp;
	unsigned int i;
	
	printf("\n\n");
	for(i=0; i<8; i++)
	{
		tmp = Addr+i*4;
		printf("%8x: %8x %8x %8x %8x\n",  (unsigned int)(Addr+i*4),  *tmp, *(tmp+1), *(tmp+2), *(tmp+3));
	}
	
	printf("\n");
}

void mw(In UINT32 *pu32Addr, In UINT32 u32Value)
{
	unsigned int i;

	*pu32Addr = u32Value;

	for (i = 0 ; i < 3000 ; i ++);
	
	printf("\r\nThe Register of 0x%8lx is : 0x%8lx\r\n", (UINT32)(UINT32 *)pu32Addr, u32Value);
}
#endif
void transmit_frame_addr(void)
{
    *(UINT32 *) ETH_GLB_REG(13)=((UINT32)ETH_CPU_receive);/*CPU配置输入队列的存储地址*/
}

/*ETH_SetioutQueuelen modify by h60016335*/
void ETH_SetioutQueuelen(In UINT32 in_len,In UINT32 out_len)
{
   Frame_queue_config ip_ep_config;
   ip_ep_config.w32= (*(UINT32 *) ETH_GLB_REG(6));
   ip_ep_config.bits.iq_len = in_len;
   ip_ep_config.bits.eq_len = out_len;
   (*(UINT32 *) ETH_GLB_REG(6)) = ip_ep_config.w32;
}

/*ETH_FrameTransmit for one buffer modify by h60016335*/

UINT32 ETH_FrameTransmitPacket (In UINT32 u32FrameLen, In UINT32 * pu32FrameTransmitAddr) 
{
    queue_Recv_Tran_uses rtuses;
    do
    {
        rtuses.w32=(*(UINT32 *) ETH_GLB_REG(16));
    }
    while(rtuses.bits.tran>=32);/*只有在输出队列没有满的时候才会发包*/
    (*(UINT32 *) ETH_GLB_REG(7))=*pu32FrameTransmitAddr; /*发包的地址*/
    (*(UINT32 *) ETH_GLB_REG(8))=(u32FrameLen&0x00000fff);/*包长*/
    return 1;
}

void ETH_SET_MAC(unsigned char*mac)
{

	unsigned char t[4] = {0};
	t[0] = mac[1];
	t[1] = mac[0];
	*(UINT32 *)ETH_GLB_REG(16) = *((UINT32*)t);
	t[0] = mac[5];
	t[1] = mac[4];
	t[2] = mac[3];
	t[3] = mac[2];
	*(UINT32 *)ETH_GLB_REG(15) = *((UINT32*)t);
}

void ETH_SetMACFiltCtrl(UINT32 value)
{
	*(UINT32 *)ETH_GLB_REG(5) =value;
}
#include "Hisilicon-ETH.h"

#ifdef HI3560_V100
#define ETH_SOFT_RESET_REG IO_ADDRESS(0x101e0000 + 0x38)
#define ETH_SOFT_RESET_VAL 0x40000000
#endif
#if defined(HI3511_DEMO)|| defined(HI3511_UMAP) || defined(HI3511_V100_DEMO)
#define ETH_SOFT_RESET_REG IO_ADDRESS(0x101e0000 + 0x1c)
#define ETH_SOFT_RESET_VAL 0x1000
#endif

void eth_soft_reset(void)
{
        unsigned long t,i=100;
#if defined(HI3560_V100)
            t = *(unsigned int*)(ETH_SOFT_RESET_REG); t |= ETH_SOFT_RESET_VAL;  *(unsigned int*)(ETH_SOFT_RESET_REG) = t;
            while(i--);
            t = *(unsigned int*)(ETH_SOFT_RESET_REG); t &= ~ETH_SOFT_RESET_VAL; *(unsigned int*)(ETH_SOFT_RESET_REG) = t;
            i = 100;    
            while(i--);
#endif
#if defined(HI3511_DEMO)||defined(HI3511_UMAP) || defined(HI3511_V100_DEMO)
            t = *(unsigned int*)(ETH_SOFT_RESET_REG); t &= ~ETH_SOFT_RESET_VAL; *(unsigned int*)(ETH_SOFT_RESET_REG) = t;
            while(i--);
            t = *(unsigned int*)(ETH_SOFT_RESET_REG); t |= ETH_SOFT_RESET_VAL;  *(unsigned int*)(ETH_SOFT_RESET_REG) = t;
            i = 100;    
            while(i--);
#endif
}


#if 1
void ETH_changcontrol(UINT32 value1,UINT32 value2)
{
    UINT32 value_temp ;
    value_temp = *(volatile UINT32 *)0x101e0040;
    value_temp |=(1<<value1); 
    *(volatile UINT32 *)0x101e0040 = value_temp;
    value_temp = *(volatile UINT32 *)0x101e0044;
    value_temp |=(1<<value2);
    *(volatile UINT32 *)0x101e0044 = value_temp;
    
}
#endif
void ETH_changcontrol__(UINT32 value1)
{
    UINT32 value_temp ;
    value_temp = *(volatile UINT32 *)0x101e0040;
    value_temp |=(1<<value1); 
    *(volatile UINT32 *)0x101e0040 = value_temp;
}


#endif

