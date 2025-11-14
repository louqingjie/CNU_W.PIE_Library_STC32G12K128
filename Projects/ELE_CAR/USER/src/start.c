#include "main.h"
#include "pins.h"
#include <MATH.H>
// /// @brief 定时器时间
// uint8_t timerMs = 10;
// /// @brief start键计数
// uint8_t key0Times = 0;
// /// @brief select键计数
// uint8_t key1Times = 0;
// /// @brief BIOS当前页数
// uint8_t page[2] = {1, 1};
// /// @brief BIOS当前页数级
// uint8_t pageLevel = 0;
// /// @brief BIOS第一页栏数
// #define FIRST_PAGE_NUM 5
/// @brief ADC原始数据
uint16_t adcRaw[4];
/// @brief ADC最大值
uint16_t adcMax[4];
/// @brief ADC最小值
uint16_t adcMin[4];
/// @brief ADC归一化中滤波所用：获取的多个ADC值
uint16_t getADC[10];
/// @brief ADC归一化中滤波所用：冒泡排序暂存值
uint16_t temp;
/// @brief 舵机归中中值
uint16_t midDutyOfServo;

float adcAfterNorm[4], adcL, adcR, error, kp, ki, kd;
char getADCLimitLine[16];
uint8_t i, j, k, ADCPin[4] = {0, 1, 6, 7};

void All_Init(uint8_t timerMs)
{
    Board_Init();

    ADC_Init(ADC[0], ADC_SPEED_2X16T);
    ADC_Init(ADC[7], ADC_SPEED_2X16T);

    UART_Init(UART_1, UART1_RX_P43, UART1_TX_P44, 9600, TIM1);

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
    GPIO_EXTI_Init(DPAD_UP, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_UP);
    GPIO_EXTI_Init(DPAD_DOWN, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_DOWN);
    GPIO_EXTI_Init(DPAD_LEFT, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_LEFT);
    GPIO_EXTI_Init(DPAD_RIGHT, FALLING_EDGE);
    GPIO_EXTI_Open(DPAD_RIGHT);

    LCD_Init();

    PIT_Timer_Ms(TIM0, timerMs);
}

/// @brief 显示BIOS设置第一页
// void First_Page(uint8_t page)
// {
//     LCD_P6x8Str(0, 1, "---------------------");
//     LCD_P6x8Str(0, 0, "        BIOS         ");
//     LCD_P6x8Str(0, 4, "                     ");
//     LCD_P6x8Str(0, 6, "                     ");
//     switch (page)
//     {
//     case 1:
//         LCD_P8x16Str(0, 2, ">1.Timer Value<");
//         LCD_P6x8Str(0, 5, "2.Servo Duty         ");
//         LCD_P6x8Str(0, 7, "3.Speed Limit        ");
//         break;
//     case 2:
//         LCD_P8x16Str(0, 2, ">2.Servo Duty <");
//         LCD_P6x8Str(0, 5, "3.Speed Limit        ");
//         LCD_P6x8Str(0, 7, "4.KP, KI & KD        ");
//         break;
//     case 3:
//         LCD_P8x16Str(0, 2, ">3.Speed Limit<");
//         LCD_P6x8Str(0, 5, "4.KP, KI & KD        ");
//         LCD_P6x8Str(0, 7, "5.Quit & Save        ");
//         break;
//     case 4:
//         LCD_P8x16Str(0, 2, ">4.KP, KI & KD<");
//         LCD_P6x8Str(0, 5, "5.Quit & Save        ");
//         LCD_P6x8Str(0, 7, "1.Timer Value        ");
//         break;
//     case 5:
//         LCD_P8x16Str(0, 2, ">5.Quit & Save<");
//         LCD_P6x8Str(0, 5, "1.Timer Value        ");
//         LCD_P6x8Str(0, 7, "2.Servo Duty         ");
//         break;
//     }
// }

