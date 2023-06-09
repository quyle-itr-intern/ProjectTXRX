/**
  ******************************************************************************

  SPWM SERVO SERVO For STM32F446RE
  Author:   LVQ
  Updated:  13st April 2023
	Algorithm: Separate Time

  ******************************************************************************
*/

#ifndef __SPWM_SERVO_H
#define __SPWM_SERVO_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_delay.h"
#include "main.h"

typedef struct 
{
		uint32_t				PWMPin;
		GPIO_TypeDef*		PWMPort;
		uint16_t				PWMDuty;
		uint16_t				PWMDutyCurrent;
} LVQ_SPWM_SERVO_t;

void LVQ_SPWM_SERVO_Init(LVQ_SPWM_SERVO_t* spwm_servo, uint16_t noServo);

#endif
