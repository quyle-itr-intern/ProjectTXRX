/**
  ******************************************************************************

  ADC DMA For STM32F446RE
  Author:   LVQ
  Updated:  16st August 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_adc_dma.h"

void LVQ_ADC_DMA_ConfigChannel(ADC_TypeDef* ADCx, uint16_t GPIO_Pin);

void LVQ_ADC_DMA_Init(ADC_TypeDef* ADCx, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t u32Number, uint32_t u32StartAddress, uint32_t u32Destination)
{
		LVQ_ADC_INT_InitPin(GPIOx, GPIO_Pin);
	
		/* Init ADC settings */

    /* Enable clock and fill resolution settings */
    if (ADCx == ADC1) {
        /* Enable ADC clock */
				RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

        /* Set resolution */
				/* Bits 25:24 RES[1:0]: Resolution */
				ADCx->CR1 |= LVQ_ADC1_RESOLUTION;
    } 
		else if (ADCx == ADC2) {
        /* Enable ADC clock */
				RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;

        /* Set resolution */
				/* Bits 25:24 RES[1:0]: Resolution */
				ADCx->CR1 |= LVQ_ADC2_RESOLUTION;
    } 
		else if (ADCx == ADC3) {
        /* Enable ADC clock */
				RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;

        /* Set resolution */
				/* Bits 25:24 RES[1:0]: Resolution */
				ADCx->CR1 |= LVQ_ADC3_RESOLUTION;
    }
		/* Set common ADC settings */
		/* Clear MULTI, DELAY, DMA and ADCPRE bits */
		ADC->CCR |= 0xFFFC30E0;
		
		/* Bits 15:14 DMA: Direct memory access mode for multi ADC mode */
		ADC->CCR |= ADC_DMAAccessMode_Disabled;
		/* Bits 4:0 MULTI[4:0]: Multi ADC mode selection */
		ADC->CCR |= ADC_Mode_Independent;
		/* Bits 17:16 ADCPRE: ADC prescaler */
		ADC->CCR |= ADC_Prescaler_Div4;
		/* Bits 11:8 DELAY: Delay between 2 sampling phases */
		ADC->CCR |= ADC_TwoSamplingDelay_8Cycles;
		
		/* Clear RES and SCAN bits */
		ADCx->CR1 &=0xFCFFFEFF;
		/* Bit 8 SCAN: Scan mode */
		ADCx->CR1 |= (ENABLE << 8);
		
		/* Clear CONT, ALIGN, EXTEN and EXTSEL bits */
		ADCx->CR2 &= 0xC0FFF7FD;
		/* Bit 1 CONT: Continuous conversion */
		ADCx->CR2 |= (ENABLE << 1);
		/* Bit 11 ALIGN */
		ADCx->CR2 |= ADC_DataAlign_Right;
		/* Bits 29:28 EXTEN: External trigger enable for regular channels */
		ADCx->CR2 |= ADC_ExternalTrigConvEdge_None;
		/* Bits 27:24 EXTSEL[3:0]: External event select for regular group */
		ADCx->CR2 |= DISABLE;
		/* Bit 8 DMA: Direct memory access mode (for single ADC mode) */
		ADCx->CR2 |= (ENABLE << 8);	
		/* Bit 10 EOCS: End of conversion selection */
		ADCx->CR2 |= (1<<10);
		/* Bit 9 DDS: DMA disable selection (for single ADC mode) */
		ADCx->CR2 |= (1<<9);
	
		/* Bits 23:20 L[3:0]: Regular channel sequence length */
		ADCx->SQR1 |= (u32Number - 1) << 20;
		
		/* Initialize ADC */	
		LVQ_ADC_DMA_ConfigChannel(ADCx, GPIO_Pin);

    /* Enable ADC */
    ADCx->CR2 |= (uint32_t)ADC_CR2_ADON;
		
		/* Start software conversion */
    ADCx->CR2 |= (uint32_t)ADC_CR2_SWSTART;
		
		/* Enable clock DMA */
		RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
		
		DMA2_Stream0->M0AR = (uint32_t) u32Destination;
		
		DMA2_Stream0->PAR = (uint32_t) u32StartAddress;
		
		DMA2_Stream0->NDTR = u32Number;
		
		/* Bits 27:25 CHSEL[2:0]: channel selection */
		DMA2_Stream0->CR |= (0 << 25);
		
		/* Bits 7:6 DIR[1:0]: data transfer direction */
		DMA2_Stream0->CR |= DMA_DIR_PeripheralToMemory;
		/* Bit 9 PINC: peripheral increment mode */
		DMA2_Stream0->CR |= DMA_PeripheralInc_Disable;
		/* Bit 10 MINC: memory increment mode */
		DMA2_Stream0->CR |= DMA_MemoryInc_Enable;
		/* Bits 12:11 PSIZE[1:0]: peripheral data size */
		DMA2_Stream0->CR |= DMA_PeripheralDataSize_HalfWord;
		/* Bits 14:13 MSIZE[1:0]: memory data size */
		DMA2_Stream0->CR |= DMA_MemoryDataSize_HalfWord;
		/* Bit 8 CIRC: circular mode */
		DMA2_Stream0->CR |= DMA_Mode_Circular;
		/* Bits 17:16 PL[1:0]: priority level */ 
		DMA2_Stream0->CR |= DMA_Priority_High;
		/* Bit 0 EN: stream enable / flag stream ready when read low */
		DMA2_Stream0->CR |= DMA_SxCR_EN;
}

