/*
* remote_control.c
*
*  Created on: 2020年4月5日
*      Author: 肖时有
*/
#include "CNU_PIE_TIMER.h"
#include "remote_control.h"
#include "nrf24l01.h"
#include "main.h"

RC_ctrl_t rc_ctrl; //遥控器实体化
SendPack_t sendpack; //发送数据实体化

//遥控器初始化
void remote_control_init(void)
{  
  //Ci24R1初始化
  while(!NRF24L01_Init());
  // NRF24L01_Init();

  memset(&sendpack, 0, sizeof(SendPack_t));
  
  //初始化结束开启中断
  //PIT4中断配置 1ms中断
	
  //PIT_Timer_Ms(TIM4, 1);
	
	 Ms_Delay(200);
}

//遥控器协议解包
uint8_t Rc_unpack_data(uint8_t* data_t)
{
	int i ;
	uint8_t check = 0;
  if(data_t[0] != 11) return 0; //帧头校验失败
  for(i = 1; i < 11; i++)
    check += data_t[i];
  if(check != data_t[11]) return 0;//帧尾校验失败
   
  //解包
  rc_ctrl.rocker.value[0] = (int16_t)(data_t[1] | data_t[2] << 8);
  rc_ctrl.rocker.value[1] = (int16_t)(data_t[3] | data_t[4] << 8);
  rc_ctrl.rocker.value[2] = (int16_t)(data_t[5] | data_t[6] << 8);
  rc_ctrl.rocker.value[3] = (int16_t)(data_t[7] | data_t[8] << 8);
  rc_ctrl.key.value = (uint16_t)(data_t[9] | data_t[10] << 8);
  return 1;
}


/*
*@brief 将数据以及名字显示在对应行数
*@param 显示名字首地址
*@param 显示名字长度 最大为5个字符 
*@param 显示数据
*@param 位于遥控器的行数 0-5 共6行 否则不显示
*@param 显示大小 (0) 以6*8大小显示 (!0)以8*16大小显示
*@example ShowStringData("abc", 3, 2.55, 0, 1);
*/
void ShowStringData(char* name_t, uint8_t namelenth, float num, uint8_t row, uint8_t Size)
{
  if(row > 5 || (name_t == 0 && namelenth != 0) || sendpack.Mode[row])return;
  if(namelenth > 5)namelenth = 5;
  memcpy(sendpack.line[row].Name, name_t, namelenth);
  sendpack.line[row].Namelenth = namelenth;
  sendpack.line[row].Number[0] = num;
  sendpack.line[row].Row = row;
  sendpack.line[row].Size = Size ? 1 : 0;
  sendpack.Mode[row] = 1;
}


/*
*@brief 将两个浮点型数据显示在对应行数
*@param 显示数据左
*@param 显示数据右
*@param 位于遥控器的行数 0-5 共6行 否则不显示
*@param 显示大小 (0) 以6*8大小显示 (!0)以8*16大小显示
*@example ShowData(1.0, 0.2, 0, 1); 
*/
void ShowData(float numleft, float numright, uint8_t row, uint8_t Size)
{
  if(row > 5 || sendpack.Mode[row])return;
  sendpack.line[row].Number[0] = numleft;
  sendpack.line[row].Number[1] = numright;
  sendpack.line[row].Row = row;
  sendpack.line[row].Size = Size ? 1 : 0;
  sendpack.Mode[row] = 2;
}

/*
*@brief 清除某一行显示内容
*@param 位于遥控器的行数 0-5 共6行 否则无效
*@param 大小 （0）一次清除一行 （！0）一次清除两行
*@example ShowLineClear(0, 1);  //清除第0,1行的显示内容
*/
void ShowLineClear(uint8_t row, uint8_t Size)
{
  if(row > 5)return;
  sendpack.line[row].Row = row;
  sendpack.line[row].Size = Size ? 1 : 0;
  sendpack.Mode[row] = 3;
}

/*
*@brief 获取遥控器按键值
*@param 按键序号 
*@return 按下返回1 松开返回0 输入错误返回-1
*/
int8_t RcKeyValueRead(KEY_OFFSET_t offset)
{
  if(offset > 15)return -1;
  return (rc_ctrl.key.value & (1 << offset)) ? 1 : 0;
}

/*
*@brief 获取遥控器摇杆adc采集值
*@param  摇杆序号
*@return 返回对应方向adc采集值
*/
int16_t RcRockerValueRead(ROCKER_OFFSET_t offset)
{
  return rc_ctrl.rocker.value[offset];
}



//中断回调函数
static uint32_t timeline = 0; //时间线
uint8_t Clear_Time = 0; //遥控器断开清零计数
float Offset = 0; //异步传输偏移量
void TM4_Isr() interrupt 20
{
	PIT_Timer_Clear(TIM4);
  timeline++;
  if((timeline % 20) == 0) //20ms 发送一次
  {
    RCPacket_Send();
  }
  Clear_Time++;
  if(Clear_Time >= 50) //50ms未接收到遥控器数据则清除所有信息
  {
    Clear_Time = 0;
    memset(&rc_ctrl, 0, sizeof(rc_ctrl));
    rc_ctrl.key.value |= 1 << KEY_RCDISCONNECTED;
  }
}
//发送数据指针
SendPack_t* get_sendpack_point(void)
{
  return &sendpack;
}
