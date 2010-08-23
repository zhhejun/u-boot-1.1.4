/*
 *	Copyright 1994, 1995, 2000 Neil Russell.
 *	(See License)
 *	Copyright 2000, 2001 DENX Software Engineering, Wolfgang Denk, wd@denx.de
 */

#include <common.h>
#include <command.h>
#include <net.h>
#include "tftp.h"
#include "bootp.h"

#undef	ET_DEBUG

#if (CONFIG_COMMANDS & CFG_CMD_NET)

#define WELL_KNOWN_PORT	69		/* Well known TFTP port #		*/
#define TIMEOUT		(3*CFG_HZ)		/* Seconds to timeout for a lost pkt	*/
#define SERVER_TIME_OUT 20
#ifndef	CONFIG_NET_RETRY_COUNT
# define TIMEOUT_COUNT	(unsigned int)((SERVER_TIME_OUT*CFG_HZ*3/2)/TIMEOUT)		/* # of timeouts before giving up  */
#else
# define TIMEOUT_COUNT  (CONFIG_NET_RETRY_COUNT * 2)
#endif
					/* (for checking the image size)	*/
#define TBLKS_PER_HASHES	64
#define HASHES_PER_LINE		32		/* Number of "loading" hashes per line	*/

/*
 *	TFTP operations.
 */
#define TFTP_RRQ	1
#define TFTP_WRQ	2
#define TFTP_DATA	3
#define TFTP_ACK	4
#define TFTP_ERROR	5
#define TFTP_OACK	6

#define STATE_OK	0
#define STATE_RRQ	1
#define STATE_DATA	2
#define STATE_ERROR	3
//#define STATE_TOO_LARGE	3
//#define STATE_BAD_MAGIC	4
#define STATE_OACK	5
#define STATE_WRQ	6
#define STATE_ACK	7

#define TFTP_BLOCK_SIZE		512		    /* default TFTP block size	*/
#define TFTP_SEQUENCE_SIZE	((ulong)(1<<16))    /* sequence number is 16 bit */


static int	TftpServerPort;		/* The UDP port at their end		*/
static int	TftpOurPort;		/* The UDP port at our end		*/
static int	TftpTimeoutCount;
static ulong	TftpBlock;		/* packet sequence number		*/
static ulong	TftpLastBlock;		/* last packet sequence number received */
static ulong	TftpBlockWrap;		/* count of sequence number wraparounds */
static ulong	TftpBlockWrapOffset;	/* memory offset due to wrapping	*/

static int	TftpState=0;

ulong upload_addr = CFG_LOAD_ADDR;		/* Default upLoad Address */
ulong upload_size = 0;

#define DEFAULT_NAME_LEN	(8 + 4 + 1)
static char default_filename[DEFAULT_NAME_LEN];
static char *tftp_filename;

static int store_block (unsigned block, uchar * src, unsigned len)
{
	DECLARE_GLOBAL_DATA_PTR;

	ulong offset = block * TFTP_BLOCK_SIZE + TftpBlockWrapOffset;
	ulong newsize = offset + len;

#ifndef CFG_NO_FLASH
	/* check if we are copying to Flash */
	if ( (addr2info(load_addr + offset) != NULL) ) {
		int rc;
		gd->flags |= GD_FLG_QUIET;
		rc = flash_write (src, load_addr + offset, len);
		gd->flags &= ~GD_FLG_QUIET;

		if (rc != 0) {
			putc('\n');
			flash_perror (rc);
			return (1);
		}
	} else 
#endif
	{
		memcpy((void *)(load_addr + offset), src, len);
	}

	if (NetBootFileXferSize < newsize)
		NetBootFileXferSize = newsize;

	return 0;
}

static void TftpSend (void);
static void TftpTimeout (void);

/**********************************************************************/

