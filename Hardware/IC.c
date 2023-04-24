/**
  ************************************* Copyright ****************************** 
  *

  *                 (C) Copyright 2023,--,China, CUIT.
  *                            All Rights Reserved
  *                     By(JCML)
  * FileName   : IC.c   
  * Version    : v1.0		
  * Author     : JCML			
  * Date       : 2023-03-09         
  * Description: 输入捕获 单通道
******************************************************************************
 */

#include "IC.h"
#include "main.h"
#include "tim.h"
#include "HCSR04.h"//中断函数冲冲突

/* USER CODE BEGIN PV */

uint16_t PRE1=1700;
uint16_t CMP1=500;
uint32_t capture_Buf[4] = {0};   //存放计数
uint32_t capture_Buf1[4] = {0};   //存放计数
uint8_t capture_Cnt = 0;    //状标志位
float duty_cycle;  //占空
uint32_t frequency;  //频率

/* USER CODE END PV */

/***********************************************************
*@名称 	:IC_Init
*@描述	:初始化
*@参数	:无
*@返回值	:无
*@作者	:JCML
*@日期	:2023-03-09
***********************************************************/

void IC_Init(void)
{
//  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); //启动频率捕获（上升沿捕获）
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2); //启动频率捕获（下降沿捕获）
  
}

/***********************************************************
*@名称 	:GetICValue
*@描述	:获取频率占空比
*@参数	:Channel通道 duty 指向占空比的指针 freq 频率
*@返回值	:无
*@作者	:JCML
*@日期	:2023-03-09
***********************************************************/

void GetICValue(uint8_t Channel, float *duty, uint32_t *freq)
{
	if (Channel==1){*duty=duty_cycle;*freq=frequency/1000;}//将单位换成KHz
	else if (Channel==2){/**duty=Duty1;*freq=Freq1;*/}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	// 获取频率
				if(TIM2==htim->Instance)
				{
				 if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 )
				  {
				    if(capture_Cnt == 0)
				    {
				      /* Get the 1st Input Capture value */

				    	 assert_param(IS_TIM_CC1_INSTANCE(htim2->Instance));
				    	 capture_Buf[0] =  htim->Instance->CCR1;
				    	 capture_Buf1[0] =  htim->Instance->CCR2;
				    	 capture_Cnt = 1;
				    }
				    else if(capture_Cnt == 1)
				    {
				      /* Get the 2nd Input Capture value */
				    	// capture_Buf[2] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
				    	 assert_param(IS_TIM_CC1_INSTANCE(htim2->Instance));
				    	 capture_Buf[2] =  htim->Instance->CCR1;
				    	 capture_Buf1[2] =  htim->Instance->CCR2;
				    	 HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); //启动频率捕获
				      /* Capture computation */
				      if (capture_Buf[2] > capture_Buf[0])
				      {
				        capture_Buf[3] = (capture_Buf[2] - capture_Buf[0]);
				        capture_Buf1[3] = (capture_Buf1[2] - capture_Buf[0]);
				      }
				      else if (capture_Buf[2] < capture_Buf[0])
				      {
				        /* 0xFFFF is max TIM1_CCRx value */
				        capture_Buf[3] = ((0xFFFF - capture_Buf[0]) + capture_Buf[1]) + 1;
				        capture_Buf1[3] = ((0xFFFF - capture_Buf[0]) + capture_Buf1[1]) + 1;
				      }
				      else
				      {
				        /* If capture values are equal, we have reached the limit of frequency
				           measures */
				        Error_Handler();
				      }

				      /* Frequency computation: for this example TIMx (TIM1) is clocked by
				         APB2Clk */
				      frequency = HAL_RCC_GetPCLK2Freq() / capture_Buf[3];
				      duty_cycle=capture_Buf1[3]*100/capture_Buf[3];
				      capture_Cnt = 0;
				    }
				  }

				}
}

/* USER CODE END 4 */




//uint8_t TIM5_CH1_Edge=0;  //状态变化时，计数值
//uint32_t TIM5_CH1_VAL=0;  //储存计数器的记录值
//uint32_t TIM5_CH1_OVER=0; //计数器溢出的个数
//float Time;   //高电平持续时间


////uint32_t Cap_val1, Cap_val2;
////float Duty0, Freq0,Duty1, Freq1;

//void IC_Init(void)
//{
//	HAL_TIM_PWM_Start 	(&htim1  ,TIM_CHANNEL_1 );
//	HAL_TIM_Base_Start_IT(&htim2);
//	HAL_TIM_IC_Start_IT (&htim2,TIM_CHANNEL_1);
//	HAL_TIM_IC_Start_IT (&htim2,TIM_CHANNEL_2);
//}

//void GetICValue(uint8_t Channel, float *duty, float *freq)
//{
//	if(TIM5_CH1_Edge == 2)
//			{
//				TIM5_CH1_Edge = 0;  //重新开始捕获
//				Time = TIM5_CH1_VAL + TIM5_CH1_OVER*0xffff;
//				*duty=Time;
//				HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);  //打开输入捕获
//			}
//}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim == &htim2)
//	{
//		if(TIM5_CH1_Edge == 1)
//		{
//			TIM5_CH1_OVER++;  //定时器溢出值增加
//		}
//	}
//	if (htim == &htim3)		//获取TIM3定时器的更新中断标志位
//	{
//		time++;
//	}
//}
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim == &htim2)
//	{
//		if(TIM5_CH1_Edge == 0) //捕获到上升沿
//		{
//			TIM5_CH1_Edge++;  //进入捕获下降沿状态
//			TIM5_CH1_OVER = 0;  //定时器溢出值清零
//			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING); //设置捕获极性为下降沿
//			__HAL_TIM_SET_COUNTER(&htim2,0);  //设置定时器CNT计数器的值为0
//		}
//		else //捕获到下升沿
//		{
//			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_1); //关闭定时器5
//			TIM5_CH1_Edge++;  //进入到主函数状态
//			TIM5_CH1_VAL = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1); //读取捕获通道的值
//			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING); //设置捕获极性为上降沿
//		}
//	}
//}


//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	
//}

//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim ->Channel == HAL_TIM_ACTIVE_CHANNEL_1 )
//	{
//	/*读取寄存器的值*/
//		Cap_val1 = HAL_TIM_ReadCapturedValue (&htim2 ,TIM_CHANNEL_1 );
//		/*计算占空比，频率*/
//		if(Cap_val1 != 0)
//		{
//			Duty0 = (float)(Cap_val1+ 1)*100 / (Cap_val1 +1) ;
//			Freq0 = 72000000 / 72 / (float )(Cap_val1 +1) ;
//		}
//	}
//	else if(htim ->Channel == HAL_TIM_ACTIVE_CHANNEL_2 )
//	{
//	/*读取寄存器的值*/
//		Cap_val2 = HAL_TIM_ReadCapturedValue (&htim2 ,TIM_CHANNEL_2 );
//		/*计算占空比，频率*/
//		if(Cap_val2 != 0)
//		{
//			Duty1 = (float)(Cap_val2+ 1)*100 / (Cap_val2 +1) ;
//			Freq1 = 72000000 / 72 / (float )(Cap_val2 +1) ;
//		}
//	}
//}



