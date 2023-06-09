/**
  ******************************************************************************

  PWM For SLVQ32F446RE
  Author:   LVQ
  Updated:  19st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_pwm.h"

/* Private functions */
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM1Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM2Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM3Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM4Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM5Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM8Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM9Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM10Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM11Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM12Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM13Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM14Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);

LVQ_TIMER_Result_t LVQ_TIMER_EnableClock(TIM_TypeDef* TIMx);
LVQ_TIMER_Result_t LVQ_TIMER_GetTimerProperties(TIM_TypeDef* TIMx, LVQ_TIMER_PROPERTIES_t* Timer_Data);
LVQ_TIMER_Result_t LVQ_TIMER_CreateDataForPWMFrequency(LVQ_TIMER_PROPERTIES_t* Timer_Data, double PWMFrequency);
LVQ_TIMER_Result_t LVQ_TIMER_OC(LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel);

LVQ_PWM_Result_t LVQ_PWM_SetDuty(LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel, uint32_t Pulse)
{
		/* Check pulse length */
    if (Pulse >= TIM_Data->Period) {
        /* Pulse too high */
        return LVQ_PWM_Result_PulseTooHigh;
    }
		uint8_t ch = (uint8_t)Channel;
		/* Save pulse value */
    TIM_Data->CH_Periods[ch] = Pulse;
		
		switch (Channel) 
		{
			case LVQ_PWM_Channel_1:
					TIM_SetCompare1(TIM_Data->TIM, Pulse);
					break;
			case LVQ_PWM_Channel_2:
					TIM_SetCompare2(TIM_Data->TIM, Pulse);
					break;
			case LVQ_PWM_Channel_3:
					TIM_SetCompare3(TIM_Data->TIM, Pulse);
					break;
			case LVQ_PWM_Channel_4:
					TIM_SetCompare4(TIM_Data->TIM, Pulse);
					break;
			default:
					break;
		}
		return LVQ_PWM_Result_Ok;
}

LVQ_PWM_Result_t LVQ_PWM_SetDutyPercent(LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel, float percent) 
{
    /* Check input value */
    if (percent > 100) {
        return LVQ_PWM_SetDuty(TIM_Data, Channel, TIM_Data->Period);
    } else if (percent <= 0) {
        return LVQ_PWM_SetDuty(TIM_Data, Channel, 0);
    }

    /* Set channel value */
    return LVQ_PWM_SetDuty(TIM_Data, Channel, (uint32_t)((float)(TIM_Data->Period - 1) * percent) / 100);
}

LVQ_PWM_Result_t LVQ_PWM_SetDutyMicros(LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel, uint32_t micros) 
{
    /* If we choose too much micro seconds that we have valid */
    if (micros > TIM_Data->Micros) {
        /* Too high pulse */
        return LVQ_PWM_Result_PulseTooHigh;
    }

    /* Set PWM channel */
    return LVQ_PWM_SetDuty(TIM_Data, Channel, (uint32_t)((TIM_Data->Period - 1) * micros) / TIM_Data->Micros);
}

