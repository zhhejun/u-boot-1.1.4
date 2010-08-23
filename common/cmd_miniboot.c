
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/io.h>

#ifdef CMD_MINI_BOOT

#define MINIBOOT_BOOTARGS_MAGIC 0x4f41494d

struct miniboot_info
{
	unsigned long rom_base;
	unsigned long text_base;
	unsigned long bss_start;
	unsigned long bss_end;

	unsigned long mb_args;
};

static int find_miniboot(unsigned long start, struct miniboot_info *minf)
{
	unsigned long deadbeef_align=0x10;
	int i;

	memset(minf, 0, sizeof(*minf));

	if( !( (__raw_readl(start)&0xffffff00) == 0xea000000 && 
			__raw_readl(start + sizeof(unsigned long)) ==0xeafffffe))
		return 1;

	for(i=deadbeef_align; i<(deadbeef_align*4); i+=deadbeef_align) {
		if(__raw_readl(start + i) == (~__raw_readl(start + i + sizeof(unsigned long))))
			goto _found;
	}
	return 2;

_found:
	minf->rom_base = start;
	start += i;
	minf->text_base = __raw_readl(start);
	minf->bss_start = __raw_readl(start + 3*sizeof(unsigned long));
	minf->bss_end = __raw_readl(start + 4*sizeof(unsigned long));

	return 0;
}

#define MAX_MINIBOOT_SIZE 8192

static int miniboot_set_bootargs(struct miniboot_info *minf, int argc, char *argv[])
{
	char *buf;
	char *bootargs;
	int len = minf->bss_start - minf->text_base;
	char cmd[256];
	unsigned long blk_size;
	
	if(len <1024 || len > MAX_MINIBOOT_SIZE)
		goto error_exit;

	buf = malloc(MAX_MINIBOOT_SIZE);
	if(buf ==NULL) {
		printf("can not alloc buffer for mini-boot!\n");
		goto error_exit;
	}
	memset(buf, 0, MAX_MINIBOOT_SIZE);
	memcpy(buf, (void*)minf->rom_base, len);
	bootargs = getenv("bootargs");
	if(bootargs) {
		unsigned long magic = MINIBOOT_BOOTARGS_MAGIC;
		if( (len + 0x10 + strlen(bootargs)) >= MAX_MINIBOOT_SIZE)
			goto error_exit;

		memcpy(buf + len, &magic, 4); 
		len += 4;

		if(argc>2 && strcmp(argv[3], "-")==0) {
			memcpy(buf + len, "- ", 2);
		} else {
			memcpy(buf + len, "+ ", 2);
		}
		len += 2;

		strcpy(buf + len, bootargs);
		len += strlen(bootargs);

		printf("%s\n", bootargs);
	} 

	blk_size = ((len + CFG_FLASH_BOOT_BLOCK_SIZE -1)/CFG_FLASH_BOOT_BLOCK_SIZE) *CFG_FLASH_BOOT_BLOCK_SIZE;

	printf("WARNNING: Setting bootargs for mini-boot ...\n");

	sprintf(cmd, "protect off 0x%08X +0x%X ", minf->rom_base, blk_size);
	printf("%s\n", cmd);
	run_command(cmd, 0);

	sprintf(cmd, "erase 0x%08X +0x%X ", minf->rom_base, blk_size);
	printf("%s\n", cmd);
	run_command(cmd, 0);

	sprintf(cmd, "cp 0x%p 0x%08X 0x%X ", buf, minf->rom_base, len);
	printf("%s\n", cmd);
	run_command(cmd, 0);

	return 0;

error_exit:
	printf("operations on miniboot failed!\n");

	return 0;
}

int do_miniboot ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int rc = 0;
	struct miniboot_info mi;

	if( (rc=find_miniboot(ROM_TEXT_ADRS, &mi))) {
		printf("none of mini-boot detected, error code %d!\n", rc);
		return 0;
	}
	if(argc < 2)
		return 0;

	if(strcmp(argv[1], "set") ==0) {
		if(strcmp(argv[2], "bootargs") ==0)
			rc = miniboot_set_bootargs(&mi, argc, argv);
		printf("GOOD LUCK: Reconfiguring mini-boot finished\n");
	} else if(strcmp(argv[1], "show") ==0) {
		if(strcmp(argv[2], "bootargs") ==0) {
			unsigned long magic;
			int args_addr = mi.rom_base + (mi.bss_start - mi.text_base);
			memcpy(&magic, (void *)args_addr, 4);
			if(magic != MINIBOOT_BOOTARGS_MAGIC) {
				printf("mini-boot have'nt got a bootargs yet!\n");
				return 0;
			}
			printf("%s\n", (const char*)args_addr + 4 + 2);
		}
	} else {
	}

	return rc;
}

U_BOOT_CMD(
	miniboot,     4,     1,      do_miniboot,
	"miniboot [set/show] [bootargs]\n",
	"miniboot [set/show] [bootargs]\n"
);
#endif

