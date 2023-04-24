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
  * Description: ���벶�� ��ͨ��
******************************************************************************
 */

#include "IC.h"
#include "main.h"
#include "tim.h"
#include "HCSR04.h"//�жϺ������ͻ

/* USER CODE BEGIN PV */

uint16_t PRE1=1700;
uint16_t CMP1=500;
uint32_t capture_Buf[4] = {0};   //��ż���
uint32_t capture_Buf1[4] = {0};   //��ż���
uint8_t capture_Cnt = 0;    //״��־λ
float duty_cycle;  //ռ��
uint32_t frequency;  //Ƶ��

/* USER CODE END PV */

/***********************************************************
*@���� 	:IC_Init
*@����	:��ʼ��
*@����	:��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-09
***********************************************************/

void IC_Init(void)
{
//  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); //����Ƶ�ʲ��������ز���
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2); //����Ƶ�ʲ����½��ز���
  
}

/***********************************************************
*@���� 	:GetICValue
*@����	:��ȡƵ��ռ�ձ�
*@����	:Channelͨ�� duty ָ��ռ�ձȵ�ָ�� freq Ƶ��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-09
***********************************************************/

void GetICValue(uint8_t Channel, float *duty, uint32_t *freq)
{
	if (Channel==1){*duty=duty_cycle;*freq=frequency/1000;}//����λ����KHz
	else if (Channel==2){/**duty=Duty1;*freq=Freq1;*/}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	// ��ȡƵ��
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
				    	 HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); //����Ƶ�ʲ���
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




//uint8_t TIM5_CH1_Edge=0;  //״̬�仯ʱ������ֵ
//uint32_t TIM5_CH1_VAL=0;  //����������ļ�¼ֵ
//uint32_t TIM5_CH1_OVER=0; //����������ĸ���
//float Time;   //�ߵ�ƽ����ʱ��


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
//				TIM5_CH1_Edge = 0;  //���¿�ʼ����
//				Time = TIM5_CH1_VAL + TIM5_CH1_OVER*0xffff;
//				*duty=Time;
//				HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);  //�����벶��
//			}
//}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim == &htim2)
//	{
//		if(TIM5_CH1_Edge == 1)
//		{
//			TIM5_CH1_OVER++;  //��ʱ�����ֵ����
//		}
//	}
//	if (htim == &htim3)		//��ȡTIM3��ʱ���ĸ����жϱ�־λ
//	{
//		time++;
//	}
//}
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim == &htim2)
//	{
//		if(TIM5_CH1_Edge == 0) //����������
//		{
//			TIM5_CH1_Edge++;  //���벶���½���״̬
//			TIM5_CH1_OVER = 0;  //��ʱ�����ֵ����
//			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING); //���ò�����Ϊ�½���
//			__HAL_TIM_SET_COUNTER(&htim2,0);  //���ö�ʱ��CNT��������ֵΪ0
//		}
//		else //����������
//		{
//			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_1); //�رն�ʱ��5
//			TIM5_CH1_Edge++;  //���뵽������״̬
//			TIM5_CH1_VAL = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1); //��ȡ����ͨ����ֵ
//			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING); //���ò�����Ϊ�Ͻ���
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
//	/*��ȡ�Ĵ�����ֵ*/
//		Cap_val1 = HAL_TIM_ReadCapturedValue (&htim2 ,TIM_CHANNEL_1 );
//		/*����ռ�ձȣ�Ƶ��*/
//		if(Cap_val1 != 0)
//		{
//			Duty0 = (float)(Cap_val1+ 1)*100 / (Cap_val1 +1) ;
//			Freq0 = 72000000 / 72 / (float )(Cap_val1 +1) ;
//		}
//	}
//	else if(htim ->Channel == HAL_TIM_ACTIVE_CHANNEL_2 )
//	{
//	/*��ȡ�Ĵ�����ֵ*/
//		Cap_val2 = HAL_TIM_ReadCapturedValue (&htim2 ,TIM_CHANNEL_2 );
//		/*����ռ�ձȣ�Ƶ��*/
//		if(Cap_val2 != 0)
//		{
//			Duty1 = (float)(Cap_val2+ 1)*100 / (Cap_val2 +1) ;
//			Freq1 = 72000000 / 72 / (float )(Cap_val2 +1) ;
//		}
//	}
//}



