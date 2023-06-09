/**
  ******************************************************************************

  SPI DMA For STM32F446RE
  Author:   LVQ
  Updated:  22st August 2022

  ******************************************************************************
*/

#ifndef __SPI_DMA_H
#define __SPI_DMA_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_STM32F4xx_Libraries
 * @{
 */

/**

SPIx     | DMA  | DMA TX Stream | DMA TX Channel | DMA RX Stream | DMA RX Channel

SPI1     | DMA2 | DMA Stream 3  | DMA Channel 3  | DMA Stream 2  | DMA Channel 3
SPI2     | DMA1 | DMA Stream 4  | DMA Channel 0  | DMA Stream 3  | DMA Channel 0
SPI3     | DMA1 | DMA Stream 5  | DMA Channel 0  | DMA Stream 0  | DMA Channel 0
SPI4     | DMA2 | DMA Stream 1  | DMA Channel 4  | DMA Stream 0  | DMA Channel 4
												-- Don't have in STM32f446RE --
SPI5     | DMA2 | DMA Stream 6  | DMA Channel 7  | DMA Stream 5  | DMA Channel 7
SPI6     | DMA2 | DMA Stream 5  | DMA Channel 1  | DMA Stream 6  | DMA Channel 0

*/

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_spi.h"
#include "lvq_stm32f4_dma.h"

/**
 * @defgroup LVQ_SPI_DMA_Macros
 * @brief    Library defines
 * @{
 */

/* SPI1 TX and RX default settings */
#ifndef SPI1_DMA_TX_STREAM
#define SPI1_DMA_TX_STREAM    DMA2_Stream3
#define SPI1_DMA_TX_CHANNEL   DMA_Channel_3
#endif
#ifndef SPI1_DMA_RX_STREAM
#define SPI1_DMA_RX_STREAM    DMA2_Stream2
#define SPI1_DMA_RX_CHANNEL   DMA_Channel_3
#endif

/* SPI2 TX and RX default settings */
#ifndef SPI2_DMA_TX_STREAM
#define SPI2_DMA_TX_STREAM    DMA1_Stream4
#define SPI2_DMA_TX_CHANNEL   DMA_Channel_0
#endif
#ifndef SPI2_DMA_RX_STREAM
#define SPI2_DMA_RX_STREAM    DMA1_Stream3
#define SPI2_DMA_RX_CHANNEL   DMA_Channel_0
#endif

/* SPI3 TX and RX default settings */
#ifndef SPI3_DMA_TX_STREAM
#define SPI3_DMA_TX_STREAM    DMA1_Stream5
#define SPI3_DMA_TX_CHANNEL   DMA_Channel_0
#endif
#ifndef SPI3_DMA_RX_STREAM
#define SPI3_DMA_RX_STREAM    DMA1_Stream0
#define SPI3_DMA_RX_CHANNEL   DMA_Channel_0
#endif

/* SPI4 TX and RX default settings */
#ifndef SPI4_DMA_TX_STREAM
#define SPI4_DMA_TX_STREAM    DMA2_Stream1
#define SPI4_DMA_TX_CHANNEL   DMA_Channel_4
#endif
#ifndef SPI4_DMA_RX_STREAM
#define SPI4_DMA_RX_STREAM    DMA2_Stream0
#define SPI4_DMA_RX_CHANNEL   DMA_Channel_4
#endif

/* SPI5 TX and RX default settings */
#ifndef SPI5_DMA_TX_STREAM
#define SPI5_DMA_TX_STREAM    DMA2_Stream6
#define SPI5_DMA_TX_CHANNEL   DMA_Channel_7
#endif
#ifndef SPI5_DMA_RX_STREAM
#define SPI5_DMA_RX_STREAM    DMA2_Stream5
#define SPI5_DMA_RX_CHANNEL   DMA_Channel_7
#endif

/* SPI6 TX and RX default settings */
#ifndef SPI6_DMA_TX_STREAM
#define SPI6_DMA_TX_STREAM    DMA2_Stream5
#define SPI6_DMA_TX_CHANNEL   DMA_Channel_1
#endif
#ifndef SPI6_DMA_RX_STREAM
#define SPI6_DMA_RX_STREAM    DMA2_Stream6
#define SPI6_DMA_RX_CHANNEL   DMA_Channel_1
#endif

/**
 * @}
 */

/**
 * @defgroup LVQ_SPI_DMA_Typedefs
 * @brief    Library Typedefs
 * @{
 */
/* Typedefs here */
/**
 * @}
 */

/**
 * @defgroup LVQ_SPI_DMA_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes SPI peripheral DMA
 * @note   This function initializes TX and RX DMA streams for SPI
 *
 * @note   SPI HAVE TO be previously initialized using @ref LVQ_SPI library
 * @param  *SPIx: Pointer to SPI peripheral where you want to enable DMA
 * @retlva None
 */
void LVQ_SPI_DMA_Init(SPI_TypeDef* SPIx);

/**
 * @brief  Initializes SPI DMA functionality with custom DMA stream and channel options
 * @note   SPI HAVE TO be previously initialized using @ref LVQ_USART library
 *
 * @note   Use this function only in case default Stream and Channel settings are not good for you
 * @param  *SPIx: Pointer to SPIx where you want to set custom DMA streams and channels
 * @param  *TX_Stream: Pointer to DMAy_Streamx, where y is DMA (1 or 2) and x is Stream (0 to 7)
 * @param  TX_Channel: Select DMA TX channel for your SPI in specific DMA Stream
 * @param  *RX_Stream: Pointer to DMAy_Streamx, where y is DMA (1 or 2) and x is Stream (0 to 7)
 * @param  RX_Channel: Select DMA RX channel for your SPI in specific DMA Stream
 * @retval None
 */
void LVQ_SPI_DMA_InitWithStreamAndChannel(SPI_TypeDef* SPIx, DMA_Stream_TypeDef* TX_Stream, uint32_t TX_Channel, DMA_Stream_TypeDef* RX_Stream, uint32_t RX_Channel);

/**
 * @brief  Deinitializes SPI DMA functionality
 * @param  *SPIx: Pointer to SPIx where you want to disable DMA mode
 * @retval None
 */
void LVQ_SPI_DMA_Deinit(SPI_TypeDef* SPIx);

/**
 * @brief  Transmits (exchanges) data over SPI with DMA
 * @note   Try not to use local variables pointers for DMA memory as TX and RX Buffers
 * @param  *SPIx: Pointer to SPIx where DMA transmission will happen
 * @param  *TX_Buffer: Pointer to TX_Buffer where DMA will take data to sent over SPI.
 *            Set this parameter to NULL, if you want to sent "0x00" and only receive data into *RX_Buffer pointer
 * @param  *RX_Buffer: Pointer to RX_Buffer where DMA will save data from SPI.
 *            Set this parameter to NULL, if you don't want to receive any data, only sent from TX_Buffer
 * @param  count: Number of bytes to be send/received over SPI with DMA
 * @retval Transmission started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t LVQ_SPI_DMA_Transmit(SPI_TypeDef* SPIx, uint8_t* TX_Buffer, uint8_t* RX_Buffer, uint16_t count);

/**
 * @brief  Sends data over SPI without receiving data back using DMA
 * @note   Try not to use local variables pointers for DMA memory as TX and RX Buffers
 * @param  *SPIx: Pointer to SPIx where DMA transmission will happen
 * @param  *TX_Buffer: Pointer to TX_Buffer where DMA will take data to sent over SPI
 * @param  count: Number of bytes to be send/received over SPI with DMA
 * @retval Sending started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 * @note   Defined as macro for faster execution
 */
#define LVQ_SPI_DMA_Send(SPIx, TX_Buffer, count)   (LVQ_SPI_DMA_Transmit(SPIx, TX_Buffer, NULL, count))

/**
 * @brief  Sends dummy byte (0x00) over SPI to receive data back from slave over DMA
 * @note   Try not to use local variables pointers for DMA memory as TX and RX Buffers
 * @param  SPIx: Pointer to SPIx where DMA transmission will happen
 * @param  RX_Buffer: Pointer to RX_Buffer where DMA will save data from SPI
 * @param  count: Number of bytes to be received over SPI with DMA
 * @retval Receiving started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 * @note   Defined as macro for faster execution
 */
#define LVQ_SPI_DMA_Receive(SPIx, RX_Buffer, count)   (LVQ_SPI_DMA_Transmit(SPIx, NULL, RX_Buffer, count))

/**
 * @brief  Sends one byte value multiple times over SPI with DMA
 * @param  SPIx: Pointer to SPIx where DMA transmission will happen
 * @param  value: Byte value to be sent
 * @param  count: Number of bytes with value of @arg value will be sent
 * @retval Transmission started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t LVQ_SPI_DMA_SendByte(SPI_TypeDef* SPIx, uint8_t value, uint16_t count);

/**
 * @brief  Sends one half word value multiple times over SPI with DMA
 * @param  SPIx: Pointer to SPIx where DMA transmission will happen
 * @param  value: Byte value to be sent
 * @param  count: Number of half words with value of @arg value will be sent
 * @retval Transmission started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t LVQ_SPI_DMA_SendHalfWord(SPI_TypeDef* SPIx, uint16_t value, uint16_t count);

/**
 * @brief  Checks if SPI DMA is still sending/receiving data
 * @param  *SPIx: Pointer to SPIx where you want to enable DMA TX mode
 * @retval Sending status:
 *            - 0: SPI DMA does not sending any more
 *            - > 0: SPI DMA is still sending data
 */
uint8_t LVQ_SPI_DMA_Working(SPI_TypeDef* SPIx);

/**
 * @brief  Gets TX DMA stream for specific SPI
 * @param  *SPIx: Pointer to SPIx peripheral where you want to get TX stream
 * @retval Pointer to SPI DMA TX stream
 */
DMA_Stream_TypeDef* LVQ_SPI_DMA_GetStreamTX(SPI_TypeDef* SPIx);

/**
 * @brief  Gets RX DMA stream for specific SPI
 * @param  *SPIx: Pointer to SPIx peripheral where you want to get RX stream
 * @retval Pointer to SPI DMA RX stream
 */
DMA_Stream_TypeDef* LVQ_SPI_DMA_GetStreamRX(SPI_TypeDef* SPIx);

/**
 * @brief  Enables DMA stream interrupts for specific SPI
 * @param  *SPIx: Pointer to SPIx peripheral where you want to enable DMA stream interrupts
 * @retval None
 */
void LVQ_SPI_DMA_EnableInterrupts(SPI_TypeDef* SPIx);

/**
 * @brief  Disables DMA stream interrupts for specific SPI
 * @param  *SPIx: Pointer to SPIx peripheral where you want to disable DMA stream interrupts
 * @retval None
 */
void LVQ_SPI_DMA_DisableInterrupts(SPI_TypeDef* SPIx);

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
