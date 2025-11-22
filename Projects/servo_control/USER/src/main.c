#include "main.h"
#include <MATH.H>

// ========================= 参数区 =========================
float    kp             = 1.2;          // PID比例系数
float    kd             = 0.9;          // PID微分系数
float    kA             = 0.8;          // 误差计算系数A
float    kB             = 0.7;          // 误差计算系数B
uint8_t  stopADC        = 25;           // 停车ADC阈值
uint8_t  timerMs        = 15;           // 计时器触发时间(ms)
uint16_t maxSpeed       = 1800;         // 电机最大速度
uint16_t minSpeed       = 1250;         // 电机最小速度
uint16_t midDutyOfServo = 730;          // 舵机中位PWM值
uint16_t maxChangeDuty  = 130;          // 舵机最大变化PWM值
uint8_t  adcPin[4]      = {0, 1, 6, 7}; // ADC引脚映射

// ========================= 定义区 =========================
// ADC通道定义
char ADC[8] = {ADC_P13, ADC_P16, ADC_P10, ADC_P11,
               ADC_P02, ADC_P05, ADC_P00, ADC_P01};

// 舵机和电机PWM输出定义
#define SERVO PWMB_CH1_P74
#define MOTOR_L PWMA_CH2P_P62
#define MOTOR_R PWMA_CH3P_P64
#define MOTOR_BASE_L PWMA_CH1P_P60
#define MOTOR_BASE_R PWMA_CH4P_P66
#define BUZZER PWMB_CH3_P33

// LED指示灯引脚定义
char LEDPin[4] = {GPIO_Pin_7, GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_4};
#define LED(x) GPIO_P3, LEDPin[x]

// 按键引脚定义
char KEYPin[2] = {GPIO_Pin_7, GPIO_Pin_6};
#define KEY(x) GPIO_P0, KEYPin[x]

// 方向键引脚定义
#define DPAD_UP GPIO_P4, GPIO_P2
#define DPAD_DOWN GPIO_P4, GPIO_Pin_6
#define DPAD_LEFT GPIO_P4, GPIO_Pin_5
#define DPAD_RIGHT GPIO_P4, GPIO_Pin_1

// ========================= 全局变量 =========================
uint16_t adcMax[4] = {20, 20, 20, 20};         // ADC最大值
uint16_t adcMin[4] = {1000, 1000, 1000, 1000}; // ADC最小值
uint16_t adcRaw[4];                            // ADC原始值
uint16_t biosKey = 0;                          // BIOS页面键值
uint16_t enter;                                // 进入标志
float    error;                                // 当前误差
float    pidOut;                               // PID输出
float    lastError;                            // 上次误差
float    adcAfterNorm[4];                      // 归一化后的ADC值
char     limLine[22];                          // 显示缓冲区

void  All_Init(uint16_t midDutyOfServo, uint8_t *adcPin);
void  ADC_Norm_Fast(uint16_t *adcMax, uint16_t *adcMin, uint8_t *adcPin);
void  Get_ADC(uint16_t *adcRaw, uint8_t *adcPin);
void  Normalization(uint16_t *adcRaw, uint16_t *adcMax, uint16_t *adcMin, float *adcAfterNorm);
float ADC_To_Error(float *adcAfterNorm, float kA, float kB);
float PD_Calculate(float error, float lastError, float kp, float kd);
void  Servo_Control_By_Error(float errorInner, uint16_t midDutyOfServo, uint16_t maxChangeDuty);
void  Go(uint16_t minSpeed, uint16_t maxSpeed, float error);
void  Show_Error_On_Sceen(float errorBeforePID, float errorAfterPID, uint16_t *adcRaw);
void  Stop_Inner();
void  BIOS(uint16_t biosKey, uint16_t minSpeed, uint16_t maxSpeed, float kp, float kd, float kA, float kB);

