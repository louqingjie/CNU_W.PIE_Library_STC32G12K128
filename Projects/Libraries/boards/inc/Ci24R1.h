/*
 * Ci24R1.h
 *
 *  Created on: 2020年4月5日
 *      Author: 肖时有
 */

#ifndef __Ci24R1_H_
#define __Ci24R1_H_     

#include "common.h"

#define NRF_CS_PORT    GPIO_P2
#define NRF_CLK_PORT   GPIO_P2
#define NRF_DATA_PORT  GPIO_P2

#define NRF_CS_PIN    GPIO_Pin_2
#define NRF_CLK_PIN   GPIO_Pin_5
#define NRF_DATA_PIN  GPIO_Pin_3

#define SCK_LOW  GPIO_Write_Bit(NRF_CLK_PORT,NRF_CLK_PIN,0)
#define SCK_HIGH GPIO_Write_Bit(NRF_CLK_PORT,NRF_CLK_PIN,1)

#define CS_LOW   GPIO_Write_Bit(NRF_CS_PORT,NRF_CS_PIN,0)
#define CS_HIGH  GPIO_Write_Bit(NRF_CS_PORT,NRF_CS_PIN,1)

#define DATA_IN  GPIO_Init(NRF_DATA_PORT,NRF_DATA_PIN,GPIO_PullUp)
#define DATA_OUT GPIO_Init(NRF_DATA_PORT,NRF_DATA_PIN,GPIO_OUT_PP)

#define DATA_SET(dat) GPIO_Write_Bit(NRF_DATA_PORT,NRF_DATA_PIN,dat)
#define DATA_READ     GPIO_Read_Bit (NRF_DATA_PORT, NRF_DATA_PIN)

#define NRF_DELAY(i) Us_Delay(i)


#define TX_PACKET_LENTH		32      //12
#define RX_PACKET_LENTH         12
#define ADR_WIDTH               5       //定义地址长度（3~5）
#define IS_CRC16                1       //1表示使用 CRC16，0表示 使用CRC8 (0~1)



//函数声明
uint8_t   Ci24R1_Init(void);

uint8_t   nrf_link_check(void);                  //检测Ci24R1+与单片机是否通信正常

void nrf_tx(char* txbuf, uint16_t len);   //将发送内容置入fifo队列中，同时发送最大长度

uint8_t nrf_handler(void); 

void RCPacket_Send(void);

#endif      //_Ci24R1_H_
