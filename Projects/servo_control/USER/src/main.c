#include "main.h"
#define SERVO PWMB_CH1_P74
float i = 0;
void  Servo_Control_By_Error(float errorInner, uint16_t midDutyOfServo, uint16_t maxChangeDuty)
{
    // 误差限幅
    if (errorInner > 1)
        errorInner = 1;
    else if (errorInner < -1)
        errorInner = -1;

    // 根据误差计算舵机PWM值：中位值 ± 最大变化量×误差
    PWM_SET_Duty(SERVO, midDutyOfServo + (int)maxChangeDuty * errorInner);
}
void main()
{
    Board_Init();
    PWM_Init(PWMB_CH1_P74, 50, 730);
    while (1) {
        // PWM_SET_Duty(PWMB_CH1_P74, 830);
        // Ms_Delay(1000);
        // PWM_SET_Duty(PWMB_CH1_P74, 630);
        // Ms_Delay(1000);
        for (i = 0; i < 1; i += 0.01) {
            Servo_Control_By_Error(i, 730, 130);
            Ms_Delay(10);
        }
        for (i = 1; i > -1; i -= 0.01) {
            Servo_Control_By_Error(i, 730, 130);
            Ms_Delay(10);}
            for (i = -1; i < 0; i += 0.01) {
                Servo_Control_By_Error(i, 730, 130);
                Ms_Delay(10);
            }
        }
    }
