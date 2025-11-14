/*********************************************************************************************************************
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2023,CNU_W.PIE
 *     All rights reserved.
 *
 *     除注明出处外，以下所有内容版权均属胖胖个人所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留PP的版权声明。
 *     Except where indicated, the copyright of all the contents below is owned by PP 
 *     and can not be used for commercial purposes without permission. 
 *     The copyright notice of PP must be preserved when modifying the content.
 *
 * @file       CNU_PIE_ADC.h
 * @brief      ADC
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#ifndef __CNU_PIE_ADC_H_
#define __CNU_PIE_ADC_H_

#include "STC32Gxx.h"
#include "common.h"

//ADC精度选择
#define ADC_12BIT 0
#define ADC_11BIT 1
#define ADC_10BIT 2
#define ADC_9BIT  3
#define ADC_8BIT  4

typedef enum
{
  ADC_P10 = 0     , 
  ADC_P11, 
  ADC_P12,
	ADC_P13, 
  ADC_P14, 
	ADC_P15, 
	ADC_P16, 
	ADC_P17, 
	
	ADC_P00, 
	ADC_P01, 
	ADC_P02, 
	ADC_P03, 
	ADC_P04, 
	ADC_P05, 
	ADC_P06, 
	ADC_POWR = 0x0f	, //内部AD 1.19V
} ADC_PIN_ENUM;

typedef enum
{
  ADC_SPEED_2X1T	=	0		,	//SYSclk/2/1
  ADC_SPEED_2X2T	=	1		,	//SYSclk/2/2
  ADC_SPEED_2X3T	=	2		,	//SYSclk/2/3
  ADC_SPEED_2X4T	=	3		,	//SYSclk/2/4
  ADC_SPEED_2X5T	=	4		,	//SYSclk/2/5
  ADC_SPEED_2X6T	=	5		,	//SYSclk/2/6
  ADC_SPEED_2X7T	=	6		,	//SYSclk/2/7
  ADC_SPEED_2X8T	=	7		,	//SYSclk/2/8
  ADC_SPEED_2X9T	=	8		,	//SYSclk/2/9
  ADC_SPEED_2X10T	=	9		,	//SYSclk/2/10
  ADC_SPEED_2X11T	=	10	,	//SYSclk/2/11
  ADC_SPEED_2X12T	=	11	,	//SYSclk/2/12
  ADC_SPEED_2X13T	=	12	,	//SYSclk/2/13
  ADC_SPEED_2X14T	=	13	,	//SYSclk/2/14
  ADC_SPEED_2X15T	=	14	,	//SYSclk/2/15
  ADC_SPEED_2X16T	=	15	,	//SYSclk/2/16
} ADC_SPEED_ENUM;

void ADC_Init(ADC_PIN_ENUM ADC_PIN , ADC_SPEED_ENUM ADC_SPEED);
uint16_t ADC_Read_Once(ADC_PIN_ENUM ADC_PIN , uint8_t Precision);
uint16_t ADC_Average(ADC_PIN_ENUM ADC_PIN , uint8_t Precision , uint8_t N);


#endif