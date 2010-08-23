/*  extdrv/interface/i2c/hi_i2c.c
 *
 * Copyright (c) 2006 Hisilicon Co., Ltd.
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
 * along with this program;
 *
 * History:
 *      19-April-2006 create this file
 *      25-July-2006  when the read/write operation was wrong redo it,
 *                    but not more than 3 times
 *      19-August-2006 remove the print info of timeout,
 *                     change the read/write retry times to 5,
 *                     and if even wrong we would stop it and then print info
 */

#include <common.h>
#include <command.h>

#include "hi_i2c.h"


static unsigned int i2cinitialized =0;

#define HCLK_FREQ (100000000)
#define I2C_CLK (HCLK_FREQ)
#define I2C_RATE	(50000)


#define I2C_BASE 0x101f6000
#define I2C_REG_READL(reg,result) (result = *(volatile unsigned int*)reg)

#define I2C_REG_WRITEL(reg,data) ((*(volatile unsigned int*) reg) = data)

#define I2C_REG_READW(reg,result) (result = *(volatile unsigned short*) reg)

#define I2C_REG_WRITEW(reg,data) ((*(volatile unsigned short*) reg) = data)


#define IO_ADDRESS(reg) (reg)

#define I2C_CON		               IO_ADDRESS(I2C_BASE+0x00) /*I2C control reg*/
#define I2C_TAR		               IO_ADDRESS(I2C_BASE+0x04) /*slave chip address reg*/
#define I2C_HCNT                   IO_ADDRESS(I2C_BASE+0x14)
#define I2C_LCNT                   IO_ADDRESS(I2C_BASE+0x18)
#define I2C_HS_HCNT                IO_ADDRESS(I2C_BASE+0x1C)
#define I2C_HS_LCNT                IO_ADDRESS(I2C_BASE+0x20)
#define	I2C_INTR_MASK	           IO_ADDRESS(I2C_BASE+0x30) /*interrupt mask reg*/
#define	I2C_INTR_STAT	           IO_ADDRESS(I2C_BASE+0x2C) /*interrupt status reg*/
#define	I2C_ENABLE	               IO_ADDRESS(I2C_BASE+0x6C) /*IP enable*/
#define	I2C_DATA_CMD	           IO_ADDRESS(I2C_BASE+0x10) /*I2C TX FIFO*/
#define I2C_TXFIFO_FLAG	           IO_ADDRESS(I2C_BASE+0x74) /*indicate TXFIFO level*/
#define I2C_RXFIFO_FLAG	           IO_ADDRESS(I2C_BASE+0x78) /*indicate RXFIFO level*/
#define I2C_STATUS	               IO_ADDRESS(I2C_BASE+0x70) /*IP status flag reg*/
#define I2C_COMP_PARAM_1           IO_ADDRESS(I2C_BASE+0xF4) /*IP status flag reg*/
#define I2C_TX_TL                  IO_ADDRESS(I2C_BASE+0x3c)
#define I2C_RX_TL                  IO_ADDRESS(I2C_BASE+0x38)
#define I2C_RAW_INTSTATUS          IO_ADDRESS(I2C_BASE+0x34)

/*clear write interrupt reg */
#define I2C_CLR_INTR	           IO_ADDRESS(I2C_BASE+0x40) /*clear combined and individual interrupt*/
#define I2C_CLR_TX_ABRT	           IO_ADDRESS(I2C_BASE+0x54) /*clear RT abrt interrupt reg*/
#define I2C_CLR_STOPDET            IO_ADDRESS(I2C_BASE+0x60) /*clear stop_det interrupt reg*/
#define I2C_CLR_TX_OVER            IO_ADDRESS(I2C_BASE+0x4C) /*clear tx fifo over interrupt reg*/
#define I2C_CLR_RX_ABRT	           IO_ADDRESS(I2C_BASE+0x80) /*clear RX abrt interrupt reg*/


#define TIMECOUNT       0x100
#define RW_RETRY_TIME   5


/*
 * sends a character over I2C bus routine.
 *
 * @param sendvalue: character to send
 * @return value:0--success; -1--error.
 *
 */
static int i2csendbyte(unsigned char sendvalue)
{
    unsigned short data = 0x00FF;
    unsigned short status;
    unsigned short count;
    unsigned long i = 0;

    /*set write CMD*/
    data = sendvalue & data;

    /*check TX fifo status*/
    I2C_REG_READW(I2C_TXFIFO_FLAG, status);

    /*check TX fifo status*/
    I2C_REG_READW(I2C_TX_TL, count);
    count&=0xff;

    /*TX FIFO full and wait for a while*/
    while(status >= count)
    {
        udelay(20);
        if(++i>TIMECOUNT)
        {
            /*printk("TIME OUT: i2csendbyte\n");*/
            return -1;
        }
        I2C_REG_READW(I2C_TXFIFO_FLAG, status);
    }

     /*write one byte to TX fifo*/
    I2C_REG_WRITEW(I2C_DATA_CMD,data);

    return 0;

}

/*
 * receives a character from I2C bus routine.
 *
 * @return value:character received
 *
 */
