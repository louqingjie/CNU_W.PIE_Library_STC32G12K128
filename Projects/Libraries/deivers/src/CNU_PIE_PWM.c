/*********************************************************************************************************************
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2023,CNU_W.PIE
 *     All rights reserved.
 *     本库函数参考逐飞科技开源的STC函数库
 *     除注明出处外，以下所有内容版权均属胖胖个人所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留PP的版权声明。
 *     Except where indicated, the copyright of all the contents below is owned by PP 
 *     and can not be used for commercial purposes without permission. 
 *     The copyright notice of PP must be preserved when modifying the content.
 *
 * @file       CNU_PIE_PWM.c
 * @brief      PWM
 * @author     胖胖
 * @version    v1.1
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#include "CNU_PIE_PWM.h"
#include "CNU_PIE_GPIO.h"

//捕获比较模式寄存器
const uint32_t PWM_CCMR_ADDR[] = {0x7efec8, 0x7efec9, 0x7efeca ,0x7efecb, 0x7efee8, 0x7efee9, 0x7efeea, 0x7efeeb};
//捕获比较使能寄存器
const uint32_t PWM_CCER_ADDR[] = {0x7efecc, 0x7efecd, 0x7efeec ,0x7efeed};
//控制寄存器,高8位地址  低8位地址 + 1即可
const uint32_t PWM_CCR_ADDR[] = {0x7efed5, 0x7efed7, 0x7efed9, 0x7efedb, 0x7efef5, 0x7efef7, 0x7efef9, 0x7efefb};	
//控制寄存器,高8位地址  低8位地址 + 1即可
const uint32_t PWM_ARR_ADDR[] = {0x7efed2,0x7efef2};

 /**************************************************************************************************************************
 * @brief  初始化PWM引脚IO模式
 * @brief  内部调用，无需关心
***************************************************************************************************************************/
void PWM_PIN_SET(PWM_CHN_PIN_enum PWM_CHN_PIN)
{
	switch(PWM_CHN_PIN)
	{
		case PWMA_CH1P_P10: GPIO_Init(GPIO_P1,GPIO_Pin_0,GPIO_OUT_PP); break;
		case PWMA_CH1N_P11: GPIO_Init(GPIO_P1,GPIO_Pin_1,GPIO_OUT_PP); break;
		case PWMA_CH1P_P20: GPIO_Init(GPIO_P2,GPIO_Pin_0,GPIO_OUT_PP); break;
		case PWMA_CH1N_P21: GPIO_Init(GPIO_P2,GPIO_Pin_1,GPIO_OUT_PP); break;
		case PWMA_CH1P_P60: GPIO_Init(GPIO_P6,GPIO_Pin_0,GPIO_OUT_PP); break;
		case PWMA_CH1N_P61: GPIO_Init(GPIO_P6,GPIO_Pin_1,GPIO_OUT_PP); break;		
		case PWMA_CH2P_P12: GPIO_Init(GPIO_P1,GPIO_Pin_2,GPIO_OUT_PP); break;
		case PWMA_CH2N_P13: GPIO_Init(GPIO_P1,GPIO_Pin_3,GPIO_OUT_PP); break;
		case PWMA_CH2P_P22: GPIO_Init(GPIO_P2,GPIO_Pin_2,GPIO_OUT_PP); break;
		case PWMA_CH2N_P23: GPIO_Init(GPIO_P2,GPIO_Pin_3,GPIO_OUT_PP); break;
		case PWMA_CH2P_P62: GPIO_Init(GPIO_P6,GPIO_Pin_2,GPIO_OUT_PP); break;
		case PWMA_CH2N_P63: GPIO_Init(GPIO_P6,GPIO_Pin_3,GPIO_OUT_PP); break;
		case PWMA_CH3P_P24: GPIO_Init(GPIO_P2,GPIO_Pin_4,GPIO_OUT_PP); break;
		case PWMA_CH3N_P25: GPIO_Init(GPIO_P2,GPIO_Pin_5,GPIO_OUT_PP); break;
		case PWMA_CH3P_P64: GPIO_Init(GPIO_P6,GPIO_Pin_4,GPIO_OUT_PP); break;
		case PWMA_CH3N_P65: GPIO_Init(GPIO_P6,GPIO_Pin_5,GPIO_OUT_PP); break;
		case PWMA_CH4P_P16: GPIO_Init(GPIO_P1,GPIO_Pin_6,GPIO_OUT_PP); break;
		case PWMA_CH4N_P17: GPIO_Init(GPIO_P1,GPIO_Pin_7,GPIO_OUT_PP); break;
		case PWMA_CH4P_P26: GPIO_Init(GPIO_P2,GPIO_Pin_6,GPIO_OUT_PP); break;
		case PWMA_CH4N_P27: GPIO_Init(GPIO_P2,GPIO_Pin_7,GPIO_OUT_PP); break;
		case PWMA_CH4P_P66: GPIO_Init(GPIO_P6,GPIO_Pin_6,GPIO_OUT_PP); break;
		case PWMA_CH4N_P67: GPIO_Init(GPIO_P6,GPIO_Pin_7,GPIO_OUT_PP); break;
		case PWMA_CH4P_P34: GPIO_Init(GPIO_P3,GPIO_Pin_4,GPIO_OUT_PP); break;
		case PWMA_CH4N_P33:	GPIO_Init(GPIO_P3,GPIO_Pin_3,GPIO_OUT_PP); break;
		case PWMB_CH1_P20:  GPIO_Init(GPIO_P2,GPIO_Pin_0,GPIO_OUT_PP); break;
		case PWMB_CH1_P17:  GPIO_Init(GPIO_P1,GPIO_Pin_7,GPIO_OUT_PP); break;
		case PWMB_CH1_P00:  GPIO_Init(GPIO_P0,GPIO_Pin_0,GPIO_OUT_PP); break;
		case PWMB_CH1_P74:  GPIO_Init(GPIO_P7,GPIO_Pin_4,GPIO_OUT_PP); break;
		case PWMB_CH2_P21:  GPIO_Init(GPIO_P2,GPIO_Pin_1,GPIO_OUT_PP); break;
		case PWMB_CH2_P54:  GPIO_Init(GPIO_P5,GPIO_Pin_4,GPIO_OUT_PP); break;
		case PWMB_CH2_P01:  GPIO_Init(GPIO_P0,GPIO_Pin_1,GPIO_OUT_PP); break;
		case PWMB_CH2_P75:  GPIO_Init(GPIO_P7,GPIO_Pin_5,GPIO_OUT_PP); break;
		case PWMB_CH3_P22:  GPIO_Init(GPIO_P2,GPIO_Pin_2,GPIO_OUT_PP); break;
		case PWMB_CH3_P33:  GPIO_Init(GPIO_P3,GPIO_Pin_3,GPIO_OUT_PP); break;
		case PWMB_CH3_P02:  GPIO_Init(GPIO_P0,GPIO_Pin_2,GPIO_OUT_PP); break;
		case PWMB_CH3_P76:  GPIO_Init(GPIO_P7,GPIO_Pin_6,GPIO_OUT_PP); break;
		case PWMB_CH4_P23:  GPIO_Init(GPIO_P2,GPIO_Pin_3,GPIO_OUT_PP); break;
		case PWMB_CH4_P34:  GPIO_Init(GPIO_P3,GPIO_Pin_4,GPIO_OUT_PP); break;
		case PWMB_CH4_P03:  GPIO_Init(GPIO_P0,GPIO_Pin_3,GPIO_OUT_PP); break;
    case PWMB_CH4_P77:  GPIO_Init(GPIO_P7,GPIO_Pin_7,GPIO_OUT_PP); break;
	}
}
 /**************************************************************************************************************************
 * @brief  PWM引脚初始化
 * @exampleCode
 *      PWM_Init(PWMA_CH2P_P62, 50, 0); //初始化P62引脚 频率50 初始占空比0
 * @endcode
 * @param[in]  PWM_CHN_PIN PWM引脚号 
 * @param[in]  frequency   PWM频率              
 * @param[in]  pwm_duty    PWM占空比
***************************************************************************************************************************/
void PWM_Init(PWM_CHN_PIN_enum PWM_CHN_PIN , uint32_t frequency , uint32_t pwm_duty)
{
	
	uint32_t match_temp;
	uint32_t period_temp;
	uint16_t Frequency_Division = 0;//分频系输
	
	P_SW2 |= 0x80;
	
	//GPIO需要设置为推挽输出
	PWM_PIN_SET(PWM_CHN_PIN);//将对应的IO引脚设置为推挽输出
	
//	//分频计算，周期计算，占空比计算
	Frequency_Division = ( system_clock / frequency ) >> 16;							//多少分频
	period_temp = system_clock / frequency ;			
	period_temp = period_temp / ( Frequency_Division +1 ) - 1;				//周期

	if(pwm_duty != PRECISION)
	{
		match_temp = period_temp * ((float)pwm_duty / PRECISION);	// 占空比			
	}
	else
	{
		match_temp = (period_temp + 1);								// duty为100%
	}
	if(PWMB_CH1_P20 <= PWM_CHN_PIN)				//PWM5-8
	{
		//通道选择，引脚选择
		PWMB_ENO |= (1 << ((2 * ((PWM_CHN_PIN >> 4) - 4))));					//使能通道	
		PWMB_PS |= ((PWM_CHN_PIN & 0x03) << ((2 * ((PWM_CHN_PIN >> 4) - 4))));		//输出脚选择
		
		// 配置通道输出使能和极性	
		(*(unsigned char volatile far *) (PWM_CCER_ADDR[PWM_CHN_PIN>>5])) |= (uint8_t)(1 << (((PWM_CHN_PIN >> 4) & 0x01) * 4));
		
		//设置预分频
		PWMB_PSCRH = (uint8_t)(Frequency_Division>>8);
		PWMB_PSCRL = (uint8_t)Frequency_Division;
		
		PWMB_BKR = 0x80; 	//主输出使能 相当于总开关
		PWMB_CR1 = 0x01;	//PWM开始计数
	}
	else
	{
		PWMA_ENO |= (1 << (PWM_CHN_PIN & 0x01)) << ((PWM_CHN_PIN >> 4) * 2);	//使能通道	
		PWMA_PS  |= ((PWM_CHN_PIN & 0x07) >> 1) << ((PWM_CHN_PIN >> 4) * 2);    //输出脚选择
		
		// 配置通道输出使能和极性	
		(*(unsigned char volatile far *) (PWM_CCER_ADDR[PWM_CHN_PIN>>5])) |= (1 << ((PWM_CHN_PIN & 0x01) * 2 + ((PWM_CHN_PIN >> 4) & 0x01) * 0x04));

		
		//设置预分频
		PWMA_PSCRH = (uint8_t)(Frequency_Division>>8);
		PWMA_PSCRL = (uint8_t)Frequency_Division;

		PWMA_BKR = 0x80; 	// 主输出使能 相当于总开关
		PWMA_CR1 = 0x01;	//PWM开始计数
	}
	
	//周期
	(*(unsigned char volatile far *) (PWM_ARR_ADDR[PWM_CHN_PIN>>6])) = (uint8_t)(period_temp>>8);		//高8位
	(*(unsigned char volatile far *) (PWM_ARR_ADDR[PWM_CHN_PIN>>6] + 1)) = (uint8_t)period_temp;		//低8位

	//设置捕获值|比较值
	(*(unsigned char volatile far *) (PWM_CCR_ADDR[PWM_CHN_PIN>>4]))		= match_temp>>8;			//高8位
	(*(unsigned char volatile far *) (PWM_CCR_ADDR[PWM_CHN_PIN>>4] + 1))  = (uint8_t)match_temp;		//低8位
	
	//功能设置
	(*(unsigned char volatile far *) (PWM_CCMR_ADDR[PWM_CHN_PIN>>4])) |= 0x06<<4;		//设置为PWM模式1
	(*(unsigned char volatile far *) (PWM_CCMR_ADDR[PWM_CHN_PIN>>4])) |= 1<<3;		//开启PWM寄存器的预装载功
}
 /**************************************************************************************************************************
 * @brief  PWM引脚设置占空比
 * @exampleCode
 *      PWM_SET_Duty(PWMA_CH2P_P62, 1000); //设置P62引脚 占空比1000
 * @endcode
 * @param[in]  PWM_CHN_PIN PWM引脚号             
 * @param[in]  pwm_duty    PWM占空比
***************************************************************************************************************************/
void PWM_SET_Duty(PWM_CHN_PIN_enum PWM_CHN_PIN , uint32_t pwm_duty)
{
	uint32_t match_temp;
	uint32_t arrange = ((*(unsigned char volatile far *) (PWM_ARR_ADDR[PWM_CHN_PIN>>6]))<<8) | (*(unsigned char volatile far *) (PWM_ARR_ADDR[PWM_CHN_PIN>>6] + 1 ));
	
	P_SW2 |= 0x80;//确定使能访问XFR
	
	if(pwm_duty != PRECISION)
	{
		match_temp = arrange * ((float)pwm_duty/PRECISION);				//占空比
	}
	else
	{
		match_temp = arrange + 1;
	}
	//设置捕获值|比较值
	(*(unsigned char volatile far *) (PWM_CCR_ADDR[PWM_CHN_PIN>>4]))		= match_temp>>8;			//高8位
	(*(unsigned char volatile far *) (PWM_CCR_ADDR[PWM_CHN_PIN>>4] + 1))  = (uint8_t)match_temp;		//低8位
	
}
 /**************************************************************************************************************************
 * @brief  PWM引脚设置频率
 * @exampleCode
 *      PWM_SET_Frequency (PWMB_CH3_P33,200,5000); //设置P33引脚 频率200 占空比5000
 * @endcode
 * @param[in]  PWM_CHN_PIN PWM引脚号 
 * @param[in]  frequency   PWM频率              
 * @param[in]  pwm_duty    PWM占空比
***************************************************************************************************************************/
void PWM_SET_Frequency(PWM_CHN_PIN_enum PWM_CHN_PIN, uint32_t frequency, uint32_t pwm_duty )
{
	uint32_t match_temp;
	uint32_t period_temp; 
	uint16_t Frequency_Division = 0;//分频系输
	
	P_SW2 |= 0x80;//确定使能访问XFR
	
	//分频计算，周期计算，占空比计算
	Frequency_Division = (FOSC / frequency) >> 16;							    //分频
	period_temp = FOSC / frequency ;			
	period_temp = period_temp / (Frequency_Division + 1) - 1;				//周期

	if(pwm_duty != PRECISION)//判断占空比是否超最大精度
	{
		match_temp = period_temp * ((float)pwm_duty / PRECISION);	// 占空比			
	}
	else
	{
		match_temp = period_temp + 1;								          //否则占空比为最大
	}
	
	if(PWMB_CH1_P20 <= PWM_CHN_PIN)//PWMA
	{
		//设置预分频
		PWMB_PSCRH = (uint8_t)(Frequency_Division>>8);
		PWMB_PSCRL = (uint8_t)Frequency_Division;
	}
	else//PWMB
	{
		//设置预分频
		PWMA_PSCRH = (uint8_t)(Frequency_Division>>8);
		PWMA_PSCRL = (uint8_t)Frequency_Division;
	}
	
	//周期
	(*(unsigned char volatile far *) (PWM_ARR_ADDR[PWM_CHN_PIN>>6])) = (uint8_t)(period_temp>>8);		//高8位
	(*(unsigned char volatile far *) (PWM_ARR_ADDR[PWM_CHN_PIN>>6] + 1)) = (uint8_t)period_temp;		//低8位

	//设置捕获值|比较值
	(*(unsigned char volatile far *) (PWM_CCR_ADDR[PWM_CHN_PIN>>4]))		= match_temp>>8;			//高8位
	(*(unsigned char volatile far *) (PWM_CCR_ADDR[PWM_CHN_PIN>>4] + 1))  = (uint8_t)match_temp;		//低8位
}