// ========================= 主函数 =========================
void main()
{
    All_Init(midDutyOfServo, adcPin);      // 初始化所有外设
    ADC_Norm_Fast(adcMax, adcMin, adcPin); // 快速ADC校准
    PIT_Timer_Ms(TIM0, timerMs);           // 定时器初始化

    while (1) {
        // ========================= BIOS系统处理 =========================
        enter = 0;
        // BIOS系统处理
        if (biosKey % 4)
            LCD_CLS();
        while (biosKey % 4) {
            BIOS(biosKey, minSpeed, maxSpeed, kp, kd, kA, kB);
            enter = 1;
        }
        if (enter) {
            LCD_CLS();
            // 显示ADC校准值
            sprintf(limLine, "%04d %04d   %04d %04d", adcMax[0], adcMax[1], adcMax[2], adcMax[3]);
            LCD_P6x8Str(0, 6, limLine);
            sprintf(limLine, "%04d %04d   %04d %04d", adcMin[0], adcMin[1], adcMin[2], adcMin[3]);
            LCD_P6x8Str(0, 7, limLine);
        }
        // ========================= 运行控制处理 =========================
        // 检测是否在赛道上，如果所有传感器都检测不到黑线则停止
        while (adcRaw[0] <= adcMin[0] &&
               adcRaw[1] <= adcMin[1] &&
               adcRaw[2] <= adcMin[2] &&
               adcRaw[3] <= adcMin[3]) {
            Go(0, 0, 0); // 停止电机
        };
        // 显示误差信息
        Show_Error_On_Sceen(error, pidOut, adcRaw);
        // 电机控制
        Go(minSpeed, maxSpeed, pidOut);
    }
}

// ========================= 函数实现 =========================

/// @brief 全局初始化函数
void All_Init(uint16_t midDutyOfServo, uint8_t *adcPin)
{
    uint8_t i;
    Board_Init(); // 开发板初始化

    // ADC初始化
    for (i = 0; i < 4; i++)
        ADC_Init(ADC[adcPin[i]], ADC_SPEED_2X16T);

    // 串口初始化
    UART_Init(UART_1, UART1_RX_P43,
              UART1_TX_P44, 9600, TIM1);

    // PWM初始化（舵机和电机）
    PWM_Init(SERVO, 50, midDutyOfServo);
    PWM_Init(MOTOR_L, 10000, 10000);
    PWM_Init(MOTOR_R, 10000, 10000);
    PWM_Init(MOTOR_BASE_L, 10000, 10000);
    PWM_Init(MOTOR_BASE_R, 10000, 10000);

    // LED初始化
    for (i = 0; i < 4; i++)
        GPIO_Init(LED(i), GPIO_OUT_PP);

    // 按键初始化（上拉输入）
    GPIO_Init(KEY(0), GPIO_PullUp);
    GPIO_Init(KEY(1), GPIO_PullUp);

    // 方向键初始化
    GPIO_Init(DPAD_UP, GPIO_PullUp);
    GPIO_Init(DPAD_DOWN, GPIO_PullUp);
    GPIO_Init(DPAD_LEFT, GPIO_PullUp);
    GPIO_Init(DPAD_RIGHT, GPIO_PullUp);

    // 外部中断初始化
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

    // 设置中断优先级
    GPIO_EXTI_Set_Priority(GPIO_P0, Second_priority);
    GPIO_EXTI_Set_Priority(GPIO_P4, Third_priority);

    PWM_Init(BUZZER, 2000, 10000);

    LCD_Init(); // LCD初始化
}

/// @brief 快速ADC校准（动态范围校准）
void ADC_Norm_Fast(uint16_t *adcMax, uint16_t *adcMin, uint8_t *adcPin)
{
    char     LimLine[22];
    uint16_t i, j, getADC[4];
    uint16_t note[8] = {1046, 1175, 1318, 1397, 1568, 1760, 1976, 2093};
    LCD_P8x16Str(0, 3, "NORM...");

    // 连续采样30000次，动态获取最大值和最小值
    for (i = 0; i < 32000; i++) {
        PWM_SET_Frequency(BUZZER, note[i / 4000], 5000);
        for (j = 0; j < 4; j++) {
            getADC[j] = ADC_Read_Once(ADC[adcPin[j]], ADC_12BIT);
            if (adcMax[j] < getADC[j])
                adcMax[j] = getADC[j]; // 更新最大值
            else if (adcMin[j] > getADC[j])
                adcMin[j] = getADC[j]; // 更新最小值
        }
    }

    LCD_P8x16Str(0, 3, "FINISH!");
    // 显示校准结果
    sprintf(LimLine, "%04d %04d   %04d %04d", adcMax[0], adcMax[1], adcMax[2], adcMax[3]);
    LCD_P6x8Str(0, 6, LimLine);
    sprintf(LimLine, "%04d %04d   %04d %04d", adcMin[0], adcMin[1], adcMin[2], adcMin[3]);
    LCD_P6x8Str(0, 7, LimLine);
    PWM_SET_Frequency(BUZZER, 2093, 10000);
    Ms_Delay(100);
    for (i = 0; i < 8; i++) {
        PWM_SET_Frequency(BUZZER, note[7-i], 5000);
        Ms_Delay(100);
    }
    PWM_SET_Duty(BUZZER, 10000);
    LCD_P8x16Str(0, 3, "       ");
}

