/**
  ******************************************************************************

  USART DMA DATA For STM32F446RE
  Author:   LVQ
  Updated:  10 January 2023
	Algorithm: ...

  ******************************************************************************
*/

#include "lvq_stm32f4_usart_dma_data.h"

void Disable_NVIC_USART(USART_TypeDef* USARTx);

/* DMA Init struct */
DMA_InitTypeDef USART_DMA_Init;

/* Create variables if necessary */
#ifdef USE_USART1_DMA_DATA
			/* Tx buffer */
			uint8_t USART1_Tx_Buffer[UTX1_BUFFER_SIZE];
			LVQ_USART_TX_STATE USART1_Tx_State = txIdle;
			/* Rx buffer */
			uint8_t USART1_Rx_Buffer[URX1_BUFFER_SIZE];
			uint8_t USART1_Temp_Buffer[URX1_BUFFER_SIZE];
			/* Data read structure */
			LVQ_USART_RX_BUFFER_READ_STRUCT USART1_Rx_Read_Structure = { URX1_BUFFER_SIZE, 0, 0, (uint8_t *)0 };
#endif
#ifdef USE_USART2_DMA_DATA
			/* Tx buffer */
			uint8_t USART2_Tx_Buffer[UTX2_BUFFER_SIZE];
			LVQ_USART_TX_STATE USART2_Tx_State = txIdle;
			/* Rx buffer */
			uint8_t USART2_Rx_Buffer[URX2_BUFFER_SIZE];
			uint8_t USART2_Temp_Buffer[URX2_BUFFER_SIZE];
			/* Data read structure */
			LVQ_USART_RX_BUFFER_READ_STRUCT USART2_Rx_Read_Structure = { URX2_BUFFER_SIZE, 0, 0, (uint8_t *)0 };
#endif
#ifdef USE_USART3_DMA_DATA
			/* Tx buffer */
			uint8_t USART3_Tx_Buffer[UTX3_BUFFER_SIZE];
			LVQ_USART_TX_STATE USART3_Tx_State = txIdle;
			/* Rx buffer */
			uint8_t USART3_Rx_Buffer[URX3_BUFFER_SIZE];
			uint8_t USART3_Temp_Buffer[URX3_BUFFER_SIZE];
			/* Data read structure */
			LVQ_USART_RX_BUFFER_READ_STRUCT USART3_Rx_Read_Structure = { URX3_BUFFER_SIZE, 0, 0, (uint8_t *)0 };
#endif
#ifdef USE_UART4_DMA_DATA
			/* Tx buffer */
			uint8_t UART4_Tx_Buffer[UTX4_BUFFER_SIZE];
			LVQ_USART_TX_STATE UART4_Tx_State = txIdle;
			/* Rx buffer */
			uint8_t UART4_Rx_Buffer[URX4_BUFFER_SIZE];
			uint8_t UART4_Temp_Buffer[URX4_BUFFER_SIZE];
			/* Data read structure */
			LVQ_USART_RX_BUFFER_READ_STRUCT UART4_Rx_Read_Structure = { URX4_BUFFER_SIZE, 0, 0, (uint8_t *)0 };
#endif
#ifdef USE_UART5_DMA_DATA
			/* Tx buffer */
			uint8_t UART5_Tx_Buffer[UTX5_BUFFER_SIZE];
			LVQ_USART_TX_STATE UART5_Tx_State = txIdle;
			/* Rx buffer */
			uint8_t UART5_Rx_Buffer[URX5_BUFFER_SIZE];
			uint8_t UART5_Temp_Buffer[URX5_BUFFER_SIZE];
			/* Data read structure */
			LVQ_USART_RX_BUFFER_READ_STRUCT UART5_Rx_Read_Structure = { URX5_BUFFER_SIZE, 0, 0, (uint8_t *)0 };
#endif
#ifdef USE_USART6_DMA_DATA
			/* Tx buffer */
			uint8_t USART6_Tx_Buffer[UTX6_BUFFER_SIZE];
			LVQ_USART_TX_STATE USART6_Tx_State = txIdle;
			/* Rx buffer */
			uint8_t USART6_Rx_Buffer[URX6_BUFFER_SIZE];
			uint8_t USART6_Temp_Buffer[URX6_BUFFER_SIZE];
			/* Data read structure */
			LVQ_USART_RX_BUFFER_READ_STRUCT USART6_Rx_Read_Structure = { URX6_BUFFER_SIZE, 0, 0, (uint8_t *)0 };