void LVQ_ADC_DMA_ConfigChannel(ADC_TypeDef* ADCx, uint16_t GPIO_Pin)
{
		uint8_t pinpos, i = 1;
		/* Go through all pins */
		for (pinpos = 0; pinpos < 0x13; pinpos++)    /* 011 & 001 = 1, 011 & 010 = 2, 011 & 100 = 0*/  
		{
				/* Check if pin available */
				if ((GPIO_Pin & (1 << pinpos)) == 0) {
							continue;
				}
				ADC_RegularChannelConfig(ADCx, pinpos, i++, ADC_SampleTime_480Cycles);
		}
}

/*

switch(u32Number)
		{
			case 1:
				ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);// PA0
				break;
			case 2:
				ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);// PA0
				ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 2, ADC_SampleTime_15Cycles);// PA1
				break;
			case 3:
				ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);// PA0
				ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 2, ADC_SampleTime_15Cycles);// PA1
				ADC_RegularChannelConfig(ADCx, ADC_Channel_2, 3, ADC_SampleTime_15Cycles);// PA2
				break;
			case 4:
				ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);// PA0
				ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 2, ADC_SampleTime_15Cycles);// PA1
				ADC_RegularChannelConfig(ADCx, ADC_Channel_2, 3, ADC_SampleTime_15Cycles);// PA2
				ADC_RegularChannelConfig(ADCx, ADC_Channel_3, 4, ADC_SampleTime_15Cycles);// PA3
				break;
			case 5:
				ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);// PA0
				ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 2, ADC_SampleTime_15Cycles);// PA1
				ADC_RegularChannelConfig(ADCx, ADC_Channel_2, 3, ADC_SampleTime_15Cycles);// PA2
				ADC_RegularChannelConfig(ADCx, ADC_Channel_3, 4, ADC_SampleTime_15Cycles);// PA3
				ADC_RegularChannelConfig(ADCx, ADC_Channel_4, 5, ADC_SampleTime_15Cycles);// PA4
				break;
			case 6:
				ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);// PA0
				ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 2, ADC_SampleTime_15Cycles);// PA1
				ADC_RegularChannelConfig(ADCx, ADC_Channel_2, 3, ADC_SampleTime_15Cycles);// PA2
				ADC_RegularChannelConfig(ADCx, ADC_Channel_3, 4, ADC_SampleTime_15Cycles);// PA3
				ADC_RegularChannelConfig(ADCx, ADC_Channel_4, 5, ADC_SampleTime_15Cycles);// PA4
				ADC_RegularChannelConfig(ADCx, ADC_Channel_5, 6, ADC_SampleTime_15Cycles);// PA5
				break;
			case 7:
				ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);// PA0
				ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 2, ADC_SampleTime_15Cycles);// PA1
				ADC_RegularChannelConfig(ADCx, ADC_Channel_2, 3, ADC_SampleTime_15Cycles);// PA2
				ADC_RegularChannelConfig(ADCx, ADC_Channel_3, 4, ADC_SampleTime_15Cycles);// PA3
				ADC_RegularChannelConfig(ADCx, ADC_Channel_4, 5, ADC_SampleTime_15Cycles);// PA4
				ADC_RegularChannelConfig(ADCx, ADC_Channel_5, 6, ADC_SampleTime_15Cycles);// PA5
				ADC_RegularChannelConfig(ADCx, ADC_Channel_6, 7, ADC_SampleTime_15Cycles);// PA6
				break;
			case 8:
				ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);// PA0
				ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 2, ADC_SampleTime_15Cycles);// PA1
				ADC_RegularChannelConfig(ADCx, ADC_Channel_2, 3, ADC_SampleTime_15Cycles);// PA2
				ADC_RegularChannelConfig(ADCx, ADC_Channel_3, 4, ADC_SampleTime_15Cycles);// PA3
				ADC_RegularChannelConfig(ADCx, ADC_Channel_4, 5, ADC_SampleTime_15Cycles);// PA4
				ADC_RegularChannelConfig(ADCx, ADC_Channel_5, 6, ADC_SampleTime_15Cycles);// PA5
				ADC_RegularChannelConfig(ADCx, ADC_Channel_6, 7, ADC_SampleTime_15Cycles);// PA6
				ADC_RegularChannelConfig(ADCx, ADC_Channel_7, 8, ADC_SampleTime_15Cycles);// PA7
				break;
		}
*/
