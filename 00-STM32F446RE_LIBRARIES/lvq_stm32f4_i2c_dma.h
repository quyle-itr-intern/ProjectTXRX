/**
  ******************************************************************************

  I2C DMA For STM32F446RE
  Author:   LVQ
  Updated:  22st June 2022

  ******************************************************************************
*/

#ifndef __I2C_DMA_H
#define __I2C_DMA_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_STM32F4xx_Libraries
 * @{
 */

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_i2c.h"
#include "lvq_stm32f4_dma.h"
#include "stdlib.h"
#include "main.h"
#include "string.h"

/**
 * @defgroup LVQ_I2C_DMA_Macros
 * @brief    Library defines
 * @{
 */

/* Default DMA Stream and Channel for I2C1 */
#ifndef I2C1_DMA_TX_STREAM
#define I2C1_DMA_TX_STREAM      DMA1_Stream6
#define I2C1_DMA_TX_CHANNEL     DMA_Channel_1
#endif

#ifndef I2C1_DMA_RX_STREAM
#define I2C1_DMA_RX_STREAM      DMA1_Stream5
#define I2C1_DMA_RX_CHANNEL     DMA_Channel_1
#endif

/* Default DMA Stream and Channel for I2C2 */
#ifndef I2C2_DMA_TX_STREAM
#define I2C2_DMA_TX_STREAM      DMA1_Stream7
#define I2C2_DMA_TX_CHANNEL     DMA_Channel_7
#endif

#ifndef I2C2_DMA_RX_STREAM
#define I2C2_DMA_RX_STREAM      DMA1_Stream2
#define I2C2_DMA_RX_CHANNEL     DMA_Channel_7
#endif

/* Default DMA Stream and Channel for I2C3 */
#ifndef I2C3_DMA_TX_STREAM
#define I2C3_DMA_TX_STREAM      DMA1_Stream4
#define I2C3_DMA_TX_CHANNEL     DMA_Channel_3
#endif

#ifndef I2C3_DMA_RX_STREAM
#define I2C3_DMA_RX_STREAM      DMA1_Stream1
#define I2C3_DMA_RX_CHANNEL     DMA_Channel_1
#endif

/**
 * @}
 */

/**
 * @defgroup LVQ_I2C_DMA_Typedefs
 * @brief    Library Typedefs
 * @{
 */
/* Typedefs here */
/**
 * @}
 */

/**
 * @defgroup LVQ_I2C_DMA_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes I2C DMA TX functionality
 * @note   I2C HAVE TO be previously initialized using @ref LVQ_I2C library
 * @param  *I2Cx: Pointer to I2Cx where you want to enable DMA TX mode
 * @retval None
 */
void LVQ_I2C_DMA_TX_Init(I2C_TypeDef* I2Cx);

/**
 * @brief  Initializes I2C DMA RX functionality
 * @note   I2C HAVE TO be previously initialized using @ref LVQ_I2C library
 * @param  *I2Cx: Pointer to I2Cx where you want to enable DMA RX mode
 * @retval None
 */
void LVQ_I2C_DMA_RX_Init(I2C_TypeDef* I2Cx);

/**
 * @brief  Initializes I2C DMA TX functionality with custom DMA stream and Channel options
 * @note   I2C HAVE TO be previously initialized using @ref LVQ_I2C library
 *
 * @note   Use this function only in case default Stream and Channel settings are not good for you
 * @param  *I2Cx: Pointer to I2Cx where you want to enable DMA TX mode
 * @param  *DMA_Stream: Pointer to DMAy_Streamx, where y is DMA (1 or 2) and x is Stream (0 to 7)
 * @param  DMA_Channel: Select DMA channel for your I2C in specific DMA Stream
 * @retval None
 */
void LVQ_I2C_DMA_TX_InitWithStreamAndChannel(I2C_TypeDef* I2Cx, DMA_Stream_TypeDef* DMA_Stream, uint32_t DMA_Channel);

/**
 * @brief  Initializes I2C DMA RX functionality with custom DMA stream and Channel options
 * @note   I2C HAVE TO be previously initialized using @ref LVQ_I2C library
 *
 * @note   Use this function only in case default Stream and Channel settings are not good for you
 * @param  *I2Cx: Pointer to I2Cx where you want to enable DMA RX mode
 * @param  *DMA_Stream: Pointer to DMAy_Streamx, where y is DMA (1 or 2) and x is Stream (0 to 7)
 * @param  DMA_Channel: Select DMA channel for your I2C in specific DMA Stream
 * @retval None
 */
void LVQ_I2C_DMA_RX_InitWithStreamAndChannel(I2C_TypeDef* I2Cx, DMA_Stream_TypeDef* DMA_Stream, uint32_t DMA_Channel);

/**
 * @brief  Deinitializes I2C DMA TX functionality
 * @param  *I2Cx: Pointer to I2Cx where you want to disable DMA TX mode
 * @retval None
 */
void LVQ_I2C_DMA_Deinit(I2C_TypeDef* I2Cx);

/**
 * @breif  Enables interrupts for DMA for I2C streams
 * @note   I2C DMA must be initialized first using @ref LVQ_I2C_DMA_Init() or @ref LVQ_I2C_DMA_InitWithStreamAndChannel() functions
 * @param  *I2Cx: Pointer to I2Cx where DMA interrupts will be enabled
 * @retval None
 */
void LVQ_I2C_DMA_EnableInterrupts(I2C_TypeDef* I2Cx);

/**
 * @breif  Disables interrupts for DMA for I2C streams
 * @param  *I2Cx: Pointer to I2Cx where DMA interrupts will be disabled
 * @retval None
 */
void LVQ_I2C_DMA_DisableInterrupts(I2C_TypeDef* I2Cx);

/**
 * @brief  Gets poitner to DMA stream for desired I2C
 * @param  *I2Cx: Pointer to I2C where you wanna get its stream pointer
 * @retval Pointer to DMA stream for desired I2C
 */
DMA_Stream_TypeDef* LVQ_I2C_DMA_GetStream(I2C_TypeDef* I2Cx);

/**
 * @brief  Puts string to I2C port with DMA
 * @note   Try not to use local variables pointers for DMA memory as parameter *str
 * @param  *I2Cx: Pointer to I2Cx peripheral you will use
 * @param  *str: Pointer to string to send over I2C with DMA
 * @retval Sending started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t LVQ_I2C_DMA_Puts(I2C_TypeDef* I2Cx, uint8_t address, char* DataArray);

/**
 * @brief  Sends data over I2C with DMA TX functionality
 * @note   Try not to use local variables pointers for DMA memory as parameter *str
 * @param  *I2Cx: Pointer to I2Cx to use for send
 * @param  *DataArray: Pointer to array of data to be sent over I2C
 * @param  count: Number of data bytes to be sent over I2C with DMA
 * @retval Sending started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t LVQ_I2C_DMA_Send(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* DataArray, uint16_t count);

/**
 * @brief  Reads data over I2C with DMA RX functionality
 * @note   Try not to use local variables pointers for DMA memory as parameter *str
 * @param  *I2Cx: Pointer to I2Cx to use for send
 * @param  address: Address device to I2Cx to use for send
 * @param  reg: Address register device to I2Cx to use for send
 * @param  *DataArray: Pointer to array of data to be sent over I2C
 * @param  count: Number of data bytes to be sent over I2C with DMA
 * @retval Sending started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t LVQ_I2C_DMA_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* DataArray, uint16_t count);

/**
 * @brief  Checks if I2C DMA TX is still sending data
 * @param  *I2Cx: Pointer to I2Cx where you want to check if DMA is still working
 * @retval Sending status:
 *            - 0: I2C does not sending anymore
 *            - > 0: I2C DMA is still sending data
 */
uint16_t LVQ_I2C_DMA_Sending(I2C_TypeDef* I2Cx);

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
