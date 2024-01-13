/******************** (C) COPYRIGHT  ��۵���Ƕ��ʽ���������� ***************************


  ��۵���STM8������Ӳ������
    |--------------------|
    |  LED1-PD0          |
    |  LED2-PD1          |
    |  LED3-PD2          |
    |  LED4-PD3          |
    |--------------------|

****************************************************************************************/

#include "system.h"
#include "tim1.h"
#include "lcd.h"

static uint8_t key_press_flag = 0;
static uint16_t key_press_timer = 0;
static volatile uint8_t key_click = 0;
static uint8_t key_release_flag = 1;
static uint8_t key_release_timer = 0;

void ALL_GPIO_Init(void)
{
   GPIO_Init(POWER_PORT,POWER,GPIO_MODE_OUT_PP_LOW_FAST);               //PD6 ��Դ���ƽ�
   POWER_OFF();
   
   GPIO_Init(KEY_PORT,KEY,GPIO_MODE_IN_PU_NO_IT);                      //PA3 ���������
   
   GPIO_Init(LCD_BL_PORT,LCD_BL,GPIO_MODE_OUT_PP_LOW_FAST);            //PA2 ������ƽ�
   LCD_BL_ON();                                                        // back light is on
   //GPIO_Init(LCD_CSN_PORT,LCD_CSN,GPIO_MODE_OUT_PP_HIGH_FAST);          //PB5 LCDƬѡ���ƽ�
   //GPIO_Init(LCD_RDN_PORT,LCD_RDN,GPIO_MODE_OUT_PP_HIGH_FAST);          //PB4 LCD��ʹ�ܽ�
   //GPIO_Init(LCD_WRN_PORT,LCD_WRN,GPIO_MODE_OUT_PP_HIGH_FAST);          //PC3 LCDдʹ�ܽ�
   //GPIO_Init(LCD_DATA_PORT,LCD_DATA,GPIO_MODE_IN_PU_NO_IT);            //PC4 LCD���������
   //GPIO_Init(LCD_DATA_PORT,LCD_DATA,GPIO_MODE_OUT_PP_HIGH_FAST);          //PC4 LCD���������
   
   GPIO_Init(BLE_CSN_PORT,BLE_CSN,GPIO_MODE_OUT_PP_LOW_FAST);          //PD4 BLEƬѡʹ�ܽ�
   GPIO_Init(BLE_SCK_PORT,BLE_SCK,GPIO_MODE_OUT_PP_LOW_FAST);          //PC5 BLEʱ�������
   GPIO_Init(BLE_MOSI_MISO_PORT,BLE_MOSI_MISO,GPIO_MODE_OUT_PP_LOW_FAST);//PC6 BLE���������
   GPIO_Init(BLE_IRQ_PORT,BLE_IRQ,GPIO_MODE_IN_PU_NO_IT);            //PA1 BLE�ж������
   
   GPIO_Init(NTC_COM_PORT,NTC_COM,GPIO_MODE_OUT_PP_LOW_FAST);          //PC7 NTC������
   NTC_COM_LOW();                                                      //NTC�����˽ӵ�
   
}

void key_process(void)
{
	uint8_t key_press;   //�����Ƿ񱻰���
	key_press = KEY_GET();
	if(!key_press)                          //�ж�����а�������
	{
		key_release_timer = 0;           //��հ����ɿ���ʱ��
		if((++key_press_timer>=40)&&(key_release_flag))        //������³���40ms,
		{
			key_press_flag = 1;
                        key_release_flag = 0;
			key_click = 1;
		}
	}
	else                                   //�����ɿ�
	{
		key_press_timer = 0;
		if((++key_release_timer>=40)&&(key_press_flag))
		{
			key_release_flag = 1;
			key_press_flag = 0;
		}
	}
}

void key_task(void)
{
    static unsigned long keyProcessTimer = 0;
    static uint8_t ToggleFlag = 0;
    if (keyProcessTimer == 0)
    {
	keyProcessTimer = tick_GetTickCount();
    }
    if (tick_GetElapsedTime(keyProcessTimer) >= 20)        //100ms
    {
        keyProcessTimer = 0;
	if(key_click)
	{
            key_click = 0;
            if(!ToggleFlag)
            {
              POWER_ON();
            }
            else
            {
              Ht1621WrAllData(0,Ht1621Tab,16);
              POWER_OFF();
            }
            ToggleFlag = ~ToggleFlag;
	}
    }
}
void system_control(void)
{
    static unsigned long SystemProcessTimer = 0;
    static uint16_t SystemMinTimer = 0;
    if (SystemProcessTimer == 0)
    {
	  SystemProcessTimer = tick_GetTickCount();
     }
    if (tick_GetElapsedTime(SystemProcessTimer) >= 1000)        //1000ms
    {
	  SystemProcessTimer = 0;
	  SystemMinTimer++;
	  if(SystemMinTimer>=300)
	  {
		SystemMinTimer = 0;
                Ht1621WrAllData(0,Ht1621Tab,16);
		POWER_OFF();                       //ʱ�䵽���ػ�
	  }
	}
}

/******************* (C) COPYRIGHT ��۵���Ƕ��ʽ���������� *****END OF FILE****/