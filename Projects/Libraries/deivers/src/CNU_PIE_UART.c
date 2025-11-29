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
 * @file       CNU_PIE_UART.c
 * @brief      UART
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#include "CNU_PIE_UART.h"

int uart_receive[4];
uint8_t UART_BUSY[5];				                  //串口接收忙标志位
uint8_t uart1_tx_buff[UART1_TX_BUFFER_SIZE];	//发送缓冲
uint8_t uart1_rx_buff[UART1_RX_BUFFER_SIZE];	//接收缓冲

 /**************************************************************************************************************************
 * @brief  UART引脚初始化
 * @exampleCode
 *       UART_Init(UART_1, UART1_RX_P30, UART1_TX_P31, 230400, TIM1);     
  //初始化串口1 波特率115200 发送引脚使用P31 接收引脚使用P30 ,使用定时器1作为波特率发生器
 * @endcode
 * @param[in]  UART_N        UART串口号
 * @param[in]  UART_Rx_Pin   RX引脚              
 * @param[in]  UART_Tx_Pin   TX引脚
 * @param[in]  BaudRate      波特率
 * @param[in]  Timer_CHN     波特率发生器-定时器
***************************************************************************************************************************/
void UART_Init(UARTN_Enum UART_N, UART_PIN_Enum UART_Rx_Pin, UART_PIN_Enum UART_Tx_Pin, uint32_t BaudRate , TIMER_CHN_Enum Timer_CHN)
{
  uint16_t brt;
	brt = (uint16_t)(65536 - (FOSC/BaudRate/4));
	switch(UART_N)
	{
		case UART_1:
		{
			if     (TIM1 == Timer_CHN){SCON |= 0x50; TMOD |= 0x00; TL1 = brt; TH1 = brt >> 8; AUXR |= 0x40; TR1 = 1; UART_BUSY[1] = 0;}
			else if(TIM2 == Timer_CHN){SCON |= 0x50; T2L = brt; T2H = brt >> 8; AUXR |= 0x15;} P_SW1 &= ~(0x03<<6);
			if     ((UART1_RX_P30 == UART_Rx_Pin) && (UART1_TX_P31 == UART_Tx_Pin)) P_SW1 |= 0x00;
			else if((UART1_RX_P36 == UART_Rx_Pin) && (UART1_TX_P37 == UART_Tx_Pin)) P_SW1 |= 0x40;
			else if((UART1_RX_P16 == UART_Rx_Pin) && (UART1_TX_P17 == UART_Tx_Pin)) P_SW1 |= 0x80;
			else if((UART1_RX_P43 == UART_Rx_Pin) && (UART1_TX_P44 == UART_Tx_Pin)) P_SW1 |= 0xc0;
      UART_BUSY[1] = 0; ES = 1; break;
		}
		case UART_2:
		{
			if(TIM2 == Timer_CHN){S2CON |= 0x10; T2L = brt; T2H = brt >> 8; AUXR |= 0x14;} P_SW2 &= ~(0x01<<0);
			if     ((UART2_RX_P10 == UART_Rx_Pin) && (UART2_TX_P11 == UART_Tx_Pin)) P_SW2 |= 0x00;
			else if((UART2_RX_P46 == UART_Rx_Pin) && (UART2_TX_P47 == UART_Tx_Pin)) P_SW2 |= 0x01;
			UART_BUSY[2] = 0; ES2 = 1; break;
			break;
		}
		case UART_3:
		{
			if(TIM2 == Timer_CHN)     {S3CON |= 0x10; T2L = brt; T2H = brt >> 8; AUXR |= 0x14;}
			else if(TIM3 == Timer_CHN){S3CON |= 0x50; T3L = brt; T3H = brt >> 8; T4T3M |= 0x0a;P_SW2 &= ~(0x01<<1);}
			if     ((UART3_RX_P00 == UART_Rx_Pin) && (UART3_TX_P01 == UART_Tx_Pin)) P_SW2 |= 0x00;
			else if((UART3_RX_P50 == UART_Rx_Pin) && (UART3_TX_P51 == UART_Tx_Pin)) P_SW2 |= 0x02;
			UART_BUSY[3] = 0; ES3 = 1;
			break;
		}
		case UART_4:
		{
			if(TIM2 == Timer_CHN)      {S4CON |= 0x10; T2L = brt; T2H = brt >> 8; AUXR |= 0x14;}
			else if(TIM4 == Timer_CHN) {S4CON |= 0x50; T4L = brt; T4H = brt >> 8; T4T3M |= 0xa0;} P_SW2 &= ~(0x01<<2);
			if     ((UART4_RX_P02 == UART_Rx_Pin) && (UART4_TX_P03 == UART_Tx_Pin))                              P_SW2 |= 0x00;
			else if((UART4_RX_P52 == UART_Rx_Pin) && (UART4_TX_P53 == UART_Tx_Pin)){P5M0 = 0x00; P5M1 = 0x01<<2; P_SW2 |= 0x04;}
			UART_BUSY[4] = 0; ES4 = 1;
			break;
		}
	}
}
 /**************************************************************************************************************************
 * @brief  UART发送一个字节
 * @exampleCode
 *       UART_PutChar(UART_1, 0xff);        //串口1发送0xff
 * @endcode
 * @param[in]  UART_N        UART串口号
 * @param[in]  data_t        发送的数据              
***************************************************************************************************************************/
void UART_PutChar(UARTN_Enum UART_N,uint8_t data_t)
{
	switch(UART_N)
	{
		case UART_1:
			while (UART_BUSY[1]);
			UART_BUSY[1] = 1;  SBUF = data_t;break;
		case UART_2:
			while (UART_BUSY[2]);
			UART_BUSY[2] = 1; S2BUF = data_t;break;
		case UART_3:
			while (UART_BUSY[3]);
			UART_BUSY[3] = 1; S3BUF = data_t;break;
		case UART_4:
			while (UART_BUSY[4]);
			UART_BUSY[4] = 1; S4BUF = data_t;break;
	}
}
 /**************************************************************************************************************************
 * @brief  UART发送数组
 * @exampleCode
 *       UART_PutBuff(UART_1, &data[0] ,5);        //串口1发送data数组 发送五个字节
 * @endcode
 * @param[in]  UART_N        UART串口号 
 * @param[in]  *p            地址            
 * @param[in]  lenth         数据长度  
***************************************************************************************************************************/
void UART_PutBuff(UARTN_Enum UART_N , uint8_t *p , uint16_t lenth)
{
    while(lenth--)UART_PutChar(UART_N,*p++);
}
 /**************************************************************************************************************************
 * @brief  UART发送字符串
 * @exampleCode
*       UART_PutBuff(UART_1,“w.pie”);        //串口1发送字符串
 * @endcode
 * @param[in]  UART_N        UART串口号
 * @param[in]  *str          字符串/字符串首地址              
***************************************************************************************************************************/
void UART_PutStr(UARTN_Enum UART_N , uint8_t *str)
{
    while(*str)
    {
      UART_PutChar(UART_N, *str++);
    }
}

uint8_t UART_Receive_t(UARTN_Enum UART_N)
{
	uint8_t data_t;
	switch(UART_N)
	{
		case UART_1:
			data_t = SBUF;
			return  data_t;break;
		case UART_2:
			data_t = S2BUF;
			return data_t;break;
		case UART_3:
			data_t = S3BUF;
			return data_t;break;
		case UART_4:
			data_t = S4BUF;
			return data_t;break;
		default:
			return 0;  break;
	}
}
