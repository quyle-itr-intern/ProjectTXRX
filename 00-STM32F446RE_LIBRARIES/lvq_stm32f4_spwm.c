/**
  ******************************************************************************

  SOFT PWM For STM32F446RE
	Range PWM Generate: 20 - 5000 Hz 
  Author:   LVQ
  Updated:  14st April 2023
	Algorithm: 

  ******************************************************************************
*/

#include "lvq_stm32f4_spwm.h"

static uint8_t __nospwm = 0;
static uint8_t nextServoFlag = 0;
static uint8_t remainingServosFlag = 0x0F;
static uint32_t mind = 65535;
static LVQ_SPWM_t* __spwm;

void PWM_SetNextInterval(void)
{
		mind = 65535;
		for (int i = 0; i < __nospwm; i++) 
		{
			if ( ((1 << i) & remainingServosFlag) && (uint32_t) __spwm[i].PWMDutyCurrent < mind ) 
			{
					mind = __spwm[i].PWMDutyCurrent;
			}
		}
		nextServoFlag = 0;
		for (int i = 0; i < __nospwm; i++) 
		{
			if ( ((1 << i) & remainingServosFlag) && __spwm[i].PWMDutyCurrent == mind )
					nextServoFlag |= (1 << i);
		}
		TIM5->ARR = mind;
		TIM5->CNT = 0;
}

void LVQ_SPWM_SetDuty(uint8_t nospwm, uint8_t duty)
{
		if( __spwm[nospwm].PWMLastDuty == duty)
				return;
		__spwm[nospwm].PWMDuty = duty;
		float t = ( 1.0 / __spwm[nospwm].PWMFrequency ) * 1000000;
		__spwm[nospwm].PWMDutyHigh = (uint16_t) t * ( duty / 100.0 );
		__spwm[nospwm].PWMDutyLow = (uint16_t) t - __spwm[nospwm].PWMDutyHigh;
		if( !__spwm[nospwm].PWMLastDuty ) 
		{
				__spwm[nospwm].PWMDutyCurrent = __spwm[nospwm].PWMDutyHigh;
				__spwm[nospwm].PWMDutyDone = 0;
				remainingServosFlag |= ( 1 << nospwm );
		}
		if( !duty )
		{
				LVQ_GPIO_SetPinLow(__spwm[nospwm].PWMPort, __spwm[nospwm].PWMPin);
				remainingServosFlag &= ~( 1 << nospwm );
		}
		__spwm[nospwm].PWMLastDuty = duty;
		if( TIM5->ARR == 0 || TIM5->CNT == 0 )
				PWM_SetNextInterval();
}

void LVQ_SPWM_Init(LVQ_SPWM_t* spwm, uint16_t no)
{
		__spwm = spwm;
		__nospwm = no;
		for( uint16_t i = 0; i < __nospwm; i++ )
				LVQ_GPIO_Init(__spwm[i].PWMPort, __spwm[i].PWMPin, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_DOWN, LVQ_GPIO_Speed_High);
		for( uint16_t i = 0; i < __nospwm; i++ )
				LVQ_GPIO_SetPinHigh(__spwm[i].PWMPort, __spwm[i].PWMPin);
	
		for( uint16_t i = 0; i < __nospwm; i++ )
		{
				float t = ( 1.0 / __spwm[i].PWMFrequency ) * 1000000;
				__spwm[i].PWMDutyDone = 0;
				__spwm[i].PWMDutyHigh = (uint16_t) t * ( __spwm[i].PWMDuty / 100.0 );
				__spwm[i].PWMDutyLow = (uint16_t) t - __spwm[i].PWMDutyHigh;
		}
		
		for( uint16_t i = 0; i < __nospwm; i++ )
				__spwm[i].PWMDutyCurrent = __spwm[i].PWMDutyHigh;
	
		LVQ_TIMER_EnableClock(TIM5);
		TIM5->PSC = 90 - 1;
		TIM5->ARR = 50000 - 1;
		TIM5->CNT = 0;
		TIM5->CR1 |= TIM_CounterMode_Up;
		TIM5->EGR = TIM_PSCReloadMode_Immediate;    
		TIM5->CR1 &= ~TIM_CR1_ARPE;
		TIM5->DIER &= ~TIM_IT_Update;
		NVIC->IP[TIM5_IRQn] = 0xF0;
		NVIC->ISER[TIM5_IRQn >> 0x05] = (uint32_t)0x01 << (TIM5_IRQn & (uint8_t)0x1F);
		PWM_SetNextInterval();
		TIM5->CR1 |= TIM_CR1_CEN;
		TIM5->DIER |= TIM_IT_Update;
}

void TIM5_IRQHandler(void)
{
		TIM_ClearFlag(TIM5, TIM_IT_Update);
		TIM5->CR1 &= ~TIM_CR1_CEN;
		for (int i = 0; i < __nospwm; i++) 
		{
				if ( ! ((1 << i) & remainingServosFlag) )
						continue;
				if ( ! (nextServoFlag & (1 << i)) ) 
				{
						__spwm[i].PWMDutyCurrent -= mind;
						continue;
				}
				__spwm[i].PWMDutyCurrent -= mind;
				if( __spwm[i].PWMDutyCurrent == 0  && __spwm[i].PWMDutyDone == 0)
				{
						__spwm[i].PWMDutyCurrent = __spwm[i].PWMDutyLow;
						__spwm[i].PWMDutyDone = 1;
				}
				else if ( __spwm[i].PWMDutyCurrent == 0  && __spwm[i].PWMDutyDone == 1)
				{
						__spwm[i].PWMDutyCurrent = __spwm[i].PWMDutyHigh;
						__spwm[i].PWMDutyDone = 0;
				}

				if( !__spwm[i].PWMDutyLow )
				{
						__spwm[i].PWMDutyCurrent = __spwm[i].PWMDutyHigh;
						LVQ_GPIO_SetPinHigh(__spwm[i].PWMPort, __spwm[i].PWMPin);
						goto end;
				}
				if( __spwm[i].PWMDutyDone )
						LVQ_GPIO_SetPinLow(__spwm[i].PWMPort, __spwm[i].PWMPin);
				else
						LVQ_GPIO_SetPinHigh(__spwm[i].PWMPort, __spwm[i].PWMPin);
				end:;
		}
		PWM_SetNextInterval();
		TIM5->CR1 |= TIM_CR1_CEN;
}
