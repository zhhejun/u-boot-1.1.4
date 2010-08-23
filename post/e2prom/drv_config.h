/******************************************************************************
Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_config.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2008/4/29
Last Modified :
Description   : boot memory used define
Function List :
History       :
* main\1    2008-04-29      modify for Hi3560
******************************************************************************/
#ifndef __DRV_CONFIG_H__
#define __DRV_CONFIG_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define DDR_START       CFG_DDR_PHYS_OFFSET
#define DDR_LENGTH      CFG_DDR_SIZE
#define DDR_END         (DDR_START + DDR_LENGTH)


#define DRAW_BUFF_LEN   0x100000
#define DRAW_BUFF_ADDR  (DDR_END - DRAW_BUFF_LEN)

#define VO_BUFF1_LEN    0x100000
#define VO_BUFF1_ADDR   (DRAW_BUFF_ADDR - VO_BUFF1_LEN)

#define VO_BUFF2_LEN    0x100000
#define VO_BUFF2_ADDR   (VO_BUFF1_ADDR - VO_BUFF2_LEN)

#define PIC_BUFF_LEN    0x200000  /*support the jpeg picture(bigest 1024*768) decode*/
#define PIC_BUFF_ADDR   (VO_BUFF2_ADDR - PIC_BUFF_LEN)

#define JPEG_BUFF_LEN    0x10000
#define JPEG_BUFF_ADDR   (PIC_BUFF_ADDR - JPEG_BUFF_LEN)


#define DEMUX_BUFF1_LEN    0x8000     /*32K*/
#define DEMUX_BUFF2_LEN    0x400000   /*4M*/
#define DEMUX_BUFF3_LEN    0x800000   /*8M*/

#define DEMUX_BUFF1_NUM    2
#define DEMUX_BUFF2_NUM    1
#define DEMUX_BUFF3_NUM    1

#define DEMUX_CHNUM_ALL    (DEMUX_BUFF1_NUM + DEMUX_BUFF2_NUM + DEMUX_BUFF3_NUM)

#define DEMUX_BUFF_LEN    (DEMUX_BUFF1_LEN * DEMUX_BUFF1_NUM  \
                         + DEMUX_BUFF2_LEN * DEMUX_BUFF2_NUM  \
                         + DEMUX_BUFF3_LEN * DEMUX_BUFF3_NUM )
#define DEMUX_BUFF_ADDR   (JPEG_BUFF_ADDR - DEMUX_BUFF_LEN)
/*for test use*/
#define TUNER3130_PD512 	(DEMUX_BUFF_ADDR - 512)
#define TUNER3130_PGD512 	(TUNER3130_PD512 - 512)
#define TUNER3130_PD1K		(TUNER3130_PGD512 - 1024)
#define TUNER3130_PGD1K		(TUNER3130_PD1K - 1024)
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_CONFIG_H__ */


