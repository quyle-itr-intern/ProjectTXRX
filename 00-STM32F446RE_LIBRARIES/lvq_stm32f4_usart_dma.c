/**
  ******************************************************************************

  DMA USART For STM32F446RE
  Author:   LVQ
  Updated:  22st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_usart_dma.h"

/* Create variables if necessary */
#ifdef USE_USART1
				LVQ_USART_DMA_INT_t USART1_DMA_INT = {USART1_DMA_TX_CHANNEL, USART1_DMA_TX_STREAM, USART1_DMA_RX_CHANNEL, USART1_DMA_RX_STREAM};
#endif
#ifdef USE_USART2
				LVQ_USART_DMA_INT_t USART2_DMA_INT = {USART2_DMA_TX_CHANNEL, USART2_DMA_TX_STREAM, USART2_DMA_RX_CHANNEL, USART2_DMA_RX_STREAM};
#endif
#ifdef USE_USART3
				LVQ_USART_DMA_INT_t USART3_DMA_INT = {USART3_DMA_TX_CHANNEL, USART3_DMA_TX_STREAM, USART3_DMA_RX_CHANNEL, USART3_DMA_RX_STREAM};
#endif
#ifdef USE_UART4
				LVQ_USART_DMA_INT_t UART4_DMA_INT = {UART4_DMA_TX_CHANNEL, UART4_DMA_TX_STREAM, UART4_DMA_RX_CHANNEL, UART4_DMA_RX_STREAM};
#endif
#ifdef USE_UART5
				LVQ_USART_DMA_INT_t UART5_DMA_INT = {UART5_DMA_TX_CHANNEL, UART5_DMA_TX_STREAM, UART5_DMA_RX_CHANNEL, UART5_DMA_RX_STREAM};
#endif
#ifdef USE_USART6
				LVQ_USART_DMA_INT_t USART6_DMA_INT = {USART6_DMA_TX_CHANNEL, USART6_DMA_TX_STREAM, USART6_DMA_RX_CHANNEL, USART6_DMA_RX_STREAM};
#endif
#ifdef USE_UART7
				LVQ_USART_DMA_INT_t UART7_DMA_INT = {UART7_DMA_TX_CHANNEL, UART7_DMA_TX_STREAM};
#endif
#ifdef USE_UART8
				LVQ_USART_DMA_INT_t UART8_DMA_INT = {UART8_DMA_TX_CHANNEL, UART8_DMA_TX_STREAM};
#endif

/* Private DMA structure */
static DMA_InitTypeDef DMA_InitStruct;

/* Private functions */
LVQ_USART_DMA_INT_t* LVQ_USART_DMA_INT_GetSettings(USART_TypeDef* USARTx);

void LVQ_USART_DMA_Init(USART_TypeDef* USARTx) 
{
    /* Init DMA TX mode */
    /* Assuming USART is already initialized and clock is enabled */

    /* Get USART settings */
    LVQ_USART_DMA_INT_t* USART_Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);

    /* Enable DMA clock */
    if (USART_Settings->DMA_Stream_TX >= DMA2_Stream0) 
		{
        /* Enable DMA2 clock */
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    } else {
        /* Enable DMA1 clock */
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    }

    /* Clear flags */
    LVQ_DMA_ClearFlags(USART_Settings->DMA_Stream_TX);

    /* Set DMA options */
    DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
}

void LVQ_USART_DMA_InitWithStreamAndChannel(USART_TypeDef* USARTx, DMA_Stream_TypeDef* DMA_Stream, uint32_t DMA_Channel) 
{
    /* Get USART settings */
    LVQ_USART_DMA_INT_t* Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);

    /* Set DMA stream and channel */
    Settings->DMA_Stream_TX = DMA_Stream;
    Settings->DMA_Channel_TX = DMA_Channel;

    /* Init DMA TX */
    LVQ_USART_DMA_Init(USARTx);
}

DMA_Stream_TypeDef* LVQ_USART_DMA_GetStream(USART_TypeDef* USARTx) 
{
    /* Get USART settings */
    return LVQ_USART_DMA_INT_GetSettings(USARTx)->DMA_Stream_TX;
}

