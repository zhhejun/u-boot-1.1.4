/************************************************************************
 *                                                                      *
 *                          ETH_Struct.H										*
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

#ifndef ETH_STRUCT_H
#define ETH_STRUCT_H


#ifndef UINT8
	#define UINT8 unsigned char
#endif

/*=======================================================================
*							GLB REG
*
*
=======================================================================*/

/*CPU中断使能寄存器 modified*/
typedef union   
{
	struct
	{
                UINT32 int_cpu_rx			:1;	/*ETH有帧等待CPU接收                          */
                UINT32 int_tx_cpu			:1;	/*ETH发送完来自CPU的一帧数据指示              */
                UINT32 int_free_cpu		:1;	/*发送队列空间完全释放指示中断                    */
                UINT32 int_link_ch			:1;	/*ETH连接状态变化中断                         */
                UINT32 int_speed_ch		:1;	/*ETH速度模式变化中断                         */
                UINT32 int_duplex_ch		:1;	/*ETH双工模式变化中断                         */
                UINT32 int_mdio_finish		:1;	/*MDIO完成CPU操作指示                        */
                UINT32 int_err_bus			:1;	/*总线操作出错指示中断信号                        */
                UINT32 int_retry			:1;	/*端口状态改变中断指示                        */
		UINT32 reserved			:23;	/*     */
	}bits ;
	UINT32 w32 ;
}CPU_Int_U;

/*复位后硬件初始化完成标志寄存器*/
//typedef union   
//{
//	struct
//	{
//                UINT32 fd_init_rdy    	:1;	/*frame describe FIFO initial finish, 1 for finish */
//		UINT32 fit_init_rdy		:1;	/*帧过滤信息表初始化完成指示，1表示完成*/
//               UINT32 statis_init_rdy    :1;	/*统计计数初始化完成指示，1表示初始化完成*/
//		UINT32 reserved       	:29;	/*     */
//	}bits ;
//	UINT32 w32 ;
//}Init_OnResetInf_U;

/*端口RMII/MII模式选择寄存器*/
typedef union   
{
	struct
	{
		UINT32 out_endian	:1;		/*ETH读SDRAM大小端配置，1小端，0大端*/
		UINT32 in_endian	:1;		/*ETH写SDRAM大小端配置，1小端，0大端*/
		UINT32 reserved  	:30;   	/*     */
	}bits ;
	UINT32 w32 ;
}Set_RMII_Endian_U;

#if 0
/*帧过滤配置寄存器*/
typedef union   
{
	struct
	{
		UINT32 intab_ctrl 	:21;		/*对于目的MAC与表内对应项匹配帧接收控制，依次对应21个表项，1表示接收，0表示丢弃*/
		UINT32 multi_ctrl 	:1;		/*对于目的MAC地址最高byte为奇数的组播帧接收控制，1表示接收，0表示丢弃*/
		UINT32 uni_ctrl 	:1;		/*对于目的MAC地址最高byte为偶数的单播帧接收控制，1表示接收，0表示丢弃*/
		UINT32 broad_ctrl 	:1;		/*对于目的MAC地址为48'hff_ff_ff_ff_ff_ff的广播帧接收控制，1表示接收，0表示丢弃*/
		UINT32 outtab_ctrl 	:1;		/*对于过滤信息表中无对应表项的帧接收控制，1表示接收，0表示丢弃*/
		UINT32 multi_en 	:1;		/*对于目的MAC地址最高byte为奇数的组播帧接收控制使能，1表示接收控制有效，0表示接收控制无效*/
		UINT32 uni_en 	:1;		/*对于目的MAC地址最高byte为偶数的单播帧接收控制使能，1表示接收控制有效，0表示控制无效*/
		UINT32 broad_en 	:1;		/*对于目的MAC地址为48'hff_ff_ff_ff_ff_ff的广播帧接收控制使能，1表示接收控制有效，0表示接收控制无效*/
		UINT32 flt_en 		:1;		/*过滤使能控制，1表示对输入帧进行过滤检查，0表示不进行过滤检查全部接收*/
		UINT32 reserved  	:3;   		/*     */
	}bits ;
	UINT32 w32 ;
}Set_FrameFilt_U;

#endif

