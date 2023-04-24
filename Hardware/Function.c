/**
  ************************************* Copyright ****************************** 
  *

  *                 (C) Copyright 2023,--,China, CUIT.
  *                            All Rights Reserved
  *                     By(JCML)
  * FileName   : Function.c   
  * Version    : v2.0		
  * Author     : JCML			
  * Date       : 2023-03-07         
  * Description: ��Э������B�� 
											���⣺���趨�����¶�ֵʱ�޷���b����ʾ��С�����֣�
******************************************************************************
 */

#include "main.h"
#include "SSD1306_OLED.h"
#include "HCSR04.h"
#include "Encoder.h"
#include "DS1302.h"
#include "DS18B20.h"
#include "Function.h"
#include "EEPROM.h"
#include "Delay.h"
#include "LED.h"
#include "Serial.h"
#include "Delay.h"
#include "IC.h"

//��0��5�ֱ�Ϊ�� �� �� ʱ �� ��
uint8_t SetTimeData[6]={0};
uint8_t AlarmTime[6]={0};
char TimeInitData1[11]={0};
char TimeInitData2[9]={0};
char ArlarmInitData[9]={0};
uint8_t AlarmFlag=1;
uint8_t ReadFlag=0;
uint8_t WriteFlag=0;
uint8_t RxFlag=0;
float Tempreture;
float WarningTemp=30;
float Duty;
uint32_t Freq;
uint8_t ICFlag=1;

