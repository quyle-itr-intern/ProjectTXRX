#ifndef __USART_H
#define __USART_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_defines.h"
#include "lvq_stm32f4_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stdio.h"
#include "string.h"

/*
@verbatim
             |PINSPACK 1     |PINSPACK 2     |PINSPACK 3
U(S)ARTX     |TX     RX      |TX     RX      |TX     RX

USART1       |PA9    PA10    |PB6    PB7     |-      -
USART2       |PA2    PA3     |PD5    PD6     |-      -
USART3       |PB10   PC5     |PC10   PC11    |PD8    PD9
UART4        |PA0    PA1     |PC10   PC11    |-      -
UART5        |PC12   PD2     |-      -       |-      -
USART6       |PC6    PC7     |PG14   PG9     |-      -
UART7        |PE8    PE7     |PF7    PF6     |-      -
UART8        |PE1    PE0     |-      -       |-      -
@endverbatim
*/

#define DEFINE_USART1_CUSTOM_1 1
#define DEFINE_USART1_CUSTOM_2 0

#define DEFINE_USART3_CUSTOM_1 1
#define DEFINE_USART3_CUSTOM_2 0

#define DEFINE_UART4_CUSTOM_1 1
#define DEFINE_UART4_CUSTOM_2 0

/* F405/407/415/417/F446 */
#if defined (STM32F40_41xxx) || defined(STM32F446xx)
#define USE_USART1
#define USE_USART2
#define USE_USART3
#define USE_UART4
#define USE_UART5
#define USE_USART6
#endif
/* F427/429/437/439 */
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
#define USE_USART1
#define USE_USART2
#define USE_USART3
#define USE_UART4
#define USE_UART5
#define USE_USART6
#define USE_UART7
#define USE_UART8
#endif
/* F401/411 */
#if defined (STM32F401xx) || defined(STM32F411xE)
#define USE_USART1
#define USE_USART2
#define USE_USART6
#endif

typedef enum {
    LVQ_USART_PinsPack_1,     /*!< Select PinsPack1 from Pinout table for specific USART */
    LVQ_USART_PinsPack_2,     /*!< Select PinsPack2 from Pinout table for specific USART */
    LVQ_USART_PinsPack_3,     /*!< Select PinsPack3 from Pinout table for specific USART */
    LVQ_USART_PinsPack_Custom /*!< Select custom pins for specific USART, callback will be called, look @ref LVQ_USART_InitCustomPinsCallback */
} LVQ_USART_PinsPack_t;

typedef enum {
    LVQ_USART_HardwareFlowControl_None = 0x0000,   /*!< No flow control */
    LVQ_USART_HardwareFlowControl_RTS = 0x0100,    /*!< RTS flow control */
    LVQ_USART_HardwareFlowControl_CTS = 0x0200,    /*!< CTS flow control */
    LVQ_USART_HardwareFlowControl_RTS_CTS = 0x0300 /*!< RTS and CTS flow control */
} LVQ_USART_HardwareFlowControl_t;

/* Default buffer size for each USART */
#ifndef USART_BUFFER_SIZE
#define USART_BUFFER_SIZE               64
#endif

/* Set default buffer size for specific USART if not set by user */
#ifndef LVQ_USART1_BUFFER_SIZE
#define LVQ_USART1_BUFFER_SIZE           USART_BUFFER_SIZE
#endif
#ifndef LVQ_USART2_BUFFER_SIZE
#define LVQ_USART2_BUFFER_SIZE           USART_BUFFER_SIZE
#endif
#ifndef LVQ_USART3_BUFFER_SIZE
#define LVQ_USART3_BUFFER_SIZE           USART_BUFFER_SIZE
#endif
#ifndef LVQ_UART4_BUFFER_SIZE
#define LVQ_UART4_BUFFER_SIZE            USART_BUFFER_SIZE
#endif
#ifndef LVQ_UART5_BUFFER_SIZE
#define LVQ_UART5_BUFFER_SIZE            USART_BUFFER_SIZE
#endif
#ifndef LVQ_USART6_BUFFER_SIZE
#define LVQ_USART6_BUFFER_SIZE           USART_BUFFER_SIZE
#endif
#ifndef LVQ_UART7_BUFFER_SIZE
#define LVQ_UART7_BUFFER_SIZE            USART_BUFFER_SIZE
#endif
#ifndef LVQ_UART8_BUFFER_SIZE
#define LVQ_UART8_BUFFER_SIZE            USART_BUFFER_SIZE
#endif

/* NVIC Global Priority */
#ifndef USART_NVIC_PRIORITY
#define USART_NVIC_PRIORITY             0x06
#endif

