#include "BMI088Middleware.h"
#include "common.h"


void BMI088_GPIO_init(void)
{

}

void BMI088_com_init(void)
{


}

void BMI088_delay_ms(uint16_t ms)
{

    Ms_Delay(ms);
}

void BMI088_delay_us(uint16_t us)
{
    Us_Delay(us);
}
void BMI088_ACCEL_NS_L(void)
{
    P22 = 0;//根据硬件修改
}
void BMI088_ACCEL_NS_H(void)
{
    P22 = 1;//根据硬件修改
}
void BMI088_GYRO_NS_L(void)
{
    P27 = 0;//根据硬件修改
}
void BMI088_GYRO_NS_H(void)
{
    P27 = 1;//根据硬件修改
}

uint8_t BMI088_read_write_byte(uint8_t txdata)
{
    SPDAT = txdata;				       	//DATA寄存器赋值
    while (!(SPSTAT & 0x80));  		//查询完成标志
    SPSTAT = 0xc0;                //清中断标志
	  return SPDAT;
}

