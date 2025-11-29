/*********************************************************************************************************************
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2023,CNU_W.PIE
 *     All rights reserved.
 *     本库函数参考逐飞科技开源的STC函数库
 *     除注明出处外，以下所有内容版权均属胖胖个人所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留PP的版权声明。
 *     Except where indicated, the copyright of all the contents below is owned by PP 
 *     and can not be used for commercial purposes without permission. 
 *     The copyright notice of PP must be preserved when modifying the content.
 *
 * @file       CNU_PIE_PWM.h
 * @brief      PWM
 * @author     胖胖
 * @version    v1.1
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#ifndef __CNU_PIE_PWM_H_
#define __CNU_PIE_PWM_H_

#include "STC32Gxx.h"
#include "common.h"

#define PRECISION 10000.0f


typedef enum
{
	PWMA_CH1P_P10 = 0x00,PWMA_CH1N_P11,
	PWMA_CH1P_P20,		 PWMA_CH1N_P21,
	PWMA_CH1P_P60,		 PWMA_CH1N_P61,   

	PWMA_CH2P_P12 = 0x10,//该引脚已做 USB 内核电源稳压脚
	PWMA_CH2N_P13,          
	PWMA_CH2P_P22,		 PWMA_CH2N_P23,
	PWMA_CH2P_P62,		 PWMA_CH2N_P63,    

	PWMA_CH3P_P14 = 0x20,PWMA_CH3N_P15,
	PWMA_CH3P_P24,		 PWMA_CH3N_P25,     
	PWMA_CH3P_P64,		 PWMA_CH3N_P65,

	PWMA_CH4P_P16 = 0x30,PWMA_CH4N_P17,
	PWMA_CH4P_P26,		 PWMA_CH4N_P27,
	PWMA_CH4P_P66,		 PWMA_CH4N_P67,      
	PWMA_CH4P_P34,		 PWMA_CH4N_P33,
	
	PWMB_CH1_P20 = 0x40,
	PWMB_CH1_P17,
	PWMB_CH1_P00,
	PWMB_CH1_P74,     

	PWMB_CH2_P21 = 0x50,
	PWMB_CH2_P54,		//该引脚为复位引脚
	PWMB_CH2_P01,
	PWMB_CH2_P75,        

	PWMB_CH3_P22 = 0x60,
	PWMB_CH3_P33,
	PWMB_CH3_P02,
	PWMB_CH3_P76,            

	PWMB_CH4_P23 = 0x70,
	PWMB_CH4_P34,
	PWMB_CH4_P03,
	PWMB_CH4_P77,

}PWM_CHN_PIN_enum;
	

void PWM_Init(PWM_CHN_PIN_enum PWM_CHN_PIN , uint32_t frequency , uint32_t pwm_duty); 
void PWM_SET_Duty(PWM_CHN_PIN_enum PWM_CHN_PIN , uint32_t pwm_duty);
void PWM_SET_Frequency(PWM_CHN_PIN_enum PWM_CHN_PIN, uint32_t frequency, uint32_t pwm_duty );

#endif