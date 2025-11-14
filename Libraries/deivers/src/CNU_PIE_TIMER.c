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
 * @file       CNU_PIE_TIMER.c
 * @brief      TIMER
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
 
#include "CNU_PIE_TIMER.h"

 /**************************************************************************************************************************
 * @brief  TIMER初始化函数
 * @exampleCode
 *      Timer_Count_Init(TIMER3_P04); //初始化定时器3 P04引脚作为计数引脚
 * @endcode
 * @param[in]  Timer_Count_Pin  定时器计数引脚
***************************************************************************************************************************/
void Timer_Count_Init(TIMER_COUNT_PIN_Enum Timer_Count_Pin)
{
	switch(Timer_Count_Pin)
	{
		case TIMER0_P34:
			TL0 = 0;      TH0 = 0; 
			TMOD |= 0x04; TR0 = 1; break;
		case TIMER1_P35:
			TL1 = 0x00;   TH1 = 0x00;
			TMOD |= 0x40; TR1 = 1; break;
		case TIMER2_P12:
			T2L = 0x00;   T2H = 0x00;
			AUXR |= 0x18;          break;
		case TIMER3_P04:
			T3L = 0;      T3H = 0;
			T4T3M |= 0x0c;         break;
		case TIMER4_P06:
			T4L = 0;      T4H = 0;
			T4T3M |= 0xc0;         break;
	}
}

 /**************************************************************************************************************************
 * @brief  TIMER读取计数引脚脉冲数据
 * @exampleCode
 *      Timer_Count_Read(TIMER3_P04); //定时器3 P04引脚读取计数数值
 * @endcode
 * @retval   count  计数数据
***************************************************************************************************************************/

uint16_t Timer_Count_Read(TIMER_COUNT_PIN_Enum Timer_Count_Pin)
{
	uint16_t count = 0;
	switch(Timer_Count_Pin)
	{
		case TIMER0_P34:
		     count = (uint16_t)TH0 << 8; count = ((uint8_t)TL0) | count;
		break;
		
		case TIMER1_P35:
		     count = (uint16_t)TH1 << 8; count = ((uint8_t)TL1) | count;
		break;
		
		case TIMER2_P12:
		     count = (uint16_t)T2H << 8; count = ((uint8_t)T2L) | count;
		break;
		
		case TIMER3_P04:
		     count = (uint16_t)T3H << 8; count = ((uint8_t)T3L) | count;	
		break;
		
		case TIMER4_P06:
	       count = (uint16_t)T4H << 8; count = ((uint8_t)T4L) | count;
		break;
	}
	return count;
}

 /**************************************************************************************************************************
 * @brief  TIMER计数清零
 * @exampleCode
 *      Timer_Count_Clear(TIMER3_P04); //初始化定时器3 P04引脚计数清零
 * @endcode
***************************************************************************************************************************/
void Timer_Count_Clear(TIMER_COUNT_PIN_Enum Timer_Count_Pin)
{	
	switch(Timer_Count_Pin)
	{
		case TIMER0_P34:
		  TR0 = 0; TH0 = 0; TL0 = 0; TR0 = 1; break;
		case TIMER1_P35:
			TR1 = 0; TH1 = 0; TL1 = 0; TR1 = 1; break;
		case TIMER2_P12:
		  AUXR &= ~(1<<4);  T2H = 0; T2L = 0; AUXR |= 1<<4; break;
		case TIMER3_P04:
			T4T3M &= ~(1<<3); T3H = 0; T3L = 0; T4T3M |= (1<<3); break;
		case TIMER4_P06:
			T4T3M &= ~(1<<7); T4H = 0; T4L = 0; T4T3M |= (1<<7); break;
	}
}

 /**************************************************************************************************************************
 * @brief  TIMER定时中断初始化
 * @exampleCode
 *      PIT_Timer_Ms(TIM0 ， 20); //初始化定时器0作为中断源，20ms定时中断
 * @endcode
 * @param[in]  Timer_CHN  定时器通道号
 * @param[in]  Time       中断时间
***************************************************************************************************************************/
void PIT_Timer_Ms(TIMER_CHN_Enum Timer_CHN , uint16_t Time)
{
	uint16_t time_reg;
	time_reg = (uint16_t)65536 - (uint16_t)(FOSC / (12 * (1000 / Time)));
	switch(Timer_CHN)
	{
		case TIM0:
		TMOD |= 0x00; TL0 = time_reg; TH0 = time_reg >> 8; TR0 = 1; ET0 = 1;
		break;
		case TIM1:
		TMOD |= 0x00; TL1 = time_reg; TH1 = time_reg >> 8; TR1 = 1; ET1 = 1;
		break;
		case TIM2:
	  T2L = time_reg; 	T2H = time_reg >> 8;     AUXR |= 0x10; IE2 |= 0x04;
		break;
		case TIM3:
		T3L = time_reg; 	T3H = time_reg >> 8;     T4T3M |= 0x08; IE2 |= 0x20;
		break;
		case TIM4:
		T4L = time_reg; 	T4H = time_reg >> 8;     T4T3M |= 0x80; IE2 |= 0x40;
		break;
	}
}
 /**************************************************************************************************************************
 * @brief  TIMER定时中断清空中断标志位
 * @exampleCode
 *      PIT_Timer_Clear(TIM0); //TIM0中断标志位清空
 * @endcode
 * @param[in]  Timer_CHN  定时器通道号
***************************************************************************************************************************/
void PIT_Timer_Clear(TIMER_CHN_Enum Timer_CHN)
{
	switch(Timer_CHN)
	{
		case TIM0: TCON &= ~0x80; break;
		case TIM1: TCON &= ~0x10; break;
		case TIM2: AUXINTIF &= ~0x01; break;
		case TIM3: AUXINTIF &= ~0x02; break;
		case TIM4: AUXINTIF &= ~0x04; break;
	}
}