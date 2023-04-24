#ifndef __EEPROM_H_
#define	__EEPROM_H_


#include "main.h"
#include "Delay.h"

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_x	  ROM_SDA_GPIO_Port			/* GPIO端口 */
#define SCL_PIN		ROM_SCL_Pin			/* 连接到SCL时钟线的GPIO */
#define SDA_PIN		ROM_SDA_Pin			/* 连接到SDA数据线的GPIO */

#define SCL_H     HAL_GPIO_WritePin(ROM_SCL_GPIO_Port, SCL_PIN, GPIO_PIN_SET)		/* SCL = 1 */
#define SCL_L     HAL_GPIO_WritePin(ROM_SCL_GPIO_Port, SCL_PIN, GPIO_PIN_RESET)		/* SCL = 0 */
	
#define SDA_H     HAL_GPIO_WritePin(ROM_SDA_GPIO_Port, SDA_PIN, GPIO_PIN_SET)		/* SDA = 1 */
#define SDA_L     HAL_GPIO_WritePin(ROM_SDA_GPIO_Port, SDA_PIN, GPIO_PIN_RESET)		/* SDA = 0 */
#define SDA_READ  HAL_GPIO_ReadPin(ROM_SDA_GPIO_Port, SDA_PIN)	/* 读SDA口线状态 */


	
#define EEPROM_DEV_ADDR			0xA0		//EEPROM的设备地址 
#define EEPROM_PAGE_SIZE		 8			  /* 24xx02的页面大小 */
#define EEPROM_SIZE				   256			  /* 24xx02总容量 */


uint8_t EEPROM_Check(void);
uint8_t EEPROM_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t EEPROM_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t EEPROM_Erase(void);

//软件I2C底层
void I2C_Start_(void);
void I2C_Stop_(void);
void I2C_SendByte(uint8_t _ucByte);
uint8_t I2C_ReadByte(void);
uint8_t I2C_WaitAck(void);
void I2C_Ack(void);
void I2C_NAck(void);
uint8_t I2C_CheckDevice(uint8_t _Address);

#endif