LVQ_PWM_Result_t LVQ_PWM_InitTimer(TIM_TypeDef* TIMx, LVQ_PWM_TIM_t* TIM_Data, double PWMFrequency)
{
	  TIM_TimeBaseInitTypeDef TIM_BaseStruct;
		LVQ_TIMER_PROPERTIES_t Timer_Data;
	
	  /* Check valid timer */
    if (0
		#ifdef TIM6
						|| TIMx == TIM6
		#endif
		#ifdef TIM7
						|| TIMx == TIM7
		#endif
    ) {
        /* Timers TIM6 and TIM7 can not provide PWM feature */
        return LVQ_PWM_Result_TimerNotValid;
    }
		/* Save timer */
    TIM_Data->TIM = TIMx;
		
		LVQ_TIMER_GetTimerProperties(TIMx, &Timer_Data);
		/* Check for maximum timer frequency */
    if (PWMFrequency > Timer_Data.TimerFrequency) 
		{
        /* Frequency too high */
        return LVQ_PWM_Result_FrequencyTooHigh;
    } 
		else if (PWMFrequency == 0) 
		{
        /* Not valid frequency */
			return LVQ_PWM_Result_FrequencyTooLow;
    }
		
		/* Enable clock for Timer */
    LVQ_TIMER_EnableClock(TIMx);
		
		/* Generate settings */
		LVQ_TIMER_Result_t status = LVQ_TIMER_CreateDataForPWMFrequency(&Timer_Data, PWMFrequency);
		if(status == LVQ_TIMER_Result_FrequencyTooLow) return LVQ_PWM_Result_FrequencyTooLow;
		if(status == LVQ_TIMER_Result_FrequencyTooHigh) return LVQ_PWM_Result_FrequencyTooHigh;

		/* Data are OK */
		TIM_Data->Frequency = PWMFrequency;
    TIM_Data->Micros = 1000000 / PWMFrequency;
    TIM_Data->Period = Timer_Data.Period;
    TIM_Data->Prescaler = Timer_Data.Prescaler;
		
		/* Set timer options */
    TIM_BaseStruct.TIM_Prescaler = Timer_Data.Prescaler - 1;
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = Timer_Data.Period - 1;
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;

    /* Initialize timer */
    TIM_TimeBaseInit(TIMx, &TIM_BaseStruct);

    /* Preload enable */
    TIM_ARRPreloadConfig(TIMx, ENABLE);
		
		/* Additional for TIM1 and LVQM8 */
		if (0
#ifdef TIM1
        || TIMx == TIM1
#endif
#ifdef TIM8
        || TIMx == TIM8
#endif
       ) {
        /* Enable PWM outputs */
        TIM_CtrlPWMOutputs(TIMx, ENABLE);
    }
			 
		/* Start timer */
    TIM_Cmd(TIMx, ENABLE);  

    /* Set default values */
    TIM_Data->CH_Init = 0;

    /* Return OK */
    return LVQ_PWM_Result_Ok;	
}

LVQ_PWM_Result_t LVQ_PWM_InitChannel(LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
		TIM_Data->CH_Init = Channel;
#ifdef TIM1
    if (TIM_Data->TIM == TIM1) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM1Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM2
    if (TIM_Data->TIM == TIM2) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM2Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM3
    if (TIM_Data->TIM == TIM3) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM3Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM4
    if (TIM_Data->TIM == TIM4) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM4Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM5
    if (TIM_Data->TIM == TIM5) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM5Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM8
    if (TIM_Data->TIM == TIM8) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM8Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM9
    if (TIM_Data->TIM == TIM9) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM9Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM10
    if (TIM_Data->TIM == TIM10) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM10Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM11
    if (TIM_Data->TIM == TIM11) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM11Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM12
    if (TIM_Data->TIM == TIM12) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM12Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM13
    if (TIM_Data->TIM == TIM13) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM13Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM14
    if (TIM_Data->TIM == TIM14) {
				/* Initialize OC Timer */
				LVQ_TIMER_OC(TIM_Data, Channel);
        return LVQ_PWM_INT_InitTIM14Pins(Channel, PinsPack);
    }
#endif

    /* Timer is not valid */
    return LVQ_PWM_Result_TimerNotValid;
}

LVQ_TIMER_Result_t LVQ_TIMER_OC(LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel)
{
		TIM_OCInitTypeDef TIM_OCStruct;
		/* Common settings */
    TIM_OCStruct.TIM_Pulse = 0;
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
		
		/* Select proper channel */
    switch (Channel) {
        case LVQ_PWM_Channel_1:
                /* Init channel */
                TIM_OC1Init(TIM_Data->TIM, &TIM_OCStruct);
                TIM_OC1PreloadConfig(TIM_Data->TIM, TIM_OCPreload_Enable);
            break;
        case LVQ_PWM_Channel_2:
                /* Init channel */
                TIM_OC2Init(TIM_Data->TIM, &TIM_OCStruct);
                TIM_OC2PreloadConfig(TIM_Data->TIM, TIM_OCPreload_Enable);
            break;
        case LVQ_PWM_Channel_3:
                /* Init channel */
                TIM_OC3Init(TIM_Data->TIM, &TIM_OCStruct);
                TIM_OC3PreloadConfig(TIM_Data->TIM, TIM_OCPreload_Enable);
            break;
        case LVQ_PWM_Channel_4:
                /* Init channel */
                TIM_OC4Init(TIM_Data->TIM, &TIM_OCStruct);
                TIM_OC4PreloadConfig(TIM_Data->TIM, TIM_OCPreload_Enable);
            break;
        default:
								return LVQ_TIMER_Result_Error;
    }
		/* Return everything OK */
    return LVQ_TIMER_Result_Ok;
}

