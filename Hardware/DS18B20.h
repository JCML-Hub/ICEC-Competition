

#ifndef __DS18B20_H__
#define __DS18B20_H__


#include "main.h"


#define BSP_DS18B20_PORT             DS18B20_GPIO_Port
#define BSP_DS18B20_PIN              DS18B20_Pin
 
#define DS18B20_OUT_1                HAL_GPIO_WritePin(BSP_DS18B20_PORT, BSP_DS18B20_PIN, GPIO_PIN_SET)
#define DS18B20_OUT_0                HAL_GPIO_WritePin(BSP_DS18B20_PORT, BSP_DS18B20_PIN, GPIO_PIN_RESET)
 
#define DS18B20_IN			        HAL_GPIO_ReadPin(BSP_DS18B20_PORT, BSP_DS18B20_PIN)
 
uint8_t DS18B20_Init(void);
void DS18B20_ReadId(uint8_t *ds18b20_id);
float DS18B20_GetTemp_SkipRom(void);
float DS18B20_GetTemp_MatchRom(uint8_t * ds18b20_id);
 
#endif