/***********************************************************
*@���� 	:TimeShow
*@����	:���ڷ����������Ϸ���ʱ����ʾ
*@����	:��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void TimeShow(void)
{
	DS1302_Time_Convert();
	OLED_ShowString(0, 0, DS1302_DATA_1, 6);
	OLED_ShowString(88, 0, DS1302_DATA_2, 6);
}

/***********************************************************
*@���� 	:EnCoder_Count
*@����	:������ת�������Ĳ���ֵ
*@����	:*Numָ����ת��������ֵ��ָ�룬Step����ֵ
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void EnCoder_Count(int16_t *Num, uint8_t Step)//��ת�������ļ���
{
	switch(Step)
	{
		case 0: *Num=Encode_Get();break;
		case 1: *Num=Encode_Get()*2;break;
		case 2: *Num=Encode_Get()*5;break;
		default:break;
	}
}

/***********************************************************
*@���� 	:TimeLimite
*@����	:�޶����õ�ʱ��ֵ
*@����	:TimeData���ʱ������飬��СΪ6
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void TimeLimite(uint8_t *TimeData)//�˴���ʱ��������ƣ���ֻ�Դ��ڲ��ָ�λΪ0��С��0�����ݲ�������
{
	if(TimeData[0]>99){TimeData[0]=0;}//��Խ���ж�
	if(TimeData[1]>12){TimeData[1]=1;}//��Խ���ж�
	if( TimeData[1]==1 || TimeData[1]==3 || TimeData[1]==5 || TimeData[1]==7 || 
		TimeData[1]==8 || TimeData[1]==10 || TimeData[1]==12)//��Խ���ж�
	{
		if(TimeData[2]>31){TimeData[2]=1;}//����
	}
	else if(TimeData[1]==4 || TimeData[1]==6 || TimeData[1]==9 || TimeData[1]==11)
	{
		if(TimeData[2]>30){TimeData[2]=1;}//С��
	}
	else if(TimeData[1]==2)
	{
		if(TimeData[0]%4==0)
		{
			if(TimeData[2]>29){TimeData[2]=1;}//����2��
		}
		else
		{
			if(TimeData[2]>28){TimeData[2]=1;}//ƽ��2��
		}
	}
	if(TimeData[3]>23){TimeData[3]=0;}//ʱԽ���ж�
	if(TimeData[4]>59){TimeData[4]=0;}//��Խ���ж�
	if(TimeData[5]>59){TimeData[5]=0;}//��Խ���ж�
}

/***********************************************************
*@���� 	:Turn_Show
*@����	:��Page2ʱ�л��趨ֵʱ�ı仯���������ʾ��ǽ������趨��ֵ��λ��ȡ��
*@����	:��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void Turn_Show(uint8_t *pos)
{
	switch (*pos)//��ʾ��Ӧ�޸Ĺ���ֵ
	{
		case 0:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 15);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 16);break;
		case 1:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 15);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 16);break;
		case 2:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 15);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 16);break;
		case 3:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 15);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 16);break;
		case 4:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 15);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 16);break;
		case 5:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 15);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 16);break;
		case 6:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 15);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 16);break;
		case 7:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 15);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 16);break;
		case 8:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 15);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 16);break;
		case 9:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 15);}
			else{OLED_ShowString(108, 6, "ON", 15);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 16);break;
		case 10:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 16);
			OLED_ShowString(108, 4, "OK", 15);break;
		case 11:
			OLED_ShowNum(24, 2, SetTimeData[0]+2000, 4, 16);
			OLED_ShowNum(64, 2, SetTimeData[1], 2, 16);
			OLED_ShowNum(88, 2, SetTimeData[2], 2, 16);
			OLED_ShowNum(32, 4, SetTimeData[3], 2, 16);
			OLED_ShowNum(56, 4, SetTimeData[4], 2, 16);
			OLED_ShowNum(80, 4, SetTimeData[5], 2, 16);
			OLED_ShowNum(40, 6, AlarmTime[3], 2, 16);
			OLED_ShowNum(64, 6, AlarmTime[4], 2, 16);
			OLED_ShowNum(86, 6, AlarmTime[5], 2, 16);
			if (AlarmFlag==1){OLED_ShowString(108, 6, "OF", 16);}
			else{OLED_ShowString(108, 6, "ON", 16);}
			OLED_ShowString(108, 2, "SR", 15);
			OLED_ShowString(108, 4, "OK", 16);break;
	}
}

/***********************************************************
*@���� 	:TimeSet
*@����	:�趨ʱ��
*@����	:counterָ����ת��������ָ�룬PosĿ���趨��λ��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void TimeSet(int16_t *counter, uint8_t Pos)
{
	SetTimeData[Pos] += *counter;//�趨λ�ü�����ת��������ֵ
	TimeLimite(SetTimeData);//���Ƹı�ֵ
	Turn_Show(&Pos);//��ʾĿ���趨ֵ�ĸı����
}

/***********************************************************
*@���� 	:AlarmSet
*@����	:�趨����
*@����	:��TimeSetһ��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void AlarmSet(int16_t *counter, uint8_t Pos)
{
	
	AlarmTime[Pos-3] += *counter;
	TimeLimite(AlarmTime);
	Turn_Show(&Pos);
}
/***********************************************************
*@���� 	:ConfirmData
*@����	:ȷ���趨ֵ
*@����	:��TimeSetһ��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void ConfirmData(uint8_t *counter, uint8_t Pos)
{
	if (Pos==9 && *counter==2)
	{
		AlarmFlag++;
		AlarmFlag%=2;//�л�����״̬
	}
	else if (Pos==11&&*counter==2&&RxFlag==1)//����������
	{
			RxFlag=0;//��־λ��λ
			EEPROM_WriteBytes(RxTime, 0, 6);
			Delay_ms(200);
			EEPROM_ReadBytes(SetTimeData, 0, 6);//������ʾ
			OLED_ShowString(0, 2, "  ", 16);//�����ʾ
	}
	else if (Pos==10&&*counter==2)
	{
			WriteFlag=1;
			EEPROM_WriteBytes(SetTimeData, 0, 6);
			Delay_ms(200);
			EEPROM_WriteBytes(AlarmTime, 8, 6);
			Myprintf("EEPROM �����趨ʱ�䣺20%02d��%02d��%02d��%02dʱ%02d��%02d��\r\n", SetTimeData[0], SetTimeData[1], SetTimeData[2], 
																					SetTimeData[3], SetTimeData[4], SetTimeData[5]);
			Delay_ms(100);//��Ҫ��һ��
			Myprintf("EEPROM ��������ʱ�䣺20%02d��%02d��%02d��%02dʱ%02d��%02d��\r\n", AlarmTime[0], AlarmTime[1], AlarmTime[2], 
																					AlarmTime[3], AlarmTime[4], AlarmTime[5]);
			DS1302_Set(SetTimeData);
			OLED_ShowString(0, 4, "OK", 16);
			Delay_ms(500);
			OLED_ShowString(0, 4, "  ", 16);
	}
	Turn_Show(&Pos);
}
/***********************************************************
*@���� 	:WarningState
*@����	:������¶ȵĵ�ʱ����
*@����	:��
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void WarningState(void)
{
	DS1302_Time_Trans();
	if (AlarmFlag==0&&DS1302_time[5]==AlarmTime[5]&&DS1302_time[4]==AlarmTime[4]&&DS1302_time[3]==AlarmTime[3])
	{
		Myprintf("���嵽ʱ���ѣ�\r\n");
		Buzzer_ON();
		Delay_s(3);
		Buzzer_OFF();
	}
	if (WarningTemp < Tempreture)
	{
		Myprintf("�¶ȵ��ﾯ��ֵ��\r\n");
		LED1_Turn();
		Delay_s(1);
		LED1_Turn();
		Delay_s(1);
		LED1_Turn();
		Delay_s(1);
		LED1_Turn();
	}
}



/***********************************************************
*@���� 	:Page2_TimeInit
*@����	:Page2��ʱ������ʼ��
*@����	:timeҪ��ʾ��ʱ������
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void Page2_TimeInit(uint8_t time[6])//��Ҫ�趨��ʱ���ʼ����ʾ��ʱ��ת�浽SetTimeData����ʾ
{
	DS1302_Time_Trans();
	TimeInitData1[0]='2';
	TimeInitData1[1]='0';
	TimeInitData1[2]='0'+time[5]/10;
	TimeInitData1[3]='0'+time[5]%10;
	TimeInitData1[4]='-';
	TimeInitData1[5]='0'+time[4]/10;
	TimeInitData1[6]='0'+time[4]%10;
	TimeInitData1[7]='-';
	TimeInitData1[8]='0'+time[3]/10;
	TimeInitData1[9]='0'+time[3]%10;
	TimeInitData1[10]='\0';
	
	TimeInitData2[0]='0'+time[2]/10;
	TimeInitData2[1]='0'+time[2]%10;
	TimeInitData2[2]=':';
	TimeInitData2[3]='0'+time[1]/10;
	TimeInitData2[4]='0'+time[1]%10;
	TimeInitData2[5]=':';
	TimeInitData2[6]='0'+time[0]/10;
	TimeInitData2[7]='0'+time[0]%10;
	TimeInitData2[8]='\0';
	OLED_ShowString(24, 2, TimeInitData1, 16);
	OLED_ShowString(32, 4, TimeInitData2, 16);
}

/***********************************************************
*@���� 	:Page2_AlarmInit
*@����	:Page2����������ʼ��
*@����	:ͬǰ
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void Page2_AlarmInit(uint8_t time[6])
{
	DS1302_Time_Trans();
	ArlarmInitData[0]='0'+time[3]/10;
	ArlarmInitData[1]='0'+time[3]%10;
	ArlarmInitData[2]=':';
	ArlarmInitData[3]='0'+time[4]/10;
	ArlarmInitData[4]='0'+time[4]%10;
	ArlarmInitData[5]=':';
	ArlarmInitData[6]='0'+time[5]/10;
	ArlarmInitData[7]='0'+time[5]%10;
	ArlarmInitData[8]='\0';
	OLED_ShowString(40, 6, ArlarmInitData, 16);
}


/***********************************************************
*@���� 	:SetTemp
*@����	:�趨Ԥ���¶�ֵ
*@����	:counter��ת������
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void SetTemp(int16_t *counter)//�趨�����¶�
{
	WarningTemp += (float)(*counter)*0.1;
//	Myprintf("�����¶��趨ֵ��%02.1f\r\n", WarningTemp);
	OLED_ShowFloatNum(64, 6,WarningTemp, 22, 16);
}


/***********************************************************
*@���� 	:ChangeIC_Channel
*@����	:�л�ICͨ��
*@����	:Key���������л�
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-08
***********************************************************/

