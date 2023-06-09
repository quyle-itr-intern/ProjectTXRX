/**
  ******************************************************************************

  DELAY SYSTICK For STM32F446RE
  Author:   LVQ
  Updated:  21st June 2022

  ******************************************************************************
*/

#ifndef __DELAY_H
#define __DELAY_H
		
#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_pwm.h"
#include "lvq_stm32f4_defines.h"
#include "stdlib.h"
#include "main.h"

/**
 * @defgroup LVQ_DELAY_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Custom timer structure
 */
typedef struct {
    uint32_t ARR;             /*!< Auto reload value */
    uint32_t AutoReload;      /*!< Set to 1 if timer should be auto reloaded when it reaches zero */
    uint32_t CNT;             /*!< Counter value, counter counts down */
    uint8_t Enabled;          /*!< Set to 1 when timer is enabled */
    void (*Callback)(void*);  /*!< Callback which will be called when timer reaches zero */
    void* UserParameters;     /*!< Pointer to user parameters used for callback function */
} LVQ_DELAY_Timer_t;

/**
 * @brief  Number of allowed custom timers
 * @note   Should be changes in defines.h file if necessary
 */
#ifndef DELAY_MAX_CUSTOM_TIMERS
#define DELAY_MAX_CUSTOM_TIMERS   5
#endif

void SysTick_Handler(void);
void TimingDelay_Increasement(void);
void Delay_Init(void);
void Delay_ms_systick(uint32_t ms);
void Delay_us_systick(uint32_t us);
void Timer_reset(void);
uint32_t  Timer_read_ms(void);
uint32_t  Timer_read_us(void);

/**
 * @brief  User function, called each 1ms when interrupt from timer happen
 * @note   Here user should put things which has to be called periodically
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
__weak void LVQ_DELAY_1msHandler(void);

/**
 * @brief  Creates a new custom timer which has 1ms resolution
 * @note   It uses @ref malloc for memory allocation for timer structure
 * @param  ReloadValue: Number of milliseconds when timer reaches zero and callback function is called
 * @param  AutoReload: If set to 1, timer will start again when it reaches zero and callback is called
 * @param  StartTimer: If set to 1, timer will start immediately
 * @param  *LVQ_DELAY_CustomTimerCallback: Pointer to callback function which will be called when timer reaches zero
 * @param  *UserParameters: Pointer to void pointer to user parameters used as first parameter in callback function
 * @retval Pointer to allocated timer structure
 */
LVQ_DELAY_Timer_t* LVQ_DELAY_TimerCreate(uint32_t ReloadValue, uint8_t AutoReload, uint8_t StartTimer, void (*LVQ_DELAY_CustomTimerCallback)(void*), void* UserParameters);

/**
 * @brief  Deletes already allocated timer
 * @param  *Timer: Pointer to @ref LVQ_DELAY_Timer_t structure
 * @retval None
 */
void LVQ_DELAY_TimerDelete(LVQ_DELAY_Timer_t* Timer);

/**
 * @brief  Stops custom timer from counting
 * @param  *Timer: Pointer to @ref LVQ_DELAY_Timer_t structure
 * @retval Pointer to @ref LVQ_DELAY_Timer_t structure
 */
LVQ_DELAY_Timer_t* LVQ_DELAY_TimerStop(LVQ_DELAY_Timer_t* Timer);

/**
 * @brief  Starts custom timer counting
 * @param  *Timer: Pointer to @ref LVQ_DELAY_Timer_t structure
 * @retval Pointer to @ref LVQ_DELAY_Timer_t structure
 */
LVQ_DELAY_Timer_t* LVQ_DELAY_TimerStart(LVQ_DELAY_Timer_t* Timer);

/**
 * @brief  Resets custom timer counter value
 * @param  *Timer: Pointer to @ref LVQ_DELAY_Timer_t structure
 * @retval Pointer to @ref LVQ_DELAY_Timer_t structure
 */
LVQ_DELAY_Timer_t* LVQ_DELAY_TimerReset(LVQ_DELAY_Timer_t* Timer);

/**
 * @brief  Sets auto reload feature for timer
 * @note   Auto reload features is used for timer which starts again when zero is reached if auto reload active
 * @param  *Timer: Pointer to @ref LVQ_DELAY_Timer_t structure
 * uint8_t AutoReload: Set to 1 if you want to enable AutoReload or 0 to disable
 * @retval Pointer to @ref LVQ_DELAY_Timer_t structure
 */
LVQ_DELAY_Timer_t* LVQ_DELAY_TimerAutoReload(LVQ_DELAY_Timer_t* Timer, uint8_t AutoReload);

/**
 * @brief  Sets auto reload value for timer
 * @param  *Timer: Pointer to @ref LVQ_DELAY_Timer_t structure
 * @param  AutoReloadValue: Value for timer to be set when zero is reached and callback is called
 * @note   AutoReload feature must be enabled for timer in order to get this to work properly
 * @retval Pointer to @ref LVQ_DELAY_Timer_t structure
 */
LVQ_DELAY_Timer_t* LVQ_DELAY_TimerAutoReloadValue(LVQ_DELAY_Timer_t* Timer, uint32_t AutoReloadValue);

/**
 * @brief  Re-enables delay timer It has to be configured before with LVQ_DELAY_Init()
 * @note   This function enables delay timer. It can be systick or user selectable timer.
 * @param  None
 * @retval None
 */
void LVQ_DELAY_EnableDelayTimer(void);

/**
 * @brief  Disables delay timer
 * @note   This function disables delay timer. It can be systick or user selectable timer.
 * @param  None
 * @retval None
 */
void LVQ_DELAY_DisableDelayTimer(void);

extern __IO uint32_t LVQ_Time;
extern __IO uint32_t LVQ_Time2;

/**
 * @brief  Gets the LVQ_Time2 variable value
 * @param  None
 * @retval Current time in milliseconds
 * @note   This is not meant for public use
 */
uint16_t LVQ_DELAY_Time2(void);       

/**
 * @brief  Sets value for LVQ_Time variable
 * @param  time: Time in milliseconds
 * @retval None
 * @note   This is not meant for public use
 */
#define LVQ_DELAY_SetTime2(time)         (LVQ_Time2 = (time))

#endif
