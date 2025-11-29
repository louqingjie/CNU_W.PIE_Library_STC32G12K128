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
 * @file       CNU_PIE_I2C.h
 * @brief      I2C
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#ifndef __CNU_PIE_I2C_H_
#define __CNU_PIE_I2C_H_

#include "STC32Gxx.h"
#include "common.h"

//IIC_1 SCL:P1.5	SDA:P1.4
//IIC_2 SCL:P2.5	SDA:P2.4
//IIC_3 SCL:P7.7	SDA:P7.6
//IIC_4 SCL:P3.2	SDA:P3.3

typedef enum
{
	IIC_1 = 0x00,
	IIC_2,
	IIC_3,
	IIC_4,
}IIC_ENUM;

void I2C_Init_Master(IIC_ENUM I2C_enum , uint32_t I2C_BUS_Rate , uint8_t I2C_WDTA_EN , uint8_t I2C_Enable);
void I2C_Init_Slave(IIC_ENUM I2C_enum , uint8_t I2C_Slave_Add , uint8_t I2C_MATCH_EN , uint8_t I2C_Enable);
void I2C_WriteNbyte(uint8_t addr, uint8_t reg , uint8_t *p, uint8_t number) reentrant;
void I2C_ReadNbyte(uint8_t addr, uint8_t reg , uint8_t *p, uint8_t number);
void I2C_Change_Pin(IIC_ENUM I2C_enum);
#endif