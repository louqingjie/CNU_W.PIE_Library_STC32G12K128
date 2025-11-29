/*---------------------------------------------------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/*---------------------------------------------------------------------*/

#ifndef	__STC32G_EEPROM_H
#define	__STC32G_EEPROM_H

#include	"main.h"

//========================================================================
//                              ��������
//========================================================================


//========================================================================
//                               IAP����
//========================================================================

#define		IAP_STANDBY()	IAP_CMD = 0		//IAP���������ֹ��
#define		IAP_READ()		IAP_CMD = 1		//IAP��������
#define		IAP_WRITE()		IAP_CMD = 2		//IAPд������
#define		IAP_ERASE()		IAP_CMD = 3		//IAP��������
    sbit    IAPEN       =           IAP_CONTR^7;
#define	IAP_ENABLE()		IAPEN = 1; IAP_TPS = FOSC / 1000000
#define	IAP_DISABLE()		IAP_CONTR = 0; IAP_CMD = 0; IAP_TRIG = 0; IAP_ADDRH = 0xff; IAP_ADDRL = 0xff


void	DisableEEPROM(void);
void 	EEPROM_read_n(uint32_t EE_address,uint8_t *DataAddress,uint16_t number);
void 	EEPROM_write_n(uint32_t EE_address,uint8_t *DataAddress,uint16_t number);
void	EEPROM_SectorErase(uint32_t EE_address);


#endif