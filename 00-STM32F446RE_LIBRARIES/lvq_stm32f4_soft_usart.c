/**
  ******************************************************************************

  SOFT USART For STM32F446RE
  Author:   LVQ
  Updated:  30st March 2023
	Algorithm: CircularQueue

  ******************************************************************************
*/

#include "lvq_stm32f4_soft_usart.h"

uint8_t LVQ_SUART_FindCharacter( LVQ_SUART_t *u, uint8_t c );
uint16_t LVQ_SUART_Gets(uint8_t SUART_Number, char* buffer, uint16_t bufsize);

/* Some internal define */
#define SUART_DATA_LEN_C1 ( SUART_DATA_LEN + 1 )
#define SUART_DATA_LEN_C2 ( SUART_DATA_LEN + 2 )

/* All Soft Uart Config and State */
LVQ_SUART_t       	LVQ_SUART[NUMBER_OF_SUARTS];

/* TX RX Data Buffer */
LVQ_SUART_Buffer_t  LVQ_SUART_Buffer[NUMBER_OF_SUARTS];

/* For timing division */
__IO  uint8_t 			SU_Timer = 0;

/* Read RX single Pin Value */
uint8_t LVQ_SUART_GPIOReadPin( GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin )
{
		return LVQ_GPIO_GetInputPinValue( GPIOx, GPIO_Pin );
}

/* Write TX single Pin Value */
void LVQ_SUART_GPIOWritePin( GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState )
{
		LVQ_GPIO_SetPinValue( GPIOx, GPIO_Pin, PinState );
}

LVQ_PWM_TIM_t TIM3_Data;
static uint8_t initialize = 1;

/* Initial Soft Uart */
LVQ_SUART_State_t LVQ_SUART_Init( uint8_t SUART_Number, GPIO_TypeDef *TxPort, uint16_t TxPin, GPIO_TypeDef *RxPort, uint16_t RxPin, uint32_t baudrate )
{
		if( SUART_Number >= NUMBER_OF_SUARTS ) return LVQ_SUART_Error;

		LVQ_SUART[SUART_Number].TxNComplated = 0;

		LVQ_SUART[SUART_Number].RxBitCounter = 0;
		LVQ_SUART[SUART_Number].RxBitShift = 0;
		LVQ_SUART[SUART_Number].RxIndex = 0;

		LVQ_SUART[SUART_Number].TxEnable = 0;
		LVQ_SUART[SUART_Number].RxEnable = 0;

		LVQ_SUART[SUART_Number].TxBitCounter = 0;
		LVQ_SUART[SUART_Number].TxBitShift = 0;
		LVQ_SUART[SUART_Number].TxIndex = 0;

		LVQ_SUART[SUART_Number].TxSize = 0;

		LVQ_SUART[SUART_Number].Buffer = &LVQ_SUART_Buffer[SUART_Number];

		LVQ_SUART[SUART_Number].RxPort = RxPort;
		LVQ_SUART[SUART_Number].RxPin = RxPin;

		LVQ_SUART[SUART_Number].TxPort = TxPort;
		LVQ_SUART[SUART_Number].TxPin = TxPin;

		LVQ_SUART[SUART_Number].RxTimingFlag = 0;
		LVQ_SUART[SUART_Number].RxBitOffset = 0;
		
		LVQ_SUART[SUART_Number].Size = SUART_BUFFER_SIZE;
		LVQ_SUART[SUART_Number].Num = 0;
		LVQ_SUART[SUART_Number].In = 0;
		LVQ_SUART[SUART_Number].Out = 0;
		LVQ_SUART[SUART_Number].Initialized = 1;
		LVQ_SUART[SUART_Number].StringDelimiter = '\n';
		
		LVQ_GPIO_Init(TxPort, TxPin, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High);
		LVQ_GPIO_Init(RxPort, RxPin, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High);

		if( initialize )
				LVQ_PWM_InitTimer(TIM3, &TIM3_Data, 5 * baudrate );
		initialize = 0;
		
		NVIC_InitTypeDef NVIC_InitStruct;
		/* Start timer */
		TIM_Cmd(TIM3, DISABLE);
		/* Enable interrupt each update cycle */
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		/* Set NVIC parameters */
		NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
		NVIC_Init(&NVIC_InitStruct);
		/* Start timer */
		TIM_Cmd(TIM3, ENABLE);
		LVQ_SUART_EnableRx(0);
		return LVQ_SUART_OK;
}

