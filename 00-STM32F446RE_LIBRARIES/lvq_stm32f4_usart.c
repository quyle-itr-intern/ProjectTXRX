/**
  ******************************************************************************

  USART For STM32F446RE
  Author:   LVQ
  Updated:  18st September 2022
	Algorithm: CircularQueue

  ******************************************************************************
*/

#include "lvq_stm32f4_usart.h"

LVQ_USART1_Callback_t LVQ_USART1_Callback = NULL;

void LVQ_USART1_Set_Callback(void* cb)
{
		LVQ_USART1_Callback = cb;
}

#define UARTx USART2

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) 
{
  while(USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET){}
  USART_SendData(UARTx, ch);
  return(ch);
}

int fgetc(FILE *f) 
{
  char ch;
  while(USART_GetFlagStatus(UARTx, USART_FLAG_RXNE) == RESET){}
  ch = USART_ReceiveData(UARTx);
  return((int)ch);
}

int ferror(FILE *f) 
{
  return EOF;
}

void _ttywrch(int ch) 
{
  while(USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET){}
  USART_SendData(UARTx, ch);
}

void _sys_exit(int return_code) 
{
  while (1); /* endless loop */
}

/**
 * @brief Internal USART struct
 */
typedef struct {
    uint8_t* Buffer;
    uint16_t Size;
    uint16_t Num;
    uint16_t In;
    uint16_t Out;
    uint8_t Initialized;
    uint8_t StringDelimiter;
} LVQ_USART_t;

/* Set variables for buffers */
#ifdef USE_USART1
uint8_t LVQ_USART1_Buffer[LVQ_USART1_BUFFER_SIZE];
#endif
#ifdef USE_USART2
uint8_t LVQ_USART2_Buffer[LVQ_USART2_BUFFER_SIZE];
#endif
#ifdef USE_USART3
uint8_t LVQ_USART3_Buffer[LVQ_USART3_BUFFER_SIZE];
#endif
#ifdef USE_UART4
uint8_t LVQ_UART4_Buffer[LVQ_UART4_BUFFER_SIZE];
#endif
#ifdef USE_UART5
uint8_t LVQ_UART5_Buffer[LVQ_UART5_BUFFER_SIZE];
#endif
#ifdef USE_USART6
uint8_t LVQ_USART6_Buffer[LVQ_USART6_BUFFER_SIZE];
#endif
#ifdef USE_UART7
uint8_t LVQ_UART7_Buffer[LVQ_UART7_BUFFER_SIZE];
#endif
#ifdef USE_UART8
uint8_t LVQ_UART8_Buffer[LVQ_UART8_BUFFER_SIZE];
#endif

