#ifndef __SPI_H
#define __SPI_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif
/*	
        |PINS PACK 1            |PINS PACK 2            |PINS PACK 3
SPIX    |MOSI   MISO    SCK     |MOSI   MISO    SCK     |MOSI   MISO    SCK
        |
SPI1    |PA7    PA6     PA5     |PB5    PB4     PB3     |
SPI2    |PC3    PC2     PB10    |PB15   PB14    PB13    |PI3    PI2     PI0

*/

#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "lvq_stm32f1_gpio.h"

/**
 * @defgroup LVQ_SPI_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  SPI modes selection
 */
typedef enum {
    LVQ_SPI_Mode_0, /*!< Clock polarity low, clock phase 1st edge */
    LVQ_SPI_Mode_1, /*!< Clock polarity low, clock phase 2nd edge */
    LVQ_SPI_Mode_2, /*!< Clock polarity high, clock phase 1st edge */
    LVQ_SPI_Mode_3  /*!< Clock polarity high, clock phase 2nd edge */
} LVQ_SPI_Mode_t;

/**
 * @brief  SPI PinsPack enumeration to select pins combination for SPI
 */
typedef enum {
    LVQ_SPI_PinsPack_1,       /*!< Select PinsPack1 from Pinout table for specific SPI */
    LVQ_SPI_PinsPack_2,       /*!< Select PinsPack2 from Pinout table for specific SPI */
    LVQ_SPI_PinsPack_3,       /*!< Select PinsPack3 from Pinout table for specific SPI */
    LVQ_SPI_PinsPack_Custom   /*!< Select custom pins for specific SPI, callback will be called, look @ref TM_SPI_InitCustomPinsCallback */
} LVQ_SPI_PinsPack_t;

/**
 * @brief  Daza size enumeration
 */
typedef enum {
    LVQ_SPI_DataSize_8b, /*!< SPI in 8-bits mode */
    LVQ_SPI_DataSize_16b /*!< SPI in 16-bits mode */
} LVQ_SPI_DataSize_t;

/* Define custom pin SPI */
#define DEFINE_SPI1_CUSTOM_1 1
#define DEFINE_SPI1_CUSTOM_2 0
/**
 * @}
 */

/**
* @defgroup LVQ_SPI_Macros
* @brief    Library defines
* @{
*/

/**
 * @brief  Supported SPI modules
 */
#define USE_SPI1
#define USE_SPI2

//----- SPI1 options start -------
//Options can be overwriten in defines.h file
#ifndef LVQ_SPI1_PRESCALER
#define LVQ_SPI1_PRESCALER   SPI_BaudRatePrescaler_8
#endif
//Specify datasize
#ifndef LVQ_SPI1_DATASIZE
#define LVQ_SPI1_DATASIZE    SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef LVQ_SPI1_FIRSTBIT
#define LVQ_SPI1_FIRSTBIT    SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef LVQ_SPI1_MASTERSLAVE
#define LVQ_SPI1_MASTERSLAVE SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef LVQ_SPI1_MODE
#define LVQ_SPI1_MODE        LVQ_SPI_Mode_0
#endif
//----- SPI1 options end -------

//----- SPI2 options start -------
//Options can be overwriten in defines.h file
#ifndef LVQ_SPI2_PRESCALER
#define LVQ_SPI2_PRESCALER   SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef LVQ_SPI2_DATASIZE
#define LVQ_SPI2_DATASIZE    SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef LVQ_SPI2_FIRSTBIT
#define LVQ_SPI2_FIRSTBIT    SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef LVQ_SPI2_MASTERSLAVE
#define LVQ_SPI2_MASTERSLAVE SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef LVQ_SPI2_MODE
#define LVQ_SPI2_MODE        LVQ_SPI_Mode_0
#endif
//----- SPI2 options end -------

//----- SPI3 options start -------
//Options can be overwriten in defines.h file
#ifndef LVQ_SPI3_PRESCALER
#define LVQ_SPI3_PRESCALER   SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef LVQ_SPI3_DATASIZE
#define LVQ_SPI3_DATASIZE    SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef LVQ_SPI3_FIRSTBIT
#define LVQ_SPI3_FIRSTBIT    SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef LVQ_SPI3_MASTERSLAVE
#define LVQ_SPI3_MASTERSLAVE SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef LVQ_SPI3_MODE
#define LVQ_SPI3_MODE        LVQ_SPI_Mode_0
#endif
//----- SPI3 options end -------

//----- SPI4 options start -------
//Options can be overwriten in defines.h file
#ifndef LVQ_SPI4_PRESCALER
#define LVQ_SPI4_PRESCALER   SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef LVQ_SPI4_DATASIZE
#define LVQ_SPI4_DATASIZE    SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef LVQ_SPI4_FIRSTBIT
#define LVQ_SPI4_FIRSTBIT    SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef LVQ_SPI4_MASTERSLAVE
#define LVQ_SPI4_MASTERSLAVE SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef LVQ_SPI4_MODE
#define LVQ_SPI4_MODE        LVQ_SPI_Mode_0
#endif
//----- SPI4 options end -------

