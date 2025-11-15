#include "main.h"
#include <MATH.H>

// 参数区
float    kp             = 0.5;
float    ki             = 0.1;
float    kd             = 0.1;
float    maxAbsI        = 5;  // 最大积分量
uint8_t  timerTimes     = 5;  // 计时器触发次数
uint8_t  timerMs        = 10; // 计时器触发时间
uint16_t maxSpeed       = 1200;
uint16_t minSpeed       = 1100;
uint16_t midDutyOfServo = 730;
uint16_t maxChangeDuty  = 130;
uint8_t  adcPin[4]      = {0, 1, 6, 7};

// 定义区
char ADC[8] = {ADC_P13, ADC_P16, ADC_P10, ADC_P11,
               ADC_P02, ADC_P05, ADC_P00, ADC_P01};
#define SERVO PWMB_CH1_P74
#define MOTOR_L PWMA_CH2P_P62
#define MOTOR_R PWMA_CH3P_P64
#define MOTOR_BASE_L PWMA_CH1P_P60
#define MOTOR_BASE_R PWMA_CH4P_P66
char LEDPin[4] = {GPIO_Pin_7, GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_4};
#define LED(x) GPIO_P3, LEDPin[x]
char KEYPin[2] = {GPIO_Pin_7, GPIO_Pin_6};
#define KEY(x) GPIO_P0, KEYPin[x]
#define DPAD_UP GPIO_P4, GPIO_P2
#define DPAD_DOWN GPIO_P4, GPIO_Pin_6
#define DPAD_LEFT GPIO_P4, GPIO_Pin_5
#define DPAD_RIGHT GPIO_P4, GPIO_Pin_1
#define DPAD_MID GPIO_P2, GPIO_P7
#define BUZZER PWMB_CH3_P33

// 全局变量
uint16_t adcMaxOut[4] = {20, 0, 0, 0}, adcMinOut[4] = {1000, 1000, 1000, 1000}, adcRaw[4], biosKey = 0, enter;
uint8_t  times;
float    currentError, pidOut, lastError;
char     limLine[22];

// 声明区
void  All_Init();
void  beep(uint16_t freq, uint16_t duty, uint16_t beepTime, uint16_t sleepTime);
void  ADC_Norm_Slow(uint16_t *adcMax, uint16_t *adcMin);
void  ADC_Norm_Fast();
void  Get_ADC();
float Normalization(uint16_t *adcRaw, uint16_t *adcMax, uint16_t *adcMin);
float PID_Calculate(float error, float lastError, float kp, float ki, float kd, float maxAbsI);
// void  Uart_Send_Message(uint16_t *adcRaw);
void Servo_Control_By_Error(float errorInner, uint16_t midDutyOfServo, uint16_t maxChangeDuty);
void Go(uint16_t minSpeed, uint16_t maxSpeed, float currentError, float lastError);
void Show_Error_On_Sceen(float errorBeforePID, float errorAfterPID, uint16_t *adcRaw);
void Stop_Inner();
void BIOS(uint8_t biosKey);