/// @brief BIOS设置
// void BIOS()
// {
//     /*		level
//     page	1				2
//             1 	Time Value	-> 1	Value Choose
//             |
//             2 	Servo Duty	-> 1	Mid Duty
//             |				-> 2	Max Duty
//             |
//             3 	Speed Limit	-> 1	Max Speed
//             |				-> 2	Min Speed
//             |
//             4 	KP, KI & KD	-> 1	KP
//             |				-> 2	KI
//             |				-> 3	KD
//             |
//             5 	Quit & Save	-> 1	Yes
//     */
//     key0Times = 0;
//     key1Times = 0;
//     while (1) // 使停留在设置界面
//     {
//         while (key0Times == 0) // 当确认键未按下
//         {
//             page[0] = key1Times % FIRST_PAGE_NUM + 1;
//             First_Page(page[0]);
//         }
//         key1Times = 0; // 选择键清零
//         LCD_CLS();
//         switch (page[0])
//         {
//         case 1:
//             LCD_P6x8Str(0, 0, "     Timer Value     ");
//             while (key0Times == 1) // 确认键触发，进入timer设置页面，未二次确认前不退出
//             {
//                 switch (key1Times % 2)
//                 {
//                 case 0:
//                     LCD_P8x16Str(0, 2, ">1.Time       <");
//                     LCD_P6x8Str(0, 5, "2.2.Count            ");
//                     break;
//                 case 1:
//                     LCD_P6x8Str(0, 2, "1.Time               ");
//                     LCD_P8x16Str(0, 5, ">2.Count      <");
//                 default:
//                     break;
//                 }
//                 key1Times = timerMs / 5 - 1;
//                 switch (key1Times)
//                 {
//                 case 0:
//                     timerMs = 5;
//                     LCD_P8x16Str(16, 2, "Timer = 05ms");
//                     LCD_PrintU16(80, 5, 10);
//                     break;
//                 case 1:
//                     timerMs = 10;
//                     LCD_P8x16Str(16, 2, "Timer = 10ms");
//                     LCD_PrintU16(80, 5, 15);
//                     break;
//                 case 2:
//                     timerMs = 15;
//                     LCD_P8x16Str(16, 2, "Timer = 15ms");
//                     LCD_PrintU16(80, 5, 20);
//                     break;
//                 case 3:
//                     timerMs = 20;
//                     LCD_P8x16Str(16, 2, "Timer = 20ms");
//                     LCD_PrintU16(80, 5, 0);
//                     LCD_PrintU16(86, 5, 5);
//                     break;
//                 default:
//                     key1Times = 0;
//                     break;
//                 }
//             }
//             key0Times = 0; // 二次确认，确认键清零，第一处设置结束
//             key1Times = 0;
//             break;
//         case 2:
//             LCD_P6x8Str(0, 0, "     Servo  Duty     ");
//             while (key0Times == 2)
//             {
//                 page[1] = key1Times % 2 + 1;
//                 switch (key1Times)
//                 {
//                 case 1:
//                     LCD_P8x16Str(0, 2, ">1.Mid Duty   <");
//                     LCD_P6x8Str(0, 5, "2.Max Duty           ");
//                     break;
//                 case 2:
//                     LCD_P6x8Str(0, 4, "1.Mid Duty            ");
//                     LCD_P8x16Str(0, 4, ">2.Max Duty   <");
//                     break;
//                 default:
//                     break;
//                 }
//             }
//             LCD_CLS();
//             switch (page[1])
//             {
//             case 1:
//                 LCD_P6x8Str(0, 0, "      Mid  Duty      ");
//                 while (key0Times == 1)
//                 {
//                     /* code */
//                 }
//                 break;
//             default:
//                 break;
//             }
//             break;
//         default:
//             break;
//         }
//     }
// }

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