#ifdef USE_USART1
LVQ_USART_t LVQ_USART1 = {LVQ_USART1_Buffer, LVQ_USART1_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_USART2
LVQ_USART_t LVQ_USART2 = {LVQ_USART2_Buffer, LVQ_USART2_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_USART3
LVQ_USART_t LVQ_USART3 = {LVQ_USART3_Buffer, LVQ_USART3_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_UART4
LVQ_USART_t LVQ_UART4 = {LVQ_UART4_Buffer, LVQ_UART4_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_UART5
LVQ_USART_t LVQ_UART5 = {LVQ_UART5_Buffer, LVQ_UART5_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_USART6
LVQ_USART_t LVQ_USART6 = {LVQ_USART6_Buffer, LVQ_USART6_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_UART7
LVQ_USART_t LVQ_UART7 = {LVQ_UART7_Buffer, LVQ_UART7_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_UART8
LVQ_USART_t LVQ_UART8 = {LVQ_UART8_Buffer, LVQ_UART8_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif

/* Private functions */
void LVQ_USART1_InitPins(LVQ_USART_PinsPack_t pinspack);
void LVQ_USART2_InitPins(LVQ_USART_PinsPack_t pinspack);
void LVQ_USART3_InitPins(LVQ_USART_PinsPack_t pinspack);
void LVQ_UART4_InitPins(LVQ_USART_PinsPack_t pinspack);
void LVQ_UART5_InitPins(LVQ_USART_PinsPack_t pinspack);
void LVQ_USART6_InitPins(LVQ_USART_PinsPack_t pinspack);
void LVQ_UART7_InitPins(LVQ_USART_PinsPack_t pinspack);
void LVQ_UART8_InitPins(LVQ_USART_PinsPack_t pinspack);
void LVQ_USART_INT_InsertToBuffer(LVQ_USART_t* u, uint8_t c);
LVQ_USART_t* LVQ_USART_INT_GetUsart(USART_TypeDef* USARTx);
uint8_t LVQ_USART_INT_GetSubPriority(USART_TypeDef* USARTx);
uint8_t LVQ_USART_BufferFull(USART_TypeDef* USARTx);

/* Private initializator function */
static void LVQ_USART_INT_Init(
    USART_TypeDef* USARTx,
    LVQ_USART_PinsPack_t pinspack,
    uint32_t baudrate,
    LVQ_USART_HardwareFlowControl_t FlowControl,
    uint32_t Mode,
    uint32_t Parity,
    uint32_t StopBits,
    uint32_t WordLength
);

void LVQ_USART_Init(USART_TypeDef* USARTx, LVQ_USART_PinsPack_t pinspack, uint32_t baudrate) 
{
	#ifdef USE_USART1
			if (USARTx == USART1) {
					LVQ_USART_INT_Init(USART1, pinspack, baudrate, LVQ_USART1_HARDWARE_FLOW_CONTROL, LVQ_USART1_MODE, LVQ_USART1_PARITY, LVQ_USART1_STOP_BITS, LVQ_USART1_WORD_LENGTH);
			}
	#endif
	#ifdef USE_USART2
			if (USARTx == USART2) {
					LVQ_USART_INT_Init(USART2, pinspack, baudrate, LVQ_USART2_HARDWARE_FLOW_CONTROL, LVQ_USART2_MODE, LVQ_USART2_PARITY, LVQ_USART2_STOP_BITS, LVQ_USART2_WORD_LENGTH);
			}
	#endif
	#ifdef USE_USART3
			if (USARTx == USART3) {
					LVQ_USART_INT_Init(USART3, pinspack, baudrate, LVQ_USART3_HARDWARE_FLOW_CONTROL, LVQ_USART3_MODE, LVQ_USART3_PARITY, LVQ_USART3_STOP_BITS, LVQ_USART3_WORD_LENGTH);
			}
	#endif
	#ifdef USE_UART4
			if (USARTx == UART4) {
					LVQ_USART_INT_Init(UART4, pinspack, baudrate, LVQ_UART4_HARDWARE_FLOW_CONTROL, LVQ_UART4_MODE, LVQ_UART4_PARITY, LVQ_UART4_STOP_BITS, LVQ_UART4_WORD_LENGTH);
			}
	#endif
	#ifdef USE_UART5
			if (USARTx == UART5) {
					LVQ_USART_INT_Init(UART5, pinspack, baudrate, LVQ_UART5_HARDWARE_FLOW_CONTROL, LVQ_UART5_MODE, LVQ_UART5_PARITY, LVQ_UART5_STOP_BITS, LVQ_UART5_WORD_LENGTH);
			}
	#endif
	#ifdef USE_USART6
			if (USARTx == USART6) {
					LVQ_USART_INT_Init(USART6, pinspack, baudrate, LVQ_USART6_HARDWARE_FLOW_CONTROL, LVQ_USART6_MODE, LVQ_USART6_PARITY, LVQ_USART6_STOP_BITS, LVQ_USART6_WORD_LENGTH);
			}
	#endif
	#ifdef USE_UART7
			if (USARTx == UART7) {
					LVQ_USART_INT_Init(UART7, pinspack, baudrate, LVQ_UART7_HARDWARE_FLOW_CONTROL, LVQ_UART7_MODE, LVQ_UART7_PARITY, LVQ_UART7_STOP_BITS, LVQ_UART7_WORD_LENGTH);
			}
	#endif
	#ifdef USE_UART8
			if (USARTx == UART8) {
					LVQ_USART_INT_Init(UART8, pinspack, baudrate, LVQ_UART8_HARDWARE_FLOW_CONTROL, LVQ_UART8_MODE, LVQ_UART8_PARITY, LVQ_UART8_STOP_BITS, LVQ_UART8_WORD_LENGTH);
			}
	#endif
}

void LVQ_USART_InitWithFlowControl(USART_TypeDef* USARTx, LVQ_USART_PinsPack_t pinspack, uint32_t baudrate, LVQ_USART_HardwareFlowControl_t FlowControl) 
{
	#ifdef USE_USART1
			if (USARTx == USART1) {
					LVQ_USART_INT_Init(USART1, pinspack, baudrate, FlowControl, LVQ_USART1_MODE, LVQ_USART1_PARITY, LVQ_USART1_STOP_BITS, LVQ_USART1_WORD_LENGTH);
			}
	#endif
	#ifdef USE_USART2
			if (USARTx == USART2) {
					LVQ_USART_INT_Init(USART2, pinspack, baudrate, FlowControl, LVQ_USART2_MODE, LVQ_USART2_PARITY, LVQ_USART2_STOP_BITS, LVQ_USART2_WORD_LENGTH);
			}
	#endif
	#ifdef USE_USART3
			if (USARTx == USART3) {
					LVQ_USART_INT_Init(USART3, pinspack, baudrate, FlowControl, LVQ_USART3_MODE, LVQ_USART3_PARITY, LVQ_USART3_STOP_BITS, LVQ_USART3_WORD_LENGTH);
			}
	#endif
	#ifdef USE_UART4
			if (USARTx == UART4) {
					LVQ_USART_INT_Init(UART4, pinspack, baudrate, FlowControl, LVQ_UART4_MODE, LVQ_UART4_PARITY, LVQ_UART4_STOP_BITS, LVQ_UART4_WORD_LENGTH);
			}
	#endif
	#ifdef USE_UART5
			if (USARTx == UART5) {
					LVQ_USART_INT_Init(UART5, pinspack, baudrate, FlowControl, LVQ_UART5_MODE, LVQ_UART5_PARITY, LVQ_UART5_STOP_BITS, LVQ_UART5_WORD_LENGTH);
			}
	#endif
	#ifdef USE_USART6
			if (USARTx == USART6) {
					LVQ_USART_INT_Init(USART6, pinspack, baudrate, FlowControl, LVQ_USART6_MODE, LVQ_USART6_PARITY, LVQ_USART6_STOP_BITS, LVQ_USART6_WORD_LENGTH);
			}
	#endif
	#ifdef USE_UART7
			if (USARTx == UART7) {
					LVQ_USART_INT_Init(UART7, pinspack, baudrate, FlowControl, LVQ_UART7_MODE, TM_UART7_PARITY, LVQ_UART7_STOP_BITS, LVQ_UART7_WORD_LENGTH);
			}
	#endif
	#ifdef USE_UART8
			if (USARTx == UART8) {
					LVQ_USART_INT_Init(UART8, pinspack, baudrate, FlowControl, LVQ_UART8_MODE, TM_UART8_PARITY, LVQ_UART8_STOP_BITS, LVQ_UART8_WORD_LENGTH);
		  }
	#endif
}

uint8_t LVQ_USART_Getc(USART_TypeDef* USARTx) {
    int8_t c = 0;
    LVQ_USART_t* u = LVQ_USART_INT_GetUsart(USARTx);

    /* Check if we have any data in buffer */
    if (u->Num > 0 || u->In != u->Out) {
        /* Check overflow */
        if (u->Out == u->Size) {
            u->Out = 0;
        }

        /* Read character */
        c = u->Buffer[u->Out];

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

uint16_t LVQ_USART_Gets(USART_TypeDef* USARTx, char* buffer, uint16_t bufsize) {
    uint16_t i = 0;

    /* Get USART structure */
    LVQ_USART_t* u = LVQ_USART_INT_GetUsart(USARTx);

    /* Check for any data on USART */
    if (
        u->Num == 0 ||                                              /*!< Buffer empty */
        (
            !LVQ_USART_FindCharacter(USARTx, u->StringDelimiter) && /*!< String delimiter not in buffer */
            u->Num != u->Size                                       /*!< Buffer is not full */
        ) ) 
		{
        /* Return 0 */
        return 0;
    }

    /* If available buffer size is more than 0 characters */
    while (i < (bufsize - 1)) {
        /* We have available data */
        buffer[i] = (char) LVQ_USART_Getc(USARTx);

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

uint8_t LVQ_USART_BufferEmpty(USART_TypeDef* USARTx) {
    LVQ_USART_t* u = LVQ_USART_INT_GetUsart(USARTx);

    /* Check if number of characters is zero in buffer */
    return (u->Num == 0 && u->In == u->Out);
}

uint8_t LVQ_USART_BufferFull(USART_TypeDef* USARTx) {
    LVQ_USART_t* u = LVQ_USART_INT_GetUsart(USARTx);

    /* Check if number of characters is the same as buffer size */
    return (u->Num == u->Size);
}

void LVQ_USART_ClearBuffer(USART_TypeDef* USARTx) {
    LVQ_USART_t* u = LVQ_USART_INT_GetUsart(USARTx);

    /* Reset variables */
    u->Num = 0;
    u->In = 0;
    u->Out = 0;
}

void LVQ_USART_SetCustomStringEndCharacter(USART_TypeDef* USARTx, uint8_t Character) {
    /* Get USART structure */
    LVQ_USART_t* u = LVQ_USART_INT_GetUsart(USARTx);

    /* Set delimiter */
    u->StringDelimiter = Character;
}

uint8_t LVQ_USART_FindCharacter(USART_TypeDef* USARTx, uint8_t c) {
    uint16_t num, out;
    LVQ_USART_t* u = LVQ_USART_INT_GetUsart(USARTx);

    /* Temp variables */
    num = u->Num;
    out = u->Out;

    while (num > 0) {
        /* Check overflow */
        if (out == u->Size) {
            out = 0;
        }

        /* Check if characters matches */
        if ((uint8_t) u->Buffer[out] == (uint8_t) c) {
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

void LVQ_USART_Puts(USART_TypeDef* USARTx, char* str) {
    LVQ_USART_t* u = LVQ_USART_INT_GetUsart(USARTx);
    /* If we are not initialized */
    if (u->Initialized == 0) {
        return;
    }

    /* Go through entire string */
    while (*str) {
        /* Wait to be ready, buffer empty */
        USART_WAIT(USARTx);
        /* Send data */
        USARTx->DR = (uint16_t)(*str++ & 0x01FF);
        /* Wait to be ready, buffer empty */
        USART_WAIT(USARTx);
    }
}

void LVQ_USART_Send(USART_TypeDef* USARTx, uint8_t* DataArray, uint16_t count) 
{
    uint16_t i;
    LVQ_USART_t* u = LVQ_USART_INT_GetUsart(USARTx);
    /* If we are not initialized */
    if (u->Initialized == 0) {
        return;
    }

    /* Go through entire data array */
    for (i = 0; i < count; i++) {
        /* Wait to be ready, buffer empty */
        USART_WAIT(USARTx);
        /* Send data */
        USARTx->DR = (uint16_t)(DataArray[i]);
        /* Wait to be ready, buffer empty */
        USART_WAIT(USARTx);
    }
}

/* Private functions */
void LVQ_USART_INT_InsertToBuffer(LVQ_USART_t* u, uint8_t c) 
{
    /* Still available space in buffer */
    if (u->Num < u->Size) {
        /* Check overflow */
        if (u->In == u->Size) {
            u->In = 0;
        }
        /* Add to buffer */
        u->Buffer[u->In] = c;
        u->In++;
        u->Num++;
    }
}

void LVQ_USART_InitCustomPinsCallback(USART_TypeDef* USARTx, uint16_t AlternateFunction) 
{
    /* Custom user function. */
    /* In case user needs functionality for custom pins, this function should be declared outside this library */
		/* Check for proper USART */
		if (USARTx == USART1) 
		{
			/* Initialize custom pins */
			#if DEFINE_USART1_CUSTOM_1
				/* TX pin = PB6 */
				LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Low, AlternateFunction);
				/* RX pin = PA10 */
				LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Low, AlternateFunction);
			#endif
			
			#if DEFINE_USART1_CUSTOM_2
				/* TX pin = PA9 */
				LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Low, AlternateFunction);
				/* RX pin = PB7 */
				LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Low, AlternateFunction);
			#endif
		}
}

LVQ_USART_t* LVQ_USART_INT_GetUsart(USART_TypeDef* USARTx) 
{
    LVQ_USART_t* u;

		#ifdef USE_USART1
				if (USARTx == USART1) {
						u = &LVQ_USART1;
				}
		#endif
		#ifdef USE_USART2
				if (USARTx == USART2) {
						u = &LVQ_USART2;
				}
		#endif
		#ifdef USE_USART3
				if (USARTx == USART3) {
						u = &LVQ_USART3;
				}
		#endif
		#ifdef USE_UART4
				if (USARTx == UART4) {
						u = &LVQ_UART4;
				}
		#endif
		#ifdef USE_UART5
				if (USARTx == UART5) {
						u = &LVQ_UART5;
				}
		#endif
		#ifdef USE_USART6
				if (USARTx == USART6) {
						u = &LVQ_USART6;
				}
		#endif
		#ifdef USE_UART7
				if (USARTx == UART7) {
						u = &LVQ_UART7;
				}
		#endif
		#ifdef USE_UART8
				if (USARTx == UART8) {
						u = &LVQ_UART8;
				}
		#endif

    return u;
}

uint8_t LVQ_USART_INT_GetSubPriority(USART_TypeDef* USARTx) 
{
    uint8_t u;

		#ifdef USE_USART1
				if (USARTx == USART1) {
						u = 1;
				}
		#endif
		#ifdef USE_USART2
				if (USARTx == USART2) {
						u = 2;
				}
		#endif
		#ifdef USE_USART3
				if (USARTx == USART3) {
						u = 3;
				}
		#endif
		#ifdef USE_UART4
				if (USARTx == UART4) {
						u = 4;
				}
		#endif
		#ifdef USE_UART5
				if (USARTx == UART5) {
						u = 5;
				}
		#endif
		#ifdef USE_USART6
				if (USARTx == USART6) {
						u = 6;
				}
		#endif
		#ifdef USE_UART7
				if (USARTx == UART7) {
						u = 7;
				}
		#endif
		#ifdef USE_UART8
				if (USARTx == UART8) {
						u = 8;
				}
		#endif

    return u;
}

#ifdef USE_USART1
void LVQ_USART1_InitPins(LVQ_USART_PinsPack_t pinspack) {
    /* Init pins */
#if defined(GPIOA)
    if (pinspack == LVQ_USART_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOA, GPIO_Pin_9 | GPIO_Pin_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_USART1);
    }
#endif
#if defined(GPIOB)
    if (pinspack == LVQ_USART_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOB, GPIO_Pin_6 | GPIO_Pin_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_USART1);
    }
#endif
    if (pinspack == LVQ_USART_PinsPack_Custom) {
        /* Init custom pins, callback used */
        LVQ_USART_InitCustomPinsCallback(USART1, GPIO_AF_USART1);
    }
}
#endif

#ifdef USE_USART2
void LVQ_USART2_InitPins(LVQ_USART_PinsPack_t pinspack) {
    /* Init pins */
#if defined(GPIOA)
    if (pinspack == LVQ_USART_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOA, GPIO_Pin_2 | GPIO_Pin_3, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_USART2);
    }
#endif
#if defined(GPIOD)
    if (pinspack == LVQ_USART_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOD, GPIO_Pin_5 | GPIO_Pin_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_USART2);
    }
#endif
    if (pinspack == LVQ_USART_PinsPack_Custom) {
        /* Init custom pins, callback used */
        LVQ_USART_InitCustomPinsCallback(USART2, GPIO_AF_USART2);
    }
}
#endif

#ifdef USE_USART3
void LVQ_USART3_InitPins(LVQ_USART_PinsPack_t pinspack) {
    /* Init pins */
#if defined(GPIOB)
    if (pinspack == LVQ_USART_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOB, GPIO_Pin_10, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_USART3);
				LVQ_GPIO_InitAlternate(GPIOC, GPIO_Pin_5, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_USART3);
    }
#endif
#if defined(GPIOC)
    if (pinspack == LVQ_USART_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOC, GPIO_Pin_10 | GPIO_Pin_11, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_USART3);
    }
#endif
#if defined(GPIOD)
    if (pinspack == LVQ_USART_PinsPack_3) {
        LVQ_GPIO_InitAlternate(GPIOD, GPIO_Pin_8 | GPIO_Pin_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_USART3);
    }
#endif
    if (pinspack == LVQ_USART_PinsPack_Custom) {
        /* Init custom pins, callback used */
        LVQ_USART_InitCustomPinsCallback(USART3, GPIO_AF_USART3);
    }
}
#endif

#ifdef USE_UART4
void LVQ_UART4_InitPins(LVQ_USART_PinsPack_t pinspack) {
    /* Init pins */
#if defined(GPIOA)
    if (pinspack == LVQ_USART_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOA, GPIO_Pin_0 | GPIO_Pin_1, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_UART4);
    }
#endif
#if defined(GPIOC)
    if (pinspack == LVQ_USART_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOC, GPIO_Pin_10 | GPIO_Pin_11, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_UART4);
    }
#endif
    if (pinspack == LVQ_USART_PinsPack_Custom) {
        /* Init custom pins, callback used */
        LVQ_USART_InitCustomPinsCallback(UART4, GPIO_AF_UART4);
    }
}
#endif

#ifdef USE_UART5
void LVQ_UART5_InitPins(LVQ_USART_PinsPack_t pinspack) {
    /* Init pins */
#if defined(GPIOC) && defined(GPIOD)
    if (pinspack == LVQ_USART_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOC, GPIO_Pin_12, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_UART5);
        LVQ_GPIO_InitAlternate(GPIOD, GPIO_Pin_2, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_UART5);
    }
#endif
    if (pinspack == LVQ_USART_PinsPack_Custom) {
        /* Init custom pins, callback used */
        LVQ_USART_InitCustomPinsCallback(UART5, GPIO_AF_UART5);
    }
}
#endif

#ifdef USE_USART6
void LVQ_USART6_InitPins(LVQ_USART_PinsPack_t pinspack) {
    /* Init pins */
#if defined(GPIOC)
    if (pinspack == LVQ_USART_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOC, GPIO_Pin_6 | GPIO_Pin_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_USART6);
    }
#endif
#if defined(GPIOG)
    if (pinspack == LVQ_USART_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOG, GPIO_Pin_14 | GPIO_Pin_9, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_USART6);
    }
#endif
    if (pinspack == LVQ_USART_PinsPack_Custom) {
        /* Init custom pins, callback used */
        LVQ_USART_InitCustomPinsCallback(USART6, GPIO_AF_USART6);
    }
}
#endif

