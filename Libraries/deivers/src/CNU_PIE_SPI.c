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
 * @file       CNU_PIE_SPI.c
 * @brief      SPI
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#include "CNU_PIE_SPI.h"
#include "CNU_PIE_GPIO.h"

uint8_t 	SPI_RxTimerOut;
uint8_t 	SPI_BUF_type SPI_RxBuffer[SPI_BUF_LENTH];
bit B_SPI_Busy; //发送忙标志

 /**************************************************************************************************************************
 * @brief  SPI初始化
 * @exampleCode
 *      SPI_Init(SPI_1, 1 , SPI_LSB , SPI_CPOL_High , SPI_CPHA_2Edge , SPI_Speed_16 , SPI_Mode_Master , 1); //初始化SPI1 , 启用CS引脚 CPOL高 CPHA双边 时钟分频16分频 主机模式 开启SPI
 * @endcode
 * @param[in]  SPI_CHN    SPI组号 
 * @param[in]  SS_CFG     是否启用SS引脚              
 * @param[in]  FirstBit   SPI接收模式
 * @param[in]  cpol/cpha  SPI时钟/相位极性控制
 * @param[in]  Clock_Div  SPI总线速率
 * @param[in]  SPI_Mode   SPI主机/从机
 * @param[in]  SPI_EN     是否开启SPI
***************************************************************************************************************************/
void SPI_Init(SPI_ENUM SPI_CHN , uint8_t SS_CFG , uint8_t FirstBit , uint8_t cpol , uint8_t cpha , uint8_t Clock_Div , uint8_t SPI_Mode , uint8_t SPI_EN)
{
		switch(SPI_CHN)
  {
    case SPI_1:P_SW1 |= (0x00<<2);
        break;
    case SPI_2:P_SW1 |= (0x01<<2);
        break;
    case SPI_3:P_SW1 |= (0x02<<2);
        break;
    case SPI_4:P_SW1 |= (0x03<<2);
        break;
  }
	if(SS_CFG) SSIG = 0;//使能SS 通过SS引脚确认主设备还是从设备
	else SSIG = 1;      //禁用SS 通过SPI模式选择主从设备
	SPEN = SPI_EN;      //使能SPI
  DORD = FirstBit;    //选择接收模式 大端MSB 小端LSB
	MSTR = SPI_Mode;   //主从设置
	CPOL = cpol;        //SPI时钟极性控制
	CPHA = cpha;        //SPI时钟相位控制
	SPCTL = (SPCTL & ~0x03) | (Clock_Div);//速度/SPI时钟分频设置
	SPI_RxTimerOut = 0;
	B_SPI_Busy = 0;
}
 /**************************************************************************************************************************
 * @brief  SPI初始化
 * @exampleCode
 *      SPI_SetMode(SPI_Mode_Slave); //设置SPI为从机模式
 * @endcode
 * @param[in]  SPI_Mode   SPI模式
***************************************************************************************************************************/
void SPI_SetMode(uint8_t SPI_Mode)
{
	if(SPI_Mode == SPI_Mode_Slave)
	{
		MSTR = 0; 	//重新设置为从机待机
		SSIG = 0; 	//SS引脚确定主从
	}
	else
	{
		MSTR = 1; 	//使能 SPI 主机模式
		SSIG = 1; 	//忽略SS引脚功能
	}
}
 /**************************************************************************************************************************
 * @brief  SPI写一个字节数据
 * @exampleCode
 *      SPI_WriteByte(0xFF); //SPI写一个字节数据 0xff
 * @endcode
 * @param[in]  dat   SPI写入的数据
***************************************************************************************************************************/
void SPI_WriteByte(uint8_t dat)
{
	if(ESPI)
	{
		B_SPI_Busy = 1;
		SPDAT = dat;
		while(B_SPI_Busy);  //中断模式
	}
	else
	{
		SPDAT = dat;
		while(SPIF == 0); //查询模式
	  {SPIF = 1; WCOL = 1;}  //清除SPIF和WCOL标志
	}
}
 /**************************************************************************************************************************
 * @brief  SPI读取一个字节数据
 * @exampleCode
 * uint8_t data;
 * data = SPI_ReadByte(); //SPI读取一个字节数据
 * @endcode
 * @retval data   SPI读取的数据
***************************************************************************************************************************/
uint8_t SPI_ReadByte(void)
{
	SPDAT = 0xff;
	while(SPIF == 0) ;
	{SPIF = 1; WCOL = 1;}    //清0 SPIF和WCOL标志
	return (SPDAT);
}

uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
    SPDAT = TxData;					        //DATA寄存器赋值
    while (!(SPSTAT & 0x80));  		//查询完成标志
    SPSTAT = 0xc0;                //清中断标志
	  return SPDAT;
}