static void
TftpSend (void)
{
	volatile uchar *	pkt;
	volatile uchar *	xp;
	int			len = 0;
	int			uplen=0;
	volatile ushort *s;

	/*
	 *	We will always be sending some sort of packet, so
	 *	cobble together the packet headers now.
	 */
	pkt = NetTxPacket + NetEthHdrSize() + IP_HDR_SIZE;

	switch (TftpState) {

	case STATE_RRQ:
	case STATE_WRQ:
		xp = pkt;

		s = (ushort *)pkt;
		if(TftpState == STATE_WRQ)
			*s++ = htons(TFTP_WRQ);
		else *s++ = htons(TFTP_RRQ);
		pkt = (uchar *)s;

		strcpy ((char *)pkt, tftp_filename);
		pkt += strlen(tftp_filename) + 1;
		strcpy ((char *)pkt, "octet");
		pkt += 5 /*strlen("octet")*/ + 1;
		strcpy ((char *)pkt, "timeout");
		pkt += 7 /*strlen("timeout")*/ + 1;
		sprintf((char *)pkt, "%d", SERVER_TIME_OUT);
#ifdef ET_DEBUG
		printf("send option \"timeout %s\"\n", (char *)pkt);
#endif
		pkt += strlen((char *)pkt) + 1;
		len = pkt - xp;
		printf("%");
		break;

	case STATE_DATA:
	case STATE_OACK:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_ACK);
		*s++ = htons(TftpBlock);
		pkt = (uchar *)s;
		len = pkt - xp;
		break;

	case STATE_ERROR:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_ERROR);
		*s++ = htons(0);
		pkt = (uchar *)s;
		strcpy ((char *)pkt, "Hisilicon tftp-client error");
		pkt += strlen((char *)pkt) + 1;
		len = pkt - xp;
		break;

	case STATE_ACK:
		xp = pkt;
		s = (ushort *)pkt;
		*s++ = htons(TFTP_DATA);
		*s++ = htons(TftpBlock+1);
		pkt = (uchar *)s;

		uplen = (upload_size-TftpBlock*TFTP_BLOCK_SIZE);
		uplen = uplen > TFTP_BLOCK_SIZE ? TFTP_BLOCK_SIZE : uplen;
		memcpy((void*)pkt, (const char*)upload_addr + TftpBlock*TFTP_BLOCK_SIZE , uplen);
		pkt += uplen;
		len = pkt - xp;
		break;

	default:
		return;
		break;
	}

	NetSendUDPPacket(NetServerEther, NetServerIP, TftpServerPort, TftpOurPort, len);
}

static void tftp_show_transferd(int block, unsigned long wrap)
{
#define SHOW_TRANSFERD(tail) printf ("\t[%2lu.%03lu MB]%s", ((block-1)* TFTP_BLOCK_SIZE + wrap)>>20, \
					(((block-1) * TFTP_BLOCK_SIZE + wrap)&0x0FFFFF)>>10, tail)

	if( ((block-1) & (TBLKS_PER_HASHES-1)) == 0)
		putc('#');

	if( ((block-1) & (TBLKS_PER_HASHES*HASHES_PER_LINE-1)) == 0) {
		if((block-1) ==0) {
			if(wrap==0) {
				puts("\t[ Connected ]\n");
			} else {
				SHOW_TRANSFERD(" [BlockCounter Reset]\n");
			}
		} else {
			SHOW_TRANSFERD("\n");
		}
	}
#undef SHOW_TRANSFERD
}

