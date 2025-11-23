#include "main.h"
void main()
{
	Board_Init(); // 开发板初始化
	PWM_Init(PWMB_CH1_P74, 50, 730);
	while(1)
		PWM_SET_Duty(PWMB_CH1_P74, 730);
}