static unsigned char i2creceivebyte(void)
{
    unsigned char ucvalue;
    unsigned short status;
    unsigned long i = 0;

    /*read Rx FIFO*/
    I2C_REG_WRITEW(I2C_DATA_CMD,0x0100);

    I2C_REG_READW(I2C_RXFIFO_FLAG, status);

    /*RX FIFO is  empty and wait for a while*/
    while(status == 0)
    {
        udelay(20);
        if(++i>TIMECOUNT)
        {
            /*printk("TIME OUT: i2creceivebyte\n");*/
            return -1;
        }
        I2C_REG_READW(I2C_RXFIFO_FLAG, status);

    }

    I2C_REG_READW(I2C_DATA_CMD,ucvalue);
    return(ucvalue);
}

/*
 * sends ack check routine.
 *
 * @return value:0--success; -1--error.
 *
 */
static int i2cchecksendresult(void)
{
    unsigned short status, status1;
    unsigned long i = 0;

    I2C_REG_READW(I2C_TXFIFO_FLAG, status);
    I2C_REG_READW(I2C_STATUS, status1);

    /*TX FIFO is not empty and wait for a while*/
    while((status != 0) || (status1 & 0x1))
    {
        udelay(20);
        if(++i>TIMECOUNT)
        {
            /*printk("TIME OUT: i2cchecksendresult,I2C_TXFIFO_FLAG:%d,
                    I2C_STATUS:%d \n",status, status1);*/
            return -1;
        }
        I2C_REG_READW(I2C_TXFIFO_FLAG, status);
        I2C_REG_READW(I2C_STATUS, status1);
    }
    return 0;
}

/*
 * writes address of device to I2C bus routine.
 *
 * @param address: address of device
 *
 */
static void i2csetaddress(unsigned char address)
{
    I2C_REG_WRITEW(I2C_ENABLE, 0);
    I2C_REG_WRITEW(I2C_TAR, address>>1);
    I2C_REG_WRITEW(I2C_ENABLE, 1);
}


/*
 *  read continuing data from the I2C bus of a device rountine.
 *
 *  @param  devaddress:  address of the device
 *  @param  regaddress: address of register within device
 * 	@param 	reg_addr_count : regaddress byte count
 *  @param  data:   data point from the device readed
 *  @param  count:  data count from the device readed 
 *   
 *  @return value: 0--success; -1--error.
 *
 */
int  hi_i2c_muti_read(unsigned char devaddress, unsigned int regaddress, int reg_addr_count,
	unsigned char* data, unsigned long  count)
{
    unsigned long j = 0;
    int ret = 0;
    if (data == 0)
    {	
    	return -1;
    }

    i2csetaddress((unsigned char)(devaddress));
    for (j = 0; 0 == ret &&  j < reg_addr_count;j++)
    {
    	ret = i2csendbyte((unsigned char)(regaddress  >> ((reg_addr_count  - j - 1) * 8)));
    }
    for(j = 0;0 == ret && j < count; j++)
	{
		*data = i2creceivebyte();
		data ++;
	}

    return ret; 


}


/*
 *  writes continuing data to a device on the I2C bus rountine.
 *
 *  @param  devaddress:  address of the device
 *  @param  regaddress: address of register within device
 *	@param 	reg_addr_count : regaddress byte count
 *  @param  data:   data point for writed to device
 *  @param  count:  data count for writed to device
 *   
 *  @return value:0--success; -1--error.
 *
 */
int  hi_i2c_muti_write(unsigned char devaddress,unsigned int regaddress,int reg_addr_count, 
	unsigned char* data,unsigned long  count)
{
    int ret = 0, i;
    unsigned long j = 0;
    if (data == 0)
    {	
	    return -1;
    }
    for (i = 0; i < RW_RETRY_TIME; i++)
    {
        i2csetaddress((unsigned char)(devaddress));
        for (j = 0; 0 == ret  &&  j < reg_addr_count;j++)
        {
	    ret = i2csendbyte((unsigned char)(regaddress  >> ((reg_addr_count -j -1)*8)));
      	}
    	for (j = 0; 0 == ret && j< count;j++)
    	{
    	    ret = i2csendbyte(*data++);
    	}
        if (0 == ret)
        {
        	ret = i2cchecksendresult();
    		udelay(4000);
    		return 0;

        }       
    }

    printf("I2C_WRITE_WRONG: timeout more than %d times and stop here. \n",RW_RETRY_TIME );

    return ret;
}

/*
 * initializes I2C interface routine.
 *
 * @return value:0--success; -EFAULT--error.
 *
 */
int hi_i2c_init(void)
{
    unsigned int cnt, hcnt, lcnt;
    unsigned int reg;
    
   if(i2cinitialized == 0)
    {
        printf("Hisilicon I2C Driver \n"); 
        I2C_REG_READL(IO_ADDRESS(0x101e0040),reg);
        reg |= 0x10000000;
        I2C_REG_WRITEL(IO_ADDRESS(0x101e0040),reg);
        I2C_REG_WRITEW(I2C_ENABLE, 0);
        /*
        standard speed.
        */
        cnt = I2C_CLK/I2C_RATE;
        hcnt = (cnt/9)*4;
        lcnt = (cnt/9)*5;
    
        I2C_REG_WRITEW(I2C_CON, 0x0063);
        I2C_REG_WRITEW(I2C_HCNT, hcnt);
        I2C_REG_WRITEW(I2C_LCNT, lcnt);
        I2C_REG_WRITEW(I2C_INTR_MASK, 0x0000);
        i2cinitialized = 1;
    }
    return 0;

}



