#include "lcd.h"
#include "measurement.h"
#include "stdlib.h"
//unsigned int tmp;
int n1, n2, n3, n4;
unsigned char DispTab[]={0xAF,0x06,0xCB,0x4F,0x66,0x6D,0xED,0x07,0xEF,0x6F};
//                             1    2    3    4    5    6    7    8    9    10
uint16_t ShowTemp = 0;
uint8_t NegativeFlag = 0;
//������������
void Display_VotlvlAndUnit(uint8_t BtLevel);
void Display_Deci();
void Display_Unit(uint8_t bleFlag);
void Display_Ten();
void Display_Hundred(uint8_t BtLevel);
/*******************************************************************************
****��������:
****��������:1621д���ݺ���
****�汾:V1.0
****����:14-2-2014
****��ڲ���:Data-Ҫ���͵�����  cnt-Ҫ���͵�����λ��
****���ڲ���:
****˵��:
********************************************************************************/
void Ht1621Wr_Data(unsigned char Data,unsigned char cnt) 
{ 
  unsigned char i; 
  for (i=0;i<cnt;i++) 
   {   
     LCD_WRN_OFF(); 
     if((Data & 0x80)==0x80) 
        {LCD_DATA_ON();}
     else 
        {LCD_DATA_OFF();}
     LCD_WRN_ON(); 
     Data<<=1; 
   } 
} 
/*******************************************************************************
****��������:
****��������:1621дָ���
****�汾:V1.0
****����:14-2-2014
****��ڲ���:Cmd ����
****���ڲ���:
****˵��:
********************************************************************************/
void Ht1621WrCmd(unsigned char Cmd) 
{ 
   LCD_CSN_OFF(); 
   Ht1621Wr_Data(0x80,4);          //д�������־100 
   Ht1621Wr_Data(Cmd,8);           //д���������� 
   LCD_CSN_ON(); 
} 
/*******************************************************************************
****��������:
****��������:1621дһ�����ݺ���
****�汾:V1.0
****����:14-2-2014
****��ڲ���:Addr--��ַ Data--����
****���ڲ���:
****˵��:
********************************************************************************/
void Ht1621WrOneData(unsigned char Addr,unsigned char Data)
{
  LCD_CSN_OFF();
  Ht1621Wr_Data(0xa0,3);  //д�����ݱ�־101
  Ht1621Wr_Data(Addr<<2,6); //д���ַ����
  Ht1621Wr_Data(Data,4); //д�����ݵ�ǰ��λ 7  6  5  4
  LCD_CSN_ON();
}
/*******************************************************************************
****��������:
****��������:1621д�������ݺ���
****�汾:V1.0
****����:14-2-2014
****��ڲ���:Addr--��ַ p--����  cnt���ݸ���
****���ڲ���:
****˵��:
********************************************************************************/
void Ht1621WrAllData(unsigned char Addr,unsigned char *p,unsigned char cnt)
{
  /*
  unsigned char i;
  LCD_CSN_OFF();
  Ht1621Wr_Data(0xa0,3); //д�����ݱ�־101
  Ht1621Wr_Data(Addr<<2,6); //д���ַ����
  for (i=0;i<cnt;i++)
   {
    Ht1621Wr_Data(*p,8); //д������
    p++;
   }
  LCD_CSN_ON();
*/
   Ht1621WrOneData(0, 0x00);
   Ht1621WrOneData(1, 0x00<<4);
   Ht1621WrOneData(2, 0x00);
   Ht1621WrOneData(3, 0x00<<4);
   Ht1621WrOneData(4, 0x00);
   Ht1621WrOneData(5, 0x00<<4);
   Ht1621WrOneData(6, 0x00);
   Ht1621WrOneData(7, 0x00<<4);
   Ht1621WrOneData(8, 0x00);
}
/*******************************************************************************
****��������:
****��������:1621��ʼ������
****�汾:V1.0
****����:14-2-2014
****��ڲ���:
****���ڲ���:
****˵��:
********************************************************************************/
void Ht1621_Init(void) 
{ 
   // Ht1621_GND_0();       //HT1621 power on
   // Ht1621_VCC_1();
   Ht1621WrCmd(BIAS); 
   Ht1621WrCmd(RC256);             //ʹ���ڲ����� 
   //Ht1621WrCmd(XTAL);             //ʹ���ⲿ���� 
   Ht1621WrCmd(SYSDIS); 
   Ht1621WrCmd(WDTDIS1); 
   Ht1621WrCmd(SYSEN); 
   Ht1621WrCmd(LCDON); 
} 
/*******************************************************************************
****��������:
****��������:1621��ʾ����
****�汾:V1.0
****����:14-2-2014
****��ڲ���:
****���ڲ���:
****˵��:
********************************************************************************/
void Display(uint8_t BtLevel,uint8_t bleFlag) 
{ 
  Display_VotlvlAndUnit(BtLevel);
  Display_Deci();
  Display_Unit(bleFlag);
  Display_Ten();
  Display_Hundred(BtLevel);
}
void Display_Hundred(uint8_t BtLevel)
{
  if(n4 != 0)
  {
    if((BtLevel > 0)&&(BtLevel < 5))
    {
      Ht1621WrOneData(0, DispTab[n4]|0x10);                           //add P1
      Ht1621WrOneData(1, DispTab[n4]<<4);
    }
    else
    {
      Ht1621WrOneData(0, DispTab[n4]);
      Ht1621WrOneData(1, DispTab[n4]<<4);
    }
  }
  else
  {
    if(NegativeFlag) //�Ǹ���
    {
      if(n3!=0)                 //��һ��ʮλ����
      {
        if((BtLevel > 0)&&(BtLevel < 5))
        {
          Ht1621WrOneData(0, 0x40|0x10);
        }
        else
        {
          Ht1621WrOneData(0, 0x40);
        }
      }
      else
      {
         if((BtLevel > 0)&&(BtLevel < 5))
        {
          Ht1621WrOneData(0, 0x10);
        }
      }
    }
    else           //������
    {
      if((BtLevel > 0)&&(BtLevel < 5))
      {
        Ht1621WrOneData(0, 0x10);           //p1
      }
    }
  }
}
void Display_Ten()
{
  static uint8_t Toggle_flag = 0;
  if(n4 == 0)
  {
    if(n3 == 0)
    {
      if(NegativeFlag)  //�Ǹ���
      {
        if(BatVoltLevel==5)
        {
          if(!Toggle_flag)
          {
            Ht1621WrOneData(2, 0x10|0x40);
          }
          Ht1621WrOneData(2, 0x40);
        }
        else
        {
          Ht1621WrOneData(2, 0x10|0x40);
        }
      }
      else            //������
      {
        if(BatVoltLevel==5)
        {
          if(!Toggle_flag)
          {
            Ht1621WrOneData(2, 0x10);
          }
        }
        else
        {
          Ht1621WrOneData(2, 0x10);
        }
      }
    }
    else
    {
      if(BatVoltLevel==5)
      {
        if(Toggle_flag)
        {
          Ht1621WrOneData(2, DispTab[n3]|0x10);
          Ht1621WrOneData(3, DispTab[n3]<<4);
        }
        else
        {
          Ht1621WrOneData(2, DispTab[n3]);                //�޵�ؿ�
          Ht1621WrOneData(3, DispTab[n3]<<4);
        }
      }
      else
      {
        Ht1621WrOneData(2, DispTab[n3]|0x10);
        Ht1621WrOneData(3, DispTab[n3]<<4);
      }
    }
  }
  else
  {
    if(BatVoltLevel==5)
    {
         if(Toggle_flag)
        {
          Ht1621WrOneData(2, DispTab[n3]|0x10);
          Ht1621WrOneData(3, DispTab[n3]<<4);
        }
        else
        {
          Ht1621WrOneData(2, DispTab[n3]);
          Ht1621WrOneData(3, DispTab[n3]<<4);
        }
    }
    else
    {
        Ht1621WrOneData(2, DispTab[n3]|0x10);
        Ht1621WrOneData(3, DispTab[n3]<<4);
    }
  }
  Toggle_flag = ~Toggle_flag;
}
/*******************************************************************************
****��������:
****��������:��ʾ���ź���
****�汾:V1.0
****����:14-2-2014
****��ڲ���:Addr--��ַ Data--����
****���ڲ���:
****˵��:
********************************************************************************/
void Display_Unit(uint8_t bleFlag)  //
{
    if(bleFlag)
    {
      Ht1621WrOneData(4, DispTab[n2]|0x10);
      Ht1621WrOneData(5, DispTab[n2]<<4);
    }
    else
    {
      Ht1621WrOneData(4, DispTab[n2]);
      Ht1621WrOneData(5, DispTab[n2]<<4);
    }
}
/*******************************************************************************
****��������:
****��������:��ʾ���ź���
****�汾:V1.0
****����:14-2-2014
****��ڲ���:Addr--��ַ Data--����
****���ڲ���:
****˵��:
********************************************************************************/
void Display_Deci()
{
   Ht1621WrOneData(6, DispTab[n1]|0x10);
   Ht1621WrOneData(7, DispTab[n1]<<4);
}
/*******************************************************************************
****��������:
****��������:��ʾ���ź���
****�汾:V1.0
****����:14-2-2014
****��ڲ���:Addr--��ַ Data--����
****���ڲ���:
****˵��:
********************************************************************************/
void Display_VotlvlAndUnit(uint8_t BtLevel)
{
  // static uint8_t DisplayFlash = 0;
    switch (BtLevel)
    {
    case 0:
      Ht1621WrOneData(8, 0x80);       
      break;
    case 1:
      Ht1621WrOneData(8, 0x80);
      break;
    case 2:
      Ht1621WrOneData(8, 0x10|0x80);           //p2
      break;
     case 3:
      Ht1621WrOneData(8, 0x30|0x80);           //p2
      break;
     case 4:
      Ht1621WrOneData(8, 0x70|0x80);           //p2
      break;
    case 5:
      Ht1621WrOneData(8, 0x80);            //only show display
      break;
    default:break;
    } 
}
/*******************************************************************************
****��������:
****��������:����ת������
****�汾:V1.0
****����:14-2-2014
****��ڲ���:adc_value-��Ҫת������ֵ
****���ڲ���:
****˵��:
********************************************************************************/
void data_convertor(int16_t adc_value) 
{  
    unsigned int tmp;
    if(adc_value>=0)
    {
      NegativeFlag = 0;
    }
    else
    {
      NegativeFlag = 1;
    }
    tmp=abs(adc_value);         //adc 
    n4=tmp/1000;
    tmp=tmp%1000;
    n3=tmp/100;
    tmp=tmp%100;	
    n2=tmp/10;
    tmp=tmp%10;		
    n1=tmp; 
 }
void display_task()
{
    static unsigned long lcdProcessTimer = 0;
    if (lcdProcessTimer == 0)
    {
	lcdProcessTimer = tick_GetTickCount();
    }
    if (tick_GetElapsedTime(lcdProcessTimer) >= 2000)       //100ms
    {
       	lcdProcessTimer = 0;
        ShowTemp = (int16_t)(10.0*Temperature);
          data_convertor(ShowTemp);
          Ht1621WrAllData(0,Ht1621Tab,16);
          Display(BatVoltLevel,1);
    }					
}