//----- SPI5 options start -------
//Options can be overwriten in defines.h file
#ifndef LVQ_SPI5_PRESCALER
#define LVQ_SPI5_PRESCALER   SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef LVQ_SPI5_DATASIZE
#define LVQ_SPI5_DATASIZE    SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef LVQ_SPI5_FIRSTBIT
#define LVQ_SPI5_FIRSTBIT    SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef LVQ_SPI5_MASTERSLAVE
#define LVQ_SPI5_MASTERSLAVE SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef LVQ_SPI5_MODE
#define LVQ_SPI5_MODE        LVQ_SPI_Mode_0
#endif
//----- SPI5 options end -------

//----- SPI6 options start -------
//Options can be overwriten in defines.h file
#ifndef LVQ_SPI6_PRESCALER
#define LVQ_SPI6_PRESCALER   SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef LVQ_SPI6_DATASIZE
#define LVQ_SPI6_DATASIZE    SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef LVQ_SPI6_FIRSTBIT
#define LVQ_SPI6_FIRSTBIT    SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef LVQ_SPI6_MASTERSLAVE
#define LVQ_SPI6_MASTERSLAVE SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef LVQ_SPI6_MODE
#define LVQ_SPI6_MODE        LVQ_SPI_Mode_0
#endif
//----- SPI6 options end -------

/**
 * @brief  Check SPI busy status
 */
#define SPI_IS_BUSY(SPIx) (((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))

/**
 * @brief  SPI wait till end
 */
#define SPI_WAIT(SPIx)            while (SPI_IS_BUSY(SPIx))

/**
 * @brief  Checks if SPI is enabled
 */
#define SPI_CHECK_ENABLED(SPIx)   if (!((SPIx)->CR1 & SPI_CR1_SPE)) {return;}

/**
 * @brief  Checks if SPI is enabled and returns value from function if not
 */
#define SPI_CHECK_ENABLED_RESP(SPIx, val)   if (!((SPIx)->CR1 & SPI_CR1_SPE)) {return (val);}

/**
 * @}
 */

/**
 * @defgroup LVQ_SPI_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes SPIx peripheral with custom pinspack and default other settings
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  pinspack: Pinspack you will use from default SPI table. This parameter can be a value of @ref LVQ_SPI_PinsPack_t enumeration
 * @retval None
 */
void LVQ_SPI_Init(SPI_TypeDef* SPIx, LVQ_SPI_PinsPack_t pinspack);

/**
 * @brief  Initializes SPIx peripheral with custom pinspack and SPI mode and default other settings
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  pinspack: Pinspack you will use from default SPI table. This parameter can be a value of @ref LVQ_SPI_PinsPack_t enumeration
 * @param  SPI_Mode: SPI mode you will use. This parameter can be a value of @ref LVQ_SPI_Mode_t enumeration
 * @retval None
 */
void LVQ_SPI_InitWithMode(SPI_TypeDef* SPIx, LVQ_SPI_PinsPack_t pinspack, LVQ_SPI_Mode_t SPI_Mode);

/**
 * @brief  Initializes SPIx peripheral with custom settings
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  pinspack: Pinspack you will use from default SPI table. This parameter can be a value of @ref LVQ_SPI_PinsPack_t enumeration
 * @param  SPI_BaudRatePrescaler: SPI baudrate prescaler. This parameter can be a value of @ref SPI_BaudRatePrescaler
 * @param  SPI_Mode_t: SPI mode you will use. This parameter can be a value of @ref LVQ_SPI_Mode_t enumeration
 * @param  SPI_Mode: SPI mode you will use:
 *            - SPI_Mode_Master: SPI in master mode (default)
 *            - SPI_Mode_Slave: SPI in slave mode
 * @param  SPI_FirstBit: select first bit for SPI
 *            - SPI_FirstBit_MSB: MSB is first bit (default)
 *            - SPI_FirstBit_LSB: LSB is first bit
 * @retval None
 */
void LVQ_SPI_InitFull(SPI_TypeDef* SPIx, LVQ_SPI_PinsPack_t pinspack, uint16_t SPI_BaudRatePrescaler, LVQ_SPI_Mode_t SPI_Mode_t, uint16_t SPI_Mode, uint16_t SPI_FirstBit);

/**
 * @brief  Calculates bits for SPI prescaler register to get minimal prescaler value for SPI peripheral
 * @note   SPI has 8 prescalers available, 2,4,6,...,128,256
 * @note   This function will return you a bits you must set in your CR1 register.
 *
 * @note   Imagine, you can use 20MHz max clock in your system, your system is running on 168MHz, and you use SPI on APB2 bus.
 *         On 168 and 180MHz devices, APB2 works on Fclk/2, so 84 and 90MHz.
 *         So, if you calculate this, prescaler will need to be 84MHz / 20MHz = 4.xx, but if you use 4 prescaler, then you will be over 20MHz.
 *         You need 8 prescaler then. This function will calculate this.
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6.
 *            Different SPIx works on different clock and is important to know for which SPI you need prescaler.
 * @param  MAX_SPI_Frequency: Max SPI frequency you can use. Function will calculate the minimum prescaler you need for that.
 *
 * @retval Bits combination for SPI_CR1 register, with aligned location already, prepared to set parameter for @ref TM_SPI_InitFull() function.
 */
