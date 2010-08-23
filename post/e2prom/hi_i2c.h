/*
 * extdrv/include/hi_i2c.h for Linux .
 *
 * History: 
 *      10-April-2006 create this file
 *
 *
 */

#ifndef __INC_HI_I2C_H__
#define __INC_HI_I2C_H__

extern int hi_i2c_init(void);
extern int hi_i2c_muti_read(unsigned char, unsigned int, int,unsigned char*, unsigned long);
extern int hi_i2c_muti_write(unsigned char,unsigned int,int,unsigned char*,unsigned long);

#endif 
