#ifndef __DAC_H
#define __DAC_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
@verbatim
DAC channel     STM32F4xx

DAC1            PA4
DAC2            PA5
*/

/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_rccConfig.h"

/**
 * @defgroup LVQ_DAC_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Select which DAC channel
 */
typedef enum {
    LVQ_DAC1, /*!< DAC channel 1 */
    LVQ_DAC2  /*!< DAC channel 2 */
} LVQ_DAC_Channel_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_DAC_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes DAC channel and it's pin
 * @param  DACx: DAC Channel you will use. This parameter can be a value of @ref LVQ_DAC_Channel_t enumeration
 * @retval None
 */
void LVQ_DAC_Init(LVQ_DAC_Channel_t DACx);

/**
 * @brief  Sets analog value to ADCx
 * @param  DACx: DAC Channel you will use. This parameter can be a value of @ref LVQ_DAC_Channel_t enumeration
 * @param  value: 12-bit unsigned value for 12-bit DAC
 * @retval None
 */
void LVQ_DAC_SetValue(LVQ_DAC_Channel_t DACx, uint16_t value);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
