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
 * @file       CNU_PIE_EXTI.c
 * @brief      EXTI
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
#include "CNU_PIE_EXTI.h"
 
 uint8_t Port_Exti_Flag[8];
 
uint8_t GPIO_EXTI_Init(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin , EXTI_MODE_Enum EXTI_Mode)
 {
	 if(GPIO_Port > GPIO_P7)      return FAIL; //初始化错误值返回FAIL
	 if(GPIO_Pin  > GPIO_Pin_All) return FAIL; //初始化错误值返回FAIL
	 if(EXTI_Mode > HIGH_LEVEL)   return FAIL; //初始化错误值返回FAIL
	 
	 switch (GPIO_Port)
	 {
		 case GPIO_P0://端口0
			 switch (EXTI_Mode)
			 {
				 P0INTE |= GPIO_Pin;//使能P0段口对应引脚开启外部中断
				 case FALLING_EDGE:
					 P0IM1 &= ~GPIO_Pin,	P0IM0 &= ~GPIO_Pin;	 break; //下降沿中断		 
				 case RISING_EDGE:
					 P0IM1 &= ~GPIO_Pin,	P0IM0 |=  GPIO_Pin;   break;//上升沿中断
				 case LOW_LEVEL:
					 P0IM1 |=  GPIO_Pin,	P0IM0 &= ~GPIO_Pin;	 break; //低电平中断
				 case HIGH_LEVEL:
					 P0IM1 |=  GPIO_Pin,	P0IM0 |=  GPIO_Pin;   break;//高电平中断
				 default:
					 return FAIL; break;//初始化失败
			 }break;
     case GPIO_P1://端口1
			 switch (EXTI_Mode)
			 {
				 P1INTE |= GPIO_Pin;//使能P1段口对应引脚开启外部中断
				 case FALLING_EDGE:
					 P1IM1 &= ~GPIO_Pin,	P1IM0 &= ~GPIO_Pin;	 break; //下降沿中断		 
				 case RISING_EDGE:
					 P1IM1 &= ~GPIO_Pin,	P1IM0 |=  GPIO_Pin;   break;//上升沿中断
				 case LOW_LEVEL:
					 P1IM1 |=  GPIO_Pin,	P1IM0 &= ~GPIO_Pin;	 break; //低电平中断
				 case HIGH_LEVEL:
					 P1IM1 |=  GPIO_Pin,	P1IM0 |=  GPIO_Pin;   break;//高电平中断
				 default:
					 return FAIL; break;//初始化失败
			 }break;		
     case GPIO_P2://端口2
			 switch (EXTI_Mode)
			 {
				 P2INTE |= GPIO_Pin;//使能P2段口对应引脚开启外部中断
				 case FALLING_EDGE:
					 P2IM1 &= ~GPIO_Pin,	P2IM0 &= ~GPIO_Pin;	 break; //下降沿中断		 
				 case RISING_EDGE:
					 P2IM1 &= ~GPIO_Pin,	P2IM0 |=  GPIO_Pin;   break;//上升沿中断
				 case LOW_LEVEL:
					 P2IM1 |=  GPIO_Pin,	P2IM0 &= ~GPIO_Pin;	 break; //低电平中断
				 case HIGH_LEVEL:
					 P2IM1 |=  GPIO_Pin,	P2IM0 |=  GPIO_Pin;   break;//高电平中断
				 default:
					 return FAIL; break;//初始化失败
			 }break;		
     case GPIO_P3://端口3
			 switch (EXTI_Mode)
			 {
				 P3INTE |= GPIO_Pin;//使能P3段口对应引脚开启外部中断
				 case FALLING_EDGE:
					 P3IM1 &= ~GPIO_Pin,	P3IM0 &= ~GPIO_Pin;	 break; //下降沿中断		 
				 case RISING_EDGE:
					 P3IM1 &= ~GPIO_Pin,	P3IM0 |=  GPIO_Pin;   break;//上升沿中断
				 case LOW_LEVEL:
					 P3IM1 |=  GPIO_Pin,	P3IM0 &= ~GPIO_Pin;	 break; //低电平中断
				 case HIGH_LEVEL:
					 P3IM1 |=  GPIO_Pin,	P3IM0 |=  GPIO_Pin;   break;//高电平中断
				 default:
					 return FAIL; break;//初始化失败
			 }break;	
     case GPIO_P4://端口4
			 switch (EXTI_Mode)
			 {
				 P4INTE |= GPIO_Pin;//使能P4段口对应引脚开启外部中断
				 case FALLING_EDGE:
					 P4IM1 &= ~GPIO_Pin,	P4IM0 &= ~GPIO_Pin;	 break; //下降沿中断		 
				 case RISING_EDGE:
					 P4IM1 &= ~GPIO_Pin,	P4IM0 |=  GPIO_Pin;   break;//上升沿中断
				 case LOW_LEVEL:
					 P4IM1 |=  GPIO_Pin,	P4IM0 &= ~GPIO_Pin;	 break; //低电平中断
				 case HIGH_LEVEL:
					 P4IM1 |=  GPIO_Pin,	P4IM0 |=  GPIO_Pin;   break;//高电平中断
				 default:
					 return FAIL; break;//初始化失败
			 }break;	
     case GPIO_P5://端口5
			 switch (EXTI_Mode)
			 {
				 P5INTE |= GPIO_Pin;//使能P2段口对应引脚开启外部中断
				 case FALLING_EDGE:
					 P5IM1 &= ~GPIO_Pin,	P5IM0 &= ~GPIO_Pin;	 break; //下降沿中断		 
				 case RISING_EDGE:
					 P5IM1 &= ~GPIO_Pin,	P5IM0 |=  GPIO_Pin;   break;//上升沿中断
				 case LOW_LEVEL:
					 P5IM1 |=  GPIO_Pin,	P5IM0 &= ~GPIO_Pin;	 break; //低电平中断
				 case HIGH_LEVEL:
					 P5IM1 |=  GPIO_Pin,	P5IM0 |=  GPIO_Pin;   break;//高电平中断
				 default:
					 return FAIL; break;//初始化失败
			 }break;		
     case GPIO_P6://端口6
			 switch (EXTI_Mode)
			 {
				 P6INTE |= GPIO_Pin;//使能P6段口对应引脚开启外部中断
				 case FALLING_EDGE:
					 P6IM1 &= ~GPIO_Pin,	P6IM0 &= ~GPIO_Pin;	 break; //下降沿中断		 
				 case RISING_EDGE:
					 P6IM1 &= ~GPIO_Pin,	P6IM0 |=  GPIO_Pin;   break;//上升沿中断
				 case LOW_LEVEL:
					 P6IM1 |=  GPIO_Pin,	P6IM0 &= ~GPIO_Pin;	 break; //低电平中断
				 case HIGH_LEVEL:
					 P6IM1 |=  GPIO_Pin,	P6IM0 |=  GPIO_Pin;   break;//高电平中断
				 default:
					 return FAIL; break;//初始化失败
			 }break;
     case GPIO_P7://端口7
			 switch (EXTI_Mode)
			 {
				 P7INTE |= GPIO_Pin;//使能P2段口对应引脚开启外部中断
				 case FALLING_EDGE:
					 P7IM1 &= ~GPIO_Pin,	P7IM0 &= ~GPIO_Pin;	 break; //下降沿中断		 
				 case RISING_EDGE:
					 P7IM1 &= ~GPIO_Pin,	P7IM0 |=  GPIO_Pin;   break;//上升沿中断
				 case LOW_LEVEL:
					 P7IM1 |=  GPIO_Pin,	P7IM0 &= ~GPIO_Pin;	 break; //低电平中断
				 case HIGH_LEVEL:
					 P7IM1 |=  GPIO_Pin,	P7IM0 |=  GPIO_Pin;   break;//高电平中断
				 default:
					 return FAIL; break;//初始化失败
			 }break;
		 default:
			 return FAIL; break;				 
	 }
	return SUCCEED;	//成功
 }
 
 uint8_t GPIO_EXTI_Open(GPIO_Port_enum GPIO_Port , GPIO_Pin_enum GPIO_Pin)
 {
	 if(GPIO_Port > GPIO_P7)      return FAIL; //初始化错误值返回FAIL
	 if(GPIO_Pin  > GPIO_Pin_All) return FAIL; //初始化错误值返回FAIL
	 
	 switch (GPIO_Port)
	 {
		 case GPIO_P0://端口0
				  P0INTE |= GPIO_Pin;//使能P0段口对应引脚开启外部中断
		 break;
     case GPIO_P1://端口1
				  P1INTE |= GPIO_Pin;//使能P1段口对应引脚开启外部中断
		 break;		
     case GPIO_P2://端口2
			    P2INTE |= GPIO_Pin;//使能P2段口对应引脚开启外部中断
		 break;		
     case GPIO_P3://端口3
		      P3INTE |= GPIO_Pin;//使能P3段口对应引脚开启外部中断
		 break;	
     case GPIO_P4://端口4
			    P4INTE |= GPIO_Pin;//使能P4段口对应引脚开启外部中断
		 break;	
     case GPIO_P5://端口5
		      P5INTE |= GPIO_Pin;//使能P2段口对应引脚开启外部中断
		 break;		
     case GPIO_P6://端口6
			    P6INTE |= GPIO_Pin;//使能P6段口对应引脚开启外部中断
		 break;
     case GPIO_P7://端口7
			    P7INTE |= GPIO_Pin;//使能P2段口对应引脚开启外部中断
	   break;
		 default:
			 return FAIL; break;				 
	 }
	return SUCCEED;	//成功
 }
 
 uint8_t GPIO_EXTI_Set_Priority(GPIO_Port_enum GPIO_Port , EXTI_PRIORITY_Enum EXTI_Priority)
 {
	 if(GPIO_Port > GPIO_P7)              return FAIL; //初始化错误值返回FAIL
	 if(EXTI_Priority  > Lowest_priority) return FAIL; //初始化错误值返回FAIL
	 
	 switch (GPIO_Port)
	 {
		 case GPIO_P0://端口0
			 switch (EXTI_Priority)
			 {
				 case Highest_priority:
					 PIN_IP &= ~P0_PRI0RITY,	PIN_IPH &= ~P0_PRI0RITY;	 break;  
				 case Second_priority:
					 PIN_IP |=  P0_PRI0RITY,	PIN_IPH &= ~P0_PRI0RITY;   break;
				 case Third_priority:
					 PIN_IP &= ~P0_PRI0RITY,	PIN_IPH |=  P0_PRI0RITY;	 break;
				 case Lowest_priority:
					 PIN_IP |=  P0_PRI0RITY,	PIN_IPH |=  P0_PRI0RITY;   break;
				 default:
					 return FAIL; break;//初始化失败
			 }break;
     case GPIO_P1://端口1
			 switch (EXTI_Priority)
			 {
				 case Highest_priority:
					 PIN_IP &= ~P1_PRI0RITY,	PIN_IPH &= ~P1_PRI0RITY;	 break; 
				 case Second_priority:
					 PIN_IP |=  P1_PRI0RITY,	PIN_IPH |=  P1_PRI0RITY;   break;
				 case Third_priority:
					 PIN_IP |=  P1_PRI0RITY,	PIN_IPH &= ~P1_PRI0RITY;	 break;
				 case Lowest_priority:
					 PIN_IP |=  P1_PRI0RITY,	PIN_IPH |=  P1_PRI0RITY;   break;
				 default:
					 return FAIL; break;//初始化失败
			 }break;		
     case GPIO_P2://端口2
			 switch (EXTI_Priority)
			 {
				 case Highest_priority:
					 PIN_IP &= ~P2_PRI0RITY,	PIN_IPH &= ~P2_PRI0RITY;	 break; 
				 case Second_priority:
					 PIN_IP |=  P2_PRI0RITY,	PIN_IPH |=  P2_PRI0RITY;   break;
				 case Third_priority:
					 PIN_IP |=  P2_PRI0RITY,	PIN_IPH &= ~P2_PRI0RITY;	 break;
				 case Lowest_priority:
					 PIN_IP |=  P2_PRI0RITY,	PIN_IPH |=  P2_PRI0RITY;   break;
				 default:
					 return FAIL; break;//初始化失败
			 }break;		
     case GPIO_P3://端口3
			 switch (EXTI_Priority)
			 {
				 case Highest_priority:
					 PIN_IP &= ~P3_PRI0RITY,	PIN_IPH &= ~P3_PRI0RITY;	 break; 
				 case Second_priority:
					 PIN_IP |=  P3_PRI0RITY,	PIN_IPH |=  P3_PRI0RITY;   break;
				 case Third_priority:
					 PIN_IP |=  P3_PRI0RITY,	PIN_IPH &= ~P3_PRI0RITY;	 break;
				 case Lowest_priority:
					 PIN_IP |=  P3_PRI0RITY,	PIN_IPH |=  P3_PRI0RITY;   break;
				 default:
					 return FAIL; break;//初始化失败
			 }break;	
     case GPIO_P4://端口4
			 switch (EXTI_Priority)
			 {
				 case Highest_priority:
					 PIN_IP &= ~P4_PRI0RITY,	PIN_IPH &= ~P4_PRI0RITY;	 break; 
				 case Second_priority:
					 PIN_IP |=  P4_PRI0RITY,	PIN_IPH |=  P4_PRI0RITY;   break;
				 case Third_priority:
					 PIN_IP |=  P4_PRI0RITY,	PIN_IPH &= ~P4_PRI0RITY;	 break;
				 case Lowest_priority:
					 PIN_IP |=  P4_PRI0RITY,	PIN_IPH |=  P4_PRI0RITY;   break;
				 default:
					 return FAIL; break;//初始化失败
			 }break;	
     case GPIO_P5://端口5
			 switch (EXTI_Priority)
			 {
				 case Highest_priority:
					 PIN_IP &= ~P5_PRI0RITY,	PIN_IPH &= ~P5_PRI0RITY;	 break; 
				 case Second_priority:
					 PIN_IP |=  P5_PRI0RITY,	PIN_IPH |=  P5_PRI0RITY;   break;
				 case Third_priority:
					 PIN_IP |=  P5_PRI0RITY,	PIN_IPH &= ~P5_PRI0RITY;	 break;
				 case Lowest_priority:
					 PIN_IP |=  P5_PRI0RITY,	PIN_IPH |=  P5_PRI0RITY;   break;
				 default:
					 return FAIL; break;//初始化失败
			 }break;		
     case GPIO_P6://端口6
			 switch (EXTI_Priority)
			 {
				 case Highest_priority:
					 PIN_IP &= ~P6_PRI0RITY,	PIN_IPH &= ~P6_PRI0RITY;	 break; 
				 case Second_priority:
					 PIN_IP |=  P6_PRI0RITY,	PIN_IPH |=  P6_PRI0RITY;   break;
				 case Third_priority:
					 PIN_IP |=  P6_PRI0RITY,	PIN_IPH &= ~P6_PRI0RITY;	 break;
				 case Lowest_priority:
					 PIN_IP |=  P6_PRI0RITY,	PIN_IPH |=  P6_PRI0RITY;   break;
				 default:
					 return FAIL; break;//初始化失败
			 }break;
     case GPIO_P7://端口7
			 switch (EXTI_Priority)
			 {
				 case Highest_priority:
					 PIN_IP &= ~P7_PRI0RITY,	PIN_IPH &= ~P7_PRI0RITY;	 break; 
				 case Second_priority:
					 PIN_IP |=  P7_PRI0RITY,	PIN_IPH |=  P7_PRI0RITY;   break;
				 case Third_priority:
					 PIN_IP |=  P7_PRI0RITY,	PIN_IPH &= ~P7_PRI0RITY;	 break;
				 case Lowest_priority:
					 PIN_IP |=  P7_PRI0RITY,	PIN_IPH |=  P7_PRI0RITY;   break;
				 default:
					 return FAIL; break;//初始化失败
			 }break;
		 default:
			 return FAIL; break;				 
	 }
	return SUCCEED;	//成功
 }
 