void main()
{
    All_Init();
    ADC_Norm_Fast();
    // // ADC_Norm_Slow(adcMaxOut, adcMinOut);
    // // PIT_Timer_Ms(TIM0, timerMs);
    while (1) {
        enter = 0;
        if (biosKey % 4)
            LCD_CLS();
        while (biosKey % 4) {
            BIOS(biosKey % 4);
            enter = 1;
        }
        if (enter) {
            LCD_CLS();
            sprintf(limLine, "%04d %04d   %04d %04d", adcMaxOut[0], adcMaxOut[1], adcMaxOut[2], adcMaxOut[3]);
            LCD_P6x8Str(0, 6, limLine);
            sprintf(limLine, "%04d %04d   %04d %04d", adcMinOut[0], adcMinOut[1], adcMinOut[2], adcMinOut[3]);
            LCD_P6x8Str(0, 7, limLine);
        }
        Get_ADC();
        while (adcRaw[0] < adcMinOut[0] &&
               adcRaw[1] < adcMinOut[1] &&
               adcRaw[2] < adcMinOut[2] &&
               adcRaw[3] < adcMinOut[3]) {
            Go(0, 0, 0, 0);
            Get_ADC();
        };
        // currentError = (sqrt((float)adcRaw[0] * adcRaw[1]) - sqrt((float)adcRaw[2] * adcRaw[3])) /
        //                (sqrt((float)adcRaw[0] * adcRaw[1]) + sqrt((float)adcRaw[2] * adcRaw[3]));
        currentError = Normalization(adcRaw, adcMaxOut, adcMinOut);
        pidOut       = PID_Calculate(currentError, lastError, kp, ki, kd, maxAbsI);

        Show_Error_On_Sceen(currentError, pidOut, adcRaw);
        Servo_Control_By_Error(pidOut, midDutyOfServo, maxChangeDuty);
        Go(minSpeed, maxSpeed, currentError, lastError);
        lastError = currentError;
        Ms_Delay(50);
    }
}

// void PIT0_Activated() interrupt TMR0_VECTOR
// {
//     times++;
//     if (times >= (timerTimes - 1)) {
//         Servo_Control_By_Error(pidOut, midDutyOfServo, maxChangeDuty);

//         times = 0;

//         // Uart_Send_Message(adcRaw);
//     }
// }

/// @brief 全局初始化
void All_Init()
{
    uint8_t i;
    Board_Init();

    ADC_Init(ADC[0], ADC_SPEED_2X16T);
    ADC_Init(ADC[1], ADC_SPEED_2X16T);
    ADC_Init(ADC[6], ADC_SPEED_2X16T);
    ADC_Init(ADC[7], ADC_SPEED_2X16T);

    UART_Init(UART_1, UART1_RX_P43,
              UART1_TX_P44, 9600, TIM1);

    PWM_Init(SERVO, 50, midDutyOfServo);

    PWM_Init(MOTOR_L, 10000, 10000);
    PWM_Init(MOTOR_R, 10000, 10000);
    PWM_Init(MOTOR_BASE_L, 10000, 10000);
    PWM_Init(MOTOR_BASE_R, 10000, 10000);
    for (i = 0; i < 4; i++)
        GPIO_Init(LED(i), GPIO_OUT_PP);

    GPIO_Init(KEY(0), GPIO_PullUp);
    GPIO_Init(KEY(1), GPIO_PullUp);

    GPIO_Init(DPAD_UP, GPIO_PullUp);
    GPIO_Init(DPAD_DOWN, GPIO_PullUp);
    GPIO_Init(DPAD_LEFT, GPIO_PullUp);
    GPIO_Init(DPAD_RIGHT, GPIO_PullUp);
    GPIO_Init(DPAD_MID, GPIO_PullUp);

    // PWM_Init(BUZZER, 1000, 10000);

    GPIO_EXTI_Init(KEY(0), FALLING_EDGE);
    GPIO_EXTI_Open(KEY(0));
    GPIO_EXTI_Init(KEY(1), FALLING_EDGE);
    GPIO_EXTI_Open(KEY(1));
    // GPIO_EXTI_Init(DPAD_MID, FALLING_EDGE);
    // GPIO_EXTI_Open(DPAD_MID);
    GPIO_EXTI_Init(DPAD_UP, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_UP);
    GPIO_EXTI_Init(DPAD_DOWN, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_DOWN);
    GPIO_EXTI_Init(DPAD_LEFT, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_LEFT);
    GPIO_EXTI_Init(DPAD_RIGHT, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_RIGHT);
    // GPIO_EXTI_Set_Priority(GPIO_P2, Highest_priority);
    GPIO_EXTI_Set_Priority(GPIO_P0, Second_priority);
    GPIO_EXTI_Set_Priority(GPIO_P4, Third_priority);

    LCD_Init();
}

