#ifndef __LCD_H
#define __LCD_H
#include "stm8s.h"
#include "HardwareCFG.h"
//HT1621ָ��
#define  BIAS     0x52             //0b1000 0101 0010  1/3duty 4com 
#define  SYSDIS   0X00             //0b1000 0000 0000  ����ϵͳ������LCDƫѹ������ 
#define  SYSEN    0X02             //0b1000 0000 0010 ��ϵͳ���� 
#define  LCDOFF   0X04             //0b1000 0000 0100  ��LCDƫѹ 
#define  LCDON    0X06             //0b1000 0000 0110  ��LCDƫѹ 
#define  XTAL     0x28             //0b1000 0010 1000 �ⲿ��ʱ�� 
#define  RC256    0X30             //0b1000 0011 0000  �ڲ�ʱ�� 
#define  WDTDIS1  0X0A            //0b1000 0000 1010  ��ֹ���Ź� 

extern void Ht1621Wr_Data(unsigned char Data,unsigned char cnt) ;
extern void Ht1621WrCmd(unsigned char Cmd) ;
extern void Ht1621WrOneData(unsigned char Addr,unsigned char Data);
extern void Ht1621WrAllData(unsigned char Addr,unsigned char *p,unsigned char cnt);
extern void Ht1621_Init(void) ;
//extern void Display(void) ;
extern void Display_lcd_dot(uint8_t ubits);
extern void data_convertor(int16_t adc_value);
extern void Display(uint8_t BtLevel,uint8_t bleFlag);

void display_task();

extern uint16_t ShowTemp;
//extern uint8_t NegativeFlag;
#endif