#ifdef USE_UART7
void LVQ_UART7_InitPins(LVQ_USART_PinsPack_t pinspack) {
    /* Init pins */
#if defined(GPIOE)
    if (pinspack == LVQ_USART_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOE, GPIO_Pin_8 | GPIO_Pin_7, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_UART7);
    }
#endif
#if defined(GPIOF)
    if (pinspack == LVQ_USART_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOF, GPIO_Pin_7 | GPIO_Pin_6, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_UART7);
    }
#endif
    if (pinspack == TM_USART_PinsPack_Custom) {
        /* Init custom pins, callback used */
        TM_USART_InitCustomPinsCallback(UART7, GPIO_AF_UART7);
    }
}
#endif

#ifdef USE_UART8
void LVQ_UART8_InitPins(LVQ_USART_PinsPack_t pinspack) {
    /* Init pins */
#if defined(GPIOE)
    if (pinspack == LVQ_USART_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOE, GPIO_Pin_1 | GPIO_Pin_0, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High, GPIO_AF_UART8);
    }
#endif
    if (pinspack == LVQ_USART_PinsPack_Custom) {
        /* Init custom pins, callback used */
        LVQ_USART_InitCustomPinsCallback(UART8, GPIO_AF_UART8);
    }
}
#endif

static void LVQ_USART_INT_Init(
		USART_TypeDef* USARTx, 
		LVQ_USART_PinsPack_t pinspack,
    uint32_t baudrate,
    LVQ_USART_HardwareFlowControl_t FlowControl,
    uint32_t Mode,
    uint32_t Parity,
    uint32_t StopBits,
    uint32_t WordLength ) {
    LVQ_USART_t* u = LVQ_USART_INT_GetUsart(USARTx);
		
		/* Fill NVIC settings */
		/* Compute the Corresponding IRQ Priority --------------------------------*/
		uint8_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

		/*
		*     @arg NVIC_PriorityGroup_0: 0 bits for pre-emption priority : 0x700
		*                                4 bits for subpriority
		*     @arg NVIC_PriorityGroup_1: 1 bits for pre-emption priority : 0x600
		*                                3 bits for subpriority
		*     @arg NVIC_PriorityGroup_2: 2 bits for pre-emption priority : 0x500
		*                                2 bits for subpriority 
		*     @arg NVIC_PriorityGroup_3: 3 bits for pre-emption priority : 0x400
		*                                1 bits for subpriority
		*     @arg NVIC_PriorityGroup_4: 4 bits for pre-emption priority : 0x300
		*                                0 bits for subpriority
		*/
	
		SCB->AIRCR = 0x500;	
		
		tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
		tmppre = (0x04 - tmppriority);
		tmpsub = tmpsub >> tmppriority;

    tmppriority = USART_NVIC_PRIORITY << tmppre;
    tmppriority |=  (uint8_t)(LVQ_USART_INT_GetSubPriority(USARTx) & tmpsub);
		
		tmppriority = tmppriority << 0x04;
		
    /*
     * Initialize USARTx pins
     * Set channel for USARTx NVIC
     */
#ifdef USE_USART1
    if (USARTx == USART1) {
        /* Enable USART clock */
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

        /* Init pins */
        LVQ_USART1_InitPins(pinspack);

        /* Enable the Selected IRQ Channels --------------------------------------*/
				NVIC->ISER[USART1_IRQn >> 0x05] = (uint32_t)0x01 << (USART1_IRQn & (uint8_t)0x1F);
				
				/* IRQ Priority */
				NVIC->IP[USART1_IRQn] = tmppriority;
    }
#endif
#ifdef USE_USART2
    if (USARTx == USART2) {
        /* Enable USART clock */
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

        /* Init pins */
        LVQ_USART2_InitPins(pinspack);

        /* Enable the Selected IRQ Channels --------------------------------------*/
				NVIC->ISER[USART2_IRQn >> 0x05] = (uint32_t)0x01 << (USART2_IRQn & (uint8_t)0x1F);
			
				/* IRQ Priority */
				NVIC->IP[USART2_IRQn] = tmppriority;
    }
#endif
#ifdef USE_USART3
    if (USARTx == USART3) {
        /* Enable USART clock */
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

        /* Init pins */
        LVQ_USART3_InitPins(pinspack);

        /* Enable the Selected IRQ Channels --------------------------------------*/
				NVIC->ISER[USART3_IRQn >> 0x05] = (uint32_t)0x01 << (USART3_IRQn & (uint8_t)0x1F);
			
				/* IRQ Priority */
				NVIC->IP[USART3_IRQn] = tmppriority;
    }
#endif
#ifdef USE_UART4
    if (USARTx == UART4) {
        /* Enable UART clock */
        RCC->APB1ENR |= RCC_APB1ENR_UART4EN;

        /* Init pins */
        LVQ_UART4_InitPins(pinspack);

        /* Enable the Selected IRQ Channels --------------------------------------*/
				NVIC->ISER[UART4_IRQn >> 0x05] = (uint32_t)0x01 << (UART4_IRQn & (uint8_t)0x1F);
			
				/* IRQ Priority */
				NVIC->IP[UART4_IRQn] = tmppriority;
    }
#endif
#ifdef USE_UART5
    if (USARTx == UART5) {
        /* Enable UART clock */
        RCC->APB1ENR |= RCC_APB1ENR_UART5EN;

        /* Init pins */
        LVQ_UART5_InitPins(pinspack);

        /* Enable the Selected IRQ Channels --------------------------------------*/
				NVIC->ISER[UART5_IRQn >> 0x05] = (uint32_t)0x01 << (UART5_IRQn & (uint8_t)0x1F);
			
				/* IRQ Priority */
				NVIC->IP[UART5_IRQn] = tmppriority;
    }
#endif
#ifdef USE_USART6
    if (USARTx == USART6) {
        /* Enable UART clock */
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;

        /* Init pins */
        LVQ_USART6_InitPins(pinspack);

        /* Enable the Selected IRQ Channels --------------------------------------*/
				NVIC->ISER[USART6_IRQn >> 0x05] = (uint32_t)0x01 << (USART6_IRQn & (uint8_t)0x1F);
			
				/* IRQ Priority */
				NVIC->IP[USART6_IRQn] = tmppriority;
    }
#endif
#ifdef USE_UART7
    if (USARTx == UART7) {
        /* Enable UART clock */
        RCC->APB1ENR |= RCC_APB1ENR_UART7EN;

        /* Init pins */
        LVQ_UART7_InitPins(pinspack);

        /* Enable the Selected IRQ Channels --------------------------------------*/
				NVIC->ISER[UART7_IRQn >> 0x05] = (uint32_t)0x01 << (UART7_IRQn & (uint8_t)0x1F);
		
				/* IRQ Priority */
				NVIC->IP[UART7_IRQn] = tmppriority;
    }
#endif
#ifdef USE_UART8
    if (USARTx == UART8) {
        /* Enable UART clock */
        RCC->APB1ENR |= RCC_APB1ENR_UART8EN;

        /* Init pins */
        LVQ_UART8_InitPins(pinspack);

        /* Enable the Selected IRQ Channels --------------------------------------*/
				NVIC->ISER[UART8_IRQn >> 0x05] = (uint32_t)0x01 << (UART8_IRQn & (uint8_t)0x1F);
				
				/* IRQ Priority */
				NVIC->IP[UART8_IRQn] = tmppriority;
    }
#endif

    /* Deinit USART, force reset */
		
		/* Set USART baudrate */
		if(USARTx == USART1 || USARTx == USART6)
		{
			double USARTDIV = 90000000.0/(16*baudrate);
			USARTx->BRR = (uint16_t)( ((uint16_t) USARTDIV << 4) | (uint16_t) (USARTDIV - (uint16_t)USARTDIV)*16);
		}
    else
		{
			double USARTDIV = 45000000.0/(16*baudrate);
			USARTx->BRR = (uint16_t)( ((uint16_t) USARTDIV << 4) | (uint16_t) (USARTDIV - (uint16_t)USARTDIV)*16);
		}
		
		/* Transmitter enable set bit TE, Receiver enable set bit RE */
		USARTx->CR1 |= Mode;
		
		/* Word length set bit M to 8 Data bits, clear bit M to 9 Data bits */
		USART1->CR1 &= ~(1<<12);
		USARTx->CR1 |= Parity;
		
		/* STOP bits 00: 1 Stop bit, 01: 0.5 Stop bit, 10: 2 Stop bits, 11: 1.5 Stop bit */
		USARTx->CR2 &= ~(3<<12);
		USARTx->CR2 |= StopBits;

    /* We are not initialized */
    u->Initialized = 0;

    do {
        volatile uint32_t x = 0xFFF;
        while (x--);
    } while (0);

    /* Init */

    /* Enable RX interrupt */
    USARTx->CR1 |= USART_CR1_RXNEIE;
		
		USARTx->CR1 |= USART_CR1_IDLEIE;

    /* We are initialized now */
    u->Initialized = 1;

    /* Enable USART peripheral */
    USARTx->CR1 |= USART_CR1_UE;
}

