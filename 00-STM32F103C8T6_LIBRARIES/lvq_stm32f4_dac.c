#include "lvq_stm32f4_dac.h"

void LVQ_DAC_Init(LVQ_DAC_Channel_t DACx) 
{
    DAC_InitTypeDef DAC_InitStruct;
    uint16_t GPIO_Pin;

    /* Select proper GPIO pin */
    if (DACx == LVQ_DAC1) 
		{
        GPIO_Pin = GPIO_PIN_4;
    } 
		else 
	  {
        GPIO_Pin = GPIO_PIN_5;
    }

    /* Initialize proper GPIO pin */
    LVQ_GPIO_Init(GPIOA, GPIO_Pin, LVQ_GPIO_Mode_AN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_Fast);

    /* Enable DAC clock */
		RCC_APB1PeriphClockCmd(RCC_APB1ENR_DACEN, ENABLE);

    /* Set DAC options */
    DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

    /* Init and enable proper DAC */
    if (DACx == LVQ_DAC1) {
        DAC_Init(DAC_Channel_1, &DAC_InitStruct);
        /* Enable DAC channel 1 */
        DAC->CR |= DAC_CR_EN1;
    } else {
        DAC_Init(DAC_Channel_2, &DAC_InitStruct);
        /* Enable DAC channel 2 */
        DAC->CR |= DAC_CR_EN2;
    }
}

void LVQ_DAC_SetValue(LVQ_DAC_Channel_t DACx, uint16_t value) 
{
    /* Check value */
    if (value > 4095) 
		{
        value = 4095;
    }

    /* Set 12-bit value, right aligned */
    if (DACx == LVQ_DAC1) 
		{
        DAC->DHR12R1 = value;
    } 
		else 
		{
        DAC->DHR12R2 = value;
    }
}