/*帧过滤配置寄存器*/
typedef union   
{
	struct
	{
		UINT32 multi_ctrl 	:1;		/*对于目的MAC地址最高byte为奇数的组播帧接收控制，1表示接收，0表示丢弃*/
		UINT32 uni_ctrl 	:1;		/*对于目的MAC地址最高byte为偶数的单播帧接收控制，1表示接收，0表示丢弃*/
		UINT32 broad_ctrl 	:1;		/*对于目的MAC地址为48'hff_ff_ff_ff_ff_ff的广播帧接收控制，1表示接收，0表示丢弃*/
		UINT32 mac1_ctrl 	:1;		/*对接收帧的目的mac为mac1配置帧的控制*/
		UINT32 mac0_ctrl 	:1;		/*对接收帧的目的mac为mac0配置帧的控制*/
		UINT32 local_mac_ctrl 	:1;		/*对接收帧的目的mac为本机mac的控制*/
		UINT32 mac1_en   :1;      /*mac1过滤表使能控制*/
		UINT32 mac0_en 		:1;		/*mac0过滤表使能控制*/
        UINT32 local_mac_en :1;  /*本机mac的过滤使能控制*/
       // UINT32 flt_en  :1;       /*过滤使能控制，1表示对所有输入帧进行过滤检查*/
		UINT32 reserved  	:23;   		/*     */
	}bits ;
	UINT32 w32 ;
}Set_FrameFilt_U;




//start modify by h60016335
 





/*帧过滤配置寄存器*/
typedef union   
{
	struct
	{
		UINT32 eq_len 	:6;		/*输出队列长度配置*/
		UINT32 reservedl :2;    /*  */
		UINT32 iq_len  	:6;		/*输入队列长度配置*/
		UINT32 reservedh  	:18;   	/*     */
	}bits ;
	UINT32 w32 ;
}Set_FrameQueueLength_U;

/*帧流控配置寄存器*/
typedef union   
{
	struct
	{
		UINT32 qlimit_down 	:6;		/*接收队列解除流控下限*/
		UINT32 reservedl    :2;    /*  */   
		UINT32 qlimit_up	  	:6;		/*接收队列流控上限*/
		UINT32 reservedh  		:18;   	/*     */
	}bits ;
	UINT32 w32 ;
}Set_PauseFrameLimitLevel_U;


/*CPU清空输出队列寄存器*/
typedef union   
{
	struct
	{
                UINT32 Flush_in		:1;		/*CPU清空输出队列信号                         */
                UINT32 reserved		:31;		/*	*/
	}bits ;
	UINT32 w32 ;
}CPU_ClearQueue_U;

/*设定硬件重传帧次数寄存器*/
#if 0
typedef union   
{
	struct
	{
                UINT32 retry_time	:4;		/*TXFIFO下溢出允许重传次数(打到这个次数会产生中断) */
                UINT32 reserved	:28;	
	}bits ;
	UINT32 w32 ;
}Frame_RetryTimes_U;
#endif

/*队列使用长度寄存器*/
typedef union   
{
	struct
	{
                UINT32 eq_cnt		:6;		/*输出队列使用长度  */
                UINT32 reservedl :2 ;                   
                UINT32 iq_cnt		:6;		/*输入队列使用长度 */
                UINT32 reservedh	:18;		/*	*/
	}bits ;
	UINT32 w32 ;
}Queue_UseLength_U;


/*=======================================================================
*							MAC REG
*
*
=======================================================================*/

/*设置帧结构的寄存器*/
typedef union
{
	struct
	{
		UINT32 para			:16;  	/*输出流控帧间隔参数，缺省为16'h1fff*/
		UINT32 ipg			:7;  		/*帧间隙，缺省为96*/
		UINT32 pre_cnt_limit	:3;   		/*连续检测到帧前导码的个数，缺省为3*/
		UINT32 reserved      	:6;   		/*     */
	}bits ;
	UINT32 w32 ;
}Frame_Para_U;





/*端口软件复位寄存器*/
/*typedef union
{
	struct
	{
		UINT32 reset_port		:1;	//端口复位，缺省为1'b0，1有效
		UINT32 reserved       	:31;	
	}bits ;
	UINT32 w32 ;
}Port_Reset_U;*/

