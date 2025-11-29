#include "nrf24l01.h"
#include "CNU_PIE_SPI.h"
#include "string.h"
#include "remote_control.h"
#include "CNU_PIE_EXTI.h"
#include "isr.h"
#include "main.h"

//NRF24L01+״̬
typedef enum
{
  NOT_INIT = 0,
  TX_MODE,
  RX_MODE,
} nrf_mode_e;

uint8_t Channal = 0;
int RecFPS = 0;

#define CHANAL          52                              //Ƶ��ѡ��

uint8_t TX_ADDRESS[5] = {'R', 'C', 'T', 'L', 0 }; //���͵�ַ
uint8_t RX_ADDRESS[5] = {'R', 'C', 'T', 'L', 0 }; //���յ�ַ

uint8_t TX_Buff[TX_PACKET_LENTH];  
uint8_t RX_Buff[RX_PACKET_LENTH];

/******************************** NRF24L01+ �Ĵ������� �궨��***************************************/

// SPI(nRF24L01) commands , NRF��SPI����궨�壬���NRF����ʹ���ĵ�
#define NRF_READ_REG    0x00    // Define read command to register
#define NRF_WRITE_REG   0x20    // Define write command to register
#define RD_RX_PLOAD     0x61    // Define RX payload register address
#define WR_TX_PLOAD     0xA0    // Define TX payload register address
#define FLUSH_TX        0xE1    // Define flush TX register command
#define FLUSH_RX        0xE2    // Define flush RX register command
#define REUSE_TX_PL     0xE3    // Define reuse TX payload register command
#define _NOP            0xFF    // Define No Operation, might be used to read status register

//����Ƶ 
#define CE_ON       0x70
#define CE_OFF      0x71
#define FEATURE     0x1D  //  Feature Register address

// SPI(nRF24L01) registers(addresses) ��NRF24L01 ��ؼĴ�����ַ�ĺ궨��
#define CONFIG      0x00        // 'Config' register address
#define EN_AA       0x01        // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02        // 'Enabled RX addresses' register address
#define SETUP_AW    0x03        // 'Setup address width' register address
#define SETUP_RETR  0x04        // 'Setup Auto. Retrans' register address
#define RF_CH       0x05        // 'RF channel' register address
#define RF_SETUP    0x06        // 'RF setup' register address
#define STATUS      0x07        // 'Status' register address
#define OBSERVE_TX  0x08        // 'Observe TX' register address
#define CD          0x09        // 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A        // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B        // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C        // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D        // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E        // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F        // 'RX address pipe5' register address
#define TX_ADDR     0x10        // 'TX address' register address
#define RX_PW_P0    0x11        // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12        // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13        // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14        // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15        // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16        // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17        // 'FIFO Status Register' register address


//������Ҫ��״̬���
#define TX_FULL     0x01        //TX FIFO �Ĵ�������־�� 1 Ϊ ����0Ϊ ����
#define MAX_RT      0x10        //�ﵽ����ط������жϱ�־λ
#define TX_DS       0x20        //��������жϱ�־λ
#define RX_DR       0x40        //���յ������жϱ�־λ



//�ڲ��Ĵ���������������
void nrf_writereg(uint8_t reg, uint8_t dat);
uint8_t nrf_readreg(uint8_t reg);

void nrf_writebuf(uint8_t reg , uint8_t *pBuf, uint16_t len) reentrant;
void nrf_readbuf (uint8_t reg, uint8_t *pBuf, uint16_t  len) reentrant;

void nrf_rx_mode(void);    //�������ģʽ
void nrf_tx_mode(void);    //���뷢��ģʽ
uint8_t nrf_link_check(void);   //���NRF24L01+�뵥Ƭ���Ƿ�ͨ������
/*!
*  @brief      NRF24L01+ ģʽ���
*/
volatile uint8_t  nrf_mode = NOT_INIT;