#ifdef USE_USART1
void USART1_IRQHandler(void) {
    /* Check if interrupt was because data is received */
    if (USART1->SR & USART_SR_RXNE) {
#ifdef 	LVQ_USART1_USE_CUSTOM_IRQ
        /* Call user function */
					LVQ_USART1_ReceiveHandler(USART1->DR);
#else
			#ifdef LVQ_USART1_USE_UPDATE_FIRMWARE
					LVQ_USART1_Callback(USART1->DR);
			#else
        /* Put received data into internal buffer */
        LVQ_USART_INT_InsertToBuffer(&LVQ_USART1, USART1->DR);
			#endif
#endif
    }
}
#endif

#ifdef LVQ_RECEIVE_IT_DATA_USART2
		extern LVQ_USART_Data_RXTX_t LVQ_Usart2;
#endif

#ifdef USE_USART2
void USART2_IRQHandler(void) {
    /* Check if interrupt was because data is received */
    if (USART2->SR & USART_SR_RXNE) {
#ifdef LVQ_USART2_USE_CUSTOM_IRQ
        /* Call user function */
        LVQ_USART2_ReceiveHandler(USART2->DR);
#else
        /* Put received data into internal buffer */
        LVQ_USART_INT_InsertToBuffer(&LVQ_USART2, USART2->DR);
			
				#ifdef LVQ_RECEIVE_IT_DATA_USART2
						LVQ_USART_Receive_IT(&LVQ_Usart2);
				#endif
#endif
    }
}
#endif

