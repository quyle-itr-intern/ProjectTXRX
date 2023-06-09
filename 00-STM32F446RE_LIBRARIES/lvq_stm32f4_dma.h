/**
  ******************************************************************************

  DMA For SLVQ32F446RE
  Author:   LVQ
  Updated:  21st June 2022

  ******************************************************************************
*/

#ifndef __DMA_H
#define __DMA_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */

#include "lvq_stm32f4_rccConfig.h"
#include "stdlib.h"
#include "main.h"

/**
 * @defgroup LVQ_DMA_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  DMA macros for interrupt flags
 */
#define DMA_FLAG_TCIF    ((uint32_t)0x00000020) /*!< DMA stream transfer complete */
#define DMA_FLAG_HTIF    ((uint32_t)0x00000010) /*!< DMA stream half transfer complete */
#define DMA_FLAG_TEIF    ((uint32_t)0x00000008) /*!< DMA stream transfer error */
#define DMA_FLAG_DMEIF   ((uint32_t)0x00000004) /*!< DMA stream direct mode error */
#define DMA_FLAG_FEIF    ((uint32_t)0x00000001) /*!< DMA stream FIFO error */
#define DMA_FLAG_ALL     ((uint32_t)0x0000003D) /*!< DMA stream all flags */

/* DMA1 preemption priority */
#ifndef DMA1_NVIC_PREEMPTION_PRIORITY
#define DMA1_NVIC_PREEMPTION_PRIORITY   0x05
#endif

/* DMA2 preemption priority */
#ifndef DMA2_NVIC_PREEMPTION_PRIORITY
#define DMA2_NVIC_PREEMPTION_PRIORITY   0x01
#endif

/**
 * @}
 */

/**
 * @defgroup LVQ_DMA_Typedefs
 * @brief    Library Typedefs
 * @{
 */
/* Typedefs here */
/**
 * @}
 */

/**
 * @defgroup LVQ_DMA_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Clears all flags for selected DMA stream
 * @param  *DMA_Stream: Pointer to @ref DMA_Stream_TypeDef DMA stream where you want to clear flags
 * @retval None
 */
void LVQ_DMA_ClearFlags(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Clears selected DMA interrupt flag
 * @param  *DMA_Stream: Pointer to @ref DMA_Stream_TypeDef DMA stream where you want to clear flags
 * @param  flags: Flag(s) which you wanna clear:
 *           - DMA_FLAG_TCIF: Transfer complete interrupt flag
 *           - DMA_FLAG HTIF: Half transfer complete interrupt flag
 *           - DMA_FLAG_TEIF: Transfer error interrupt flag
 *           - DMA_FLAG_DMEIF: Direct mode error interrupt flag
 *           - DMA_FLAG_FEIF: FIFO error interrupt flag
 *           - DMA_FLAG_ALL: All flags
 * @retval None
 */
void LVQ_DMA_ClearFlag(DMA_Stream_TypeDef* DMA_Stream, uint32_t flags);

/**
 * @brief  Enables interrupts for DMA stream
 * @note   It adds IRQ to NVIC and enables all possible DMA STREAM interrupts
 * @param  *DMA_Stream: Pointer to DMA stream where to enable interrupts
 * @retval None
 */
void LVQ_DMA_EnableInterrupts(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Disables interrupts for DMA stream
 * @note   It adds IRQ to NVIC and enables all possible DMA STREAM interrupts
 * @param  *DMA_Stream: Pointer to DMA stream where to disable interrupts
 * @retval None
 */
void LVQ_DMA_DisableInterrupts(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Transfer complete callback
 * @note   This function is called when interrupt for specific stream happens
 * @param  *DMA_Stream: Pointer to DMA stream where interrupt happens
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void LVQ_DMA_TransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Half transfer complete callback
 * @note   This function is called when interrupt for specific stream happens
 * @param  *DMA_Stream: Pointer to DMA stream where interrupt happens
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void LVQ_DMA_HalfTransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Transfer error callback
 * @note   This function is called when interrupt for specific stream happens
 * @param  *DMA_Stream: Pointer to DMA stream where interrupt happens
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void LVQ_DMA_TransferErrorHandler(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Direct mode error callback
 * @note   This function is called when interrupt for specific stream happens
 * @param  *DMA_Stream: Pointer to DMA stream where interrupt happens
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void LVQ_DMA_DirectModeErrorHandler(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  FIFO error error callback
 * @note   This function is called when interrupt for specific stream happens
 * @param  *DMA_Stream: Pointer to DMA stream where interrupt happens
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void LVQ_DMA_FIFOErrorHandler(DMA_Stream_TypeDef* DMA_Stream);

uint32_t LVQ_DMA_GetFlags(DMA_Stream_TypeDef* DMA_Stream, uint32_t flag);

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
