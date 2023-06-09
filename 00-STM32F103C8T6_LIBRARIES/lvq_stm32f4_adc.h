#ifndef __ADC_H
#define __ADC_H 

/**
 * @addtogroup LVQ_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup LVQ_ADC
 
CHANNEL   ADC1   ADC2   ADC3

0         PA0    PA0    PA0
1         PA1    PA1    PA1
2         PA2    PA2    PA2
3         PA3    PA3    PA3
4         PA4    PA4    PF6
5         PA5    PA5    PF7
6         PA6    PA6    PF8
7         PA7    PA7    PF9
8         PB0    PB0    PF10
9         PB1    PB1    PF3
10        PC0    PC0    PC0
11        PC1    PC1    PC1
12        PC2    PC2    PC2
13        PC3    PC3    PC3
14        PC4    PC4    PF4
15        PC5    PC5    PF5
@endverbatim
 */
#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"

/**
 * @defgroup LVQ_ADC_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  Default resolution for ADC1
 */
#ifndef LVQ_ADC1_RESOLUTION
#define LVQ_ADC1_RESOLUTION      ADC_Resolution_12b
#endif

/**
 * @brief  Default resolution for ADC2
 */
#ifndef LVQ_ADC2_RESOLUTION
#define LVQ_ADC2_RESOLUTION      ADC_Resolution_12b
#endif

/**
 * @brief  Default resolution for ADC3
 */
#ifndef LVQ_ADC3_RESOLUTION
#define LVQ_ADC3_RESOLUTION      ADC_Resolution_12b
#endif

/**
 * @brief  Default supply voltage in mV
 */
#ifndef ADC_SUPPLY_VOLTAGE
#define ADC_SUPPLY_VOLTAGE      3300
#endif

/**
 * @brief  Multipliers for VBAT measurement */
#if defined (STM32F40_41xxx)
#define ADC_VBAT_MULTI          2
#endif
#if defined (STM32F427_437xx) || defined (STM32F429_439xx) || defined (STM32F401xx) || defined (STM32F411xE)
#define ADC_VBAT_MULTI          4
#endif

#define ADC_VBAT_MULTI          4

/**
 * @}
 */

/**
 * @defgroup LVQ_ADC_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  ADC available channels
 */
typedef enum {
    LVQ_ADC_Channel_0,  /*!< Operate with ADC channel 0 */
    LVQ_ADC_Channel_1,  /*!< Operate with ADC channel 1 */
    LVQ_ADC_Channel_2,  /*!< Operate with ADC channel 2 */
    LVQ_ADC_Channel_3,  /*!< Operate with ADC channel 3 */
    LVQ_ADC_Channel_4,  /*!< Operate with ADC channel 4 */
    LVQ_ADC_Channel_5,  /*!< Operate with ADC channel 5 */
    LVQ_ADC_Channel_6,  /*!< Operate with ADC channel 6 */
    LVQ_ADC_Channel_7,  /*!< Operate with ADC channel 7 */
    LVQ_ADC_Channel_8,  /*!< Operate with ADC channel 8 */
    LVQ_ADC_Channel_9,  /*!< Operate with ADC channel 9 */
    LVQ_ADC_Channel_10, /*!< Operate with ADC channel 10 */
    LVQ_ADC_Channel_11, /*!< Operate with ADC channel 11 */
    LVQ_ADC_Channel_12, /*!< Operate with ADC channel 12 */
    LVQ_ADC_Channel_13, /*!< Operate with ADC channel 13 */
    LVQ_ADC_Channel_14, /*!< Operate with ADC channel 14 */
    LVQ_ADC_Channel_15, /*!< Operate with ADC channel 15 */
} LVQ_ADC_Channel_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_ADC_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes ADCx peripheral
 * @param  *ADCx: ADCx peripheral to initialize
 * @retval None
 */
void LVQ_ADC_InitADC(ADC_TypeDef* ADCx);

/**
 * @brief  Initializes ADCx with ADCx channel
 * @param  *ADCx: ADCx peripheral to operate with
 * @param  channel: channel for ADCx
 * @retval None
 */
void LVQ_ADC_Init(ADC_TypeDef* ADCx, uint8_t channel);

/**
 * @brief  Reads from ADCx channel
 * @param  *ADCx: ADCx peripheral to operate with
 * @param  channel: channel for ADCx to read from
 * @retval ADC value
 */
uint16_t LVQ_ADC_Read(ADC_TypeDef* ADCx, uint8_t channel);

/**
 * @brief  Enables Vbat channel for ADC
 * @param  None
 * @retval None
 */
void LVQ_ADC_EnableVbat(void);

/**
 * @brief  Disables Vbat channel for ADC
 * @param  None
 * @retval None
 */
void LVQ_ADC_DisableVbat(void);

/**
 * @brief  Reads vbat pin voltage
 * @param  *ADCx: ADCx peripheral to use for Vbat measurement
 * @retval voltage in mV
 */
uint16_t LVQ_ADC_ReadVbat(ADC_TypeDef* ADCx);

/**
 * @}
 */

#endif
