#ifndef __START_H__
#define __START_H__

#include "main.h"

void All_Init(uint8_t);
// void BIOS();
void beep(uint16_t, uint16_t, uint16_t, uint16_t);
void ADC_Norm_Slow();
void Get_ADC();
float Normalization();
#endif