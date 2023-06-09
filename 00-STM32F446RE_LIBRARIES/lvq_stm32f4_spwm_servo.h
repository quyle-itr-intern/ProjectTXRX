/**
  ******************************************************************************

  SPWM SERVO For STM32F446RE
  Author:   LVQ
  Updated:  15st April 2023
	Algorithm: 

  ******************************************************************************
*/

#ifndef __SPWM_SERVO_H
#define __SPWM_SERVO_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_delay.h"
#include "main.h"

typedef struct {
		uint32_t				PWMPin;
		GPIO_TypeDef*		PWMPort;
		uint16_t				PWMDuty;
} LVQ_SPWM_SERVO_t;

void LVQ_SPWM_SERVO_Init(LVQ_SPWM_SERVO_t* spwm, uint8_t noServo);
void LVQ_SPWM_Write(uint8_t noServo, uint8_t angle);
void LVQ_SPWM_WriteMicroSeconds(uint8_t noServo, uint16_t microSeconds);
void LVQ_SPWM_AdjustDuty(int16_t minMicroseconds, int16_t maxMicroseconds);

#endif
