/**
  ******************************************************************************

  USART For STM32F446RE
  Author:   LVQ
  Updated:  18st September 2022
	Algorithm: CircularQueue

  ******************************************************************************
*/

#include "lvq_stm32f4_usart_advance.h"

__weak void LVQ_USART_ReceiveIT_Done(LVQ_USART_Data_RXTX_t* LVQ_USART, uint16_t u16Length)
{
		
}

void LVQ_USART_ReceiveToIdle(LVQ_USART_Data_RXTX_t* LVQ_USART, USART_TypeDef* USARTx, uint8_t* u8pBuffer, uint16_t u16Length)
{
		LVQ_USART->Instance = USARTx;
	  LVQ_USART->pRxBuffer = u8pBuffer;
		LVQ_USART->RxXferCount = u16Length;
		LVQ_USART->RxXferSize = u16Length;
}

void LVQ_USART_Receive_IT(LVQ_USART_Data_RXTX_t* LVQ_USART)
{
		uint8_t* pData;
	
		pData = (uint8_t*) LVQ_USART->pRxBuffer;
		
		*pData = (uint8_t)(LVQ_USART->Instance->DR & (uint8_t)0x00FF);
	
		LVQ_USART->pRxBuffer += 1U;
	
		if(--LVQ_USART->RxXferCount == 0)
		{
				LVQ_USART->pRxBuffer -= LVQ_USART->RxXferSize;
				LVQ_USART->RxXferCount = LVQ_USART->RxXferSize;
				LVQ_USART_ReceiveIT_Done(LVQ_USART, LVQ_USART->RxXferSize);
		}
}







