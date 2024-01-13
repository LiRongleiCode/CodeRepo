
/******************** (C) COPYRIGHT  风驰电子嵌入式开发工作室 ********************
 * 文件名  ：main.c
 * 描述    ：STM8S工程模板     
 * 实验平台：风驰电子STM8开发板
 * 库版本  ：V2.0.0
 * 作者    ：ling_guansheng  QQ:779814207
 * 博客    ：
 *修改时间 ：2011-12-20
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_clk.h"
#include "intrinsics.h"
#include "tim1.h"
#include "Ble.h"
#include "uart.h"
#include "lcd.h"
#include "system.h"
#include "measurement.h"
#include "adc.h"
#include "NTC.h"
/*
  产品唯一标识码定义
  生产时可以直接修改下面的值
*/
#define FirstNUM          0x63
#define SecondNUM         0x00
#define ThirdNUM          0x00

/* Private variables ---------------------------------------------------------*/
uint8_t txcnt = 0;
uint8_t rxcnt = 0;
unsigned char Ht1621Tab[]={0x00,0x00,0x00,0x00};
    uint8_t HtFullTab = 0xFF;
/* Private variables ---------------------------------------------------------*/
/*
uint8_t SystemCtlTimer = 0;                  //used for system contron timer
uint8_t KeyProcessTimer = 0; 
uint8_t AdcScanTimer = 0;
uint8_t MeasureTaskTimer = 0;
uint8_t BleTaskTimer = 0;
uint16_t DisplayTaskTimer = 0;
*/
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void BLE_Task(void)
{
    static unsigned long bleProcessTimer = 0;
    if (bleProcessTimer == 0)
    {
	bleProcessTimer = tick_GetTickCount();
    }
    if (tick_GetElapsedTime(bleProcessTimer) >= 500)        //100ms
    {
        bleProcessTimer = 0;
        //////ble rtx api
        txcnt = 3; //txcnt=0 is for rx only application
        //rxcnt = 6; //rxcnt=0 is for tx only application
        rxcnt=0;
        adv_data[18] = FirstNUM;
        adv_data[19] = SecondNUM;
        adv_data[20] = ThirdNUM;
          
        adv_data[22] = (uint8_t)(ShowTemp/10);
        adv_data[23] = 0xFF-adv_data[22];
        adv_data[24] = (uint8_t)(ShowTemp%10);
        BLE_TRX();
    }
}
int main(void)
{
  /*设置内部高速时钟16M为主时钟*/ 
   CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 
  /*!<Set High speed internal clock */
   ADC_Init(NTC_AD_PORT,NTC_AD,ADC1_CHANNEL_3);
   ALL_GPIO_Init();
   Tim1_Init();
   tick_Init();
   BLE_Init(BLE_TX_POWER);
   //Uart_Init();
   Ht1621_Init();
   delay_ms(1000);
   delay_ms(1000);
   Ht1621WrAllData(0,Ht1621Tab,16);
   delay_ms(1000);
   Ht1621WrOneData(0, HtFullTab);
   Ht1621WrOneData(1, HtFullTab<<4);
   Ht1621WrOneData(2, HtFullTab);
   Ht1621WrOneData(3, HtFullTab<<4);
   Ht1621WrOneData(4, HtFullTab);
   Ht1621WrOneData(5, HtFullTab<<4);
   Ht1621WrOneData(6, HtFullTab);
   Ht1621WrOneData(7, HtFullTab<<4);
   Ht1621WrOneData(8, HtFullTab);
   
   __enable_interrupt(); 
  while (1)
  {
      system_control();               //5 分钟自动关机
      key_task();                     //按键开关机
      MeasurementTask();              //ADC 获取温度值和电池电量
      BLE_Task();                     //蓝牙任务
      display_task();
  }
}



#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 风驰电子嵌入式开发工作室 *****END OF FILE****/
