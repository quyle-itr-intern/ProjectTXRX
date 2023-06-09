/**
  ******************************************************************************

  LOW POWER For SLVQ32F446RE
  Author:   LVQ
  Updated:  13st August 2022

  ******************************************************************************
*/

#ifndef __LOWPOWER_H
#define __LOWPOWER_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_STM32F4xx_Libraries
 * @{
 */
 
#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_delay.h"
#include "stm32f4xx_pwr.h"

/**
 * @defgroup LVQ_LOWPOWER_Macros
 * @brief    Library macros
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup LVQ_LOWPOWER_Typedefs
 * @brief    Library typedefs
 * @{
 */

typedef enum {
    LVQ_LOWPOWERMODE_SleepUntilInterrupt = 0x00, /*!< Sleep mode until any interrupt in NVIC occurs */
    LVQ_LOWPOWERMODE_SleepUntilEvent,            /*!< Sleep mode until any event occurs */
    LVQ_LOWPOWERMODE_StopUntilInterrupt,         /*!< Stop mode until interrupt in EXTI line occurs */
    LVQ_LOWPOWERMODE_StopUntilEvent,             /*!< Stop mode until event occurs */
    LVQ_LOWPOWERMODE_Standby                     /*!< Standby mode until any interrupt occurs */
} LVQ_LOWPOWERMODE_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_LOWPOWER_Functions
 * @brief    Library Functions
 *
 *           All functions for sleep mode configuration
 * @{
 */

/**
 * @brief  Put device into sleep mode
 *
 * @note   MCU will be in sleep mode until next interrupt occured.
 *         This means, that delay timer (Systick or any user selectable delay timer) can wakeup SLVQ32F4 too.
 *         If you want prevent this, you have to disable delay timer as well it.
 *         This can be done with this function.
 *
 * @param  delay_timer: Set to > 0 and delay timer will be disabled or set to 0 and it will stay as is
 * @retval None
 */
void LVQ_LOWPOWER_SleepUntilInterrupt(uint8_t delay_timer);

/**
 * @brief  Put device into sleep mode
 * @note   MCU will be in sleep mode until next event occured
 * @param  None
 * @retval None
 */
void LVQ_LOWPOWER_SleepUntilEvent(void);

/**
 * @brief  Go to STOP mode with "Wait For Interrupt" instruction
 * @param  None
 * @retval None
 */
void LVQ_LOWPOWER_StopUntilInterrupt(void);

/**
 * @brief  Go to STOP mode with "Wait For Event" instruction
 * @param  None
 * @retval None
 */
void LVQ_LOWPOWER_StopUntilEvent(void);

/**
 * @brief  To go Standby mode
 * @param  None
 * @retval None
 */
void LVQ_LOWPOWER_Standby(void);

/**
 * @brief  Checks if system was reset because of wakeup from standby mode
 * @param  None
 * @retval 0: System was not reset because of wake up from standby mode
 *         1: System was reset because of wake up from standby mode
 */
uint8_t LVQ_LOWPOWER_StandbyReset(void);

/**
 * @brief  Enables Wakeup pin, fixed pin, PA0
 * @note   This pin is fixed and can't be changed.
 * @note   Rising edge on wakeup pin will wake up device from standby mode
 * @param  None
 * @retval None
 */
void LVQ_LOWPOWER_EnableWakeUpPin(void);

/**
 * @brief  Disables Wakeup pin
 * @note   If you go to standby mode after PA0 is disabled, then you can't wake him up with rising edge
 * @param  None
 * @retval None
 */
void LVQ_LOWPOWER_DisableWakeUpPin(void);

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