/*CPU设定端口速度、链接、双工寄存器/清除模式改变指示信号寄存器*/
typedef union
{
	struct
	{
		UINT32 duplex_stat_dio	:1;   	/*CPU设定双工模式，缺省为0*/
		UINT32 link_stat_dio	:1;   	/*CPU设定链接状态，缺省为0*/
		UINT32 speed_stat_dio	:1;   	/*CPU设定速度模式，缺省为0*/
		UINT32 reserved       	:29;	/*     */
	}bits ;
	UINT32 w32 ;
}Set_CPUSetSpeLinDup_U;

/*读取端口速度、链接、双工寄存器*/
typedef union
{
	struct
	{
		UINT32 duplex_stat		:1;   	/*CPU设定双工模式，缺省为0*/
		UINT32 link_stat		:1;   	/*CPU设定链接状态，缺省为0*/
		UINT32 speed_stat		:1;   	/*CPU设定速度模式，缺省为0*/
		UINT32 statis_state	:7;   	/*统计计数模块STATIS当前状态机状态，只读*/
		UINT32 reserved       	:22;	/*     */
	}bits ;
	UINT32 w32 ;
}Set_CPUReadSpeLinDup_U;


/*=======================================================================
*							MAC0 REG
*
*
=======================================================================*/

/*端口的端口配置寄存器*/
typedef union
{
	struct
	{
		UINT32 len_max		:11;		/*端口允许接收的最大帧长，缺省为1518                  */
		UINT32 rx_min_thr		:6;		/*端口允许接收的最小帧长，缺省为42                    */
		UINT32 rx_shframe_en	:1;		/*端口短帧接收使能信号，缺省为1                       */
		UINT32 pause_en		:1;		/*端口流控帧发送使能信号，缺省为1                     */
		UINT32 ex_loop_en		:1;		/*端口外环回使能信号                                  */
		UINT32 in_loop_en		:1;		/*端口内环回使能信号                                  */
		UINT32 colthreshold	:4;		/*端口冲突次数统计，缺省为1，表示发送出现一次冲突的帧的个数*/
		UINT32 cntr_roll_dis	:1;		/*端口统计计数环回使能信号，缺省为0                   */
		UINT32 cntr_clr_all		:1;		/*端口统计计数器清空信号，缺省为0                     */
		UINT32 cntr_rdclr_en	:1;		/*端口统计计数器读清空使能信号，缺省为0               */
		UINT32 crcgen_dis		:1;		/*CRC生成禁止控制寄存器，1表示输出帧不计算CRC，缺省为0 */
		UINT32 add_pad_en		:1;		/*发送时自动添加pad使能，为1自动添加pad，为0时不添加pad */
		UINT32 reserved       	:2;   		/*     */
	}bits ;
	UINT32 w32 ;
}Port_Conf_U;

/*=======================================================================
*							MDIO REG
*
*
=======================================================================*/


/*设置端口设定状态选择信息寄存器*/
typedef union
{
	struct
	{
		UINT32 stat_ctrl	:1;	/*外部端口工作状态信息选择控制寄存器， 
							1'b1:使用CPU设定的状态信息
							1'b0: 使用从MDIO接口获得的状态信息
							*/
		UINT32 reserved	:31;	/*     */
	}bits ;
	UINT32 w32 ;
}Port_Stat_Choice_U;


/*PHY芯片配置寄存器*/
typedef union   
{
	struct
	{
                UINT32 phy_inaddr	:5;	/*对外部操作的PHY的内部地址。PHY内部有32个寄存器。用5位二进制数来表示。缺省为0*/
                UINT32 frq_dv		:1;	/*对外部PHY进行读写操作时，对MDC的分频系数。3位，*/
                /*用来实现对MDIO的分频值从2分频至16分频。缺省为0*/                                                                   
                UINT32 reservedl :2 ;/* */
                UINT32 phy_exaddr	:5;	/*对外部操作的PHY的对应的外部物理地址。*/
                					/*一个MDIO可以对外面的27个PHY进行读写访问。每个PHY有一个相应的地址。缺省为0*/
                UINT32 rw		:1;	/*用来表示对PHY的访问为读操作还是写操作。高表示写操作，低为读操作。*/
                UINT32 reservedh	:1;	/*                                                   */
                UINT32 finish		:1;	/*完成对PHY的读/写操作后，会置1。当要进行第二次读写操作时，*/
                					/*CPU会先对该位进行写清零。缺省为1                     */
		UINT32 cpu_data_in:16;	/*用于MDIO模块对PHY进行对写操作的数据寄存器，进行写操作时，*/
							/*CPU将要对MDIO写入的16数据先写入到该寄存器中。        */
	}bits ;
	UINT32 w32 ;
}PHY_Conf_U;

