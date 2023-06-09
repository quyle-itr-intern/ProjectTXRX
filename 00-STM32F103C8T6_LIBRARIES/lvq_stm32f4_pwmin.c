/**
  ******************************************************************************

  CRC For SLVQ32F446RE
  Author:   LVQ
  Updated:  26st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_pwmin.h"

/* Private functions */
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM1Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM2Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM3Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM4Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM5Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM8Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM9Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM10Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM11Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM12Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM13Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM14Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitChannel(LVQ_PWMIN_t* TIM_Data, LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack);

/* Private variables */
__IO float IC2Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;
GPIO_InitTypeDef PWMIN_GPIO_InitStruct;
static uint16_t PWMIN_SubPriorities = 0;

LVQ_PWMIN_Result_t LVQ_PWMIN_InitTimer(
    TIM_TypeDef* TIMx,
    LVQ_PWMIN_t* PWMIN_Data,
    LVQ_PWMIN_Channel_t PWMIN_Channel,
    LVQ_PWMIN_PinsPack_t PinsPack,
    float MinExpectedFrequency,
    IRQn_Type TIMx_IRQn
) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    LVQ_PWMIN_Result_t res;

    /* Check timer */
    if (0
#ifdef TIM6
        || TIMx == TIM6
#endif
#ifdef TIM7
        || TIMx == TIM7
#endif
       ) {
        /* Invalid timer */
        return LVQ_PWMIN_Result_TimerNotValid;
    }

    /* Save timer and settings */
    PWMIN_Data->__TIM = TIMx;

    /* Get properties about timer */
    LVQ_TIMER_GetTimerProperties(PWMIN_Data->__TIM, &PWMIN_Data->__TIM_Data);

    /* Generate settings */
    LVQ_TIMER_CreateDataForPWMFrequency(&PWMIN_Data->__TIM_Data, MinExpectedFrequency);

    /* Enable TIM clock */
    LVQ_TIMER_EnableClock(PWMIN_Data->__TIM);

    /* Set timer settings */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
    if (PWMIN_Data->__TIM_Data.MaxPeriod == 0x00010000) {
        TIM_TimeBaseStruct.TIM_Period = 0x0000FFFF; /* 16-bit */
    } else {
        TIM_TimeBaseStruct.TIM_Period = PWMIN_Data->__TIM_Data.MaxPeriod;
    }
    TIM_TimeBaseStruct.TIM_Prescaler = PWMIN_Data->__TIM_Data.Prescaler - 1;

    /* Initialize TIM */
    TIM_TimeBaseInit(PWMIN_Data->__TIM, &TIM_TimeBaseStruct);

    /* Default GPIO settings */
    PWMIN_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    PWMIN_GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    PWMIN_GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    PWMIN_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

    /* Enable TIMER pin */
    if ((res = LVQ_PWMIN_INT_InitChannel(PWMIN_Data, PWMIN_Channel, PinsPack)) != LVQ_PWMIN_Result_Ok) {
        /* Return result */
        return res;
    }

    /* Calculate subpriority */
    for (PWMIN_Data->__SubPriority = 0; PWMIN_Data->__SubPriority < 16; PWMIN_Data->__SubPriority++) {
        /* Check if it is available */
        if (!(PWMIN_SubPriorities & (1 << PWMIN_Data->__SubPriority))) {
            /* Available */
            PWMIN_SubPriorities |= 1 << PWMIN_Data->__SubPriority;

            /* Stop the loop */
            break;
        }
    }

    /* Enable the TIM global Interrupt */
    NVIC_InitStruct.NVIC_IRQChannel = TIMx_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = PWMIN_PRIORITY;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = PWMIN_Data->__SubPriority;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    /* Set timer settings */
    if (PWMIN_Channel == LVQ_PWMIN_Channel_1) {
        TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    } else {
        TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
    }
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStruct.TIM_ICFilter = 0x0;

    /* Initialize PWM Input config */
    TIM_PWMIConfig(PWMIN_Data->__TIM, &TIM_ICInitStruct);

    /* Select input trigger */
    if (PWMIN_Channel == LVQ_PWMIN_Channel_1) {
        TIM_SelectInputTrigger(PWMIN_Data->__TIM, TIM_TS_TI1FP1);
    } else {
        TIM_SelectInputTrigger(PWMIN_Data->__TIM, TIM_TS_TI2FP2);
    }

    /* Select the slave Mode: Reset Mode */
    TIM_SelectSlaveMode(PWMIN_Data->__TIM, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(PWMIN_Data->__TIM, TIM_MasterSlaveMode_Enable);

    /* TIM enable counter */
    TIM_Cmd(PWMIN_Data->__TIM, ENABLE);

    /* Enable compare interrupt request */
    if (PWMIN_Channel == LVQ_PWMIN_Channel_1) {
        TIM_ITConfig(PWMIN_Data->__TIM, TIM_IT_CC1, ENABLE);
    } else {
        TIM_ITConfig(PWMIN_Data->__TIM, TIM_IT_CC2, ENABLE);
    }

    /* Return OK */
    return LVQ_PWMIN_Result_Ok;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_Get(LVQ_PWMIN_t* PWMIN_Data) 
{
    /* Set new data */
    PWMIN_Data->Frequency = PWMIN_Data->__Freq;
    PWMIN_Data->DutyCycle = PWMIN_Data->__Duty;

    /* Reset data for future calls */
    PWMIN_Data->__Freq = 0;
    PWMIN_Data->__Duty = 0;

    /* Return Ok */
    return LVQ_PWMIN_Result_Ok;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_InterruptHandler(LVQ_PWMIN_t* PWMIN_Data) 
{
    /* Interrupt handler for PWMIN */

    if (TIM_GetITStatus(PWMIN_Data->__TIM, TIM_IT_CC1)) {
        /* Read data */

        /* Clear pending bit */
        TIM_ClearITPendingBit(PWMIN_Data->__TIM, TIM_IT_CC1);

        /* Get the Input Capture value */
        IC2Value = (float)TIM_GetCapture1(PWMIN_Data->__TIM);

        if (IC2Value != 0) {
            /* Duty cycle and frequency */
            DutyCycle = ((float)TIM_GetCapture2(PWMIN_Data->__TIM) * 100) / IC2Value;
            Frequency = (float)PWMIN_Data->__TIM_Data.TimerFrequency / (float)(IC2Value * PWMIN_Data->__TIM_Data.Prescaler);

            /* Save data */
            PWMIN_Data->__Duty = DutyCycle;
            PWMIN_Data->__Freq = Frequency;
        } else {
            /* Reset data */
            DutyCycle = 0;
            Frequency = 0;
        }
    }
    if (TIM_GetITStatus(PWMIN_Data->__TIM, TIM_IT_CC2)) {
        /* Read data */

        /* Clear pending bit */
        TIM_ClearITPendingBit(PWMIN_Data->__TIM, TIM_IT_CC2);

        /* Get the Input Capture value */
        IC2Value = (float)TIM_GetCapture2(PWMIN_Data->__TIM);

        if (IC2Value != 0) {
            /* Duty cycle and frequency */
            DutyCycle = ((float)TIM_GetCapture1(PWMIN_Data->__TIM) * 100) / IC2Value;
            Frequency = (float)PWMIN_Data->__TIM_Data.TimerFrequency / (IC2Value * PWMIN_Data->__TIM_Data.Prescaler);

            /* Save data */
            PWMIN_Data->__Duty = DutyCycle;
            PWMIN_Data->__Freq = Frequency;
        } else {
            /* Reset data */
            DutyCycle = 0;
            Frequency = 0;
        }
    }

    /* Return OK */
    return LVQ_PWMIN_Result_Ok;
}

/* Private functions */
LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitChannel(LVQ_PWMIN_t* TIM_Data, LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
#ifdef TIM1
    if (TIM_Data->__TIM == TIM1) {
        return LVQ_PWMIN_INT_InitTIM1Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM2
    if (TIM_Data->__TIM == TIM2) {
        return LVQ_PWMIN_INT_InitTIM2Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM3
    if (TIM_Data->__TIM == TIM3) {
        return LVQ_PWMIN_INT_InitTIM3Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM4
    if (TIM_Data->__TIM == TIM4) {
        return LVQ_PWMIN_INT_InitTIM4Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM5
    if (TIM_Data->__TIM == TIM5) {
        return LVQ_PWMIN_INT_InitTIM5Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM8
    if (TIM_Data->__TIM == TIM8) {
        return LVQ_PWMIN_INT_InitTIM8Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM9
    if (TIM_Data->__TIM == TIM9) {
        return LVQ_PWMIN_INT_InitTIM9Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM10
    if (TIM_Data->__TIM == TIM10) {
        return LVQ_PWMIN_INT_InitTIM10Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM11
    if (TIM_Data->__TIM == TIM11) {
        return LVQ_PWMIN_INT_InitTIM11Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM12
    if (TIM_Data->__TIM == TIM12) {
        return LVQ_PWMIN_INT_InitTIM12Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM13
    if (TIM_Data->__TIM == TIM13) {
        return LVQ_PWMIN_INT_InitTIM13Pins(Channel, PinsPack);
    }
#endif
#ifdef TIM14
    if (TIM_Data->__TIM == TIM14) {
        return LVQ_PWMIN_INT_InitTIM14Pins(Channel, PinsPack);
    }
#endif

    /* Timer is not valid */
    return LVQ_PWMIN_Result_TimerNotValid;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM1Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_8, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOE
                    LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWMIN_Channel_2:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOE
                    LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM1);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM2Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_0, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_3:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_15, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWMIN_Channel_2:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_1, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_3, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM2);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM3Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_4, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_3:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWMIN_Channel_2:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_3:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM3);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM4Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOD
                    LVQ_GPIO_InitAlternate(GPIOD, GPIO_PIN_12, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWMIN_Channel_2:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOD
                    LVQ_GPIO_InitAlternate(GPIOD, GPIO_PIN_13, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM4);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM5Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_0, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOH
                    LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWMIN_Channel_2:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_1, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOH
                    LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_11, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM5);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM8Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOI
                    LVQ_GPIO_InitAlternate(GPIOI, GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWMIN_Channel_2:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOC
                    LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOI
                    LVQ_GPIO_InitAlternate(GPIOI, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM8);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM9Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_2, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM9);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOE
                    LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM9);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWMIN_Channel_2:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_3, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM9);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOE
                    LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM9);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM10Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_8, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM10);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOF
                    LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM10);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM11Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM11);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOF
                    LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM11);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM12Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_14, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM12);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOH
                    LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM12);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        case LVQ_PWMIN_Channel_2:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOB
                    LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_15, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM12);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOH
                    LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM12);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM13Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_1, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM13);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOF
                    LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_8, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM13);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}

LVQ_PWMIN_Result_t LVQ_PWMIN_INT_InitTIM14Pins(LVQ_PWMIN_Channel_t Channel, LVQ_PWMIN_PinsPack_t PinsPack) 
{
    LVQ_PWMIN_Result_t result = LVQ_PWMIN_Result_PinNotValid;

    switch (Channel) {
        case LVQ_PWMIN_Channel_1:
            switch (PinsPack) {
                case LVQ_PWMIN_PinsPack_1:
#ifdef GPIOA
                    LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM14);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                case LVQ_PWMIN_PinsPack_2:
#ifdef GPIOF
                    LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_TIM14);
                    result = LVQ_PWMIN_Result_Ok;
#endif
                    break;
                default:
                    result = LVQ_PWMIN_Result_PinNotValid;
                    break;
            }
            break;
        default:
            result = LVQ_PWMIN_Result_ChannelNotValid;
            break;
    }

    return result;
}
