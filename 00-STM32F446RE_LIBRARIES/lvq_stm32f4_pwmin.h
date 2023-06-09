/**
  ******************************************************************************

  PWMIN For SLVQ32F446RE
  Author:   LVQ
  Updated:  30 July 2022

  ******************************************************************************
*/

#ifndef _PWMIN_H
#define _PWMIN_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{

TIMER   |CHANNEL 1            |CHANNEL 2
        |PP1    PP2    PP3    |PP1    PP2    PP3

TIM 1   |PA8    PE9    -      |PA9    PE10   -
TIM 2   |PA0    PA5    PA15   |PA1    PB3    -
TIM 3   |PA6    PB4    PC6    |PA7    PB5    PC7
TIM 4   |PB6    PD12   -      |PB7    PD13   -
TIM 5   |PA0    PH10   -      |PA1    PH11   -
TIM 8   |PC6    PI5    -      |PC7    PI6    -
TIM 9   |PA2    PE5    -      |PA3    PE6    -
TIM 10  |PB8    PF6    -      |-      -      -
TIM 11  |PB9    PF7    -      |-      -      -
TIM 12  |PB14   PH6    -      |PB15   PH9    -
TIM 13  |PA6    PF8    -      |-      -      -
TIM 14  |PA7    PF9    -      |-      -      -

*/
#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_pwm.h"

#ifndef PWMIN_PRIORITY
#define PWMIN_PRIORITY      0x01
#endif

/**
 * @brief  PWM Input result enumeration
 */
typedef enum {
    LVQ_PWMIN_Result_Ok = 0,          /*!< Everything OK */
    LVQ_PWMIN_Result_TimerNotValid,   /*!< Invalid timer selected for PWM input capture */
    LVQ_PWMIN_Result_ChannelNotValid, /*!< Invalid input channel selected for timer */
    LVQ_PWMIN_Result_PinNotValid      /*!< Invalid pin selected for timer */
} LVQ_PWMIN_Result_t;

/**
 * @brief  PWM Input working struct
 */
typedef struct {
    float Frequency;                  /*!< Measured frequency on input pin */
    float DutyCycle;                  /*!< Measured duty cycle on input pin */
    TIM_TypeDef* __TIM;               /*!< Pointer to timer used for measure. For private use only */
    LVQ_TIMER_PROPERTIES_t __TIM_Data; /*!< Settings about timer. For private use only */
    float __Freq;                     /*!< Temporary frequency value. For private use only */
    float __Duty;                     /*!< Temporary duty cycle value. For private use only */
    uint8_t __SubPriority;            /*!< Subpriority for NVIC. For private use only */
} LVQ_PWMIN_t;

/**
 * @brief  Input channel for measure PWM selection
 */
typedef enum {
    LVQ_PWMIN_Channel_1 = 0, /*!< Use Channel 1 for PWMIN measure */
    LVQ_PWMIN_Channel_2      /*!< Use Channel 2 for PWMIN measure */
} LVQ_PWMIN_Channel_t;

/**
 * @brief  Pinspack for PWMIN channel pin
 */
typedef enum {
    LVQ_PWMIN_PinsPack_1 = 0, /*!< Use pinspack1 pin from PWMIN pinout table */
    LVQ_PWMIN_PinsPack_2,     /*!< Use pinspack2 pin from PWMIN pinout table */
    LVQ_PWMIN_PinsPack_3      /*!< Use pinspack3 pin from PWMIN pinout table */
} LVQ_PWMIN_PinsPack_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_PWMIN_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes and prepares timer for PWM input capture
 * @param  *TIMx: Pointer to TIM you will use for PWM input capture
 * @param  *PWMIN_Data: Pointer to an empty @ref LVQ_PWMIN_t structure
 * @param  PWMIN_Channel: Channel you will use on timer. This parameter can be a value of @ref LVQ_PWMIN_Channel_t enumeration
 * @param  PinsPack: Pinspack you will use for your channel. This parameter can be a value of @ref LVQ_PWMIN_PinsPack_t enumeration
 * @param  MinExpectedFrequency: Type minimal input frequency you expect in input.
 *            If you don't know what to expect, set to 1
 * @param  TIMx_IRQn: IRQ for NVIC settings. For TIM2 it can be TIM2_IRQn, for TIM3,4,5,... it can be TIMx_IRQn, where x is a number of timer.
 *            This parameter can be a value of @ref IRQn_Type enumeration
 * @retval Member of LVQ_PWMIN_Result_t
 */
LVQ_PWMIN_Result_t LVQ_PWMIN_InitTimer(TIM_TypeDef* TIMx, LVQ_PWMIN_t* PWMIN_Data, LVQ_PWMIN_Channel_t PWMIN_Channel, LVQ_PWMIN_PinsPack_t PinsPack, float MinExpectedFrequency, IRQn_Type TIMx_IRQn);

/**
 * @brief  Reads data from measured input PWM signal
 * @note   Data are valid if frequency is greater than 0
 * @param  *PWMIN_Data: Pointer to LVQ_PWMIN_t struct.
 *            Frequency and Duty cycle will be stored in this structure.
 * @retval Member of @ref LVQ_PWMIN_Result_t
 */
LVQ_PWMIN_Result_t LVQ_PWMIN_Get(LVQ_PWMIN_t* PWMIN_Data);

/**
 * @brief  Most important function.
 *         This function handles TIMx interrupts to calculate PWM input data.
 * @note   You need to call it in your TIMx handler.
@verbatim
//For TIM2, you will do:
void TIM2_IRQHandler(void) {
    LVQ_PWMIN_InterruptHandler(&PWMIN_TIM2_Data);
}
@endverbatim
 * @param  *PWMIN_Data: Pointer to @ref LVQ_PWMIN_t structure
 * @retval Member of @ref LVQ_PWMIN_Result_t
 */
LVQ_PWMIN_Result_t LVQ_PWMIN_InterruptHandler(LVQ_PWMIN_t* PWMIN_Data);

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