/// @brief 蜂鸣函数
/// @param freq 蜂鸣器频率
/// @param duty 蜂鸣器占空比
/// @param beepTime 蜂鸣时长（毫秒）
/// @param sleepTime 静默时长（毫秒）
void beep(uint16_t freq, uint16_t duty, uint16_t beepTime, uint16_t sleepTime)
{
    PWM_SET_Frequency(BUZZER, freq, duty);
    Ms_Delay(beepTime);
    PWM_SET_Duty(BUZZER, 0);
    Ms_Delay(sleepTime);
}

/// @brief 获取ADC最值
void ADC_Norm_Slow(uint16_t *adcMax, uint16_t *adcMin)
{
    char     getADCLine[16];
    uint8_t  i, j, k;
    uint16_t getADC[10], temp;

    LCD_CLS();
    LCD_P8x16Str(0, 3, "ADC Norm Start ");
    beep(1000, 5000, 500, 500);

    for (k = 0; k < 4; k++) {
        sprintf(getADCLine, "GET ADC %d MAX!!!", k);
        LCD_P8x16Str(0, 3, getADCLine);
        beep(500, 5000, 500, 1500);
        for (i = 0; i < 10; i++) {
            getADC[i] = ADC_Read_Once(ADC[adcPin[k]], ADC_12BIT);
            Ms_Delay(10);
        }
        for (i = 1; i <= 10 - 1; i++) /*i代表排序轮数，总轮数=元素个数-1*/
        {
            for (j = 0; j < 10 - i;
                 j++) /*j代表每轮排序次数，次数=个数-轮数-1，但j初值为0*/
            {
                if (getADC[j] > getADC[j + 1]) /*如果前一项比后一项大，则两项的值互换*/
                {
                    temp          = getADC[j];
                    getADC[j]     = getADC[j + 1];
                    getADC[j + 1] = temp;
                }
            }
        }
        adcMax[k] = (getADC[4] + getADC[5]) / 2;
        LCD_CLS();
        sprintf(getADCLine, "ADC %d MAX=%d", k, adcMax[k]);
        LCD_P8x16Str(0, 3, getADCLine);
        beep(2000, 5000, 100, 500);
    }

    for (k = 0; k < 4; k++) {
        sprintf(getADCLine, "GET ADC %d MIN!!!", k);
        LCD_P8x16Str(0, 3, getADCLine);
        beep(500, 5000, 500, 1500);
        for (i = 0; i < 10; i++) {
            getADC[i] = ADC_Read_Once(ADC[adcPin[k]], ADC_12BIT);
            Ms_Delay(10);
        }
        for (i = 1; i <= 10 - 1; i++) /*i代表排序轮数，总轮数=元素个数-1*/
        {
            for (j = 0; j < 10 - i;
                 j++) /*j代表每轮排序次数，次数=个数-轮数-1，但j初值为0*/
            {
                if (getADC[j] > getADC[j + 1]) /*如果前一项比后一项大，则两项的值互换*/
                {
                    temp          = getADC[j];
                    getADC[j]     = getADC[j + 1];
                    getADC[j + 1] = temp;
                }
            }
        }
        adcMin[k] = (getADC[4] + getADC[5]) / 2;
        LCD_CLS();
        sprintf(getADCLine, "ADC %d MIN=%d", k, adcMin[k]);
        LCD_P8x16Str(0, 3, getADCLine);
        beep(2000, 5000, 100, 500);
    }
    LCD_CLS();
}

void ADC_Norm_Fast()
{
    char     LimLine[22];
    uint16_t i, j, getADC[4];
    LCD_P8x16Str(0, 3, "NORM...");
    for (i = 0; i < 30000; i++) {
        for (j = 0; j < 4; j++) {
            getADC[j] = ADC_Read_Once(ADC[adcPin[j]], ADC_12BIT);
            if (adcMaxOut[j] < getADC[j])
                adcMaxOut[j] = getADC[j];
            else if (adcMinOut[j] > getADC[j])
                adcMinOut[j] = getADC[j];
        }
    }
    LCD_P8x16Str(0, 3, "FINISH!");
    sprintf(LimLine, "%04d %04d   %04d %04d", adcMaxOut[0], adcMaxOut[1], adcMaxOut[2], adcMaxOut[3]);
    LCD_P6x8Str(0, 6, LimLine);
    sprintf(LimLine, "%04d %04d   %04d %04d", adcMinOut[0], adcMinOut[1], adcMinOut[2], adcMinOut[3]);
    LCD_P6x8Str(0, 7, LimLine);
    Ms_Delay(1000);
    LCD_P8x16Str(0, 3, "       ");
}

