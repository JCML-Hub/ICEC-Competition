#ifndef __DS1302_H__
#define __DS1302_H__
#include "main.h"

#define DS1302_SECOND		0x81
#define DS1302_MINUTE		0x83
#define DS1302_HOUR			0x85
#define DS1302_DATE			0x87
#define DS1302_MONTH		0x89
#define DS1302_DAY			0x8B	//星期
#define DS1302_YEAR			0x8D
#define DS1302_WP			0x8E	//写保护	

extern uint8_t DS1302_Time[8];//存原始数据
extern uint8_t DS1302_time[];

extern char DS1302_DATA_1[];
extern char DS1302_DATA_2[];

void ds1302_DATAOUT_init(void);
void DS1302_W_Byte(uint8_t data);
void DS1302_W_Cmd(uint8_t address,uint8_t data);
uint8_t DS1302_R_Time(uint8_t address);
void DS1302_Set(uint8_t SetTime[]);
void DS1302_Read(void);
void DS1302_Time_Convert(void);
void DS1302_Time_Trans(void);
#endif




