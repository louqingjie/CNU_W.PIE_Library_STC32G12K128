#include "main.h"
#include <MATH.H>

// ???
float    kp             = 0.5;
float    ki             = 0.1;
float    kd             = 0.1;
float    maxAbsI        = 5;  // ?????
uint8_t  timerTimes     = 5;  // ???????
uint8_t  timerMs        = 10; // ???????
uint16_t maxSpeed       = 1200;
uint16_t minSpeed       = 1100;
uint16_t midDutyOfServo = 730;
uint16_t maxChangeDuty  = 130;
uint8_t  adcPin[4]      = {0, 1, 6, 7};

// ???
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

// ????
uint16_t adcMaxOut[4] = {20, 0, 0, 0}, adcMinOut[4] = {1000, 1000, 1000, 1000}, adcRaw[4], biosKey = 0;
uint8_t  times;
float    currentError, pidOut, lastError;

// ???
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
void BIOS_1();

void main()
{
    All_Init();
    PWM_SET_Frequency(BUZZER, 1000, 5000);
    // ADC_Norm_Fast();
    // // ADC_Norm_Slow(adcMaxOut, adcMinOut);
    // // PIT_Timer_Ms(TIM0, timerMs);
    // LCD_P6x8Str(0, 3, "---------------------");
    // LCD_P6x8Str(0, 5, "---------------------");
    // while (1) {
    //     while (GPIO_Read_Bit(DPAD_MID) == 0) {
    //         while (biosKey % 3 == 1)
    //             BIOS_1();
    //         Get_ADC();
    //         while (adcRaw[0] < adcMinOut[0] &&
    //                adcRaw[1] < adcMinOut[1] &&
    //                adcRaw[2] < adcMinOut[2] &&
    //                adcRaw[3] < adcMinOut[3]) {
    //             Go(0, 0, 0, 0);
    //             Get_ADC();
    //         };
    //         // currentError = (sqrt((float)adcRaw[0] * adcRaw[1]) - sqrt((float)adcRaw[2] * adcRaw[3])) /
    //         //                (sqrt((float)adcRaw[0] * adcRaw[1]) + sqrt((float)adcRaw[2] * adcRaw[3]));
    //         currentError = Normalization(adcRaw, adcMaxOut, adcMinOut);
    //         pidOut       = PID_Calculate(currentError, lastError, kp, ki, kd, maxAbsI);

    //         Show_Error_On_Sceen(currentError, pidOut, adcRaw);
    //         Servo_Control_By_Error(pidOut, midDutyOfServo, maxChangeDuty);
    //         Go(minSpeed, maxSpeed, currentError, lastError);
    //         lastError = currentError;
    //         Ms_Delay(50);
    //     }
    //     while (GPIO_Read_Bit(DPAD_MID) == 1)
    //         ;
    //     biosKey++;
    // }
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

/// @brief ?????
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

    PWM_Init(BUZZER, 1000, 10000);

    GPIO_EXTI_Init(KEY(0), FALLING_EDGE);
    GPIO_EXTI_Open(KEY(0));
    GPIO_EXTI_Init(KEY(1), FALLING_EDGE);
    GPIO_EXTI_Open(KEY(1));
    GPIO_EXTI_Init(DPAD_UP, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_UP);
    GPIO_EXTI_Init(DPAD_DOWN, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_DOWN);
    GPIO_EXTI_Init(DPAD_LEFT, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_LEFT);
    GPIO_EXTI_Init(DPAD_RIGHT, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_RIGHT);
    GPIO_EXTI_Init(DPAD_MID, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_MID);

    LCD_Init();
}

/// @brief ????
/// @param freq ?????
/// @param duty ??????
/// @param beepTime ????(??)
/// @param sleepTime ????(??)
void beep(uint16_t freq, uint16_t duty, uint16_t beepTime, uint16_t sleepTime)
{
    PWM_SET_Frequency(BUZZER, freq, duty);
    Ms_Delay(beepTime);
    PWM_SET_Duty(BUZZER, 0);
    Ms_Delay(sleepTime);
}