static void
TftpHandler (uchar * pkt, unsigned dest, unsigned src, unsigned len)
{
	ushort proto;
	ushort *s;



	if (dest != TftpOurPort) {
		return;
	}
	if ( !(TftpState==STATE_RRQ || TftpState==STATE_WRQ) && src != TftpServerPort) {
		return;
	}

	if (len < 2) {
		return;
	}
	len -= 2;
	/* warning: don't use increment (++) in ntohs() macros!! */
	s = (ushort *)pkt;
	proto = *s++;
	pkt = (uchar *)s;
	switch (ntohs(proto)) {

	case TFTP_RRQ:
	case TFTP_WRQ:
		break;
	case TFTP_ACK:
		TftpServerPort = src;
		if(ntohs(*(ushort *)pkt)==0 && TftpBlock==0xFFFF && TftpState!=STATE_WRQ)
		{
			TftpBlockWrap++;
			TftpBlockWrapOffset += TFTP_BLOCK_SIZE * TFTP_SEQUENCE_SIZE;

			upload_addr += TFTP_BLOCK_SIZE * TFTP_SEQUENCE_SIZE;
			upload_size -= TFTP_BLOCK_SIZE * TFTP_SEQUENCE_SIZE;
		}

		TftpState=STATE_ACK;

		TftpBlock = ntohs(*(ushort *)pkt);
		if(TftpLastBlock == TftpBlock) {
			putc('%');
		} else {
			tftp_show_transferd(TftpBlock, TftpBlockWrapOffset);
			TftpTimeoutCount=0;
		}
		TftpLastBlock = TftpBlock;
		NetSetTimeout (TIMEOUT, TftpTimeout);

		if(TFTP_BLOCK_SIZE*TftpBlock> upload_size )
		{
			NetState = NETLOOP_SUCCESS;
			TftpState = STATE_OK;
			printf ("\n\t %lu.%03lu MB upload ok.\n", (TftpBlockWrapOffset+upload_size)>>20, 
					((TftpBlockWrapOffset+upload_size)&0x0FFFFF)>>10);
		break;
		}
		TftpSend (); /* Send ACK */
		break;

	case TFTP_OACK:
#ifdef ET_DEBUG
		printf("Got OACK: %s %s\n", pkt, pkt+strlen(pkt)+1);
#endif
		if(TftpState == STATE_WRQ)
		{
			TftpState = STATE_ACK;
			TftpBlock = 0;
		}
		else
		{
			TftpState = STATE_OACK;
		}
		TftpServerPort = src;
		TftpSend (); /* Send ACK */
		break;

	case TFTP_DATA:
		if (len < 2)
			return;

		len -= 2;
		TftpBlock = ntohs(*(ushort *)pkt);

		/*
		 * RFC1350 specifies that the first data packet will
		 * have sequence number 1. If we receive a sequence
		 * number of 0 this means that there was a wrap
		 * around of the (16 bit) counter.
		 */
		if (TftpBlock == 0) {
			TftpBlockWrap++;
			TftpBlockWrapOffset += TFTP_BLOCK_SIZE * TFTP_SEQUENCE_SIZE;
		}

#ifdef ET_DEBUG
		if (TftpState == STATE_RRQ) {
			puts ("Server did not acknowledge timeout option!\n");
		}
#endif

		if (TftpState == STATE_RRQ || TftpState == STATE_OACK) {
			/* first block received */
			TftpState = STATE_DATA;
			TftpServerPort = src;

			if (TftpBlock != 1) {	/* Assertion */
				printf ("\nTFTP error: "
					"First block is not block 1 (%ld)\n"
					"Starting again\n\n",
					TftpBlock);
				NetStartAgain ();
				break;
			}
		}

		if (TftpBlock == TftpLastBlock) {
			/*
			 *	Same block again; ignore it.
			 */
			putc ('%');
			break;
		}
		else
		{
			tftp_show_transferd(TftpBlock, TftpBlockWrapOffset);
			TftpTimeoutCount=0;
		}

		TftpLastBlock = TftpBlock;
		NetSetTimeout (TIMEOUT, TftpTimeout);

		/*
		 *	Acknoledge the block just received, which will prompt
		 *	the server for the next one.
		 */
		TftpSend ();

		if(store_block (TftpBlock - 1, pkt + 2, len)) {
            TftpState = STATE_ERROR;
			TftpSend ();
			NetState = NETLOOP_FAIL;
			return ;
		}
		if (len < TFTP_BLOCK_SIZE) {
			/*
			 *	We received the whole thing.  Try to
			 *	run it.
			 */
			printf ("\n\t %lu.%03lu MB download ok.\n", ((TftpBlock-1)* TFTP_BLOCK_SIZE + TftpBlockWrapOffset)>>20,
					(((TftpBlock-1) * TFTP_BLOCK_SIZE + TftpBlockWrapOffset)&0x0FFFFF)>>10);
			NetState = NETLOOP_SUCCESS;
		}
		break;

	case TFTP_ERROR:
		printf ("\nTFTP error: '%s' (%d)\n",
					pkt + 2, ntohs(*(ushort *)pkt));
		puts ("Starting again\n\n");
		NetStartAgain ();
		break;

	default:
		TftpState=-1;
		break;
	}
}