void TIM3_IRQHandler(void)
{
		TIM_ClearFlag(TIM3, TIM_IT_Update);
		LVQ_SUART_Handler();
}

/* Send one bit to TX pin */
void LVQ_SUART_TransmitBit( LVQ_SUART_t *SU, uint8_t Bit )
{
		LVQ_SUART_GPIOWritePin(SU->TxPort,SU->TxPin, Bit);
}

/* Enable Soft Uart Receiving */
LVQ_SUART_State_t LVQ_SUART_EnableRx( uint8_t SUART_Number )
{
		if( SUART_Number >= NUMBER_OF_SUARTS ) 
				return LVQ_SUART_Error;
		LVQ_SUART[SUART_Number].RxEnable = 1;
		return LVQ_SUART_OK;
}

/* Disable Soft Uart Receiving */
LVQ_SUART_State_t LVQ_SUART_DisableRx( uint8_t SUART_Number )
{
		if( SUART_Number >= NUMBER_OF_SUARTS )
				return LVQ_SUART_Error;
		LVQ_SUART[SUART_Number].RxEnable = 0;
		return LVQ_SUART_OK;
}

/* Read Size of Received Data in buffer */
uint8_t LVQ_SUART_RxAlavailable( uint8_t SUART_Number )
{
		return LVQ_SUART[SUART_Number].RxIndex;
}

/* Move Received Data to Another Buffer */
LVQ_SUART_State_t LVQ_SUART_ReadRxBuffer( uint8_t SUART_Number, uint8_t *Buffer, uint8_t Len )
{
		uint16_t i;
		if( SUART_Number >= NUMBER_OF_SUARTS ) 
				return LVQ_SUART_Error;
		for( i = 0; i < Len; i++)
		{
				Buffer[i] = LVQ_SUART[SUART_Number].Buffer->LVQ_SUART_RXBuffer[i];
		}
		for( i = 0; i < LVQ_SUART[SUART_Number].RxIndex; i++ )
		{
				LVQ_SUART[SUART_Number].Buffer->LVQ_SUART_RXBuffer[i] = LVQ_SUART[SUART_Number].Buffer->LVQ_SUART_RXBuffer[ i + Len ];
		}
		LVQ_SUART[SUART_Number].RxIndex -= Len;
		return LVQ_SUART_OK;
}

/* Soft Uart Transmit Data Process */
void LVQ_SUART_TxProcess( LVQ_SUART_t *SU )
{
		if( SU->TxEnable )
		{
				/* Start */
				if( SU->TxBitCounter == 0 )
				{
						SU->TxNComplated = 1;
						SU->TxBitShift = 0;
						LVQ_SUART_TransmitBit(SU,0);
						SU->TxBitCounter++;
				}
				/* Data */
				else if( SU->TxBitCounter < SUART_DATA_LEN_C1 )
				{
						LVQ_SUART_TransmitBit(SU, (( SU->Buffer->LVQ_SUART_TXBuffer[SU->TxIndex]) >> ( SU->TxBitShift ) ) & 0x01 );
						SU->TxBitCounter++;
						SU->TxBitShift++;
				}
				/* Stop */
				else if( SU->TxBitCounter == SUART_DATA_LEN_C1 )
				{
						LVQ_SUART_TransmitBit(SU,1);
						SU->TxBitCounter++;
				}
				/* Complete */
				else if( SU->TxBitCounter == SUART_DATA_LEN_C2 )
				{
						/* Reset Bit Counter */
						SU->TxBitCounter = 0;

						/* Ready To Send Another Data */
						SU->TxIndex++;

						/* Check Size of Data */
						if( SU->TxSize > SU->TxIndex )
						{
								/* Continue Sending */
								SU->TxNComplated = 1;
								SU->TxEnable = 1;
						}
						else
						{
								/* Finish */
								SU->TxNComplated = 0;
								SU->TxEnable = 0;
						}
				}
		}
}

void LVQ_SUART_InsertToBuffer(LVQ_SUART_t* u, uint8_t c);

