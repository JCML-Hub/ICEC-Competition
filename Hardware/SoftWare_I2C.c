#include "SoftWare_I2C.h"

void I2C_Delay(void)//软件定时2us
{
	uint32_t Delay = 1 * 168/4;
	do{__NOP();}
	while (Delay --);
}
void I2C_W_SCL(uint8_t x)
{//SCL写入
	if(x==0){HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET);}
	else{HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);}
}
void I2C_W_SDA(uint8_t x)
{//SDA写入
	if(x==0){HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET);}
	else{HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET);}
}
uint8_t I2C_R_SCL(void)
{//读取SCL的电平
	return HAL_GPIO_ReadPin(I2C_SCL_GPIO_Port, I2C_SCL_Pin);
}
uint8_t I2C_R_SDA(void)
{//读取SCL的电平
	return HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin);
}
//内部使用，用户无需调用
void I2C_Start(void)
{
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	I2C_Delay();
	I2C_W_SDA(0);
	I2C_Delay();
	I2C_W_SCL(0);
	I2C_Delay();
}

//内部使用，用户无需调用
void I2C_Stop(void)
{
	I2C_W_SDA(0);
	I2C_W_SCL(0);
	I2C_Delay();
	I2C_W_SCL(1);
	I2C_Delay();
	I2C_W_SDA(1);
	I2C_Delay();
}

unsigned char I2C_Wait_Ack(void)
{
	unsigned char ucErrTime=0;

	I2C_W_SDA(1); //准备、待拉低
	I2C_W_SCL(1);  

	while(I2C_R_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
//			printf("err");
			return 1;
		}
	}
	I2C_W_SCL(0);
	I2C_Delay();
	return 0;
}

void Write_I2C_Byte(unsigned char I2C_Byte)
{
	unsigned char i = 8;
	while(i--)		
	{
		if(I2C_Byte & 0x80){I2C_W_SDA(1);}
		else{I2C_W_SDA(0);}
		I2C_Byte <<= 1;		//移往下一位
		I2C_Delay();		
		I2C_W_SCL(1); //开始传输
		I2C_Delay();
		I2C_W_SCL(0); //传输完毕
	}
}

unsigned char I2C_Read_Byte(void)
{
	unsigned char receive=0;
	int i;
	
	I2C_W_SCL(0);
	for(i=0;i<8;i++)
	{
		I2C_W_SDA(1);
		I2C_Delay();
		I2C_W_SCL(1);//开始读一位、
		I2C_Delay();
		receive <<= 1;
		if(I2C_R_SDA())	receive++;  //DATA为高、新位加1 ，否则为0继续下移
		I2C_W_SCL(0);
		I2C_Delay();
   }					 
    return receive;
}

void I2C_SendACK(void)
{
	I2C_W_SDA(0);
	I2C_Delay();
    I2C_W_SCL(1);
	I2C_Delay();
    I2C_W_SCL(0);
	I2C_Delay();
	I2C_W_SDA(1);
	I2C_Delay();
}

void I2C_NoACK(void)
{
	I2C_W_SDA(1);
	I2C_Delay();
    I2C_W_SCL(1);
	I2C_Delay();
    I2C_W_SCL(0);
	I2C_Delay();
	I2C_W_SDA(0);
	I2C_Delay();
}



