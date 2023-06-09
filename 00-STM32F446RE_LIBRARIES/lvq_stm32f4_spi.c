#include "lvq_stm32f4_spi.h"

/* Private functions */
static void LVQ_SPIx_Init(SPI_TypeDef* SPIx, LVQ_SPI_PinsPack_t pinspack, LVQ_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit);
void LVQ_SPI1_INT_InitPins(LVQ_SPI_PinsPack_t pinspack);
void LVQ_SPI2_INT_InitPins(LVQ_SPI_PinsPack_t pinspack);
void LVQ_SPI3_INT_InitPins(LVQ_SPI_PinsPack_t pinspack);
void LVQ_SPI4_INT_InitPins(LVQ_SPI_PinsPack_t pinspack);
void LVQ_SPI5_INT_InitPins(LVQ_SPI_PinsPack_t pinspack);
void LVQ_SPI6_INT_InitPins(LVQ_SPI_PinsPack_t pinspack);

void LVQ_SPI_Init(SPI_TypeDef* SPIx, LVQ_SPI_PinsPack_t pinspack) 
{
    /* Init with default settings */
#ifdef USE_SPI1
    if (SPIx == SPI1) {
        LVQ_SPIx_Init(SPI1, pinspack, LVQ_SPI1_MODE, LVQ_SPI1_PRESCALER, LVQ_SPI1_MASTERSLAVE, LVQ_SPI1_FIRSTBIT);
    }
#endif
#ifdef USE_SPI2
    if (SPIx == SPI2) {
        LVQ_SPIx_Init(SPI2, pinspack, LVQ_SPI2_MODE, LVQ_SPI2_PRESCALER, LVQ_SPI2_MASTERSLAVE, LVQ_SPI2_FIRSTBIT);
    }
#endif
#ifdef USE_SPI3
    if (SPIx == SPI3) {
        LVQ_SPIx_Init(SPI3, pinspack, LVQ_SPI3_MODE, LVQ_SPI3_PRESCALER, LVQ_SPI3_MASTERSLAVE, LVQ_SPI3_FIRSTBIT);
    }
#endif
#ifdef USE_SPI4
    if (SPIx == SPI4) {
        LVQ_SPIx_Init(SPI4, pinspack, LVQ_SPI4_MODE, LVQ_SPI4_PRESCALER, LVQ_SPI4_MASTERSLAVE, LVQ_SPI4_FIRSTBIT);
    }
#endif
#ifdef USE_SPI5
    if (SPIx == SPI5) {
        LVQ_SPIx_Init(SPI5, pinspack, LVQ_SPI5_MODE, LVQ_SPI5_PRESCALER, LVQ_SPI5_MASTERSLAVE, LVQ_SPI5_FIRSTBIT);
    }
#endif
#ifdef USE_SPI6
    if (SPIx == SPI6) {
        LVQ_SPIx_Init(SPI6, pinspack, LVQ_SPI6_MODE, LVQ_SPI6_PRESCALER, LVQ_SPI6_MASTERSLAVE, LVQ_SPI6_FIRSTBIT);
    }
#endif
}

void LVQ_SPI_InitWithMode(SPI_TypeDef* SPIx, LVQ_SPI_PinsPack_t pinspack, LVQ_SPI_Mode_t SPI_Mode) 
{
    /* Init with custom mode, 0, 1, 2, 3 */
#ifdef USE_SPI1
    if (SPIx == SPI1) {
        LVQ_SPIx_Init(SPI1, pinspack, SPI_Mode, LVQ_SPI1_PRESCALER, LVQ_SPI1_MASTERSLAVE, LVQ_SPI1_FIRSTBIT);
    }
#endif
#ifdef USE_SPI2
    if (SPIx == SPI2) {
        LVQ_SPIx_Init(SPI2, pinspack, SPI_Mode, LVQ_SPI2_PRESCALER, LVQ_SPI2_MASTERSLAVE, LVQ_SPI2_FIRSTBIT);
    }
#endif
#ifdef USE_SPI3
    if (SPIx == SPI3) {
        LVQ_SPIx_Init(SPI3, pinspack, SPI_Mode, LVQ_SPI3_PRESCALER, LVQ_SPI3_MASTERSLAVE, LVQ_SPI3_FIRSTBIT);
    }
#endif
#ifdef USE_SPI4
    if (SPIx == SPI4) {
        LVQ_SPIx_Init(SPI4, pinspack, SPI_Mode, LVQ_SPI4_PRESCALER, LVQ_SPI4_MASTERSLAVE, LVQ_SPI4_FIRSTBIT);
    }
#endif
#ifdef USE_SPI5
    if (SPIx == SPI5) {
        LVQ_SPIx_Init(SPI5, pinspack, SPI_Mode, LVQ_SPI5_PRESCALER, LVQ_SPI5_MASTERSLAVE, LVQ_SPI5_FIRSTBIT);
    }
#endif
#ifdef USE_SPI6
    if (SPIx == SPI6) {
        LVQ_SPIx_Init(SPI6, pinspack, SPI_Mode, LVQ_SPI6_PRESCALER, LVQ_SPI6_MASTERSLAVE, LVQ_SPI6_FIRSTBIT);
    }
#endif
}

