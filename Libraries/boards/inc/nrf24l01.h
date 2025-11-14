#ifndef __NRF24L01_H
#define __NRF24L01_H	

#include "common.h"
#include "CNU_PIE_GPIO.h"

extern uint8_t Channal; //频道全局定义



#define TX_PACKET_LENTH		32 //12
#define RX_PACKET_LENTH   12
#define ADR_WIDTH         5       //定义地址长度（3~5）
#define IS_CRC16          1       //1表示使用 CRC16，0表示 使用CRC8 (0~1)

extern uint8_t TX_Buff[TX_PACKET_LENTH];  
extern uint8_t RX_Buff[RX_PACKET_LENTH];

//NRF24L01
#define 	RF2G4_CE_Port   	GPIO_P3
#define 	RF2G4_CSN_Port 		GPIO_P2
#define 	RF2G4_IRQ_Port  	GPIO_P2
#define 	RF2G4_SCK_Port   	GPIO_P2
#define 	RF2G4_MOSI_Port 	GPIO_P2
#define 	RF2G4_MISO_Port  	GPIO_P2


#define 	RF2G4_CE_Pin   	  GPIO_Pin_4
#define 	RF2G4_CSN_Pin 		GPIO_Pin_2
#define 	RF2G4_IRQ_Pin  	  GPIO_Pin_6
#define 	RF2G4_SCK_Pin   	GPIO_Pin_5
#define 	RF2G4_MOSI_Pin  	GPIO_Pin_3
#define 	RF2G4_MISO_Pin  	GPIO_Pin_4

#define 	RF2G4_CE_HIGH     GPIO_Write_Bit(RF2G4_CE_Port  , RF2G4_CE_Pin  , 1) 
#define 	RF2G4_CE_LOW      GPIO_Write_Bit(RF2G4_CE_Port  , RF2G4_CE_Pin  , 0) 
#define 	RF2G4_CSN_HIGH    GPIO_Write_Bit(RF2G4_CSN_Port , RF2G4_CSN_Pin , 1) 
#define 	RF2G4_CSN_LOW     GPIO_Write_Bit(RF2G4_CSN_Port , RF2G4_CSN_Pin , 0) 
#define 	RF2G4_IRQ_HIGH    GPIO_Write_Bit(RF2G4_IRQ_Port , RF2G4_IRQ_Pin , 1) 
#define 	RF2G4_IRQ_LOW     GPIO_Write_Bit(RF2G4_IRQ_Port , RF2G4_IRQ_Pin , 0) 

#define  SCK_H    GPIO_Write_Bit(RF2G4_SCK_Port  , RF2G4_SCK_Pin  , 1) 
#define  SCK_L    GPIO_Write_Bit(RF2G4_SCK_Port  , RF2G4_SCK_Pin  , 0) 
#define  MOSI_H   GPIO_Write_Bit(RF2G4_MOSI_Port , RF2G4_MOSI_Pin  , 1) 
#define  MOSI_L   GPIO_Write_Bit(RF2G4_MOSI_Port , RF2G4_MOSI_Pin  , 0) 
#define  MISO     GPIO_Read_Bit(RF2G4_MISO_Port  , RF2G4_MISO_Pin)


//函数声明

uint8_t NRF24L01_Init(void);

uint8_t   nrf_link_check(void);                  //检测Ci24R1+与单片机是否通信正常

void nrf_tx_packet(uint8_t* pBuf, uint8_t len);     //nrf发送

void nrf_handler(void); 

void RCPacket_Send(void);

#endif	/* __NRF24L01_H */