/*PHY上供读取的数据保存寄存器,只读*/
typedef union   
{
	struct
	{
		UINT32 cpu_data_out	:16;   	/*用于MDIO模块对PHY进行对读操作的数据寄存器，*/
									/*将从PHY读取的16位数据将写入到该寄存器中。*/
		UINT32 reserved		:16;		/*     */
	}bits ;
	UINT32 w32 ;
}Read_PHY_Data_U;

/*设置PHY地址寄存器*/
typedef union   
{
	struct
	{
		UINT32 phy_addr0		:5;   	/*PHY0的外部物理地址，缺省为5'd0*/
		UINT32 reserved       	:27;	/*     */
	}bits ;
	UINT32 w32 ;
}Set_phy_addr_U;

/*PHY中速度、链接、双工模式的保存寄存器*/
typedef union   
{
	struct
	{
                UINT32 duplex_mdio2mac   	:1;	/*从MDIO接口得到的端口双工工作状态，处于半双工或全双工方式。*/
                							/*1为全双工方式。以bitmap的形式，每一位对应一个端口。         */
                UINT32 link_mdio2mac     	:1;	/*从MDIO接口得到的链接状态指示。1为链接状态，0为未链接状态。*/
                							/*以bitmap的形式，每一位对应一个端口。                        */
		UINT32 speed_mdio2mac	:1;	/*从MDIO接口得到的端口速度工作状态，处于10M或100M方式。*/
									/*1为100M工作方式，0为10M方式。以bitmap的形式，每一位对应一个端口。*/
		UINT32 reserved       		:29;	/*     */
	}bits ;
	UINT32 w32 ;
}PHY_Read_SpeLinDup_U;

/*PHY芯片状态寄存器所在地址的配置寄存器*/
typedef union   
{
	struct
	{
                UINT32 duplex_index      :4;	/*PHY芯片状态寄存器中存储双工信息的偏移地址，缺省值按照Intel9785设置*/
                UINT32 link_index        	:4;	/*PHY芯片状态寄存器中存储链接信息的偏移地址，缺省值按照Intel9785设置*/
                UINT32 speed_index       :4;	/*PHY芯片状态寄存器中存储速度信息的偏移地址，缺省值按照Intel9785设置*/
		        UINT32 internal_addr_duplex :5;  /*PHY芯片存储双工信息的的寄存器地址，缺省值按照Intel9785设置*/
		        UINT32 internal_addr_link :5;  /*PHY芯片存储链接信息的的寄存器地址，缺省值按照Intel9785设置*/
		        UINT32 internal_addr_speed :5; /*PHY芯片存储速度信息的的寄存器地址，缺省值按照Intel9785设置*/
		UINT32 reserved       	:5;	/*     */
	}bits ;
	UINT32 w32 ;
}PHY_Stat_Addr_U;

/*=======================================================================
*							Receive and transmit frame REG
*
*
=======================================================================*/
/*帧描述子保存寄存器*/
typedef union
{
    struct
    {
        UINT32 transmit_addr :32;  /*CPU添加到输出队列发送帧的起始地址*/
    }bits;
    UINT32 w32;
}Frame_Transmit_Addr;

/*帧描述子保存寄存器*/
typedef union
{
    struct
    {
        UINT32 fm_len :12; /*帧长度，以字节为单位*/
        UINT32 reserved :20;    
    }bits;
    UINT32 w32;
}Frame_Transmit_Describe;


typedef union
{
    struct
    {
        UINT32 fm_len :12;/*输入队列待接收帧帧长度*/
        UINT32 reservedl :4;
        UINT32 reseive_index :6;/*输入队列待接收帧的地址索引*/
        UINT32 reservedh :10;    
    }bits;
    UINT32 w32;
}Frame_Receive_Describe;


