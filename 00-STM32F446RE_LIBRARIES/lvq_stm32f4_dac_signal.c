/**
  ******************************************************************************

  DAC SIGNAL For STM32F446RE
  Author:   LVQ
  Updated:  13st August 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_dac_signal.h"

LVQ_DAC_SIGNAL_Result_t LVQ_DAC_SIGNAL_SetCustomSignal(LVQ_DAC_SIGNAL_Channel_t DACx, uint16_t* Signal_Data, uint16_t Signal_Length, double frequency);

uint16_t DAC_SIGNAL_Sinus[DAC_SIGNAL_SINUS_LENGTH] = {
    2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056,
    4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
    2047, 1647, 1263, 909, 599, 344, 155, 38,
    0, 38, 155, 344, 599, 909, 1263, 1647
};

uint16_t DAC_SIGNAL_Triangle[DAC_SIGNAL_TRIANGLE_LENGTH] = {
    0, 256, 512, 768, 1024, 1279, 1535, 1791,
    2047, 2303, 2559, 2815, 3071, 3326, 3582, 3838,
    4095, 3838, 3582, 3326, 3071, 2815, 2559, 2303,
    2047, 1791, 1535, 1279, 1024, 768, 512, 256
};

uint16_t DAC_SIGNAL_Sawtooth[DAC_SIGNAL_SAWTOOTH_LENGTH] = {
    0, 132, 264, 396, 528, 660, 792, 924,
    1057, 1189, 1321, 1453, 1585, 1717, 1849, 1981,
    2113, 2245, 2377, 2509, 2641, 2773, 2905, 3037,
    3170, 3302, 3434, 3566, 3698, 3830, 3962, 4095
};

uint16_t DAC_SIGNAL_Square[DAC_SIGNAL_SQUARE_LENGTH] = {
    0, 4095
};

TIM_TypeDef* DAC_TIM[2];
uint8_t dac_timer_set[2] = {0, 0};

LVQ_DAC_SIGNAL_Result_t LVQ_DAC_SIGNAL_Init(LVQ_DAC_SIGNAL_Channel_t DACx, TIM_TypeDef* TIMx) 
{
    uint16_t GPIO_Pin;

    /* Check used timer */
    /* Set proper trigger */
    if (
        TIMx == TIM2 ||
        TIMx == TIM4 ||
        TIMx == TIM5 ||
        TIMx == TIM6 ||
        TIMx == TIM7 ||
        TIMx == TIM8
    ) {
        /* Set timer */
        DAC_TIM[DACx] = TIMx;
        /* Set flag */
        dac_timer_set[DACx] = 1;
    } else {
        /* Timer is not valid */
        return LVQ_DAC_SIGNAL_Result_TimerNotValid;
    }


    /* Select proper GPIO pin */
    if (DACx == LVQ_SIGNAL_DAC1) {
        GPIO_Pin = GPIO_PIN_4;
    } else {
        GPIO_Pin = GPIO_PIN_5;
    }

    /* Initialize proper GPIO pin */
    LVQ_GPIO_Init(GPIOA, GPIO_Pin, LVQ_GPIO_Mode_AN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_Fast);

    /* Return OK */
    return LVQ_DAC_SIGNAL_Result_Ok;
}

LVQ_DAC_SIGNAL_Result_t LVQ_DAC_SIGNAL_SetSignal(LVQ_DAC_SIGNAL_Channel_t DACx, LVQ_DAC_SIGNAL_Signal_t signal_type, double frequency) 
{
    LVQ_DAC_SIGNAL_Result_t result;
    switch (signal_type) 
		{
        case LVQ_DAC_SIGNAL_Signal_Sinus:
            result = LVQ_DAC_SIGNAL_SetCustomSignal(DACx, DAC_SIGNAL_Sinus, DAC_SIGNAL_SINUS_LENGTH, frequency);
            break;
        case LVQ_DAC_SIGNAL_Signal_Sawtooth:
            result = LVQ_DAC_SIGNAL_SetCustomSignal(DACx, DAC_SIGNAL_Sawtooth, DAC_SIGNAL_SAWTOOTH_LENGTH, frequency);
            break;
        case LVQ_DAC_SIGNAL_Signal_Triangle:
            result = LVQ_DAC_SIGNAL_SetCustomSignal(DACx, DAC_SIGNAL_Triangle, DAC_SIGNAL_TRIANGLE_LENGTH, frequency);
            break;
        case LVQ_DAC_SIGNAL_Signal_Square:
            result = LVQ_DAC_SIGNAL_SetCustomSignal(DACx, DAC_SIGNAL_Square, DAC_SIGNAL_SQUARE_LENGTH, frequency);
            break;
        default:
            result = LVQ_DAC_SIGNAL_Result_Error;
    }

    /* Return result */
    return result;
}