void LVQ_USART_DMA_Deinit(USART_TypeDef* USARTx) 
{
    /* Get USART settings */
    LVQ_USART_DMA_INT_t* Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);

    /* Deinit DMA Stream */
    DMA_DeInit(Settings->DMA_Stream_TX);
}

uint8_t LVQ_USART_DMA_Send(USART_TypeDef* USARTx, uint8_t* DataArray, uint16_t count) 
{
    /* Get USART settings */
    LVQ_USART_DMA_INT_t* Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);

    /* Check if DMA is working now */
    if (Settings->DMA_Stream_TX->NDTR) 
		{
        /* DMA works right now */
        return 0;
    }

    /* Set DMA options */
    DMA_InitStruct.DMA_Channel = Settings->DMA_Channel_TX;
    DMA_InitStruct.DMA_BufferSize = count;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &USARTx->DR;
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t) &DataArray[0];

    /* Deinit first, clear all flags */
    LVQ_DMA_ClearFlags(Settings->DMA_Stream_TX);

    /* Init DMA */
    DMA_Init(Settings->DMA_Stream_TX, &DMA_InitStruct);

    /* Enable DMA Stream */
    Settings->DMA_Stream_TX->CR |= DMA_SxCR_EN;

    /* Enable USART TX DMA */
    USARTx->CR3 |= USART_CR3_DMAT;

    /* DMA has started */
    return 1;
}

uint8_t LVQ_USART_DMA_Puts(USART_TypeDef* USARTx, char* DataArray) 
{
    /* Call DMA Send function */
    return LVQ_USART_DMA_Send(USARTx, (uint8_t*)DataArray, strlen(DataArray));
}

uint16_t LVQ_USART_DMA_Sending(USART_TypeDef* USARTx) 
{
    /* Get USART settings */
    LVQ_USART_DMA_INT_t* Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);

    /* DMA has work to do still */
    if (Settings->DMA_Stream_TX->NDTR) {
        return 1;
    }

    /* Check DMA Stream register of remaining data bytes */
    return 0;/* !USART_TXEMPTY(USARTx); */
}

void LVQ_USART_DMA_EnableInterrupts(USART_TypeDef* USARTx) 
{
    /* Get USART settings */
    LVQ_USART_DMA_INT_t* Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);

    /* Enable DMA interrupts */
    LVQ_DMA_EnableInterrupts(Settings->DMA_Stream_TX);
}

void LVQ_USART_DMA_DisableInterrupts(USART_TypeDef* USARTx) 
{
    /* Get USART settings */
    LVQ_USART_DMA_INT_t* Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);

    /* Disable DMA interrupts */
    LVQ_DMA_DisableInterrupts(Settings->DMA_Stream_TX);
}

/* Private functions */
LVQ_USART_DMA_INT_t* LVQ_USART_DMA_INT_GetSettings(USART_TypeDef* USARTx) 
{
    LVQ_USART_DMA_INT_t* result;
#ifdef USE_USART1
    if (USARTx == USART1) 
		{
        result = &USART1_DMA_INT;
    }
#endif
#ifdef USE_USART2
    if (USARTx == USART2) 
		{
        result = &USART2_DMA_INT;
    }
#endif
#ifdef USE_USART3
    if (USARTx == USART3) 
		{
        result = &USART3_DMA_INT;
    }
#endif
#ifdef USE_UART4
    if (USARTx == UART4) 
		{
        result = &UART4_DMA_INT;
    }
#endif
#ifdef USE_UART5
    if (USARTx == UART5) 
		{
        result = &UART5_DMA_INT;
    }
#endif
#ifdef USE_USART6
    if (USARTx == USART6) 
		{
        result = &USART6_DMA_INT;
    }
#endif
#ifdef USE_UART7
    if (USARTx == UART7) 
		{
        result = &UART7_DMA_INT;
    }
#endif
#ifdef USE_UART8
    if (USARTx == UART8) 
		{
        result = &UART8_DMA_INT;
    }
#endif

    /* Return */
    return result;
}
