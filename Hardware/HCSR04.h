#ifndef __HCSR04_H
#define __HCSR04_H

extern uint64_t time;

void HC_SR04_Init(void);
int16_t GetDistance(void);
float ScanDistance_mm(void);//���ص�λΪ���׵ľ�����
float ScanDistance_cm(void);//���ص�λΪ���׵ľ�����
float ScanDistance_m(void);//���ص�λΪ�׵ľ�����

#endif
