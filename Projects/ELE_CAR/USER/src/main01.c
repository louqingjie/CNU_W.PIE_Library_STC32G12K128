#include "main.h"
uint8_t i;
void    main()
{
    Board_Init();
    GPIO_Init(GPIO_P3, GPIO_Pin_7, GPIO_OUT_PP);
    GPIO_EXTI_Init(GPIO_P2, GPIO_Pin_7, FALLING_EDGE);
    GPIO_EXTI_Open(GPIO_P2, GPIO_Pin_7);
    while (1) {
        if (i % 2 == 1)
            GPIO_Write_Bit(GPIO_P3, GPIO_Pin_7, 0);
        else
            GPIO_Write_Bit(GPIO_P3, GPIO_Pin_7, 1);
    }
}

void P2_EXTI_Activated() interrupt P2INT_VECTOR
{
    GPIO_EXTI_Flag_Read(GPIO_P2);
    if (Port_Exti_Flag[2]) {
        GPIO_EXTI_Flag_Clear(GPIO_P2);
        if (Port_Exti_Flag[2] & Port_Pin_7)
            i++;
    }
}