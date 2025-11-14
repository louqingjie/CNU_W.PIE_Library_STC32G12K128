/*
 * remote_control.h
 *
 *  Created on: 2020年4月5日
 *      Author: 肖时有
 */

#ifndef __REMOTE_CONTROL_H_     
#define __REMOTE_CONTROL_H_     

#include "common.h"

// 按键序号
//--------------------------------------------------------------
typedef enum
{
  
  KEY_OFFSET_WKUP        =      0, 	// 唤醒键
  KEY_OFFSET_1	         =      1, 	// 按键1
  KEY_OFFSET_UP	         =      2, 	// 上
  KEY_OFFSET_DOWN        =      3, 	// 下
  KEY_OFFSET_LEFT        =      4, 	// 左
  KEY_OFFSET_RIGHT       =      5, 	// 右
  KEY_OFFSET_A	         =      6, 	// A
  KEY_OFFSET_B 	         =      7, 	// B
  KEY_OFFSET_C	         =      8, 	// C
  KEY_OFFSET_D 	         =      9, 	// D
  KEY_OFFSET_Rocker11    =      10,	// 摇杆1的按键 
  KEY_OFFSET_Rocker21    =      11,	// 摇杆2的按键
  
  KEY_RCDISCONNECTED     =      15,     //遥控器连接状态
  
}KEY_OFFSET_t;

//摇杆序号
//--------------------------------------------------------------
typedef enum
{
  
  ROCKER_LEFT_VERTICAL    =      0,         //左摇杆竖直方向
  ROCKER_LEFT_HORIZONTAL  =      1,         //左摇杆水平方向
  ROCKER_RIGHT_VERTICAL   =      2,         //右摇杆竖直方向
  ROCKER_RIGHT_HORIZONTAL =      3,         //右摇杆水平方向
  
}ROCKER_OFFSET_t;


//遥控器接收协议包结构体
#pragma pack(1)//一字节对齐
typedef struct
{
	#pragma pack(1)//一字节对齐
  struct 
  {	
    int16_t value[4];		
  }rocker;
	#pragma  pack()
  
	#pragma pack(1)//一字节对齐
  struct
  {	
    uint16_t value;	
  }key;
	#pragma  pack()
  
}RC_ctrl_t;
#pragma  pack()

//行显示数据
typedef struct LineText_t 
{
  char Name[5];
  uint8_t Namelenth;
  float Number[2];
  uint8_t Row;
  uint8_t Size;
}LineText_t;

//发送数据包
typedef struct SendPack_t
{
  LineText_t line[6];
  uint8_t Mode[6]; //0：不发送  1：字符串（小于等于5）+数字 2：数字 + 数字 3：清行
}SendPack_t;


/*
*遥控器初始化
*/
void remote_control_init(void); 

/*
*@brief 获取遥控器按键值
*@param 按键序号 
*@return 按下返回1 松开返回0 输入错误返回-1
*/
int8_t RcKeyValueRead(KEY_OFFSET_t offset);

/*
*@brief 获取遥控器摇杆adc采集值
*@param  摇杆序号
*@return 返回对应方向adc采集值
*/
int16_t RcRockerValueRead(ROCKER_OFFSET_t offset);

/*
*@brief 将数据以及名字显示在对应行数
*@param 显示名字首地址
*@param 显示名字长度 最大为5个字符 
*@param 显示数据
*@param 位于遥控器的行数 0-5 共6行 否则无效
*@param 显示大小 (0) 以6*8大小显示 (!0)以8*16大小显示
*@example Send_Data("abcd", 3, 2.55, 0, 0); //将长度为3的字符串 abc 和 数字 2.55 以6*8从第0行显示
*/
void ShowStringData(char* name_t, uint8_t namelenth, float num, uint8_t row, uint8_t Size);

/*
*@brief 将两个浮点型数据显示在对应行数
*@param 显示数据左
*@param 显示数据右
*@param 位于遥控器的行数 0-5 共6行 否则无效
*@param 显示大小 (0) 以6*8大小显示 (!0)以8*16大小显示
*@example ShowData(1.0, 0.2, 0, 1); //将数字 1 和 0.2 以 8*16 的大小从第0行显示
*/
void ShowData(float numleft, float numright, uint8_t row, uint8_t Size);


/*
*@brief 清除某一行显示内容
*@param 位于遥控器的行数 0-5 共6行 否则无效
*@param 大小 （0）一次清除一行 （！0）一次清除两行
*@example ShowLineClear(0, 1);  //清除第0,1行的显示内容
*/
void ShowLineClear(uint8_t row, uint8_t Size);



//下方函数用户无需调用
uint8_t Rc_unpack_data(uint8_t* data_t); //协议包解析
SendPack_t* get_sendpack_point(void);

#endif      //__REMOTE_CONTROL_H_     
