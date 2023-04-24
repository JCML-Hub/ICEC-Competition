/**
  ************************************* Copyright ****************************** 
  *                 (C) Copyright 2023,--,China, CUIT.
  *                            All Rights Reserved                          
  *                     By(JCML)  
  * FileName   : Encoder.c   
  * Version    : v1.0		
  * Author     : JCML			
  * Date       : 2023-01-08         
  * Description: 旋转编码器使用外部中断计次 有正反计次效果
  * May Be Used Function List:  

******************************************************************************
 */

#include "main.h"
#include "gpio.h"
#include "stm32f1xx_hal_gpio.h"

int16_t Encoder_Count;

/***********************************************************
*@fuction	:Encode_Get
*@brief		:返回除得到的计数值，由于Encoder_Count一直在变化需要一个temp暂存
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
*@brief		:正向反向计数
*@param		:GPIO_Pin触发引脚
*@return	:void
*@author	:JCML
*@date		:2023-01-08
***********************************************************/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == ROTARY_S1_Pin)//判断是哪一个引脚的信号
	{
		if (HAL_GPIO_ReadPin(ROTARY_S1_GPIO_Port, ROTARY_S1_Pin) == 0)
		{
			if (HAL_GPIO_ReadPin(ROTARY_S2_GPIO_Port, ROTARY_S2_Pin) == 0)//检测电平判断
			{
				Encoder_Count--;
			}
		}
	}
	else if (GPIO_Pin == ROTARY_S2_Pin)
	{
		if (HAL_GPIO_ReadPin(ROTARY_S2_GPIO_Port, ROTARY_S2_Pin) == 0)//检测电平判断
		{
			if (HAL_GPIO_ReadPin(ROTARY_S1_GPIO_Port, ROTARY_S1_Pin) == 0)//检测电平判断
			{
				Encoder_Count++;
			}
		}
	}

}


