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
 * @file       CNU_PIE_EXTI.h
 * @brief      EXTI
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#ifndef __CNU_PIE_EXTI_H_
#define __CNU_PIE_EXTI_H_

#include "STC32Gxx.h"
#include "common.h"

#include "CNU_PIE_GPIO.h"

#define P0_PRI0RITY 0x01
#define P1_PRI0RITY 0x02
#define P2_PRI0RITY 0x04
#define P3_PRI0RITY 0x08
#define P4_PRI0RITY 0x10
#define P5_PRI0RITY 0x20
#define P6_PRI0RITY 0x40
#define P7_PRI0RITY 0x80


extern uint8_t Port_Exti_Flag[8];


typedef enum
{
	FALLING_EDGE = 0,	//下降沿
	RISING_EDGE,	    //上升沿
	LOW_LEVEL,        //低电平
	HIGH_LEVEL,       //高电平
}EXTI_MODE_Enum;

typedef enum
{
	Highest_priority = 0,	 //最高
	Second_priority,	     //第二
	Third_priority,        //第三
	Lowest_priority,       //最低
}EXTI_PRIORITY_Enum;

uint8_t GPIO_EXTI_Init(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin , EXTI_MODE_Enum EXTI_Mode);
uint8_t GPIO_EXTI_Open(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin);
uint8_t GPIO_EXTI_Set_Priority(GPIO_Port_enum GPIO_Port , EXTI_PRIORITY_Enum EXTI_Priority);
uint8_t GPIO_EXTI_Flag_Read(GPIO_Port_enum GPIO_Port);
uint8_t GPIO_EXTI_Flag_Clear(GPIO_Port_enum GPIO_Port);

#endif