void LVQ_SPI_InitFull(SPI_TypeDef* SPIx, LVQ_SPI_PinsPack_t pinspack, uint16_t SPI_BaudRatePrescaler, LVQ_SPI_Mode_t SPI_Mode_t, uint16_t SPI_Mode, uint16_t SPI_FirstBit) 
{
    /* Init FULL SPI settings by user */
#ifdef USE_SPI1
    if (SPIx == SPI1) {
        LVQ_SPIx_Init(SPI1, pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
    }
#endif
#ifdef USE_SPI2
    if (SPIx == SPI2) {
        LVQ_SPIx_Init(SPI2, pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
    }
#endif
#ifdef USE_SPI3
    if (SPIx == SPI3) {
        LVQ_SPIx_Init(SPI3, pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
    }
#endif
#ifdef USE_SPI4
    if (SPIx == SPI4) {
        LVQ_SPIx_Init(SPI4, pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
    }
#endif
#ifdef USE_SPI5
    if (SPIx == SPI5) {
        LVQ_SPIx_Init(SPI5, pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
    }
#endif
#ifdef USE_SPI6
    if (SPIx == SPI6) {
        LVQ_SPIx_Init(SPI6, pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
    }
#endif
}

uint16_t LVQ_SPI_GetPrescalerFromMaxFrequency(SPI_TypeDef* SPIx, uint32_t MAX_SPI_Frequency)
{
    RCC_ClocksTypeDef RCC_Clocks;
    uint32_t APB_Frequency;
    uint8_t i;

    /* Prevent false input */
    if (MAX_SPI_Frequency == 0) {
        return SPI_BaudRatePrescaler_256;
    }

    /* Get clock values from RCC */
    RCC_GetClocksFreq(&RCC_Clocks);

    /* Calculate max SPI clock */
    if (0
#ifdef USE_SPI1
        || SPIx == SPI1
#endif
#ifdef USE_SPI4
        || SPIx == SPI4
#endif
#ifdef USE_SPI5
        || SPIx == SPI5
#endif
#ifdef USE_SPI6
        || SPIx == SPI6
#endif 
		) {
        APB_Frequency = RCC_Clocks.PCLK2_Frequency; /* SPI1,4,5,6 */
    } else {
        APB_Frequency = RCC_Clocks.PCLK1_Frequency; /* SPI2,3 */
    }

    /* Calculate prescaler value */
    /* Bits 5:3 in CR1 SPI registers are prescalers */
    /* 000 = 2, 001 = 4, 010 = 8, ..., 111 = 256 */
    for (i = 0; i < 8; i++) {
        if (APB_Frequency / (1 << (i + 1)) <= MAX_SPI_Frequency) {
            /* Bits for BP are 5:3 in CR1 register */
            return (i << 3);
        }
    }

    /* Use max prescaler possible */
    return SPI_BaudRatePrescaler_256;
}

LVQ_SPI_DataSize_t LVQ_SPI_SetDataSize(SPI_TypeDef* SPIx, LVQ_SPI_DataSize_t DataSize) 
{
    LVQ_SPI_DataSize_t status = (SPIx->CR1 & SPI_CR1_DFF) ? LVQ_SPI_DataSize_16b : LVQ_SPI_DataSize_8b;

    /* Disable SPI first */
    SPIx->CR1 &= ~SPI_CR1_SPE;

    /* Set proper value */
    if (DataSize == LVQ_SPI_DataSize_16b) {
        /* Set bit for frame */
        SPIx->CR1 |= SPI_CR1_DFF;
    } else {
        /* Clear bit for frame */
        SPIx->CR1 &= ~SPI_CR1_DFF;
    }

    /* Enable SPI back */
    SPIx->CR1 |= SPI_CR1_SPE;

    /* Return status */
    return status;
}

                     /*  SPIx Send Data And Read Data 8 bit   */
void LVQ_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint32_t count) 
{
    uint32_t i;

    /* Check if SPI is enabled */
    SPI_CHECK_ENABLED(SPIx);

    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);

    for (i = 0; i < count; i++) {
        /* Fill output buffer with data */
        SPIx->DR = dataOut[i];

        /* Wait for SPI to end everything */
        SPI_WAIT(SPIx);

        /* Read data register */
        dataIn[i] = SPIx->DR;
    }
}

void LVQ_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint32_t count) 
{
    uint32_t i;

    /* Check if SPI is enabled */
    SPI_CHECK_ENABLED(SPIx);

    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);

    for (i = 0; i < count; i++) {
        /* Fill output buffer with data */
        SPIx->DR = dataOut[i];

        /* Wait for SPI to end everything */
        SPI_WAIT(SPIx);

        /* Read data register */
        (void)SPIx->DR;
    }
}

void LVQ_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint32_t count) 
{
    uint32_t i;

    /* Check if SPI is enabled */
    SPI_CHECK_ENABLED(SPIx);

    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);

    for (i = 0; i < count; i++) {
        /* Fill output buffer with data */
        SPIx->DR = dummy;

        /* Wait for SPI to end everything */
        SPI_WAIT(SPIx);

        /* Save data to buffer */
        dataIn[i] = SPIx->DR;
    }
}

                     /*  SPIx Send Data And Read Data 16 bit   */
