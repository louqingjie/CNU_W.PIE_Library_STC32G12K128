#include <math.h>
#include <stdio.h>
#include <stdarg.h>

#include "Font.h" 
#include "OLED.h" 
#include "string.h"
#include "CNU_PIE_I2C.h"

void oled_init(void)
{
  oled_wrcmd(0xae);
  oled_wrcmd(0x00);
  oled_wrcmd(0x10);
  oled_wrcmd(0x40);
  oled_wrcmd(0x81);
  oled_wrcmd(0xcf);
  oled_wrcmd(0xa0);
  oled_wrcmd(0xc0);
  oled_wrcmd(0xa6);
  oled_wrcmd(0xa8);
  oled_wrcmd(0x3f);
  oled_wrcmd(0xd3);
  oled_wrcmd(0x00);
  oled_wrcmd(0xd5);
  oled_wrcmd(0x80);
  oled_wrcmd(0xd9);
  oled_wrcmd(0xf1);
  oled_wrcmd(0xda);
  oled_wrcmd(0x12);
  oled_wrcmd(0xdb);
  oled_wrcmd(0x40);
  oled_wrcmd(0x20);
  oled_wrcmd(0x02);
  oled_wrcmd(0x8d);
  oled_wrcmd(0x14);
  oled_wrcmd(0xa4);
  oled_wrcmd(0xa6);
  oled_wrcmd(0xaf);
  oled_cls();
  oled_setpos(0,0);
}

void oled_wrcmd(unsigned char WrCmd) 
{
	uint8_t cmd[2];
	cmd[0]=0x00;
	cmd[1]=WrCmd;
  I2C_WriteNbyte(OLED_ID,cmd[0],&cmd[1],1);
}
void oled_wrdata(unsigned char WrData) 
{
	uint8_t dat[2];
	dat[0]=0x40;
	dat[1]=WrData;
	I2C_WriteNbyte(OLED_ID,dat[0],&dat[1],1);
}
void oled_cls(void)
{
  unsigned char i,n;
  for(i=0;i<8;i++)
  {
    oled_wrcmd((uint8_t)(0xb0+i));
    oled_wrcmd(0x00);
    oled_wrcmd(0x10);
    for(n=0;n<128;n++)
      oled_wrdata(0x00);
  }
}
void oled_cls_line(unsigned char i)
{
	int n = 0;
    oled_wrcmd((uint8_t)(0xb0+i));
    oled_wrcmd(0x01);
    oled_wrcmd(0x10);
    for(n=0;n<128;n++)
     oled_wrdata(0x00);
}

void oled_setpos(unsigned char x,unsigned char y)
{
    oled_wrcmd((uint8_t)(0xb0+y));
    oled_wrcmd(((x&0xf0)>>4)|0x10);
    oled_wrcmd((x&0x0f));
}
void oled_on(void)
{
    oled_wrcmd(0x8d);
    oled_wrcmd(0x14);
    oled_wrcmd(0xaf);
}
void oled_off(void)
{
    oled_wrcmd(0x8d);
    oled_wrcmd(0x10);
    oled_wrcmd(0xae);
}
void oled_p8x16_str(unsigned char x,unsigned char y,char ch[]) 
{
  unsigned char c=0,i=0,j=0;

  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    oled_setpos(x,y);
    for(i=0;i<8;i++)
      oled_wrdata(F8X16[c*16+i]);
    oled_setpos(x,(uint8_t)(y+1));
    for(i=0;i<8;i++)
      oled_wrdata(F8X16[c*16+i+8]);
    x+=8;
    j++;
  }
}
void oled_p6x8_str(unsigned char x,unsigned char y,char ch[]) 
{
  unsigned char c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    oled_setpos(x,y);
    for(i=0;i<6;i++)
    oled_wrdata(F6x8[c][i]);
    x+=6;
    j++;
  }
}
void oled_show_float(unsigned char x,unsigned char y,float num,unsigned int N) 
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
  oled_p6x8_str(x,y,tmp);
}

//////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////

