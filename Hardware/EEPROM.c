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
  * Description: ģ��IICͨ�ŷ�ʽ��дEEPROM������
******************************************************************************
 */

#include "EEPROM.h"
#include "Delay.h"

static void I2C_GPIO_Config(void)
{
	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	I2C_Stop_();
}

void I2C_Start_(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	SDA_L;
	SCL_H;
	Delay_us(2);
	SDA_H;
}


void I2C_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 SDA_H; // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		Delay_us(2);
	}
}


uint8_t I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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

	SDA_H;	/* CPU�ͷ�SDA���� */
		Delay_us(2);
	SCL_H;	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
		Delay_us(2);
	if (SDA_READ)	/* CPU��ȡSDA����״̬ */
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
	SDA_L;	/* CPU����SDA = 0 */
		Delay_us(2);
	SCL_H;	/* CPU����1��ʱ�� */
		Delay_us(2);
	SCL_L;
		Delay_us(2);
	SDA_H;	/* CPU�ͷ�SDA���� */
}


void I2C_NAck(void)
{
	SDA_H;	/* CPU����SDA = 1 */
		Delay_us(2);
	SCL_H;	/* CPU����1��ʱ�� */
	Delay_us(2);
	SCL_L;
	Delay_us(2);	
}


uint8_t I2C_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	I2C_GPIO_Config();		/* ����GPIO */

	
	I2C_Start_();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	I2C_SendByte(_Address |I2C_WR);
	ucAck = I2C_WaitAck();	/* ����豸��ACKӦ�� */

	I2C_Stop_();			/* ����ֹͣ�ź� */

	return ucAck;
}


/***********************************************************
*@���� 	:EEPROM_Check
*@����	:����������Ƿ����豸
*@����	:��
*@����ֵ	:�Ƿ����豸
*@����	:JCML
*@����	:2023-03-05
***********************************************************/

uint8_t EEPROM_Check(void)
{
	if (I2C_CheckDevice(EEPROM_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{

		I2C_Stop_();				// ʧ�ܺ��мǷ���I2C����ֹͣ�ź� 
		return 0;
	}
}

/***********************************************************
*@���� 	:EEPROM_ReadBytes
*@����	:��ȡEEPROM����
*@����	:WriteBuf ָ��Ŀ�������ָ�룬 AddressEEPROM�ĵ�ַΪ0���ɣ�Size�����С
*@����ֵ	:�Ƿ�д��ɹ�
*@����	:JCML
*@����	:2023-03-05
***********************************************************/

uint8_t EEPROM_ReadBytes(uint8_t *_pReadBuf, uint16_t Address, uint16_t _Size)
{
	uint16_t i;
	// ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� 
	I2C_Start_(); //1.����IIC�ź�
	I2C_SendByte(EEPROM_DEV_ADDR | I2C_WR);	//2.��������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� 

	if (I2C_WaitAck() != 0)	//3.�ȴ�ACK��Ӧ
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	I2C_SendByte((uint8_t)Address);// 4.�����ֽڵ�ַ
	if (I2C_WaitAck() != 0)  //	5.�ȴ�ACK 
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	I2C_Start_(); //6.��������I2C���ߣ���ʼ��ȡ����		
  I2C_SendByte(EEPROM_DEV_ADDR | I2C_RD);// 7.��������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ
	if (I2C_WaitAck() != 0)	//����ACK��Ӧ
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}		
	for (i = 0; i < _Size; i++) //9.ѭ����ȡ���� 
	{
		_pReadBuf[i] = I2C_ReadByte();	//ÿ�ζ�1���ֽ� 
		
		// ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack 
		if (i != _Size - 1)
		{
			I2C_Ack();	// ����AckӦ��
		}
		else
		{
			I2C_NAck();	// ���1���ֽڶ���󣬲���Nack��Ӧ���ź�
		}
	}	
	I2C_Stop_();  //10.ֹͣ�ź�
	return 1;	/* ִ�гɹ� */
cmd_fail: //����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 
	I2C_Stop_();  	// ����I2C����ֹͣ�ź� 
	return 0;
}


/***********************************************************
*@���� 	:EEPROM_WriteBytes
*@����	:EEPROMд������
*@����	:WriteBuf ָ��Ŀ�������ָ�룬 AddressEEPROM�ĵ�ַΪ0���ɣ�Size�����С
*@����ֵ	:�Ƿ�д��ɹ�
*@����	:JCML
*@����	:2023-03-05
***********************************************************/

uint8_t EEPROM_WriteBytes(uint8_t *WriteBuf, uint16_t Address, uint16_t _Size)
{
	uint16_t i,m;
	uint16_t Addr;	
	/* 
		д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��page size = 8��һ��ֻ��д��8bit����
		
	*/
	Addr = Address;	
	for (i = 0; i < _Size; i++)
	{
		// �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ 
		if ((i == 0) || (Addr & (EEPROM_PAGE_SIZE - 1)) == 0)
		{
			I2C_Stop_();//��1.��ֹͣ�źţ������ڲ�д������
			for (m = 0; m < 1000; m++)  //ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����
			{				
				I2C_Start_();			//2.����I2C���������ź� 	
				I2C_SendByte(EEPROM_DEV_ADDR | I2C_WR);		//2.��������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ	
				if (I2C_WaitAck() == 0)//��3��������һ��ʱ�ӣ��ȴ���Ӧ
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	// EEPROM����д��ʱ 
			}
			I2C_SendByte((uint8_t)Addr);		// ��4���������ֽڵ�ַSLAVE_ADDRESS
			if (I2C_WaitAck() != 0)		// ��5�����ȴ�ACK��Ӧ
			{
				goto cmd_fail;	// EEPROM������Ӧ�� 
			}
		}
		I2C_SendByte(WriteBuf[i]);	// 7.��ʼд������ 
		if (I2C_WaitAck() != 0)// 8.����ACK
		{
			goto cmd_fail;	// EEPROM������Ӧ�� 
		}

		Addr++;	//��ַ��1 		
	}
	I2C_Stop_();  //I2Cֹͣ�ź�
	return 1;

	cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	I2C_Stop_();	// ����I2C����ֹͣ�ź� 
	return 0;
}


/***********************************************************
*@���� 	:EEPROM_Erase
*@����	:�洢EEPROM������
*@����	:��
*@����ֵ	:1��ʾ�ɹ������� 0��ʾû��
*@����	:JCML
*@����	:2023-03-05
***********************************************************/

uint8_t EEPROM_Erase(void)
{
	uint16_t i;
	uint8_t buf[EEPROM_SIZE];
	
	/* ��仺���� */
	for (i = 0; i < EEPROM_SIZE; i++)
	{
		buf[i] = 0xFF;
	}
	
	// дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 
	if (EEPROM_WriteBytes(buf, 0, EEPROM_SIZE) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
