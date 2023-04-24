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
  *                 Description: DS1302驱动库（用HAL库编写）需要定义DS1302_CE，DS1302_IO，DS1302_SCLK三个引脚

******************************************************************************

 */

#include "main.h"
#include "DS1302.h"
#include "delay.h"


uint8_t DS1302_Time[] = {22, 12, 14, 18, 02, 00, 3};//存原始数据
uint8_t DS1302_time[6]={23,03,07,23,05,50};

char DS1302_DATA_1[6];//存日期字符串
char DS1302_DATA_2[6];//存时间字符串



/***********************************************************
*@名称 	:ds1302_DATAOUT_init
*@描述	:
*@参数	:无
*@返回值	:无
*@作者	:JCML
*@日期	:2023-03-01
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
*@名称 	:DS1302_W_Byte
*@描述	:向DS1302发送一字节数据
*@参数	:无
*@返回值	:无
*@作者	:JCML
*@日期	:2023-03-02
***********************************************************/

void DS1302_W_Byte(uint8_t data)//向DS1302发送一字节数据
{
	uint8_t count=0;
	ds1302_DATAOUT_init(); // I/O配置为输出 
	

	HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// 拉低时钟
	
	for(count=0;count<8;count++)
	{	
		HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// 拉低时钟
		if(data&0x01)
		{
			HAL_GPIO_WritePin(DS1302_IO_GPIO_Port,DS1302_IO_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(DS1302_IO_GPIO_Port,DS1302_IO_Pin, GPIO_PIN_RESET);
		}//先准备好数据再发送
		HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_SET); //拉高时钟线，发送数据
		data>>=1; 
	}

}

/***********************************************************
*@名称 	:DS1302_W_Cmd
*@描述	:向DS1302发送指定数据
*@参数	:address 对应位的地址， data 数据
*@返回值	:无
*@作者	:JCML
*@日期	:2023-03-02
***********************************************************/

void DS1302_W_Cmd(uint8_t address,uint8_t data)//向指定寄存器地址发送数据
{
	uint8_t temp1=address;
	uint8_t temp2=data;
	
	

	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_RESET); // 拉低RST
	
	HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// 拉低时钟
	Delay_us(1);
	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_SET); // 拉高RST
	Delay_us(2);
	DS1302_W_Byte(temp1); // 写命令
	DS1302_W_Byte(temp2); // 写数据
	
	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_RESET); // 拉低RST
	HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// 拉低时钟
	Delay_us(2);

}
/***********************************************************
*@名称 	:DS1302_R_Time
*@描述	:从指定地址读取一字节数据
*@参数	:address 对应位的地址
*@返回值	:对应地址的时间数据
*@作者	:JCML
*@日期	:2023-03-02
***********************************************************/

uint8_t DS1302_R_Time(uint8_t address)
{
	uint8_t temp3=address;
	uint8_t count=0;
	uint8_t return_data=0x00;
	

	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_RESET); // 拉低RST
	HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// 拉低时钟
	Delay_us(3);
	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_SET); // 拉高RST
	Delay_us(3);
	
	DS1302_W_Byte(temp3); // 写地址
	
	ds1302_DATAINPUT_init();//配置I/O口为输入
	 
	for(count=0;count<8;count++)
	{
		Delay_us(2);//使电平持续一段时间
		return_data>>=1;
		HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_SET);// 拉高时钟
		Delay_us(4);//使高电平持续一段时间
		HAL_GPIO_WritePin(DS1302_SCLK_GPIO_Port,DS1302_SCLK_Pin, GPIO_PIN_RESET);// 拉低时钟
		Delay_us(14);//延时14us后再去读取电压，更加准确
		if(HAL_GPIO_ReadPin(DS1302_IO_GPIO_Port,DS1302_IO_Pin)) 
		{return_data=return_data|0x80;}
	}
	Delay_us(2);
	HAL_GPIO_WritePin(DS1302_CE_GPIO_Port,DS1302_CE_Pin, GPIO_PIN_RESET); // 拉低RST
	HAL_GPIO_WritePin(DS1302_IO_GPIO_Port,DS1302_IO_Pin, GPIO_PIN_RESET);	//	DATA拉低
	
	return return_data; // 返回数据

}

/***********************************************************
*@名称 	:DS1302_Set
*@描述	:设定时间，只需调用一次即可
*@参数	:前一个参数对应以下的几个地址， 后一个直接在x后修改对应数字即可
		秒：1000 0000即0x80
		分：1000 0010即0x82
		时：1000 0100即0x84
		日：1000 0110即0x86
		月：1000 1000即0x88
		周：1000 1010即0x8a
		年：1000 1100即0x8c
*@返回值	:无
*@作者	:JCML
*@日期	:2023-03-02
***********************************************************/

void DS1302_Set(uint8_t SetTime[6])
{
	
	DS1302_W_Cmd(0x8e,0x00);//关闭写保护
	DS1302_W_Cmd(0x80,SetTime[5]/10*16+SetTime[5]%10);//seconds
	DS1302_W_Cmd(0x82,SetTime[4]/10*16+SetTime[4]%10);//minutes
	DS1302_W_Cmd(0x84,SetTime[3]/10*16+SetTime[3]%10);//hours
	DS1302_W_Cmd(0x86,SetTime[2]/10*16+SetTime[2]%10);//date
	DS1302_W_Cmd(0x88,SetTime[1]/10*16+SetTime[1]%10);//months
	DS1302_W_Cmd(0x8a,0x07);//days
	DS1302_W_Cmd(0x8d,SetTime[0]/10*16+SetTime[0]%10);//year
	DS1302_W_Cmd(0x8e,0x80);//开启写保护
}

/***********************************************************
*@名称 	:DS1302_Read
*@描述	:读取并将时间转存至DS1302_Time数组中
*@参数	:无
*@返回值	:无
*@作者	:JCML
*@日期	:2023-03-02
***********************************************************/

void DS1302_Read(void)
{
	uint8_t Temp;
	Temp = DS1302_R_Time(DS1302_SECOND);//读秒
	DS1302_Time[0] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_MINUTE);//读分
	DS1302_Time[1] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_HOUR);//读时
	DS1302_Time[2] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_DATE);//读日
	DS1302_Time[3] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_MONTH);//读月
	DS1302_Time[4] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_YEAR);//读年
	DS1302_Time[5] = Temp/16*10+Temp%16;
	Temp = DS1302_R_Time(DS1302_DAY);//读星期
	DS1302_Time[6] = Temp/16*10+Temp%16;
}

/***********************************************************
*@名称 	:DS1302_Time_Convert
*@描述	:转换数字的时间和日期为字符串好显示
*@参数	:无
*@返回值	:无
*@作者	:JCML
*@日期	:2023-03-02
***********************************************************/

void DS1302_Time_Convert(void)
{
	DS1302_Read();  //BCD码转换为10进制
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
	DS1302_Read();  //BCD码转换为10进制
	DS1302_time[0]=DS1302_Time[5];
	DS1302_time[1]=DS1302_Time[4];
	DS1302_time[2]=DS1302_Time[3];
	DS1302_time[3]=DS1302_Time[2];
	DS1302_time[4]=DS1302_Time[1];
	DS1302_time[5]=DS1302_Time[0];
}

