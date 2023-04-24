/**
  ************************************* Copyright ****************************** 
  *

  *                 (C) Copyright 2023,--,China, CUIT.
  *                 All Rights Reserved
  *                 By(JCML)
  *                 FileName   : DS1302.c   
  *                 Version    : v1.0		
  *                 Author     : JCML			
  *                 Date       : 2023-03-02         
  *                 Description: DS1302�����⣨��HAL���д����Ҫ����DS1302_CE��DS1302_IO��DS1302_SCLK��������

******************************************************************************

 */

#include "main.h"
#include "DS1302.h"
#include "delay.h"


uint8_t DS1302_Time[] = {22, 12, 14, 18, 02, 00, 3};//��ԭʼ����
uint8_t DS1302_time[6]={23,03,07,23,05,50};

char DS1302_DATA_1[6];//�������ַ���
char DS1302_DATA_2[6];//��ʱ���ַ���



/***********************************************************
*@���� 	:ds1302_DATAOUT_init
*@����	:
*@����	:��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-01
***********************************************************/

void ds1302_DATAOUT_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DS1302_IO_GPIO_Port,DS1302_IO_Pin, GPIO_PIN_RESET);
	

	/*Configure GPIO pin : DATA_Pin */

  GPIO_InitStruct.Pin = DS1302_IO_Pin;   
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DS1302_IO_GPIO_Port, &GPIO_InitStruct);
}

void ds1302_DATAINPUT_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

	 /*Configure GPIO pin : PA2 */

  GPIO_InitStruct.Pin = DS1302_IO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DS1302_IO_GPIO_Port, &GPIO_InitStruct);
}

/***********************************************************
*@���� 	:DS1302_W_Byte
*@����	:��DS1302����һ�ֽ�����
*@����	:��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-02
***********************************************************/

void DS1302_W_Byte(uint8_t data)//��DS1302����һ�ֽ�����
{
	uint8_t count=0;
	ds1302_DATAOUT_init(); // I/O����Ϊ��� 
	

	HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// ����ʱ��
	
	for(count=0;count<8;count++)
	{	
		HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// ����ʱ��
		if(data&0x01)
		{
			HAL_GPIO_WritePin(DS1302_IO_GPIO_Port,DS1302_IO_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(DS1302_IO_GPIO_Port,DS1302_IO_Pin, GPIO_PIN_RESET);
		}//��׼���������ٷ���
		HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_SET); //����ʱ���ߣ���������
		data>>=1; 
	}

}

/***********************************************************
*@���� 	:DS1302_W_Cmd
*@����	:��DS1302����ָ������
*@����	:address ��Ӧλ�ĵ�ַ�� data ����
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-02
***********************************************************/

void DS1302_W_Cmd(uint8_t address,uint8_t data)//��ָ���Ĵ�����ַ��������
{
	uint8_t temp1=address;
	uint8_t temp2=data;
	
	

	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_RESET); // ����RST
	
	HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// ����ʱ��
	Delay_us(1);
	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_SET); // ����RST
	Delay_us(2);
	DS1302_W_Byte(temp1); // д����
	DS1302_W_Byte(temp2); // д����
	
	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_RESET); // ����RST
	HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// ����ʱ��
	Delay_us(2);

}
/***********************************************************
*@���� 	:DS1302_R_Time
*@����	:��ָ����ַ��ȡһ�ֽ�����
*@����	:address ��Ӧλ�ĵ�ַ
*@����ֵ	:��Ӧ��ַ��ʱ������
*@����	:JCML
*@����	:2023-03-02
***********************************************************/

uint8_t DS1302_R_Time(uint8_t address)
{
	uint8_t temp3=address;
	uint8_t count=0;
	uint8_t return_data=0x00;
	

	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_RESET); // ����RST
	HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// ����ʱ��
	Delay_us(3);
	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_SET); // ����RST
	Delay_us(3);
	
	DS1302_W_Byte(temp3); // д��ַ
	
	ds1302_DATAINPUT_init();//����I/O��Ϊ����
	 
	for(count=0;count<8;count++)
	{
		Delay_us(2);//ʹ��ƽ����һ��ʱ��
		return_data>>=1;
		HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_SET);// ����ʱ��
		Delay_us(4);//ʹ�ߵ�ƽ����һ��ʱ��
		HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// ����ʱ��
		Delay_us(14);//��ʱ14us����ȥ��ȡ��ѹ������׼ȷ
		if(HAL_GPIO_ReadPin(DS1302_IO_GPIO_Port,DS1302_IO_Pin)) 
		{return_data=return_data|0x80;}
	}
	Delay_us(2);
	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_RESET); // ����RST
	HAL_GPIO_WritePin(DS1302_IO_GPIO_Port,DS1302_IO_Pin, GPIO_PIN_RESET);	//	DATA����
	
	return return_data; // ��������

}

