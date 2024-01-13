#include "measurement.h"
#include "adc.h"
#include "NTC.h"
/*******************************************************************************
**函数名称：void MeasurementTask()
**功能描述：初始化ADC
**入口参数：无
**输出：无
*******************************************************************************/
#define N  50


uint8_t BatVoltLevel = 0;
float Temperature = 0.0;

void MeasurementTask(void)
{
    static unsigned long MeasurementProcessTimer = 0;
    static uint8_t MeasurementStep = 0;
    static float FilterResistor[N] = {0.0};//{100.0,100.0,100.0,100.0,100.0,100.0};
    static float SumResistor = 0.0;
    static uint8_t FilterTimes = 0;
    static uint16_t ADCValue = 0;
    float TempValue = 0.0;
    if (MeasurementProcessTimer == 0)
    {
	MeasurementProcessTimer = tick_GetTickCount();
    }
    if (tick_GetElapsedTime(MeasurementProcessTimer) >= 20)        //100ms
    {
       	MeasurementProcessTimer = 0;
	switch (MeasurementStep)
	{
            case 0:
            {
		ADC_Init(NTC_AD_PORT,NTC_AD,ADC1_CHANNEL_3);
		break;
            }
            case 1:
            {
		ADC_Data_Read(&ADCValue);
                TempValue = (10.0/(1024-ADCValue))*ADCValue;
                FilterResistor[FilterTimes] = TempValue;
                 FilterTimes++;
                 if(FilterTimes == N)
                 {
                    FilterTimes = 0;
                //  } 
                    for(unsigned char i = 0; i<N;i++)
                    {
                        SumResistor += FilterResistor[i];//FilterResistor[FilterTimes];
                     }
                    TempValue = SumResistor/N;
                 }
                 SumResistor = 0.0;
                Temperature = lib_NTCBBQ_Res2Temp(TempValue);
		break;
            } 
            case 2:
            {
		ADC_Init(BAT_AD_PORT,BAT_AD,ADC1_CHANNEL_4);
		break;
            }
            case 3:
            {
		ADC_Data_Read(&ADCValue);
		TempValue = 2.0*3.3*ADCValue/1024;
                if(TempValue >= 2.8)
                {
                  BatVoltLevel = 4;
                }
                else if(TempValue >= 2.6)
                {
                  BatVoltLevel = 3;
                }
                else if(TempValue >= 2.4)
                {
                  BatVoltLevel = 2;
                }
                else if(TempValue >= 2.2)
                {
                  BatVoltLevel = 1;
                }
                else if(TempValue >= 2.0)
                {
                  BatVoltLevel = 0;
                }
                else
                {
                  BatVoltLevel = 5;
                }
		break;
            }
            default:break;
	}
	MeasurementStep++;
	if(MeasurementStep >= 4)
	{
            MeasurementStep = 0;
        }
    }
}