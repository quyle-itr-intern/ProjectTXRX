/**
  ******************************************************************************

  ADC DMA For STM32F446RE
  Author:   LVQ
  Updated:  16st August 2022

  ******************************************************************************
*/

#ifndef __ADC_DMA_H
#define __ADC_DMA_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_delay.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_adc.h"
#include "stm32f4xx_dma.h"

void LVQ_ADC_DMA_Init(ADC_TypeDef* ADCx, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t u32Number, uint32_t u32StartAddress, uint32_t u32Destination);

#endif