/* Soft Uart Receive Data Process */
void LVQ_SUART_RxDataBitProcess( LVQ_SUART_t *SU, uint8_t Bit )
{
		if( SU->RxEnable )
		{
				/* Start */
				if( SU->RxBitCounter == 0 )
				{
						/* Start Bit is 0 */
						if(Bit)
								return;

						SU->RxBitShift=0;
						SU->RxBitCounter++;
						SU->BufferTemp = 0;
				}
				/* Data */
				else if( SU->RxBitCounter < SUART_DATA_LEN_C1 )
				{
						SU->BufferTemp |= ( ( Bit & 0x01 ) << SU->RxBitShift );
						SU->RxBitCounter++;
						SU->RxBitShift++;
				}
				/* Stop and Complete */
				else if(SU->RxBitCounter == SUART_DATA_LEN_C1)
				{
						SU->RxBitCounter = 0;
						SU->RxTimingFlag = 0;

						/* Stop Bit must be 1 */
						if(Bit)
						{
							LVQ_SUART_InsertToBuffer(SU, SU->BufferTemp );
							/* Received successfully */
							/* Change RX Buffer Index */
							if( SU->RxIndex < ( SU->Size - 1 ) )
										SU->RxIndex++;
						}
						/* if not : ERROR -> Overwrite data */
				}
		}
}

/* Wait Until Transmit Completed */
/* You do not usually need to use this function! */
void LVQ_SUART_WaitUntilTxComplate( uint8_t SUART_Number )
{
		while( LVQ_SUART[SUART_Number].TxNComplated );
}

/* Copy Data to Transmit Buffer and Start Sending */
LVQ_SUART_State_t LVQ_SUART_Puts( uint8_t SUART_Number, uint8_t *Str, uint8_t Len )
{
		uint16_t i;

		if( SUART_Number >= NUMBER_OF_SUARTS )
				return LVQ_SUART_Error;
		if( LVQ_SUART[SUART_Number].TxNComplated ) 
				return LVQ_SUART_Error;

		LVQ_SUART[SUART_Number].TxIndex = 0;
		LVQ_SUART[SUART_Number].TxSize = Len;

		for( i = 0; i < Len; i++ )
		{
			LVQ_SUART[SUART_Number].Buffer->LVQ_SUART_TXBuffer[i] = Str[i];
		}

		LVQ_SUART[SUART_Number].TxNComplated = 1;
		LVQ_SUART[SUART_Number].TxEnable = 1;

		return LVQ_SUART_OK;
}

/* Capture RX and Get BitOffset */
uint8_t LVQ_SUART_ScanRxPorts(void)
{
		uint8_t i;
		uint8_t Buffer = 0x00, Bit;

		for( i = 0; i < NUMBER_OF_SUARTS; i++ )
		{
				/* Read RX GPIO Value */
				Bit = LVQ_SUART_GPIOReadPin(LVQ_SUART[i].RxPort, LVQ_SUART[i].RxPin);

				/* Starting conditions */
				if(!LVQ_SUART[i].RxBitCounter && !LVQ_SUART[i].RxTimingFlag && !Bit)
				{
						/* Save RX Bit Offset */
						/* Calculate middle position of data puls */
						LVQ_SUART[i].RxBitOffset = ( ( SU_Timer + 2 ) % 5 );

						/* Timing Offset is Set */
						LVQ_SUART[i].RxTimingFlag = 1;
				}

				/* Add all RX GPIO State to Buffer */
				Buffer |= ( ( Bit&0x01 ) << i );
		}
		return Buffer;
}

/* SoftUartHandler must call in interrupt every 0.2*(1/BR) */
/* if BR = 9600 then 0.2*(1/9600) = 20.8333333 uS */
void LVQ_SUART_Handler(void)
{
		int     	i;
		uint8_t 	SU_DBuffer;

		/* Capture RX and Get BitOffset */
		SU_DBuffer = LVQ_SUART_ScanRxPorts();

		for( i = 0; i < NUMBER_OF_SUARTS; i++ )
		{
				/* Receive Data if we in middle data pulse position */
				if(LVQ_SUART[i].RxBitOffset == SU_Timer)
				{
						LVQ_SUART_RxDataBitProcess( &LVQ_SUART[i], ( ( SU_DBuffer >> i ) & 0x01) );
				}
		}

		/* Sending always happens in the first time slot */
		if(SU_Timer == 0)
		{
				/* Transmit Data */
				for( i = 0; i < NUMBER_OF_SUARTS; i++ )
				{
					LVQ_SUART_TxProcess(&LVQ_SUART[i]);
				}
		}

		/* Timing process */
		SU_Timer++;
		if( SU_Timer >= 5 )
				SU_Timer=0;
}
	