void LVQ_SPI_SendMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t* dataIn, uint32_t count) 
{
    uint32_t i;

    /* Check if SPI is enabled */
    SPI_CHECK_ENABLED(SPIx);

    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);

    for (i = 0; i < count; i++) {
        /* Fill output buffer with data */
        SPIx->DR = dataOut[i];

        /* Wait for SPI to end everything */
        SPI_WAIT(SPIx);

        /* Read data register */
        dataIn[i] = SPIx->DR;
    }
}

void LVQ_SPI_WriteMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint32_t count) 
{
    uint32_t i;

    /* Check if SPI is enabled */
    SPI_CHECK_ENABLED(SPIx);

    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);

    for (i = 0; i < count; i++) {
        /* Fill output buffer with data */
        SPIx->DR = dataOut[i];

        /* Wait for SPI to end everything */
        SPI_WAIT(SPIx);

        /* Read data register */
        (void)SPIx->DR;
    }
}

void LVQ_SPI_ReadMulti16(SPI_TypeDef* SPIx, uint16_t* dataIn, uint16_t dummy, uint32_t count) 
{
    uint32_t i;

    /* Check if SPI is enabled */
    SPI_CHECK_ENABLED(SPIx);

    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);

    for (i = 0; i < count; i++) {
        /* Fill output buffer with data */
        SPIx->DR = dummy;

        /* Wait for SPI to end everything */
        SPI_WAIT(SPIx);

        /* Save data to buffer */
        dataIn[i] = SPIx->DR;
    }
}

void LVQ_SPI_InitCustomPinsCallback(SPI_TypeDef* SPIx, uint16_t AlternateFunction) {
    /* Custom user function. */
    /* In case user needs functionality for custom pins, this function should be declared outside this library */
		/* Check for SPI1 */
		#if DEFINE_SPI1_CUSTOM_1
		if (SPIx == SPI1) {
			/* Init custom pins */
			/* MOSI = PA7, MISO = PA6 */
			LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_7 | GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, AlternateFunction);
			/* SCK = PB3 */
			LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_3, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, AlternateFunction);
		}
		#endif
		
		#if DEFINE_SPI1_CUSTOM_2
		if (SPIx == SPI1) {
			/* Init custom pins */
			/* MOSI = PB5, MISO = PB4 */
			LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_4 | GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, AlternateFunction);
			/* SCK = PA5 */
			LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, AlternateFunction);
		}
		#endif
		
}