#define MCP_IODIR   0x00 
#define MCP_IPOL    0x02 
#define MCP_GPINTEN 0x04 
#define MCP_DEFVAL  0x06 
                         
                         
#define MCP_INTCON  0x08 
#define MCP_GPPU    0x0c 
#define MCP_INTF    0x0e 
#define MCP_INTCAP  0x10 
#define MCP_GPIO    0x12 
#define MCP_OLAT    0x14 

#define MCP_IOCON   0X0a 


#define CON_BANK    0       
#define CON_MIRROR  0       
#define CON_SEQOP   1       
#define CON_DISSLW  0       
#define CON_HAEN    1       
#define CON_ODR     0       
#define CON_INTPOL  0       


void writ_mcp_iic(unsigned char reg,unsigned char pbuf)
{
  uint8_t cmd[2];
	cmd[0]=reg;
	cmd[1]=pbuf;
  I2C_WriteNbyte(MCP_ID,cmd[0],&cmd[1],1);
}
int read_mcp_iic(uint8_t reg,uint8_t *pbuf)
{
	I2C_ReadNbyte(MCP_ID,reg,pbuf,1);
	return *pbuf;
}

void init_mcp23017(void)
{
        writ_mcp_iic(MCP_IOCON,(CON_BANK  <<7)|       
                               (CON_MIRROR<<6)|       
                               (CON_SEQOP <<5)|       
                               (CON_DISSLW<<4)|       
                               (CON_HAEN  <<3)|       
                               (CON_ODR   <<2)|       
                               (CON_INTPOL<<1) );     
}

//0 out,1 in
void gpio_iic_init(PORTn_23n17 port,unsigned char set)
{
  if(port==portA)
  {
    writ_mcp_iic(MCP_IODIR,set); 
    writ_mcp_iic(MCP_GPIO,set^0xff);
  }
  else if(port==portB)
  {
    writ_mcp_iic(MCP_IODIR+0x01,set); 
    writ_mcp_iic(MCP_GPIO+0x01,set^0xff);
  }
}

int gpio_iic_set(PORTn_23n17 port,unsigned char set)
{
  unsigned char pbuf;
  if(port==portA)
  {
    read_mcp_iic(MCP_IODIR,&pbuf);
    writ_mcp_iic(MCP_OLAT,(pbuf^0xff)&set);
    read_mcp_iic(MCP_OLAT,&pbuf);
  }
  else
  {
    read_mcp_iic(MCP_IODIR+0x01,&pbuf);
    writ_mcp_iic(MCP_OLAT+0x01,(pbuf^0xff)&set);
    read_mcp_iic(MCP_OLAT+0x01,&pbuf);
  } 
  if(pbuf==set)return 1;
  else return 0;
}


unsigned char gpio_iic_read(PORTn_23n17 port)
{
  unsigned char pbuf;
  if(port==portA)
  read_mcp_iic(MCP_GPIO,&pbuf);
  else
   read_mcp_iic(MCP_GPIO+0x01,&pbuf);
  return pbuf;
}
int Scan_Keyboard(void)              
{
    if(gpio_iic_read(portA)==239)     //key7 A4
        return 9;
    if(gpio_iic_read(portA)==223)     //key4 A5
        return 4;
    if(gpio_iic_read(portA)==191)     //key4 A6
        return 10;
		if(gpio_iic_read(portA)==127)     //key4 A6
        return 12;
    if(gpio_iic_read(portB)==251)     //key1 B2
        return 1;
    if(gpio_iic_read(portB)==223)     //key2 B5
        return 2;
    if(gpio_iic_read(portB)==254)     //key3 B0
        return 3;
    if(gpio_iic_read(portB)==253)     //key5 B1
        return 5;
    if(gpio_iic_read(portB)==127)     //key6 B7
        return 6;
    if(gpio_iic_read(portB)==247)     //key7 B3
        return 7;
    if(gpio_iic_read(portB)==239)     //key7 B4
        return 8;
    if(gpio_iic_read(portB)==191)     //key4 A6
        return 11;

    return 0;
}

