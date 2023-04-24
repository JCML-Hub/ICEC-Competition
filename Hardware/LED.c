/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2022,--,China, CUIT.
  *                            All Rights Reserved
  *                              
  *                     By(JCML)
    *
  *    
  * FileName   : LED.c   
  * Version    : v1.0		
  * Author     : JCML			
  * Date       : 2022-12-28         
  * Description: 引脚为A1和A2的两个LED开启关闭以及电平翻转
  * May Be Used Function List:  
1. Head file￡o 
#include "LED.h" 

2. Function File￡o
LED_Init();
LED0_ON();
LED0_OFF();
LED0_Turn();
LED1_ON();
LED1_OFF();
LED1_Turn();

******************************************************************************
 */

#include "main.h"
#include "LED.h"

/***********************************************************
*@fuction	:LED_Init();
*@brief		:LED的初始化
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-28
***********************************************************/

void LED_Init(void)
{
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
}

/***********************************************************
*@fuction	:LED0_ON();
*@brief		:LED0开
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-28
***********************************************************/

void LED1_ON(void)
{
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	
}
/***********************************************************
*@fuction	:LED0_OFF();
*@brief		:LED0关
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-28
***********************************************************/

void LED1_OFF(void)
{
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
}
/***********************************************************
*@fuction	:LED0_Turn();
*@brief		:LED0电平翻转
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-28
***********************************************************/

void LED1_Turn(void)	//电平反翻转
{
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

/***********************************************************
*@fuction	:LED1_ON();
*@brief		:LED1开
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-28
***********************************************************/

void LED2_ON(void)
{
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
	
}
/***********************************************************
*@fuction	:LED1_OFF();
*@brief		:LED1关
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-28
***********************************************************/

void LED2_OFF(void)
{
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
}
/***********************************************************
*@fuction	:LED1_Turn();
*@brief		:LED1电平翻转
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-28
***********************************************************/

void LED2_Turn(void)	//电平反翻转
{
	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}
/***********************************************************
*@fuction	:Buzzer_ON();
*@brief		:Buzzer开
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-28
***********************************************************/
void Buzzer_ON(void)
{
	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
	
}
/***********************************************************
*@fuction	:Buzzer_OFF();
*@brief		:Buzzer关
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-28
***********************************************************/

void Buzzer_OFF(void)
{
	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);
}
/***********************************************************
*@fuction	:Buzzer_Turn();
*@brief		:Buzzer翻转
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2022-12-28
***********************************************************/

void Buzzer_Turn(void)	//电平反翻转
{
	HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
}


