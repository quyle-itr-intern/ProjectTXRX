/**
  ******************************************************************************

  SOFT PWM For STM32F446RE
	Range PWM Generate: 20 - 5000 Hz 
  Author:   LVQ
  Updated:  14st April 2023
	Algorithm: 

  ******************************************************************************
*/

#ifndef __SPWM_H
#define __SPWM_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_delay.h"
#include "main.h"

typedef struct 
{
		uint32_t				PWMPin;
		GPIO_TypeDef*		PWMPort;
		uint16_t				PWMFrequency;
		uint16_t				PWMDuty;
		uint16_t				PWMDutyLow;
		uint16_t				PWMDutyHigh;
		uint16_t				PWMDutyCurrent;
		uint8_t 				PWMDutyDone;
		uint8_t 				PWMLastDuty;
} LVQ_SPWM_t;

void LVQ_SPWM_Init(LVQ_SPWM_t* spwm, uint16_t no);
void LVQ_SPWM_SetDuty(uint8_t __nospwm, uint8_t duty);

#endif
