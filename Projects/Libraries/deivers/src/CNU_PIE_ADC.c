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
 * @file       CNU_PIE_ADC.c
 * @brief      ADC
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#include "CNU_PIE_ADC.h"
 /**************************************************************************************************************************
 * @brief  ADC初始化函数
 * @exampleCode
 *      ADC_Init(ADC_P10 , ADC_SPEED_2X16T); //ADC P10引脚初始化为ADC ，时钟分频ADC_SPEED_2X16T
 * @endcode
 * @param[in]  ADC_PIN   ADC引脚
 * @param[in]  ADC_SPEED ADC时钟分频系输
***************************************************************************************************************************/
void ADC_Init(ADC_PIN_ENUM ADC_PIN , ADC_SPEED_ENUM ADC_SPEED)
{
	ADC_CONTR |= 1<<7;				
	ADC_CONTR &= (0xF0);		
	ADC_CONTR |= ADC_PIN;
	
	if((ADC_PIN >> 3) == 1) //P0端口
	{
		//IO口需要设置为高阻输入
		P0M0 &= ~(1 << (ADC_PIN & 0x07));
		P0M1 |= (1 << (ADC_PIN & 0x07));
	}
	else if((ADC_PIN >> 3) == 0) //P1端口	
	{
		//IO口需要设置为高阻输入
		P1M0 &= ~(1 << (ADC_PIN & 0x07));
	  P1M1 |= (1 << (ADC_PIN & 0x07));
	}

	ADCCFG |= ADC_SPEED&0x0F;			//Fosc_ADC = SYSCLK/2(SPEED+1)
	
	ADCCFG |= 1<<5;					//转换结果右对齐。 ADC_RES 保存结果的高 2 位， ADC_RESL 保存结果的低 8 位。
}
 /**************************************************************************************************************************
 * @brief  ADC初始化函数
 * @exampleCode
 *      uint16_t data;
 *      data = ADC_Init(ADC_P10 , ADC_12BIT); //ADC P10读取一次数据 12位精度
 * @endcode
 * @retval ADC_Value   ADC读取一次的数据
***************************************************************************************************************************/
uint16_t ADC_Read_Once(ADC_PIN_ENUM ADC_PIN , uint8_t Precision)
{
	uint16_t ADC_Value;
	
	ADC_CONTR &= (0xF0);			//清除ADC_CHS[3:0] ： ADC 模拟通道选择位
	ADC_CONTR |= ADC_PIN;
	
	ADC_CONTR |= 0x40;  			// 启动 AD 转换
	while (!(ADC_CONTR & 0x20));  	// 查询 ADC 完成标志
	ADC_CONTR &= ~0x20;  			// 清完成标志
	
	ADC_Value = ADC_RES;  			//存储 ADC 的 12 位结果的高 4 位
	ADC_Value <<= 8;
	ADC_Value |= ADC_RESL;  		//存储 ADC 的 12 位结果的低 8 位
	
	ADC_RES = 0;
	ADC_RESL = 0;
	
	ADC_Value >>= Precision;		//取多少位
	
	return ADC_Value;
}
 /**************************************************************************************************************************
 * @brief  ADC传输取数+滤波（均值冒泡）函数
 * @exampleCode
 *      uint16_t data;
 *      data = ADC_Average(ADC_P10 , ADC_12BIT , 10); //ADC P10读取十次数据 12位精度 返回一个数据
 * @endcode
 * @retval ADC_Value   ADC读取一次的数据
***************************************************************************************************************************/
uint16_t ADC_Average(ADC_PIN_ENUM ADC_PIN , uint8_t Precision , uint8_t N) 
{
  uint32_t sum=0;
  uint8_t M=N;
	int i=0;
	int j=0;
  int str[20]={0};
  for(i=0;i<N;i++)str[i]=ADC_Read_Once(ADC_PIN,Precision);
  for(i=1;i<N;i++)
    for(j=0;j<N-i;j++)
    {
      if(str[j]>str[j+1])
      {
        int t=str[j+1];
        str[j+1]=str[j];
        str[j]=t;
      }
    }
  for(i=1;i<N-1;i++)sum+=str[i];
  return ((uint16_t)(sum/M));
}
