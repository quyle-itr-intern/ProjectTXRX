/**
  ******************************************************************************

  USART For STM32F446RE
  Author:   LVQ
  Updated:  18st September 2022
	Algorithm: CircularQueue

  ******************************************************************************
*/

#ifndef __USART_ADVANCE_H
#define __USART_ADVANCE_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_defines.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_usart.h"
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

typedef struct {
		USART_TypeDef* Instance;
		uint8_t*       pTxBuffer;        /*!< Pointer to UART Tx transfer Buffer */
		uint16_t       TxXferSize;       /*!< UART Tx Transfer size              */
		uint16_t       TxXferCount;      /*!< UART Tx Transfer Counter           */
		uint8_t*       pRxBuffer;        /*!< Pointer to UART Rx transfer Buffer */
		uint16_t       RxXferSize;       /*!< UART Rx Transfer size              */
		uint16_t       RxXferCount;      /*!< UART Rx Transfer Counter           */

} LVQ_USART_Data_RXTX_t;

#define LVQ_RECEIVE_IT_DATA_USART

/**
 * @brief  Receive data until idle for USARTx.
 * @param  *USARTx: Pointer to USARTx peripheral you will use for initialization
 * @param  u8pBuffer: Store data from USART 
 * @param  u16Length: Length of array data store
 * @retval None   
 */
void LVQ_USART_ReceiveToIdle(LVQ_USART_Data_RXTX_t* LVQ_USART, USART_TypeDef* USARTx, uint8_t* u8pBuffer, uint16_t u16Length);

void LVQ_USART_Receive_IT(LVQ_USART_Data_RXTX_t* LVQ_USART);

void LVQ_USART_ReceiveIT_Done(LVQ_USART_Data_RXTX_t* LVQ_USART, uint16_t u16Length);

#endif
