/*
* Ci24R1.c
*
*  Created on: 2020年4月5日
*      Author: 肖时有
*/

#include "CNU_PIE_GPIO.h"
#include "Ci24R1.h"
#include "remote_control.h"

//NRF24L01+状态
typedef enum
{
  NOT_INIT = 0,
  TX_MODE,
  RX_MODE,
} nrf_mode_e;


#define CHANAL          1                              //频道选择

uint8_t TX_ADDRESS[5] = {'R', 'C', 'T', 'L', CHANAL }; //发送地址 0x52 0x43 0x54 0x4C
uint8_t RX_ADDRESS[5] = {'R', 'C', 'T', 'L', CHANAL }; //接收地址 0x52 0x43 0x54 0x4C

uint8_t TX_Buff[TX_PACKET_LENTH];  
uint8_t RX_Buff[RX_PACKET_LENTH];

/******************************** NRF24L01+ 寄存器命令 宏定义***************************************/

// SPI(nRF24L01) commands , NRF的SPI命令宏定义，详见NRF功能使用文档
#define NRF_READ_REG    0x00    // Define read command to register
#define NRF_WRITE_REG   0x20    // Define write command to register
#define RD_RX_PLOAD     0x61    // Define RX payload register address
#define WR_TX_PLOAD     0xA0    // Define TX payload register address
#define FLUSH_TX        0xE1    // Define flush TX register command
#define FLUSH_RX        0xE2    // Define flush RX register command
#define REUSE_TX_PL     0xE3    // Define reuse TX payload register command
#define _NOP            0xFF    // Define No Operation, might be used to read status register

//单射频 
#define CE_ON       0x70
#define CE_OFF      0x71
#define FEATURE     0x1D  //  Feature Register address

// SPI(nRF24L01) registers(addresses) ，NRF24L01 相关寄存器地址的宏定义
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


//几个重要的状态标记
#define TX_FULL     0x01        //TX FIFO 寄存器满标志。 1 为 满，0为 不满
#define MAX_RT      0x10        //达到最大重发次数中断标志位
#define TX_DS       0x20        //发送完成中断标志位
#define RX_DR       0x40        //接收到数据中断标志位



//内部寄存器操作函数声明
static void spi_wbyte(uint8_t byte);
static uint8_t spi_rbyte();
static void nrf_writecmd(uint8_t cmd);
static void nrf_writereg(uint8_t reg, uint8_t dat);
static uint8_t nrf_readreg (uint8_t reg);

static void nrf_writebuf(uint8_t reg , uint8_t *pBuf, uint16_t len);
static void nrf_readbuf (uint8_t reg, uint8_t *pBuf, uint16_t  len);
uint8_t nrf_readirq(uint8_t reg);

static void nrf_rx_mode(void);  //进入接收模式
static void nrf_tx_mode(void);  //进入发送模式

/*!
*  @brief      NRF24L01+ 模式标记
*/
volatile uint8_t  nrf_mode = NOT_INIT;

uint8_t Ci24R1_Init(void)
{
  //引脚配置
  GPIO_Init(NRF_CS_PORT  ,NRF_CS_PIN  ,GPIO_OUT_PP);
	GPIO_Init(NRF_CLK_PORT ,NRF_CLK_PIN ,GPIO_OUT_PP);
	GPIO_Init(NRF_DATA_PORT,NRF_DATA_PIN,GPIO_OUT_PP);
	
  nrf_writecmd(CE_OFF);
  
  nrf_writereg(NRF_WRITE_REG + SETUP_AW, ADR_WIDTH - 2);          //设置地址长度为 TX_ADR_WIDTH
  nrf_writereg(NRF_WRITE_REG + RF_CH, CHANAL);                    //设置RF通道为CHANAL
  
  /*            | NRF24L01      | SI24R1        |Ci24R1 
  *     1Mbps   |  0x06   0dBm  |  0x07    7dBm | 0x07  11dBm
  *     250kbps |   无          |  0x27    7dBm | 0x27  11dBm
  */            
  nrf_writereg(NRF_WRITE_REG + RF_SETUP, 0x07);                   //设置TX发射参数,
  
  nrf_writereg(NRF_WRITE_REG + FEATURE, 0x04);	 	 //使能动态负载 SI24R1 Ci24R1 
  
  nrf_writereg(NRF_WRITE_REG + EN_AA, 0x01);                      //使能通道0的自动应答
  
  nrf_writereg(NRF_WRITE_REG + EN_RXADDR, 0x01);                  //使能通道0的接收地址
  
  //RX模式配置
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH); //写RX节点地址
  
  nrf_writereg(NRF_WRITE_REG + RX_PW_P0, RX_PACKET_LENTH);         //选择通道0的有效数据宽度
  
  nrf_writereg(FLUSH_RX, _NOP);                                   //清除RX FIFO寄存器
  
  //TX模式配置
  nrf_writebuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH);   //写TX节点地址
  
  nrf_writereg(NRF_WRITE_REG + SETUP_RETR, 0x05);                 //设置自动重发间隔时间:250us; 最大自动重发次数:5次
  
  nrf_writereg(FLUSH_TX, _NOP);                                   //清除TX FIFO寄存器
  
  nrf_rx_mode();                                                  //默认进入接收模式
  
  nrf_writecmd(CE_ON);
  
  return nrf_link_check();
}