/* U(S)ART settings, can be changed in your defines.h project file */
/* USART1 default settings */
#ifndef LVQ_USART1_HARDWARE_FLOW_CONTROL
#define LVQ_USART1_HARDWARE_FLOW_CONTROL     LVQ_USART_HardwareFlowControl_None
#endif
#ifndef LVQ_USART1_MODE
#define LVQ_USART1_MODE                      USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef LVQ_USART1_PARITY
#define LVQ_USART1_PARITY                    USART_Parity_No
#endif
#ifndef LVQ_USART1_STOP_BITS
#define LVQ_USART1_STOP_BITS                 USART_StopBits_1
#endif
#ifndef LVQ_USART1_WORD_LENGTH
#define LVQ_USART1_WORD_LENGTH               USART_WordLength_8b
#endif

/* USART2 default settings */
#ifndef LVQ_USART2_HARDWARE_FLOW_CONTROL
#define LVQ_USART2_HARDWARE_FLOW_CONTROL     LVQ_USART_HardwareFlowControl_None
#endif
#ifndef LVQ_USART2_MODE
#define LVQ_USART2_MODE                      USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef LVQ_USART2_PARITY
#define LVQ_USART2_PARITY                    USART_Parity_No
#endif
#ifndef LVQ_USART2_STOP_BITS
#define LVQ_USART2_STOP_BITS                 USART_StopBits_1
#endif
#ifndef LVQ_USART2_WORD_LENGTH
#define LVQ_USART2_WORD_LENGTH               USART_WordLength_8b
#endif

/* USART3 default settings */
#ifndef LVQ_USART3_HARDWARE_FLOW_CONTROL
#define LVQ_USART3_HARDWARE_FLOW_CONTROL     LVQ_USART_HardwareFlowControl_None
#endif
#ifndef LVQ_USART3_MODE
#define LVQ_USART3_MODE                      USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef LVQ_USART3_PARITY
#define LVQ_USART3_PARITY                    USART_Parity_No
#endif
#ifndef LVQ_USART3_STOP_BITS
#define LVQ_USART3_STOP_BITS                 USART_StopBits_1
#endif
#ifndef LVQ_USART3_WORD_LENGTH
#define LVQ_USART3_WORD_LENGTH               USART_WordLength_8b
#endif

/* UART4 default settings */
#ifndef LVQ_UART4_HARDWARE_FLOW_CONTROL
#define LVQ_UART4_HARDWARE_FLOW_CONTROL      LVQ_USART_HardwareFlowControl_None
#endif
#ifndef LVQ_UART4_MODE
#define LVQ_UART4_MODE                       USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef LVQ_UART4_PARITY
#define LVQ_UART4_PARITY                     USART_Parity_No
#endif
#ifndef LVQ_UART4_STOP_BITS
#define LVQ_UART4_STOP_BITS                  USART_StopBits_1
#endif
#ifndef LVQ_UART4_WORD_LENGTH
#define LVQ_UART4_WORD_LENGTH                USART_WordLength_8b
#endif

/* UART5 default settings */
#ifndef LVQ_UART5_HARDWARE_FLOW_CONTROL
#define LVQ_UART5_HARDWARE_FLOW_CONTROL      LVQ_USART_HardwareFlowControl_None
#endif
#ifndef LVQ_UART5_MODE
#define LVQ_UART5_MODE                       USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef LVQ_UART5_PARITY
#define LVQ_UART5_PARITY                     USART_Parity_No
#endif
#ifndef LVQ_UART5_STOP_BITS
#define LVQ_UART5_STOP_BITS                  USART_StopBits_1
#endif
#ifndef LVQ_UART5_WORD_LENGTH
#define LVQ_UART5_WORD_LENGTH                USART_WordLength_8b
#endif

/* USART6 default settings */
#ifndef LVQ_USART6_HARDWARE_FLOW_CONTROL
#define LVQ_USART6_HARDWARE_FLOW_CONTROL     LVQ_USART_HardwareFlowControl_None
#endif
#ifndef LVQ_USART6_MODE
#define LVQ_USART6_MODE                      USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef LVQ_USART6_PARITY
#define LVQ_USART6_PARITY                    USART_Parity_No
#endif
#ifndef LVQ_USART6_STOP_BITS
#define LVQ_USART6_STOP_BITS                 USART_StopBits_1
#endif
#ifndef LVQ_USART6_WORD_LENGTH
#define LVQ_USART6_WORD_LENGTH               USART_WordLength_8b
#endif