uint16_t LVQ_SPI_GetPrescalerFromMaxFrequency(SPI_TypeDef* SPIx, uint32_t MAX_SPI_Frequency);

/**
 * @brief  Sets data size for SPI at runtime
 * @note   You can select either 8 or 16 bits data array.
 * @param  *SPIx: Pointer to SPIx peripheral where data size will be set
 * @param  DataSize: Datasize which will be used. This parameter can be a value of @ref TM_SPI_DataSize_t enumeration
 * @retval Status of data size before changes happen
 */
LVQ_SPI_DataSize_t LVQ_SPI_SetDataSize(SPI_TypeDef* SPIx, LVQ_SPI_DataSize_t DataSize);

/**
 * @brief  Sends single byte over SPI
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  data: 8-bit data size to send over SPI
 * @retval Received byte from slave device
 */
static __INLINE uint8_t LVQ_SPI_Send(SPI_TypeDef* SPIx, uint8_t data) {
    /* Check if SPI is enabled */
    SPI_CHECK_ENABLED_RESP(SPIx, 0)

    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);

    /* Fill output buffer with data */
    SPIx->DR = data;

    /* Wait for transmission to complete */
    SPI_WAIT(SPIx);

    /* Return data from buffer */
    return (uint8_t) SPIx->DR;
}

/**
 * @brief  Sends and receives multiple bytes over SPIx
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  *dataOut: Pointer to array with data to send over SPI
 * @param  *dataIn: Pointer to array to to save incoming data
 * @param  count: Number of bytes to send/receive over SPI
 * @retval None
 */
void LVQ_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint32_t count);

/**
 * @brief  Writes multiple bytes over SPI
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  *dataOut: Pointer to array with data to send over SPI
 * @param  count: Number of elements to send over SPI
 * @retval None
 */
void LVQ_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint32_t count);

/**
 * @brief  Receives multiple data bytes over SPI
 * @note   Selected SPI must be set in 16-bit mode
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  *dataIn: Pointer to 8-bit array to save data into
 * @param  dummy: Dummy byte  to be sent over SPI, to receive data back. In most cases 0x00 or 0xFF
 * @param  count: Number of bytes you want read from device
 * @retval None
 */
void LVQ_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint32_t count);

/**
 * @brief  Sends single byte over SPI
 * @note   Selected SPI must be set in 16-bit mode
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  data: 16-bit data size to send over SPI
 * @retval Received 16-bit value from slave device
 */
static __INLINE uint16_t LVQ_SPI_Send16(SPI_TypeDef* SPIx, uint8_t data) {
    /* Check if SPI is enabled */
    SPI_CHECK_ENABLED_RESP(SPIx, 0)

    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);

    /* Fill output buffer with data */
    SPIx->DR = data;

    /* Wait for transmission to complete */
    SPI_WAIT(SPIx);

    /* Return data from buffer */
    return SPIx->DR;
}

/**
 * @brief  Sends and receives multiple bytes over SPIx in 16-bit SPI mode
 * @note   Selected SPI must be set in 16-bit mode
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  *dataOut: Pointer to array with data to send over SPI
 * @param  *dataIn: Pointer to array to to save incoming data
 * @param  count: Number of 16-bit values to send/receive over SPI
 * @retval None
 */
void LVQ_SPI_SendMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t* dataIn, uint32_t count);

/**
 * @brief  Writes multiple data via SPI in 16-bit SPI mode
 * @note   Selected SPI must be set in 16-bit mode
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  *dataOut: Pointer to 16-bit array with data to send over SPI
 * @param  count: Number of elements to send over SPI
 * @retval None
 */
void LVQ_SPI_WriteMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint32_t count);

/**
 * @brief  Receives multiple data bytes over SPI in 16-bit SPI mode
 * @note   Selected SPI must be set in 16-bit mode
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  *dataIn: Pointer to 16-bit array to save data into
 * @param  dummy: Dummy 16-bit value to be sent over SPI, to receive data back. In most cases 0x00 or 0xFF
 * @param  count: Number of 16-bit values you want read from device
 * @retval None
 */
void LVQ_SPI_ReadMulti16(SPI_TypeDef* SPIx, uint16_t* dataIn, uint16_t dummy, uint32_t count);

/**
 * @brief  Init custom SPI pins for your SPIx. This is callback function and will be called from my library if needed.
 * @note   When you call TM_SPI_Init() function, and if you pass TM_SPI_PinsPack_Custom to function,
 *         then this function will be called where you can initialize custom pins for SPI peripheral
 *
 * @note   You have to initialize MOSI, MISO and SCK pin
 *
 * @param  *SPIx: Pointer to SPIx peripheral for which you have to set your custom pin settings
 * @param  AlternateFunction: Alternate function which should be used for GPIO initialization
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
 void LVQ_SPI_InitCustomPinsCallback(SPI_TypeDef* SPIx, uint16_t AlternateFunction);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
