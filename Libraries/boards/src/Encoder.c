#include "Encoder.h"
#include "CNU_PIE_TIMER.h"
#include "CNU_PIE_GPIO.h"

#define Encoder_Dir   P52  //编码器方向引脚定义 
#define Encoder_Tim   P04  //编码器计数引脚定义 

Encoder_TypeDef Encoder_X;

void Encoder_Init(void)
{
	Timer_Count_Init(TIMER3_P04);//编码器脉冲引脚捕获引脚初始化
	
	GPIO_Init(GPIO_P5 , GPIO_Pin_2 , GPIO_PullUp);
}

int Encoder_Count_Read(void)
{
	Encoder_X.pouse = Timer_Count_Read(TIMER3_P04); 
	
	if(Encoder_Dir==0) Encoder_X.pouse_t = Encoder_X.pouse_t - (Encoder_X.pouse - Encoder_X.pouse_last);
	                else Encoder_X.pouse_t = Encoder_X.pouse_t + (Encoder_X.pouse - Encoder_X.pouse_last);
	
  Encoder_X.pouse_last = Encoder_X.pouse;
	
	return Encoder_X.pouse_t;
}

void Encoder_Clear(void)
{
	Timer_Count_Clear(TIMER3_P04);
	Encoder_X.pouse = 0;
	Encoder_X.pouse_last = 0;
	Encoder_X.pouse_t = 0;
}