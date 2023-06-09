/**
  ******************************************************************************

  MCO For STM32F446RE
  Author:   LVQ
  Updated:  16st March 2023

  ******************************************************************************
*/

#ifndef LVQ_MCO_H
#define LVQ_MCO_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup LVQ_MCO
 *
 * MCO1 output
 *  - Pin PA8
 *  - HSI: High Speed Internal clock, 16MHz RC oscillator
 *  - HSE: High Speed External clock, external crystal or user clock
 *  - LSE: External 32738Hz low-speed oscillator or ceramic resonator
 *  - PLLCLK: Output from PLL
 *
 * \par MCO2 output = PC9
 *
 * MCO2 output
 *  - Pin PC9
 *  - SYSCLK: Output system core clock
 *  - PLLI2SCLK: Accurate clock for high-quality audio performance in I2S and SAI interfaces
 *  - HSE: High Speed External clock, external crystal or user clock
 *  - PLLCLK: Output from PLL
 *
 * \par Changelog
 *
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "lvq_stm32f4_gpio.h"


/**
 * @defgroup LVQ_MCO_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Prescaler values for output clock
 */
typedef enum {
    LVQ_MCO_Prescaler_1, /*!< Output original value of clock on MCO pin */
    LVQ_MCO_Prescaler_2, /*!< Output clock is original / 2 */
    LVQ_MCO_Prescaler_3, /*!< Output clock is original / 3 */
    LVQ_MCO_Prescaler_4, /*!< Output clock is original / 4 */
    LVQ_MCO_Prescaler_5  /*!< Output clock is original / 5 */
} LVQ_MCO_Prescaler_t;

/**
 * @brief  Possible output clocks on MCO1 pin
 */
typedef enum {
    LVQ_MCO1_Source_HSI,   /*!< High Speed Internal clock, 16MHz RC oscillator */
    LVQ_MCO1_Source_HSE,   /*!< High Speed External clock, external crystal or user clock */
    LVQ_MCO1_Source_LSE,   /*!< External 32738Hz low-speed oscillator or ceramic resonator */
    LVQ_MCO1_Source_PLLCLK /*!< Output from PLL */
} LVQ_MCO1_Source_t;

/**
 * @brief  Possible output clocks on MCO2 pin
 */
typedef enum {
    LVQ_MCO2_Source_SYSCLK,    /*!< System core clock */
    LVQ_MCO2_Source_PLLI2SCLK, /*!< Accurate clock for high-quality audio performance in I2S and SAI interfaces */
    LVQ_MCO2_Source_HSE,       /*!< High Speed External clock, external crystal or user clock */
    LVQ_MCO2_Source_PLLCLK     /*!< Output from PLL */
} LVQ_MCO2_Source_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_MCO_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes and prepares MCO1 pin to output clock
 * @param  None
 * @retval None
 */
void LVQ_MCO_InitMCO1(void);

/**
 * @brief  Sets output for MCO1 pin
 * @param  Source: Clock source to output. This parameter can be a value of @ref LVQ_MCO1_Source_t enumeration.
 * @param  Prescaler: Prescaler used for clock. This parameter can be a value of @ref LVQ_MCO_Prescaler_t enumeration.
 * @retval None
 */
void LVQ_MCO_SetOutput1(LVQ_MCO1_Source_t Source, LVQ_MCO_Prescaler_t Prescaler);

/**
 * @brief  Initializes and prepares MCO2 pin to output clock
 * @param  None
 * @retval None
 */
void LVQ_MCO_InitMCO2(void);

/**
 * @brief  Sets output for MCO2 pin
 * @param  Source: Clock source to output. This parameter can be a value of @ref LVQ_MCO2_Source_t enumeration.
 * @param  Prescaler: Prescaler used for clock. This parameter can be a value of @ref LVQ_MCO_Prescaler_t enumeration.
 * @retval None
 */
void LVQ_MCO_SetOutput2(LVQ_MCO2_Source_t Source, LVQ_MCO_Prescaler_t Prescaler);

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