/// @brief 获取ADC采样值（中值滤波）
void Get_ADC(uint16_t *adcRaw, uint8_t *adcPin)
{
    int i;
    for (i = 0; i < 4; i++)
        adcRaw[i] = ADC_Read_Once(ADC[adcPin[i]], ADC_12BIT);
}

/// @brief ADC数据归一化处理
/// @return 归一化后的误差值(-1~1)
void Normalization(uint16_t *adcRaw, uint16_t *adcMax, uint16_t *adcMin, float *adcAfterNorm)
{
    uint8_t i;
    // 对每个ADC通道进行归一化处理
    for (i = 0; i < 4; i++) {
        // 边界检查
        if (adcRaw[i] > adcMax[i]) { // 超上限
            adcRaw[i] = adcMax[i];
        } else if (adcRaw[i] < adcMin[i]) { // 超下限
            adcRaw[i] = adcMin[i];
        }
        adcAfterNorm[i] = (float)(adcRaw[i] - adcMin[i]) / (adcMax[i] - adcMin[i]);
    }
}

float ADC_To_Error(float *adcAfterNorm, float kA, float kB)
{
    float num = kA * (adcAfterNorm[0] - adcAfterNorm[3]) + kB * (adcAfterNorm[1] - adcAfterNorm[2]);
    float den = kA * (adcAfterNorm[0] + adcAfterNorm[3]) + kB * fabs(adcAfterNorm[1] - adcAfterNorm[2]);
    return (float)num / den;
}

float PD_Calculate(float error, float lastError, float kp, float kd)
{
    float output, derivative;
    derivative = kd * (error - lastError);
    output     = kp * error + derivative;
    if (output > 1)
        output = 1;
    else if (output < -1)
        output = -1;
    return output;
}

/// @brief 舵机控制函数
void Servo_Control_By_Error(float errorInner, uint16_t midDutyOfServo, uint16_t maxChangeDuty)
{
    // 误差限幅
    if (errorInner > 1)
        errorInner = 1;
    else if (errorInner < -1)
        errorInner = -1;
    // 根据误差计算舵机PWM值：中位值 ± 最大变化量×误差
    PWM_SET_Duty(SERVO, midDutyOfServo + (int)maxChangeDuty * errorInner);
}

/// @brief 电机控制函数
void Go(uint16_t minSpeed, uint16_t maxSpeed, float error)
{
    // 根据误差变化率计算速度：基础速度 + 变化率×系数
    int speed = (int)((maxSpeed - minSpeed) * (1 - fabs(error))) + minSpeed;

    // 速度限幅
    if (speed > maxSpeed)
        speed = maxSpeed;
    else if (speed < minSpeed)
        speed = minSpeed;

    // 设置左右电机PWM
    PWM_SET_Duty(MOTOR_L, 10000 - speed);
    PWM_SET_Duty(MOTOR_R, 10000 - speed);
}

/// @brief 显示误差信息
void Show_Error_On_Sceen(float errorBeforePID, float errorAfterPID, uint16_t *adcRaw)
{
    char errorLine[22];
    // 显示ADC原始值
    sprintf(errorLine, "%04d %04d   %04d %04d", adcRaw[0], adcRaw[1], adcRaw[2], adcRaw[3]);
    LCD_P6x8Str(0, 0, errorLine);

    // 显示PID前后的误差值
    if (errorBeforePID >= 0)
        sprintf(errorLine, "+%.4f", errorBeforePID);
    else
        sprintf(errorLine, "%.4f", errorBeforePID);
    LCD_P6x8Str(42, 2, errorLine);

    if (errorAfterPID >= 0)
        sprintf(errorLine, "%s  -->  +%.4f", errorLine, errorAfterPID);
    else
        sprintf(errorLine, "%s  -->  %.4f", errorLine, errorAfterPID);
}

/// @brief 停止控制（舵机回中，电机停止）
void Stop_Inner()
{
    PWM_SET_Duty(MOTOR_L, 10000); // 电机停止
    PWM_SET_Duty(MOTOR_R, 10000);
}

