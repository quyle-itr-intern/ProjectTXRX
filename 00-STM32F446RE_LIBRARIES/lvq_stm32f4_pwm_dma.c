/**
  ******************************************************************************

  DMA PWM For SLVQ32F446RE
  Author:   LVQ
  Updated:  24st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_pwm_dma.h"

/* Private DMA structure */
static DMA_InitTypeDef DMA_InitStruct;
LVQ_PWM_TIM_t TIM4_Data;


void LVQ_PWM_DMA_Init(void)
{
	LVQ_PWM_InitTimer(TIM4, &TIM4_Data, 800000);
	LVQ_PWM_InitChannel(&TIM4_Data, LVQ_PWM_Channel_1, LVQ_PWM_PinsPack_1);
	
	LVQ_PWM_SetDutyPercent(&TIM4_Data, LVQ_PWM_Channel_1, 0);
	
	printf("%d\r\n", TIM4_Data.Period);
	/* Enable DMA1 clock */
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	
	/* Clear flags */
  LVQ_DMA_ClearFlags(DMA1_Stream0);
	
	/* Set DMA options */
    DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
	LVQ_DMA_EnableInterrupts(DMA1_Stream0);
	TIM4->DIER |= (1<<9);
}

uint8_t LVQ_PWM_DMA_SetDuty(uint16_t* DataArray)
{
	  /* Check if DMA is working now */
    if (DMA1_Stream0->NDTR) 
		{
        /* DMA works right now */
        return 0;
    }
		/* Set DMA options */
    DMA_InitStruct.DMA_Channel = DMA_Channel_2;
    DMA_InitStruct.DMA_BufferSize = MAX_LED*24 + 50;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &TIM4->CCR1;
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t) &DataArray[0];
	
		/* Deinit first, clear all flags */
    LVQ_DMA_ClearFlags(DMA1_Stream0);

    /* Init DMA */
    DMA_Init(DMA1_Stream0, &DMA_InitStruct);
		
		/* Enable DMA */
		DMA_Cmd(DMA1_Stream0, ENABLE);
		
		TIM_Cmd(TIM4, ENABLE);
		
		return 1;
}


















