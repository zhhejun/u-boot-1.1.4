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

#if CONFIG_POST & CFG_POST_NAND

#include <asm/arch/nand_flash/nand_config.h>

extern FlashInfo *flashInfo;
extern int erase_block_flash(int);
extern int write_data(long lStart, long lCount, uchar * pnData);

#define VERIFY_ADDR 0xf2000000

int write_mem_nand()
{
    unsigned long j;
    char* dst = (char*)VERIFY_ADDR;
    for (j = 0; j < flashInfo->wBytesPerPage; j++)
    {
       *(dst +j)  = (char)j;
    }
    
    return 0;
}
int nand_post_test(int flags)
{
    unsigned char *pbuf;
    unsigned char readbuffer[PAGE_SIZE]={0};
    unsigned long block_num, page_num, offset_addr;
	int i;
	int value = 0;
	int result = 0;
	printf("\n");
	pbuf = (unsigned char *)VERIFY_ADDR;
    
    NF_GetInfo();
    write_mem_nand();
    
	for (block_num = NAND_START_POST_BLOCK; block_num < NAND_END_POST_BLOCK; block_num++) 
	{
        result = NF_IsBadBlock(block_num);
        if(result ==1)
        {
            printf("block %d is bad, jump to next block!\n", block_num);
            continue;
        }
        printf("--------Erase   block:%2d..\n", block_num);
        result = NF_EraseBlock(block_num);
        if(result != 0)
		{
            printf("erase nand flash block failed, exit!\n");
            return -1;
		}
		printf("OK\r");
		printf("--------Program and verify block:%2d...\n", block_num);
        for(page_num =0; page_num < flashInfo->wPagesPerBlock; page_num++)
        {
    		result = NF_WritePage( block_num, page_num, pbuf);
    		if(result != 0)
    		{
                printf("write nand flash page fail!\n");
                return -1;
    		}
    		result = NF_ReadPage( block_num, page_num, readbuffer);
    		if(result != 0)
    		{
                printf("read nand flash page failed, exit!\n");
                return -1;
    		}
    		for (i = 0; i < (int)flashInfo->wBytesPerPage; i++) 
    		{
    			if (readbuffer[i] != pbuf[i]) 
    			{
    				value = 1;
    				result = 1;
     				goto exit;
    			}
    		}
        }

        exit:
        offset_addr = block_num*flashInfo->wBytesPerBlock;
        printf("test %dth block, begin with 0x%x\n",block_num, offset_addr);
		if (value)
	    {
			printf("test failed, there is an error at %dth page!\n", page_num);
	    }
		else
		{
		    printf("test pass!\n");
		}
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