/// @brief BIOS系统（参数调整界面）
void BIOS(uint16_t biosKey, uint16_t minSpeed, uint16_t maxSpeed, float kp, float kd, float kA, float kB)
{
    uint8_t page = biosKey % 4;
    char    biosLine[16];
    Stop_Inner(); // 进入BIOS时停止运动

    switch (page) {
    case 1: // 页面1：最大速度和Kp调整
        LCD_P8x16Str(0, 0, "     BIOS 1    ");
        LCD_P6x8Str(0, 2, "---------------------");
        sprintf(biosLine, "MaxSpeed = %d ", maxSpeed);
        LCD_P8x16Str(0, 3, biosLine);
        sprintf(biosLine, "Kp = %0.2f    ", kp);
        LCD_P8x16Str(0, 6, biosLine);
        break;
    case 2: // 页面2：最小速度和Ki调整
        LCD_P8x16Str(0, 0, "     BIOS 2    ");
        LCD_P6x8Str(0, 2, "---------------------");
        sprintf(biosLine, "MinSpeed = %d ", minSpeed);
        LCD_P8x16Str(0, 3, biosLine);
        sprintf(biosLine, "Kd = %0.2f   ", kd);
        LCD_P8x16Str(0, 6, biosLine);
        break;
    case 3: // 页面3：积分限幅和Kd调整
        LCD_P8x16Str(0, 0, "     BIOS 3    ");
        LCD_P6x8Str(0, 2, "---------------------");
        sprintf(biosLine, "KA = %0.2f       ", kA);
        LCD_P8x16Str(0, 3, biosLine);
        sprintf(biosLine, "KB = %0.2f       ", kB);
        LCD_P8x16Str(0, 6, biosLine);
        break;
    default:
        break;
    }
}

// ========================= 中断服务函数 =========================

void PIT0_Activated() interrupt TMR0_VECTOR
{
    PIT_Timer_Clear(TIM0);
    Get_ADC(adcRaw, adcPin);                                       // 获取ADC采样值
    Normalization(adcRaw, adcMax, adcMin, adcAfterNorm);           // 归一化处理
    error     = ADC_To_Error(adcAfterNorm, kA, kB);                // 计算误差值
    pidOut    = PD_Calculate(error, lastError, kp, kd);            // PID计算
    lastError = error;                                             // 更新上次误差
    Servo_Control_By_Error(pidOut, midDutyOfServo, maxChangeDuty); // 舵机控制
}

/// @brief P0口外部中断服务函数（按键中断）
void P0_EXTI_Activated() interrupt P0INT_VECTOR
{
    GPIO_EXTI_Flag_Read(GPIO_P0);
    if (Port_Exti_Flag[0]) {
        GPIO_EXTI_Flag_Clear(GPIO_P0);
        if (Port_Exti_Flag[0] & Port_Pin_7) {
            biosKey++; // 按键1：切换BIOS页面
        } else if (Port_Exti_Flag[0] & Port_Pin_6) {
            biosKey = 0; // 按键2：退出BIOS
        }
    }
}

/// @brief P4口外部中断服务函数（方向键中断）
void P4_EXTI_Activated() interrupt P4INT_VECTOR
{
    GPIO_EXTI_Flag_Read(GPIO_P4);
    if (Port_Exti_Flag[4]) {
        GPIO_EXTI_Flag_Clear(GPIO_P4);

        // 根据当前BIOS页面调整不同参数
        if (Port_Exti_Flag[4] & Port_Pin_2) { // 上键：增加参数
            if (biosKey % 4 == 1)
                maxSpeed += 50;
            else if (biosKey % 4 == 2)
                minSpeed += 50;
            else if (biosKey % 4 == 3)
                kA += 0.05;
        } else if (Port_Exti_Flag[4] & Port_Pin_6) { // 下键：减小参数
            if (biosKey % 4 == 1)
                maxSpeed -= 50;
            else if (biosKey % 4 == 2)
                minSpeed -= 50;
            else if (biosKey % 4 == 3)
                kA -= 0.05;
        } else if (Port_Exti_Flag[4] & Port_Pin_1) { // 右键：增加PID参数
            if (biosKey % 4 == 1)
                kp += 0.1;
            else if (biosKey % 4 == 2)
                kd += 0.05;
            else if (biosKey % 4 == 3)
                kB += 0.05;
        } else if (Port_Exti_Flag[4] & Port_Pin_5) { // 左键：减小PID参数
            if (biosKey % 4 == 1)
                kp -= 0.1;
            else if (biosKey % 4 == 2)
                kd -= 0.05;
            else if (biosKey % 4 == 3)
                kB -= 0.05;
        }
    }
}