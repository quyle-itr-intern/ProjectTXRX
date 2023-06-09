#ifndef _PWM_H_
#define _PWM_H_

#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	
typedef enum {
	PWM_Channel_1,
	PWM_Channel_2,
	PWM_Channel_3,
	PWM_Channel_4
} PWM_Channel_t;

/**
 * Pin selected for corresponding channel on specific channel
 *
 */
typedef enum {
	PWM_PinsPack_1,
	PWM_PinsPack_2,
	PWM_PinsPack_3,
}PWM_PinsPack_t;

void TIMER_PROPERTIES_EnableClock(TIM_TypeDef* TIMx);
void PWM_INT_InitTIM1Pins(PWM_Channel_t Channel, PWM_PinsPack_t PinsPack);
void PWM_INT_InitTIM2Pins(PWM_Channel_t Channel, PWM_PinsPack_t PinsPack);
void PWM_INT_InitTIM3Pins(PWM_Channel_t Channel, PWM_PinsPack_t PinsPack);
void PWM_INT_InitTIM4Pins(PWM_Channel_t Channel, PWM_PinsPack_t PinsPack);

void BasicPWM1(void);
void BasicPWM2(void);
void TIM_OC(TIM_TypeDef* TIMx,PWM_Channel_t Channel);
void PWM_InitChannel(TIM_TypeDef* TIMx, PWM_Channel_t Channel, PWM_PinsPack_t PinsPack);
void PWM_Init(TIM_TypeDef* TIMx,PWM_Channel_t Channel, PWM_PinsPack_t PinsPack, u16 arr, u16 psc);
void PWM_Set(TIM_TypeDef* TIMx,PWM_Channel_t Channel, u16 pwm);
void PWM_Set_Frequency(TIM_TypeDef* TIMx,PWM_Channel_t Channel,u32 frequency);
#endif
