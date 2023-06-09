/**
  ******************************************************************************

  MODBUS RTU SLAVE For STM32F446RE
  Author:   LVQ
  Updated:  8 January 2023
	Algorithm: ...

  ******************************************************************************
*/

#ifndef __MODBUS_RTU_SLAVE_H
#define __MODBUS_RTU_SLAVE_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_defines.h"
#include "lvq_stm32f4_modbus_rtu.h"
#include "lvq_stm32f4_usart.h"
#include "stdio.h"

#define SLAVE_ID 7

#define ILLEGAL_FUNCTION       0x01
#define ILLEGAL_DATA_ADDRESS   0x02
#define ILLEGAL_DATA_VALUE     0x03

uint8_t Function_Slave_Read_Holding_Registers(void);
uint8_t Function_Slave_Read_Input_Registers(void);

uint8_t Function_Slave_Read_Coils(void);
uint8_t Function_Slave_Read_Inputs(void);

uint8_t Function_Slave_Write_Single_Register(void);
uint8_t Function_Slave_Write_Holding_Registers(void);

uint8_t Function_Slave_Write_Single_Coil(void);
uint8_t Function_Slave_Write_MultiCoils(void);

#endif 
