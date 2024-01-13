#ifndef __ADC_H
#define __ADC_H
#include "stm8s.h"
#include "HardwareCFG.h"

void ADC_Init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin,ADC1_Channel_TypeDef ADC1_Channel);

void ADC_Data_Read(uint16_t *AD_Value);

#endif