/*!
*  @brief      检测Ci24R1+与MCU是否正常连接
*  @return     NRF24L01+ 的通信状态，0表示通信不正常，1表示正常
*  @since      v5.0
*  Sample usage:
while(Ci24R1_link_check() == 0)
{
printf("\n通信失败");
		}
*/
uint8_t nrf_link_check(void)
{
#define NRF_CHECH_DATA  0x06        //此值为校验数据时使用，可修改为其他值
  
  uint8_t reg;
  
  uint8_t buff[5] = {NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA};
  uint8_t i;
  //写入校验数据
  reg = NRF_WRITE_REG + TX_ADDR; 
  
  nrf_writebuf(reg ,buff, 5);//写入校验数据
  
  //读取校验数据
  reg = TX_ADDR;
  nrf_readbuf(reg ,buff, 5);//读取校验数据
  
  /*比较*/
  for(i = 0; i < 5; i++)
  {
    if(buff[i] != NRF_CHECH_DATA)
    {
      return 0;        //MCU与NRF不正常连接
    }
  }
  return 1 ;             //MCU与NRF成功连接
}

/*!
*  @brief      NRF24L01+进入接收模式
*  @since      v5.0
*/
void nrf_rx_mode(void)
{
  nrf_writecmd(CE_OFF);
  
  nrf_writereg(NRF_WRITE_REG + EN_AA, 0x01);          //使能通道0的自动应答
  
  nrf_writereg(NRF_WRITE_REG + EN_RXADDR, 0x01);      //使能通道0的接收地址
  
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH); //写RX节点地址
  
  
  nrf_writereg(NRF_WRITE_REG + CONFIG, 0x0B | (IS_CRC16 << 2));       //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
  
  /* 清除中断标志*/
  nrf_writereg(NRF_WRITE_REG + STATUS, 0xff);
  
  nrf_writereg(FLUSH_RX, _NOP);                    //清除RX FIFO寄存器
  
  nrf_writecmd(CE_ON); //拉高CE
  
  nrf_mode = RX_MODE;
}

/*!
*  @brief      NRF24L01+进入发送模式
*  @since      v5.0
*/
void nrf_tx_mode(void)
{
  nrf_writecmd(CE_OFF);
  
  nrf_writebuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH); //写TX节点地址
  
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH); //设置RX节点地址 ,主要为了使能ACK
  
  nrf_writereg(NRF_WRITE_REG + CONFIG, 0x0A | (IS_CRC16 << 2)); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断
  
  nrf_writecmd(CE_ON);
  
  nrf_mode = TX_MODE;
  
  NRF_DELAY(25);
}


//发送数据
void nrf_tx(char* txbuf, uint16_t len)
{  
	int i = 0;
	uint8_t crc = 0;
  if((txbuf == NULL) || len <= 0)return;
  if(len > 30) len = 30;
  TX_Buff[0] = len + 1; //帧头为字节数
  memcpy(TX_Buff + 1, txbuf, len); //获取数据
  crc = 0;
  for(i = 0; i < len; i++)
    crc += *(txbuf + i);
  TX_Buff[len + 1] = crc; //帧尾校验
  if( nrf_mode != TX_MODE)
  {
    nrf_tx_mode();
  }
  //发送
  nrf_writecmd(CE_OFF);
  nrf_writebuf(WR_TX_PLOAD, TX_Buff, len + 2);
  nrf_writecmd(CE_ON);
}


uint8_t nrf_handler(void)
{
  uint8_t state, ret = 0;
  /*读取status寄存器的值  */
  state = nrf_readreg(STATUS);
  
  /* 清除中断标志*/
  nrf_writereg(NRF_WRITE_REG + STATUS, state);
  
  if(state & RX_DR) //接收到数据
  { 
    //读取数据并解包
    nrf_writecmd(CE_OFF);
    nrf_readbuf(RD_RX_PLOAD, RX_Buff, RX_PACKET_LENTH); 
    nrf_writecmd(CE_ON);
    Rc_unpack_data(RX_Buff);
    ret = 1;
  }
  
  if(state & TX_DS) //发送完数据
  {         
    if( nrf_mode != RX_MODE)
    {
      nrf_rx_mode();
    }
  }
  
  if(state & MAX_RT)      //发送超时
  {
    nrf_writereg(FLUSH_TX, _NOP);   //清除TX FIFO寄存器
    if( nrf_mode != RX_MODE)       //进入 接收状态
    {
      nrf_rx_mode();
    }                                  
  }
  
  if(state & TX_FULL) //TX FIFO 满
  {
    
  }
  return ret;
}