LVQ_DAC_SIGNAL_Result_t LVQ_DAC_SIGNAL_SetCustomSignal(LVQ_DAC_SIGNAL_Channel_t DACx, uint16_t* Signal_Data, uint16_t Signal_Length, double frequency) 
{
    DAC_InitTypeDef DAC_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    DMA_InitTypeDef DMA_InitStruct;
    LVQ_TIMER_PROPERTIES_t Timer_Data;

    /* Check if timer is set */
    if (!dac_timer_set[DACx]) 
		{
        return LVQ_DAC_SIGNAL_Result_Error;
    }

    /* Check used timer */
    /* Set proper trigger */
    if (DAC_TIM[DACx] == TIM2) {
        DAC_InitStruct.DAC_Trigger = DAC_Trigger_T2_TRGO;
    } else if (DAC_TIM[DACx] == TIM4) {
        DAC_InitStruct.DAC_Trigger = DAC_Trigger_T4_TRGO;
    } else if (DAC_TIM[DACx] == TIM5) {
        DAC_InitStruct.DAC_Trigger = DAC_Trigger_T5_TRGO;
    } else if (DAC_TIM[DACx] == TIM6) {
        DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
    } else if (DAC_TIM[DACx] == TIM7) {
        DAC_InitStruct.DAC_Trigger = DAC_Trigger_T7_TRGO;
    } else if (DAC_TIM[DACx] == TIM8) {
        DAC_InitStruct.DAC_Trigger = DAC_Trigger_T8_TRGO;
    } else {
        /* Timer is not valid */
        return LVQ_DAC_SIGNAL_Result_TimerNotValid;
    }

    /* Get timer data */
    LVQ_TIMER_GetTimerProperties(DAC_TIM[DACx], &Timer_Data);

    /* Get period and prescaler values */
    LVQ_TIMER_CreateDataForPWMFrequency(&Timer_Data, frequency * Signal_Length);

    /* Check valid frequency */
    if (Timer_Data.Frequency == 0) 
		{
        return LVQ_DAC_SIGNAL_Result_Error;
    }

    /* Enable DAC clock */
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    /* Enable DMA1 clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    /* Initialize DAC */
    DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

    /* Disable DMA */
    if (DACx == LVQ_SIGNAL_DAC1) 
		{
        /* Init DAC channel 1 */
        DAC_Init(DAC_Channel_1, &DAC_InitStruct);
    } 
		else if (DACx == LVQ_SIGNAL_DAC2) 
		{
        /* Init DAC channel 2 */
        DAC_Init(DAC_Channel_2, &DAC_InitStruct);
    }

    /* Enable timer clock */
    LVQ_TIMER_EnableClock(DAC_TIM[DACx]);

    /* Time base configuration */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
    TIM_TimeBaseStruct.TIM_Period = Timer_Data.Period - 1;
    TIM_TimeBaseStruct.TIM_Prescaler = Timer_Data.Prescaler - 1;
    TIM_TimeBaseStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

    /* Initialize timer */
    TIM_TimeBaseInit(DAC_TIM[DACx], &TIM_TimeBaseStruct);

    /* Enable TIM selection */
    TIM_SelectOutputTrigger(DAC_TIM[DACx], TIM_TRGOSource_Update);

    /* Set DMA options */
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)Signal_Data;
    DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct.DMA_BufferSize = Signal_Length;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    switch (DACx) 
		{
        case LVQ_SIGNAL_DAC1:
            /* Set peripheral location = 12bit right aligned for channel 1 */
            DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R1;

            /* Disable DMA */
            DMA_DeInit(DAC_SIGNAL_DMA_DAC1_STREAM);

            /* Set channel used */
            DMA_InitStruct.DMA_Channel = DAC_SIGNAL_DMA_DAC1_CHANNEL;

            /* Initialize DMA */
            DMA_Init(DAC_SIGNAL_DMA_DAC1_STREAM, &DMA_InitStruct);

            /* Enable DMA Stream for DAC Channel 1 */
            DMA_Cmd(DAC_SIGNAL_DMA_DAC1_STREAM, ENABLE);

            /* Enable DAC Channel 1 */
            DAC_Cmd(DAC_Channel_1, ENABLE);

            /* Enable DMA for DAC Channel 1 */
            DAC_DMACmd(DAC_Channel_1, ENABLE);
            break;
        case LVQ_SIGNAL_DAC2:
            /* Disable DMA */
            DMA_DeInit(DAC_SIGNAL_DMA_DAC2_STREAM);

            /* Set channel used */
            DMA_InitStruct.DMA_Channel = DAC_SIGNAL_DMA_DAC2_CHANNEL;

            /* Set peripheral location = 12bit right aligned for channel 2 */
            DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R2;

            /* Initialize DMA */
            DMA_Init(DAC_SIGNAL_DMA_DAC2_STREAM, &DMA_InitStruct);

            /* Enable DMA Stream for DAC Channel 2 */
            DMA_Cmd(DAC_SIGNAL_DMA_DAC2_STREAM, ENABLE);

            /* Enable DAC Channel 2 */
            DAC_Cmd(DAC_Channel_2, ENABLE);

            /* Enable DMA for DAC Channel 2 */
            DAC_DMACmd(DAC_Channel_2, ENABLE);
            break;
        default:
            break;
    }

    /* Enable timer */
    DAC_TIM[DACx]->CR1 |= TIM_CR1_CEN;

    /* Return OK */
    return LVQ_DAC_SIGNAL_Result_Ok;
}