uint8_t LVQ_SUART_Getc(LVQ_SUART_t *u) 
{
    int8_t c = 0;

    /* Check if we have any data in buffer */
    if (u->Num > 0 || u->In != u->Out) 
		{
        /* Check overflow */
        if (u->Out == u->Size) {
            u->Out = 0;
        }

        /* Read character */
        c = u->Buffer->LVQ_SUART_RXBuffer[u->Out];

        /* Increase output pointer */
        u->Out++;

        /* Decrease number of elements */
        if (u->Num) {
            u->Num--;
        }
    }

    /* Return character */
    return c;
}

uint16_t LVQ_SUART_Gets(uint8_t SUART_Number, char* buffer, uint16_t bufsize) 
{
    uint16_t i = 0;
	
		LVQ_SUART_t *u = &LVQ_SUART[SUART_Number];

    /* Check for any data on USART */
    if (
        u->Num == 0 ||                                              /*!< Buffer empty */
        (
            !LVQ_SUART_FindCharacter(u, u->StringDelimiter) && /*!< String delimiter not in buffer */
						u->Num != u->Size                                       /*!< Buffer is not full */
        ) ) 
		{
        /* Return 0 */
        return 0;
    }

    /* If available buffer size is more than 0 characters */
    while (i < (bufsize - 1)) {
        /* We have available data */
        buffer[i] = (char) LVQ_SUART_Getc(u);

        /* Check for end of string */
        if ((uint8_t) buffer[i] == (uint8_t) u->StringDelimiter) {
            /* Done */
            break;
        }

        /* Increase */
        i++;
    }

    /* Add zero to the end of string */
    buffer[++i] = 0;
		/* LVQ_USART_ClearBuffer(USARTx); */
    /* Return number of characters in buffer */
    return i;
}

uint8_t LVQ_SUART_BufferEmpty( LVQ_SUART_t *u ) 
{
    /* Check if number of characters is zero in buffer */
    return (u->Num == 0 && u->In == u->Out);
}

uint8_t LVQ_SUART_BufferFull( LVQ_SUART_t *u ) 
{
    /* Check if number of characters is the same as buffer size */
    return (u->Num == u->Size);
}

void LVQ_SUART_ClearBuffer( LVQ_SUART_t *u ) 
{
    /* Reset variables */
    u->Num = 0;
    u->In = 0;
    u->Out = 0;
}

void LVQ_SUART_SetCustomStringEndCharacter( LVQ_SUART_t *u, uint8_t Character ) 
{
    /* Set delimiter */
    u->StringDelimiter = Character;
}

uint8_t LVQ_SUART_FindCharacter( LVQ_SUART_t *u, uint8_t c ) 
{
    uint16_t num, out;

    /* Temp variables */
    num = u->Num;
    out = u->Out;

    while (num > 0) {
        /* Check overflow */
        if (out == u->Size) {
            out = 0;
        }

        /* Check if characters matches */
        if ( (uint8_t) u->Buffer->LVQ_SUART_RXBuffer[out] == (uint8_t) c ) 
				{
            /* Character found */
            return 1;
        }

        /* Set new variables */
        out++;
        num--;
    }

    /* Character is not in buffer */
    return 0;
}

/* Private functions */
void LVQ_SUART_InsertToBuffer(LVQ_SUART_t* u, uint8_t c) 
{
    /* Still available space in buffer */
    if (u->Num < u->Size) {
        /* Check overflow */
        if (u->In == u->Size) {
            u->In = 0;
        }
        /* Add to buffer */
        u->Buffer->LVQ_SUART_RXBuffer[u->In] = c;
        u->In++;
        u->Num++;
    }
}
