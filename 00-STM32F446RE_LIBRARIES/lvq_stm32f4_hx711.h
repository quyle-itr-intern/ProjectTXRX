/**
  ******************************************************************************

  HX711 For STM32F446RE
  Author:   LVQ
  Updated:  11st September 2022

  ******************************************************************************
*/

#ifndef __HX711_H
#define __HX711_H

/**
 * @addtogroup LVQ_STM32F4xx_Libraries
 * @{
 */

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_pwm.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_delay.h"

/* Define data pin HX711 */
#define LVQ_HX711_DATA_PIN 					  GPIO_PIN_0

/* Define data gpio HX711 */
#define LVQ_HX711_DATA_GPIO 					GPIOA

/* Define clock pin HX711 */
#define LVQ_HX711_CLOCK_PIN 					GPIO_PIN_1

/* Define clock gpio HX711 */
#define LVQ_HX711_CLOCK_GPIO 					GPIOA

#define LVQ_HX711_CLOCK_HIGH          LVQ_GPIO_SetPinHigh(LVQ_HX711_CLOCK_GPIO, LVQ_HX711_CLOCK_PIN)
#define LVQ_HX711_CLOCK_LOW          	LVQ_GPIO_SetPinLow(LVQ_HX711_CLOCK_GPIO, LVQ_HX711_CLOCK_PIN)
#define LVQ_HX711_DATA_VALUE          LVQ_GPIO_GetInputPinValue(LVQ_HX711_DATA_GPIO, LVQ_HX711_DATA_PIN)

/**
 * @defgroup LVQ_ROTARY_ENCODER_Typedefs
 * @brief    Library Typedefs
 * @{
 */
 
typedef struct {
		float offset;
		float coef;	
} LVQ_HX711_DaTa_t;


void LVQ_HX711_Init(void);
uint32_t LVQ_HX711_Read(void);
uint32_t LVQ_HX711_Read_Ave(uint16_t u16Sample);

float LVQ_HX711_Weight(LVQ_HX711_DaTa_t *HX711, uint16_t u16Sample);
void LVQ_HX711_Tare(LVQ_HX711_DaTa_t *HX711, uint16_t u16Sample);
void LVQ_HX711_Coef_Set(LVQ_HX711_DaTa_t *HX711, float coef);
void LVQ_HX711_Calibration(LVQ_HX711_DaTa_t *HX711, float weight);


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
