/**
  ******************************************************************************

  PWM For STM32F446RE
  Author:   LVQ
  Updated:  19st June 2022

  ******************************************************************************
*/

#ifndef __PWM_H
#define __PWM_H 
/**
 * @addtogroup LVQ_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup LVQ_PWM
 * @brief    PWM library for SLVQ32F4xx devices, supporting all possible timers with PWM feature - http://sLVQ32f4-discovery.net/2014/09/library-33-pwm-for-sLVQ32f4xx/
 * @{
 *
 * This library allows you to use PWM feature on any timer with supported PWM output
 *
 * \par Pinout
 *
 * PWM pins are connected to fixed possible pins
 *
@verbatim
TIMER   |CHANNEL 1            |CHANNEL 2            |CHANNEL 3            |CHANNEL 4
        |PP1    PP2    PP3    |PP1    PP2    PP3    |PP1    PP2    PP3    |PP1    PP2    PP3

TIM 1   |PA8    PE9    -      |PA9    PE10   -      |PA10   PE13   -      |PA11   PE14   -
TIM 2   |PA0    PA5    PA15   |PA1    PB3    -      |PA2    PB10   -      |PA3    PB11   -
TIM 3   |PA6    PB4    PC6    |PA7    PB5    PC7    |PB0    PC8    -      |PB1    PC9    -
TIM 4   |PB6    PD12   -      |PB7    PD13   -      |PB8    PD14   -      |PB9    PD15    -
TIM 5   |PA0    PH10   -      |PA1    PH11   -      |PA2    PH12   -      |PA3    PI0    -
TIM 8   |PC6    PI5    -      |PC7    PI6    -      |PC8    PI7    -      |PC9    PI2    -
TIM 9   |PA2    PE5    -      |PA3    PE6    -      |-      -      -      |-      -      -
TIM 10  |PB8    PF6    -      |-      -      -      |-      -      -      |-      -      -
TIM 11  |PB9    PF7    -      |-      -      -      |-      -      -      |-      -      -
TIM 12  |PB14   PH6    -      |PB15   PH9    -      |-      -      -      |-      -      -
TIM 13  |PA6    PF8    -      |-      -      -      |-      -      -      |-      -      -
TIM 14  |PA7    PF9    -      |-      -      -      |-      -      -      |-      -      -

*/

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_gpio.h"

/**
 * @defgroup LVQ_PWM_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  PWM Result enumeration
 */
typedef enum {
    LVQ_PWM_Result_Ok = 0,           /*!< Everything OK */
    LVQ_PWM_Result_FrequencyTooHigh, /*!< You select too high frequency for timer for PWM */
    LVQ_PWM_Result_FrequencyTooLow,  /*!< Prescaler value is too big for selected frequency */
    LVQ_PWM_Result_PulseTooHigh,     /*!< Pulse for Output compare is larger than timer period */
    LVQ_PWM_Result_TimerNotValid,    /*!< Selected timer is not valid. This happens when you select TIM6 or TIM7,
                                            because they don't have PWM capability */
    LVQ_PWM_Result_ChannelNotValid,  /*!< Channel is not valid. Some timers don't have all 4 timers available for PWM */
    LVQ_PWM_Result_PinNotValid       /*!< Selected pin is not valid. Most channels have only 2 possible pins for PWM,
                                            but some 3. If you select pin 3 on channel that don't have 3rd pin available
                                            for PWM, this will be returned */
} LVQ_PWM_Result_t;

/**
 * @brief  PWM Timer data
 */
typedef struct {
    TIM_TypeDef* TIM;       /*!< Pointer to timer used */
    uint32_t Period;        /*!< Period used, set on initialization for PWM */
    uint32_t Prescaler;     /*!< Prescaler used for PWM frequency */
    uint32_t Frequency;     /*!< PWM frequency used */
    uint32_t Micros;        /*!< Microseconds used for one period.
                                    This is not useful in large pwm frequency, but good for controlling servos or similar,
                                    Where you need exact time of pulse high */
    uint32_t CH_Periods[4]; /*!< Array of periods for PWM compare */
    uint8_t CH_Init;        /*!< Flag to check if specific channel is already initialized */
} LVQ_PWM_TIM_t;

/**
 * @brief  Structure for timer data
 */
typedef struct {
    uint32_t TimerFrequency; /*!< timer's working frequency */
    uint32_t MaxPeriod;      /*!< Max timer period */
    uint32_t MaxPrescaler;   /*!< Max timer prescaler */
    uint32_t Period;         /*!< Timer's working period */
    uint32_t Prescaler;      /*!< Timer's working prescaler */
    uint32_t Frequency;      /*!< Timer's reload frequency */
} LVQ_TIMER_PROPERTIES_t;