/* UART7 default settings */
#ifndef LVQ_UART7_HARDWARE_FLOW_CONTROL
#define LVQ_UART7_HARDWARE_FLOW_CONTROL      LVQ_USART_HardwareFlowControl_None
#endif
#ifndef LVQ_UART7_MODE
#define LVQ_UART7_MODE                       USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef LVQ_UART7_PARITY
#define LVQ_UART7_PARITY                     USART_Parity_No
#endif
#ifndef LVQ_UART7_STOP_BITS
#define LVQ_UART7_STOP_BITS                  USART_StopBits_1
#endif
#ifndef LVQ_UART7_WORD_LENGTH
#define LVQ_UART7_WORD_LENGTH                USART_WordLength_8b
#endif

/* UART8 default settings */
#ifndef LVQ_UART8_HARDWARE_FLOW_CONTROL
#define LVQ_UART8_HARDWARE_FLOW_CONTROL      LVQ_USART_HardwareFlowControl_None
#endif
#ifndef LVQ_UART8_MODE
#define LVQ_UART8_MODE                       USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef LVQ_UART8_PARITY
#define LVQ_UART8_PARITY                     USART_Parity_No
#endif
#ifndef LVQ_UART8_STOP_BITS
#define LVQ_UART8_STOP_BITS                  USART_StopBits_1
#endif
#ifndef LVQ_UART8_WORD_LENGTH
#define LVQ_UART8_WORD_LENGTH                USART_WordLength_8b
#endif

/**
 * @brief  Wait till USART finishes transmission
 */
#define USART_TXEMPTY(USARTx)               ((USARTx)->SR & USART_FLAG_TXE)
#define USART_WAIT(USARTx)                  do { while (!USART_TXEMPTY(USARTx)); } while (0)

/**
 * @brief  Default string delimiter for USART
 */
#define USART_STRING_DELIMITER              '\n'

/**
 * @defgroup LVQ_USART_Functions
 * @brief    USART Functions
 * @{
 */

/**
 * @brief  Initializes USARTx peripheral and corresponding pins
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @param  pinspack: This parameter can be a value of @ref LVQ_USART_PinsPack_t enumeration
 * @param  baudrate: Baudrate number for USART communication
 * @retval None
 */
void LVQ_USART_Init(USART_TypeDef* USARTx, LVQ_USART_PinsPack_t pinspack, uint32_t baudrate);

/**
 * @brief  Initializes USARTx peripheral and corresponding pins with custom hardware flow control mode
 * @note   Hardware flow control pins are not initialized. Easy solution is to use @arg TM_USART_PinsPack_Custom pinspack option
 *         when you call @ref TM_USART_Init() function and initialize all USART pins at a time inside @ref TM_USART_InitCustomPinsCallback()
 *         callback function, which will be called from my library
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @param  pinspack: This parameter can be a value of @ref LVQ_USART_PinsPack_t enumeration
 * @param  baudrate: Baudrate number for USART communication
 * @param  FlowControl: Flow control mode you will use. This parameter can be a value of @ref LVQ_USART_HardwareFlowControl_t enumeration
 * @retval None
 */
void LVQ_USART_InitWithFlowControl(USART_TypeDef* USARTx, LVQ_USART_PinsPack_t pinspack, uint32_t baudrate, LVQ_USART_HardwareFlowControl_t FlowControl);

/**
 * @brief  Puts character to USART port
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @param  c: character to be send over USART
 * @retval None
 */
static __INLINE void LVQ_USART_Putc(USART_TypeDef* USARTx, volatile char c) {
    /* Check USART if enabled */
    if ((USARTx->CR1 & USART_CR1_UE)) {
        /* Wait to be ready, buffer empty */
        USART_WAIT(USARTx);
        /* Send data */
        USARTx->DR = (uint16_t)(c & 0x01FF);
        /* Wait to be ready, buffer empty */
        USART_WAIT(USARTx);
    }
}

/**
 * @brief  Puts string to USART port
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @param  *str: Pointer to string to send over USART
 * @retval None
 */
void LVQ_USART_Puts(USART_TypeDef* USARTx, char* str);

/**
 * @brief  Sends data array to USART port
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @param  *DataArray: Pointer to data array to be sent over USART
 * @param  count: Number of elements in data array to be send over USART
 * @retval None
 */
void LVQ_USART_Send(USART_TypeDef* USARTx, uint8_t* DataArray, uint16_t count);

/**
 * @brief  Gets character from internal USART buffer
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @retval Character from buffer, or 0 if nothing in buffer
 */
uint8_t LVQ_USART_Getc(USART_TypeDef* USARTx);

/**
 * @brief  Gets string from USART
 *
 *         This function can create a string from USART received data.
 *
 *         It generates string until "\n" is not recognized or buffer length is full.
 *
 * @note   As of version 1.5, this function automatically adds 0x0A (Line feed) at the end of string.
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @param  *buffer: Pointer to buffer where data will be stored from buffer
 * @param  bufsize: maximal number of characters we can add to your buffer, including leading zero
 * @retval Number of characters in buffer
 */
