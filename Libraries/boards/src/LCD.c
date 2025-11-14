#include <math.h>
#include "intrins.h"
#include "LCD.h" 
#include "Font.h" 
#include "CNU_PIE_SPI.h"
const unsigned char gImage_Wpie[1024] = { /* 0X22,0X01,0X80,0X00,0X40,0X00, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0XE0,0XE0,0XE0,0XF0,0XE0,0XC0,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XE0,0XF0,0XE0,0XC0,0X80,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XE0,0XE0,0XE0,0XC0,0XC0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X1F,0XFF,0XFF,0XFF,0XFF,0XFE,
0XF0,0X00,0X00,0X00,0X00,0X00,0X80,0XFC,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XC0,
0X00,0X00,0X00,0X00,0X80,0XE0,0XFC,0XFF,0XFF,0XFF,0X3F,0X0F,0X03,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XC0,0XC0,0XE0,0XE0,0XE0,0XE0,
0XE0,0XE0,0XC0,0XC0,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X1F,0X7F,0XFF,0XFF,
0XFF,0XFE,0XF8,0XC0,0XC0,0XFC,0XFF,0XFF,0XFF,0XFF,0X0F,0X1F,0XFF,0XFF,0XFF,0XFF,
0XFE,0XF0,0X80,0XFC,0XFF,0XFF,0XFF,0XFF,0X7F,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0XFC,0XFC,0XFC,0X78,0X78,0XF0,
0XF0,0XE0,0XC0,0XC0,0XC0,0X80,0X00,0X00,0X00,0X00,0X20,0X30,0X38,0X7C,0X3C,0X3C,
0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XC0,0XF8,0XFC,0XFC,0X3C,0X3C,0X1C,
0X0C,0X00,0X00,0X00,0X00,0X00,0X80,0XF0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X7F,0X3F,0X1E,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X1F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0X07,0X00,0X00,0X00,0X00,0X3F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0X1F,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XC0,0XE0,0XE0,
0XF1,0XF9,0X3F,0X1F,0X1F,0X0F,0X07,0X03,0X00,0X00,0X00,0X0C,0XFC,0XFC,0XF8,0XF0,
0X00,0X00,0X00,0X00,0X70,0XFC,0XFC,0XFF,0XFF,0XDF,0X33,0XF9,0XF8,0XF8,0XF0,0XE0,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,0X03,0X03,0X03,
0X01,0X01,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X01,0X7F,0XFF,0XFF,0XFF,0X7F,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X1F,
0XFF,0XFF,0XFF,0X1F,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0XE0,
0XE0,0XF0,0XE0,0XE0,0X40,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XEF,0X07,0X03,0X01,
0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,
0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X03,0X07,0X0F,0X3F,0X7E,0XFD,0XF9,0XF0,0XE0,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X01,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X01,0X01,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,0X01,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,0X01,0X01,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,0X01,
0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};
 /**************************************************************************************************************************
 * @brief  OLED写数据
 * @brief  内部调用，无需关心
***************************************************************************************************************************/
void LCD_WrDat(unsigned char data_t)
{
  unsigned char i=8;
  OLED_CS_Clear();
  OLED_DC_Set();
  OLED_CLK_Clear();
  while(i--)
  {
    if(data_t&0x80)OLED_D1_Set();
    else OLED_D1_Clear();
    OLED_CLK_Set();
_nop_();
    OLED_CLK_Clear();
    data_t<<=1;
  }
  OLED_CS_Set();
}
 /**************************************************************************************************************************
 * @brief  OLED写命令
 * @brief  内部调用，无需关心
***************************************************************************************************************************/
void LCD_WrCmd(unsigned char cmd)
{
  unsigned char i=8;
  OLED_CS_Clear();
  OLED_DC_Clear();
  OLED_CLK_Clear();
  while(i--)
  {
    if(cmd&0x80)OLED_D1_Set();
    else OLED_D1_Clear();
    OLED_CLK_Set();
_nop_();
    OLED_CLK_Clear();
    cmd<<=1;
  }
  OLED_CS_Set();
}
 /**************************************************************************************************************************
 * @brief  OLED图像填充
 * @brief  内部调用，无需关心
***************************************************************************************************************************/
void LCD_Fill(unsigned char bmp_data)
{
  unsigned char y,x;
  
  for(y=0;y<8;y++)
  {
    LCD_WrCmd((uint8_t)(0xb0+y));
    LCD_WrCmd(0x01);
    LCD_WrCmd(0x10);
    for(x=0;x<128;x++)
      LCD_WrDat(bmp_data);
  }
}
 /**************************************************************************************************************************
 * @brief  OLED画点
 * @brief  内部调用，无需关心
***************************************************************************************************************************/
void LCD_Set_Pos(uint8_t x, uint8_t y) 
{
  LCD_WrCmd((uint8_t)(0xb0+y));
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  LCD_WrCmd((x&0x0f)|0x01);
}
 /**************************************************************************************************************************
 * @brief  OLED清屏
 * @exampleCode
 *      LCD_CLS; //屏幕清屏
 * @endcode
***************************************************************************************************************************/
void LCD_CLS(void)
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    LCD_WrCmd((uint8_t)(0xb0+y));
    LCD_WrCmd(0x01);
    LCD_WrCmd(0x10);
    for(x=0;x<128;x++)
      LCD_WrDat(0);
  }
}
 /**************************************************************************************************************************
 * @brief  OLED画logo
 * @brief  内部调用，无需关心
***************************************************************************************************************************/
void Draw_WPIELogo(void)
{
	uint8_t x,y;
  unsigned int ii=0;
  for(y=0;y<8;y++)
  {
    for(x=0;x<128;x++)
    {
      LCD_Set_Pos(x,y);
      LCD_WrDat(gImage_Wpie[ii++]);
    }
  }
}
 /**************************************************************************************************************************
 * @brief  OLED初始化
 * @exampleCode
 *      LCD_Init(); //OLED屏幕初始化
 * @endcode
***************************************************************************************************************************/
void LCD_Init(void)
{
  GPIO_Init(OLED_CS_PORT ,OLED_CS_PIN ,GPIO_OUT_PP);
	GPIO_Init(OLED_RST_PORT,OLED_RST_PIN,GPIO_OUT_PP);
	GPIO_Init(OLED_DC_PORT ,OLED_DC_PIN ,GPIO_OUT_PP);
	GPIO_Init(OLED_D1_PORT ,OLED_D1_PIN ,GPIO_OUT_PP);
	GPIO_Init(OLED_CLK_PORT,OLED_CLK_PIN,GPIO_OUT_PP);
	Ms_Delay(200);
   
  
  OLED_CLK_Set();
  //OLED_CS_Set();	//预制SLK和SS为高电平
  
  OLED_RST_Clear();
  Ms_Delay(50);
  OLED_RST_Set();
  
  LCD_WrCmd(0xae);//--turn off oled panel
  LCD_WrCmd(0x00);//---set low column address
  LCD_WrCmd(0x10);//---set high column address
  LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  LCD_WrCmd(0x81);//--set contrast control register
  LCD_WrCmd(0xc8); // Set SEG Output Current Brightness
  LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  LCD_WrCmd(0xa6);//--set normal display
  // LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  // LCD_WrCmd(0x3f);//--1/64 duty
  LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  LCD_WrCmd(0x00);//-not offset
  LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  LCD_WrCmd(0xd9);//--set pre-charge period
  LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  LCD_WrCmd(0xda);//--set com pins hardware configuration
  LCD_WrCmd(0x12);
  LCD_WrCmd(0xdb);//--set vcomh
  LCD_WrCmd(0x40);//Set VCOM Deselect Level
  LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  LCD_WrCmd(0x00);//
  LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
  LCD_WrCmd(0x14);//--set(0x10) disable
  LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
  LCD_WrCmd(0xaf);//--turn on oled panel
  LCD_Fill(0x00);  //初始清屏  
	LCD_Set_Pos(0,0);
  Draw_WPIELogo();
  Ms_Delay(1000);
  LCD_CLS();
  LCD_Set_Pos(0,0);
}
 /**************************************************************************************************************************
 * @brief  OLED画字符串
 * @exampleCode
 *      LCD_P6x8Str(0,0,"w.pie") //在起始坐标x为0，y为0绘制一个w.pie的字符串
 * @endcode
 * @param[in]  x    x起始坐标
 * @param[in]  y    y起始坐标(行数)             
 * @param[in]  ch[] 字符串
***************************************************************************************************************************/
void LCD_P6x8Str(unsigned char x,unsigned char y,char ch[])
{
  unsigned char c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    LCD_Set_Pos(x,y);
    for(i=0;i<6;i++)
    LCD_WrDat(F6x8[c][i]);
    x+=6;
    j++;
  }
}
 /**************************************************************************************************************************
 * @brief  OLED画字符串
 * @exampleCode
 *      LCD_P8x16Str(0,0,"w.pie") //在起始坐标x为0，y为0绘制一个w.pie的字符串
 * @endcode
 * @param[in]  x    x起始坐标
 * @param[in]  y    y起始坐标(行数)             
 * @param[in]  ch[] 字符串
***************************************************************************************************************************/
void LCD_P8x16Str(unsigned char x,unsigned char y,char ch[])
{
  unsigned char c=0,i=0,j=0;
  
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    LCD_Set_Pos(x,y);
    for(i=0;i<8;i++)
      LCD_WrDat(F8X16[c*16+i]);
    LCD_Set_Pos(x,(uint8_t)(y+1));
    for(i=0;i<8;i++)
      LCD_WrDat(F8X16[c*16+i+8]);
    x+=8;
    j++;
  }
}
 /**************************************************************************************************************************
 * @brief  OLED画无符号整形
 * @exampleCode
        unsigned int a;
 *      LCD_PrintU16(0,0,a) //在起始坐标x为0，y为0的地方显示变量a的值
 * @endcode
 * @param[in]  x    x起始坐标
 * @param[in]  y    y起始坐标(行数)             
 * @param[in]  num  要显示的变量
***************************************************************************************************************************/
void LCD_PrintU16(unsigned char x,unsigned char y,unsigned int num)
{
	int j=0;
  char tmp[6],i;
  tmp[5]=0;
  tmp[4]=(unsigned char)(num%10+0x30);
  tmp[3]=(unsigned char)(num/10%10+0x30);
  tmp[2]=(unsigned char)(num/100%10+0x30);
  tmp[1]=(unsigned char)(num/1000%10+0x30);
  tmp[0]=(unsigned char)(num/10000%10+0x30);
  
  for(i=0;i<4;i++)
  {
    if(tmp[0]=='0')//移位
    {
      for(j=0;j<5-i;j++)
        tmp[j]=tmp[j+1];
    } 
    else
      break;
  }
  
  LCD_P6x8Str(x,y,tmp);
  
}
 /**************************************************************************************************************************
 * @brief  OLED画浮点型
 * @exampleCode
        float a;
 *      LCD_PrintFloat(0 , 0 , a , 7) //在起始坐标x为0，y为0的地方显示变量a的值,一共显示7位
 * @endcode
 * @param[in]  x    x起始坐标
 * @param[in]  y    y起始坐标(行数)             
 * @param[in]  num  要显示的变量
***************************************************************************************************************************/
void LCD_PrintFloat(unsigned char x,unsigned char y,float num,unsigned int N)
{
  #define MAX_STR 12 
  char tmp[MAX_STR]={0};int n=3;
  float NUM=num;
  float _num=fabs(NUM);
	int i;
	uint32_t num0;
 if(NUM<0)
 {
   NUM=_num;
   tmp[0]='-';
 }
 else tmp[0]='+';
 for(i=0;i<MAX_STR+1;i++)
 {
   _num/=10;
   if(_num<1)break;
   else  n++;
 }
 if(N>7)N=7;
 if(((int)(N)+2-n)>0)for(i=0;i<(N+2-n);i++)NUM*=10;
 num0=(uint32_t)NUM;
 for(i=N>=n?(N+2-1):n-1;i>0;i--)
  {
    if(i!=n-1)
    {
      tmp[i]=(char)(num0%10+0x30);
      num0/=10;
    }
    else
      tmp[i]='.';
  }
  tmp[N+2]=0;
  LCD_P6x8Str(x,y,tmp);
}