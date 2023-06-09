/**
  ******************************************************************************

  SPWM SERVO SERVO For STM32F446RE
  Author:   LVQ
  Updated:  13st April 2023
	Algorithm: Separate Time

  ******************************************************************************
*/

#include "lvq_stm32f4_soft_pwm.h"

static uint8_t NoServo = 0;
static uint8_t nextServoFlag = 0;
static uint8_t remainingServosFlag = 0x0F;
static uint8_t startCycle = 0;
static uint32_t mind = 65535;

static LVQ_SPWM_SERVO_t* __spwm_servo;

void PWM_SetNextInterval(void)
{
	 mind = 65535;
  for (int i = 0; i < NoServo; i++) 
	{
			if ( ((1 << i) & remainingServosFlag) && (uint32_t) __spwm_servo[i].PWMDutyCurrent < mind && __spwm_servo[i].PWMDuty != 0) 
			{
					mind = __spwm_servo[i].PWMDutyCurrent;
			}
			if( __spwm_servo[i].PWMDuty == 0 )
					remainingServosFlag &= ~(1 << i);
  }

  nextServoFlag = 0;
  for (int i = 0; i < NoServo; i++) 
	{
			if ( ((1 << i) & remainingServosFlag) && __spwm_servo[i].PWMDutyCurrent == mind )
					nextServoFlag |= (1 << i);
  }
	
	if ( ( remainingServosFlag == 0 ) && !startCycle) 
	{
			startCycle = 1;
			mind = 0;
			
			for (int i = 0; i < NoServo; i++) 
			{
					if ( __spwm_servo[i].PWMDuty > mind ) 
					{
							mind = __spwm_servo[i].PWMDuty;
					}
			}
			mind = 20000 - mind;
	}
	TIM7->ARR = mind;
}

void LVQ_SPWM_SERVO_Init(LVQ_SPWM_SERVO_t* spwm_servo, uint16_t noServo)
{
		__spwm_servo = spwm_servo;
		NoServo = noServo;
		for( uint16_t i = 0; i < noServo; i++ )
				LVQ_GPIO_Init(spwm_servo[i].PWMPort, spwm_servo[i].PWMPin, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_DOWN, LVQ_GPIO_Speed_High);
		for( uint16_t i = 0; i < noServo; i++ )
				LVQ_GPIO_SetPinHigh(spwm_servo[i].PWMPort, spwm_servo[i].PWMPin);
		for( uint16_t i = 0; i < noServo; i++ )
				spwm_servo[0].PWMDutyCurrent = spwm_servo[0].PWMDuty;
		LVQ_TIMER_EnableClock(TIM7);
		TIM7->PSC = 90 - 1;
		TIM7->ARR = 50000 - 1;
		TIM7->CNT = 0;
		TIM7->CR1 |= TIM_CounterMode_Up;
		TIM7->EGR = TIM_PSCReloadMode_Immediate;    
		TIM7->CR1 &= ~TIM_CR1_ARPE;
		TIM7->DIER &= ~TIM_IT_Update;
		NVIC->IP[TIM7_IRQn] = 0xF0;
		NVIC->ISER[TIM7_IRQn >> 0x05] = (uint32_t)0x01 << (TIM7_IRQn & (uint8_t)0x1F);
		TIM7->CR1 |= TIM_CR1_CEN;
		TIM7->DIER |= TIM_IT_Update;
}

void TIM7_IRQHandler(void)
{
		TIM_ClearFlag(TIM7, TIM_IT_Update);
		TIM7->CR1 &= ~TIM_CR1_CEN;
		if( !startCycle )
		{
				for (int i = 0; i < NoServo; i++) 
				{
						if ( ! ((1 << i) & remainingServosFlag) )
								continue;
						if ( ! (nextServoFlag & (1 << i)) ) 
						{
								__spwm_servo[i].PWMDutyCurrent -= mind;
								continue;
						}
						__spwm_servo[i].PWMDutyCurrent -= mind;
						if( __spwm_servo[i].PWMDutyCurrent == 0 )
								remainingServosFlag &= ~(1 << i);
						LVQ_GPIO_SetPinLow(__spwm_servo[i].PWMPort, __spwm_servo[i].PWMPin);
				}
		}
		if( startCycle )
		{
				for (int i = 0; i < NoServo; i++) 
				{
						LVQ_GPIO_SetPinHigh(__spwm_servo[i].PWMPort, __spwm_servo[i].PWMPin);
						remainingServosFlag |= (1 << i);
						__spwm_servo[i].PWMDutyCurrent =  __spwm_servo[i].PWMDuty;
				}
				startCycle = 0;
		}
		PWM_SetNextInterval();
		TIM7->CNT  = 0;
		TIM7->CR1 |= TIM_CR1_CEN;
}
