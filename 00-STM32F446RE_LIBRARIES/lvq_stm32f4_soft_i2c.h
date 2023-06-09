/**
  ******************************************************************************

  SOFT SPI For STM32F446RE
  Author:   LVQ
  Updated:  5st April 2023
	Algorithm: 

  ******************************************************************************
*/

#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_delay.h"
#include "main.h"

typedef enum 
{
		LVQ_SI2C_OK,
		LVQ_SI2C_Error
}	LVQ_SI2C_State_t;

typedef enum 
{
		LVQ_SI2C_Standard = 0,
		LVQ_SI2C_Fast_Mode,
		LVQ_SI2C_Fast_Mode_Plus,
		LVQ_SI2C_High_Speed
}	LVQ_SI2C_Speed_t;

typedef struct {
		GPIO_TypeDef  			*SSDAPort;
		uint16_t 						SSDAPin;
		GPIO_TypeDef				*SSCLPort;
		uint16_t 						SSCLPin;
		LVQ_SI2C_Speed_t 	  SI2C_Speed;
} LVQ_SI2C_t;

void LVQ_SI2C_Init(LVQ_SI2C_t si2c);

void LVQ_SI2C_Send(LVQ_SI2C_t si2c, uint8_t address, uint8_t reg, uint8_t data);

void LVQ_SI2C_WriteMultiReg16(LVQ_SI2C_t si2c, uint8_t address, uint16_t reg, uint8_t* data, uint16_t count);
void LVQ_SI2C_ReadMultiReg16(LVQ_SI2C_t si2c, uint8_t address, uint16_t reg, uint8_t* data, uint16_t count);

#endif