/// @brief 从ADC中获取数值
void Get_ADC()
{
    uint8_t  i, j, k;
    uint16_t temp, getADC[9];

    for (k = 0; k < 4; k++) {
        for (i = 0; i < 9; i++) {
            getADC[i] = ADC_Read_Once(ADC[adcPin[k]], ADC_12BIT);
        }
        for (i = 1; i <= 9 - 1; i++) /*i代表排序轮数，总轮数=元素个数-1*/
        {
            for (j = 0; j < 9 - i;
                 j++) /*j代表每轮排序次数，次数=个数-轮数-1，但j初值为0*/
            {
                if (getADC[j] > getADC[j + 1]) /*如果前一项比后一项大，则两项的值互换*/
                {
                    temp          = getADC[j];
                    getADC[j]     = getADC[j + 1];
                    getADC[j + 1] = temp;
                }
            }
        }
        adcRaw[k] = getADC[5];
    }
    // adcRaw[0] = ADC_Read_Once(ADC[0], ADC_12BIT);
    // adcRaw[1] = ADC_Read_Once(ADC[1], ADC_12BIT);
    // adcRaw[2] = ADC_Read_Once(ADC[6], ADC_12BIT);
    // adcRaw[3] = ADC_Read_Once(ADC[7], ADC_12BIT);
}

/// @brief 归一化，计算误差
float Normalization(uint16_t *adcRaw, uint16_t *adcMax, uint16_t *adcMin)
{
    uint8_t i;
    double  adcAfterNorm[4], adcL, adcR;
    char    adcNormLine[4][22];
    uint8_t warningOfADC[4], xOfWarning[4] = {4, 9, 11, 17};

    for (i = 0; i < 4; i++) {
        if (adcRaw[i] > adcMax[i]) // ADC超上限
        {
            adcRaw[i] = adcMax[i];
            GPIO_Write_Bit(LED(i), 0);
            warningOfADC[i] = 1;
        } else if (adcRaw[i] < adcMin[i]) // ADC超下限
        {
            adcRaw[i] = adcMin[i];
            GPIO_Write_Bit(LED(i), 0);
            warningOfADC[i] = -1;
        } else
            warningOfADC[i] = 0;
        adcAfterNorm[i] = (float)(adcRaw[i] - adcMin[i]) / (adcMax[i] - adcMin[i]); // ADC归一化至0~1的值
        sprintf(adcNormLine[i], "%.2f", adcAfterNorm[i]);
    }
    if (warningOfADC[i] == 1)
        LCD_P6x8Str(xOfWarning[i], 0, "!");
    else if (warningOfADC[i] == 1)
        LCD_P6x8Str(xOfWarning[i], 1, "!");
    sprintf(adcNormLine[0], "%s %s   %s %s", adcNormLine[0], adcNormLine[1], adcNormLine[2], adcNormLine[3]);
    LCD_P6x8Str(0, 1, adcNormLine[0]);
    adcL = (adcAfterNorm[0] + adcAfterNorm[1]) / 2;
    adcR = (adcAfterNorm[2] + adcAfterNorm[3]) / 2;
    sprintf(adcNormLine[0], "%.3f->", adcL);
    LCD_P6x8Str(0, 2, adcNormLine[0]);
    sprintf(adcNormLine[0], "<-%.3f", adcR);
    LCD_P6x8Str(84, 2, adcNormLine[0]);
    return (adcL - adcR) / (adcL + adcR + 0.001);
}

