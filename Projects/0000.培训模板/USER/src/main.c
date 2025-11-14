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
 * @file       main.h
 * @brief      主函数
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/

#include "main.h" 

void main(void)                                     //必要的主函数
{                                                   //主函数的大括号
	/*初始化*/
	Board_Init();                                     //培训底板初始化
	GPIO_Init(GPIO_P3, GPIO_Pin_4, GPIO_OUT_PP);			//将P34引脚初始化为输出
	
	
	
	while(1)
	{                                                 //while循环的大括号
		/*对外设的操作*/
		
		
	 	
	}                                                 //while循环的大括号
}                                                   //主函数的大括号

