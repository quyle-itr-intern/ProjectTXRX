/**
  ******************************************************************************

  BUTTONS For SLVQ32F446RE
  Author:   LVQ
  Updated:  26st June 2022

  ******************************************************************************
*/

#ifndef __BUTTON_H
#define __BUTTON_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_delay.h"

/**
 * @defgroup LVQ_BUTTON_Macros
 * @brief    Library defines
 * @{
 */

/* Number of maximal supported buttons */
#ifndef BUTTON_MAX_BUTTONS
#define BUTTON_MAX_BUTTONS        10
#endif

/* Number of milliseconds for normal press detection */
#ifndef BUTTON_NORMAL_PRESS_TIME
#define BUTTON_NORMAL_PRESS_TIME  1000
#endif

/* Number of milliseconds for long press detection */
#ifndef BUTTON_LONG_PRESS_TIME
#define BUTTON_LONG_PRESS_TIME    2000
#endif

/**
 * @}
 */

/**
 * @defgroup LVQ_BUTTON_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Button possible press types
 */
typedef enum {
    LVQ_BUTTON_PressType_OnPressed = 0x00, /*!< Button pressed */
    LVQ_BUTTON_PressType_Normal,           /*!< Normal press type, released */
    LVQ_BUTTON_PressType_Long              /*!< Long press type */
} LVQ_BUTTON_PressType_t;

/**
 * @brief  Button private structure
 */
typedef struct _LVQ_BUTTON_t {
    GPIO_TypeDef* GPIOx;                          /*!< GPIOx PORT for button */
    uint16_t GPIO_Pin;                            /*!< GPIO pin for button */
    uint8_t GPIO_State;                           /*!< GPIO state for pin when pressed */
    void (*ButtonHandler)(LVQ_BUTTON_PressType_t); /*!< Button function handler */
    uint32_t StartTime;                           /*!< Time when button was pressed */
    uint8_t LastStatus;                           /*!< Button status on last check */
    uint8_t State;                                /*!< Current button state */
    uint16_t PressNormalTime;                     /*!< Time in ms for normal press for button */
    uint16_t PressLongTime;                       /*!< Time in ms for long press for button */
} LVQ_BUTTON_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_BUTTON_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes a new button to library
 * @note   This library uses @ref malloc() to allocate memory, so make sure you have enough heap memory
 * @param  *GPIOx: Pointer to GPIOx where button is located
 * @param  GPIO_Pin: GPIO pin where button is located
 * @param  ButtonState: Button state when it is pressed.
 *            - 0: Button is low when pressed
 *            - > 0: Button is high when pressed
 * @param  *ButtonHandler:
 * @retval Button creation status:
 *            - 0: Button was not created
 *            - > 0: Button created and saved to library, button pointer is returned
 */
LVQ_BUTTON_t* LVQ_BUTTON_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t ButtonState, void (*ButtonHandler)(LVQ_BUTTON_PressType_t));

/**
 * @brief  Sets press timing values
 * @param  *ButtonStruct: Pointer to @ref LVQ_BUTTON_t structure for button used
 * @param  Normal: Time that button must be pressed to indicate normal press. Value is in milliseconds
 * @param  Normal: Time that button must be pressed to indicate long press. Value is in milliseconds
 * @retval Pointer to @ref LVQ_BUTTON_t
 */
LVQ_BUTTON_t* LVQ_BUTTON_SetPressTime(LVQ_BUTTON_t* ButtonStruct, uint16_t Normal, uint16_t Long);

/**
 * @brief  Updates buttons. This function have to be called periodically
 * @note   Function will automatically call callback functions for buttons
 * @param  None
 * @retval None
 */
void LVQ_BUTTON_Update(void);

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