#ifdef LVQ_RECEIVE_IT_DATA_USART3
		extern LVQ_USART_Data_RXTX_t LVQ_Usart3;
#endif

#ifdef USE_USART3
void USART3_IRQHandler(void) {
    /* Check if interrupt was because data is received */
    if (USART3->SR & USART_SR_RXNE) {
#ifdef LVQ_USART3_USE_CUSTOM_IRQ
        /* Call user function */
        LVQ_USART3_ReceiveHandler(USART3->DR);
#else
        /* Put received data into internal buffer */
        LVQ_USART_INT_InsertToBuffer(&LVQ_USART3, USART3->DR);
			
				#ifdef LVQ_RECEIVE_IT_DATA_USART3
						LVQ_USART_Receive_IT(&LVQ_Usart3);
				#endif
#endif
    }
		#ifdef LVQ_RECEIVE_IT_DATA_USART3
			if(USART3->SR & USART_FLAG_IDLE)
			{
					uint16_t count = LVQ_Usart3.RxXferSize - LVQ_Usart3.RxXferCount;
					USART3->SR &= ~USART_FLAG_IDLE;
					USART3->DR;
					LVQ_Usart3.pRxBuffer -= LVQ_Usart3.RxXferSize - LVQ_Usart3.RxXferCount;
					LVQ_Usart3.RxXferCount = LVQ_Usart3.RxXferSize;
					LVQ_USART_ReceiveIT_Done(&LVQ_Usart3, count);
			}
		#endif
}
#endif

