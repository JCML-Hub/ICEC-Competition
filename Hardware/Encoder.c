/**
  ************************************* Copyright ****************************** 
  *                 (C) Copyright 2023,--,China, CUIT.
  *                            All Rights Reserved                          
  *                     By(JCML)  
  * FileName   : Encoder.c   
  * Version    : v1.0		
  * Author     : JCML			
  * Date       : 2023-01-08         
  * Description: ��ת������ʹ���ⲿ�жϼƴ� �������ƴ�Ч��
  * May Be Used Function List:  

******************************************************************************
 */

#include "main.h"
#include "gpio.h"
#include "stm32f1xx_hal_gpio.h"

int16_t Encoder_Count;

/***********************************************************
*@fuction	:Encode_Get
*@brief		:���س��õ��ļ���ֵ������Encoder_Countһֱ�ڱ仯��Ҫһ��temp�ݴ�
*@param		:void
*@return	:Encoder_Count
*@author	:JCML
*@date		:2023-01-08
***********************************************************/

int16_t Encode_Get(void)
{
	int16_t Temp;
	Temp = -Encoder_Count;
	Encoder_Count = 0;
	return Temp;
}
/***********************************************************
*@fuction	:HAL_GPIO_EXTI_Callback
*@brief		:���������
*@param		:GPIO_Pin��������
*@return	:void
*@author	:JCML
*@date		:2023-01-08
***********************************************************/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == ROTARY_S1_Pin)//�ж�����һ�����ŵ��ź�
	{
		if (HAL_GPIO_ReadPin(ROTARY_S1_GPIO_Port, ROTARY_S1_Pin) == 0)
		{
			if (HAL_GPIO_ReadPin(ROTARY_S2_GPIO_Port, ROTARY_S2_Pin) == 0)//����ƽ�ж�
			{
				Encoder_Count--;
			}
		}
	}
	else if (GPIO_Pin == ROTARY_S2_Pin)
	{
		if (HAL_GPIO_ReadPin(ROTARY_S2_GPIO_Port, ROTARY_S2_Pin) == 0)//����ƽ�ж�
		{
			if (HAL_GPIO_ReadPin(ROTARY_S1_GPIO_Port, ROTARY_S1_Pin) == 0)//����ƽ�ж�
			{
				Encoder_Count++;
			}
		}
	}

}


