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
 * @file       common.c
 * @brief      通用
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#include "common.h"
#include "intrins.h"
 #include "CNU_PIE_GPIO.h"
volatile unsigned int DELAY_MS = 0;
volatile unsigned int DELAY_US = 0;
unsigned long system_clock;
 /**************************************************************************************************************************
 * @brief  设置主时钟频率
 * @note   内部调用用户无需关心
 * @param[in]  NULL
 * @retval  设置的主时钟频率
***************************************************************************************************************************/
uint32_t System_Clock_Set(void)
{
	system_clock = FOSC;
	P_SW2 |= 0x80;//使能访问特殊寄存器
	switch (system_clock)
	{
		case 22118400://22.1184MHz
			CLKDIV = 0x04; IRTRIM = T22M_ADDR; VRTRIM = VRT27M_ADDR;
		  IRCBAND = 0x02; CLKDIV = 0x00; break;
		case 24000000://24MHz
		  CLKDIV = 0x04; IRTRIM = T24M_ADDR; VRTRIM = VRT27M_ADDR;
		  IRCBAND = 0x02; CLKDIV = 0x00; break;
    case 27000000://27MHz
			CLKDIV = 0x04; IRTRIM = T27M_ADDR; VRTRIM = VRT27M_ADDR;
		  IRCBAND = 0x02; CLKDIV = 0x00; break;
		case 30000000://30MHz
			CLKDIV = 0x04; IRTRIM = T30M_ADDR; VRTRIM = VRT27M_ADDR;
		  IRCBAND = 0x02; CLKDIV = 0x00; break;
		case 33177600://33.1776MHz
			CLKDIV = 0x04; IRTRIM = T33M_ADDR; VRTRIM = VRT27M_ADDR;
		  IRCBAND = 0x02; CLKDIV = 0x00; break;
		case 35000000://35MHz
			CLKDIV = 0x04; IRTRIM = T35M_ADDR; VRTRIM = VRT44M_ADDR;
		  IRCBAND = 0x03; CLKDIV = 0x00; break;
    default://默认35MHz
			CLKDIV = 0x04; IRTRIM = T35M_ADDR; VRTRIM = VRT44M_ADDR;
		  IRCBAND = 0x03; CLKDIV = 0x00; break;
	}
	return system_clock;
}
 /**************************************************************************************************************************
 * @brief  延时函数初始化
 * @note   内部调用用户无需关心
 * @param[in]  NULL
 * @retval     NULL
***************************************************************************************************************************/
void Delay_Init(void)
{
	DELAY_MS = system_clock / 6000; DELAY_US = system_clock / 7000000;
	if(system_clock <= 12000000) DELAY_US++;//自适应主时钟
}
 /**************************************************************************************************************************
 * @brief  寄存器相关配置
 * @note   内部调用用户无需关心
 * @param[in]  NULL
 * @retval     NULL
***************************************************************************************************************************/
void Register_Set(void)
{
	EAXFR = 1;				// 使能访问XFR
	CKCON = 0x00;			// 设置外部数据总线为最快
	WTST = 0;
	P54RST = 1;	      // 使P54为复位引脚
	P_SW2 = 0x80;			// 开启特殊地址访问
	//if(System_Clock_Set() != 35000000)  WTST = 0;//CPU读取程序存储器的等待时间 0为最快
	//else WTST = 0x07; //当主频在35MHz时或超频工作，需要设置等待时长，默认为7个时钟周期	
#if (1 == EXTERNAL_CRYSTA_ENABLE)
	XOSCCR = 0xc0; 			//启动外部晶振
	while (!(XOSCCR & 1)); 	//等待时钟稳定
	CLKDIV = 0x00; 			//时钟不分频
	CLKSEL = 0x01; 			//选择外部晶振
#else
	//自动设置系统频率
	#if (33177600 == FOSC)
		system_clock = System_Clock_Set();
	#else
		system_clock = FOSC;
	#endif
#endif
	 
	Delay_Init();       //延时函数初始化
	//ENLVR = 0;        // 禁止开发板低电压复位
	
	WTST = 0;
  P_SW2 |= 0x80;
  CLKDIV = 0;				//24MHz主频，分频设置
	
	P0M0 = 0x00;P0M1 = 0x00;// P0
	P1M0 = 0x00;P1M1 = 0x00;// P1
	P2M0 = 0x00;P2M1 = 0x00;// P2
	P3M0 = 0x00;P3M1 = 0x00;// P3
	P4M0 = 0x00;P4M1 = 0x00;// P4
  P5M0 = 0x00;P5M1 = 0x00;// P5
	P6M0 = 0x00;P6M1 = 0x00;// P6
	P7M0 = 0x00;P7M1 = 0x00;// P7
	
	ADCCFG = 0;
	AUXR = 0;
	SCON = 0;
	S2CON = 0;
	S3CON = 0;
	S4CON = 0;
	P_SW1 = 0;
	IE2 = 0;
	TMOD = 0;
}
 /**************************************************************************************************************************
 * @brief  毫秒级延时函数
 * @note   实现毫秒延时，自适应主时钟
 * @param[in]  延时时间
 * @retval     NULL
***************************************************************************************************************************/
void Ms_Delay(uint16_t ms)
{
	uint16_t i;
	do{
		i = DELAY_MS;
		while(--i);
	}while(--ms);
}
 /**************************************************************************************************************************
 * @brief  微秒级延时函数
 * @note   实现微秒延时，自适应主时钟，不准确延时
 * @param[in]  延时时间
 * @retval     NULL
***************************************************************************************************************************/
void Us_Delay(uint32_t us)
{
	uint16_t i;
	do {
			i = DELAY_US;
			while(--i);
	   }while(--us);
}
 /**************************************************************************************************************************
 * @brief  禁用全局中断
 * @note   禁止中断
 * @param[in]  NULL
 * @retval     NULL
***************************************************************************************************************************/
void DisableGlobalIRQ(void)
{
	EA = 0;
}
 /**************************************************************************************************************************
 * @brief  开启全局中断
 * @note   开始中断
 * @param[in]  NULL
 * @retval     NULL
***************************************************************************************************************************/
void EnableGlobalIRQ(void)
{
	EA = 1;
}
 /**************************************************************************************************************************
 * @brief  开发板初始化
 * @note   寄存器配置+中断是否开启+延时初始化
 * @param[in]  NULL
 * @retval     NULL
***************************************************************************************************************************/
void Board_Init(void)
{
	Register_Set();    //寄存器配置
	EnableGlobalIRQ();//启用全局中断
}