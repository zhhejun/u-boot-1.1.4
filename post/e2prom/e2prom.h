#ifndef __HI_E2PROM_H__
#define __HI_E2PROM_H__



int  e2prom_24lc16_read( unsigned int sub_addr, int sub_addr_count, unsigned char* pdata_out, int count);


int e2prom_24lc16_write(unsigned int sub_addr, int sub_addr_count, unsigned char* data, int count);

#endif


