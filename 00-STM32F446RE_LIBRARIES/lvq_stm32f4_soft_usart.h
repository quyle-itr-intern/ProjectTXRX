/**
  ******************************************************************************

  SOFT USART For STM32F446RE
  Author:   LVQ
  Updated:  30st March 2023
	Algorithm: CircularQueue

  ******************************************************************************
*/

#ifndef __SOFT_USART_H
#define __SOFT_USART_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_defines.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_pwm.h"
#include "stm32f4xx_gpio.h"
#include "stdio.h"
#include "string.h"
#include "main.h"

#define 	NUMBER_OF_SUARTS 		 1 

#define 	SUART_DATA_LEN       8 	

/* Default buffer size for each USART */
#ifndef 	SUART_BUFFER_SIZE
#define 	SUART_BUFFER_SIZE               64
#endif

typedef struct 
{
		uint8_t	LVQ_SUART_RXBuffer[SUART_BUFFER_SIZE];
		uint8_t	LVQ_SUART_TXBuffer[SUART_BUFFER_SIZE];
} LVQ_SUART_Buffer_t;

typedef enum 
{
		LVQ_SUART_OK,
		LVQ_SUART_Error
}	LVQ_SUART_State_t;

typedef struct 
{
  uint16_t 							Size;
  uint16_t 							Num;
  uint16_t 							In;
  uint16_t 							Out;
  uint8_t 							Initialized;
  uint8_t 							StringDelimiter;
	__IO uint8_t 					TxNComplated;
	uint8_t								TxEnable;
	uint8_t								RxEnable;
	uint8_t 							TxBitShift,TxBitCounter;
	uint8_t 							RxBitShift,RxBitCounter;
	uint8_t								TxIndex,TxSize;
	uint8_t								RxIndex;
	uint8_t								BufferTemp;
	LVQ_SUART_Buffer_t		*Buffer;
	GPIO_TypeDef  				*TxPort;
	uint16_t 							TxPin;
	GPIO_TypeDef		  		*RxPort;
	uint16_t 							RxPin;
	uint8_t 							RxTimingFlag;
	uint8_t 							RxBitOffset;
} LVQ_SUART_t;

/* Call Every (0.2)*(1/9600) = 20.83 uS */
void 								LVQ_SUART_Handler(void);

uint16_t LVQ_SUART_Gets(uint8_t SUART_Number, char* buffer, uint16_t bufsize);
	
void 								LVQ_SUART_WaitUntilTxComplate(uint8_t SoftUartNumber);
uint8_t 						LVQ_SUART_RxAlavailable(uint8_t SoftUartNumber);
LVQ_SUART_State_t LVQ_SUART_Puts(uint8_t SoftUartNumber,uint8_t *Str, uint8_t size);
LVQ_SUART_State_t LVQ_SUART_EnableRx(uint8_t SoftUartNumber);
LVQ_SUART_State_t LVQ_SUART_DisableRx(uint8_t SoftUartNumber);
LVQ_SUART_State_t LVQ_SUART_Init( uint8_t SUART_Number, GPIO_TypeDef *TxPort, uint16_t TxPin, GPIO_TypeDef *RxPort, uint16_t RxPin, uint32_t baudrate );
LVQ_SUART_State_t LVQ_SUART_ReadRxBuffer(uint8_t SoftUartNumber,uint8_t *Buffer,uint8_t size);

#endif