// RF2G4��ʼ��
//------------------------------------------------------------------------------------------
uint8_t NRF24L01_Init(void)
{
	uint32_t channal =CHANAL;
	
	GPIO_Init(RF2G4_CE_Port, RF2G4_CE_Pin, GPIO_OUT_PP);
	GPIO_Init(RF2G4_CSN_Port, RF2G4_CSN_Pin, GPIO_OUT_PP);
	
	GPIO_Init(RF2G4_MISO_Port, RF2G4_MISO_Pin, GPIO_HighZ);
	GPIO_PinPullConfig(RF2G4_MISO_Port,RF2G4_MISO_Pin,GPIO_NO_PULL);
	
  GPIO_Init(RF2G4_IRQ_Port, RF2G4_IRQ_Pin, GPIO_HighZ);
	GPIO_EXTI_Init(RF2G4_IRQ_Port,RF2G4_IRQ_Pin,FALLING_EDGE);
	GPIO_EXTI_Open(RF2G4_IRQ_Port,RF2G4_IRQ_Pin);
	GPIO_EXTI_Set_Priority(RF2G4_IRQ_Port,Highest_priority);
	
	
	SPI_Init(SPI_2, 0 , SPI_MSB , SPI_CPOL_Low , SPI_CPHA_1Edge , SPI_Speed_4 , SPI_Mode_Master , 1);
	
	Channal = channal;
	RX_ADDRESS[4] = Channal;
	TX_ADDRESS[4] = Channal;
	
	//2401�Ĵ�������
	RF2G4_CE_LOW; 						 
	
	nrf_writereg(NRF_WRITE_REG + SETUP_AW, ADR_WIDTH - 2);          //���õ�ַ����Ϊ TX_ADR_WIDTH
	
  nrf_writereg(NRF_WRITE_REG + RF_CH, CHANAL);                    //����RFͨ��ΪCHANAL
	
	/*            | NRF24L01      | SI24R1        |Ci24R1 
  *     1Mbps   |  0x06   0dBm  |  0x07    7dBm | 0x07  11dBm
  *     250kbps |  0x26   0dBm  |  0x27    7dBm | 0x27  11dBm
  */  
  nrf_writereg(NRF_WRITE_REG + RF_SETUP, 0x06);                   //����TX�������

 //nrf_writereg(NRF_WRITE_REG + FEATURE, 0x04);	 									//ʹ�ܶ�̬���� SI24R1 Ci24R1 
  
  nrf_writereg(NRF_WRITE_REG + EN_AA, 0x01);                      //ʹ��ͨ��0���Զ�Ӧ��
  
  nrf_writereg(NRF_WRITE_REG + EN_RXADDR, 0x01);                  //ʹ��ͨ��0�Ľ��յ�ַ
  
  //RXģʽ����
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH); //дRX0�ڵ��ַ
	
  nrf_writereg(NRF_WRITE_REG + RX_PW_P0, RX_PACKET_LENTH);         //ѡ��ͨ��0����Ч���ݿ���
	
  nrf_writereg(FLUSH_RX, _NOP);                                //���RX FIFO�Ĵ���
  
  //TXģʽ����
  nrf_writebuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH); 	//дTX�ڵ��ַ
  
  nrf_writereg(NRF_WRITE_REG + SETUP_RETR, 0x05);                 //�����Զ��ط����ʱ��:250us; ����Զ��ط�����:10��
  
  nrf_writereg(FLUSH_TX, _NOP);                                    //���TX FIFO�Ĵ���
  
  nrf_rx_mode();                                                  //Ĭ�Ͻ������ģʽ
	
	RF2G4_CE_HIGH; 		
	
	return nrf_link_check();	
}


// ���NRF24L01+��MCU�Ƿ���������
// return = 0:�ɹ�,	1:ʧ��
//---------------------------------------------------------------------------------------------------
uint8_t nrf_link_check(void)
{
#define NRF_CHECH_DATA  0x06        //��ֵΪУ������ʱʹ�ã����޸�Ϊ����ֵ
  
  uint8_t reg;
  
  uint8_t buff[5] = {NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA};
  uint8_t i;
  //д��У������
  reg = NRF_WRITE_REG + TX_ADDR; 
	
  RF2G4_CE_LOW;
	
  nrf_writebuf(reg ,buff, 5);//д��У������
  
  //��ȡУ������
  reg = TX_ADDR;
  nrf_readbuf(reg ,buff, 5);//��ȡУ������
	
  RF2G4_CE_HIGH;
  /*�Ƚ�*/
  for(i = 0; i < 5; i++)
  {
    if(buff[i] != NRF_CHECH_DATA)
    {
      return 0;          //MCU��NRF����������
    }
  }
  return 1 ;             //MCU��NRF�ɹ�����
}