uint8_t GPIO_EXTI_Flag_Read(GPIO_Port_enum GPIO_Port)
{
	 switch (GPIO_Port)
	 {
		 case GPIO_P0://端口0
			Port_Exti_Flag[0] = P0INTF  ;break; 
     case GPIO_P1://端口1         
			Port_Exti_Flag[1] = P1INTF  ;break; 
     case GPIO_P2://端口2         
		  Port_Exti_Flag[2] = P2INTF  ;break; 
     case GPIO_P3://端口3         
		  Port_Exti_Flag[3] = P3INTF  ;break;  
     case GPIO_P4://端口4         
			Port_Exti_Flag[4] = P4INTF  ;break; 
     case GPIO_P5://端口5         
	    Port_Exti_Flag[5] = P5INTF  ;break; 
     case GPIO_P6://端口6         
		  Port_Exti_Flag[6] = P6INTF  ;break; 
     case GPIO_P7://端口7         
			Port_Exti_Flag[7] = P7INTF  ;break; 
		 default:
			 return FAIL; break;				 
	 }
	return SUCCEED;	//成功
}
 
uint8_t GPIO_EXTI_Flag_Clear(GPIO_Port_enum GPIO_Port)
{
	 switch (GPIO_Port)
	 {
		 case GPIO_P0://端口0
			P0INTF = 0;break; 
     case GPIO_P1://端口1
			P1INTF = 0;break; 
     case GPIO_P2://端口2
		  P2INTF = 0;break; 
     case GPIO_P3://端口3
		  P3INTF = 0;break;  
     case GPIO_P4://端口4
			P4INTF = 0;break; 
     case GPIO_P5://端口5
	    P5INTF = 0;break; 
     case GPIO_P6://端口6
		  P6INTF = 0;break; 
     case GPIO_P7://端口7
			P7INTF = 0;break; 
		 default:
			 return FAIL; break;				 
	 }
	return SUCCEED;	//成功
}
