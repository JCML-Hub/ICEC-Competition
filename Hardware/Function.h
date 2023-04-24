#ifndef __FUNCTION_H__
#define __FUNCTION_H__

extern uint8_t AlarmFlag;

void EnCoder_Count(int16_t *Num, uint8_t Step);
void AlarmSet(int16_t *counter, uint8_t Pos);

void ChangeShow(uint8_t *ShowFlag);
void TimeSet(int16_t *counter, uint8_t Pos);
void KeepShow(uint8_t *Page_Flag);
void WarningState(void);//此处需要赋给结构体中的AlarmFlag的地址
void ConfirmData(uint8_t *counter, uint8_t Pos);
void Turn_Show(uint8_t *pos);
void SetTemp(int16_t *counter);//设定报警温度
void ChangeIC_Channel(uint8_t *Key);
#endif





