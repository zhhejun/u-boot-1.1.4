
#if (!(CFG_DDRC_CHIPBANK == DDRC_CHIP_4BANK)) &&(!(CFG_DDRC_CHIPBANK == DDRC_CHIP_8BANK))
#error "Sorry, CFG_DDRC_CHIPBANK not supported!"
#endif

//#define DDRC_CONF_DDRCONFIG	( (CFG_DDRC_READDELAY_CL<<8) | DDRC_CONF_ADDRESS_MAP )

#define DDRC_CONF_PHYREADDELAY	CFG_DDRCPHY_READDELAY_CL

/* THE MAP BETWEEN BUSWITH-BANK-CHIPCAP-CHIPBITS TO ROW-COL-BA2-BA1-BA0 */
#define DDRC_CONF_ADDRESS_MAP   ( (CFG_DDRC_BUSWITH<<7) | \
                                        (CFG_DDRC_CHIPBANK<<6) | \
                                        (CFG_DDRC_CHIPCAP<<3)  | \
                                        (CFG_DDRC_CHIPBITS) )

/* CAUTION: WE ARE USING:16BITS UPER, 256Mb UPER */

/* BUSWITH-16BITS */
#if (DDRC_CONF_ADDRESS_MAP == 0x011)
	/*	16BITS 256Mb 4BANK */
	#define DDR_ROW 13
	#define DDR_COL 9
	#define DDR_BA1 11
	#define DDR_BA0 10
#elif (DDRC_CONF_ADDRESS_MAP == 0x19)
	/*	16BITS 512Mb 4BANK */
	#define DDR_ROW 13
	#define DDR_COL 10
	#define DDR_BA1 12
	#define DDR_BA0 11
#elif (DDRC_CONF_ADDRESS_MAP == 0x61)
	/*	16BITS 1GMb 8BANK */
	#define DDR_ROW 13
	#define DDR_COL 10
	#define DDR_BA2 13
	#define DDR_BA1 12
	#define DDR_BA0 11
#elif (DDRC_CONF_ADDRESS_MAP == 0x69)
	/*	16BITS 2GMb 8BANK */
	#define DDR_ROW 14
	#define DDR_COL 9
	#define DDR_BA2 12
	#define DDR_BA1 11
	#define DDR_BA0 10
#endif

/* BUSWITH-32BITS */
#if (DDRC_CONF_ADDRESS_MAP == 0x91)
	/*	16BITS 256Mb 4BANK */
	#define DDR_ROW 13
	#define DDR_COL 9
	#define DDR_BA1 12
	#define DDR_BA0 11
#elif (DDRC_CONF_ADDRESS_MAP == 0x99)
	/*	16BITS 512Mb 4BANK */
	#define DDR_ROW 13
	#define DDR_COL 10
	#define DDR_BA1 13
	#define DDR_BA0 12
#elif (DDRC_CONF_ADDRESS_MAP == 0xE1)
	/*	16BITS 1Gb 8BANK */
	#define DDR_ROW 13
	#define DDR_COL 10
	#define DDR_BA2 14
	#define DDR_BA1 13
	#define DDR_BA0 12
#elif (DDRC_CONF_ADDRESS_MAP == 0xE9)
	#define DDR_ROW 14
	#define DDR_COL 9
	#define DDR_BA2 13
	#define DDR_BA1 12
	#define DDR_BA0 11
#endif

#if !defined(DDR_BA1) || !defined(DDR_BA0) || !defined(DDR_ROW) || !defined(DDR_COL)
	#error "DDR_BA1, DDR_BA0, DDR_ROW, DDR_COL not defined!"
#endif

#if (CFG_DDRC_READDELAY_CL == DDRC_READDELAY_2)
	#define DDR_CAS_VAL 2
#elif (CFG_DDRC_READDELAY_CL == DDRC_READDELAY_2_5)
	#define DDR_CAS_VAL 6
#elif (CFG_DDRC_READDELAY_CL == DDRC_READDELAY_3)
	#define DDR_CAS_VAL 3
#elif (CFG_DDRC_READDELAY_CL == DDRC_READDELAY_4)
	#define DDR_CAS_VAL 4
#elif (CFG_DDRC_READDELAY_CL == DDRC_READDELAY_5)
	#define DDR_CAS_VAL 5
#elif (CFG_DDRC_READDELAY_CL == DDRC_READDELAY_6)
	#define DDR_CAS_VAL 6
#else
	#error "CFG_DDRC_READDELAY_CL not supported!"
#endif

#define CFG_DDR_BURST_LENGTH	2

#if (CFG_DDR_BURST_LENGTH == 2)
	#define DDR_BURST_LEN 1
#elif (CFG_DDR_BURST_LENGTH == 4)
	#define DDR_BURST_LEN 2
#elif (CFG_DDR_BURST_LENGTH == 8)
	#define DDR_BURST_LEN 3
#else
	#error "CFG_DDR_BURST_LENGTH not supported!"
#endif
/* DDRII TYPE */
#define CFG_DDRC_TYPE		0x7

/* */
#define CFG_DDRC_AUTO_REFRESH  0x8

#if (DDR_ROW == 11)
#define CFG_DDR_ROW  0x0
#elif (DDR_ROW == 12)
#define CFG_DDR_ROW  0x1
#elif (DDR_ROW == 13)
#define CFG_DDR_ROW  0x2
#elif (DDR_ROW == 14)
#define CFG_DDR_ROW  0x3
#elif (DDR_ROW == 15)
#define CFG_DDR_ROW  0x4
#elif (DDR_ROW == 16)
#define CFG_DDR_ROW  0x5
#endif

#if (DDR_COL == 8)
#define CFG_DDR_COL  0x0
#elif (DDR_COL == 9)
#define CFG_DDR_COL  0x1
#elif (DDR_COL == 10)
#define CFG_DDR_COL  0x2
#elif (DDR_COL == 11)
#define CFG_DDR_COL  0x3
#elif (DDR_COL == 12)
#define CFG_DDR_COL  0x4
#endif

#define REG_DDRC_CONF_VALUE	 ((CFG_DDRC_AUTO_REFRESH << 28)  | \
								(CFG_DDRC_TYPE << 12)		 | \
								(CFG_DDRC_CHIPBANK << 7)	 | \
								(CFG_DDR_ROW << 4) 		 		 | \
								(CFG_DDR_COL))
#define CFG_DDRC_WR   0x3
#if (CFG_DDRC_WR == 2)
	#define CFG_DDR_MRS_WR			0x1
#elif (CFG_DDRC_WR == 3)
	#define CFG_DDR_MRS_WR			0x2
#elif (CFG_DDRC_WR == 4)
	#define CFG_DDR_MRS_WR			0x3
#elif (CFG_DDRC_WR == 5)
	#define CFG_DDR_MRS_WR			0x4
#elif (CFG_DDRC_WR == 6)
	#define CFG_DDR_MRS_WR			0x5
#endif

#define CFG_DDR_MRS_CL			DDR_CAS_VAL
#define CFG_DDRC_MRS            ((CFG_DDR_MRS_WR << 9)| (CFG_DDR_MRS_CL << 4) | 0x0102)

#define CFG_DDRC_EMRS1			0x0446

#define REG_DDRC_EMRS01_CONFIG_VALUE	((CFG_DDRC_EMRS1 << 16) | ( CFG_DDRC_MRS))
