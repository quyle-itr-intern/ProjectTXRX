/**
  ******************************************************************************

  I2C Setup For STM32F446RE
  Author:   LVQ
  Updated:  17st June 2022

  ******************************************************************************
*/

#ifndef __I2C_H
#define __I2C_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_delay.h"
#include "lvq_stm32f4_gpio.h"
#include <stdint.h>

/*
Pinout
       |PINSPACK 1   |PINSPACK 2   |PINSPACK 3
I2CX   |SCL   SDA    |SCL   SDA    |SCL   SDA
       |             |             |
I2C1   |PB6   PB7    |PB8   PB9    |PB6   PB9
I2C2   |PB10  PC12   |PB10  PB3    |
I2C3   |PA8   PC9    |PA8   PB4    |-     -
*/

/**
 * @defgroup LVQ_I2C_Macros
 * @brief    Library defines
 * @{
 */
 
 /* F405/407/415/417/F446 */
#if defined (STM32F401xx) || defined(STM32F446xx)
#define USE_I2C1
#define USE_I2C2
#define USE_I2C3
#endif

/**
 * @brief  Timeout for I2C
 */
#ifndef LVQ_I2C_TIMEOUT
		#define LVQ_I2C_TIMEOUT                  5000
#endif

#if defined (STM32F401xx)
		#ifndef LVQ_I2C_CLOCK_FREQUENCY
				#define LVQ_I2C_CLOCK_FREQUENCY          42000000 /* Hz */
		#endif
#elif defined (STM32F40_41xxx) || defined(STM32F446xx)
		#ifndef LVQ_I2C_CLOCK_FREQUENCY
				#define LVQ_I2C_CLOCK_FREQUENCY          45000000 /* Hz */
		#endif
#endif

/* I2C1 settings, change them in defines.h project file */
#ifndef LVQ_I2C1_ACKNOWLEDGED_ADDRESS
#define LVQ_I2C1_ACKNOWLEDGED_ADDRESS    I2C_AcknowledgedAddress_7bit
#endif
#ifndef LVQ_I2C1_MODE
#define LVQ_I2C1_MODE                    I2C_Mode_I2C
#endif
#ifndef LVQ_I2C1_OWN_ADDRESS
#define LVQ_I2C1_OWN_ADDRESS             0x00
#endif
#ifndef LVQ_I2C1_ACK
#define LVQ_I2C1_ACK                     I2C_Ack_Disable
#endif
#ifndef LVQ_I2C1_DUTY_CYCLE
#define LVQ_I2C1_DUTY_CYCLE              I2C_DutyCycle_16_9
#endif

/* I2C2 settings, change them in defines.h project file */
#ifndef LVQ_I2C2_ACKNOWLEDGED_ADDRESS
#define LVQ_I2C2_ACKNOWLEDGED_ADDRESS    I2C_AcknowledgedAddress_7bit
#endif
#ifndef LVQ_I2C2_MODE
#define LVQ_I2C2_MODE                    I2C_Mode_I2C
#endif
#ifndef LVQ_I2C2_OWN_ADDRESS
#define LVQ_I2C2_OWN_ADDRESS             0x00
#endif
#ifndef LVQ_I2C2_ACK
#define LVQ_I2C2_ACK                     I2C_Ack_Disable
#endif
#ifndef LVQ_I2C2_DUTY_CYCLE
#define LVQ_I2C2_DUTY_CYCLE              I2C_DutyCycle_2
#endif

/* I2C3 settings, change them in defines.h project file */
#ifndef LVQ_I2C3_ACKNOWLEDGED_ADDRESS
#define LVQ_I2C3_ACKNOWLEDGED_ADDRESS    I2C_AcknowledgedAddress_7bit
#endif
#ifndef LVQ_I2C3_MODE
#define LVQ_I2C3_MODE                    I2C_Mode_I2C
#endif
#ifndef LVQ_I2C3_OWN_ADDRESS
#define LVQ_I2C3_OWN_ADDRESS             0x00
#endif
#ifndef LVQ_I2C3_ACK
#define LVQ_I2C3_ACK                     I2C_Ack_Disable
#endif
#ifndef LVQ_I2C3_DUTY_CYCLE
#define LVQ_I2C3_DUTY_CYCLE              I2C_DutyCycle_2
#endif

#define LVQ_I2C_CLOCK_STANDARD           100000  /*!< I2C Standard speed */
#define LVQ_I2C_CLOCK_FAST_MODE          400000  /*!< I2C Fast mode speed */
#define LVQ_I2C_CLOCK_FAST_MODE_PLUS     1000000 /*!< I2C Fast mode plus speed */
#define LVQ_I2C_CLOCK_HIGH_SPEED         3400000 /*!< I2C High speed */

/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0

/**
* @}
*/

/**
 * @defgroup LVQ_I2C_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  I2C pinspack enumeration
 */
