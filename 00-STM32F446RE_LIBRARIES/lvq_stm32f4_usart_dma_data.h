/**
  ******************************************************************************

  USART DMA DATA For STM32F446RE
  Author:   LVQ
  Updated:  10 January 2023
	Algorithm: ...

  ******************************************************************************
*/

#ifndef __USART_DMA_DATA_H
#define __USART_DMA_DATA_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_defines.h"
#include "lvq_stm32f4_usart.h"
#include "lvq_stm32f4_usart_dma.h"
#include "stm32f4xx_dma.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define USE_USART6_DMA_DATA

/* UART packet process struct */
typedef enum {
  txIdle = 0,
  txBusy
} LVQ_USART_TX_STATE;

/* Data read structure */
typedef struct {
	uint16_t size_max;
  uint16_t index;                      /* Index of data in buffer */
  uint16_t count;                      /* Return data count */
  uint8_t *buffer;                     /* Return buffer pointer of valid data */
} LVQ_USART_RX_BUFFER_READ_STRUCT;

#define UTX1_BUFFER_SIZE               1024
#define URX1_BUFFER_SIZE               512

#define UTX2_BUFFER_SIZE               1024
#define URX2_BUFFER_SIZE               512

#define UTX3_BUFFER_SIZE               1024
#define URX3_BUFFER_SIZE               512

#define UTX4_BUFFER_SIZE               1024
#define URX4_BUFFER_SIZE               512

#define UTX5_BUFFER_SIZE               1024
#define URX5_BUFFER_SIZE               512

#define UTX6_BUFFER_SIZE               1024
#define URX6_BUFFER_SIZE               512

#ifdef USE_USART1_DMA_DATA
	extern uint8_t USART1_Tx_Buffer[];
	extern LVQ_USART_TX_STATE USART1_Tx_State;
	extern uint8_t USART1_Rx_Buffer[];
	extern uint8_t USART1_Temp_Buffer[];
	extern LVQ_USART_RX_BUFFER_READ_STRUCT USART1_Rx_Read_Structure;
#endif

#ifdef USE_USART2_DMA_DATA
	extern uint8_t USART2_Tx_Buffer[];
	extern LVQ_USART_TX_STATE USART2_Tx_State;
	extern uint8_t USART2_Rx_Buffer[];
	extern uint8_t USART2_Temp_Buffer[];
	extern LVQ_USART_RX_BUFFER_READ_STRUCT USART2_Rx_Read_Structure;
#endif

#ifdef USE_USART3_DMA_DATA
	extern uint8_t USART3_Tx_Buffer[];
	extern LVQ_USART_TX_STATE USART3_Tx_State;
	extern uint8_t USART3_Rx_Buffer[];
	extern uint8_t USART3_Temp_Buffer[];
	extern LVQ_USART_RX_BUFFER_READ_STRUCT USART3_Rx_Read_Structure;
#endif

#ifdef USE_UART4_DMA_DATA
	extern uint8_t UART4_Tx_Buffer[];
	extern LVQ_USART_TX_STATE UART4_Tx_State;
	extern uint8_t UART4_Rx_Buffer[];
	extern uint8_t UART4_Temp_Buffer[];
	extern LVQ_USART_RX_BUFFER_READ_STRUCT UART4_Rx_Read_Structure;
#endif

#ifdef USE_UART5_DMA_DATA
	extern uint8_t UART5_Tx_Buffer[];
	extern LVQ_USART_TX_STATE UART5_Tx_State;
	extern uint8_t UART5_Rx_Buffer[];
	extern uint8_t UART5_Temp_Buffer[];
	extern LVQ_USART_RX_BUFFER_READ_STRUCT UART5_Rx_Read_Structure;
#endif

#ifdef USE_USART6_DMA_DATA
	extern uint8_t USART6_Tx_Buffer[];
	extern LVQ_USART_TX_STATE USART6_Tx_State;
	extern uint8_t USART6_Rx_Buffer[];
	extern uint8_t USART6_Temp_Buffer[];
	extern LVQ_USART_RX_BUFFER_READ_STRUCT USART6_Rx_Read_Structure;
#endif

void LVQ_USART_Setup(USART_TypeDef* USARTx, LVQ_USART_PinsPack_t pinspack, uint32_t baudrate);
void LVQ_USART_Rx_Receive_Binary(USART_TypeDef* USARTx, LVQ_USART_RX_BUFFER_READ_STRUCT* USART_Rx_Read_Structure, uint8_t *buffer, uint16_t size);

void LVQ_USART_Write(USART_TypeDef* USARTx, uint8_t* data, uint16_t count, LVQ_USART_TX_STATE* Tx_State);
void LVQ_USART_TxUpdate(USART_TypeDef* USARTx, uint16_t count, LVQ_USART_TX_STATE* Tx_State);

LVQ_USART_TX_STATE LVQ_USART_FlushTxBuffer(USART_TypeDef* USARTx, LVQ_USART_TX_STATE* Tx_State);

#endif