/*!
*  @brief      NRF24L01+�������ģʽ
*  @since      v5.0
*/
void nrf_rx_mode(void)
{
  RF2G4_CE_LOW;
	
  nrf_writereg(NRF_WRITE_REG + EN_AA, 0x01);          //ʹ��ͨ��0���Զ�Ӧ��
  
  nrf_writereg(NRF_WRITE_REG + EN_RXADDR, 0x01);      //ʹ��ͨ��0�Ľ��յ�ַ
  
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH); //дRX�ڵ��ַ
	
  nrf_writereg(NRF_WRITE_REG + CONFIG, 0x0B | (IS_CRC16 << 2));       //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
  
  /* ����жϱ�־*/
  nrf_writereg(NRF_WRITE_REG + STATUS, _NOP);
  
  nrf_writereg(FLUSH_RX, _NOP);                    //���RX FIFO�Ĵ���
  
  RF2G4_CE_HIGH; 
  
  nrf_mode = RX_MODE;
}

/*!
*  @brief      NRF24L01+���뷢��ģʽ
*  @since      v5.0
*/
void nrf_tx_mode(void)
{
  RF2G4_CE_LOW;           			
  
  nrf_writebuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH); //дTX�ڵ��ַ
  
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH); //����RX�ڵ��ַ ,��ҪΪ��ʹ��ACK
	
  nrf_writereg(NRF_WRITE_REG + CONFIG, 0x0A | (IS_CRC16 << 2)); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
  
  RF2G4_CE_HIGH;   
	
	nrf_mode = TX_MODE;
	
	Ms_Delay(25);
}

//���Ͳ���̫��
void nrf_tx_packet(uint8_t* txbuf, uint8_t len)
{
	uint8_t crc = 0;
	int i ;
	if((txbuf == 0) || len <= 0)return;
  TX_Buff[0] = len + 1; //֡ͷ �ֽڳ���
	memcpy(TX_Buff + 1, txbuf, len); //��ȡ����
	for(i = 0; i < len; i++)
		crc += *(txbuf + i);
	TX_Buff[len + 1] = crc; //֡βУ��
	//����
  if( nrf_mode != TX_MODE)
  {
    nrf_tx_mode();
  }
  RF2G4_CE_LOW;  
  nrf_writebuf(WR_TX_PLOAD, (uint8_t *)TX_Buff, len + 2);
  RF2G4_CE_HIGH;  
}


void nrf_handler(void)
{
  uint8_t state;
  /*��ȡstatus�Ĵ�����ֵ  */
	//RF2G4_CE_LOW;
  state = nrf_readreg(STATUS);

  /* ����жϱ�־*/
  nrf_writereg(NRF_WRITE_REG + STATUS, state);
  //RF2G4_CE_HIGH; 
  if(state & RX_DR) //���յ�����
  { 
    //��ȡ���ݲ����
    RF2G4_CE_LOW;
    nrf_readbuf(RD_RX_PLOAD, RX_Buff, RX_PACKET_LENTH); 
		Rc_unpack_data(RX_Buff);
    RF2G4_CE_HIGH;
  }
  
  if(state & TX_DS) //����������
  {
		//RF2G4_CE_LOW;
    nrf_writereg(FLUSH_TX, _NOP);      //���TX FIFO
		//RF2G4_CE_HIGH;
    if( nrf_mode != RX_MODE)
    {
      nrf_rx_mode();
    }
  }
  
  if(state & MAX_RT)      //���ͳ�ʱ
  {
		//RF2G4_CE_LOW;
    nrf_writereg(FLUSH_TX, _NOP);   //���TX FIFO�Ĵ���
		//RF2G4_CE_HIGH;
    if( nrf_mode != RX_MODE)       //���� ����״̬
    {
      nrf_rx_mode();
    }                                  
  }
  
  if(state & TX_FULL) //TX FIFO ��
  {
    
  }
}

