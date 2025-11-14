#ifndef __PIN_H__
#define __PIN_H__

#include "main.h"

/// @brief 8个ADC针脚
char ADC[8] = {ADC_P13, ADC_P16, ADC_P10, ADC_P11,
               ADC_P02, ADC_P05, ADC_P00, ADC_P01};

/// @brief 舵机
#define SERVO PWMB_CH1_P74

/// @brief 左侧电机调速引脚
#define MOTOR_L PWMA_CH2P_P62
/// @brief 右侧电机调速引脚
#define MOTOR_R PWMA_CH3P_P64
/// @brief 左侧电机基准引脚
#define MOTOR_BASE_L PWMA_CH1P_P60
/// @brief 左侧电机基准引脚
#define MOTOR_BASE_R PWMA_CH4P_P66

/// @brief 四颗LED灯
char LEDPin[4] = {GPIO_Pin_7, GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_4};
#define LED(x) GPIO_P3, LEDPin[x]

/// @brief 两个按键
char KEYPin[2] = {GPIO_Pin_7, GPIO_Pin_6};
#define KEY(x) GPIO_P0, KEYPin[x]

/// @brief 十字键
#define DPAD_UP GPIO_P4, GPIO_P2
#define DPAD_DOWN GPIO_P4, GPIO_Pin_6
#define DPAD_LEFT GPIO_P4, GPIO_Pin_5
#define DPAD_RIGHT GPIO_P4, GPIO_Pin_1
#define DPAD_MID GPIO_P2, GPIO_P7

/// @brief 蜂鸣器
#define BUZZER PWMB_CH3_P33

#endif