static void
TftpTimeout (void)
{
	if( NetState == NETLOOP_FAIL && upload_size==0) {
		TftpState = STATE_ERROR;
		TftpSend ();
	} else if (++TftpTimeoutCount > TIMEOUT_COUNT) {
		TftpState = STATE_ERROR;
		TftpSend ();
		puts ("\nRetry count exceeded; starting again\n");
		NetStartAgain ();
	} else {
		puts ("T");
		NetSetTimeout (TIMEOUT, TftpTimeout);
		TftpSend ();
	}
}


void
TftpStart (void)
{
#ifdef CONFIG_TFTP_PORT
	char *ep;             /* Environment pointer */
#endif

	if(upload_size)
		TftpState = STATE_WRQ;
	else TftpState = STATE_RRQ;

	if (BootFile[0] == '\0') {
		sprintf(default_filename, "%02lX%02lX%02lX%02lX.img",
			NetOurIP & 0xFF,
			(NetOurIP >>  8) & 0xFF,
			(NetOurIP >> 16) & 0xFF,
			(NetOurIP >> 24) & 0xFF	);
		tftp_filename = default_filename;

		printf ("*** Warning: no boot file name; using '%s'\n",
			tftp_filename);
	} else {
		tftp_filename = BootFile;
	}

#if defined(CONFIG_NET_MULTI)
	printf ("Using %s device\n", eth_get_name());
#endif
	if( TftpState == STATE_WRQ)
	{
		puts ("TFTP to server ");	print_IPaddr (NetServerIP);
	}
	else
	{
	puts ("TFTP from server ");	print_IPaddr (NetServerIP);
	}
	puts ("; our IP address is ");	print_IPaddr (NetOurIP);

	/* Check if we need to send across this subnet */
	if (NetOurGatewayIP && NetOurSubnetMask) {
	    IPaddr_t OurNet 	= NetOurIP    & NetOurSubnetMask;
	    IPaddr_t ServerNet 	= NetServerIP & NetOurSubnetMask;

	    if (OurNet != ServerNet) {
		puts ("; sending through gateway ");
		print_IPaddr (NetOurGatewayIP) ;
	    }
	}
	putc ('\n');

	if( TftpState == STATE_WRQ)
		printf ("Upload Filename '%s'.", tftp_filename);
	else printf ("Download Filename '%s'.", tftp_filename);

	if (NetBootFileSize) {
		printf (" Size is 0x%x Bytes = ", NetBootFileSize<<9);
		print_size (NetBootFileSize<<9, "");
	}

	putc ('\n');

	if( TftpState == STATE_WRQ)
	{
		printf ("Upload from address: 0x%lx, ", upload_addr);
		printf ("%lu.%03lu MB to be send ...\n", upload_size>>20, (upload_size&0x0FFFFF)>>10);
		printf ("Uploading: *\b");
	}
	else 
	{
		printf ("Download to address: 0x%lx\n", load_addr);
		printf ("Downloading: *\b");
	}

	NetSetTimeout (TIMEOUT, TftpTimeout);
	NetSetHandler (TftpHandler);

	TftpServerPort = WELL_KNOWN_PORT;
	TftpTimeoutCount = 0;
	/* Use a pseudo-random port unless a specific port is set */
	TftpOurPort = 1024 + (get_timer(0) % 3072);
#ifdef CONFIG_TFTP_PORT
	if ((ep = getenv("tftpdstp")) != NULL) {
		TftpServerPort = simple_strtol(ep, NULL, 10);
	}
	if ((ep = getenv("tftpsrcp")) != NULL) {
		TftpOurPort= simple_strtol(ep, NULL, 10);
	}
#endif
	TftpBlock = 0;

	TftpLastBlock = 0;
	TftpBlockWrap = 0;
	TftpBlockWrapOffset = 0;

	/* zero out server ether in case the server ip has changed */
	memset(NetServerEther, 0, 6);

	TftpSend ();
}

#endif /* CFG_CMD_NET */