/// @brief ADC读取最大最小值，为归一化做准备
void ADC_Norm_Slow()
{
    LCD_CLS();
    LCD_P8x16Str(0, 3, "ADC Norm Start ");
    beep(1000, 0, 500, 500);
    for (k = 0; k < 4; k++)
    {
        sprintf(getADCLimitLine, "GET ADC %d MAX!!!", k);
        LCD_P8x16Str(0, 3, getADCLimitLine);
        beep(500, 0, 500, 500);
        for (i = 0; i < 10; i++)
        {
            getADC[i] = ADC_Read_Once(ADC[ADCPin[k]], ADC_12BIT);
            Ms_Delay(10);
        }
        for (i = 1; i <= 10 - 1; i++) /*i代表排序轮数，总轮数=元素个数-1*/
        {
            for (j = 0; j < 10 - i; j++) /*j代表每轮排序次数，次数=个数-轮数-1，但j初值为0*/
            {
                if (getADC[j] > getADC[j + 1]) /*如果前一项比后一项大，则两项的值互换*/
                {
                    temp = getADC[j];
                    getADC[j] = getADC[j + 1];
                    getADC[j + 1] = temp;
                }
            }
        }
        adcMax[k] = (getADC[4] + getADC[5]) / 2;
        sprintf(getADCLimitLine, "%dMAX=%d       ", k, adcMax[k]);
        LCD_P8x16Str(0, 3, getADCLimitLine);
        beep(2000, 0, 100, 500);

        sprintf(getADCLimitLine, "GET ADC %d MIN!!!", k);
        LCD_P8x16Str(0, 3, getADCLimitLine);
        beep(500, 0, 500, 500);
        for (i = 0; i < 10; i++)
        {
            getADC[i] = ADC_Read_Once(ADC[ADCPin[k]], ADC_12BIT);
            Ms_Delay(10);
        }
        for (i = 1; i <= 10 - 1; i++) /*i代表排序轮数，总轮数=元素个数-1*/
        {
            for (j = 0; j < 10 - i; j++) /*j代表每轮排序次数，次数=个数-轮数-1，但j初值为0*/
            {
                if (getADC[j] > getADC[j + 1]) /*如果前一项比后一项大，则两项的值互换*/
                {
                    temp = getADC[j];
                    getADC[j] = getADC[j + 1];
                    getADC[j + 1] = temp;
                }
            }
        }
        adcMin[k] = (getADC[4] + getADC[5]) / 2;
        sprintf(getADCLimitLine, "%dMIN=%d       ", k, adcMin[k]);
        LCD_P8x16Str(0, 3, getADCLimitLine);
        beep(2000, 0, 100, 500);
    }
    LCD_CLS();
}

/// @brief 从ADC中获取数值
void Get_ADC()
{
    adcRaw[0] = ADC_Read_Once(ADC[0], ADC_12BIT);
    adcRaw[1] = ADC_Read_Once(ADC[1], ADC_12BIT);
    adcRaw[2] = ADC_Read_Once(ADC[6], ADC_12BIT);
    adcRaw[3] = ADC_Read_Once(ADC[7], ADC_12BIT);
    UART_PutBuff(UART_1, )
}

/// @brief 归一化，计算误差
/// @return 误差
float Normalization()
{
    for (i = 0; i < 4; i++)
    {
        if (adcRaw[i] > adcMax[i]) // ADC超上限
        {
            adcRaw[i] = adcMax[i];
            GPIO_Write_Bit(LED(i), 0);
        }
        else if (adcRaw[i] < adcMin[i]) // ADC超下限
        {
            adcRaw[i] = adcMin[i];
            GPIO_Write_Bit(LED(i), 0);
        }
        adcAfterNorm[i] = (float)(adcRaw[i] - adcMin[i]) / (adcMax[i] - adcMin[i]); // ADC归一化至0~1的值
    }
    adcL = sqrt(adcAfterNorm[0] * adcAfterNorm[1]);
    adcR = sqrt(adcAfterNorm[2] * adcAfterNorm[3]);
    return (adcL - adcR) / (adcL + adcR);
}