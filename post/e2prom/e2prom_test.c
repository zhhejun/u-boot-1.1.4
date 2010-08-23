#include <common.h>

#ifdef CONFIG_POST

#include <post.h>

#if CONFIG_POST & CFG_POST_E2PROM
#include "e2prom.h"
#include "drv_config.h"
#include "hi_i2c.h"
#define cmp(src,dst,count,name) \
do{				\
	int i;			\
	for(i = 0; i < (count); i++){	\
		if ((src)[i] != (dst)[i]){	\
			printf("%s->%d, test %s fail! src[%d] = %d,dst[%d] = %d\n",__func__,__LINE__,name,i,(src)[i],i,(dst)[i]);	\
			return -1;		\
		}				\
	}					\
	printf("%s->%d, test %s done!\n",__func__,__LINE__,name);	\
}while(0)

int test_e2prom(int flags)
{
	unsigned char d16[16],d32[32],gd16[16],gd32[32],d7[7],gd7[7],d45[45],gd45[45];
	unsigned char* pd512 =(unsigned char*) TUNER3130_PD512,
	* pgd512 = (unsigned char*)TUNER3130_PGD512,
	* pd1k = (unsigned char*)TUNER3130_PD1K ,
	* pgd1k =  (unsigned char*)TUNER3130_PGD1K;
	
	int ret,i;
	ret = hi_i2c_init();
	if (ret){
		printf("%s->%d,init i2c fail!\n");
		return -1;
	}
	/*init data*/
	for (i = 0; i < 16; i++){
		d16[i] = (unsigned char)(i * 2 + 16);
	}
	
	for (i = 0; i < 32; i++){
		d32[i] = (unsigned char)(i * 3 + 17);
	}
	for (i = 0; i< 512; i++){
		pd512[i] = i%256;
	}	
	for (i = 0; i < 1024; i ++){
		pd1k[i] = i%256;
	}
	for (i = 0; i < 7; i++) d7[i] = (unsigned char)(i*11+7);
	for (i = 0; i < 45; i++) d45[i] = (unsigned char)(i* 17 +11);

	printf("%s->%d, test 7 byte read & write,firs write, then read\n",__func__,__LINE__);
	ret = e2prom_24lc16_write(0x0,1,d7,7);
	if (ret){
		printf("write 7 data fail!\n");
		return -1;
	}
	ret = e2prom_24lc16_read(0x0,1,gd7,7);
	if (ret){
		printf("read 7 data fail!\n");
		return -1;
	}
	cmp(d7,gd7,7,"7 byte");

	printf("%s->%d, test 45 byte read & write,firs write, then read\n",__func__,__LINE__);
	ret = e2prom_24lc16_write(0x0,1,d45,45);
	if (ret){
		printf("write 45 data fail!\n");
		return -1;
	}
	ret = e2prom_24lc16_read(0x0,1,gd45,45);
	if (ret){
		printf("read 45 data fail!\n");
		return -1;
	}
	cmp(d45,gd45,45,"45 byte");

	printf("%s->%d, test 16 byte read & write,firs write, then read\n",__func__,__LINE__);
	ret = e2prom_24lc16_write(0x10+0x7,1,d16,16);
	if (ret){
		printf("write 16 data fail!\n");
		return -1;
	}
	ret = e2prom_24lc16_read(0x10+0x7,1,gd16,16);
	if (ret){
		printf("read 16 data fail!\n");
		return -1;
	}
	cmp(d16,gd16,16,"16 byte");

	printf("%s->%d, test 32 byte read & write,firs write, then read\n",__func__,__LINE__);
	ret = e2prom_24lc16_write(0x10+0x7 +0x10,1,d32,32);
	if (ret){
		printf("write 32 data fail!\n");
		return -1;
	}
	ret = e2prom_24lc16_read(0x10+0x7 +0x10,1,gd32,32);
	if (ret){
		printf("read 32 data fail!\n");
		return -1;
	}
	cmp(d32,gd32,32,"32 byte");

	printf("%s->%d, test 512 byte read & write,firs write, then read\n",__func__,__LINE__);
	ret = e2prom_24lc16_write(0x10+0x7 +0x10 + 32,1,pd512,512);
	if (ret){
		printf("write 512 data fail!\n");
		return -1;
	}
	ret = e2prom_24lc16_read(0x10+0x7 +0x10 +32,1,pgd512,512);
	if (ret){
		printf("read 512 data fail!\n");
		return -1;
	}
	cmp(pd512,pgd512,512,"512 byte");

	printf("%s->%d, test 32 byte read & write,firs write, then read\n",__func__,__LINE__);
	ret = e2prom_24lc16_write(0x10+0x7 +0x10,1,d32,32);
	if (ret){
		printf("write 32 data fail!\n");
		return -1;
	}
	ret = e2prom_24lc16_read(0x10+0x7 +0x10,1,gd32,32);
	if (ret){
		printf("read 32 data fail!\n");
		return -1;
	}
	cmp(d32,gd32,32,"32 byte");
	printf("%s->%d, test 1k byte read & write,firs write, then read\n",__func__,__LINE__);
	ret = e2prom_24lc16_write(0x0,1,pd1k,1024);
	if (ret){
		printf("write 1k data fail!\n");
		return -1;
	}
	ret = e2prom_24lc16_read(0x0,1,pgd1k,1024);
	if (ret){
		printf("read 1k data fail!\n");
		return -1;
	}
	cmp(pd1k,pgd1k,1024,"1k byte");
	printf("test e2prom done!\n");
	return 0;	
}

#endif
#endif