typedef union
{
    struct
    {
        UINT32 iq_len :6;/*接收队列长度配置*/
        UINT32 reservedl :2;
        UINT32 eq_len  :6;/*发送队列长度配置*/
        UINT32 reservedh  :18;
    }bits;
    UINT32 w32;
}Frame_queue_config;

typedef union
{
    struct
    {
        UINT32 tran :6;  /*输出队列使用情况*/ 
        UINT32 reservedl :2;
        UINT32 recv :6; /*输入队列使用情况*/
        UINT32 reservedm :2;
        UINT32 cpu_cnt:6;/*输入队列对CPU分配的可用帧首地址计数*/
        UINT32 reservedh1:2;
        UINT32 eq_in_rdy:1;/*cpu可以配置输出队列帧描述子（帧首地址及帧长）*/
        UINT32 cpuaddr_in_rdy:1;/*1：cpu可以配置输入队列帧起始地址*/
        UINT32 reservedh2 :6;          
    }bits;
    UINT32 w32;
}queue_Recv_Tran_uses;


typedef union
{
    struct
    {
        UINT32 avail :1;/*输入队列待接收帧帧描述子有效信号*/
        UINT32 reserved :31;
    }bits;
    UINT32 w32;
}Frame_Receive_Describe_Avail;


typedef union
{
    struct
    {
        UINT32 receive_addr :32;/*CPU 配置输入队列的每个空间对应的存储空间对应的地址*/
    }bits;
    UINT32 w32;
}Frame_Receive_Addr;









typedef union
{
    struct 
    {  UINT8 i0    :1;
        UINT8 i1    :1;
        UINT8 i2    :1;
        UINT8 i3    :1;
        UINT8 i4    :1;
        UINT8 i5    :1;
        UINT8 i6    :1;
        UINT8 i7    :1;
    }bits;
    UINT8 W8;
}UnsignInt8Bits;

typedef union
{
    struct 
    {
    UINT32 i0    :1;
    UINT32 i1    :1;
    UINT32 i2    :1;
    UINT32 i3    :1;
    UINT32 i4    :1;
    UINT32 i5    :1;
    UINT32 i6    :1;
    UINT32 i7    :1;
    UINT32 i8    :1;
    UINT32 i9    :1;
    UINT32 i10   :1;
    UINT32 i11   :1;
    UINT32 i12   :1;
    UINT32 i13   :1;
    UINT32 i14   :1;
    UINT32 i15   :1;
    UINT32 i16   :1;
    UINT32 i17   :1;
    UINT32 i18   :1;
    UINT32 i19   :1;
    UINT32 i20   :1;
    UINT32 i21   :1;
    UINT32 i22   :1;
    UINT32 i23   :1;
    UINT32 i24   :1;
    UINT32 i25   :1;
    UINT32 i26   :1;
    UINT32 i27   :1;
    UINT32 i28   :1;
    UINT32 i29   :1;
    UINT32 i30   :1;
    UINT32 i31   :1;
    }bits;
    UINT32 W32;
}UnsignInt32Bits ;

typedef union
{
    struct 
    {
    UINT32 i7    :1;
    UINT32 i6    :1;
    UINT32 i5    :1;
    UINT32 i4    :1;
    UINT32 i3    :1;
    UINT32 i2    :1;
    UINT32 i1    :1;
    UINT32 i0    :1;
    UINT32 i15    :1;
    UINT32 i14   :1;
    UINT32 i13   :1;
    UINT32 i12   :1;
    UINT32 i11   :1;
    UINT32 i10   :1;
    UINT32 i9   :1;
    UINT32 i8   :1;
    UINT32 i23   :1;
    UINT32 i22   :1;
    UINT32 i21   :1;
    UINT32 i20   :1;
    UINT32 i19   :1;
    UINT32 i18   :1;
    UINT32 i17   :1;
    UINT32 i16   :1;
    UINT32 i31   :1;
    UINT32 i30   :1;
    UINT32 i29   :1;
    UINT32 i28   :1;
    UINT32 i27   :1;
    UINT32 i26   :1;
    UINT32 i25   :1;
    UINT32 i24   :1;
    }bits;
    UINT32 W32;
}UnsignInt32Bits_Re ;



#endif
