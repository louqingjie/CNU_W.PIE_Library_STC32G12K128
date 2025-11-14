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
 * @file       CNU_PIE_WDog.c
 * @brief      WDog
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#include "CNU_PIE_WDog.h"
 /**************************************************************************************************************************
 * @brief  看门狗初始化程序
 * @param[in]  WDT   结构参数,请参考WDT.h里的定义
***************************************************************************************************************************/
void WDog_Init(WDog_InitTypeDef *WDT)
{
	if(WDT->WDT_Enable == ENABLE)		EN_WDT = 1;	//使能看门狗

	WDT_PS_Set(WDT->WDT_PS);	//看门狗定时器时钟分频系数		WDT_SCALE_2,WDT_SCALE_4,WDT_SCALE_8,WDT_SCALE_16,WDT_SCALE_32,WDT_SCALE_64,WDT_SCALE_128,WDT_SCALE_256
	if(WDT->WDT_IDLE_Mode == WDT_IDLE_STOP)	IDL_WDT = 0;	//IDLE模式停止计数
	else									IDL_WDT = 1;	//IDLE模式继续计数
}

 /**************************************************************************************************************************
 * @brief  清除看门狗初始化程序 喂狗
***************************************************************************************************************************/
void WDog_Clear (void)
{
	CLR_WDT = 1;    // 喂狗
}