/**
  ******************************************************************************

  AT24C64 Setup For STM32F446RE
  Author:   LVQ
  Updated:  29st March 2023

  ******************************************************************************
*/

#ifndef __AT24C64_H
#define __AT24C64_H

#include "lvq_stm32f4_i2c.h"
#include "lvq_stm32f4_soft_i2c.h"
#include "main.h"

typedef struct {
		I2C_TypeDef* I2Cx;
		LVQ_I2C_PinsPack_t I2C_Pinspack;
		uint32_t I2C_Speed;
		uint8_t I2C_Address;
	
		/* Info W25Q32 */
		uint16_t PageSize;
		uint32_t PageCount;
}	LVQ_AT24C64_Info_t;

void LVQ_AT24C64_Init(LVQ_AT24C64_Info_t at24c64);
void LVQ_AT24C64_WritePage(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes, uint8_t *data, uint16_t count);
void LVQ_AT24C64_ReadPage(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes, uint8_t *data, uint16_t count);
void LVQ_AT24C64_WritePages(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes, uint8_t *data, uint16_t count);
void LVQ_AT24C64_ReadPages(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes, uint8_t *data, uint16_t count);

void LVQ_AT24C64_WriteNo(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes, float data);
float LVQ_AT24C64_ReadNo(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes);

#endif