typedef enum {
    LVQ_I2C_PinsPack_1,      /*!< Use Pinspack1 from Pinout table for I2Cx */
    LVQ_I2C_PinsPack_2,      /*!< Use Pinspack2 from Pinout table for I2Cx */
    LVQ_I2C_PinsPack_3,      /*!< Use Pinspack3 from Pinout table for I2Cx */
    LVQ_I2C_PinsPack_Custom  /*!< Use custom pins for I2Cx */
} LVQ_I2C_PinsPack_t;


/**
 * @}
 */

/**
 * @defgroup LVQ_I2C_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes I2C
 * @param  *I2Cx: I2C used
 * @param  pinspack: Pins used. This parameter can have a value of @ref LVQ_I2C_PinsPack_t enumeration
 * @param  clockSpeed: Clock speed for SCL in Hertz
 * @retval None
 */
void LVQ_I2C_Init(I2C_TypeDef* I2Cx, LVQ_I2C_PinsPack_t pinspack, uint32_t clockSpeed);

/**
 * @brief  Reads single byte from slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to read from
 * @retval Data from slave
 */
uint8_t LVQ_I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg);

/**
 * @brief  Reads multi bytes from slave
 * @param  *I2Cx: I2C used
 * @param  uint8_t address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  uint8_t reg: register to read from
 * @param  uint8_t *data: pointer to data array to store data from slave
 * @param  uint8_t count: how many bytes will be read
 * @retval None
 */
void LVQ_I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count);

/**
 * @brief  Reads byte from slave without specify register address
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @retval Data from slave
 */
uint8_t LVQ_I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t address);

/**
 * @brief  Reads multi bytes from slave without setting register from where to start read
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  *data: pointer to data array to store data from slave
 * @param  count: how many bytes will be read
 * @retval None
 */
void LVQ_I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count);

/**
 * @brief  Writes single byte to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  data: data to be written
 * @retval None
 */
void LVQ_I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data);

/**
 * @brief  Writes multi bytes to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  *data: pointer to data array to write it to slave
 * @param  count: how many bytes will be written
 * @retval None
 */
void LVQ_I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count);

/**
 * @brief  Writes byte to slave without specify register address
 *
 *         Useful if you have I2C device to read like that:
 *            - I2C START
 *            - SEND DEVICE ADDRESS
 *            - SEND DATA BYTE
 *            - I2C STOP
 *
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  data: data byte which will be send to device
 * @retval None
 */
void LVQ_I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data);

/**
 * @brief  Writes multi bytes to slave without setting register from where to start write
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  *data: pointer to data array to write data to slave
 * @param  count: how many bytes you want to write
 * @retval None
 */
void LVQ_I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count);

/**
 * @brief  Checks if device is connected to I2C bus
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @retval Device status:
 *            - 0: Device is not connected
 *            - > 0: Device is connected
 */
uint8_t LVQ_I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t address);

/**
 * @brief  I2C Start condition
 * @param  *I2Cx: I2C used
 * @param  address: slave address
 * @param  direction: master to slave or slave to master
 * @param  ack: ack enabled or disabled
 * @retval Start condition status
 * @note   For private use
 */
int16_t LVQ_I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack);

/**
 * @brief  Stop condition on I2C
 * @param  *I2Cx: I2C used
 * @retval Stop condition status
 * @note   For private use
 */
uint8_t LVQ_I2C_Stop(I2C_TypeDef* I2Cx);

/**
 * @brief  Reads byte without ack
 * @param  *I2Cx: I2C used
 * @retval Byte from slave
 * @note   For private use
 */
uint8_t LVQ_I2C_ReadNack(I2C_TypeDef* I2Cx);

/**
 * @brief  Reads byte with ack
 * @param  *I2Cx: I2C used
 * @retval Byte from slave
 * @note   For private use
 */
uint8_t LVQ_I2C_ReadAck(I2C_TypeDef* I2Cx);

/**
 * @brief  Writes to slave
 * @param  *I2Cx: I2C used
 * @param  data: data to be sent
 * @retval None
 * @note   For private use
 */
void LVQ_I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);

/**
 * @brief  Callback for custom pins initialization.
 *
 *         When you call LVQ_I2C_Init() function, and if you pass LVQ_I2C_PinsPack_Custom to function,
 *         then this function will be called where you can initialize custom pins for I2C peripheral.
 * @param  *I2Cx: I2C for which initialization will be set
 * @param  AlternateFunction: Alternate function which should be used for GPIO initialization
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void LVQ_I2C_InitCustomPinsCallback(I2C_TypeDef* I2Cx, uint16_t AlternateFunction);

void LVQ_I2C_WriteMultiReg16(I2C_TypeDef* I2Cx, uint8_t address, uint16_t reg, uint8_t* data, uint16_t count);
void LVQ_I2C_ReadMultiReg16(I2C_TypeDef* I2Cx, uint8_t address, uint16_t reg, uint8_t* data, uint16_t count);

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