#endif

/* Init buffer read */
void LVQ_USART_RestoreBytes(LVQ_USART_RX_BUFFER_READ_STRUCT *read_struct, uint16_t count)
{
  uint16_t roll_count = count;

  /* Remove overflow buffer */
  while (roll_count > read_struct->size_max)
    roll_count -= read_struct->size_max;

  /* Return bytes back into buffer */
  if (roll_count > read_struct->index) 
	{
    read_struct->index = read_struct->size_max - roll_count + read_struct->index;
  } 
	else 
	{
    read_struct->index -= roll_count;
  }
}

/* Read buffer from DMA
 ** Return value: Number of bytes vaiable.
 */
void LVQ_USART_Read(USART_TypeDef* USARTx, LVQ_USART_RX_BUFFER_READ_STRUCT *read_struct)
{
	LVQ_USART_DMA_INT_t* USART_Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);
  __IO uint16_t dma_curr_ndtr;
  __IO uint16_t dma_curr_index;
  uint16_t data_index = 0;
  uint16_t data_count = 0;

  /* Get current NDTR */
  if ((dma_curr_ndtr = USART_Settings->DMA_Stream_RX->NDTR) == 0) {/* Not initialize or turn-around state*/
    read_struct->count = 0;
    return;
  }

  /* Get current data indexs */
  dma_curr_index = read_struct->size_max - dma_curr_ndtr;
  if (read_struct->index < dma_curr_index) {
    /* Data is available */
    data_index = read_struct->index;
    data_count = dma_curr_index - read_struct->index;
    read_struct->index += data_count;
    read_struct->index &= ( read_struct->size_max - 1 );
  } else if (read_struct->index > dma_curr_index) {
    /* Data is available with overlap */
    data_index = read_struct->index;
    data_count = read_struct->size_max - read_struct->index;
    read_struct->index = 0;
  } else {                             /* No new data */
  }

  /* Return the reading */
  if (data_count > 0) 
	{
		if(USARTx == USART1){
			#ifdef USE_USART1_DMA_DATA 
						read_struct->buffer = &USART1_Rx_Buffer[data_index];
			#endif
		} else if(USARTx == USART2) {
			#ifdef USE_USART2_DMA_DATA 
						read_struct->buffer = &USART2_Rx_Buffer[data_index];
			#endif
		} else if(USARTx == USART3) {
			#ifdef USE_USART3_DMA_DATA 
						read_struct->buffer = &USART3_Rx_Buffer[data_index];
			#endif
		} else if(USARTx == UART4) {
			#ifdef USE_UART4_DMA_DATA 
						read_struct->buffer = &UART4_Rx_Buffer[data_index];
			#endif
		} else if(USARTx == UART5) {
			#ifdef USE_UART5_DMA_DATA 
						read_struct->buffer = &UART5_Rx_Buffer[data_index];
			#endif
		} else if(USARTx == USART6) {
			#ifdef USE_USART6_DMA_DATA 
						read_struct->buffer = &USART6_Rx_Buffer[data_index];
			#endif
		}
    read_struct->count = data_count;
  } else {
    read_struct->count = 0;
  }
	
	free(USART_Settings);
}

void LVQ_USART_ReadEx(USART_TypeDef* USARTx, LVQ_USART_RX_BUFFER_READ_STRUCT *read_struct, uint8_t *buffer, uint16_t buffer_size, uint16_t*reading_count)
{
  uint16_t bytes_to_read, data_read_index;
  bytes_to_read = buffer_size;         /* Tracking count of data readings */
  data_read_index = 0;                 /* Increment buffer index */
  do 
	{
    LVQ_USART_Read(USARTx, read_struct);
    if (read_struct->count <= bytes_to_read) 
		{
      memcpy(&buffer[data_read_index], read_struct->buffer, read_struct->count);
      data_read_index += read_struct->count;
      bytes_to_read -= read_struct->count;
    } 
		else 
			{
      /* Return some byte back to buffer */
      LVQ_USART_RestoreBytes(read_struct, (read_struct->count - bytes_to_read));/* Fixed: Waijung 14.05a */

      /* Return reading data */
      memcpy(&buffer[data_read_index], read_struct->buffer, bytes_to_read);
      bytes_to_read = 0;
    }
  } while ((bytes_to_read > 0) && (read_struct->count > 0));

  /* Number of reading bytes */
  *reading_count = buffer_size - bytes_to_read;
}