//数据包压缩后发送
uint8_t label = 0;
void RCPacket_Send(void)
{
	int i;
	uint8_t crc = 0;
  SendPack_t* pack_t = get_sendpack_point();
  int number = 0;
  for(i = label; i < 3 + label; i++) //包压缩
  {
    int pot = number * 10 + 1;
    if(pack_t->Mode[i] == 1) //字符串+数字
    {
      TX_Buff[pot] = 0;
      TX_Buff[pot] |= (pack_t->line[i].Namelenth << 4) | (pack_t->line[i].Row << 1) | (pack_t->line[i].Size);
      memcpy(TX_Buff + pot + 1, pack_t->line[i].Name, pack_t->line[i].Namelenth); 
      memcpy(TX_Buff + pot + 6, pack_t->line[i].Number, sizeof(float));
      number ++;
    }
    else if(pack_t->Mode[i] == 2) //数字+数字
    {
      TX_Buff[pot] = 1 << 7;
      TX_Buff[pot] |= (pack_t->line[i].Row << 1) | (pack_t->line[i].Size);
      memcpy(TX_Buff + pot + 1, pack_t->line[i].Number, 2 * sizeof(float)); 
      number ++;
    }
    else if(pack_t->Mode[i] == 3) //清行
    {
      TX_Buff[pot] = 0x70;
      TX_Buff[pot] |= (pack_t->line[i].Row << 1) | (pack_t->line[i].Size);
      number ++;
    }
    
    pack_t->Mode[i] = 0; //获取数据后清楚标记
  }
  if(number != 0) //包发送
  {
    TX_Buff[0] = number * 10 + 1; //帧头
    crc = 0;
    for(i = 1; i < TX_Buff[0] ; i++)
      crc += TX_Buff[i];
    TX_Buff[TX_Buff[0]] = crc; //帧尾
    if( nrf_mode != TX_MODE)
    {
      nrf_tx_mode();
    }
    //发送
    nrf_writecmd(CE_OFF);
    nrf_writebuf(WR_TX_PLOAD, TX_Buff, TX_PACKET_LENTH);
    nrf_writecmd(CE_ON);
  }
  if(label)label = 0;
  else label = 3;
}


//------------------SPI读写操作------------------------//
void spi_wbyte(uint8_t byte)
{
	uint8_t i;
  for(i = 0; i < 8; i++)
  {
    SCK_LOW;
    DATA_SET(byte & 0x80);
    byte <<= 1;
    SCK_HIGH;
  }
}

uint8_t spi_rbyte()
{
  uint8_t byte = 0;
	uint8_t i;
  for(i = 0; i < 8; i++)
  {
    byte <<= 1;
    SCK_HIGH;
    byte |= DATA_READ;
    SCK_LOW;
  }
  return byte;
}

void nrf_writecmd(uint8_t cmd)
{
  SCK_LOW;
  CS_LOW;
  DATA_OUT;
  spi_wbyte(cmd);
  SCK_LOW;
  CS_HIGH;
}

void nrf_writereg(uint8_t reg, uint8_t dat)
{
  SCK_LOW;
  CS_LOW;
  DATA_OUT;
  spi_wbyte(reg);
  spi_wbyte(dat);
  SCK_LOW;
  CS_HIGH;
}


uint8_t nrf_readreg(uint8_t reg)
{
	uint8_t byte;
  SCK_LOW;
  CS_LOW;
  DATA_OUT;
  spi_wbyte(reg);
  SCK_LOW;
  DATA_IN;
  byte = spi_rbyte();
  SCK_LOW;
  CS_HIGH;
  return byte;
}


/*!
*  @brief      NRF24L01+写寄存器一串数据
*  @param      reg         寄存器
*  @param      pBuf        需要写入的数据缓冲区
*  @param      len         需要写入数据长度
*  @return     NRF24L01+ 状态
*  @since      v5.0
*  Sample usage:    nrf_writebuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //写TX节点地址
*/
void nrf_writebuf(uint8_t reg, uint8_t *pBuf, uint16_t len)
{
	int i;
  SCK_LOW;
  CS_LOW;
  DATA_OUT;
  spi_wbyte(reg);
  SCK_LOW;
  for(i = 0; i < len; i++)
  spi_wbyte(*(pBuf + i));
  SCK_LOW;
  CS_HIGH;
}


/*!
*  @brief      NRF24L01+读寄存器一串数据
*  @param      reg         寄存器
*  @param      dat         需要读取的数据的存放地址
*  @param      len         需要读取的数据长度
*  @return     NRF24L01+ 状态
*  @since      v5.0
*  Sample usage:
uint8 data;
nrf_readreg(STATUS,&data);
*/
void nrf_readbuf(uint8_t reg, uint8_t *pBuf, uint16_t len)
{
	int i ;
  SCK_LOW;
  CS_LOW;
  DATA_OUT;
  spi_wbyte(reg);
  SCK_LOW;
  DATA_IN;
  for(i = 0; i < len; i++)
    *(pBuf + i) = spi_rbyte();
  SCK_LOW;
  CS_HIGH;
}
