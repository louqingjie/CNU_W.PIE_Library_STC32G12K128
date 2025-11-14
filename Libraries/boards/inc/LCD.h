#ifndef __LCD_H_
#define __LCD_H_

#include "CNU_PIE_GPIO.h"

//--------------OLED��������---------------------
#define PAGE_SIZE    8
#define XLevelL		0x00
#define XLevelH		0x10
#define YLevel       0xB0
#define	Brightness	 0xFF 
#define WIDTH 	     128
#define HEIGHT 	     64	


#define OLED_CMD     0	
#define OLED_DATA    1	

#define SET   1
#define RESET 0

#define OLED_CS_PORT  GPIO_P2
#define OLED_DC_PORT  GPIO_P2
#define OLED_RST_PORT GPIO_P2
#define OLED_CLK_PORT GPIO_P2
#define OLED_D1_PORT  GPIO_P2

#define OLED_CS_PIN  GPIO_Pin_2
#define OLED_DC_PIN  GPIO_Pin_6
#define OLED_RST_PIN GPIO_Pin_4
#define OLED_CLK_PIN GPIO_Pin_5
#define OLED_D1_PIN  GPIO_Pin_3

//-----------------OLED���Ų���---------------- 
#define OLED_CS_Clear()  GPIO_Write_Bit(OLED_CS_PORT , OLED_CS_PIN , RESET)
#define OLED_CS_Set()  GPIO_Write_Bit(OLED_CS_PORT , OLED_CS_PIN , SET)

#define OLED_DC_Clear()  GPIO_Write_Bit(OLED_DC_PORT , OLED_DC_PIN , RESET)
#define OLED_DC_Set()  GPIO_Write_Bit(OLED_DC_PORT , OLED_DC_PIN , SET)
 					   
#define OLED_RST_Clear()  GPIO_Write_Bit(OLED_RST_PORT , OLED_RST_PIN , RESET)
#define OLED_RST_Set()  GPIO_Write_Bit(OLED_RST_PORT , OLED_RST_PIN , SET)

#define OLED_CLK_Clear()  GPIO_Write_Bit(OLED_CLK_PORT , OLED_CLK_PIN , RESET)
#define OLED_CLK_Set()  GPIO_Write_Bit(OLED_CLK_PORT , OLED_CLK_PIN , SET)

#define OLED_D1_Clear()  GPIO_Write_Bit(OLED_D1_PORT , OLED_D1_PIN , RESET)
#define OLED_D1_Set()  GPIO_Write_Bit(OLED_D1_PORT , OLED_D1_PIN , SET)

void LCD_Init(void);
void LCD_CLS(void);
void LCD_P6x8Str(unsigned char x,unsigned char y,char ch[]);
void LCD_P8x16Str(unsigned char x,unsigned char y,char ch[]);
void LCD_PrintU16(unsigned char x,unsigned char y,unsigned int num);
void LCD_PrintFloat(unsigned char x,unsigned char y,float num,unsigned int N);
#endif