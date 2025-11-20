#include "main.h"
#include <MATH.H>

// ========================= 参数区 =========================
float    kp             = 0.5;          // PID比例系数
float    ki             = 0.0035;        // PID积分系数
float    kd             = 0.4;         // PID微分系数
float    maxAbsI        = 100;            // 最大积分量，防止积分饱和
uint8_t  timerTimes     = 5;            // 计时器触发次数
uint8_t  timerMs        = 10;           // 计时器触发时间(ms)
uint16_t maxSpeed       = 1900;         // 电机最大速度
uint16_t minSpeed       = 1200;         // 电机最小速度
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
#define DPAD_MID GPIO_P2, GPIO_P7

// 蜂鸣器定义
#define BUZZER PWMB_CH3_P33

// ========================= 全局变量 =========================
uint16_t adcMaxOut[4] = {20, 20, 20, 20};         // ADC最大值
uint16_t adcMinOut[4] = {1000, 1000, 1000, 1000}; // ADC最小值
uint16_t adcRaw[4];                               // ADC原始值
uint16_t biosKey = 0;                             // BIOS页面键值
uint16_t enter;                                   // 进入标志
uint8_t  times;                                   // 计时器计数
float    currentError;                            // 当前误差
float    pidOut;                                  // PID输出
float    lastError;                               // 上次误差
float    sumOfError = 0;                          // 误差积分和
char     limLine[22];                             // 显示缓冲区

// ========================= 函数声明 =========================
void  All_Init();                                                                                // 全局初始化
void  beep(uint16_t freq, uint16_t duty, uint16_t beepTime, uint16_t sleepTime);                 // 蜂鸣函数
void  ADC_Norm_Slow(uint16_t *adcMax, uint16_t *adcMin);                                         // 慢速ADC校准
void  ADC_Norm_Fast();                                                                           // 快速ADC校准
void  Get_ADC();                                                                                 // 获取ADC值
float Normalization(uint16_t *adcRaw, uint16_t *adcMax, uint16_t *adcMin);                       // 归一化处理
float PID_Calculate(float error, float lastError, float kp, float ki, float kd, float maxAbsI);  // PID计算
void  Servo_Control_By_Error(float errorInner, uint16_t midDutyOfServo, uint16_t maxChangeDuty); // 舵机控制
void  Go(uint16_t minSpeed, uint16_t maxSpeed, float currentError, float lastError);             // 电机控制
void  Show_Error_On_Sceen(float errorBeforePID, float errorAfterPID, uint16_t *adcRaw);          // 显示误差
void  Stop_Inner();                                                                              // 停止控制
void  BIOS(uint8_t biosKey);                                                                     // BIOS系统

// ========================= 主函数 =========================
void main()
{
    All_Init();      // 初始化所有外设
    ADC_Norm_Fast(); // 快速ADC校准

    while (1) {
        enter = 0;
        // BIOS系统处理
        if (biosKey % 4)
            LCD_CLS();
        while (biosKey % 4) {
            BIOS(biosKey % 4);
            enter = 1;
        }
        if (enter) {
            LCD_CLS();
            // 显示ADC校准值
            sprintf(limLine, "%04d %04d   %04d %04d", adcMaxOut[0], adcMaxOut[1], adcMaxOut[2], adcMaxOut[3]);
            LCD_P6x8Str(0, 6, limLine);
            sprintf(limLine, "%04d %04d   %04d %04d", adcMinOut[0], adcMinOut[1], adcMinOut[2], adcMinOut[3]);
            LCD_P6x8Str(0, 7, limLine);
        }

        Get_ADC(); // 获取ADC值

        // 检测是否在赛道上，如果所有传感器都检测不到黑线则停止
        while (adcRaw[0] < 20 &&
               adcRaw[1] < 20 &&
               adcRaw[2] < 20 &&
               adcRaw[3] < 20) {
            Go(0, 0, 0, 0); // 停止电机
            Get_ADC();
        };

        // 计算归一化误差
        currentError = Normalization(adcRaw, adcMaxOut, adcMinOut);
        // PID计算
        pidOut = PID_Calculate(currentError, lastError, kp, ki, kd, maxAbsI);

        // 显示误差信息
        Show_Error_On_Sceen(currentError, pidOut, adcRaw);
        // 舵机控制
        Servo_Control_By_Error(pidOut, midDutyOfServo, maxChangeDuty);
        // 电机控制
        Go(minSpeed, maxSpeed, currentError, lastError);
        // 更新上次误差
        lastError = currentError;
        Ms_Delay(5); // 延时10ms
    }
}

