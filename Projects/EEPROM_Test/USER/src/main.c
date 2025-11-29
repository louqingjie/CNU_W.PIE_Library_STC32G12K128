#include "main.h"
#include "STC32G_EEPROM.h"
uint16_t memvalue, lastMemValue, countOfTimer;
uint8_t *addrOfMem;
void     Nixie_Show_One_Num(char num, uint8_t bitNum)
{
    switch (num) {
    case 0:
        P0 = 0x3F;
        break;
    case 1:
        P0 = 0x06;
        break;
    case 2:
        P0 = 0x5b;
        break;
    case 3:
        P0 = 0x4f;
        break;
    case 4:
        P0 = 0x66;
        break;
    case 5:
        P0 = 0x6d;
        break;
    case 6:
        P0 = 0x7d;
        break;
    case 7:
        P0 = 0x07;
        break;
    case 8:
        P0 = 0xFF;
        break;
    case 9:
        P0 = 0x6f;
        break;
    default:
        break;
    }
    switch (bitNum) {
    case 1:
        P22 = 1;
        P23 = 1;
        P24 = 1;
        break;
    case 2:
        P22 = 0;
        P23 = 1;
        P24 = 1;
        break;
    case 3:
        P22 = 1;
        P23 = 0;
        P24 = 1;
        break;
    case 4:
        P22 = 0;
        P23 = 0;
        P24 = 1;
        break;
    default:
        break;
    }
}
void Nixie_Show_Four_Num(uint16_t num)
{
    if (num > 9999) {
        num = 9999;
    }
    Nixie_Show_One_Num((int)num / 1000, 1);
    Ms_Delay(1);
    Nixie_Show_One_Num((int)(num / 100) % 10, 2);
    Ms_Delay(1);
    Nixie_Show_One_Num((int)(num / 10) % 10, 3);
    Ms_Delay(1);
    Nixie_Show_One_Num((int)num % 10, 4);
    Ms_Delay(1);
}

void main()
{
    Board_Init();
    addrOfMem = &memvalue;
    EEPROM_read_n(0, addrOfMem, 2);
    GPIO_Init(GPIO_P0, GPIO_Pin_All, GPIO_OUT_PP);
    GPIO_Init(GPIO_P2, GPIO_Pin_All, GPIO_OUT_PP);
    GPIO_EXTI_Init(GPIO_P3, GPIO_Pin_1, FALLING_EDGE);
    GPIO_EXTI_Open(GPIO_P3, GPIO_Pin_1);
    GPIO_EXTI_Init(GPIO_P3, GPIO_Pin_0, FALLING_EDGE);
    GPIO_EXTI_Open(GPIO_P3, GPIO_Pin_0);
    PIT_Timer_Ms(TIM0, 10);
    while (1) {
        if (GPIO_Read_Bit(GPIO_P3, GPIO_Pin_2) == 0) {
            memvalue++;
            while (GPIO_Read_Bit(GPIO_P3, GPIO_Pin_2) == 0)
                Nixie_Show_Four_Num(memvalue);
        }
        if (GPIO_Read_Bit(GPIO_P3, GPIO_Pin_3) == 0) {
            memvalue--;
            while (GPIO_Read_Bit(GPIO_P3, GPIO_Pin_3) == 0)
                Nixie_Show_Four_Num(memvalue);
        }
        Nixie_Show_Four_Num(memvalue);
        if (countOfTimer >= 100) {
            countOfTimer = 0;
            memvalue++;
        }
        if (lastMemValue != memvalue) {
            EEPROM_SectorErase(0);
            EEPROM_write_n(0, addrOfMem, 2);
        }
        lastMemValue = memvalue;    
    }
}

void PIT_Activated() interrupt TMR0_VECTOR
{
    PIT_Timer_Clear(TIM0);
    countOfTimer++;
}