/**
 * @brief  Timer result enumeration
 */
typedef enum {
    LVQ_TIMER_Result_Ok,               /*!< Everything OK */
    LVQ_TIMER_Result_Error,            /*!< An error occurred */
    LVQ_TIMER_Result_TimerNotValid,    /*!< Timer is not valid */
    LVQ_TIMER_Result_FrequencyTooHigh, /*!< Frequency for timer is too high */
    LVQ_TIMER_Result_FrequencyTooLow   /*!< Frequency for timer is too low */
} LVQ_TIMER_Result_t;

/**
 * @brief  Channel selection for PWM on specific timer
 */
typedef enum {
    LVQ_PWM_Channel_1 = 0x00, /*!<  Operate with channel 1 */
    LVQ_PWM_Channel_2 = 0x01, /*!<  Operate with channel 2 */
    LVQ_PWM_Channel_3 = 0x02, /*!<  Operate with channel 3 */
    LVQ_PWM_Channel_4 = 0x03  /*!<  Operate with channel 4 */
} LVQ_PWM_Channel_t;

/**
 * @brief  Pin selected for corresponding channel on specific channel
 */
typedef enum {
    LVQ_PWM_PinsPack_1 = 0x00, /*!< Pinspack 1 from pinout table */
    LVQ_PWM_PinsPack_2,        /*!< Pinspack 2 from pinout table */
    LVQ_PWM_PinsPack_3         /*!< Pinspack 3 from pinout table */
} LVQ_PWM_PinsPack_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_PWM_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes specific timer for PWM capability
 * @param  *TIMx: Pointer to selected timer, you want to use for PWM
 * @param  *TIM_Data: Pointer to blank @ref LVQ_PWM_TIM_t structure. Here will init function save all data for specific timer
 * @param  PWMFrequency: Select custom frequency for PWM
 * @retval Member of @ref LVQ_PWM_Result_t
 */
LVQ_PWM_Result_t LVQ_PWM_InitTimer(TIM_TypeDef* TIMx, LVQ_PWM_TIM_t* TIM_Data, double PWMFrequency);

/**
 * @brief  Initializes channel used for specific timer
 * @param  *TIM_Data: Pointer to struct with already initialized timer for PWM
 * @param  Channel: Select channel you will use on specific timer. This parameter can be a value of @ref LVQ_PWM_Channel_t
 * @param  PinsPack: Select which pinspack you will use for pin. This parameter can be a value of @ref LVQ_PWM_PinsPack_t
 * @retval Member of @ref LVQ_PWM_Result_t
 */
LVQ_PWM_Result_t LVQ_PWM_InitChannel(LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel, LVQ_PWM_PinsPack_t PinsPack);

/**
 * @brief  Sets PWM value for specific timer and channel
 * @param  *TIM_Data: Pointer to struct with already initialized timer for PWM
 * @param  Channel: Select channel you will use on specific timer. This parameter can be a value of @ref LVQ_PWM_Channel_t
 * @param  Pulse: Pulse, to be set PWM duty cycle
 * @retval Member of @ref LVQ_PWM_Result_t
 */
LVQ_PWM_Result_t LVQ_PWM_SetDuty(LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel, uint32_t Pulse);

/**
 * @brief  Sets PWM value for specific timer and channel with percentage feature
 * @param  *TIM_Data: Pointer to struct with already initialized timer for PWM
 * @param  Channel: Select channel you will use on specific timer. This parameter can be a value of @ref LVQ_PWM_Channel_t
 * @param  percent: Percentage from 0 to 100, to set PWM duty cycle
 * @retval Member of @ref LVQ_PWM_Result_t
 */
LVQ_PWM_Result_t LVQ_PWM_SetDutyPercent(LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel, float percent);

LVQ_PWM_Result_t LVQ_PWM_SetDutyMicros(LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel, uint32_t micros);

LVQ_TIMER_Result_t LVQ_TIMER_CreateDataForPWMFrequency(LVQ_TIMER_PROPERTIES_t* Timer_Data, double PWMFrequency);

LVQ_TIMER_Result_t LVQ_TIMER_GetTimerProperties(TIM_TypeDef* TIMx, LVQ_TIMER_PROPERTIES_t* Timer_Data);

LVQ_TIMER_Result_t LVQ_TIMER_EnableClock(TIM_TypeDef* TIMx);

LVQ_PWM_Result_t LVQ_PWM_SetFrequency(TIM_TypeDef* TIMx, LVQ_PWM_TIM_t* TIM_Data, LVQ_PWM_Channel_t Channel, double PWMFrequency);
/**
 * @}
 */

#endif