void ChangeIC_Channel(uint8_t *Key)
{
	static uint8_t State=0;
	if (*Key == 2)
	{
		State++;
		State%=2;		
	}
	if (State==0){GetICValue(1, &Duty, &Freq);ICFlag=1;}
	else{GetICValue(2, &Duty, &Freq);ICFlag=2;}
}
/***********************************************************
*@���� 	:KeepShow
*@����	:��̬��ʾ
*@����	:Page_Flag��ʾҳ
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void KeepShow(uint8_t *Page_Flag)
{
	switch (*Page_Flag)
	{
		case 1:
			OLED_Clear();
			OLED_ShowChinese(36, 0, "������", 32);
			OLED_ShowChinese(40, 2, "��", 16);
			OLED_ShowChinese(72, 2, "��", 16);
			OLED_ShowChinese(104, 2, "��", 16);
			OLED_ShowChinese(29, 4, "ʱ", 16);
			OLED_ShowChinese(61, 4, "��", 16);
			OLED_ShowChinese(95, 4, "��", 16);
			OLED_ShowChinese(42, 6, "��", 2);
			OLED_ShowChinese(102, 6, "�H", 2);break;
		case 2:
			OLED_Clear();
			EEPROM_ReadBytes(SetTimeData, 0, 6);
			Delay_ms(100);
			EEPROM_ReadBytes(AlarmTime, 8, 6);
			Myprintf("EEPROM ��ȡ�趨ʱ�䣺20%02d��%02d��%02d��%02dʱ%02d��%02d��\r\n", SetTimeData[0], SetTimeData[1], SetTimeData[2], 
																					SetTimeData[3], SetTimeData[4], SetTimeData[5]);
			Delay_ms(200);//��Ҫ��һ��
			Myprintf("EEPROM ��ȡ����ʱ�䣺%02dʱ%02d��%02d��\r\n",AlarmTime[3], AlarmTime[4], AlarmTime[5]);
			OLED_ShowChinese(48, 0, "��ʱ", 32);
			
			Page2_TimeInit(SetTimeData);
			OLED_ShowChinese(0, 6, "����",2);
			OLED_ShowString(30, 6, ":", 16);
			OLED_ShowString(56, 6, ":", 16);
			OLED_ShowString(80, 6, ":", 16);
			Page2_AlarmInit(AlarmTime);break;
		case 3:
			OLED_Clear();
			OLED_ShowChinese(48, 0, "���", 32);
			OLED_ShowChinese(0, 2, "��λ", 2);OLED_ShowChinese(0, 4, "��λ", 2);
			OLED_ShowString(40, 2, ":", 16);OLED_ShowString(40, 4, ":", 16);
			OLED_ShowString(110, 2, "CM", 16);
			OLED_ShowString(110, 4, "M", 16);
			OLED_ShowString(40, 6, ">USART<", 16);break;
		case 4:
			OLED_Clear();
			OLED_ShowChinese(48, 0, "����", 32);
			OLED_ShowString(32, 2, "Duty", 16);OLED_ShowString(92, 2, "Freq", 16);
			OLED_ShowString(56, 5, "%", 6);OLED_ShowString(104, 5, "KHz", 6);
			OLED_ShowString(0, 6, "TempSet:", 16);break;
		default:
			break;
	}
	*Page_Flag = 0;
}

/***********************************************************
*@���� 	:ChangeShow
*@����	:��̬��ʾ
*@����	:ͬǰ
*@����ֵ	:��
*@����	:JCML
*@����	:2023-03-07
***********************************************************/

void ChangeShow(uint8_t *ShowFlag)
{
	float temp1=0, temp2=0;
	DS1302_Read();//DS1302��ȡʱ��
	
	switch (*ShowFlag)
	{
		case 1:
			OLED_ShowNum(8, 2, DS1302_Time[5]+2000, 4, 16);
			OLED_ShowNum(56, 2, DS1302_Time[4], 2, 16);
			OLED_ShowNum(88, 2, DS1302_Time[3], 2, 16);
			
			OLED_ShowNum(13, 4, DS1302_Time[2], 2, 16);
			OLED_ShowNum(45, 4, DS1302_Time[1], 2, 16);
			OLED_ShowNum(77, 4, DS1302_Time[0], 2, 16);
			//��ȡ����ʾ�¶�
			Tempreture=DS18B20_GetTemp_SkipRom();
			OLED_ShowFloatNum(0, 6, Tempreture, 21, 16);
			OLED_ShowFloatNum(62, 6, Tempreture*1.8+32, 21, 16);
			break;
		case 2:
//			TimeSet(counter, Pos);
			if (GetRxFlag())
			{
				RxFlag=1;
				StringTrans();
				OLED_ShowString(0, 2, "OK", 16);//��ʾ�Ѿ��յ�����
				Myprintf("���ڽ����趨ʱ�䣺20%02d��%02d��%02d��%02dʱ%02d��%02d��\r\n", RxTime[0], RxTime[1], RxTime[2], 
																					RxTime[3], RxTime[4], RxTime[5]);
			}
			TimeShow();
			break;
		case 3:
			temp1=ScanDistance_cm();
			temp2=ScanDistance_m();
			TimeShow();
			OLED_ShowNum(32, 2, 1, 1, 16);OLED_ShowNum(32, 4, 2, 1, 16);
			OLED_ShowFloatNum(40, 2, temp1, 33, 16);
			OLED_ShowFloatNum(40, 4, temp2, 33, 16);
			Myprintf("����ֵ1:%.6fCM, ����ֵ2:%.6fM\r\n", temp1, temp2);
			break;
		case 4:
			TimeShow();
			OLED_ShowNum(32, 4, Duty, 3, 16);
			OLED_ShowNum(80, 4, Freq, 3, 16);
			if (ICFlag==1){OLED_ShowString(0, 4, "CH1:", 16);}
			else{OLED_ShowString(0, 4, "CH2:", 16);}
			Myprintf("���벶��ռ�ձ�:%02.1f%%, ���벶��Ƶ��ֵ:%8dKHz\r\n", Duty, Freq);//
			break;
		default:
			break;
	}
}

