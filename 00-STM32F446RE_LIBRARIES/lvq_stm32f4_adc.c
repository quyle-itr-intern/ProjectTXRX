#include "lvq_stm32f4_adc.h"

/* Private functions */
void LVQ_ADC_INT_Channel_0_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_1_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_2_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_3_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_4_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_5_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_6_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_7_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_8_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_9_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_10_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_11_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_12_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_13_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_14_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_Channel_15_Init(ADC_TypeDef* ADCx);
void LVQ_ADC_INT_InitPin(GPIO_TypeDef* GPIOx, uint16_t PinX);

void LVQ_ADC_Init(ADC_TypeDef* ADCx, uint8_t channel)
{
    LVQ_ADC_Channel_t ch = (LVQ_ADC_Channel_t) channel;
    if (ch == LVQ_ADC_Channel_0) 
		{
        LVQ_ADC_INT_Channel_0_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_1) 
		{
        LVQ_ADC_INT_Channel_1_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_2) 
		{
        LVQ_ADC_INT_Channel_2_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_3) 
		{
        LVQ_ADC_INT_Channel_3_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_4) 
		{
        LVQ_ADC_INT_Channel_4_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_5) 
		{
        LVQ_ADC_INT_Channel_5_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_6) 
		{
        LVQ_ADC_INT_Channel_6_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_7) 
		{
        LVQ_ADC_INT_Channel_7_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_8) 
		{
        LVQ_ADC_INT_Channel_8_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_9) 
		{
        LVQ_ADC_INT_Channel_9_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_10) 
		{
        LVQ_ADC_INT_Channel_10_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_11) 
		{
        LVQ_ADC_INT_Channel_11_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_12) 
		{
        LVQ_ADC_INT_Channel_12_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_13) 
		{
        LVQ_ADC_INT_Channel_13_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_14) 
		{
        LVQ_ADC_INT_Channel_14_Init(ADCx);
    } 
		else if (ch == LVQ_ADC_Channel_15) 
		{
        LVQ_ADC_INT_Channel_15_Init(ADCx);
    }
    /* Init ADC */
    LVQ_ADC_InitADC(ADCx);
}

void LVQ_ADC_InitADC(ADC_TypeDef* ADCx) 
{
    /* Init ADC settings */
	
		/* Clear CONT, ALIGN, EXTEN and EXTSEL bits */
		ADCx->CR2 &= 0xC0FFF7FD;
		/* Bit 1 CONT: Continuous conversion */
		ADCx->CR2 |= (DISABLE << 1);
		/* Bit 11 ALIGN */
		ADCx->CR2 |= ADC_DataAlign_Right;
		/* Bits 29:28 EXTEN: External trigger enable for regular channels */
		ADCx->CR2 |= ADC_ExternalTrigConvEdge_None;
		/* Bits 27:24 EXTSEL[3:0]: External event select for regular group */
		ADCx->CR2 |= DISABLE;
		
		/* Clear RES and SCAN bits */
		ADCx->CR1 &=0xFCFFFEFF;
		/* Bit 8 SCAN: Scan mode */
		ADCx->CR1 |= (DISABLE << 8);
	
		/* Bits 23:20 L[3:0]: Regular channel sequence length */
		ADCx->SQR1 |= (1 - 1) << 20;

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

    /* Initialize ADC */

    /* Enable ADC */
    ADCx->CR2 |= (uint32_t)ADC_CR2_ADON;
}

uint16_t LVQ_ADC_Read(ADC_TypeDef* ADCx, uint8_t channel) 
{
    uint32_t timeout = 0xFFF;

    ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SampleTime_15Cycles);

    /* Start software conversion */
    ADCx->CR2 |= (uint32_t)ADC_CR2_SWSTART;
	
		/* Wait till done */
    while (!(ADCx->SR & ADC_FLAG_EOC)) 
		{
        if (timeout-- == 0x00) {
            return 0;
        }
    }

    /* Return result */
    return ADCx->DR;
}

void LVQ_ADC_EnableVbat(void) 
{
    /* Enable VBAT */
    ADC->CCR |= ADC_CCR_VBATE;
}

void LVQ_ADC_DisableVbat(void) 
{
    /* Disable VBAT */
    ADC->CCR &= ~ADC_CCR_VBATE;
}

uint16_t LVQ_ADC_ReadVbat(ADC_TypeDef* ADCx)
{
    uint32_t result;
    /* Read battery voltage data */
    /* Start conversion */
    ADC_RegularChannelConfig(ADCx, ADC_Channel_Vbat, 1, ADC_SampleTime_112Cycles);

    /* Start software conversion */
    ADCx->CR2 |= (uint32_t)ADC_CR2_SWSTART;

    /* Wait till done */
    while ((ADCx->SR & ADC_FLAG_EOC) == RESET);

    /* Get result */
    result = ADCx->DR;

    /* Convert to voltage */
    result = result * ADC_VBAT_MULTI * ADC_SUPPLY_VOLTAGE / 0xFFF;

    /* Return value in mV */
    return (uint16_t) result;
}

