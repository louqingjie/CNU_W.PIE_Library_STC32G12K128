#include "main.h"
#define MOTOR_L PWMA_CH2P_P62
#define MOTOR_R PWMA_CH3P_P64
#define MOTOR_BASE_L PWMA_CH1P_P60
#define MOTOR_BASE_R PWMA_CH4P_P66
uint32_t encNum[2];
uint32_t aveNum[20];
char     encLine[16];
void     main(void)
{
    Board_Init();
    GPIO_EXTI_Init(GPIO_P0, GPIO_Pin_4, FALLING_EDGE);
    GPIO_EXTI_Open(GPIO_P0, GPIO_Pin_4);
    GPIO_EXTI_Init(GPIO_P5, GPIO_Pin_2, RISING_EDGE);
    GPIO_EXTI_Open(GPIO_P5, GPIO_Pin_2);
    PWM_Init(MOTOR_L, 10000, 10000);
    PWM_Init(MOTOR_R, 10000, 10000);
    PWM_Init(MOTOR_BASE_L, 10000, 10000);
    PWM_Init(MOTOR_BASE_R, 10000, 10000);
    LCD_Init();
    PIT_Timer_Ms(TIM0, 20);
    while (1) {
        PWM_SET_Duty(MOTOR_L, 8000);
        PWM_SET_Duty(MOTOR_R, 8000);
        sprintf(encLine, "%04d  %04d", encNum[0], encNum[1]);
        LCD_P8x16Str(0, 3, encLine);
        encNum[0] = 0;
        encNum[1] = 0;
        Ms_Delay(200);
    }
}

void P0_EXTI_Activated() interrupt P0INT_VECTOR
{
    GPIO_EXTI_Flag_Read(GPIO_P0);
    if (Port_Exti_Flag[0]) {
        GPIO_EXTI_Flag_Clear(GPIO_P0);
        if (Port_Exti_Flag[0] & Port_Pin_4) {
            encNum[0]++;
        }
    }
}

void P5_EXTI_Activated() interrupt P5INT_VECTOR
{
    GPIO_EXTI_Flag_Read(GPIO_P5);
    if (Port_Exti_Flag[5]) {
        GPIO_EXTI_Flag_Clear(GPIO_P5);
        if (Port_Exti_Flag[5] & Port_Pin_2) {
            encNum[1]++;
        }
    }
}

void PIT0_Activated() interrupt TMR0_VECTOR
{
    PIT_Timer_Clear(TIM0);
}