/**
  ******************************************************************************

  SOFT SPI For STM32F446RE
  Author:   LVQ
  Updated:  5st April 2023
	Algorithm: 

  ******************************************************************************
*/

#include "lvq_stm32f1_soft_spi.h"

#define SPEED_SSPI 					1
#define BITMASK_DATASIZE8  	0x80
#define BITMASK_DATASIZE16 	0x8000

/**
 * @brief  Init Config Soft SPI
 * @param  sspi: struct soft spi contain infomation soft spi and slave  
 * @retval not return
 */
void LVQ_SSPI_Init(LVQ_SSPI_t sspi)
{
		LVQ_GPIO_Init(sspi.SMOSIPort, sspi.SMOSIPin, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_Fast);
		LVQ_GPIO_Init(sspi.SMISOPort, sspi.SMISOPin, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_Fast);
		LVQ_GPIO_Init(sspi.SSCKPort, sspi.SSCKPin, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_Fast);
		//LVQ_GPIO_Init(sspi.NSSPort, sspi.NSSPin, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_Fast);

		uint8_t mode = sspi.SSPI_Mode;
		switch(mode)
		{
			case LVQ_SSPI_Mode_0:
					LVQ_GPIO_SetPinLow(sspi.SSCKPort, sspi.SSCKPin);
					break;
			case LVQ_SSPI_Mode_1:
					LVQ_GPIO_SetPinLow(sspi.SSCKPort, sspi.SSCKPin);
					break;
			case LVQ_SSPI_Mode_2:
					LVQ_GPIO_SetPinHigh(sspi.SSCKPort, sspi.SSCKPin);
					break;
			case LVQ_SSPI_Mode_3:
					LVQ_GPIO_SetPinHigh(sspi.SSCKPort, sspi.SSCKPin);
					break;
			default:
					break;
		}
		//LVQ_GPIO_SetPinHigh(sspi.NSSPort, sspi.NSSPin);
}

/**
 * @brief  Send Byte Data With Data Size 8 or 16 Bit
 * @param  sspi: struct soft spi contain infomation soft spi and slave 
 * @param  data_write : data write can 8 or 16 bit data depends on config 
 * @retval value receive from slave ca 8 or 16 bit data depends on config 
 */
uint16_t LVQ_SSPI_SendByte(LVQ_SSPI_t sspi, uint16_t data_write)
{
		uint8_t mode = sspi.SSPI_Mode, dataSize = 8;
		uint16_t reData = 0, bitmask = BITMASK_DATASIZE8;
		volatile uint8_t preData, Data;
		preData = (uint8_t) data_write;
		if( sspi.SSPI_DataSize == LVQ_SSPI_DataSize_16)
		{
				dataSize = 16;
				bitmask = BITMASK_DATASIZE16;
		}
		for( uint8_t i = 0; i < dataSize; i++)
		{
			  switch(mode)
				{
					case LVQ_SSPI_Mode_0:
							if( ( data_write & bitmask ) > 0 )
										LVQ_GPIO_SetPinHigh(sspi.SMOSIPort, sspi.SMOSIPin);
							else
										LVQ_GPIO_SetPinLow(sspi.SMOSIPort, sspi.SMOSIPin);
							Delay_us(SPEED_SSPI);
							LVQ_GPIO_SetPinHigh(sspi.SSCKPort, sspi.SSCKPin);
							reData |= LVQ_GPIO_GetInputPinValue(sspi.SMISOPort, sspi.SMISOPin);
							Delay_us(SPEED_SSPI);
							LVQ_GPIO_SetPinLow(sspi.SSCKPort, sspi.SSCKPin);
							reData <<= 1;
							data_write <<= 1;
							break;
					case LVQ_SSPI_Mode_1:
							if( ( data_write & bitmask ) > 0 )
										LVQ_GPIO_SetPinHigh(sspi.SMOSIPort, sspi.SMOSIPin);
							else
										LVQ_GPIO_SetPinLow(sspi.SMOSIPort, sspi.SMOSIPin);
							LVQ_GPIO_SetPinHigh(sspi.SSCKPort, sspi.SSCKPin);
							Delay_us(SPEED_SSPI);
							LVQ_GPIO_SetPinLow(sspi.SSCKPort, sspi.SSCKPin);
							reData |= LVQ_GPIO_GetInputPinValue(sspi.SMISOPort, sspi.SMISOPin);
							Delay_us(SPEED_SSPI);
							reData <<= 1;
							data_write <<= 1;
							Delay_us(5);
							break;
					case LVQ_SSPI_Mode_2:
							if( ( data_write & bitmask ) > 0 )
										LVQ_GPIO_SetPinHigh(sspi.SMOSIPort, sspi.SMOSIPin);
							else
										LVQ_GPIO_SetPinLow(sspi.SMOSIPort, sspi.SMOSIPin);
							Delay_us(SPEED_SSPI);
							LVQ_GPIO_SetPinLow(sspi.SSCKPort, sspi.SSCKPin);
							reData |= LVQ_GPIO_GetInputPinValue(sspi.SMISOPort, sspi.SMISOPin);
							Delay_us(SPEED_SSPI);
							LVQ_GPIO_SetPinHigh(sspi.SSCKPort, sspi.SSCKPin);
							reData <<= 1;
							data_write <<= 1;
							break;
					case LVQ_SSPI_Mode_3:
							LVQ_GPIO_SetPinLow(sspi.SSCKPort, sspi.SSCKPin);
							if( ( data_write & bitmask ) > 0 )
										LVQ_GPIO_SetPinHigh(sspi.SMOSIPort, sspi.SMOSIPin);
							else
										LVQ_GPIO_SetPinLow(sspi.SMOSIPort, sspi.SMOSIPin);
							
							Delay_us(SPEED_SSPI);
							reData |= LVQ_GPIO_GetInputPinValue(sspi.SMISOPort, sspi.SMISOPin);
							LVQ_GPIO_SetPinHigh(sspi.SSCKPort, sspi.SSCKPin);
							Delay_us(SPEED_SSPI);
							reData <<= 1;
							data_write <<= 1;
							break;
					default:
							break;
				}
		}
		Data = (uint8_t) reData;
		return reData >> 1;
}