/* Read Ascii packet
 * Return char count, exclude NULL
 * Terminator: "\n", "\r", "\r\n"
 */
uint16_t LVQ_USART_ReadLine(USART_TypeDef* USARTx, LVQ_USART_RX_BUFFER_READ_STRUCT *read_struct, const char *terminator, uint16_t terminator_count, uint8_t *buffer, uint16_t buffer_size)
{
  uint16_t count, packet_len = 0, receive_count = 0;
  uint16_t i;
  uint8_t terminator_found = 0;

  /* Determine maximum number of bytes to read */
  count = buffer_size - 1;
  if (count >= read_struct->size_max)
    count = read_struct->size_max - 1;

  /* Ignore terminator is invalid */
  if (terminator_count < 1)
    return 0;

  /* Read packet */
  do 
	{
    LVQ_USART_Read(USARTx, read_struct);            /* Check DMA buffer */
    receive_count += read_struct->count;/* Total number of data received */

    /* Search terminator */
    i = 0;
    while (!terminator_found && (i < read_struct->count)) {
      if (read_struct->buffer[i] == (uint8_t)terminator[terminator_count - 1])
        terminator_found = 1;
      i++;
    }

    packet_len += i;
    if (terminator_found) 
		{
      terminator_found = 0;

      /* Roll-back buffer index */
      if ((packet_len > count) || (packet_len < terminator_count)) 
			{ /* Packet count is invalid, drop it */
        LVQ_USART_RestoreBytes(read_struct, (receive_count-packet_len));

        /* Reset */
        packet_len = 0;
        receive_count = 0;
      } 
			else 
			{
        LVQ_USART_RestoreBytes(read_struct, receive_count);

        /* Load data into buffer */
        LVQ_USART_ReadEx(USARTx, read_struct, buffer, packet_len, &i);
        buffer[packet_len] = '\0';     /* Append NULL */

        /* Validate terminator */
        if (!strncmp((char *)&buffer[packet_len-terminator_count], terminator, terminator_count)) 
				{
          return packet_len;           /* packet reading success, return number of received bytes */
        } else {
          /* Invalid terminator */
          packet_len = 0;
          receive_count = 0;
        }
      }
    }
  } while (read_struct->count > 0);

  /* Could not find the packet terminator, reset reading struct to its original position */
  if (receive_count > 0) 
	{
    LVQ_USART_RestoreBytes(read_struct, receive_count);
  }

  /* No byte receive */
  return 0;
}

/* Read Binary packet
 * 0: Not ready, 1: Data is ready
 */
uint8_t LVQ_USART_ReadBinary(USART_TypeDef* USARTx, LVQ_USART_RX_BUFFER_READ_STRUCT *read_struct, const char *header, uint16_t header_count, const char *terminator, uint16_t terminator_count, uint8_t *buffer, uint16_t data_count)
{
  uint16_t receive_count = 0, drop_count = 0, binary_state = 0, binary_index = 0;
  uint16_t i;
  do 
	{
    LVQ_USART_Read(USARTx, read_struct);           /* Check DMA buffer */
    receive_count += read_struct->count;/* Total number of data received */

    /* Binary packet processing */
		
		/* State machine */
		
    for (i=0; i<read_struct->count; i++) 
		{
      switch ( binary_state ) 
			{
       case 0:
				                              /* Search for header */
        if (binary_index < header_count) {
          if (read_struct->buffer[i] == header[binary_index]) {
            binary_index++;
          } else {
            binary_index = 0;
            drop_count = receive_count - (read_struct->count - i - 1);/* Drop packet */
          }
          break;
        } else {                       /* Change to DATA state */
          binary_index = 0;
          binary_state++;
        }

       case 1:                         /* Wait for data */

        if (binary_index < data_count) 
				{
          buffer[binary_index] = read_struct->buffer[i];
          binary_index++;

          /* Check if ready (No terminator) */
          if ((binary_index >= data_count) && (terminator_count == 0)) 
					{
            LVQ_USART_RestoreBytes(read_struct, (read_struct->count - i - 1));  /* Restore some bytes when data enough */
            return 1;                  /* Return success status */
          }
          break;
        } else {                       /* Change to Terminator state */
          binary_index = 0;
          binary_state ++;
        }

       case 2:                         /* Scan for terminator */
        if (binary_index < terminator_count) 
				{
          if (read_struct->buffer[i] == terminator[binary_index]) 
					{
            binary_index ++;
          } 
					else 
					{
            binary_state = 0;   
            binary_index = 0;
            drop_count = receive_count - (read_struct->count - i - 1);/* Drop packet */
          }
        }

        if (binary_index >= terminator_count) 
				{/* Success */
          /* Restore some bytes */
          LVQ_USART_RestoreBytes(read_struct, (read_struct->count - i - 1));  /* Restore some bytes when data enough */
          return 1;                    /* Return success status */
        }
        break;
      }
    }
  } while (read_struct->count > 0);

  /* Restore bytes */
  LVQ_USART_RestoreBytes(read_struct, (receive_count - drop_count));
  return 0;
}