//���ݰ�ѹ������
uint8_t label = 0;
void RCPacket_Send(void)
{
	int i;
	uint8_t crc = 0;
  SendPack_t* pack_t = get_sendpack_point();
  int number = 0;
  for(i = label; i < 3 + label; i++) //��ѹ��
  {
    int pot = number * 10 + 1;
    if(pack_t->Mode[i] == 1) //�ַ���+����
    {
      TX_Buff[pot] = 0;
      TX_Buff[pot] |= (pack_t->line[i].Namelenth << 4) | (pack_t->line[i].Row << 1) | (pack_t->line[i].Size);
      memcpy(TX_Buff + pot + 1, pack_t->line[i].Name, pack_t->line[i].Namelenth); 
      memcpy(TX_Buff + pot + 6, pack_t->line[i].Number, sizeof(float));
      number ++;
    }
    else if(pack_t->Mode[i] == 2) //����+����
    {
      TX_Buff[pot] = 1 << 7;
      TX_Buff[pot] |= (pack_t->line[i].Row << 1) | (pack_t->line[i].Size);
      memcpy(TX_Buff + pot + 1, pack_t->line[i].Number, 2 * sizeof(float)); 
      number ++;
    }
    else if(pack_t->Mode[i] == 3) //����
    {
      TX_Buff[pot] = 0x70;
      TX_Buff[pot] |= (pack_t->line[i].Row << 1) | (pack_t->line[i].Size);
      number ++;
    }
    
    pack_t->Mode[i] = 0; //��ȡ���ݺ�������
  }
  if(number != 0) //������
  {
    TX_Buff[0] = number * 10 + 1; //֡ͷ
    crc = 0;
    for(i = 1; i < TX_Buff[0] ; i++)
      crc += TX_Buff[i];
    TX_Buff[TX_Buff[0]] = crc; //֡β
    if( nrf_mode != TX_MODE)
    {
      nrf_tx_mode();
    }
    //����
    RF2G4_CE_LOW;
    nrf_writebuf(WR_TX_PLOAD, TX_Buff, TX_PACKET_LENTH);
    RF2G4_CE_HIGH;
  }
  if(label)label = 0;
  else label = 3;
}

//----------------------SPIд����----------------------------//
void nrf_writereg(uint8_t reg, uint8_t dat)
{
	RF2G4_CSN_LOW;              // ʹ��SPI����
	
	SPI_ReadWriteByte(reg);		 // ���ͼĴ�����
	SPI_ReadWriteByte(dat);	  // д��Ĵ�����ֵ
	
	RF2G4_CSN_HIGH;              // ��ֹSPI����	 
}


void nrf_writebuf(uint8_t reg, uint8_t *pBuf, uint16_t len) reentrant
{
	uint16_t i;
	RF2G4_CSN_LOW;              // ʹ��SPI����
	
	SPI_ReadWriteByte(reg);		// ���ͼĴ�����
	for(i = 0; i < len; i++)
		SPI_ReadWriteByte(*(pBuf + i));
	
	RF2G4_CSN_HIGH;              // ��ֹSPI����	 
}

//----------------------SPI������----------------------------//
uint8_t nrf_readreg(uint8_t reg)
{
	uint8_t value;
	RF2G4_CSN_LOW;          			// ʹ��SPI����		
	
	SPI_ReadWriteByte(reg);   			// ���ͼĴ�����
	value = SPI_ReadWriteByte(0xFF);		// ��ȡ�Ĵ�������
	
	RF2G4_CSN_HIGH;          			// ��ֹSPI����		
	
	return value;
}

void nrf_readbuf(uint8_t reg, uint8_t *pBuf, uint16_t len) reentrant
{
	uint16_t i;
	RF2G4_CSN_LOW;           			// ʹ��SPI����

	SPI_ReadWriteByte(reg);		// ���ͼĴ���ֵ(λ��),����ȡ״ֵ̬

	for(i = 0; i < len; i++)
		*(pBuf + i) = SPI_ReadWriteByte(0xFF);	// ��������

	RF2G4_CSN_HIGH;       				// �ر�SPI����
}
		
void P2_INT_ISR_Handler (void) interrupt P2INT_VECTOR
{
  GPIO_EXTI_Flag_Read(GPIO_P2);   //��־λ��ֵ+��ձ�־λ
	if(Port_Exti_Flag[2])
	{
	GPIO_EXTI_Flag_Clear(GPIO_P2);
	if(Port_Exti_Flag[2] & Port_Pin_0)
	{
		//P2.0�ж�
	}
	if(Port_Exti_Flag[2] & Port_Pin_1)
	{
		//P2.1�ж�
	}
	if(Port_Exti_Flag[2] & Port_Pin_2)
	{
		//P2.2�ж�
	}
	if(Port_Exti_Flag[2] & Port_Pin_3)
	{
		//P2.3�ж�
	}
	if(Port_Exti_Flag[2] & Port_Pin_4)
	{
		//P2.4�ж�
	}
	if(Port_Exti_Flag[2] & Port_Pin_5)
	{
		//P2.5�ж�
	}
	if(Port_Exti_Flag[2] & Port_Pin_6)
	{
		//P2.6�ж�
		nrf_handler();
	}
	if(Port_Exti_Flag[2] & Port_Pin_7)
	{
		//P2.7�ж�
	}
  } 
}


