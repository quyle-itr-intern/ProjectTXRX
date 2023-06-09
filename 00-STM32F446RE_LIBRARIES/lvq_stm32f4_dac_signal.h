/**
  ******************************************************************************

  DAC SIGNAL For STM32F446RE
  Author:   LVQ
  Updated:  13st August 2022

  ******************************************************************************
*/

#ifndef __DAC_SIGNAL_H
#define __DAC_SIGNAL_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_STM32F4xx_Libraries
 * @{
 */

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_pwm.h"
#include "lvq_stm32f4_gpio.h"
#include "stm32f4xx_dac.h"

/**
 * @defgroup LVQ_DAC_SIGNAL_Macros
 * @brief    Library defines
 * @{
 */

/* Signals used */
#define DAC_SIGNAL_SINUS_LENGTH         32
#define DAC_SIGNAL_TRIANGLE_LENGTH      32
#define DAC_SIGNAL_SAWTOOTH_LENGTH      32
#define DAC_SIGNAL_SQUARE_LENGTH        2

/* DAC DMA settings */
#define DAC_SIGNAL_DMA_DAC1_STREAM      DMA1_Stream5
#define DAC_SIGNAL_DMA_DAC1_CHANNEL     DMA_Channel_7
#define DAC_SIGNAL_DMA_DAC2_STREAM      DMA1_Stream6
#define DAC_SIGNAL_DMA_DAC2_CHANNEL     DMA_Channel_7

/**
 * @}
 */

/**
 * @defgroup LVQ_DAC_SIGNAL_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Signal result enumeration
 */
typedef enum {
    LVQ_DAC_SIGNAL_Result_Ok = 0x00,    /*!< Everything OK */
    LVQ_DAC_SIGNAL_Result_Error,        /*!< An error occurred */
    LVQ_DAC_SIGNAL_Result_TimerNotValid /*!< Used timer for DMA and DAC request is not valid */
} LVQ_DAC_SIGNAL_Result_t;

/**
 * @brief  Signals available
 */
typedef enum {
    LVQ_DAC_SIGNAL_Signal_Sinus = 0x00,    /*!< Select sinus signal on DAC output */
    LVQ_DAC_SIGNAL_Signal_Triangle = 0x01, /*!< Select triangle signal on DAC output */
    LVQ_DAC_SIGNAL_Signal_Sawtooth = 0x02, /*!< Select sawtooth signal on DAC output */
    LVQ_DAC_SIGNAL_Signal_Square = 0x03    /*!< Use square signal on DAC output */
} LVQ_DAC_SIGNAL_Signal_t;

/**
 * @brief  DAC used for signal
 */
typedef enum {
    LVQ_SIGNAL_DAC1 = 0x00, /*!< Use DAC1 for specific settings */
    LVQ_SIGNAL_DAC2 = 0x01  /*!< Use DAC2 for specific settings */
} LVQ_DAC_SIGNAL_Channel_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_DAC_SIGNAL_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes DAC output pin and prepare to work
 * @param  DACx: DAC channel you will use. This parameter can be a value of @ref LVQ_DAC_SIGNAL_Channel_t enumeration
 * @param  *TIMx: Pointer to TIMER use for DMA treshold
 *            - TIM2, TIM4, TIM5, TIM6, TIM7, TIM8
 * @retval Member of @ref LVQ_DAC_SIGNAL_Result_t
 */
LVQ_DAC_SIGNAL_Result_t LVQ_DAC_SIGNAL_Init(LVQ_DAC_SIGNAL_Channel_t DACx, TIM_TypeDef* TIMx);

/**
 * @brief  Sets output signal type with specific frequency
 * @param  DACx: DAC channel you will use. This parameter can be a value of @ref LVQ_DAC_SIGNAL_Channel_t enumeration
 * @param  signal_type: Signal type you will set for specific dac channel. This parameter can be a value of @ref LVQ_DAC_SIGNAL_Signal_t enumeration
 * @param  frequency: Signal's frequency
 * @retval Member of @ref LVQ_DAC_SIGNAL_Result_t
 */
LVQ_DAC_SIGNAL_Result_t LVQ_DAC_SIGNAL_SetSignal(LVQ_DAC_SIGNAL_Channel_t DACx, LVQ_DAC_SIGNAL_Signal_t signal_type, double frequency);

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
