#ifndef __IC_H__
#define __IC_H__
#include "main.h"
void IC_Init(void);
void GetICValue(uint8_t Channel, float *duty, uint32_t *freq);
#endif

