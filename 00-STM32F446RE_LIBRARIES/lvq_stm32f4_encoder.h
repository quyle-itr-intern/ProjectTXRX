/**
  ******************************************************************************

  ENCODER For STM32F446RE
  Author:   LVQ
  Updated:  23st August 2022

  ******************************************************************************
*/

#ifndef __ENCODER_H
#define __ENCODER_H

/**
 * @addtogroup LVQ_STM32F4xx_Libraries
 * @{
 */

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_exti.h"

/**
 * @brief  ENCODER_MODE
 */
#define ENCODER_MODE_X2

/**
 * @defgroup LVQ_ROTARY_ENCODER_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Rotary encoder rotation status
 */
typedef enum {
    LVQ_ENCODER_Rotate_Increment, /*!< Encoder was incremented */
    LVQ_ENCODER_Rotate_Decrement, /*!< Encoder was decremented */
    LVQ_ENCODER_Rotate_Nothing    /*!< Encoder stop at it was before */
} LVQ_ENCODER_Rotate_t;

/**
 * @brief  Rotary encoder mode selection for rotation
 */
typedef enum {
    LVQ_ENCODER_Mode_Zero, /*!< Rotary encoder mode zero. It is used for direction when it will be increment od decrement, default used */
    LVQ_ENCODER_Mode_One   /*!< Rotary encoder mode one. It is used for direction when it will be increment od decrement */
} LVQ_ENCODER_Mode_t;

/**
 * @brief  Rotary main working structure
 */
typedef struct {
    int32_t Absolute;        /*!< Absolute rotation from beginning, for public use */
    int32_t Diff;            /*!< Rotary difference from last check, for public use */
    LVQ_ENCODER_Rotate_t Rotation; /*!< Increment, Decrement or nothing, for public use */
    LVQ_ENCODER_Mode_t Mode;       /*!< Rotary encoder mode selected */
    uint8_t LastA;           /*!< Last status of A pin when checking. Meant for private use */
		uint8_t LastB;           /*!< Last status of B pin when checking. Meant for private use */
    int32_t RE_Count;        /*!< Temporary variable to store data between rotation and user check */
    GPIO_TypeDef* GPIO_A;    /*!< Pointer to GPIOx for Rotary encode A pin. Meant for private use */
    GPIO_TypeDef* GPIO_B;    /*!< Pointer to GPIOx for Rotary encode B pin. Meant for private use */
    uint16_t GPIO_PIN_A;     /*!< GPIO pin for rotary encoder A pin. This pin is also set for interrupt */
    uint16_t GPIO_PIN_B;     /*!< GPIO pin for rotary encoder B pin. */
} LVQ_ENCODER_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_ROTARY_ENCODER_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Prepare Rotary Encoder to work
 * @param  *data: Pointer to @ref LVQ_ENCODER_t structure
 * @retval None
 */
void LVQ_ENCODER_Init(LVQ_ENCODER_t* data, GPIO_TypeDef* GPIO_A_Port, uint16_t GPIO_A_Pin, GPIO_TypeDef* GPIO_B_Port, uint16_t GPIO_B_Pin);

/**
 * @brief  Set rotary encoder custom mode
 * @param  *data: Pointer to @ref LVQ_ENCODER_t structure for specific rotary encoder input
 * @param  mode: Rotary mode you will use. This parameter can be a value of @ref LVQ_ENCODER_Mode_t enumeration
 * @retval None
 */
void LVQ_ENCODER_SetMode(LVQ_ENCODER_t* data, LVQ_ENCODER_Mode_t mode);

/**
 * @brief  Checks and gets new values of rotary encoder
 * @param  *data: Pointer to @ref LVQ_ENCODER_t structure
 * @retval Member of @ref LVQ_ENCODER_Rotate_t
 */
LVQ_ENCODER_Rotate_t LVQ_ENCODER_Get(LVQ_ENCODER_t* data);

/**
 * @brief  Process function.
 * @note   This function have to be called inside your interrupt handler.
 * @param  *data: Pointer to rotary encoder @ret LVQ_ENCODER_t data where interrupt occured
 * @retval None
 */
void LVQ_ENCODER_Process(LVQ_ENCODER_t* data);

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
