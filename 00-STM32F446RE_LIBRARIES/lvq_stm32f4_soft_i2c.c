/**
  ******************************************************************************

  SOFT I2C For STM32F446RE
  Author:   LVQ
  Updated:  8st April 2023
	Algorithm: 

  ******************************************************************************
*/

#include "lvq_stm32f4_soft_i2c.h"

void LVQ_SI2C_Init(LVQ_SI2C_t si2c)
{
		LVQ_GPIO_Init(si2c.SSCLPort, si2c.SSCLPin, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High);
		LVQ_GPIO_Init(si2c.SSDAPort, si2c.SSDAPin, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High);
		LVQ_GPIO_SetPinHigh(si2c.SSDAPort, si2c.SSDAPin);
		LVQ_GPIO_SetPinHigh(si2c.SSCLPort, si2c.SSCLPin);
}

void LVQ_SI2C_Start(LVQ_SI2C_t si2c)
{
		LVQ_GPIO_SetPinHigh(si2c.SSDAPort, si2c.SSDAPin);
		Delay_us_systick(3);
		LVQ_GPIO_SetPinLow(si2c.SSDAPort, si2c.SSDAPin);
		Delay_us_systick(2);
		LVQ_GPIO_SetPinLow(si2c.SSCLPort, si2c.SSCLPin);
}

void LVQ_SI2C_Stop(LVQ_SI2C_t si2c)
{
		LVQ_GPIO_SetPinLow(si2c.SSDAPort, si2c.SSDAPin);
		LVQ_GPIO_SetPinHigh(si2c.SSCLPort, si2c.SSCLPin);
		Delay_us_systick(1);
		LVQ_GPIO_SetPinHigh(si2c.SSDAPort, si2c.SSDAPin);
		Delay_us_systick(4);
}

uint8_t LVQ_SI2C_WriteData(LVQ_SI2C_t si2c, uint8_t data_write)
{
		uint8_t res = 0;
		for( uint8_t i = 0; i < 8; i++ )
		{
				if( ( data_write & 0x80 ) > 0 )
						LVQ_GPIO_SetPinHigh(si2c.SSDAPort, si2c.SSDAPin);
				else
						LVQ_GPIO_SetPinLow(si2c.SSDAPort, si2c.SSDAPin);
				Delay_us_systick(3);
				LVQ_GPIO_SetPinHigh(si2c.SSCLPort, si2c.SSCLPin);
				Delay_us_systick(5);
				LVQ_GPIO_SetPinLow(si2c.SSCLPort, si2c.SSCLPin);
				Delay_us_systick(2);
				data_write <<= 1;
    }
		LVQ_GPIO_SetPinAsInput(si2c.SSDAPort, si2c.SSDAPin);
		Delay_us_systick(3);
		LVQ_GPIO_SetPinHigh(si2c.SSCLPort, si2c.SSCLPin);
		Delay_us_systick(3);
		res = LVQ_GPIO_GetInputPinValue(si2c.SSDAPort, si2c.SSDAPin);
		Delay_us_systick(2);
		LVQ_GPIO_SetPinLow(si2c.SSCLPort, si2c.SSCLPin);
		Delay_us_systick(5);
		LVQ_GPIO_SetPinAsOutput(si2c.SSDAPort, si2c.SSDAPin);
		return res;
}

uint8_t LVQ_SI2C_ReadData(LVQ_SI2C_t si2c, uint8_t ack)
{
		uint8_t reData = 0;
		LVQ_GPIO_SetPinAsInput(si2c.SSDAPort, si2c.SSDAPin);
		for( uint8_t i = 0; i < 8; i++ )
		{
				reData <<= 1;
				LVQ_GPIO_SetPinHigh(si2c.SSCLPort, si2c.SSCLPin);
				Delay_us_systick(3);
				reData |= LVQ_GPIO_GetInputPinValue(si2c.SSDAPort, si2c.SSDAPin);
				Delay_us_systick(2);
				LVQ_GPIO_SetPinLow(si2c.SSCLPort, si2c.SSCLPin);
				Delay_us_systick(5);
    }
		LVQ_GPIO_SetPinAsOutput(si2c.SSDAPort, si2c.SSDAPin);
		if( ack )
						LVQ_GPIO_SetPinHigh(si2c.SSDAPort, si2c.SSDAPin);
		else
				LVQ_GPIO_SetPinLow(si2c.SSDAPort, si2c.SSDAPin);
		Delay_us_systick(3);
		LVQ_GPIO_SetPinHigh(si2c.SSCLPort, si2c.SSCLPin);
		Delay_us_systick(5);
		LVQ_GPIO_SetPinLow(si2c.SSCLPort, si2c.SSCLPin);
		Delay_us_systick(5);
		return reData;
}

void LVQ_SI2C_Send(LVQ_SI2C_t si2c, uint8_t address, uint8_t reg, uint8_t data)
{
		LVQ_SI2C_Start(si2c);
		LVQ_SI2C_WriteData(si2c, address);
		LVQ_SI2C_WriteData(si2c, reg);
		LVQ_SI2C_WriteData(si2c, data);
		LVQ_SI2C_Stop(si2c);
}

void LVQ_SI2C_WriteMultiReg16(LVQ_SI2C_t si2c, uint8_t address, uint16_t reg, uint8_t* data, uint16_t count) 
{
    LVQ_SI2C_Start(si2c);
		LVQ_SI2C_WriteData(si2c, address);
    LVQ_SI2C_WriteData(si2c, ( reg >> 8 ) & 0xFF );
		LVQ_SI2C_WriteData(si2c, ( reg >> 0 ) & 0xFF );
    while (count--) {
        LVQ_SI2C_WriteData(si2c, *data++);
    }
    LVQ_SI2C_Stop(si2c);
}

void LVQ_SI2C_ReadMultiReg16(LVQ_SI2C_t si2c, uint8_t address, uint16_t reg, uint8_t* data, uint16_t count) 
{
		LVQ_SI2C_Start(si2c);
		LVQ_SI2C_WriteData(si2c, address);
    LVQ_SI2C_WriteData(si2c, ( reg >> 8 ) & 0xFF );
		LVQ_SI2C_WriteData(si2c, ( reg >> 0 ) & 0xFF );
		LVQ_SI2C_Stop(si2c);
		LVQ_SI2C_Start(si2c);
		LVQ_SI2C_WriteData(si2c, address | 0x01);
		while (count--) {
        if (!count) {
            /* Last byte */
            *data++ = LVQ_SI2C_ReadData(si2c, 1);
        } else {
            *data++ = LVQ_SI2C_ReadData(si2c, 0);
        }
    }
}