#ifdef USE_UART4
void UART4_IRQHandler(void) {
    /* Check if interrupt was because data is received */
    if (UART4->SR & USART_SR_RXNE) {
#ifdef LVQ_UART4_USE_CUSTOM_IRQ
        /* Call user function */
        LVQ_UART4_ReceiveHandler(UART4->DR);
#else
        /* Put received data into internal buffer */
        LVQ_USART_INT_InsertToBuffer(&LVQ_UART4, UART4->DR);
#endif
    }
}
#endif

#ifdef USE_UART5
void UART5_IRQHandler(void) {
    /* Check if interrupt was because data is received */
    if (UART5->SR & USART_SR_RXNE) {
#ifdef LVQ_UART5_USE_CUSTOM_IRQ
        /* Call user function */
        LVQ_UART5_ReceiveHandler(UART5->DR);
#else
        /* Put received data into internal buffer */
        LVQ_USART_INT_InsertToBuffer(&LVQ_UART5, UART5->DR);
#endif
    }
}
#endif

#ifdef USE_USART6
void USART6_IRQHandler(void) {
    /* Check if interrupt was because data is received */
    if (USART6->SR & USART_SR_RXNE) {
#ifdef LVQ_USART6_USE_CUSTOM_IRQ
        /* Call user function */
        LVQ_USART6_ReceiveHandler(USART6->DR);
#else
        /* Put received data into internal buffer */
        LVQ_USART_INT_InsertToBuffer(&LVQ_USART6, USART6->DR);
#endif
    }
}
#endif

#ifdef USE_UART7
void UART7_IRQHandler(void) {
    /* Check if interrupt was because data is received */
    if (UART7->SR & USART_SR_RXNE) {
#ifdef LVQ_UART7_USE_CUSTOM_IRQ
        /* Call user function */
        LVQ_UART7_ReceiveHandler(UART7->DR);
#else
        /* Put received data into internal buffer */
        LVQ_USART_INT_InsertToBuffer(&LVQ_UART7, UART7->DR);
#endif
    }
}
#endif

#ifdef USE_UART8
void UART8_IRQHandler(void) {
    /* Check if interrupt was because data is received */
    if (UART8->SR & USART_SR_RXNE) {
#ifdef LVQ_UART8_USE_CUSTOM_IRQ
        /* Call user function */
        LVQ_UART8_ReceiveHandler(UART8->DR);
#else
        /* Put received data into internal buffer */
        LVQ_USART_INT_InsertToBuffer(&LVQ_UART8, UART8->DR);
#endif
    }
}
#endif