/* Wait for DMA transfer to completion */
LVQ_USART_TX_STATE LVQ_USART_FlushTxBuffer(USART_TypeDef* USARTx, LVQ_USART_TX_STATE* Tx_State)
{
	LVQ_USART_DMA_INT_t* USART_Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);
	
  if (*Tx_State != txIdle) {
    /* Check last DMA transmit */
    while ( (LVQ_DMA_GetFlags(USART_Settings->DMA_Stream_TX, DMA_FLAG_TCIF) == RESET) || (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) ) {
    }

    /* Clear DMA Streams flags */
    LVQ_DMA_ClearFlag(USART_Settings->DMA_Stream_TX, DMA_FLAG_TCIF | DMA_FLAG_HTIF);

    /* Disable the DMA Streams */
    DMA_Cmd(USART_Settings->DMA_Stream_TX, DISABLE);

    /* Disable the USART Tx DMA request */
    USART_DMACmd(USARTx, USART_DMAReq_Tx, DISABLE);

    /* Update state */
    *Tx_State = txIdle;
  }
	
	free(USART_Settings);

  return *Tx_State;
}

/* Write to DMA buffer */
void LVQ_USART_Write(USART_TypeDef* USARTx, uint8_t* data, uint16_t count, LVQ_USART_TX_STATE* Tx_State)
{
  if (count > 0) 
	{
		/* Wait for Tx state to idle */
		LVQ_USART_FlushTxBuffer(USARTx, Tx_State);

		/* Setup */
		if ( USARTx == USART1 ){
			#ifdef USE_USART1_DMA_DATA 
				memcpy(USART1_Tx_Buffer, data, count);
			#endif
		} else if ( USARTx == USART2 ) {
			#ifdef USE_USART2_DMA_DATA
				memcpy(USART2_Tx_Buffer, data, count);
			#endif
		} else if ( USARTx == USART3 ) {
			#ifdef USE_USART3_DMA_DATA 
				memcpy(USART3_Tx_Buffer, data, count);
			#endif
		} else if ( USARTx == UART4 ) {
			#ifdef USE_UART4_DMA_DATA 
				memcpy(UART4_Tx_Buffer, data, count);
			#endif
		} else if ( USARTx == UART5 ) {
			#ifdef USE_UART5_DMA_DATA 
				memcpy(UART5_Tx_Buffer, data, count);
			#endif
		} else if ( USARTx == USART6 ) {
			#ifdef USE_USART6_DMA_DATA 
				memcpy(USART6_Tx_Buffer, data, count);
			#endif
		}
		/* Update */
		LVQ_USART_TxUpdate(USARTx, count, Tx_State);
  }
}

/* Update transmit buffer */
void LVQ_USART_TxUpdate(USART_TypeDef* USARTx, uint16_t count, LVQ_USART_TX_STATE* Tx_State)
{
	LVQ_USART_DMA_INT_t* USART_Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);
	
  if (count > 0) {
    /* Wait for Tx state to idle */
    LVQ_USART_FlushTxBuffer(USARTx, Tx_State);
		USART_Settings->DMA_Stream_TX->NDTR = (uint16_t)count;

    /* Enable */
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
    USART_ClearFlag(USARTx, USART_FLAG_TC);
    DMA_Cmd(USART_Settings->DMA_Stream_TX, ENABLE);

    /* Tx is busy */
    *Tx_State = txBusy;
  }
	
	free(USART_Settings);
}

