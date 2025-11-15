#include "main.h"
#include <MATH.H>

// ???
float    kp             = 0.5;
float    ki             = 0.1;
float    kd             = 0.1;
float    maxAbsI        = 5;  // ?????
uint8_t  timerTimes     = 5;  // ???????
uint8_t  timerMs        = 10; // ???????
uint16_t maxSpeed       = 1000;
uint16_t minSpeed       = 800;
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
uint16_t adcMaxOut[4], adcMinOut[4], adcRaw[4];
uint8_t  times;
float    currentError, pidOut, lastError;

// ???
void  All_Init();
void  beep(uint16_t freq, uint16_t duty, uint16_t beepTime, uint16_t sleepTime);
void  ADC_Norm_Slow(uint16_t *adcMax, uint16_t *adcMin);
void  Get_ADC(uint16_t *adcRaw);
float Normalization(uint16_t *adcRaw, uint16_t *adcMax, uint16_t *adcMin);
float PID_Calculate(float error, float lastError, float kp, float ki, float kd, float maxAbsI);
void Uart_Send_Message(uint16_t *adcRaw);
void  Servo_Control_By_Error(float errorInner, uint16_t midDutyOfServo, uint16_t maxChangeDuty);
void  Go(uint16_t minSpeed, uint16_t maxSpeed, float currentError, float lastError);

void main()
{
    All_Init();
    ADC_Norm_Slow(adcMaxOut, adcMinOut);
    PIT_Timer_Ms(TIM0, timerMs);
    while (1) {
        Get_ADC(adcRaw);
        currentError = Normalization(adcRaw, adcMaxOut, adcMinOut);
        pidOut       = PID_Calculate(currentError, lastError, kp, ki, kd, maxAbsI);
        Go(minSpeed, maxSpeed, currentError, lastError);
    }
}

void PIT0_Activated() interrupt TMR0_VECTOR
{
    times++;
    if (times >= (timerTimes - 1)) {
        Servo_Control_By_Error(pidOut, midDutyOfServo, maxChangeDuty);
        times = 0;
        Uart_Send_Message(adcRaw);
    }
}

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

/// @brief ?ADC?????
void Get_ADC(uint16_t *adcRaw)
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
}

/// @brief ???,????
float Normalization(uint16_t *adcRaw, uint16_t *adcMax, uint16_t *adcMin)
{
    uint8_t i;
    double  adcAfterNorm[4], adcL, adcR;

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
    }
    adcL = sqrt(adcAfterNorm[0] * adcAfterNorm[1]);
    adcR = sqrt(adcAfterNorm[2] * adcAfterNorm[3]);
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

void Uart_Send_Message(uint16_t *adcRaw)
{
    UART_PutStr(UART_1, "test");
}

void Servo_Control_By_Error(float errorInner, uint16_t midDutyOfServo, uint16_t maxChangeDuty)
{
    if (errorInner > 1)
        errorInner = 1;
    else if (errorInner < -1)
        errorInner = -1;
    PWM_SET_Duty(SERVO, midDutyOfServo + (int)maxChangeDuty * errorInner);
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
