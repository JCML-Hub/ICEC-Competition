/**
  ************************************* Copyright ****************************** 
  *

  *                 (C) Copyright 2023,--,China, CUIT.
  *                            All Rights Reserved
  *                     By(JCML)
  * FileName   : iic_analog.c   
  * Version    : v1.0		
  * Author     : JCML			
  * Date       : 2023-03-05         
  * Description: 模拟IIC通信方式读写EEPROM的数据
******************************************************************************
 */

#include "EEPROM.h"
#include "Delay.h"

static void I2C_GPIO_Config(void)
{
	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	I2C_Stop_();
}

void I2C_Start_(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	SDA_H;
	SCL_H;
	Delay_us(2);
	SDA_L;
	Delay_us(2);
	SCL_L;
	Delay_us(2);
}


void I2C_Stop_(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	SDA_L;
	SCL_H;
	Delay_us(2);
	SDA_H;
}


void I2C_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}
			Delay_us(2);
		SCL_H;
			Delay_us(2);
		SCL_L;
		if (i == 7)
		{
			 SDA_H; // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		Delay_us(2);
	}
}


uint8_t I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		SCL_H;
			Delay_us(2);
		if (SDA_READ)
		{
			value++;
		}
		SCL_L;
			Delay_us(2);
	}
	return value;
}


uint8_t I2C_WaitAck(void)
{
	uint8_t re;

	SDA_H;	/* CPU释放SDA总线 */
		Delay_us(2);
	SCL_H;	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
		Delay_us(2);
	if (SDA_READ)	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	SCL_L;
		Delay_us(2);
	return re;
}


void I2C_Ack(void)
{
	SDA_L;	/* CPU驱动SDA = 0 */
		Delay_us(2);
	SCL_H;	/* CPU产生1个时钟 */
		Delay_us(2);
	SCL_L;
		Delay_us(2);
	SDA_H;	/* CPU释放SDA总线 */
}


void I2C_NAck(void)
{
	SDA_H;	/* CPU驱动SDA = 1 */
		Delay_us(2);
	SCL_H;	/* CPU产生1个时钟 */
	Delay_us(2);
	SCL_L;
	Delay_us(2);	
}


uint8_t I2C_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	I2C_GPIO_Config();		/* 配置GPIO */

	
	I2C_Start_();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	I2C_SendByte(_Address |I2C_WR);
	ucAck = I2C_WaitAck();	/* 检测设备的ACK应答 */

	I2C_Stop_();			/* 发送停止信号 */

	return ucAck;
}


/***********************************************************
*@名称 	:EEPROM_Check
*@描述	:检测总线上是否有设备
*@参数	:无
*@返回值	:是否有设备
*@作者	:JCML
*@日期	:2023-03-05
***********************************************************/

uint8_t EEPROM_Check(void)
{
	if (I2C_CheckDevice(EEPROM_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{

		I2C_Stop_();				// 失败后，切记发送I2C总线停止信号 
		return 0;
	}
}

/***********************************************************
*@名称 	:EEPROM_ReadBytes
*@描述	:读取EEPROM数据
*@参数	:WriteBuf 指向目的数组的指针， AddressEEPROM的地址为0即可，Size数组大小
*@返回值	:是否写入成功
*@作者	:JCML
*@日期	:2023-03-05
***********************************************************/

uint8_t EEPROM_ReadBytes(uint8_t *_pReadBuf, uint16_t Address, uint16_t _Size)
{
	uint16_t i;
	// 采用串行EEPROM随即读取指令序列，连续读取若干字节 
	I2C_Start_(); //1.开启IIC信号
	I2C_SendByte(EEPROM_DEV_ADDR | I2C_WR);	//2.发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 

	if (I2C_WaitAck() != 0)	//3.等待ACK响应
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	I2C_SendByte((uint8_t)Address);// 4.发送字节地址
	if (I2C_WaitAck() != 0)  //	5.等待ACK 
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	I2C_Start_(); //6.重新启动I2C总线，开始读取数据		
  I2C_SendByte(EEPROM_DEV_ADDR | I2C_RD);// 7.发起控制字节，高7bit是地址，bit0是读写控制位
	if (I2C_WaitAck() != 0)	//发送ACK响应
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}		
	for (i = 0; i < _Size; i++) //9.循环读取数据 
	{
		_pReadBuf[i] = I2C_ReadByte();	//每次读1个字节 
		
		// 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack 
		if (i != _Size - 1)
		{
			I2C_Ack();	// 产生Ack应答
		}
		else
		{
			I2C_NAck();	// 最后1个字节读完后，产生Nack非应答信号
		}
	}	
	I2C_Stop_();  //10.停止信号
	return 1;	/* 执行成功 */
cmd_fail: //命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 
	I2C_Stop_();  	// 发送I2C总线停止信号 
	return 0;
}


/***********************************************************
*@名称 	:EEPROM_WriteBytes
*@描述	:EEPROM写入数据
*@参数	:WriteBuf 指向目的数组的指针， AddressEEPROM的地址为0即可，Size数组大小
*@返回值	:是否写入成功
*@作者	:JCML
*@日期	:2023-03-05
***********************************************************/

uint8_t EEPROM_WriteBytes(uint8_t *WriteBuf, uint16_t Address, uint16_t _Size)
{
	uint16_t i,m;
	uint16_t Addr;	
	/* 
		写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page。page size = 8，一次只能写入8bit数据
		
	*/
	Addr = Address;	
	for (i = 0; i < _Size; i++)
	{
		// 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址 
		if ((i == 0) || (Addr & (EEPROM_PAGE_SIZE - 1)) == 0)
		{
			I2C_Stop_();//　1.发停止信号，启动内部写操作　
			for (m = 0; m < 1000; m++)  //通过检查器件应答的方式，判断内部写操作是否完成
			{				
				I2C_Start_();			//2.发起I2C总线启动信号 	
				I2C_SendByte(EEPROM_DEV_ADDR | I2C_WR);		//2.发起控制字节，高7bit是地址，bit0是读写控制位	
				if (I2C_WaitAck() == 0)//第3步：发送一个时钟，等待响应
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	// EEPROM器件写超时 
			}
			I2C_SendByte((uint8_t)Addr);		// 第4步：发送字节地址SLAVE_ADDRESS
			if (I2C_WaitAck() != 0)		// 第5步：等待ACK响应
			{
				goto cmd_fail;	// EEPROM器件无应答 
			}
		}
		I2C_SendByte(WriteBuf[i]);	// 7.开始写入数据 
		if (I2C_WaitAck() != 0)// 8.发送ACK
		{
			goto cmd_fail;	// EEPROM器件无应答 
		}

		Addr++;	//地址增1 		
	}
	I2C_Stop_();  //I2C停止信号
	return 1;

	cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	I2C_Stop_();	// 发送I2C总线停止信号 
	return 0;
}


/***********************************************************
*@名称 	:EEPROM_Erase
*@描述	:存储EEPROM的数据
*@参数	:无
*@返回值	:1表示成功擦除， 0表示没有
*@作者	:JCML
*@日期	:2023-03-05
***********************************************************/

uint8_t EEPROM_Erase(void)
{
	uint16_t i;
	uint8_t buf[EEPROM_SIZE];
	
	/* 填充缓冲区 */
	for (i = 0; i < EEPROM_SIZE; i++)
	{
		buf[i] = 0xFF;
	}
	
	// 写EEPROM, 起始地址 = 0，数据长度为 256 
	if (EEPROM_WriteBytes(buf, 0, EEPROM_SIZE) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
