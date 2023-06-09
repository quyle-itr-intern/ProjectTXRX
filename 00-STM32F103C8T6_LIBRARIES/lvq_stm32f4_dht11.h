/**
  ******************************************************************************

  DHT11 For SLVQ32F446RE
  Author:   LVQ
  Updated:  11st August 2022

  ******************************************************************************
*/

#ifndef __DHT11_H
#define __DHT11_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */

/**
 * Includes
 */
#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_delay.h"
#include "lvq_stm32f4_gpio.h"

/**
 * @defgroup LVQ_DHT11_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  Default data pin, overwrite it in defines.h file
 */
#ifndef DHT11_PIN
#define DHT11_PORT             GPIOA
#define DHT11_PIN              GPIO_PIN_1
#endif

/* Pin settings */
#define DHT11_PIN_LOW          LVQ_GPIO_SetPinLow(DHT11_PORT, DHT11_PIN)
#define DHT11_PIN_HIGH         LVQ_GPIO_SetPinHigh(DHT11_PORT, DHT11_PIN)
#define DHT11_PIN_IN           LVQ_GPIO_SetPinAsInput(DHT11_PORT, DHT11_PIN)
#define DHT11_PIN_OUT          LVQ_GPIO_SetPinAsOutput(DHT11_PORT, DHT11_PIN)
#define DHT11_PIN_READ         LVQ_GPIO_GetInputPinValue(DHT11_PORT, DHT11_PIN)

/**
 * @}
 */

/**
 * @defgroup LVQ_DHT11_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * Enumerations
 *
 * There are several different possible results.
 * If LVQ_DHT11_OK is returned from read function then you have valid data.
 */
typedef enum {
    LVQ_DHT11_OK,                       /*!< Data OK */
    LVQ_DHT11_ERROR,                    /*!< An error occurred */
    LVQ_DHT11_CONNECTION_ERROR,         /*!< Device is not connected */
    LVQ_DHT11_WAITHIGH_ERROR,           /*!< Wait high pulse timeout */
    LVQ_DHT11_WAITLOW_ERROR,            /*!< Wait low pulse timeout */
    LVQ_DHT11_WAITHIGH_LOOP_ERROR,      /*!< Loop error for high pulse */
    LVQ_DHT11_WAITLOW_LOOP_ERROR,       /*!< Loop error for low pulse */
    LVQ_DHT11_PARITY_ERROR              /*!< Data read fail */
} LVQ_DHT11_t;

/**
 * @brief  DHT11 main data structure
 */
typedef struct {
    uint16_t Temp; /*!< Temperature in tenths of degrees.
                       If real temperature is 27.3°C, this variable's value is 273 */
    uint16_t Hum; /*!< Humidity in tenths of percent.
                       If real humidity is 55.5%, this variable's value is 555 */
} LVQ_DHT11_Data_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_DHT11_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes DHT11 sensor
 * @param  None
 * @retval LVQ_DHT11_OK
 */
LVQ_DHT11_t LVQ_DHT11_Init(void);

/**
 * @brief  Reads data from sensor
 * @param  *data: Pointer to @ref LVQ_DHT11_Data_t data structure to store data into
 * @retval Data valid:
 *            - LVQ_DHT11_OK: Data valid
 *            - Else: Data not valid
 */
LVQ_DHT11_t LVQ_DHT11_Read(LVQ_DHT11_Data_t* data);

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
