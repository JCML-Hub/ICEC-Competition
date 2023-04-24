#ifndef __EEPROM_H_
#define	__EEPROM_H_


#include "main.h"
#include "Delay.h"

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GPIO_x	  ROM_SDA_GPIO_Port			/* GPIO�˿� */
#define SCL_PIN		ROM_SCL_Pin			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define SDA_PIN		ROM_SDA_Pin			/* ���ӵ�SDA�����ߵ�GPIO */

#define SCL_H     HAL_GPIO_WritePin(ROM_SCL_GPIO_Port, SCL_PIN, GPIO_PIN_SET)		/* SCL = 1 */
#define SCL_L     HAL_GPIO_WritePin(ROM_SCL_GPIO_Port, SCL_PIN, GPIO_PIN_RESET)		/* SCL = 0 */
	
#define SDA_H     HAL_GPIO_WritePin(ROM_SDA_GPIO_Port, SDA_PIN, GPIO_PIN_SET)		/* SDA = 1 */
#define SDA_L     HAL_GPIO_WritePin(ROM_SDA_GPIO_Port, SDA_PIN, GPIO_PIN_RESET)		/* SDA = 0 */
#define SDA_READ  HAL_GPIO_ReadPin(ROM_SDA_GPIO_Port, SDA_PIN)	/* ��SDA����״̬ */


	
#define EEPROM_DEV_ADDR			0xA0		//EEPROM���豸��ַ 
#define EEPROM_PAGE_SIZE		 8			  /* 24xx02��ҳ���С */
#define EEPROM_SIZE				   256			  /* 24xx02������ */


uint8_t EEPROM_Check(void);
uint8_t EEPROM_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t EEPROM_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t EEPROM_Erase(void);

//���I2C�ײ�
void I2C_Start_(void);
void I2C_Stop_(void);
void I2C_SendByte(uint8_t _ucByte);
uint8_t I2C_ReadByte(void);
uint8_t I2C_WaitAck(void);
void I2C_Ack(void);
void I2C_NAck(void);
uint8_t I2C_CheckDevice(uint8_t _Address);

#endif