/// @brief ??ADC??
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
        for (i = 1; i <= 10 - 1; i++) /*i??????,???=????-1*/
        {
            for (j = 0; j < 10 - i;
                 j++) /*j????????,??=??-??-1,?j???0*/
            {
                if (getADC[j] > getADC[j + 1]) /*??????????,???????*/
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
        for (i = 1; i <= 10 - 1; i++) /*i??????,???=????-1*/
        {
            for (j = 0; j < 10 - i;
                 j++) /*j????????,??=??-??-1,?j???0*/
            {
                if (getADC[j] > getADC[j + 1]) /*??????????,???????*/
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
    for (i = 0; i < 50000; i++) {
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
    Ms_Delay(2000);
    LCD_P8x16Str(0, 3, "       ");
}

/// @brief ?ADC?????
void Get_ADC()
{
    uint8_t  i, j, k;
    uint16_t temp, getADC[9];

    for (k = 0; k < 4; k++) {
        for (i = 0; i < 9; i++) {
            getADC[i] = ADC_Read_Once(ADC[adcPin[k]], ADC_12BIT);
        }
        for (i = 1; i <= 9 - 1; i++) /*i??????,???=????-1*/
        {
            for (j = 0; j < 9 - i;
                 j++) /*j????????,??=??-??-1,?j???0*/
            {
                if (getADC[j] > getADC[j + 1]) /*??????????,???????*/
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

/// @brief ???,????
float Normalization(uint16_t *adcRaw, uint16_t *adcMax, uint16_t *adcMin)
{
    uint8_t i;
    double  adcAfterNorm[4], adcL, adcR;
    char    adcNormLine[4][22];

    for (i = 0; i < 4; i++) {
        if (adcRaw[i] > adcMax[i]) // ADC???
        {
            adcRaw[i] = adcMax[i];
            GPIO_Write_Bit(LED(i), 0);
        } else if (adcRaw[i] < adcMin[i]) // ADC???
        {
            adcRaw[i] = adcMin[i];
            GPIO_Write_Bit(LED(i), 0);
        }
        adcAfterNorm[i] = (float)(adcRaw[i] - adcMin[i]) / (adcMax[i] - adcMin[i]); // ADC????0~1??
        sprintf(adcNormLine[i], "%.2f", adcAfterNorm[i]);
    }
    sprintf(adcNormLine[0], "%s %s   %s %s", adcNormLine[0], adcNormLine[1], adcNormLine[2], adcNormLine[3]);
    LCD_P6x8Str(0, 1, adcNormLine[0]);
    adcL = (adcAfterNorm[0] + adcAfterNorm[1]) / 2;
    adcR = (adcAfterNorm[2] + adcAfterNorm[3]) / 2;
    sprintf(adcNormLine[0], "%.3f           %.3f", adcL, adcR);
    LCD_P6x8Str(0, 2, adcNormLine[0]);
    return (adcL - adcR) / (adcL + adcR);
}

float PID_Calculate(float error, float lastError, float kp, float ki, float kd, float maxAbsI)
{
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
    if (errorAfterPID >= 0)
        sprintf(errorLine, "%s  -->  +%.4f", errorLine, errorAfterPID);
    else
        sprintf(errorLine, "%s  -->  %.4f", errorLine, errorAfterPID);
    LCD_P6x8Str(0, 4, errorLine);

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

void BIOS_1()
{
    char biosLine[16];
    Stop_Inner();
    LCD_P8x16Str(0, 0, "    BIOS  1    ");
    LCD_P8x16Str(0, 1, "---------------");
    sprintf(biosLine, "MaxSpeed = %d", maxSpeed);
    LCD_P8x16Str(0, 2, biosLine);
    sprintf(biosLine, "K1 = %0.2f  ", ki);
    LCD_P8x16Str(0, 3, biosLine);
}

// void P0_EXTI_Activated() interrupt P0INT_VECTOR
// {
//     GPIO_EXTI_Flag_Read(GPIO_P0);
//     if (Port_Exti_Flag[0]) {
//         GPIO_EXTI_Flag_Clear(GPIO_P0);
//         if (Port_Exti_Flag[0] & Port_Pin_7) {
//             if (biosKey % 3 == 1)
//                 maxSpeed += 50;
//             else if (biosKey % 3 == 2)
//                 minSpeed += 50;
//         } else if (Port_Exti_Flag[0] & Port_Pin_6) {
//             if (biosKey % 3 == 1)
//                 maxSpeed -= 50;
//             else if (biosKey % 3 == 2)
//                 minSpeed -= 50;
//         }
//     }
// }

// void P4_EXTI_Activated() interrupt P4INT_VECTOR
// {
//     GPIO_EXTI_Flag_Read(GPIO_P4);
//     if (Port_Exti_Flag[4]) {
//         GPIO_EXTI_Flag_Clear(GPIO_P4);
//         if (Port_Exti_Flag[4] & Port_Pin_2) {
//             if (biosKey % 3 == 1)
//                 ki += 0.01;
//             else if (biosKey % 3 == 2)
//                 maxAbsI++;
//         } else if (Port_Exti_Flag[4] & Port_Pin_6) {
//             if (biosKey % 3 == 1)
//                 ki -= 0.01;
//             else if (biosKey % 3 == 2)
//                 maxAbsI--;
//         } else if (Port_Exti_Flag[4] & Port_Pin_5) {
//             if (biosKey % 3 == 1)
//                 kp += 0.1;
//             else if (biosKey % 3 == 2)
//                 kd += 0.05;
//         } else if (Port_Exti_Flag[4] & Port_Pin_1) {
//             if (biosKey % 3 == 1)
//                 kp -= 0.1;
//             else if (biosKey % 3 == 2)
//                 kd -= 0.05;
//         }
//     }
// }
