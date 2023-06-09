/**
  ******************************************************************************

  WATCHDOG For SLVQ32F446RE
  Author:   LVQ
  Updated:  4st July 2022

  ******************************************************************************
*/

#ifndef __WATCHDOG_H
#define __WATCHDOG_H 
/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */
 
#include "lvq_stm32f4_rccConfig.h"

/**
 * @defgroup LVQ_WATCHDOG_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief Watchdog timeout settings
 */
typedef enum {
    LVQ_WATCHDOG_Timeout_5ms = 0x00,   /*!< System reset called every 5ms */
    LVQ_WATCHDOG_Timeout_10ms = 0x01,  /*!< System reset called every 10ms */
    LVQ_WATCHDOG_Timeout_15ms = 0x02,  /*!< System reset called every 15ms */
    LVQ_WATCHDOG_Timeout_30ms = 0x03,  /*!< System reset called every 30ms */
    LVQ_WATCHDOG_Timeout_60ms = 0x04,  /*!< System reset called every 60ms */
    LVQ_WATCHDOG_Timeout_120ms = 0x05, /*!< System reset called every 120ms */
    LVQ_WATCHDOG_Timeout_250ms = 0x06, /*!< System reset called every 250ms */
    LVQ_WATCHDOG_Timeout_500ms = 0x07, /*!< System reset called every 500ms */
    LVQ_WATCHDOG_Timeout_1s = 0x08,    /*!< System reset called every 1s */
    LVQ_WATCHDOG_Timeout_2s = 0x09,    /*!< System reset called every 2s */
    LVQ_WATCHDOG_Timeout_4s = 0x0A,    /*!< System reset called every 4s */
    LVQ_WATCHDOG_Timeout_8s = 0x0B,    /*!< System reset called every 8s */
    LVQ_WATCHDOG_Timeout_16s = 0x0C,   /*!< System reset called every 16s */
    LVQ_WATCHDOG_Timeout_32s = 0x0D    /*!< System reset called every 32s. This is maximum value allowed with IWDG timer */
} LVQ_WATCHDOG_Timeout_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_WATCHDOG_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initialize Watchdog timer
 * @note   After you initialize it, you can't disable it unless reset occur.
 * @param  timeout. Timeout value when watchdog reset happen if not reset by user.
 *            This parameter can be a value of @ref LVQ_WATCHDOG_Timeout_t enumeration
 * @retval Value if system was reset because of watchdog timer
 *            - 1: Reset happen because of watchdog
 *            - 0: Otherwise
 */
uint8_t LVQ_WATCHDOG_Init(LVQ_WATCHDOG_Timeout_t timeout);

/**
 * @brief  Reset Watchdog timer
 * @note   This function have to be called periodically to reset watchdog timer, or your system will reset
 * @param  None
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define LVQ_WATCHDOG_Reset()     (IWDG->KR = 0xAAAA)

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif

