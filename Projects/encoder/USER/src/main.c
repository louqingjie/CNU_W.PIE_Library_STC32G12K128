#include "main.h"
#define MOTOR_L PWMA_CH2P_P62
#define MOTOR_R PWMA_CH3P_P64
#define MOTOR_BASE_L PWMA_CH1P_P60
#define MOTOR_BASE_R PWMA_CH4P_P66
float    kp = 2, ki = 0.2, kd = 0;
uint16_t speed = 1000, maxSpeed = 4000;
int      output, error, lastError, lastLastError, duty;
uint16_t encNum;
char     encLine[16];
float    PID_Incremental_Calculate(float error, float lastError, float lastLastError);
void     main(void)
{
    Board_Init();
    GPIO_EXTI_Init(GPIO_P0, GPIO_Pin_4, FALLING_EDGE);
    GPIO_EXTI_Open(GPIO_P0, GPIO_Pin_4);
    PWM_Init(MOTOR_L, 10000, 10000);
    PWM_Init(MOTOR_R, 10000, 10000);
    PWM_Init(MOTOR_BASE_L, 10000, 10000);
    PWM_Init(MOTOR_BASE_R, 10000, 10000);
    LCD_Init();
    PIT_Timer_Ms(TIM0, 20);
    while (1) {
        error  = speed - encNum * 10;
        output = (int)PID_Incremental_Calculate(error, lastError, lastLastError);
        duty   = speed + 10*output;
        if (duty > 4000)
            duty = 4000;
        if (duty < 0)
            duty = 0;
        PWM_SET_Duty(MOTOR_L, 10000 - duty);
        PWM_SET_Duty(MOTOR_R, 10000 - duty);
        sprintf(encLine, "%04d %+d    ", encNum, output);
        LCD_P8x16Str(0, 3, encLine);
        sprintf(encLine, "%05d    ", duty);
        LCD_P8x16Str(0, 1, encLine);
        lastLastError = lastError;
        lastError     = error;
        encNum        = 0;
        Ms_Delay(20);
    }
}

float PID_Incremental_Calculate(float error, float lastError, float lastLastError)
{
    float pTerm, iTerm, dTerm, output;

    pTerm = kp * (error - lastError);
    iTerm = ki * error;
    dTerm = kd * (error - 2 * lastError + lastLastError);

    output = pTerm + iTerm + dTerm;

    return output;
}

void P0_EXTI_Activated() interrupt P0INT_VECTOR
{
    GPIO_EXTI_Flag_Read(GPIO_P0);
    if (Port_Exti_Flag[0]) {
        GPIO_EXTI_Flag_Clear(GPIO_P0);
        if (Port_Exti_Flag[0] & Port_Pin_4) {
            encNum++;
        }
    }
}

void PIT0_Activated() interrupt TMR0_VECTOR
{
    PIT_Timer_Clear(TIM0);
}