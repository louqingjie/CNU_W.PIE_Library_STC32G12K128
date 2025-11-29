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
 * @file       CNU_PIE_UART.h
 * @brief      UART
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#ifndef __CNU_PIE_UART_H_
#define __CNU_PIE_UART_H_

#include "STC32Gxx.h"
#include "common.h"

#include "CNU_PIE_TIMER.h"



extern int uart_receive[4];

#define UART1_RX_BUFFER_SIZE	100
#define UART1_TX_BUFFER_SIZE	100

#define	UART1_CLEAR_RX_FLAG (SCON  &= ~0x01)
#define	UART2_CLEAR_RX_FLAG (S2CON &= ~0x01)
#define	UART3_CLEAR_RX_FLAG (S3CON &= ~0x01)
#define	UART4_CLEAR_RX_FLAG (S4CON &= ~0x01)

#define	UART1_CLEAR_TX_FLAG (SCON  &= ~0x02)
#define	UART2_CLEAR_TX_FLAG (S2CON &= ~0x02)
#define	UART3_CLEAR_TX_FLAG (S3CON &= ~0x02)
#define	UART4_CLEAR_TX_FLAG (S4CON &= ~0x02)


#define UART1_GET_RX_FLAG   (SCON  & 0x01)
#define UART2_GET_RX_FLAG   (S2CON & 0x01)
#define UART3_GET_RX_FLAG   (S3CON & 0x01)
#define UART4_GET_RX_FLAG   (S4CON & 0x01)
						    
#define UART1_GET_TX_FLAG   (SCON  & 0x02)
#define UART2_GET_TX_FLAG   (S2CON & 0x02)
#define UART3_GET_TX_FLAG   (S3CON & 0x02)
#define UART4_GET_TX_FLAG   (S4CON & 0x02)

typedef enum
{
   UART_1,
   UART_2,
   UART_3,
   UART_4,
}UARTN_Enum;

typedef enum
{
	UART1_RX_P30, UART1_TX_P31,		
	UART1_RX_P36, UART1_TX_P37,
	UART1_RX_P16, UART1_TX_P17,
	UART1_RX_P43, UART1_TX_P44,
	
	UART2_RX_P10, UART2_TX_P11,
	UART2_RX_P46, UART2_TX_P47,
	
	UART3_RX_P00, UART3_TX_P01,
	UART3_RX_P50, UART3_TX_P51,
	
	UART4_RX_P02, UART4_TX_P03,
	UART4_RX_P52, UART4_TX_P53,	
}UART_PIN_Enum;

extern uint8_t uart1_tx_buff[UART1_TX_BUFFER_SIZE];	//发送缓冲
extern uint8_t uart1_rx_buff[UART1_RX_BUFFER_SIZE];	//接收缓冲
extern uint8_t UART_BUSY[5];

void UART_Init(UARTN_Enum UART_N, UART_PIN_Enum UART_Rx_Pin, UART_PIN_Enum UART_Tx_Pin, uint32_t BaudRate , TIMER_CHN_Enum Timer_CHN);
void UART_PutChar(UARTN_Enum UART_N,uint8_t data_t);
void UART_PutBuff(UARTN_Enum UART_N , uint8_t *p , uint16_t lenth);
void UART_PutStr(UARTN_Enum UART_N , uint8_t *str);
uint8_t UART_Receive_t(UARTN_Enum UART_N);

#endif