/* Enable UART-DMA module */
void LVQ_USART_Setup(USART_TypeDef* USARTx, LVQ_USART_PinsPack_t pinspack, uint32_t baudrate)
{
	LVQ_USART_Init(USARTx, pinspack, baudrate);
	Disable_NVIC_USART(USARTx);

  /* DMA init struct configuration */
	/* Get USART settings */
	LVQ_USART_DMA_INT_t* USART_Settings = LVQ_USART_DMA_INT_GetSettings(USARTx);

	/* Enable DMA clock */
	if (USART_Settings->DMA_Stream_TX >= DMA2_Stream0) 
	{
			/* Enable DMA2 clock */
			RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	} else {
			/* Enable DMA1 clock */
			RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	}

	/* Clear flags */
	LVQ_DMA_ClearFlags(USART_Settings->DMA_Stream_TX);
	LVQ_DMA_ClearFlags(USART_Settings->DMA_Stream_RX);
	
	USART_DMA_Init.DMA_PeripheralBaseAddr = (uint32_t)(&USARTx->DR);
	USART_DMA_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	USART_DMA_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;
	USART_DMA_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART_DMA_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	USART_DMA_Init.DMA_FIFOMode = DMA_FIFOMode_Disable;
	USART_DMA_Init.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	USART_DMA_Init.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	USART_DMA_Init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	/* Configure DMA controller to manage RX DMA request */
	USART_DMA_Init.DMA_Channel = USART_Settings->DMA_Channel_RX;
	USART_DMA_Init.DMA_DIR = DMA_DIR_PeripheralToMemory;
	USART_DMA_Init.DMA_Mode = DMA_Mode_Circular;
	USART_DMA_Init.DMA_Priority = DMA_Priority_VeryHigh;
	
	if ( USARTx == USART1 )
	{
		#ifdef USE_USART1_DMA_DATA 
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)USART1_Rx_Buffer;
	  	USART_DMA_Init.DMA_BufferSize = (uint16_t)URX1_BUFFER_SIZE;
		#endif
	} 
	else if ( USARTx == USART2 )
	{
		#ifdef USE_USART2_DMA_DATA
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)USART2_Rx_Buffer;
	  	USART_DMA_Init.DMA_BufferSize = (uint16_t)URX2_BUFFER_SIZE;
		#endif
	}
	else if ( USARTx == USART3 )
	{
		#ifdef USE_USART3_DMA_DATA 
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)USART3_Rx_Buffer;
			USART_DMA_Init.DMA_BufferSize = (uint16_t)URX3_BUFFER_SIZE;
		#endif
	}
	else if ( USARTx == UART4 )
	{
		#ifdef USE_UART4_DMA_DATA 
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)UART4_Rx_Buffer;
			USART_DMA_Init.DMA_BufferSize = (uint16_t)URX4_BUFFER_SIZE;
		#endif
	}
	else if ( USARTx == UART5 )
	{
		#ifdef USE_UART5_DMA_DATA 
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)UART5_Rx_Buffer;
			USART_DMA_Init.DMA_BufferSize = (uint16_t)URX5_BUFFER_SIZE;
		#endif
	}
	else if ( USARTx == USART6 )
	{
		#ifdef USE_USART6_DMA_DATA 
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)USART6_Rx_Buffer;
			USART_DMA_Init.DMA_BufferSize = (uint16_t)URX6_BUFFER_SIZE;
		#endif
	}

	DMA_Init(USART_Settings->DMA_Stream_RX, &USART_DMA_Init);

	/* Enable the USART Rx DMA requests */
	USART_DMACmd(USARTx, USART_DMAReq_Rx , ENABLE);

	/* Enable the DMA Stream */
	DMA_Cmd(USART_Settings->DMA_Stream_RX, ENABLE);

	/* Configure DMA controller to manage TX DMA request */
	USART_DMA_Init.DMA_Channel = USART_Settings->DMA_Channel_TX;
	USART_DMA_Init.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	USART_DMA_Init.DMA_BufferSize = (uint16_t)UTX2_BUFFER_SIZE;
	USART_DMA_Init.DMA_Mode = DMA_Mode_Normal;
	USART_DMA_Init.DMA_Priority = DMA_Priority_High;
	
	if ( USARTx == USART1 )
	{
		#ifdef USE_USART1_DMA_DATA 
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)USART1_Tx_Buffer;
	  	USART_DMA_Init.DMA_BufferSize = 0;
		#endif
	} 
	else if ( USARTx == USART2 )
	{
		#ifdef USE_USART2_DMA_DATA
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)USART2_Tx_Buffer;
	  	USART_DMA_Init.DMA_BufferSize = 0;
		#endif
	}
	else if ( USARTx == USART3 )
	{
		#ifdef USE_USART3_DMA_DATA 
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)USART3_Tx_Buffer;
			USART_DMA_Init.DMA_BufferSize = 0;
		#endif
	}
	else if ( USARTx == UART4 )
	{
		#ifdef USE_UART4_DMA_DATA 
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)UART4_Tx_Buffer;
			USART_DMA_Init.DMA_BufferSize = 0;
		#endif
	}
	else if ( USARTx == UART5 )
	{
		#ifdef USE_UART5_DMA_DATA 
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)UART5_Tx_Buffer;
			USART_DMA_Init.DMA_BufferSize = 0;
		#endif
	}
	else if ( USARTx == USART6 )
	{
		#ifdef USE_USART6_DMA_DATA 
			USART_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)USART6_Tx_Buffer;
			USART_DMA_Init.DMA_BufferSize = 0;
		#endif
	}
	
	DMA_Init(USART_Settings->DMA_Stream_TX, &USART_DMA_Init);
	/* Disable the DMA Streams TX because no DATA transfer */
  DMA_Cmd(USART_Settings->DMA_Stream_TX, DISABLE);
	
	/* Init Tx state */
}

