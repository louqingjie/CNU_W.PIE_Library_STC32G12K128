#include "main.h"
void main()
{
	PWM_Init(PWMB_CH1_P74, 50, 730);
	while(1)
		PWM_SET_Frequency(PWMB_CH1_P74,50, 730);
}