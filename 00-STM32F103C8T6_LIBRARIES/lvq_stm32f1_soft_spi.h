/**
  ******************************************************************************

  SOFT SPI For STM32F446RE
  Author:   LVQ
  Updated:  5st April 2023
	Algorithm: 

  ******************************************************************************
*/

#ifndef __SOFT_SPI_H
#define __SOFT_SPI_H

#include "lvq_stm32f1_rccConfig.h"
#include "lvq_stm32f1_gpio.h"
#include "delay_systick.h"

typedef enum 
{
		LVQ_SSPI_OK,
		LVQ_SSPI_Error
}	LVQ_SSPI_State_t;

typedef enum 
{
		LVQ_SSPI_Mode_0 = 0,
		LVQ_SSPI_Mode_1 = 1,
		LVQ_SSPI_Mode_2 = 2,
		LVQ_SSPI_Mode_3 = 3
}	LVQ_SSPI_Mode_t;

typedef enum 
{
		LVQ_SSPI_DataSize_8 = 0,
		LVQ_SSPI_DataSize_16 = 1,
}	LVQ_SSPI_DataSize_t;

/*

Mode	CPOL	CPHA

0			0			0
1			0			1
2			1			0
3			1			1

*/

typedef struct 
{
	GPIO_TypeDef  			*SMOSIPort;
	uint16_t 						SMOSIPin;
	GPIO_TypeDef				*SMISOPort;
	uint16_t 						SMISOPin;
	GPIO_TypeDef				*SSCKPort;
	uint16_t 						SSCKPin;
	GPIO_TypeDef				*NSSPort;
	uint16_t 						NSSPin;
	LVQ_SSPI_Mode_t 	  SSPI_Mode;
	LVQ_SSPI_DataSize_t SSPI_DataSize;
} LVQ_SSPI_t;

void LVQ_SSPI_Init(LVQ_SSPI_t sspi);
uint16_t LVQ_SSPI_SendByte(LVQ_SSPI_t sspi, uint16_t data_write);
void LVQ_SSPI_SendMulti(LVQ_SSPI_t sspi, uint8_t* dataOut, uint8_t* dataIn, uint32_t count);
void LVQ_SSPI_SendMulti16(LVQ_SSPI_t sspi, uint16_t* dataOut, uint16_t* dataIn, uint32_t count);

#endif
