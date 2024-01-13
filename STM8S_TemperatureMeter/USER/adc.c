#include "adc.h"
/*******************************************************************************
**�������ƣ�void ADC_Init()
**������������ʼ��ADC
**��ڲ�������
**�������
*******************************************************************************/
void ADC_Init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin,ADC1_Channel_TypeDef ADC1_Channelx)
{
 //����PC->4 Ϊ���� ��������  �жϽ�ֹ
  GPIO_Init(GPIOx , GPIO_Pin , GPIO_MODE_IN_FL_NO_IT); 
  
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE ,   //����ת��ģʽ
            ADC1_Channelx,               //����Ϊͨ�� 2 ����
            ADC1_PRESSEL_FCPU_D18 ,        //��������Ϊ16M / 18
            ADC1_EXTTRIG_TIM ,             //�ⲿ����
            DISABLE ,                      //��ʹ���ⲿ����
            ADC1_ALIGN_RIGHT ,             //���������Ҷ���
            ADC1_SCHMITTTRIG_ALL ,        //ʩ����
            DISABLE                        //��ʹ��ʩ����
            );
  
  ADC1_Cmd(ENABLE);     //����ADC
}

/*******************************************************************************
**�������ƣ�void ADC_Data_Read(u16 *AD_Value)
**������������ȡADC���һ��ģ��ת�����
**��ڲ�����unsigned int *AD_Value
            *AD_Value ->��ȡADC�������ݵ�ָ��
**�������
*******************************************************************************/
void ADC_Data_Read(uint16_t *AD_Value)
{
  ADC1_Cmd(ENABLE);     //����ADC
  
  while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);  //�ȴ�ת������
  ADC1_ClearFlag(ADC1_FLAG_EOC);  //���ת��������־
  *AD_Value = ADC1_GetConversionValue();  //��ȡת������
}