float PID_Calculate(float error, float lastError, float kp, float ki, float kd, float maxAbsI)
{
    char  pidLine[50];
    float output, sumOfError;
    sumOfError += error;
    if (sumOfError > maxAbsI)
        sumOfError = maxAbsI;
    else if (sumOfError < -maxAbsI)
        sumOfError = -maxAbsI;
    output = kp * error + ki * sumOfError + kd * (error - lastError);
    if (output > 1)
        output = 1;
    else if (output < -1)
        output = -1;
    lastError = error;
    sprintf("%0.1f * %0.2f(%0.2f) + %0.2f * %0.2f(%0.2f) + %0.2f * %0.2f(%0.2f) = %0.2f", kp, error, kp * error, ki, sumOfError, ki * sumOfError, kd, error - lastError, kd * (error - lastError));
    LCD_P6x8Str(0,3,pidLine);
    return output;
}

// void Uart_Send_Message(uint16_t *adcRaw)
// {
//     UART_PutStr(UART_1, "test");
// }

void Servo_Control_By_Error(float errorInner, uint16_t midDutyOfServo, uint16_t maxChangeDuty)
{
    if (errorInner > 1)
        errorInner = 1;
    else if (errorInner < -1)
        errorInner = -1;
    PWM_SET_Frequency(SERVO, 50, midDutyOfServo + (int)maxChangeDuty * errorInner);
}

void Show_Error_On_Sceen(float errorBeforePID, float errorAfterPID, uint16_t *adcRaw)
{
    char errorLine[22];
    sprintf(errorLine, "%04d %04d   %04d %04d", adcRaw[0], adcRaw[1], adcRaw[2], adcRaw[3]);
    LCD_P6x8Str(0, 0, errorLine);
    if (errorBeforePID >= 0)
        sprintf(errorLine, "+%.4f", errorBeforePID);
    else
        sprintf(errorLine, "%.4f", errorBeforePID);
    LCD_P6x8Str(42, 2, errorLine);
    if (errorAfterPID >= 0)
        sprintf(errorLine, "%s  -->  +%.4f", errorLine, errorAfterPID);
    else
        sprintf(errorLine, "%s  -->  %.4f", errorLine, errorAfterPID);

    // bitOfError[0] = errorToDisplay % 10;
    // bitOfError[1] = (errorToDisplay / 10) % 10;
    // bitOfError[2] = (errorToDisplay / 100) % 10;
    // bitOfError[3] = errorToDisplay / 1000;
    // if (error >= 0)
    //     sprintf(errorLine[0], "E = +%d.%d%d%d", bitOfError[3],
    //             bitOfError[2], bitOfError[1], bitOfError[0]);
    // else
    //     sprintf(errorLine[0], "E = -%d.%d%d%d", bitOfError[3],
    //             bitOfError[2], bitOfError[1], bitOfError[0]);
    // LCD_P8x16Str(0, 4, errorLine[0]);
    // LCD_PrintFloat(0, 4, error, 4);
    // for (i = 0; i < 4; i++) {
    //     errorToDisplay = adcRaw[i];
    //     bitOfError[0]  = errorToDisplay % 10;
    //     bitOfError[1]  = (errorToDisplay / 10) % 10;
    //     bitOfError[2]  = (errorToDisplay / 100) % 10;
    //     bitOfError[3]  = errorToDisplay / 1000;
    //     sprintf(errorLine[i], "%d%d%d%d", bitOfError[3],
    //             bitOfError[2], bitOfError[1], bitOfError[0]);
    // }
    // sprintf(errorLine[0], "%s %s   %s %s", errorLine[0], errorLine[1], errorLine[2], errorLine[3]);
    // LCD_P6x8Str(0, 0, errorLine[0]);
}

void Go(uint16_t minSpeed, uint16_t maxSpeed, float currentError, float lastError)
{
    int speed = (int)((maxSpeed - minSpeed) * abs(currentError - lastError) / 2) + minSpeed;
    if (speed > maxSpeed)
        speed = maxSpeed;
    else if (speed < minSpeed)
        speed = minSpeed;
    PWM_SET_Duty(MOTOR_L, 10000 - speed);
    PWM_SET_Duty(MOTOR_R, 10000 - speed);
}

