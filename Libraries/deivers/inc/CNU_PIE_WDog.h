/*********************************************************************************************************************
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2023,CNU_W.PIE
 *     All rights reserved.
 *     本库函数参考STC官方函数库
 *     除注明出处外，以下所有内容版权均属胖胖个人所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留PP的版权声明。
 *     Except where indicated, the copyright of all the contents below is owned by PP 
 *     and can not be used for commercial purposes without permission. 
 *     The copyright notice of PP must be preserved when modifying the content.
 *
 * @file       CNU_PIE_WDog.h
 * @brief      WDog
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#ifndef __CNU_PIE_WDOG_H_
#define __CNU_PIE_WDOG_H_

#include "STC32Gxx.h"
#include "common.h"

#define D_WDT_FLAG			(1<<7)
#define D_EN_WDT			(1<<5)
#define D_CLR_WDT			(1<<4)	/* auto clear	*/
#define D_IDLE_WDT			(1<<3)	/* WDT counter when Idle	*/

#define WDT_IDLE_STOP		0
#define WDT_IDLE_RUN		1

#define WDT_SCALE_2			0		/* WDT Timeout=(12*32768*SCALE)/SYSclk */
#define WDT_SCALE_4			1
#define WDT_SCALE_8			2
#define WDT_SCALE_16		3
#define WDT_SCALE_32		4
#define WDT_SCALE_64		5
#define WDT_SCALE_128		6
#define WDT_SCALE_256		7

#define	WDT_PS_Set(n)	WDT_CONTR = (WDT_CONTR & ~0x07) | (n & 0x07)		/* 看门狗定时器时钟分频系数设置 */
#define	WDT_reset(n)	WDT_CONTR = D_EN_WDT + D_CLR_WDT + D_IDLE_WDT + (n)		/* 初始化WDT，喂狗 */

typedef struct
{
	uint8_t	WDT_Enable;				//看门狗使能  	ENABLE,DISABLE
	uint8_t	WDT_IDLE_Mode;		//IDLE模式停止计数		WDT_IDLE_STOP,WDT_IDLE_RUN
	uint8_t	WDT_PS;						//看门狗定时器时钟分频系数		WDT_SCALE_2,WDT_SCALE_4,WDT_SCALE_8,WDT_SCALE_16,WDT_SCALE_32,WDT_SCALE_64,WDT_SCALE_128,WDT_SCALE_256
} WDog_InitTypeDef;

void WDog_Inilize(WDog_InitTypeDef *WDT);
void WDog_Clear (void);




#endif
