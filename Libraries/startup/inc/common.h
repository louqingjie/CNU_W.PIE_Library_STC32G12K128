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
 * @file       common.h
 * @brief      数据类型声明
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/

#ifndef __COMMON_H_
#define __COMMON_H_

#include <string.h>
#include <stdio.h>
#include "STC32Gxx.h"
#include "intrins.h"

//单片机主频设置 22118400 24000000 27000000 30000000 33177600 35000000
#define FOSC 33177600//24MHz
#define EXTERNAL_CRYSTA_ENABLE 0
extern unsigned long system_clock;

typedef unsigned char   uint8_t;
typedef unsigned int  	uint16_t; 
typedef unsigned long  	uint32_t ; 							
typedef signed char     int8_t   ; 
typedef signed int      int16_t  ; 
typedef signed long     int32_t  ; 		
typedef float fp32;
typedef volatile int8_t   vint8_t  ; 
typedef volatile int16_t  vint16_t ; 
typedef volatile int32_t  vint32_t ; 													
typedef volatile uint8_t  vuint8_t ; 
typedef volatile uint16_t vuint16_t; 
typedef volatile uint32_t vuint32_t; 

void Board_Init(void);
extern void Ms_Delay(uint16_t ms);
extern void Us_Delay(uint32_t us);
extern void EnableGlobalIRQ(void);
extern void DisableGlobalIRQ(void);
#endif