void Stop_Inner()
{
    PWM_SET_Frequency(SERVO, 50, midDutyOfServo);
    PWM_SET_Duty(MOTOR_L, 10000);
    PWM_SET_Duty(MOTOR_R, 10000);
}

void BIOS(uint8_t page)
{
    char biosLine[16];
    Stop_Inner();
    switch (page) {
    case 1:
        LCD_P8x16Str(0, 0, "     BIOS 1    ");
        LCD_P6x8Str(0, 2, "---------------------");
        sprintf(biosLine, "Max Speed = %d", maxSpeed / 10);
        LCD_P8x16Str(0, 3, biosLine);
        sprintf(biosLine, "Kp = %0.2f  ", kp);
        LCD_P8x16Str(0, 6, biosLine);
        break;
    case 2:
        LCD_P8x16Str(0, 0, "     BIOS 2    ");
        LCD_P6x8Str(0, 2, "---------------------");
        sprintf(biosLine, "Min Speed = %d", maxSpeed / 10);
        LCD_P8x16Str(0, 3, biosLine);
        sprintf(biosLine, "Ki = %0.2f  ", ki);
        LCD_P8x16Str(0, 6, biosLine);
        break;
    case 3:
        LCD_P8x16Str(0, 0, "     BIOS 3    ");
        LCD_P6x8Str(0, 2, "---------------------");
        sprintf(biosLine, "MaxI = %02.2f    ", maxAbsI);
        LCD_P8x16Str(0, 3, biosLine);
        sprintf(biosLine, "Kd = %0.2f  ", kd);
        LCD_P8x16Str(0, 6, biosLine);
        break;
    default:
        break;
    }
}

void P0_EXTI_Activated() interrupt P0INT_VECTOR
{
    GPIO_EXTI_Flag_Read(GPIO_P0);
    if (Port_Exti_Flag[0]) {
        GPIO_EXTI_Flag_Clear(GPIO_P0);
        if (Port_Exti_Flag[0] & Port_Pin_7) {
            biosKey++;
        } else if (Port_Exti_Flag[0] & Port_Pin_6) {
            biosKey = 0;
        }
    }
}

void P4_EXTI_Activated() interrupt P4INT_VECTOR
{
    GPIO_EXTI_Flag_Read(GPIO_P4);
    if (Port_Exti_Flag[4]) {
        GPIO_EXTI_Flag_Clear(GPIO_P4);
        if (Port_Exti_Flag[4] & Port_Pin_2) {
            if (biosKey % 4 == 1)
                maxSpeed += 50;
            else if (biosKey % 4 == 2)
                minSpeed += 50;
            else if (biosKey % 4 == 3)
                maxAbsI += 0.5;
        } else if (Port_Exti_Flag[4] & Port_Pin_6) {
            if (biosKey % 4 == 1)
                maxSpeed -= 50;
            else if (biosKey % 4 == 2)
                minSpeed -= 50;
            else if (biosKey % 4 == 3)
                maxAbsI -= 0.5;
        } else if (Port_Exti_Flag[4] & Port_Pin_1) {
            if (biosKey % 4 == 1)
                kp += 0.1;
            else if (biosKey % 4 == 2)
                ki += 0.01;
            else if (biosKey % 4 == 3)
                kd += 0.05;
        } else if (Port_Exti_Flag[4] & Port_Pin_5) {
            if (biosKey % 4 == 1)
                kp -= 0.1;
            else if (biosKey % 4 == 2)
                ki -= 0.01;
            else if (biosKey % 4 == 3)
                kd -= 0.05;
        }
    }
}

// void P2_EXTI_Activated() interrupt P2INT_VECTOR
// {
//     GPIO_EXTI_Flag_Read(GPIO_P2);
//     if (Port_Exti_Flag[2]) {
//         GPIO_EXTI_Flag_Clear(GPIO_P0);
//         if (Port_Exti_Flag[2] & Port_Pin_7)
//             biosKey++;
//     }
// }