// ========================= 中断服务函数 =========================
// 定时器中断（注释掉）
/*
void PIT0_Activated() interrupt TMR0_VECTOR
{
    times++;
    if (times >= (timerTimes - 1)) {
        Servo_Control_By_Error(pidOut, midDutyOfServo, maxChangeDuty);
        times = 0;
    }
}
*/

// ========================= 函数实现 =========================

/// @brief 全局初始化函数
void All_Init()
{
    uint8_t i;
    Board_Init(); // 开发板初始化

    // ADC初始化
    ADC_Init(ADC[0], ADC_SPEED_2X16T);
    ADC_Init(ADC[1], ADC_SPEED_2X16T);
    ADC_Init(ADC[6], ADC_SPEED_2X16T);
    ADC_Init(ADC[7], ADC_SPEED_2X16T);

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
    GPIO_Init(DPAD_MID, GPIO_PullUp);

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

    LCD_Init(); // LCD初始化
}

/// @brief 蜂鸣器控制函数
/// @param freq 蜂鸣器频率(Hz)
/// @param duty 蜂鸣器占空比
/// @param beepTime 蜂鸣时长(ms)
/// @param sleepTime 静默时长(ms)
void beep(uint16_t freq, uint16_t duty, uint16_t beepTime, uint16_t sleepTime)
{
    PWM_SET_Frequency(BUZZER, freq, duty); // 设置频率和占空比
    Ms_Delay(beepTime);                    // 蜂鸣持续时间
    PWM_SET_Duty(BUZZER, 0);               // 关闭蜂鸣器
    Ms_Delay(sleepTime);                   // 静默时间
}