/* Private functions */
static void LVQ_SPIx_Init(SPI_TypeDef* SPIx, LVQ_SPI_PinsPack_t pinspack, LVQ_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit) 
{
    SPI_InitTypeDef SPI_InitStruct;

    /* Set default settings */
    SPI_StructInit(&SPI_InitStruct);
#ifdef USE_SPI1
    if (SPIx == SPI1) {
        /* Enable SPI clock */
				RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

        /* Init pins */
        LVQ_SPI1_INT_InitPins(pinspack);

        /* Set options */
				SPIx->CR1 |= LVQ_SPI1_DATASIZE;
    }
#endif
#ifdef USE_SPI2
    if (SPIx == SPI2) {
        /* Enable SPI clock */
				RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

        /* Init pins */
        LVQ_SPI2_INT_InitPins(pinspack);

        /* Set options */
				SPIx->CR1 |= LVQ_SPI2_DATASIZE;
    }
#endif
#ifdef USE_SPI3
    if (SPIx == SPI3) {
        /* Enable SPI clock */
				RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;

        /* Init pins */
        LVQ_SPI3_INT_InitPins(pinspack);

        /* Set options */
        SPIx->CR1 |= LVQ_SPI3_DATASIZE;
    }
#endif
#ifdef USE_SPI4
    if (SPIx == SPI4) {
        /* Enable SPI clock */
				RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;

        /* Init pins */
        LVQ_SPI4_INT_InitPins(pinspack);

        /* Set options */
				SPIx->CR1 |= LVQ_SPI4_DATASIZE;
    }
#endif
#ifdef USE_SPI5
    if (SPIx == SPI5) {
        /* Enable SPI clock */
				RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;

        /* Init pins */
        LVQ_SPI5_INT_InitPins(pinspack);

        /* Set options */
				SPIx->CR1 |= LVQ_SPI5_DATASIZE;
    }
#endif
#ifdef USE_SPI6
    if (SPIx == SPI6) {
        /* Enable SPI clock */
				RCC->APB2ENR |= RCC_APB2ENR_SPI6EN;

        /* Init pins */
        LVQ_SPI6_INT_InitPins(pinspack);

        /* Set options */
				SPIx->CR1 |= LVQ_SPI6_DATASIZE;
    }
#endif

    /* Fill SPI settings */
		
		/* Bit 10 RXONLY */
		SPIx->CR1 |= SPI_Direction_2Lines_FullDuplex;
		/* Bit 7 LSBFIRST */
		SPIx->CR1 |= SPI_FirstBit;
		/* Bit 9 SSM */
		SPIx->CR1 |= SPI_NSS_Soft;
		/* Bit 2 MSTR */
		SPIx->CR1 |= SPI_MasterSlave;
		/* Bit 5:3 BR */
		SPIx->CR1 = (SPIx->CR1 & ~(7 << 3)) | SPI_BaudRatePrescaler;

    /* SPI mode */
    if (SPI_Mode == LVQ_SPI_Mode_0) 
		{
				SPIx->CR1 |= SPI_CPOL_Low;
				SPIx->CR1 |= SPI_CPHA_1Edge;
    } 
		else if (SPI_Mode == LVQ_SPI_Mode_1) 
		{
				SPIx->CR1 |= SPI_CPOL_Low;
				SPIx->CR1 |= SPI_CPHA_2Edge;
    } 
		else if (SPI_Mode == LVQ_SPI_Mode_2) 
		{
				SPIx->CR1 |= SPI_CPOL_High;
				SPIx->CR1 |= SPI_CPHA_1Edge;
    } 
		else if (SPI_Mode == LVQ_SPI_Mode_3) 
		{
				SPIx->CR1 |= SPI_CPOL_High;
				SPIx->CR1 |= SPI_CPHA_2Edge;
    }

    /* Disable first */
    SPIx->CR1 &= ~SPI_CR1_SPE;

    /* Init SPI */

    /* Enable SPI */
    SPIx->CR1 |= SPI_CR1_SPE;
}

