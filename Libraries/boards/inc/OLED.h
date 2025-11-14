#ifndef OLED_H
#define OLED_H

#define OLED_ID 0x7a
#define MCP_ID 0x42

/*OLED*/
void oled_init(void);
void oled_wrcmd(unsigned char WrCmd) ;
void oled_wrdata(unsigned char WrData) ;
void oled_cls(void);
void oled_cls_line(unsigned char i);
void oled_setpos(unsigned char x,unsigned char y);
void oled_on(void);
void oled_off(void);
void oled_p8x16_str(unsigned char x,unsigned char y,char ch[]) ;
void oled_p6x8_str(unsigned char x,unsigned char y,char ch[]) ;
void oled_show_float(unsigned char x,unsigned char y,float num,unsigned int N) ;
#define showb(x,y,ch)	 oled_p8x16_str(x,y,ch)
#define showl(x,y,ch)	 oled_p6x8_str(x,y,ch)
#define showf(x,y,num) oled_float(x,y,num)

/*MCP23017*/
typedef enum
{
  portA,
  portB,
}PORTn_23n17;

void init_mcp23017(void);
unsigned char gpio_iic_read(PORTn_23n17 port);
void gpio_iic_init(PORTn_23n17 port,unsigned char set);
int  gpio_iic_set(PORTn_23n17 port,unsigned char set);
int Scan_Keyboard(void);


#endif