/**
 * @brief  Send And Receive Multi Data With Data Size 8 Bit
 * @param  sspi: struct soft spi contain infomation soft spi and slave 
 * @param  dataOut: data send to slave
 * @param  dataIn: data receive to slave
 * @param  count: number data send to slave
 * @retval not return
 */
void LVQ_SSPI_SendMulti(LVQ_SSPI_t sspi, uint8_t* dataOut, uint8_t* dataIn, uint32_t count)
{
		LVQ_GPIO_SetPinLow(sspi.NSSPort, sspi.NSSPin);
		for( uint32_t i = 0; i < count; i++ )
		{
				dataIn[i] = LVQ_SSPI_SendByte( sspi, dataOut[i] ) & 0xFF;
    }
		LVQ_GPIO_SetPinHigh(sspi.NSSPort, sspi.NSSPin);
}

/**
 * @brief  Send Multi Data With Data Size 8 Bit
 * @param  sspi: struct soft spi contain infomation soft spi and slave 
 * @param  dataOut: data send to slave
 * @param  count: number data send to slave
 * @retval not return
 */
void LVQ_SSPI_WriteMulti(LVQ_SSPI_t sspi, uint8_t* dataOut, uint32_t count)
{
		LVQ_GPIO_SetPinLow(sspi.NSSPort, sspi.NSSPin);
		for( uint32_t i = 0; i < count; i++ )
		{
				LVQ_SSPI_SendByte( sspi, dataOut[i] );
    }
		LVQ_GPIO_SetPinHigh(sspi.NSSPort, sspi.NSSPin);
}

/**
 * @brief  Receive Multi Data With Data Size 8 Bit
 * @param  sspi: struct soft spi contain infomation soft spi and slave 
 * @param  dataIn: data receive to slave
 * @param  dummy: data send while read data from slave
 * @param  count: number data send to slave
 * @retval not return
 */
void LVQ_SSPI_ReadMulti(LVQ_SSPI_t sspi, uint8_t* dataIn, uint8_t dummy, uint32_t count)
{
		LVQ_GPIO_SetPinLow(sspi.NSSPort, sspi.NSSPin);
		for( uint32_t i = 0; i < count; i++ )
		{
				dataIn[i] = LVQ_SSPI_SendByte( sspi, dummy ) & 0xFF;
    }
		LVQ_GPIO_SetPinHigh(sspi.NSSPort, sspi.NSSPin);
}

/**
 * @brief  Send And Receive Multi Data With Data Size 16 Bit
 * @param  sspi: struct soft spi contain infomation soft spi and slave 
 * @param  dataOut: data send to slave
 * @param  dataIn: data receive to slave
 * @param  count: number data send to slave
 * @retval not return
 */
void LVQ_SSPI_SendMulti16(LVQ_SSPI_t sspi, uint16_t* dataOut, uint16_t* dataIn, uint32_t count)
{
		LVQ_GPIO_SetPinLow(sspi.NSSPort, sspi.NSSPin);
		for( uint32_t i = 0; i < count; i++ )
		{
				dataIn[i] = LVQ_SSPI_SendByte( sspi, dataOut[i] ) & 0xFFFF;
    }
		LVQ_GPIO_SetPinHigh(sspi.NSSPort, sspi.NSSPin);
}

/**
 * @brief  Send Multi Data With Data Size 16 Bit
 * @param  sspi: struct soft spi contain infomation soft spi and slave 
 * @param  dataOut: data send to slave
 * @param  count: number data send to slave
 * @retval not return
 */
void LVQ_SSPI_WriteMulti16(LVQ_SSPI_t sspi, uint16_t* dataOut, uint32_t count)
{
		LVQ_GPIO_SetPinLow(sspi.NSSPort, sspi.NSSPin);
		for( uint32_t i = 0; i < count; i++ )
		{
				LVQ_SSPI_SendByte( sspi, dataOut[i] );
    }
		LVQ_GPIO_SetPinHigh(sspi.NSSPort, sspi.NSSPin);
}

/**
 * @brief  Receive Multi Data With Data Size 16 Bit
 * @param  sspi: struct soft spi contain infomation soft spi and slave 
 * @param  dataIn: data receive to slave
 * @param  dummy: data send while read data from slave
 * @param  count: number data send to slave
 * @retval not return
 */
void LVQ_SSPI_ReadMulti16(LVQ_SSPI_t sspi, uint16_t* dataIn, uint16_t dummy, uint32_t count)
{
		LVQ_GPIO_SetPinLow(sspi.NSSPort, sspi.NSSPin);
		for( uint32_t i = 0; i < count; i++ )
		{
				dataIn[i] = LVQ_SSPI_SendByte( sspi, dummy ) & 0xFFFF;
    }
		LVQ_GPIO_SetPinHigh(sspi.NSSPort, sspi.NSSPin);
}