/* Data */
float UART2_Rx_data0 = 0;
float UART2_Rx_data1 = 0;
float UART2_Rx_data2 = 0;
float UART2_Rx_data3 = 0;

/* Non-Blocking */
uint8_t UART2_Rx_Receive_Ascii(USART_TypeDef* USARTx, LVQ_USART_RX_BUFFER_READ_STRUCT* USART_Rx_Read_Structure, uint8_t *buffer, uint16_t size)
{
  uint8_t ready = 0;
  const char terminator[1] = { 10 };   /* Terminator */

  /* Rx Non-Blocking */
  if (LVQ_USART_ReadLine(USARTx, USART_Rx_Read_Structure, terminator, 1, buffer, size) > 0) 
	{
    ready = (uint8_t)(sscanf((const char*)buffer,"Input %f %f %f %f\n" ,		//
      &UART2_Rx_data0 ,&UART2_Rx_data1 ,&UART2_Rx_data2 ,
      &UART2_Rx_data3 ) == 4);		//
  }

  return ready;
}

void LVQ_USART_Rx_Receive_Binary(USART_TypeDef* USARTx, LVQ_USART_RX_BUFFER_READ_STRUCT* USART_Rx_Read_Structure, uint8_t *buffer, uint16_t size)
{
  const char header[2] = { 126, 126 }; /* Header 7E 7E */

  const char terminator[2] = { 3, 3 }; /* Terminator 03 03*/

  while (! LVQ_USART_ReadBinary(USARTx, USART_Rx_Read_Structure, header, 2, terminator, 2, buffer, size)) ;
}

void Disable_NVIC_USART(USART_TypeDef* USARTx)
{
		NVIC_InitTypeDef NVIC_InitStruct;
#ifdef USE_USART1
    if (USARTx == USART1) {
        /* Set IRQ channel */
        NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    }
#endif
#ifdef USE_USART2
    if (USARTx == USART2) {
        /* Set IRQ channel */
        NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    }
#endif
#ifdef USE_USART3
    if (USARTx == USART3) {
        /* Set IRQ channel */
        NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    }
#endif
#ifdef USE_UART4
    if (USARTx == UART4) {
        /* Set IRQ channel */
        NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
    }
#endif
#ifdef USE_UART5
    if (USARTx == UART5) {
        /* Set IRQ channel */
        NVIC_InitStruct.NVIC_IRQChannel = UART5_IRQn;
    }
#endif
#ifdef USE_USART6
    if (USARTx == USART6) {
        /* Set IRQ channel */
        NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
    }
#endif
		/* Fill NVIC settings */
    NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStruct);
}