/* Private functions */
#ifdef USE_SPI1
void LVQ_SPI1_INT_InitPins(LVQ_SPI_PinsPack_t pinspack) {
    /* Init SPI pins */
#if defined(GPIOA)
    if (pinspack == LVQ_SPI_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI1);
    }
#endif
#if defined(GPIOB)
    if (pinspack == LVQ_SPI_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI1);
    }
#endif
    if (pinspack == LVQ_SPI_PinsPack_Custom) {
        /* Call user function */
        LVQ_SPI_InitCustomPinsCallback(SPI1, GPIO_AF_SPI1);
    }
}
#endif

#ifdef USE_SPI2
void LVQ_SPI2_INT_InitPins(LVQ_SPI_PinsPack_t pinspack) {
    /* Init SPI pins */
#if defined(GPIOB) && defined(GPIOC)
    if (pinspack == LVQ_SPI_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI2);
        LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_2 | GPIO_PIN_3, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI2);
    }
#endif
#if defined(GPIOB)
    if (pinspack == LVQ_SPI_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI2);
    }
#endif
#if defined(GPIOI)
    if (pinspack == LVQ_SPI_PinsPack_3) {
        LVQ_GPIO_InitAlternate(GPIOI, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI2);
    }
#endif
    if (pinspack == LVQ_SPI_PinsPack_Custom) {
        /* Call user function */
        LVQ_SPI_InitCustomPinsCallback(SPI2, GPIO_AF_SPI2);
    }
}
#endif

#ifdef USE_SPI3
void LVQ_SPI3_INT_InitPins(LVQ_SPI_PinsPack_t pinspack) {
    /* Enable SPI pins */
#if defined(GPIOB)
    if (pinspack == LVQ_SPI_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI3);
    }
#endif
#if defined(GPIOC)
    if (pinspack == LVQ_SPI_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI3);
    }
#endif
    if (pinspack == LVQ_SPI_PinsPack_Custom) {
        /* Call user function */
        LVQ_SPI_InitCustomPinsCallback(SPI3, GPIO_AF_SPI3);
    }
}
#endif

#ifdef USE_SPI4
void LVQ_SPI4_INT_InitPins(LVQ_SPI_PinsPack_t pinspack) {
    /* Init SPI pins */
#if defined(GPIOE)
    if (pinspack == LVQ_SPI_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI4);
    }
#endif
#if defined(GPIOE)
    if (pinspack == LVQ_SPI_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOE, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI4);
    }
#endif
    if (pinspack == LVQ_SPI_PinsPack_Custom) {
        /* Call user function */
        LVQ_SPI_InitCustomPinsCallback(SPI4, GPIO_AF_SPI4);
    }
}
#endif

#ifdef USE_SPI5
void LVQ_SPI5_INT_InitPins(LVQ_SPI_PinsPack_t pinspack) {
    /* Init SPI pins */
#if defined(GPIOF)
    if (pinspack == LVQ_SPI_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI5);
    }
#endif
#if defined(GPIOF) && defined(GPIOH)
    if (pinspack == LVQ_SPI_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_11, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI5);
        LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_6 | GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI5);
    }
#endif
    if (pinspack == LVQ_SPI_PinsPack_Custom) {
        /* Call user function */
        LVQ_SPI_InitCustomPinsCallback(SPI5, GPIO_AF_SPI5);
    }
}
#endif

#ifdef USE_SPI6
void LVQ_SPI6_INT_InitPins(LVQ_SPI_PinsPack_t pinspack) {
#if defined(GPIOG)
    if (pinspack == LVQ_SPI_PinsPack_1) {
        /* Init SPI pins */
        LVQ_GPIO_InitAlternate(GPIOG, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High, GPIO_AF_SPI6);
    }
#endif
    if (pinspack == LVQ_SPI_PinsPack_Custom) {
        /* Call user function */
        LVQ_SPI_InitCustomPinsCallback(SPI6, GPIO_AF_SPI6);
    }
}
#endif
