#include <common.h>
#include <command.h>

#ifdef CFG_MMU_HANDLE 

int do_cachestart_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	
	ulong input_value,output_value;
	size_t size;
	if(argc < 2){
		printf("command error\n");
		printf("Usage : cachestart_test input value\n");
		return 1;
	}
	
	
	input_value = simple_strtoul(argv[1], NULL, 16);
	
	printf("The address F2000000 original value is 0x%x\n", *(ulong *)(0xf2000000));
	
	printf("the input value is 0x%x \n", input_value);
	
	*(ulong *)(0xf2000000) = input_value;

	dcache_stop_noflush();
	
	output_value = *(ulong *)(0xf2000000);
	
	printf("The address F2000000 read back value is 0x%x\n", output_value);
		
	return 1;
}

U_BOOT_CMD(
	cachestart_test,	2,	1,	do_cachestart_test,
	"test if the cache started\n",
);

int do_cacheflush_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int ret = 0;
	ulong input_value,output_value;
	size_t size;
	if(argc < 2){
		printf("command error\n");
		printf("Usage : cacheflush_test input value\n");
		return 1;
	}
	input_value = simple_strtoul(argv[1], NULL, 16);
	
	printf("The address F2000000 original value is 0x%x\n", *(ulong *)(0xf2000000));
	
	printf("the input value is 0x%x \n", input_value);
	
	*(ulong *)(0xf2000000) = input_value;

	dcache_stop();
	
	output_value = *(ulong *)(0xf2000000);
	
	printf("The address F2000000 read back value is 0x%x\n", output_value);

	return 1;
}

U_BOOT_CMD(
	cacheflush_test,	2,	1,	do_cacheflush_test,
	"test if the cache flushed to mem\n",
);

#endif
