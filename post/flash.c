/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

#ifdef CONFIG_POST

#include <post.h>
#include <watchdog.h>

#if CONFIG_POST & CFG_POST_FLASH

#define VERIFY_ADDR 0xf2000000
int write_mem()
{
    unsigned long j;
    char* dst = (char*)VERIFY_ADDR;
    for (j = 0; j < 128*1024; j++)
    {
       *(dst +j)  = (char)j;
    }
    
    return 0;
}
extern int erase_block_flash(int);
extern int write_data(long lStart, long lCount, uchar * pnData);
int flash_post_test(int flags)
{
    flash_info_t *  pFlashInfo;
    int             blockSize;
    unsigned short *pbuf, *temp;
	int offset, n, i;
	int value = 0;
	int result = 0;
	printf("\n");
	pbuf = (unsigned short *)VERIFY_ADDR;
	temp = pbuf;

    write_mem();


    pFlashInfo = addr2info(CFG_FLASH_BASE);

    flash_print_info(pFlashInfo);

    blockSize = pFlashInfo->size / pFlashInfo->sector_count;
        
	for (n = FLASH_START_POST_BLOCK; n < FLASH_END_POST_BLOCK; n++) {
		offset = n * blockSize;
		printf("--------Erase   block:%2d..\n", n);

        flash_erase(pFlashInfo, n, n);
        
		printf("OK\r");
		printf("--------Program block:%2d...\n", n);

		write_buff(pFlashInfo, pbuf, CFG_FLASH_BASE + offset, blockSize);

		printf("OK\r");
		printf("--------Verify  block:%2d...\n", n);
		for (i = 0; i < blockSize; i += 2) {
			if (*(unsigned short *)(CFG_FLASH_BASE + offset + i) !=
			    *temp++) {
				value = 1;
				result = 1;
			}
		}

        printf("test %dth block, begin with 0x%x\n",n, (CFG_FLASH_BASE+offset));
		if (value)
	    {
			printf("test failed!\n");
	    }
		else
		{
		    printf("test pass!\n");
		}
		temp = pbuf;
		value = 0;
	}
	printf("\n");
	if (result)
		return -1;
	else
		return 0;
}

#endif /* CONFIG_POST */

#endif
