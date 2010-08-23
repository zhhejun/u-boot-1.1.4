#include <config.h>
#include <common.h>
#include <command.h>

#include "e2prom.h"
#include "hi_i2c.h"

#define E2PROM_DEV_ADDRESS  0xa0    /*e2prom device address definition*/
#define E2PROM_PAGE_SIZE 16 
#define E2PROM_SIZE 2048 /*24lc16 is a 16kbit device*/
int e2prom_24lc16_read( unsigned int sub_addr, int sub_addr_count, unsigned char* pdata_out, int count)
{
	int ret = 0;
	unsigned int devaddr = E2PROM_DEV_ADDRESS;
	if (0 == pdata_out || count <= 0){
		printf("%s->%d,data null!\n",__func__,__LINE__);
		//return -EINVAL;
		return -1;
	}
	if (sub_addr + count > E2PROM_SIZE){
		printf("%s->%d,offset + data count out of range\n",__func__,__LINE__);
		return -1;
	}
	/*if sub_addr not start  from n*E2PROM_PAGE_SIZE,cut off head to set page aliagned!*/
	if (sub_addr % E2PROM_PAGE_SIZE != 0)
	{		
		int headlen = E2PROM_PAGE_SIZE - sub_addr % E2PROM_PAGE_SIZE;
		if (sub_addr > 0xff && sub_addr_count == 1){
			devaddr = E2PROM_DEV_ADDRESS;
			devaddr |= (((sub_addr >> 8) << 1) & 0xe);
		}	
		headlen = headlen > count ? count : headlen;	
		ret = hi_i2c_muti_read(devaddr, sub_addr, sub_addr_count, pdata_out, headlen);
		sub_addr += headlen;
		pdata_out += headlen;
		count -= headlen;		
	}
	/*deal with whole page*/
	while(count >= E2PROM_PAGE_SIZE && !ret){
		if (sub_addr > 0xff && sub_addr_count == 1){
			devaddr = E2PROM_DEV_ADDRESS;
			devaddr |= (((sub_addr >> 8) << 1) & 0xe);
		}	
		ret = hi_i2c_muti_read(devaddr, sub_addr, sub_addr_count, pdata_out, E2PROM_PAGE_SIZE);
		count -= E2PROM_PAGE_SIZE;
		sub_addr += E2PROM_PAGE_SIZE;
		pdata_out += E2PROM_PAGE_SIZE;
	}
	/*left*/
	if (!ret && count > 0){
		if (sub_addr > 0xff && sub_addr_count == 1){
			devaddr = E2PROM_DEV_ADDRESS;
			devaddr |= (((sub_addr >> 8) << 1) & 0xe);
		}	
		ret = hi_i2c_muti_read(devaddr, sub_addr, sub_addr_count, pdata_out, count);
	}
	return ret;
}
int  e2prom_24lc16_write(unsigned int sub_addr, int sub_addr_count, unsigned char* data, int count)
{
	int ret = 0;
	unsigned int devaddr = E2PROM_DEV_ADDRESS;
	if (0 == data || count <= 0){
		printf("%s->%d,data null!\n",__func__,__LINE__);
		return -1;
	}
	if (sub_addr + count > E2PROM_SIZE){
		printf("%s->%d,offset + data count out of range\n",__func__,__LINE__);
		return -1;
	}
	/*if sub_addr not start  from n*E2PROM_PAGE_SIZE,cut off head to set page aliagned!*/
	if (sub_addr % E2PROM_PAGE_SIZE != 0)
	{		
		int headlen = E2PROM_PAGE_SIZE - sub_addr % E2PROM_PAGE_SIZE;
		if (sub_addr > 0xff && sub_addr_count == 1){
			devaddr = E2PROM_DEV_ADDRESS;
			devaddr |= (((sub_addr >> 8) << 1) & 0xe);
		}	
		headlen = headlen > count ? count : headlen;	
		ret = hi_i2c_muti_write(devaddr, sub_addr, sub_addr_count, data, headlen);
		sub_addr += headlen;
		data += headlen;
		count -= headlen;		

	}
	/*deal with whole page*/
	while(count >= E2PROM_PAGE_SIZE && !ret){
		if (sub_addr > 0xff && sub_addr_count == 1){
			devaddr = E2PROM_DEV_ADDRESS;
			devaddr |= (((sub_addr >> 8) << 1) & 0xe);
		}	
		ret = hi_i2c_muti_write(devaddr, sub_addr, sub_addr_count, data, E2PROM_PAGE_SIZE);
		count -= E2PROM_PAGE_SIZE;
		sub_addr += E2PROM_PAGE_SIZE;
		data += E2PROM_PAGE_SIZE;
	}
	/*left*/
	if (!ret && count > 0){
		if (sub_addr > 0xff && sub_addr_count == 1){
			devaddr = E2PROM_DEV_ADDRESS;
			devaddr |= (((sub_addr >> 8) << 1) & 0xe);
		}	
		ret = hi_i2c_muti_write(devaddr, sub_addr, sub_addr_count, data, count);
	}
	return ret;
}
