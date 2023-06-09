/**
  ******************************************************************************

  AT24C64 Setup For STM32F446RE
  Author:   LVQ
  Updated:  29st March 2023

  ******************************************************************************
*/

#include "math.h"
#include "lvq_stm32f4_at24c64.h"

#ifdef USE_SOFT_I2C_FOR_AT24C64
	LVQ_SI2C_t si2c = {
			.SSDAPort = GPIOA,
			.SSDAPin = GPIO_PIN_0,
			.SSCLPort = GPIOA,
			.SSCLPin = GPIO_PIN_1,
			.SI2C_Speed = LVQ_SI2C_Standard
	};
#endif

void LVQ_AT24C64_Init(LVQ_AT24C64_Info_t at24c64)
{
		/* Init I2C1 peripheral */
		#ifndef USE_SOFT_I2C_FOR_AT24C64
			LVQ_I2C_Init(at24c64.I2Cx, at24c64.I2C_Pinspack, at24c64.I2C_Speed);
		#else
			LVQ_SI2C_Init(si2c);
		#endif
}

void LVQ_AT24C64_WritePage(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes, uint8_t *data, uint16_t count)
{
		uint16_t padding = log(at24c64.PageSize)/log(2);
	
		uint16_t CountNumber = 0;
		if( count + offset_bytes > at24c64.PageSize )
				CountNumber = at24c64.PageSize - offset_bytes;
		else
				CountNumber = count;
		
		uint16_t page_address = ( page_number << padding ) | offset_bytes;

		#ifndef USE_SOFT_I2C_FOR_AT24C64
				LVQ_I2C_WriteMultiReg16(at24c64.I2Cx, at24c64.I2C_Address, page_address, data, CountNumber);  // write the data to the EEPROM
		#else
				LVQ_SI2C_WriteMultiReg16(si2c, at24c64.I2C_Address, page_address, data, CountNumber);  // write the data to the EEPROM
		#endif
}

void LVQ_AT24C64_ReadPage(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes, uint8_t *data, uint16_t count)
{
		uint16_t padding = log(at24c64.PageSize)/log(2);
	
		uint16_t CountNumber = 0;
		if( count + offset_bytes > at24c64.PageSize )
				CountNumber = at24c64.PageSize - offset_bytes;
		else
				CountNumber = count;
		
		uint16_t page_address = ( page_number << padding ) | offset_bytes;

		#ifndef USE_SOFT_I2C_FOR_AT24C64
				LVQ_I2C_ReadMultiReg16(at24c64.I2Cx, at24c64.I2C_Address, page_address, data, CountNumber);  // write the data to the EEPROM
		#else
				LVQ_SI2C_ReadMultiReg16(si2c, at24c64.I2C_Address, page_address, data, CountNumber);  // write the data to the EEPROM
		#endif
}

void LVQ_AT24C64_WritePages(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes, uint8_t *data, uint16_t count)
{
		uint16_t padding = log(at24c64.PageSize)/log(2);
	
		int16_t CountNumber = 0;
		if( count + offset_bytes >  ( at24c64.PageCount - page_number ) * at24c64.PageSize )
				CountNumber = ( at24c64.PageCount - page_number ) * at24c64.PageSize - offset_bytes;
		else
				CountNumber = count;

		do
		{	
				LVQ_AT24C64_WritePage(at24c64, page_number, offset_bytes, data, CountNumber); 
				page_number += 1;   
				CountNumber -= at24c64.PageSize - offset_bytes;  
				data += at24c64.PageSize;  
				offset_bytes = 0; 
				Delay_ms_systick (5);  
		} while ( CountNumber > 0 );
}

void LVQ_AT24C64_ReadPages(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes, uint8_t *data, uint16_t count)
{
		uint16_t padding = log(at24c64.PageSize)/log(2);
	
		int16_t CountNumber = 0;
		if( count + offset_bytes >  ( at24c64.PageCount - page_number ) * at24c64.PageSize )
				CountNumber = ( at24c64.PageCount - page_number ) * at24c64.PageSize - offset_bytes;
		else
				CountNumber = count;
		
		do
		{	
				LVQ_AT24C64_ReadPage(at24c64, page_number, offset_bytes, data, CountNumber); 
				page_number += 1;   
				CountNumber -= at24c64.PageSize - offset_bytes;  
				data += at24c64.PageSize;  
				offset_bytes = 0; 
				Delay_ms_systick (5);  
		} while ( CountNumber > 0 );
}

void Float_To_Bytes(uint8_t * float_bytes_temp, float float_data)
{
		union {
				float float_value;
				uint8_t bytes[4];
		} ConvertData;
    ConvertData.float_value = float_data;
    for (uint8_t i = 0; i < 4; i++) 
				float_bytes_temp[i] = ConvertData.bytes[i];
}

float Bytes_To_Float(uint8_t * float_bytes_temp)
{
		union {
			float a;
			uint8_t bytes[4];
		} ConvertData;
   for (uint8_t i = 0; i < 4; i++)
    	ConvertData.bytes[i] = float_bytes_temp[i];
   float float_value =  ConvertData.a;
   return float_value;
}

void Convert_Float_To_Bytes(uint8_t * float_bytes_temp, float float_data)
{
		uint8_t Data[4];
		*(float *) Data = float_data;
    for (uint8_t i = 0; i < 4; i++) 
				float_bytes_temp[i] = Data[i];
}

float Convert_Bytes_To_Float(uint8_t * float_bytes_temp)
{
	 uint32_t ConvertData[1];
	 ConvertData[0]	= ( float_bytes_temp[3] << 24 ) + ( float_bytes_temp[2] << 16 ) + ( float_bytes_temp[1] << 8 ) + float_bytes_temp[0];
   float float_value = *(float *) ConvertData;
   return float_value;
}

void LVQ_AT24C64_WriteNo(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes, float data)
{
		uint8_t bytes_temp[4];
		Float_To_Bytes(bytes_temp, data);
		LVQ_AT24C64_WritePage(at24c64, page_number, offset_bytes, bytes_temp, 4);
}

float LVQ_AT24C64_ReadNo(LVQ_AT24C64_Info_t at24c64, uint16_t page_number, uint16_t offset_bytes)
{
		uint8_t float_temp[4];
		LVQ_AT24C64_ReadPage(at24c64, page_number, offset_bytes, float_temp, 4);
		return Bytes_To_Float(float_temp);
}
