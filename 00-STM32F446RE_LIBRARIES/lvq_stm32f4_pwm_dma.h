/**
  ******************************************************************************

  DMA PWM For SLVQ32F446RE
  Author:   LVQ
  Updated:  24st June 2022

  ******************************************************************************
*/

#ifndef __PWM_DMA_H
#define __PWM_DMA_H 

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_pwm.h"
#include "lvq_stm32f4_dma.h"
#include "lvq_stm32f4_usart.h"
#include "lvq_stm32f4_ws2812.h"

void LVQ_PWM_DMA_Init(void);
uint8_t LVQ_PWM_DMA_SetDuty(uint16_t* DataArray);

#endif