/* Private functions */
void LVQ_ADC_INT_Channel_0_Init(ADC_TypeDef* ADCx) 
{
    LVQ_ADC_INT_InitPin(GPIOA, GPIO_PIN_0);
}
void LVQ_ADC_INT_Channel_1_Init(ADC_TypeDef* ADCx) 
{
    LVQ_ADC_INT_InitPin(GPIOA, GPIO_PIN_1);
}
void LVQ_ADC_INT_Channel_2_Init(ADC_TypeDef* ADCx) 
{
    LVQ_ADC_INT_InitPin(GPIOA, GPIO_PIN_2);
}
void LVQ_ADC_INT_Channel_3_Init(ADC_TypeDef* ADCx) 
{
    LVQ_ADC_INT_InitPin(GPIOA, GPIO_PIN_3);
}
void LVQ_ADC_INT_Channel_4_Init(ADC_TypeDef* ADCx) 
{
    if (ADCx == ADC1 || ADCx == ADC2) 
		{
        LVQ_ADC_INT_InitPin(GPIOA, GPIO_PIN_4);
    }
    if (ADCx == ADC3) 
		{
#if defined(GPIOF)
        LVQ_ADC_INT_InitPin(GPIOF, GPIO_PIN_6);
#endif
    }
}
void LVQ_ADC_INT_Channel_5_Init(ADC_TypeDef* ADCx) 
{
    if (ADCx == ADC1 || ADCx == ADC2)			
		{
        LVQ_ADC_INT_InitPin(GPIOA, GPIO_PIN_5);
    }
    if (ADCx == ADC3) 
		{
#if defined(GPIOF)
        LVQ_ADC_INT_InitPin(GPIOF, GPIO_PIN_7);
#endif
    }
}
void LVQ_ADC_INT_Channel_6_Init(ADC_TypeDef* ADCx) 
{
    if (ADCx == ADC1 || ADCx == ADC2) 
		{
        LVQ_ADC_INT_InitPin(GPIOA, GPIO_PIN_6);
    }
    if (ADCx == ADC3) 
		{
#if defined(GPIOF)
        LVQ_ADC_INT_InitPin(GPIOF, GPIO_PIN_8);
#endif
    }
}
void LVQ_ADC_INT_Channel_7_Init(ADC_TypeDef* ADCx) 
{
    if (ADCx == ADC1 || ADCx == ADC2) 
		{
        LVQ_ADC_INT_InitPin(GPIOA, GPIO_PIN_7);
    }
    if (ADCx == ADC3) 
		{
#if defined(GPIOF)
        LVQ_ADC_INT_InitPin(GPIOF, GPIO_PIN_9);
#endif
    }
}
void LVQ_ADC_INT_Channel_8_Init(ADC_TypeDef* ADCx) 
{
    if (ADCx == ADC1 || ADCx == ADC2) 
		{
        LVQ_ADC_INT_InitPin(GPIOB, GPIO_PIN_0);
    }
    if (ADCx == ADC3) 
		{
#if defined(GPIOF)
        LVQ_ADC_INT_InitPin(GPIOF, GPIO_PIN_10);
#endif
    }
}
void LVQ_ADC_INT_Channel_9_Init(ADC_TypeDef* ADCx) 
{
    if (ADCx == ADC1 || ADCx == ADC2) 
		{
        LVQ_ADC_INT_InitPin(GPIOB, GPIO_PIN_1);
    }
    if (ADCx == ADC3) 
		{
#if defined(GPIOF)
        LVQ_ADC_INT_InitPin(GPIOF, GPIO_PIN_3);
#endif
    }
}
void LVQ_ADC_INT_Channel_10_Init(ADC_TypeDef* ADCx) 
{
    LVQ_ADC_INT_InitPin(GPIOC, GPIO_PIN_0);
}
void LVQ_ADC_INT_Channel_11_Init(ADC_TypeDef* ADCx) 
{
    LVQ_ADC_INT_InitPin(GPIOC, GPIO_PIN_1);
}
void LVQ_ADC_INT_Channel_12_Init(ADC_TypeDef* ADCx) 
{
    LVQ_ADC_INT_InitPin(GPIOC, GPIO_PIN_2);
}
void LVQ_ADC_INT_Channel_13_Init(ADC_TypeDef* ADCx) 
{
    LVQ_ADC_INT_InitPin(GPIOC, GPIO_PIN_3);
}
void LVQ_ADC_INT_Channel_14_Init(ADC_TypeDef* ADCx) 
{
    if (ADCx == ADC1 || ADCx == ADC2) 
		{
        LVQ_ADC_INT_InitPin(GPIOC, GPIO_PIN_4);
    }
    if (ADCx == ADC3) 
		{
#if defined(GPIOF)
        LVQ_ADC_INT_InitPin(GPIOF, GPIO_PIN_4);
#endif
    }
}
void LVQ_ADC_INT_Channel_15_Init(ADC_TypeDef* ADCx) 
{
    if (ADCx == ADC1 || ADCx == ADC2) 
		{
        LVQ_ADC_INT_InitPin(GPIOC, GPIO_PIN_5);
    } 
		if (ADCx == ADC3) 
		{
#if defined(GPIOF)
        LVQ_ADC_INT_InitPin(GPIOF, GPIO_PIN_5);
#endif
    }
}

void LVQ_ADC_INT_InitPin(GPIO_TypeDef* GPIOx, uint16_t PinX) 
{
    /* Enable GPIO */
    LVQ_GPIO_Init(GPIOx, PinX, LVQ_GPIO_Mode_AN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_DOWN, LVQ_GPIO_Speed_Medium);
}
