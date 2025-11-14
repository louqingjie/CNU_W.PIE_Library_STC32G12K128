#ifndef _ENCODER_H
#define _ENCODER_H

#include "common.h"

typedef struct
{
	uint8_t	TIMN_enum;				//脉冲计数引脚
	uint8_t	GPIN_enum;				//判断方向引脚
	uint8_t Dir;              //旋转方向
	uint16_t pouse;           //脉冲
	uint16_t pouse_last;     //上一时刻脉冲
  int pouse_t;            //实际增量值
	int pouse_read;
} Encoder_TypeDef;

void Encoder_Init(void);
int Encoder_Count_Read(void);
void Encoder_Clear(void);

#endif