/***********************************************************
*@���� 	:DS1302_Set
*@����	:�趨ʱ�䣬ֻ�����һ�μ���
*@����	:ǰһ��������Ӧ���µļ�����ַ�� ��һ��ֱ����x���޸Ķ�Ӧ���ּ���
		�룺1000 0000��0x80
		�֣�1000 0010��0x82
		ʱ��1000 0100��0x84
		�գ�1000 0110��0x86
		�£�1000 1000��0x88
		�ܣ�1000 1010��0x8a
		�꣺1000 1100��0x8c
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-02
***********************************************************/

void DS1302_Set(uint8_t SetTime[6])
{
	
	DS1302_W_Cmd(0x8e,0x00);//�ر�д����
	DS1302_W_Cmd(0x80,SetTime[5]/10*16+SetTime[5]%10);//seconds
	DS1302_W_Cmd(0x82,SetTime[4]/10*16+SetTime[4]%10);//minutes
	DS1302_W_Cmd(0x84,SetTime[3]/10*16+SetTime[3]%10);//hours
	DS1302_W_Cmd(0x86,SetTime[2]/10*16+SetTime[2]%10);//date
	DS1302_W_Cmd(0x88,SetTime[1]/10*16+SetTime[1]%10);//months
	DS1302_W_Cmd(0x8a,0x07);//days
	DS1302_W_Cmd(0x8d,SetTime[0]/10*16+SetTime[0]%10);//year
	DS1302_W_Cmd(0x8e,0x80);//����д����
}

/***********************************************************
*@���� 	:DS1302_Read
*@����	:��ȡ����ʱ��ת����DS1302_Time������
*@����	:��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-02
***********************************************************/

void DS1302_Read(void)
{
	uint8_t Temp;
	Temp = DS1302_R_Time(DS1302_SECOND);//����
	DS1302_Time[0] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_MINUTE);//����
	DS1302_Time[1] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_HOUR);//��ʱ
	DS1302_Time[2] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_DATE);//����
	DS1302_Time[3] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_MONTH);//����
	DS1302_Time[4] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_YEAR);//����
	DS1302_Time[5] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_DAY);//������
	DS1302_Time[6] = Temp/16*10+Temp%16;
}

/***********************************************************
*@���� 	:DS1302_Time_Convert
*@����	:ת�����ֵ�ʱ�������Ϊ�ַ�������ʾ
*@����	:��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-02
***********************************************************/

void DS1302_Time_Convert(void)
{
	DS1302_Read();  //BCD��ת��Ϊ10����
	DS1302_DATA_1[0]='0'+DS1302_Time[4]/10;
	DS1302_DATA_1[1]='0'+DS1302_Time[4]%10;
	DS1302_DATA_1[2]='/';
	DS1302_DATA_1[3]='0'+DS1302_Time[3]/10;
	DS1302_DATA_1[4]='0'+DS1302_Time[3]%10;
	DS1302_DATA_1[5]='\0';
	
	DS1302_DATA_2[0]='0'+DS1302_Time[2]/10;
	DS1302_DATA_2[1]='0'+DS1302_Time[2]%10;
	DS1302_DATA_2[2]=':';
	DS1302_DATA_2[3]='0'+DS1302_Time[1]/10;
	DS1302_DATA_2[4]='0'+DS1302_Time[1]%10;
	DS1302_DATA_2[5]='\0';
}

void DS1302_Time_Trans(void)
{
	DS1302_Read();  //BCD��ת��Ϊ10����
	DS1302_time[0]=DS1302_Time[5];
	DS1302_time[1]=DS1302_Time[4];
	DS1302_time[2]=DS1302_Time[3];
	DS1302_time[3]=DS1302_Time[2];
	DS1302_time[4]=DS1302_Time[1];
	DS1302_time[5]=DS1302_Time[0];
}

