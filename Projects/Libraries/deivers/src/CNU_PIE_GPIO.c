/*********************************************************************************************************************
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2023,CNU_W.PIE
 *     All rights reserved.
 *
 *     除注明出处外，以下所有内容版权均属胖胖个人所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留PP的版权声明。
 *     Except where indicated, the copyright of all the contents below is owned by PP 
 *     and can not be used for commercial purposes without permission. 
 *     The copyright notice of PP must be preserved when modifying the content.
 *
 * @file       CNU_PIE_GPIO.c
 * @brief      GPIO
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
 #include "CNU_PIE_GPIO.h"
 
 /**************************************************************************************************************************
 * @brief  初始化GPIO引脚
 * @exampleCode
				GPIO_Init(GPIO_P0, GPIO_Pin_0, GPIO_PullUp); //初始化P00引脚并设置为准双向IO
 * @endcode
 * @param[in]  GPIO_Port GPIO端口号
 * @param[in]  GPIO_Pin  GPIO引脚号              
 * @param[in]  GPIO_Mode GPIO引脚配置
***************************************************************************************************************************/
void GPIO_Init(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin , GPIO_Mode_enum GPIO_Mode)
 {
	 switch (GPIO_Port)
	 {
		 case GPIO_P0://端口0
			 switch (GPIO_Mode)
			 {
				 case GPIO_PullUp:
					 P0M1 &= ~GPIO_Pin,	P0M0 &= ~GPIO_Pin;	 break;//上拉准双向口			 
				 case GPIO_HighZ:
					 P0M1 |=  GPIO_Pin,	P0M0 &= ~GPIO_Pin;   break;//高阻输入
				 case GPIO_OUT_OD:
					 P0M1 |=  GPIO_Pin,	P0M0 |=  GPIO_Pin;	 break;//开漏输出
				 case GPIO_OUT_PP:
					 P0M1 &= ~GPIO_Pin,	P0M0 |=  GPIO_Pin;   break;//推挽输出
				 default:
				 break;//初始化失败
			 }break;
     case GPIO_P1://端口1
			 switch (GPIO_Mode)
			 {
				 case GPIO_PullUp:
					 P1M1 &= ~GPIO_Pin,	P1M0 &= ~GPIO_Pin;	 break;//上拉准双向口			 
				 case GPIO_HighZ:
					 P1M1 |=  GPIO_Pin,	P1M0 &= ~GPIO_Pin;   break;//高阻输入
				 case GPIO_OUT_OD:
					 P1M1 |=  GPIO_Pin,	P1M0 |=  GPIO_Pin;	 break;//开漏输出
				 case GPIO_OUT_PP:
					 P1M1 &= ~GPIO_Pin,	P1M0 |=  GPIO_Pin;   break;//推挽输出
				 default:
				 break;//初始化失败
			 }break;		
     case GPIO_P2://端口2
			 switch (GPIO_Mode)
			 {
				 case GPIO_PullUp:
					 P2M1 &= ~GPIO_Pin,	P2M0 &= ~GPIO_Pin;	 break;//上拉准双向口			 
				 case GPIO_HighZ:
					 P2M1 |=  GPIO_Pin,	P2M0 &= ~GPIO_Pin;   break;//高阻输入
				 case GPIO_OUT_OD:
					 P2M1 |=  GPIO_Pin,	P2M0 |=  GPIO_Pin;	 break;//开漏输出
				 case GPIO_OUT_PP:
					 P2M1 &= ~GPIO_Pin,	P2M0 |=  GPIO_Pin;   break;//推挽输出
				 default:
				 break;//初始化失败
			 }break;		
     case GPIO_P3://端口3
			 switch (GPIO_Mode)
			 {
				 case GPIO_PullUp:
					 P3M1 &= ~GPIO_Pin,	P3M0 &= ~GPIO_Pin;	 break;//上拉准双向口			 
				 case GPIO_HighZ:
					 P3M1 |=  GPIO_Pin,	P3M0 &= ~GPIO_Pin;   break;//高阻输入
				 case GPIO_OUT_OD:
					 P3M1 |=  GPIO_Pin,	P3M0 |=  GPIO_Pin;	 break;//开漏输出
				 case GPIO_OUT_PP:
					 P3M1 &= ~GPIO_Pin,	P3M0 |=  GPIO_Pin;   break;//推挽输出
				 default:
				 break;//初始化失败
			 }break;	
     case GPIO_P4://端口4
			 switch (GPIO_Mode)
			 {
				 case GPIO_PullUp:
					 P4M1 &= ~GPIO_Pin,	P4M0 &= ~GPIO_Pin;	 break;//上拉准双向口			 
				 case GPIO_HighZ:
					 P4M1 |=  GPIO_Pin,	P4M0 &= ~GPIO_Pin;   break;//高阻输入
				 case GPIO_OUT_OD:
					 P4M1 |=  GPIO_Pin,	P4M0 |=  GPIO_Pin;	 break;//开漏输出
				 case GPIO_OUT_PP:
					 P4M1 &= ~GPIO_Pin,	P4M0 |=  GPIO_Pin;   break;//推挽输出
				 default:
				 break;//初始化失败
			 }break;	
     case GPIO_P5://端口5
			 switch (GPIO_Mode)
			 {
				 case GPIO_PullUp:
					 P5M1 &= ~GPIO_Pin,	P5M0 &= ~GPIO_Pin;	 break;//上拉准双向口			 
				 case GPIO_HighZ:
					 P5M1 |=  GPIO_Pin,	P5M0 &= ~GPIO_Pin;   break;//高阻输入
				 case GPIO_OUT_OD:
					 P5M1 |=  GPIO_Pin,	P5M0 |=  GPIO_Pin;	 break;//开漏输出
				 case GPIO_OUT_PP:
					 P5M1 &= ~GPIO_Pin,	P5M0 |=  GPIO_Pin;   break;//推挽输出
				 default:
		     break;//初始化失败
			 }break;		
     case GPIO_P6://端口6
			 switch (GPIO_Mode)
			 {
				 case GPIO_PullUp:
					 P6M1 &= ~GPIO_Pin,	P6M0 &= ~GPIO_Pin;	 break;//上拉准双向口			 
				 case GPIO_HighZ:
					 P6M1 |=  GPIO_Pin,	P6M0 &= ~GPIO_Pin;   break;//高阻输入
				 case GPIO_OUT_OD:
					 P6M1 |=  GPIO_Pin,	P6M0 |=  GPIO_Pin;	 break;//开漏输出
				 case GPIO_OUT_PP:
					 P6M1 &= ~GPIO_Pin,	P6M0 |=  GPIO_Pin;   break;//推挽输出
				 default:
				 break;//初始化失败
			 }break;
     case GPIO_P7://端口7
			 switch (GPIO_Mode)
			 {
				 case GPIO_PullUp:
					 P7M1 &= ~GPIO_Pin,	P7M0 &= ~GPIO_Pin;	 break;//上拉准双向口			 
				 case GPIO_HighZ:
					 P7M1 |=  GPIO_Pin,	P7M0 &= ~GPIO_Pin;   break;//高阻输入
				 case GPIO_OUT_OD:
					 P7M1 |=  GPIO_Pin,	P7M0 |=  GPIO_Pin;	 break;//开漏输出
				 case GPIO_OUT_PP:
					 P7M1 &= ~GPIO_Pin,	P7M0 |=  GPIO_Pin;   break;//推挽输出
				 default:
				 break;//初始化失败
			 }break;
		 default:
			break;				 
	 }
 }
 /**************************************************************************************************************************
 * @brief  设置GPIO引脚上拉电阻 -4.1k
 * @exampleCode
 *      uint8_t status ; //用于存储初始化状态
 *      status = GPIO_PinPullConfig(GPIO_P0, GPIO_Pin_0, GPIO_Pull_Up); //设置P00引脚上拉4.1k电阻
 * @endcode
 * @param[in]  GPIO_Port GPIO端口号
 * @param[in]  GPIO_Pin  GPIO引脚号              
 * @param[in]  GPIO_Pin_Config GPIO引脚是否上拉电阻
 * @retval 0 失败
 * @retval 1 成功
***************************************************************************************************************************/
uint8_t GPIO_PinPullConfig(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin , GPIO_PinConfig GPIO_Pin_Config)
 {
	 if(GPIO_Port > GPIO_P7)             return FAIL; //初始化错误值返回FAIL
	 if(GPIO_Pin  > GPIO_Pin_All)        return FAIL; //初始化错误值返回FAIL
	 if(GPIO_Pin_Config > GPIO_NO_PULL)  return FAIL; //初始化错误值返回FAIL
	 
	 switch (GPIO_Port)
	 {
		 case GPIO_P0://端口0
			 switch (GPIO_Pin_Config)
			 {
				 case GPIO_NO_PULL:
					 P0PU &= ~GPIO_Pin;	 break;//引脚不配置上拉电阻	 
				 case GPIO_Pull_Up:
					 P0PU |=  GPIO_Pin;   break;//引脚配置上拉电阻
				 default:
					 return FAIL; break;//初始化失败
			 }break;
     case GPIO_P1://端口1
			 switch (GPIO_Pin_Config)
			 {
				 case GPIO_NO_PULL:
					 P1PU &= ~GPIO_Pin;	 break;
				 case GPIO_Pull_Up:
					 P1PU |=  GPIO_Pin;   break;
				 default:
					 return FAIL; break;
			 }break;		
     case GPIO_P2://端口2
			 switch (GPIO_Pin_Config)
			 {
				 case GPIO_NO_PULL:
					 P2PU &= ~GPIO_Pin;	 break;
				 case GPIO_Pull_Up:
					 P2PU |=  GPIO_Pin;   break;
				 default:
					 return FAIL; break;
			 }break;		
     case GPIO_P3://端口3
			 switch (GPIO_Pin_Config)
			 {
				 case GPIO_NO_PULL:
					 P3PU &= ~GPIO_Pin;	 break;
				 case GPIO_Pull_Up:
					 P3PU |=  GPIO_Pin;   break;
				 default:
					 return FAIL; break;
			 }break;	
     case GPIO_P4://端口4
			 switch (GPIO_Pin_Config)
			 {
				 case GPIO_NO_PULL:
					 P4PU &= ~GPIO_Pin;	 break;
				 case GPIO_Pull_Up:
					 P4PU |=  GPIO_Pin;   break;
				 default:
					 return FAIL; break;
			 }break;	
     case GPIO_P5://端口5
			 switch (GPIO_Pin_Config)
			 {
				 case GPIO_NO_PULL:
					 P5PU &= ~GPIO_Pin;	 break;
				 case GPIO_Pull_Up:
					 P5PU |=  GPIO_Pin;   break;
				 default:
					 return FAIL; break;
			 }break;		
     case GPIO_P6://端口6
			 switch (GPIO_Pin_Config)
			 {
				 case GPIO_NO_PULL:
					 P6PU &= ~GPIO_Pin;	 break;
				 case GPIO_Pull_Up:
					 P6PU |=  GPIO_Pin;   break;
				 default:
					 return FAIL; break;
			 }break;
     case GPIO_P7://端口7
			 switch (GPIO_Pin_Config)
			 {
				 case GPIO_NO_PULL:
					 P7PU &= ~GPIO_Pin;	 break;
				 case GPIO_Pull_Up:
					 P7PU |=  GPIO_Pin;   break;
				 default:
					 return FAIL; break;
			 }break;
		 default:
			 return FAIL; break;				 
	 }
	return SUCCEED;	//成功
 }
 /**************************************************************************************************************************
 * @brief  设置GPIO引脚电平
 * @exampleCode
 *     	GPIO_Write_Bit(GPIO_P0, GPIO_Pin_0, 0);   //设置P00引脚为低电平
 * @endcode
 * @param[in]  GPIO_Port GPIO端口号
 * @param[in]  GPIO_Pin  GPIO引脚号              
 * @param[in]  data_t    GPIO引脚电平
***************************************************************************************************************************/
void GPIO_Write_Bit(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin , uint8_t data_t)
 {
	 
	 switch (GPIO_Port)
	 {
		 case GPIO_P0://端口0
			 switch (data_t)
			 {
				 case GPIO_LOW:
					 P0 &= ~GPIO_Pin;	 break;//引脚电平拉低	 
				 case GPIO_HIGH:
					 P0 |=  GPIO_Pin;   break;//引脚电平拉高
				 default:
					 break;//初始化失败
			 }break;
     case GPIO_P1://端口1
			 switch (data_t)
			 {
				 case GPIO_LOW:
					 P1 &= ~GPIO_Pin;	 break;
				 case GPIO_HIGH:
					 P1 |=  GPIO_Pin;   break;
				 default:
				 break;
			 }break;		
     case GPIO_P2://端口2
			 switch (data_t)
			 {
				 case GPIO_LOW:
					 P2 &= ~GPIO_Pin;	 break;
				 case GPIO_HIGH:
					 P2 |=  GPIO_Pin;   break;
				 default:
					break;
			 }break;		
     case GPIO_P3://端口3
			 switch (data_t)
			 {
				 case GPIO_LOW:
					 P3 &= ~GPIO_Pin;	 break;
				 case GPIO_HIGH:
					 P3 |=  GPIO_Pin;   break;
				 default:
					break;
			 }break;	
     case GPIO_P4://端口4
			 switch (data_t)
			 {
				 case GPIO_LOW:
					 P4 &= ~GPIO_Pin;	 break;
				 case GPIO_HIGH:
					 P4 |=  GPIO_Pin;   break;
				 default:
					break;
			 }break;	
     case GPIO_P5://端口5
			 switch (data_t)
			 {
				 case GPIO_LOW:
					 P5 &= ~GPIO_Pin;	 break;
				 case GPIO_HIGH:
					 P5 |=  GPIO_Pin;   break;
				 default:
					break;
			 }break;		
     case GPIO_P6://端口6
			 switch (data_t)
			 {
				 case GPIO_LOW:
					 P6 &= ~GPIO_Pin;	 break;
				 case GPIO_HIGH:
					 P6 |=  GPIO_Pin;   break;
				 default:
					 break;
			 }break;
     case GPIO_P7://端口7
			 switch (data_t)
			 {
				 case GPIO_LOW:
					 P7 &= ~GPIO_Pin;	 break;
				 case GPIO_HIGH:
					 P7 |=  GPIO_Pin;   break;
				 default:
					 break;
			 }break;
		   default:
			 break;				 
	 }	
 }
 /**************************************************************************************************************************
 * @brief  读取GPIO引脚电平
 * @exampleCode
 *      uint8_t status ; //用于存储引脚电平
 *      status = GPIO_Read_Bit(GPIO_P0, GPIO_Pin_0); //读取P00引脚电平
 * @endcode
 * @param[in]  GPIO_Port GPIO端口号
 * @param[in]  GPIO_Pin  GPIO引脚号              
 * @retval 0 低电平
 * @retval 1 高电平
***************************************************************************************************************************/
uint8_t GPIO_Read_Bit(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin)
 {
	 uint8_t Bit_Value;
	 switch (GPIO_Port)
	 {
		 case GPIO_P0://端口0
			 switch (GPIO_Pin)
			 {
				 case GPIO_Pin_0: Bit_Value = P00; return Bit_Value; break;
				 case GPIO_Pin_1: Bit_Value = P01; return Bit_Value; break;
				 case GPIO_Pin_2: Bit_Value = P02; return Bit_Value; break;
				 case GPIO_Pin_3: Bit_Value = P03; return Bit_Value; break;
				 case GPIO_Pin_4: Bit_Value = P04; return Bit_Value; break;
				 case GPIO_Pin_5: Bit_Value = P05; return Bit_Value; break;
				 case GPIO_Pin_6: Bit_Value = P06; return Bit_Value; break;
				 case GPIO_Pin_7: Bit_Value = P07; return Bit_Value; break;
				 default:  return FAIL ;break;	
			 }break;
     case GPIO_P1://端口1
			 switch (GPIO_Pin)
			 {
				 case GPIO_Pin_0: Bit_Value = P10; return Bit_Value; break;
				 case GPIO_Pin_1: Bit_Value = P11; return Bit_Value; break;
				 case GPIO_Pin_2: Bit_Value = P12; return Bit_Value; break;
				 case GPIO_Pin_3: Bit_Value = P13; return Bit_Value; break;
				 case GPIO_Pin_4: Bit_Value = P14; return Bit_Value; break;
				 case GPIO_Pin_5: Bit_Value = P15; return Bit_Value; break;
				 case GPIO_Pin_6: Bit_Value = P16; return Bit_Value; break;
				 case GPIO_Pin_7: Bit_Value = P17; return Bit_Value; break;
				 default:  return FAIL ;break;	
			 }break;			 
    case GPIO_P2://端口2
			 switch (GPIO_Pin)
			 {
				 case GPIO_Pin_0: Bit_Value = P20; return Bit_Value; break;
				 case GPIO_Pin_1: Bit_Value = P21; return Bit_Value; break;
				 case GPIO_Pin_2: Bit_Value = P22; return Bit_Value; break;
				 case GPIO_Pin_3: Bit_Value = P23; return Bit_Value; break;
				 case GPIO_Pin_4: Bit_Value = P24; return Bit_Value; break;
				 case GPIO_Pin_5: Bit_Value = P25; return Bit_Value; break;
				 case GPIO_Pin_6: Bit_Value = P26; return Bit_Value; break;
				 case GPIO_Pin_7: Bit_Value = P27; return Bit_Value; break;
				 default:  return FAIL ;break;	
			 }break;
    case GPIO_P3://端口3
			 switch (GPIO_Pin)
			 {
				 case GPIO_Pin_0: Bit_Value = P30; return Bit_Value; break;
				 case GPIO_Pin_1: Bit_Value = P31; return Bit_Value; break;
				 case GPIO_Pin_2: Bit_Value = P32; return Bit_Value; break;
				 case GPIO_Pin_3: Bit_Value = P33; return Bit_Value; break;
				 case GPIO_Pin_4: Bit_Value = P34; return Bit_Value; break;
				 case GPIO_Pin_5: Bit_Value = P35; return Bit_Value; break;
				 case GPIO_Pin_6: Bit_Value = P36; return Bit_Value; break;
				 case GPIO_Pin_7: Bit_Value = P37; return Bit_Value; break;
				 default:  return FAIL ;break;	
			 }break;	     
    case GPIO_P4://端口4
			 switch (GPIO_Pin)
			 {
				 case GPIO_Pin_0: Bit_Value = P40; return Bit_Value; break;
				 case GPIO_Pin_1: Bit_Value = P41; return Bit_Value; break;
				 case GPIO_Pin_2: Bit_Value = P42; return Bit_Value; break;
				 case GPIO_Pin_3: Bit_Value = P43; return Bit_Value; break;
				 case GPIO_Pin_4: Bit_Value = P44; return Bit_Value; break;
				 case GPIO_Pin_5: Bit_Value = P45; return Bit_Value; break;
				 case GPIO_Pin_6: Bit_Value = P46; return Bit_Value; break;
				 case GPIO_Pin_7: Bit_Value = P47; return Bit_Value; break;
				 default:  return FAIL ;break;	
			 }break;	   
    case GPIO_P5://端口5
			 switch (GPIO_Pin)
			 {
				 case GPIO_Pin_0: Bit_Value = P50; return Bit_Value; break;
				 case GPIO_Pin_1: Bit_Value = P51; return Bit_Value; break;
				 case GPIO_Pin_2: Bit_Value = P52; return Bit_Value; break;
				 case GPIO_Pin_3: Bit_Value = P53; return Bit_Value; break;
				 case GPIO_Pin_4: Bit_Value = P54; return Bit_Value; break;
				 case GPIO_Pin_5: Bit_Value = P55; return Bit_Value; break;
				 default:  return FAIL ;break;	
			 }break;			
    case GPIO_P6://端口6
			 switch (GPIO_Pin)
			 {
				 case GPIO_Pin_0: Bit_Value = P60; return Bit_Value; break;
				 case GPIO_Pin_1: Bit_Value = P61; return Bit_Value; break;
				 case GPIO_Pin_2: Bit_Value = P62; return Bit_Value; break;
				 case GPIO_Pin_3: Bit_Value = P63; return Bit_Value; break;
				 case GPIO_Pin_4: Bit_Value = P64; return Bit_Value; break;
				 case GPIO_Pin_5: Bit_Value = P65; return Bit_Value; break;
				 case GPIO_Pin_6: Bit_Value = P66; return Bit_Value; break;
				 case GPIO_Pin_7: Bit_Value = P67; return Bit_Value; break;
				 default:  return FAIL ;break;	
			 }break;
    case GPIO_P7://端口6
			 switch (GPIO_Pin)
			 {
				 case GPIO_Pin_0: Bit_Value = P70; return Bit_Value; break;
				 case GPIO_Pin_1: Bit_Value = P71; return Bit_Value; break;
				 case GPIO_Pin_2: Bit_Value = P72; return Bit_Value; break;
				 case GPIO_Pin_3: Bit_Value = P73; return Bit_Value; break;
				 case GPIO_Pin_4: Bit_Value = P74; return Bit_Value; break;
				 case GPIO_Pin_5: Bit_Value = P75; return Bit_Value; break;
				 case GPIO_Pin_6: Bit_Value = P76; return Bit_Value; break;
				 case GPIO_Pin_7: Bit_Value = P77; return Bit_Value; break;
				 default:  return FAIL ;break;	
			 }break;
	 		 default:  return FAIL ;break;				 
   }
}
 /**************************************************************************************************************************
 * @brief  翻转GPIO引脚电平
 * @exampleCode
 *      GPIO_Toggle_Bit(GPIO_P0, GPIO_Pin_0);   //翻转P00引脚电平
 * @endcode
 * @param[in]  GPIO_Port GPIO端口号
 * @param[in]  GPIO_Pin  GPIO引脚号              
***************************************************************************************************************************/
void GPIO_Toggle_Bit(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin)
 {
	
	 
	 switch (GPIO_Port)
	 {
		 case GPIO_P0:  P0 ^= GPIO_Pin ; break;	//端口0			
     case GPIO_P1:  P1 ^= GPIO_Pin ; break;	//端口1			
     case GPIO_P2:  P2 ^= GPIO_Pin ; break;	//端口2			 
     case GPIO_P3:  P3 ^= GPIO_Pin ; break;	//端口3			 
     case GPIO_P4:  P4 ^= GPIO_Pin ; break;	//端口4			
     case GPIO_P5:  P5 ^= GPIO_Pin ; break;	//端口5			 
     case GPIO_P6:  P6 ^= GPIO_Pin ; break;	//端口6		
     case GPIO_P7:  P7 ^= GPIO_Pin ; break;	//端口7
		 default:
		 break;				 
	 }
 }
 