/**
  ******************************************************************************

  KEYPAD For SLVQ32F446RE
  Author:   LVQ
  Updated:  20st June 2022

  ******************************************************************************
*/

#ifndef __KEYPAD_H
#define __KEYPAD_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */
/**
	
Keypad      SLVQ32F4xx       Description

C1          PD0             Column 1
C2          PD1             Column 2
C3          PD2             Column 3
C4          PD3             Column 4
R1          PC1             Row 1
R2          PC2             Row 2
R3          PC3             Row 3
R4          PC5             Row 4

*/
/**
 * Includes
 */

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_gpio.h"

/**
 * @defgroup LVQ_KEYPAD_Macros
 * @brief    Library defines
 * @{
 */

/* Rows */
/* Row 1 default */
#ifndef KEYPAD_ROW_1_PIN
#define KEYPAD_ROW_1_PORT           GPIOA
#define KEYPAD_ROW_1_PIN            GPIO_PIN_11
#endif
/* Row 2 default */
#ifndef KEYPAD_ROW_2_PIN
#define KEYPAD_ROW_2_PORT           GPIOB
#define KEYPAD_ROW_2_PIN            GPIO_PIN_12
#endif
/* Row 3 default */
#ifndef KEYPAD_ROW_3_PIN
#define KEYPAD_ROW_3_PORT           GPIOB
#define KEYPAD_ROW_3_PIN            GPIO_PIN_2
#endif
/* Row 4 default */
#ifndef KEYPAD_ROW_4_PIN
#define KEYPAD_ROW_4_PORT           GPIOB
#define KEYPAD_ROW_4_PIN            GPIO_PIN_1
#endif

/* Columns */
/* Column 1 default */
#ifndef KEYPAD_COLUMN_1_PIN
#define KEYPAD_COLUMN_1_PORT        GPIOA
#define KEYPAD_COLUMN_1_PIN         GPIO_PIN_12
#endif
/* Column 2 default */
#ifndef KEYPAD_COLUMN_2_PIN
#define KEYPAD_COLUMN_2_PORT        GPIOC
#define KEYPAD_COLUMN_2_PIN         GPIO_PIN_5
#endif
/* Column 3 default */
#ifndef KEYPAD_COLUMN_3_PIN
#define KEYPAD_COLUMN_3_PORT        GPIOC
#define KEYPAD_COLUMN_3_PIN         GPIO_PIN_6
#endif
/* Column 4 default */
#ifndef KEYPAD_COLUMN_4_PIN
#define KEYPAD_COLUMN_4_PORT        GPIOC
#define KEYPAD_COLUMN_4_PIN         GPIO_PIN_8
#endif

/* Number of milliseconds between 2 reads */
#ifndef KEYPAD_READ_INTERVAL
#define KEYPAD_READ_INTERVAL        100
#endif

/* Keypad no pressed */
#define KEYPAD_NO_PRESSED           (uint8_t)0xFF

/**
 * @}
 */

/**
 * @defgroup LVQ_KEYPAD_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Keypad button enumeration
 */
typedef enum {
		LVQ_KEYPAD_Button_0 = 0x00,                     /*!< Button 0 code */
    LVQ_KEYPAD_Button_1 = 0x01,                     /*!< Button 1 code */
    LVQ_KEYPAD_Button_2 = 0x02,                     /*!< Button 2 code */
    LVQ_KEYPAD_Button_3 = 0x03,                     /*!< Button 3 code */
    LVQ_KEYPAD_Button_4 = 0x04,                     /*!< Button 4 code */
    LVQ_KEYPAD_Button_5 = 0x05,                     /*!< Button 5 code */
    LVQ_KEYPAD_Button_6 = 0x06,                     /*!< Button 6 code */
    LVQ_KEYPAD_Button_7 = 0x07,                     /*!< Button 7 code */
    LVQ_KEYPAD_Button_8 = 0x08,                     /*!< Button 8 code */
    LVQ_KEYPAD_Button_9 = 0x09,                     /*!< Button 9 code */
    LVQ_KEYPAD_Button_A = 0x0A,                     /*!< Button A code. Only on large size */
    LVQ_KEYPAD_Button_B = 0x0B,                     /*!< Button B code. Only on large size */
    LVQ_KEYPAD_Button_C = 0x0C,                     /*!< Button C code. Only on large size */
    LVQ_KEYPAD_Button_D = 0x0D,                     /*!< Button D code. Only on large size */
		LVQ_KEYPAD_Button_STAR = 0x0E,                  /*!< Button START code */
    LVQ_KEYPAD_Button_HASH = 0x0F,                  /*!< Button HASH code */
    LVQ_KEYPAD_Button_NOPRESSED = KEYPAD_NO_PRESSED /*!< No button pressed */
} LVQ_KEYPAD_Button_t;

/**
 * @brief  Keypad size enumeration
 */
typedef enum {
    LVQ_KEYPAD_Type_Large = 0x00, /*!< Keypad 4x4 size */
    LVQ_KEYPAD_Type_Small         /*!< Keypad 3x4 size */
} LVQ_KEYPAD_Type_t;

/**
 * @brief  Keypad init status
 */
typedef enum {
    LVQ_KEYPAD_Result_Ok = 0x00, /*!< Keypad 4x4 size */
    LVQ_KEYPAD_Result_Error         /*!< Keypad 3x4 size */
} LVQ_KEYPAD_Result_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_KEYPAD_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes keypad functionality
 * @param  type: Keypad type you will use. This parameter can be a value of @ref LVQ_KEYPAD_Type_t enumeration
 * @retval None
 */
LVQ_KEYPAD_Result_t LVQ_KEYPAD_Init(LVQ_KEYPAD_Type_t type);

/**
 * @brief  Reads keypad data
 * @param  None
 * @retval Button status. This parameter will be a value of @ref LVQ_KEYPAD_Button_t enumeration
 */
LVQ_KEYPAD_Button_t LVQ_KEYPAD_Read(void);

/**
 * @brief  Updates keypad
 * @note   This function must be called from interrupt routine every 1ms
 * @param  None
 * @retval None
 */
void LVQ_KEYPAD_Update(void);
uint8_t LVQ_KEYPAD_INT_Read(void) ;

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

