#ifndef	__SOFTWARE_IIC_H__
#define __SOFTWARE_IIC_H__

#include "main.h"

//µ×²ãº¯Êý
void I2C_W_SCL(uint8_t x);
void I2C_W_SDA(uint8_t x);
uint8_t I2C_R_SCL(void);
uint8_t I2C_R_SDA(void);
//I2CÖ´ÐÐº¯Êý
void I2C_Stop		(void);
void I2C_Start		(void);
void I2C_SendACK	(void);
void I2C_NoACK		(void);
void Write_I2C_Byte	(unsigned char I2C_Byte);
unsigned char I2C_Wait_Ack	(void);
unsigned char I2C_Read_Byte	(void);

#endif