/// @brief 慢速ADC校准（精确校准）
void ADC_Norm_Slow(uint16_t *adcMax, uint16_t *adcMin)
{
    char     getADCLine[16];
    uint8_t  i, j, k;
    uint16_t getADC[10], temp;

    LCD_CLS();
    LCD_P8x16Str(0, 3, "ADC Norm Start ");
    beep(1000, 5000, 500, 500); // 提示音

    // 获取每个ADC通道的最大值
    for (k = 0; k < 4; k++) {
        sprintf(getADCLine, "GET ADC %d MAX!!!", k);
        LCD_P8x16Str(0, 3, getADCLine);
        beep(500, 5000, 500, 1500);

        // 采样10次
        for (i = 0; i < 10; i++) {
            getADC[i] = ADC_Read_Once(ADC[adcPin[k]], ADC_12BIT);
            Ms_Delay(10);
        }

        // 冒泡排序取中值
        for (i = 1; i <= 10 - 1; i++) {
            for (j = 0; j < 10 - i; j++) {
                if (getADC[j] > getADC[j + 1]) {
                    temp          = getADC[j];
                    getADC[j]     = getADC[j + 1];
                    getADC[j + 1] = temp;
                }
            }
        }
        adcMax[k] = (getADC[4] + getADC[5]) / 2; // 取中值

        LCD_CLS();
        sprintf(getADCLine, "ADC %d MAX=%d", k, adcMax[k]);
        LCD_P8x16Str(0, 3, getADCLine);
        beep(2000, 5000, 100, 500);
    }

    // 获取每个ADC通道的最小值（类似最大值获取）
    for (k = 0; k < 4; k++) {
        sprintf(getADCLine, "GET ADC %d MIN!!!", k);
        LCD_P8x16Str(0, 3, getADCLine);
        beep(500, 5000, 500, 1500);

        for (i = 0; i < 10; i++) {
            getADC[i] = ADC_Read_Once(ADC[adcPin[k]], ADC_12BIT);
            Ms_Delay(10);
        }

        for (i = 1; i <= 10 - 1; i++) {
            for (j = 0; j < 10 - i; j++) {
                if (getADC[j] > getADC[j + 1]) {
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

/// @brief 快速ADC校准（动态范围校准）
void ADC_Norm_Fast()
{
    char     LimLine[22];
    uint16_t i, j, getADC[4];
    LCD_P8x16Str(0, 3, "NORM...");

    // 连续采样30000次，动态获取最大值和最小值
    for (i = 0; i < 30000; i++) {
        for (j = 0; j < 4; j++) {
            getADC[j] = ADC_Read_Once(ADC[adcPin[j]], ADC_12BIT);
            if (adcMaxOut[j] < getADC[j])
                adcMaxOut[j] = getADC[j]; // 更新最大值
            else if (adcMinOut[j] > getADC[j])
                adcMinOut[j] = getADC[j]; // 更新最小值
        }
    }

    LCD_P8x16Str(0, 3, "FINISH!");
    // 显示校准结果
    sprintf(LimLine, "%04d %04d   %04d %04d", adcMaxOut[0], adcMaxOut[1], adcMaxOut[2], adcMaxOut[3]);
    LCD_P6x8Str(0, 6, LimLine);
    sprintf(LimLine, "%04d %04d   %04d %04d", adcMinOut[0], adcMinOut[1], adcMinOut[2], adcMinOut[3]);
    LCD_P6x8Str(0, 7, LimLine);
    Ms_Delay(1000);
    LCD_P8x16Str(0, 3, "       ");
}

/// @brief 获取ADC采样值（中值滤波）
void Get_ADC()
{
    uint8_t  i, j, k;
    uint16_t temp, getADC[9];

    // 对每个ADC通道进行采样
    for (k = 0; k < 4; k++) {
        // 采样9次
        for (i = 0; i < 9; i++) {
            getADC[i] = ADC_Read_Once(ADC[adcPin[k]], ADC_12BIT);
        }

        // 冒泡排序
        for (i = 1; i <= 9 - 1; i++) {
            for (j = 0; j < 9 - i; j++) {
                if (getADC[j] > getADC[j + 1]) {
                    temp          = getADC[j];
                    getADC[j]     = getADC[j + 1];
                    getADC[j + 1] = temp;
                }
            }
        }
        adcRaw[k] = getADC[5]; // 取中值作为最终结果
    }
}

/// @brief ADC数据归一化处理
/// @return 归一化后的误差值(-1~1)
float Normalization(uint16_t *adcRaw, uint16_t *adcMax, uint16_t *adcMin)
{
    uint8_t i;
    double  adcAfterNorm[4], adcL, adcR;
    char    adcNormLine[4][22];
    uint8_t warningOfADC[4], xOfWarning[4] = {4, 9, 11, 17};

    // 对每个ADC通道进行归一化处理
    for (i = 0; i < 4; i++) {
        // 边界检查
        if (adcRaw[i] > adcMax[i]) { // 超上限
            adcRaw[i] = adcMax[i];
            GPIO_Write_Bit(LED(i), 0); // LED报警
            warningOfADC[i] = 1;
        } else if (adcRaw[i] < adcMin[i]) { // 超下限
            adcRaw[i] = adcMin[i];
            GPIO_Write_Bit(LED(i), 0);
            warningOfADC[i] = -1;
        } else {
            warningOfADC[i] = 0;
        }

        // 归一化计算：将ADC值映射到0~1范围
        adcAfterNorm[i] = (float)(adcRaw[i] - adcMin[i]) / (adcMax[i] - adcMin[i]);
        sprintf(adcNormLine[i], "%.2f", adcAfterNorm[i]);
    }

    // 显示警告信息
    if (warningOfADC[i] == 1)
        LCD_P6x8Str(xOfWarning[i], 0, "!");
    else if (warningOfADC[i] == 1)
        LCD_P6x8Str(xOfWarning[i], 1, "!");

    // 显示归一化后的值
    sprintf(adcNormLine[0], "%s %s   %s %s", adcNormLine[0], adcNormLine[1], adcNormLine[2], adcNormLine[3]);
    LCD_P6x8Str(0, 1, adcNormLine[0]);

    // 计算左右两侧传感器的平均值
    adcL = (adcAfterNorm[0]*0.5 + adcAfterNorm[1]*1.5) / 2; // 左侧平均值
    adcR = (adcAfterNorm[2]*0.5 + adcAfterNorm[3]*1.5) / 2; // 右侧平均值

    // 显示左右侧数值
    sprintf(adcNormLine[0], "%.3f->", adcL);
    LCD_P6x8Str(0, 2, adcNormLine[0]);
    sprintf(adcNormLine[0], "<-%.3f", adcR);
    LCD_P6x8Str(84, 2, adcNormLine[0]);

    // 计算并返回归一化误差：(左侧-右侧)/(左侧+右侧)
    return (adcL - adcR) / (adcL + adcR + 0.001); // +0.001防止除零
}

/// @brief PID控制器计算
/// @return PID输出值(-1~1)
float PID_Calculate(float error, float lastError, float kp, float ki, float kd, float maxAbsI)
{
    char  pidLine[50];
    float output, derivative;

    // 积分项计算（带限幅）
    sumOfError += error;
    if (sumOfError > maxAbsI)
        sumOfError = maxAbsI;
    else if (sumOfError < -maxAbsI)
        sumOfError = -maxAbsI;

    // 微分项计算
    derivative = kd * (error - lastError);

    // PID输出计算
    output = kp * error + ki * sumOfError + derivative;

    // 输出限幅(-1~1)
    if (output > 1)
        output = 1;
    else if (output < -1)
        output = -1;

    lastError = error;

    // 显示PID参数和计算过程
    sprintf(pidLine, "PID: %0.1f  %0.3f  %0.2f", kp, ki, kd);
    LCD_P6x8Str(0, 3, pidLine);
    sprintf(pidLine, "%+0.2f%+0.2f%+0.2f=%+0.2f", kp * error, ki * sumOfError, derivative, output);
    LCD_P6x8Str(0, 4, pidLine);
    sprintf(pidLine, "%04d  < SPEED <  %04d", minSpeed, maxSpeed);
    LCD_P6x8Str(0, 5, pidLine);

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
    PWM_SET_Frequency(SERVO, 50, midDutyOfServo + (int)maxChangeDuty * errorInner);
}

/// @brief 电机控制函数
void Go(uint16_t minSpeed, uint16_t maxSpeed, float currentError, float lastError)
{
    // 根据误差变化率计算速度：基础速度 + 变化率×系数
    int speed = (int)((maxSpeed - minSpeed) * abs(currentError - lastError) / 2) + minSpeed;

    // 速度限幅
    if (speed > maxSpeed)
        speed = maxSpeed;
    else if (speed < minSpeed)
        speed = minSpeed;

    // 设置左右电机PWM（注意：这里可能是反向控制）
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
    PWM_SET_Frequency(SERVO, 50, midDutyOfServo); // 舵机回中
    PWM_SET_Duty(MOTOR_L, 10000);                 // 电机停止
    PWM_SET_Duty(MOTOR_R, 10000);
}

/// @brief BIOS系统（参数调整界面）
void BIOS(uint8_t page)
{
    char biosLine[16];
    Stop_Inner(); // 进入BIOS时停止运动

    switch (page) {
    case 1: // 页面1：最大速度和Kp调整
        LCD_P8x16Str(0, 0, "     BIOS 1    ");
        LCD_P6x8Str(0, 2, "---------------------");
        sprintf(biosLine, "Max Speed = %d", maxSpeed / 10);
        LCD_P8x16Str(0, 3, biosLine);
        sprintf(biosLine, "Kp = %0.2f    ", kp);
        LCD_P8x16Str(0, 6, biosLine);
        break;
    case 2: // 页面2：最小速度和Ki调整
        LCD_P8x16Str(0, 0, "     BIOS 2    ");
        LCD_P6x8Str(0, 2, "---------------------");
        sprintf(biosLine, "Min Speed = %d", minSpeed / 10);
        LCD_P8x16Str(0, 3, biosLine);
        sprintf(biosLine, "Ki = %0.4f  ", ki);
        LCD_P8x16Str(0, 6, biosLine);
        break;
    case 3: // 页面3：积分限幅和Kd调整
        LCD_P8x16Str(0, 0, "     BIOS 3    ");
        LCD_P6x8Str(0, 2, "---------------------");
        sprintf(biosLine, "MaxI = %02.2f    ", maxAbsI);
        LCD_P8x16Str(0, 3, biosLine);
        sprintf(biosLine, "Kd = %0.2f    ", kd);
        LCD_P8x16Str(0, 6, biosLine);
        break;
    default:
        break;
    }
}

// ========================= 中断服务函数 =========================

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
                maxAbsI += 0.5;
        } else if (Port_Exti_Flag[4] & Port_Pin_6) { // 下键：减小参数
            if (biosKey % 4 == 1)
                maxSpeed -= 50;
            else if (biosKey % 4 == 2)
                minSpeed -= 50;
            else if (biosKey % 4 == 3)
                maxAbsI -= 0.5;
        } else if (Port_Exti_Flag[4] & Port_Pin_1) { // 右键：增加PID参数
            if (biosKey % 4 == 1)
                kp += 0.1;
            else if (biosKey % 4 == 2)
                ki += 0.0001;
            else if (biosKey % 4 == 3)
                kd += 0.05;
        } else if (Port_Exti_Flag[4] & Port_Pin_5) { // 左键：减小PID参数
            if (biosKey % 4 == 1)
                kp -= 0.1;
            else if (biosKey % 4 == 2)
                ki -= 0.0001;
            else if (biosKey % 4 == 3)
                kd -= 0.05;
        }
    }
}

// P2口外部中断（注释掉）
/*
void P2_EXTI_Activated() interrupt P2INT_VECTOR
{
    GPIO_EXTI_Flag_Read(GPIO_P2);
    if (Port_Exti_Flag[2]) {
        GPIO_EXTI_Flag_Clear(GPIO_P0);
        if (Port_Exti_Flag[2] & Port_Pin_7)
            biosKey++;
    }
}
*/
