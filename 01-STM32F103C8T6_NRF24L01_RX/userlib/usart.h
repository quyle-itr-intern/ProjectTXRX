#ifndef _USART_H_
#define _USART_H_

#include <stdio.h>
#include <stm32f10x.h>
#include "stm32f10x_usart.h"
void UART1_Config(void);
void UART2_Config(void);
void UART3_Config(void);

void UART1_SendChar(char c);
void UART1_SendString(char *s);
void UART2_SendChar(char c);
void UART2_SendString(char *s);
void UART3_SendChar(char c);
void UART3_SendString(char *s);

#endif /*_USART_H*/
