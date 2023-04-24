/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2022,--,China, CUIT.
  *                            All Rights Reserved
  *                              
  *                     By(JCML)
    *
  *    
  * FileName   : Key.c   
  * Version    : v1.0		
  * Author     : JCML			
  * Date       : 2022-12-28         
  * Description: 以B1和B11分别为按键1和按键2的按键读取函数, 在做比较大的工程时最好外加一个变量存放按键值
  * May Be Used Function List:  
1. Head file£º 
#include "Key.h" 

2. Function File£º
Key_Init();
Key_GetNum();


******************************************************************************
 */

#include "main.h"
#include "Delay.h"

/***********************************************************
*@fuction	:Key_GetNum();
*@brief		:获取被按下的键码
*@param		:void
*@return	:KeyNum
*@author	:JCML
*@date		:2022-12-28
***********************************************************/

uint8_t GetKeyNum(void)
{
	uint8_t KeyNum = 0;
	if (HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin)==0)
	{
		Delay_ms(10);
		while(HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin)==0)
		Delay_ms(10);
		KeyNum=1;
	}
	
	if (HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin)==0)
	{
		Delay_ms(10);
		while(HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin)==0)
		Delay_ms(10);
		KeyNum=2;
	}
	if (HAL_GPIO_ReadPin(Key3_GPIO_Port, Key3_Pin)==0)
	{
		Delay_ms(10);
		while(HAL_GPIO_ReadPin(Key3_GPIO_Port, Key3_Pin)==0)
		Delay_ms(10);
		KeyNum=3;
	}	
	return KeyNum;
}

