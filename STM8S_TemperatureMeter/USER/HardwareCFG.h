#ifndef _HARDWARECFG_H_
#define _HARDWARECFG_H_

#include "stm8s.h"
/*****************************************************
KEY: GPIOC
*****************************************************/
#define KEY_PORT       		GPIOA
#define KEY             	GPIO_PIN_3
#define KEY_GET()       	GPIO_ReadInputPin(KEY_PORT,KEY)

/*****************************************************
Power: PD6
*****************************************************/
#define POWER_PORT       	GPIOD
#define POWER             	GPIO_PIN_6
#define POWER_ON()       	GPIO_WriteHigh(POWER_PORT,POWER)
#define POWER_OFF()       	GPIO_WriteLow(POWER_PORT,POWER)
#define POWER_Toggle()      GPIO_WriteReverse(POWER_PORT,POWER)
/*****************************************************
LCD: 
LCD_Light		:PA2  	backlight
CSN       		:PB5	
RDN                     :PB4
WRN                     :PC3
DATA                    :PC4
*****************************************************/

#define LCD_BL_PORT    		GPIOA
#define LCD_CSN_PORT    	GPIOB
#define LCD_RDN_PORT    	GPIOB
#define LCD_WRN_PORT    	GPIOC
#define LCD_DATA_PORT    	GPIOC

#define LCD_BL             	GPIO_PIN_2
#define LCD_CSN             GPIO_PIN_5
#define LCD_RDN             GPIO_PIN_4
#define LCD_WRN             GPIO_PIN_3
#define LCD_DATA            GPIO_PIN_4

#define	LCD_BL_ON()		GPIO_WriteHigh(LCD_BL_PORT,LCD_BL)
#define	LCD_BL_OFF()		GPIO_WriteLow(LCD_BL_PORT,LCD_BL)

#define	LCD_CSN_ON()		GPIO_Init(LCD_CSN_PORT,LCD_CSN,GPIO_MODE_OUT_PP_HIGH_FAST);//GPIO_WriteHigh(LCD_CSN_PORT,LCD_CSN)
#define	LCD_CSN_OFF()		GPIO_Init(LCD_CSN_PORT,LCD_CSN,GPIO_MODE_OUT_PP_LOW_FAST);//GPIO_WriteLow(LCD_CSN_PORT,LCD_CSN)

#define	LCD_WRN_ON()		GPIO_Init(LCD_WRN_PORT,LCD_WRN,GPIO_MODE_OUT_PP_HIGH_FAST);//GPIO_WriteHigh(LCD_WRN_PORT,LCD_WRN)
#define	LCD_WRN_OFF()		GPIO_Init(LCD_WRN_PORT,LCD_WRN,GPIO_MODE_OUT_PP_LOW_FAST);//GPIO_WriteLow(LCD_WRN_PORT,LCD_WRN)

#define DATA_GET()       	GPIO_ReadInputPin(LCD_DATA_PORT,LCD_DATA)
#define	LCD_DATA_ON()		GPIO_Init(LCD_DATA_PORT,LCD_DATA,GPIO_MODE_OUT_PP_HIGH_FAST);//GPIO_WriteHigh(LCD_DATA_PORT,LCD_DATA)
#define	LCD_DATA_OFF()		GPIO_Init(LCD_DATA_PORT,LCD_DATA,GPIO_MODE_OUT_PP_LOW_FAST);//GPIO_WriteLow(LCD_DATA_PORT,LCD_DATA)
/*****************************************************
BLE Hardware SPI:
BLE_CSN       	: PD4
BLE_SCK      	: PC5
BLE_MOSI_MISO   : PC6
BLE_IRQ   	: PA1
Update:20230428 peter
*****************************************************/
#define BLE_CSN_PORT    	GPIOD
#define BLE_SCK_PORT    	GPIOC
#define BLE_MOSI_MISO_PORT      GPIOC
#define BLE_IRQ_PORT    	GPIOA

#define BLE_CSN			GPIO_PIN_4
#define BLE_SCK      		GPIO_PIN_5
#define BLE_MOSI_MISO   	GPIO_PIN_6
#define BLE_IRQ   		GPIO_PIN_1

#define BLE_CSN_CLR()     	GPIO_WriteLow(BLE_CSN_PORT,BLE_CSN)
#define BLE_CSN_SET()    	GPIO_WriteHigh(BLE_CSN_PORT,BLE_CSN)

#define BLE_SCK_CLR()     	GPIO_WriteLow(BLE_SCK_PORT,BLE_SCK)
#define BLE_SCK_SET()    	GPIO_WriteHigh(BLE_SCK_PORT,BLE_SCK)

#define BLE_MOSI_MISO_CLR()	GPIO_WriteLow(BLE_MOSI_MISO_PORT,BLE_MOSI_MISO)
#define BLE_MOSI_MISO_SET()	GPIO_WriteHigh(BLE_MOSI_MISO_PORT,BLE_MOSI_MISO)

#define BLE_MOSI_MISO_GET()	GPIO_ReadInputPin(BLE_MOSI_MISO_PORT,BLE_MOSI_MISO)

#define BLE_IRQ_GET()  		GPIO_ReadInputPin(BLE_IRQ_PORT,BLE_IRQ)


/*****************************************************
USART: 
RX		:PC2  	
TX		:PC3	
*****************************************************/

#define RX_PORT    			GPIOC
#define TX_PORT    			GPIOC

#define RX             		GPIO_PIN_2
#define TX             		GPIO_PIN_3
/*****************************************************
Battery_voltage_level: 
BATAD		:PD3  	battery voltage level
*****************************************************/
#define BAT_AD_PORT    	GPIOD

#define BAT_AD   	GPIO_PIN_3

/*****************************************************
NTC_voltage_Measure: 
NTCAD		:PD2  	NTC positive
NTCCOM		:PC7  	NTC negative
*****************************************************/
#define NTC_AD_PORT    	GPIOD
#define NTC_COM_PORT    GPIOC

#define NTC_AD   	GPIO_PIN_2
#define NTC_COM   	GPIO_PIN_7

#define NTC_COM_LOW()     	GPIO_WriteLow(NTC_COM_PORT,NTC_COM)
#define NTC_COM_HIGH()    	GPIO_WriteHigh(NTC_COM_PORT,NTC_COM)

extern unsigned char Ht1621Tab[];

#endif