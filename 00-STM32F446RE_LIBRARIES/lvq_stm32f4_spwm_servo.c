/**
  ******************************************************************************

  SPWM SERVO For STM32F446RE
  Author:   LVQ
  Updated:  15st April 2023
	Algorithm: 

  ******************************************************************************
*/

#include "lvq_stm32f4_spwm_servo.h"

#define MAX_SERVO 10

static LVQ_SPWM_SERVO_t* __spwm_servo;
static uint8_t __noFullServo = 1;
static uint8_t __noServo = 0;
static uint8_t __servoIndex = 0;
static uint16_t __noServoFree = 0;
static uint16_t __minMicroseconds = 1000;
static uint16_t __maxMicroseconds = 2000;
static int16_t __minAngle = 0;
static int16_t __maxAngle = 180;

void LVQ_SPWM_SERVO_Init(LVQ_SPWM_SERVO_t* spwm, uint8_t noServo)
{
	__spwm_servo = spwm;
	__noServo = noServo;
	if( __noServo == 10 ) __noFullServo = 0;
	for( uint16_t i = 0; i < noServo; i++ )
				LVQ_GPIO_Init(__spwm_servo[i].PWMPort, __spwm_servo[i].PWMPin, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_DOWN, LVQ_GPIO_Speed_High);
	
	unsigned long clear = 20000;
  for (unsigned char i = 0; i < __noServo; i++)
    clear -= __spwm_servo[i].PWMDuty;
  __noServoFree = clear;
	
	LVQ_TIMER_EnableClock(TIM12);
	TIM12->PSC = 90 - 1;
	TIM12->ARR = 50000 - 1;
	TIM12->CNT = 0;
	TIM12->CR1 |= TIM_CounterMode_Up;
	TIM12->EGR |= TIM_PSCReloadMode_Immediate;    
	TIM12->CR1 &= ~TIM_CR1_ARPE;
	TIM12->DIER |= TIM_IT_Update;
	NVIC->IP[TIM8_BRK_TIM12_IRQn] = 0xF0;
	NVIC->ISER[TIM8_BRK_TIM12_IRQn >> 0x05] = (uint32_t)0x01 << (TIM8_BRK_TIM12_IRQn & (uint8_t)0x1F);
	TIM12->CR1 |= TIM_CR1_CEN;
	TIM12->ARR = 50000 - 1;
	TIM12->CNT = 0;
}

void LVQ_SPWM_Write(uint8_t noServo, uint8_t angle)
{
		uint16_t clear = 20000;
		__spwm_servo[noServo].PWMDuty = __minMicroseconds + ( ( ( __maxMicroseconds - __minMicroseconds )* 1.0 ) / ( ( __maxAngle - __minAngle ) * 1.0 ) ) * angle;
		if( __noServo == 10 )
				goto point_1;
		for (uint8_t i = 0; i < __noServo; i++)
			clear -= __spwm_servo[i].PWMDuty;
		__noServoFree = clear;
		point_1:;
}

void LVQ_SPWM_WriteMicroSeconds(uint8_t noServo, uint16_t microSeconds)
{
		uint16_t clear = 20000;
		__spwm_servo[noServo].PWMDuty = microSeconds;
		if( __noServo == 10 )
				goto point_2;
		for (uint8_t i = 0; i < __noServo; i++)
			clear -= __spwm_servo[i].PWMDuty;
		__noServoFree = clear;
		point_2:;
}

void LVQ_SPWM_AdjustDuty(int16_t minMicroseconds, int16_t maxMicroseconds)
{
		if ( minMicroseconds < 0 || maxMicroseconds < 0 || ( minMicroseconds > maxMicroseconds ) )
				return;
		__minMicroseconds = minMicroseconds;
		__maxMicroseconds = maxMicroseconds;
}

void TIM8_BRK_TIM12_IRQHandler(void)
{
		if( TIM12->SR & TIM_FLAG_Update)
		{
				TIM12->SR = (uint16_t)~TIM_FLAG_Update;
				TIM12->CR1 &= ~TIM_CR1_CEN;
				if( __servoIndex == __noServo && MAX_SERVO != __noServo)
						TIM12->ARR = __noServoFree;
				else
						TIM12->ARR = __spwm_servo[__servoIndex].PWMDuty;
				if (__servoIndex)
						LVQ_GPIO_SetPinLow(__spwm_servo[__servoIndex-1].PWMPort, __spwm_servo[__servoIndex-1].PWMPin);
				if( __servoIndex != __noServo) 
						LVQ_GPIO_SetPinHigh(__spwm_servo[__servoIndex].PWMPort, __spwm_servo[__servoIndex].PWMPin);
				__servoIndex++;
				if (__servoIndex == __noServo + __noFullServo)
					__servoIndex = 0;
				TIM12->CNT = 0;
				TIM12->CR1 |= TIM_CR1_CEN;
		}
}