LVQ_TIMER_Result_t LVQ_TIMER_CreateDataForPWMFrequency(LVQ_TIMER_PROPERTIES_t* Timer_Data, double PWMFrequency) 
{
		if (PWMFrequency > Timer_Data->TimerFrequency) {
        /* Reset values */
        Timer_Data->Prescaler = 0;
        Timer_Data->Period = 0;
        Timer_Data->Frequency = 0;

        /* Frequency too high */
        return LVQ_TIMER_Result_FrequencyTooHigh;
    } else if (PWMFrequency == 0) {
        /* Reset values */
        Timer_Data->Prescaler = 0;
        Timer_Data->Period = 0;
        Timer_Data->Frequency = 0;

        /* Not valid frequency */
        return LVQ_TIMER_Result_FrequencyTooLow;
    }

    /* Fix for 16/32bit timers */
    if (Timer_Data->MaxPeriod <= 0xFFFF) {
        Timer_Data->MaxPeriod++;
    }

    /* Get minimum prescaler and maximum resolution for timer */
    Timer_Data->Prescaler = 0;
    do {
        /* Get clock */
        Timer_Data->Period = (Timer_Data->TimerFrequency / (Timer_Data->Prescaler + 1));
        /* Get period */
        Timer_Data->Period = (Timer_Data->Period / PWMFrequency);
        /* Increase prescaler value */
        Timer_Data->Prescaler++;
    } while (Timer_Data->Period > (Timer_Data->MaxPeriod) && Timer_Data->Prescaler <= (Timer_Data->MaxPrescaler + 1));
    /* Check for too low frequency */
    if (Timer_Data->Prescaler > (Timer_Data->MaxPrescaler + 1)) {
        /* Reset values */
        Timer_Data->Prescaler = 0;
        Timer_Data->Period = 0;
        Timer_Data->Frequency = 0;

        /* Prescaler too high, frequency is too low for use */
        return LVQ_TIMER_Result_FrequencyTooLow;
    }

    /* Set frequency */
    Timer_Data->Frequency = PWMFrequency;

    /* Return OK */
    return LVQ_TIMER_Result_Ok;
}

LVQ_TIMER_Result_t LVQ_TIMER_EnableClock(TIM_TypeDef* TIMx)
{
#ifdef TIM1
    if (TIMx == TIM1) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    }
#endif
#ifdef TIM2
    if (TIMx == TIM2) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    }
#endif
#ifdef TIM3
    if (TIMx == TIM3) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    }
#endif
#ifdef TIM4
    if (TIMx == TIM4) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    }
#endif
#ifdef TIM5
    if (TIMx == TIM5) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    }
#endif
#ifdef TIM6
    if (TIMx == TIM6) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    }
#endif
#ifdef TIM7
    if (TIMx == TIM7) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    }
#endif
#ifdef TIM8
    if (TIMx == TIM8) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
    }
#endif
#ifdef TIM9
    if (TIMx == TIM9) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    }
#endif
#ifdef TIM10
    if (TIMx == TIM10) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
    }
#endif
#ifdef TIM11
    if (TIMx == TIM11) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
    }
#endif
#ifdef TIM12
    if (TIMx == TIM12) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
    }
#endif
#ifdef TIM13
    if (TIMx == TIM13) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;
    }
#endif
#ifdef TIM14
    if (TIMx == TIM14) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    }
#endif

    /* Return OK */
    return LVQ_TIMER_Result_Ok;
}