uint16_t LVQ_USART_Gets(USART_TypeDef* USARTx, char* buffer, uint16_t bufsize);

/**
 * @brief  Checks if character c is available in internal buffer
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @param  c: character to check if it is in USARTx's buffer
 * @retval Character status:
 *            - 0: Character was not found
 *            - > 0: Character has been found in buffer
 */
uint8_t LVQ_USART_FindCharacter(USART_TypeDef* USARTx, uint8_t c);

/**
 * @brief  Checks if internal USARTx buffer is empty
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @retval Buffer empty status:
 *            - 0: Buffer is not empty
 *            - > 0: Buffer is empty
 */
uint8_t LVQ_USART_BufferEmpty(USART_TypeDef* USARTx);

/**
 * @brief  Checks if internal USARTx buffer is full
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @retval Buffer full status:
 *            - 0: Buffer is not full
 *            - > 0: Buffer is full
 */
uint8_t LVQ_USART_BufferFull(USART_TypeDef* USARTx);

/**
 * @brief  Clears internal USART buffer
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @retval None
 */
void LVQ_USART_ClearBuffer(USART_TypeDef* USARTx);

/**
 * @brief  Sets custom character for @ref TM_USART_Gets() function to detect when string ends
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @param  Character: Character value to be used as string end
 * @note   Character will also be added at the end for your buffer when calling @ref TM_USART_Gets() function
 * @retval None
 */
void LVQ_USART_SetCustomStringEndCharacter(USART_TypeDef* USARTx, uint8_t Character);

/**
 * @brief  Callback for custom pins initialization for USARTx.
 *
 *         When you call @ef TM_USART_Init() function, and if you pass @arg TM_USART_PinsPack_Custom to function,
 *         then this function will be called where you can initialize custom pins for USART peripheral.
 * @param  *USARTx: Pointer to USARTx peripheral you will use for initialization
 * @param  AlternateFunction: Alternate function which should be used for GPIO initialization
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void LVQ_USART_InitCustomPinsCallback(USART_TypeDef* USARTx, uint16_t AlternateFunction);

/**
 * @brief  Callback function for receive interrupt on USART1 in case you have enabled custom USART handler mode
 * @note   With __weak parameter to prevent link errors if not defined by user
 * @param  c: character received via USART
 * @retval None
 */
__weak void LVQ_USART1_ReceiveHandler(uint8_t c);

/**
 * @brief  Callback function for receive interrupt on USART2 in case you have enabled custom USART handler mode
 * @note   With __weak parameter to prevent link errors if not defined by user
 * @param  c: character received via USART
 * @retval None
 */
__weak void LVQ_USART2_ReceiveHandler(uint8_t c);

/**
 * @brief  Callback function for receive interrupt on USART3 in case you have enabled custom USART handler mode
 * @note   With __weak parameter to prevent link errors if not defined by user
 * @param  c: character received via USART
 * @retval None
 */
__weak void LVQ_USART3_ReceiveHandler(uint8_t c);

/**
 * @brief  Callback function for receive interrupt on UART4 in case you have enabled custom USART handler mode
 * @note   With __weak parameter to prevent link errors if not defined by user
 * @param  c: character received via USART
 * @retval None
 */
__weak void LVQ_UART4_ReceiveHandler(uint8_t c);

/**
 * @brief  Callback function for receive interrupt on UART5 in case you have enabled custom USART handler mode
 * @note   With __weak parameter to prevent link errors if not defined by user
 * @param  c: character received via USART
 * @retval None
 */
__weak void LVQ_UART5_ReceiveHandler(uint8_t c);

/**
 * @brief  Callback function for receive interrupt on USART6 in case you have enabled custom USART handler mode
 * @note   With __weak parameter to prevent link errors if not defined by user
 * @param  c: character received via USART
 * @retval None
 */
__weak void LVQ_USART6_ReceiveHandler(uint8_t c);

/**
 * @brief  Callback function for receive interrupt on UART7 in case you have enabled custom USART handler mode
 * @note   With __weak parameter to prevent link errors if not defined by user
 * @param  c: character received via USART
 * @retval None
 */
__weak void LVQ_UART7_ReceiveHandler(uint8_t c);

/**
 * @brief  Callback function for receive interrupt on UART8 in case you have enabled custom USART handler mode
 * @note   With __weak parameter to prevent link errors if not defined by user
 * @param  c: character received via USART
 * @retval None
 */
__weak void LVQ_UART8_ReceiveHandler(uint8_t c);

#endif
