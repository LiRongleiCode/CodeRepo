#include "adc.h"
/*******************************************************************************
**函数名称：void ADC_Init()
**功能描述：初始化ADC
**入口参数：无
**输出：无
*******************************************************************************/
void ADC_Init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin,ADC1_Channel_TypeDef ADC1_Channelx)
{
 //设置PC->4 为输入 悬空输入  中断禁止
  GPIO_Init(GPIOx , GPIO_Pin , GPIO_MODE_IN_FL_NO_IT); 
  
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE ,   //单次转换模式
            ADC1_Channelx,               //配置为通道 2 采样
            ADC1_PRESSEL_FCPU_D18 ,        //采样速率为16M / 18
            ADC1_EXTTRIG_TIM ,             //外部触发
            DISABLE ,                      //不使能外部触发
            ADC1_ALIGN_RIGHT ,             //采样数据右对齐
            ADC1_SCHMITTTRIG_ALL ,        //施密特
            DISABLE                        //不使能施密特
            );
  
  ADC1_Cmd(ENABLE);     //启动ADC
}

/*******************************************************************************
**函数名称：void ADC_Data_Read(u16 *AD_Value)
**功能描述：读取ADC完成一次模数转换结果
**入口参数：unsigned int *AD_Value
            *AD_Value ->读取ADC采样数据的指针
**输出：无
*******************************************************************************/
void ADC_Data_Read(uint16_t *AD_Value)
{
  ADC1_Cmd(ENABLE);     //启动ADC
  
  while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);  //等待转换结束
  ADC1_ClearFlag(ADC1_FLAG_EOC);  //清除转换结束标志
  *AD_Value = ADC1_GetConversionValue();  //读取转换数据
}