LVQ_TIMER_Result_t LVQ_TIMER_GetTimerProperties(TIM_TypeDef* TIMx, LVQ_TIMER_PROPERTIES_t* Timer_Data)
{
	  RCC_ClocksTypeDef RCC_ClocksStruct;

    /* Get clocks */
    RCC_GetClocksFreq(&RCC_ClocksStruct);
	
		/* All timers have 16-bit prescaler */
    Timer_Data->MaxPrescaler = 0xFFFF;
	
		if (0 /* 32bit timers with PCLK2 max frequency */
#ifdef TIM2
        || TIMx == TIM2
#endif
#ifdef TIM5
        || TIMx == TIM5
#endif
       ) {
        Timer_Data->TimerFrequency = RCC_ClocksStruct.PCLK2_Frequency;  /* Clock */
        Timer_Data->MaxPeriod = 0xFFFFFFFF;                             /* Max period */

        /* Timer valid */
				 return LVQ_TIMER_Result_Ok;
    } 
		else if (0    /* 16bit timers with HCLK clock frequency */
#ifdef TIM1
               || TIMx == TIM1
#endif
#ifdef TIM8
               || TIMx == TIM8
#endif
#ifdef TIM9
               || TIMx == TIM9
#endif
#ifdef TIM10
               || TIMx == TIM10
#endif
#ifdef TIM11
               || TIMx == TIM11
#endif
              ) {
        Timer_Data->TimerFrequency = RCC_ClocksStruct.HCLK_Frequency;   /* Clock */
        Timer_Data->MaxPeriod = 0xFFFF;                                 /* Max period */

        /* Timer valid */
        return LVQ_TIMER_Result_Ok;
    } 
		else if (0    /* 16bit timers with PCLK2 clock frequency */
#ifdef TIM3
               || TIMx == TIM3
#endif
#ifdef TIM4
               || TIMx == TIM4
#endif
#ifdef TIM6
               || TIMx == TIM6
#endif
#ifdef TIM7
               || TIMx == TIM7
#endif
#ifdef TIM12
               || TIMx == TIM12
#endif
#ifdef TIM13
               || TIMx == TIM13
#endif
#ifdef TIM14
               || TIMx == TIM14
#endif
              ) {
        Timer_Data->TimerFrequency = RCC_ClocksStruct.PCLK2_Frequency;  /* Clock */
        Timer_Data->MaxPeriod = 0xFFFF;                                 /* Max period */

        /* Timer valid */
        return LVQ_TIMER_Result_Ok;
    }

    /* Timer is not valid */
    return LVQ_TIMER_Result_TimerNotValid;
}

/* Private functions */
LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM1Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_8, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOE
                    LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_2:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOE
                    LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_3:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOE
                    LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_13, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_4:
            switch (PinsPack)
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_11, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOE
                    LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_14, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM2Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_0, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_3:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_15, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_2:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_1, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_3, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_3:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_2, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_4:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_3, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_11, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM3Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_4, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_3:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_2:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_3:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_3:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_0, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_8, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_4:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_1, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM4Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOD
                    LVQ_GPIO_InitAlternate(GPIOD, GPIO_PIN_12, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_2:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOD
                    LVQ_GPIO_InitAlternate(GPIOD, GPIO_PIN_13, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_3:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_8, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOD
                    LVQ_GPIO_InitAlternate(GPIOD, GPIO_PIN_14, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_4:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOD
                    LVQ_GPIO_InitAlternate(GPIOD, GPIO_PIN_15, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM5Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_0, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOH
                    LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_2:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_1, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOH
                    LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_11, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_3:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_2, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOH
                    LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_12, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_4:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_3, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOI
                    LVQ_GPIO_InitAlternate(GPIOI, GPIO_PIN_0, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM8Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOI
                    LVQ_GPIO_InitAlternate(GPIOI, GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_2:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOI
                    LVQ_GPIO_InitAlternate(GPIOI, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_3:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_8, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOI
                    LVQ_GPIO_InitAlternate(GPIOI, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_4:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOI
                    LVQ_GPIO_InitAlternate(GPIOI, GPIO_PIN_8, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM9Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_2, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM9);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOE
                    LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM9);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_2:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_3, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM9);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOE
                    LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM9);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM10Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_8, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM10);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOF
                    LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM10);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM11Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM11);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOF
                    LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM11);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM12Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_14, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM12);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOH
                    LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM12);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWM_Channel_2:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_15, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM12);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOH
                    LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM12);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM13Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack) 
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM13);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOF
                    LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_8, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM13);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}

LVQ_PWM_Result_t LVQ_PWM_INT_InitTIM14Pins(LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack) 
{
    LVQ_PWM_Result_t result = LVQ_PWM_Result_PinNotValid;

    switch (Channel) 
		{
        case LVQ_PWM_Channel_1:
            switch (PinsPack)
						{
                case LVQ_PWM_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM14);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                case LVQ_PWM_PinsPack_2:
#ifdef GPIOF
                    LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM14);
                    result = LVQ_PWM_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWM_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWM_Result_ChannelNotValid;
            break;
    }
    return result;
}
