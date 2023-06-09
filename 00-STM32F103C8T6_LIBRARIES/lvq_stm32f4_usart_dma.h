/**
  ******************************************************************************

  DMA USART For SLVQ32F446RE
  Author:   LVQ
  Updated:  22st June 2022

  ******************************************************************************
*/

#ifndef __USART_DMA_H
#define __USART_DMA_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_usart.h"
#include "lvq_stm32f4_dma.h"
#include "stm32f4xx_usart.h"
#include "stdlib.h"
#include "main.h"
#include "string.h"

/**
 * @defgroup LVQ_USART_DMA_Macros
 * @brief    Library defines
 * @{
 */

/* Default DMA Stream and Channel for USART1 */
#ifndef USART1_DMA_TX_STREAM
#define USART1_DMA_TX_STREAM      DMA2_Stream7
#define USART1_DMA_TX_CHANNEL     DMA_Channel_4
#endif

/* Default DMA Stream and Channel for USART2 */
#ifndef USART2_DMA_TX_STREAM
#define USART2_DMA_TX_STREAM      DMA1_Stream6
#define USART2_DMA_TX_CHANNEL     DMA_Channel_4
#endif

/* Default DMA Stream and Channel for USART3 */
#ifndef USART3_DMA_TX_STREAM
#define USART3_DMA_TX_STREAM      DMA1_Stream3
#define USART3_DMA_TX_CHANNEL     DMA_Channel_4
#endif

/* Default DMA Stream and Channel for UART4 */
#ifndef UART4_DMA_TX_STREAM
#define UART4_DMA_TX_STREAM       DMA1_Stream4
#define UART4_DMA_TX_CHANNEL      DMA_Channel_4
#endif

/* Default DMA Stream and Channel for UART5 */
#ifndef UART5_DMA_TX_STREAM
#define UART5_DMA_TX_STREAM       DMA1_Stream7
#define UART5_DMA_TX_CHANNEL      DMA_Channel_4
#endif

/* Default DMA Stream and Channel for USART6 */
#ifndef USART6_DMA_TX_STREAM
#define USART6_DMA_TX_STREAM      DMA2_Stream6
#define USART6_DMA_TX_CHANNEL     DMA_Channel_5
#endif

/* Default DMA Stream and Channel for UART7 */
#ifndef UART7_DMA_TX_STREAM
#define UART7_DMA_TX_STREAM       DMA1_Stream1
#define UART7_DMA_TX_CHANNEL      DMA_Channel_5
#endif

/* Default DMA Stream and Channel for UART8 */
#ifndef UART8_DMA_TX_STREAM
#define UART8_DMA_TX_STREAM       DMA1_Stream0
#define UART8_DMA_TX_CHANNEL      DMA_Channel_5
#endif

/**
 * @}
 */

/**
 * @defgroup LVQ_USART_DMA_Typedefs
 * @brief    Library Typedefs
 * @{
 */
/* Typedefs here */
/**
 * @}
 */

/**
 * @defgroup LVQ_USART_DMA_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes USART DMA TX functionality
 * @note   USART HAVE TO be previously initialized using @ref LVQ_USART library
 * @param  *USARTx: Pointer to USARTx where you want to enable DMA TX mode
 * @retval None
 */
void LVQ_USART_DMA_Init(USART_TypeDef* USARTx);

/**
 * @brief  Initializes USART DMA TX functionality with custom DMA stream and Channel options
 * @note   USART HAVE TO be previously initialized using @ref LVQ_USART library
 *
 * @note   Use this function only in case default Stream and Channel settings are not good for you
 * @param  *USARTx: Pointer to USARTx where you want to enable DMA TX mode
 * @param  *DMA_Stream: Pointer to DMAy_Streamx, where y is DMA (1 or 2) and x is Stream (0 to 7)
 * @param  DMA_Channel: Select DMA channel for your USART in specific DMA Stream
 * @retval None
 */
void LVQ_USART_DMA_InitWithStreamAndChannel(USART_TypeDef* USARTx, DMA_Stream_TypeDef* DMA_Stream, uint32_t DMA_Channel);

/**
 * @brief  Deinitializes USART DMA TX functionality
 * @param  *USARTx: Pointer to USARTx where you want to disable DMA TX mode
 * @retval None
 */
void LVQ_USART_DMA_Deinit(USART_TypeDef* USARTx);

/**
 * @breif  Enables interrupts for DMA for USART streams
 * @note   USART DMA must be initialized first using @ref LVQ_USART_DMA_Init() or @ref LVQ_USART_DMA_InitWithStreamAndChannel() functions
 * @param  *USARTx: Pointer to USARTx where DMA interrupts will be enabled
 * @retval None
 */
void LVQ_USART_DMA_EnableInterrupts(USART_TypeDef* USARTx);

/**
 * @breif  Disables interrupts for DMA for USART streams
 * @param  *USARTx: Pointer to USARTx where DMA interrupts will be disabled
 * @retval None
 */
void LVQ_USART_DMA_DisableInterrupts(USART_TypeDef* USARTx);

/**
 * @brief  Gets poitner to DMA stream for desired USART
 * @param  *USARTx: Pointer to USART where you wanna get its stream pointer
 * @retval Pointer to DMA stream for desired USART
 */
DMA_Stream_TypeDef* LVQ_USART_DMA_GetStream(USART_TypeDef* USARTx);

/**
 * @brief  Puts string to USART port with DMA
 * @note   Try not to use local variables pointers for DMA memory as parameter *str
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @param  *str: Pointer to string to send over USART with DMA
 * @retval Sending started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t LVQ_USART_DMA_Puts(USART_TypeDef* USARTx, char* str);

/**
 * @brief  Sends data over USART with DMA TX functionality
 * @note   Try not to use local variables pointers for DMA memory as parameter *str
 * @param  *USARTx: Pointer to USARTx to use for send
 * @param  *DataArray: Pointer to array of data to be sent over USART
 * @param  count: Number of data bytes to be sent over USART with DMA
 * @retval Sending started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t LVQ_USART_DMA_Send(USART_TypeDef* USARTx, uint8_t* DataArray, uint16_t count);

/**
 * @brief  Checks if USART DMA TX is still sending data
 * @param  *USARTx: Pointer to USARTx where you want to check if DMA is still working
 * @retval Sending status:
 *            - 0: USART does not sending anymore
 *            - > 0: USART DMA is still sending data
 */
uint16_t LVQ_USART_DMA_Sending(USART_TypeDef* USARTx);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
