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
 * @file       CNU_PIE_GPIO.h
 * @brief      GPIO
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#ifndef __CNU_PIE_GPIO_H_
#define __CNU_PIE_GPIO_H_


#include "STC32Gxx.h"
#include "common.h"

#define SUCCEED 1
#define FAIL    0

#define GPIO_HIGH   1
#define GPIO_LOW    0

typedef enum
{
  GPIO_P0	= 0,		
	GPIO_P1,
	GPIO_P2,
	GPIO_P3,
	GPIO_P4,
	GPIO_P5,
	GPIO_P6,
	GPIO_P7,
}GPIO_Port_enum;

typedef enum
{
  GPIO_Pin_0	=	0x01,	
  GPIO_Pin_1	=	0x02,	
  GPIO_Pin_2	=	0x04,	
  GPIO_Pin_3	=	0x08,	
  GPIO_Pin_4	=	0x10,	
  GPIO_Pin_5	=	0x20,	
  GPIO_Pin_6	=	0x40,	
  GPIO_Pin_7	=	0x80,	
  GPIO_Pin_LOW  = 0x0F,	//IO低4位引脚
  GPIO_Pin_HIGH	= 0xF0,	//IO高4位引脚
  GPIO_Pin_All  = 0xFF,	//IO所有引脚
}GPIO_Pin_enum;

typedef enum
{
	GPIO_PullUp	=	0,	//上拉准双向口
	GPIO_HighZ	=	1,	//浮空输入
	GPIO_OUT_OD	=	2,	//开漏输出
	GPIO_OUT_PP	=	3,	//推挽输出
}GPIO_Mode_enum;
typedef enum
{
	GPIO_NO_PULL  = 0,  //内部不上拉
	GPIO_Pull_Up	=	1,	//内部上拉
}GPIO_PinConfig;


uint8_t GPIO_PinPullConfig(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin , GPIO_PinConfig GPIO_Pin_Config);
uint8_t GPIO_Read_Bit(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin);
extern void GPIO_Init(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin , GPIO_Mode_enum GPIO_Mode);
extern void GPIO_Write_Bit(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin , uint8_t data_t);
extern void GPIO_Toggle_Bit(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin);

#endif