/**
  ******************************************************************************

  W25Q32 For STM32F446RE
  Author:   LVQ
  Updated:  3st October 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_w25q32.h"

static void LVQ_W25Q32_Write_Enable(LVQ_W25Q32_Info_t w25q32)
{
		LVQ_GPIO_SetPinLow( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		LVQ_SPI_Send( w25q32.SPIx, Write_Enable );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
}

static void LVQ_W25Q32_Write_Disable(LVQ_W25Q32_Info_t w25q32)
{
		LVQ_GPIO_SetPinLow( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		LVQ_SPI_Send( w25q32.SPIx, Write_Disable );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
}

static void LVQ_W25Q32_Write_Enable_For_VSR(LVQ_W25Q32_Info_t w25q32)
{
		LVQ_GPIO_SetPinLow( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		LVQ_SPI_Send( w25q32.SPIx, Volatile_SR_Write_Enable );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
}

uint8_t LVQ_W25Q32_Read_Status_Register(LVQ_W25Q32_Info_t w25q32, LVQ_Read_Status_Register_t RSR)
{
		uint8_t SR = RSR;
		uint8_t res = 0;
	
		LVQ_GPIO_SetPinLow( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		switch(SR)
		{
			case LVQ_Read_Status_Register_1:
						LVQ_SPI_Send(w25q32.SPIx, Read_Status_Register_1);
						res = LVQ_SPI_Send(w25q32.SPIx, Dummy_Byte);
						break;
			case LVQ_Read_Status_Register_2:
						LVQ_SPI_Send(w25q32.SPIx, Read_Status_Register_2);
						res = LVQ_SPI_Send(w25q32.SPIx, Dummy_Byte);
						break;
			case LVQ_Read_Status_Register_3:
						LVQ_SPI_Send(w25q32.SPIx, Read_Status_Register_3);
						res = LVQ_SPI_Send(w25q32.SPIx, Dummy_Byte);
						break;
		}
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		return res;
}

void LVQ_W25Q32_Write_Status_Register(LVQ_W25Q32_Info_t w25q32, LVQ_Write_Status_Register_t WSR, uint8_t data)
{
		uint8_t SR = WSR;
	
		LVQ_GPIO_SetPinLow( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		switch(SR)
		{
			case LVQ_Write_Status_Register_1:
						LVQ_SPI_Send(w25q32.SPIx, Write_Status_Register_1);
						LVQ_SPI_Send(w25q32.SPIx, data);
						break;
			case LVQ_Write_Status_Register_2:
						LVQ_SPI_Send(w25q32.SPIx, Write_Status_Register_2);
						LVQ_SPI_Send(w25q32.SPIx, data);
						break;
			case LVQ_Write_Status_Register_3:
						LVQ_SPI_Send(w25q32.SPIx, Write_Status_Register_3);
						LVQ_SPI_Send(w25q32.SPIx, data);
						break;
		}
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
}

void LVQ_W25Q32_Init(LVQ_W25Q32_Info_t* w25q32)
{
		LVQ_GPIO_Init(w25q32->GPIO_Port_CS, w25q32->GPIO_Pin_CS, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High);
		LVQ_SPI_InitFull(w25q32->SPIx, w25q32->SPI_Pinspack, w25q32->SPI_BaudRatePrescaler, LVQ_SPI_Mode_0, SPI_Mode_Master, SPI_FirstBit_MSB);
		
		/* Info W25Q32 */
		w25q32->BlockCount = 64 ;
		w25q32->SectorSize = 4096 ; /* 4KB */
		w25q32->SectorCount = w25q32->BlockCount * 16 ;
		w25q32->PageSize = 256 ;
	  w25q32->PageCount = ( w25q32->SectorCount * w25q32->SectorSize ) / w25q32->PageSize ;
		w25q32->BlockSize = w25q32->SectorSize * 16;
		LVQ_W25Q32_UniqueID(w25q32);
}

void LVQ_W25Q32_WriteByte(LVQ_W25Q32_Info_t w25q32, uint32_t address, uint8_t data)
{
		LVQ_W25Q32_Write_Enable(w25q32);
	
		Delay_ms_systick(1);
	
		LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32.SPIx, Page_Program );
		LVQ_SPI_Send( w25q32.SPIx, address >> 16 );
		LVQ_SPI_Send( w25q32.SPIx, address >> 8 );
		LVQ_SPI_Send( w25q32.SPIx, address );
		LVQ_SPI_Send( w25q32.SPIx, data );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		
		Delay_ms_systick(1);
}

void LVQ_W25Q32_WriteData(LVQ_W25Q32_Info_t w25q32, uint32_t address, uint8_t* DataOut, uint16_t count)
{
		LVQ_W25Q32_Write_Enable(w25q32);
	
		Delay_ms_systick(1);
	
		LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32.SPIx, Page_Program );
		LVQ_SPI_Send( w25q32.SPIx, address >> 16 );
		LVQ_SPI_Send( w25q32.SPIx, address >> 8 );
		LVQ_SPI_Send( w25q32.SPIx, address );
		LVQ_SPI_WriteMulti( w25q32.SPIx, DataOut, count );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		
		Delay_ms_systick(3);
}

void LVQ_W25Q32_WritePage(LVQ_W25Q32_Info_t w25q32, uint32_t page_number, uint8_t offset_bytes, uint8_t* DataOut, uint16_t CountDataWriteBytes)
{
		uint16_t CountNumber = 0;
		if( offset_bytes > w25q32.PageSize )
				return;
		if( ( offset_bytes + CountDataWriteBytes ) > w25q32.PageSize )
					CountNumber = w25q32.PageSize - offset_bytes;
		else
					CountNumber = CountDataWriteBytes;
		
		uint32_t page_address = ( page_number * w25q32.PageSize ) + offset_bytes;
		
 		LVQ_W25Q32_Write_Enable(w25q32);
		
		Delay_ms_systick(1);
		LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32.SPIx, Page_Program );
		LVQ_SPI_Send( w25q32.SPIx, page_address >> 16 );
		LVQ_SPI_Send( w25q32.SPIx, page_address >> 8 );
		LVQ_SPI_Send( w25q32.SPIx, page_address );
		LVQ_SPI_WriteMulti( w25q32.SPIx, DataOut, CountNumber );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		
		Delay_ms_systick(4);
}

void LVQ_W25Q32_WriteSector(LVQ_W25Q32_Info_t w25q32, uint32_t sector_number, uint16_t offset_bytes, uint8_t* DataOut, uint16_t CountDataWriteBytes)
{
		int32_t CountNumber = 0;
		if( offset_bytes > w25q32.SectorSize )
				return;
	
		if( ( offset_bytes + CountDataWriteBytes ) > w25q32.SectorSize )
					CountNumber = w25q32.SectorSize - offset_bytes;
		else
					CountNumber = CountDataWriteBytes;
		
		uint32_t page_number = ( sector_number * w25q32.SectorSize ) / w25q32.PageSize + offset_bytes / w25q32.PageSize; 
		uint32_t local_offset = offset_bytes % w25q32.PageSize ;
		
		do
		{
				LVQ_W25Q32_WritePage(w25q32, page_number, local_offset, DataOut, CountNumber);
				page_number++;
				CountNumber -= w25q32.PageSize - local_offset;
				DataOut += w25q32.PageSize - local_offset;
				local_offset = 0;
		} while (CountNumber > 0);
}

void LVQ_W25Q32_WriteBlock(LVQ_W25Q32_Info_t w25q32, uint32_t block_number, uint32_t offset_bytes, uint8_t* DataOut, uint32_t CountDataWriteBytes)
{
		int32_t CountNumber = 0;
		if( offset_bytes > w25q32.BlockSize )
				return;
	
		if( ( offset_bytes + CountDataWriteBytes ) > w25q32.BlockSize )
					CountNumber = w25q32.BlockSize - offset_bytes;
		else
					CountNumber = CountDataWriteBytes;
		
		uint32_t page_number = ( block_number * w25q32.BlockSize ) / w25q32.PageSize + offset_bytes / w25q32.PageSize ; 
		uint32_t local_offset = offset_bytes % w25q32.PageSize ;
		
		do
		{
				LVQ_W25Q32_WritePage(w25q32, page_number, local_offset, DataOut, CountNumber);
				page_number++;
				CountNumber -= w25q32.PageSize - local_offset;
				DataOut += w25q32.PageSize - local_offset;
				local_offset = 0;
		} while (CountNumber > 0);
}

void LVQ_W25Q32_ReadByte(LVQ_W25Q32_Info_t w25q32, uint32_t address, uint8_t* DataIn)
{
		LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32.SPIx, Read_Data );
		LVQ_SPI_Send( w25q32.SPIx, address >> 16 );
		LVQ_SPI_Send( w25q32.SPIx, address >> 8 );
		LVQ_SPI_Send( w25q32.SPIx, address );
		LVQ_SPI_ReadMulti( w25q32.SPIx, DataIn, 0, 1 );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		
		Delay_ms_systick(1);
}

void LVQ_W25Q32_ReadBytes(LVQ_W25Q32_Info_t w25q32, uint32_t address, uint8_t* DataIn, uint32_t count)
{
		LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32.SPIx, Read_Data );
		LVQ_SPI_Send( w25q32.SPIx, address >> 16 );
		LVQ_SPI_Send( w25q32.SPIx, address >> 8 );
		LVQ_SPI_Send( w25q32.SPIx, address );
		LVQ_SPI_ReadMulti( w25q32.SPIx, DataIn, 0, count );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		
		Delay_ms_systick(1);
}

void LVQ_W25Q32_ReadData(LVQ_W25Q32_Info_t w25q32, uint32_t address, uint8_t* DataIn, uint32_t count)
{
		LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32.SPIx, Read_Data );
		LVQ_SPI_Send( w25q32.SPIx, address >> 16 );
		LVQ_SPI_Send( w25q32.SPIx, address >> 8 );
		LVQ_SPI_Send( w25q32.SPIx, address );
		LVQ_SPI_ReadMulti( w25q32.SPIx, DataIn, 0, count );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		
		Delay_ms_systick(2);
}

void LVQ_W25Q32_ReadPage(LVQ_W25Q32_Info_t w25q32, uint32_t page_number, uint8_t offset_bytes, uint8_t* DataIn, uint16_t CountDataReadBytes)
{
		uint16_t CountNumber = 0;
		if( offset_bytes > w25q32.PageSize )
				return;
		if( ( offset_bytes + CountDataReadBytes ) > w25q32.PageSize )
					CountNumber = w25q32.PageSize - offset_bytes;
		else
					CountNumber = CountDataReadBytes;
		
		uint32_t page_address = ( page_number * w25q32.PageSize ) + offset_bytes;
		
		LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32.SPIx, Read_Data );
		LVQ_SPI_Send( w25q32.SPIx, page_address >> 16 );
		LVQ_SPI_Send( w25q32.SPIx, page_address >> 8 );
		LVQ_SPI_Send( w25q32.SPIx, page_address );
		LVQ_SPI_ReadMulti( w25q32.SPIx, DataIn, 0 , CountNumber );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		
		Delay_ms_systick(3);
}

void LVQ_W25Q32_ReadSector(LVQ_W25Q32_Info_t w25q32, uint32_t sector_number, uint16_t offset_bytes, uint8_t* DataIn, uint16_t CountDataReadBytes)
{
		int32_t CountNumber = 0;
		if( offset_bytes > w25q32.SectorSize )
				return;
	
		if( ( offset_bytes + CountDataReadBytes ) > w25q32.SectorSize )
					CountNumber = w25q32.SectorSize - offset_bytes;
		else
					CountNumber = CountDataReadBytes;
		
		uint32_t page_number = ( sector_number * w25q32.SectorSize ) / w25q32.PageSize + offset_bytes / w25q32.PageSize; 
		uint32_t local_offset = offset_bytes % w25q32.PageSize ;
		
		do
		{
				LVQ_W25Q32_ReadPage(w25q32, page_number, local_offset, DataIn, CountNumber);
				page_number++;
				CountNumber -= w25q32.PageSize - local_offset;
				DataIn += w25q32.PageSize - local_offset;
				local_offset = 0;
		} while (CountNumber > 0);
}

void LVQ_W25Q32_ReadBlock(LVQ_W25Q32_Info_t w25q32, uint32_t block_number, uint32_t offset_bytes, uint8_t* DataIn, uint32_t CountDataReadBytes)
{
		int32_t CountNumber = 0;
		if( offset_bytes > w25q32.BlockSize )
				return;
	
		if( ( offset_bytes + CountDataReadBytes ) > w25q32.BlockSize )
					CountNumber = w25q32.BlockSize - offset_bytes;
		else
					CountNumber = CountDataReadBytes;
		
		uint32_t page_number = ( block_number * w25q32.BlockSize ) / w25q32.PageSize + offset_bytes / w25q32.PageSize ; 
		uint32_t local_offset = offset_bytes % w25q32.PageSize ;
		
		do
		{
				LVQ_W25Q32_ReadPage(w25q32, page_number, local_offset, DataIn, CountNumber);
				page_number++;
				CountNumber -= w25q32.PageSize - local_offset;
				DataIn += w25q32.PageSize - local_offset;
				local_offset = 0;
		} while (CountNumber > 0);
}

void LVQ_W25Q32_FastReadData(LVQ_W25Q32_Info_t w25q32, uint32_t address, uint8_t* DataIn, uint32_t count)
{
		LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32.SPIx, Fast_Read );
		LVQ_SPI_Send( w25q32.SPIx, address >> 16 );
		LVQ_SPI_Send( w25q32.SPIx, address >> 8 );
		LVQ_SPI_Send( w25q32.SPIx, address );
		LVQ_SPI_Send( w25q32.SPIx, Dummy_Byte );
		LVQ_SPI_SendMulti( w25q32.SPIx, 0, DataIn, count );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		
		Delay_ms_systick(4);
}

void LVQ_W25Q32_EraseData(LVQ_W25Q32_Info_t w25q32, uint32_t address, LVQ_Erase_Type_t Erase_Type)
{
		uint8_t ET = Erase_Type;
	
		LVQ_W25Q32_Write_Enable(w25q32);
	
		Delay_ms_systick(1);
	
		switch(ET)
		{
			case LVQ_W25Q32_Erase_4KB:
				
						LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
						LVQ_SPI_Send( w25q32.SPIx, Sector_Erase_4KB );
						LVQ_SPI_Send( w25q32.SPIx, address >> 16 );
						LVQ_SPI_Send( w25q32.SPIx, address >> 8 );
						LVQ_SPI_Send( w25q32.SPIx, address );
						LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
						Delay_ms_systick(400);
			
						break;
			case LVQ_W25Q32_Erase_32KB:
				
						LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
						LVQ_SPI_Send( w25q32.SPIx, Block_Erase_32KB );
						LVQ_SPI_Send( w25q32.SPIx, address >> 16 );
						LVQ_SPI_Send( w25q32.SPIx, address >> 8 );
						LVQ_SPI_Send( w25q32.SPIx, address );
						LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
						Delay_ms_systick(1600);
			
						break;
			case LVQ_W25Q32_Erase_64KB:
				
						LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
						LVQ_SPI_Send( w25q32.SPIx, Block_Erase_64KB );
						LVQ_SPI_Send( w25q32.SPIx, address >> 16 );
						LVQ_SPI_Send( w25q32.SPIx, address >> 8 );
						LVQ_SPI_Send( w25q32.SPIx, address );
						LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
						Delay_ms_systick(2000);
			
						break;
		}
}

void LVQ_W25Q32_Chip_Erase(LVQ_W25Q32_Info_t w25q32)
{
		LVQ_W25Q32_Write_Enable(w25q32);
	
		Delay_ms_systick(1);
	
		LVQ_GPIO_SetPinLow(w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32.SPIx, Chip_Erase );
		LVQ_GPIO_SetPinHigh( w25q32.GPIO_Port_CS, w25q32.GPIO_Pin_CS );
		
		Delay_ms_systick(50000);
}	

void LVQ_W25Q32_DeviceID(LVQ_W25Q32_Info_t* w25q32)
{
		LVQ_GPIO_SetPinLow(w25q32->GPIO_Port_CS, w25q32->GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32->SPIx, Manufacturer_Device_ID );
		for (uint8_t i = 0; i < 3; i++)
				LVQ_SPI_Send( w25q32->SPIx, 0x00 );
		for (uint8_t i = 0; i < 2; i++)
				w25q32->DeviID[i] = LVQ_SPI_Send( w25q32->SPIx, 0x00 );
		LVQ_GPIO_SetPinHigh(w25q32->GPIO_Port_CS, w25q32->GPIO_Pin_CS);
}

void LVQ_W25Q32_UniqueID(LVQ_W25Q32_Info_t* w25q32)
{
		LVQ_GPIO_SetPinLow(w25q32->GPIO_Port_CS, w25q32->GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32->SPIx, Read_Unique_ID );
		for (uint8_t i = 0; i < 4; i++)
				LVQ_SPI_Send( w25q32->SPIx, Dummy_Byte );
		for (uint8_t i = 0; i < 8; i++)
				w25q32->UniqID[i] = LVQ_SPI_Send( w25q32->SPIx, Dummy_Byte );
		LVQ_GPIO_SetPinHigh( w25q32->GPIO_Port_CS, w25q32->GPIO_Pin_CS );
}	

void LVQ_W25Q32_JEDEC_ID(LVQ_W25Q32_Info_t* w25q32)
{
		LVQ_GPIO_SetPinLow(w25q32->GPIO_Port_CS, w25q32->GPIO_Pin_CS);
		LVQ_SPI_Send( w25q32->SPIx, JEDEC_ID );
		w25q32->JEDECID[0]	=	LVQ_SPI_Send( w25q32->SPIx, Dummy_Byte );
		w25q32->JEDECID[1]	=	LVQ_SPI_Send( w25q32->SPIx, Dummy_Byte );
		w25q32->JEDECID[2]	=	LVQ_SPI_Send( w25q32->SPIx, Dummy_Byte );
		LVQ_GPIO_SetPinHigh( w25q32->GPIO_Port_CS, w25q32->GPIO_Pin_CS );
}	

void LVQ_W25Q32_WaitForWriteEnd(LVQ_W25Q32_Info_t w25q32)
{
		uint8_t res;
		do
		{
			res = LVQ_W25Q32_Read_Status_Register( w25q32, LVQ_Read_Status_Register_1 );
			Delay_ms_systick(1);
		} while ((res & 0x01) == 0x01);
}

uint8_t LVQ_W25Q32_IsEmptyPage(LVQ_W25Q32_Info_t w25q32, uint32_t page_number, uint8_t offset_bytes, uint16_t CountDataCheckBytes )
{
		if( offset_bytes > w25q32.PageSize )
				return 0;
		if( ( offset_bytes + CountDataCheckBytes ) > w25q32.PageSize )
					CountDataCheckBytes = w25q32.PageSize - offset_bytes;

		uint8_t BufferCheck[32];
		uint32_t i = 0;
		
		for( i = offset_bytes; i < w25q32.PageSize; i += sizeof(BufferCheck) )
		{		
				LVQ_W25Q32_ReadPage(w25q32, page_number, i, BufferCheck, sizeof(BufferCheck) );
				for (uint8_t x = 0; x < sizeof(BufferCheck); x++)
				{
					if (BufferCheck[x] != 0xFF)
						goto NOT_EMPTY;
				}
		}
		
		if( ( w25q32.PageSize - offset_bytes ) % sizeof(BufferCheck) != 0 )
		{
				i -= sizeof(BufferCheck);
				for( ; i < w25q32.PageSize; i++ )
				{
						LVQ_W25Q32_ReadByte(w25q32, i + page_number * w25q32.PageSize, BufferCheck);
						if ( BufferCheck[0] != 0xFF )
								goto NOT_EMPTY;
				}
		}

		return 1;
NOT_EMPTY:
		return 0;
}

uint8_t LVQ_W25Q32_IsEmptySector(LVQ_W25Q32_Info_t w25q32, uint32_t sector_number, uint8_t offset_bytes, uint16_t CountDataCheckBytes )
{
		if( offset_bytes > w25q32.SectorSize )
				return 0;
		if( ( offset_bytes + CountDataCheckBytes ) > w25q32.SectorSize )
					CountDataCheckBytes = w25q32.SectorSize - offset_bytes;

		uint8_t BufferCheck[32];
		uint32_t i = 0;
		
		for( i = offset_bytes; i < w25q32.SectorSize; i += sizeof(BufferCheck) )
		{		
				LVQ_W25Q32_ReadData(w25q32, i + sector_number * w25q32.SectorSize, BufferCheck, sizeof(BufferCheck) );
				for (uint8_t x = 0; x < sizeof(BufferCheck); x++)
				{
					if (BufferCheck[x] != 0xFF)
						goto NOT_EMPTY;
				}
		}
		
		if( ( w25q32.SectorSize - offset_bytes ) % sizeof(BufferCheck) != 0 )
		{
				i -= sizeof(BufferCheck);
				for( ; i < w25q32.SectorSize; i++ )
				{
						LVQ_W25Q32_ReadByte(w25q32, i + sector_number * w25q32.SectorSize, BufferCheck);
						if ( BufferCheck[0] != 0xFF )
								goto NOT_EMPTY;
				}
		}

		return 1;
NOT_EMPTY:
		return 0;
}

uint8_t LVQ_W25Q32_IsEmptyBlock(LVQ_W25Q32_Info_t w25q32, uint32_t block_number, uint32_t offset_bytes, uint32_t CountDataCheckBytes )
{
		if( offset_bytes > w25q32.BlockSize )
				return 0;
		if( ( offset_bytes + CountDataCheckBytes ) > w25q32.BlockSize )
					CountDataCheckBytes = w25q32.BlockSize - offset_bytes;

		uint8_t BufferCheck[32];
		uint32_t i = 0;
		
		for( i = offset_bytes; i < w25q32.BlockSize; i += sizeof(BufferCheck) )
		{		
				LVQ_W25Q32_ReadData(w25q32, i + block_number * w25q32.BlockSize, BufferCheck, sizeof(BufferCheck) );
				for (uint8_t x = 0; x < sizeof(BufferCheck); x++)
				{
					if (BufferCheck[x] != 0xFF)
						goto NOT_EMPTY;
				}
		}
		
		if( ( w25q32.BlockSize - offset_bytes ) % sizeof(BufferCheck) != 0 )
		{
				i -= sizeof(BufferCheck);
				for( ; i < w25q32.BlockSize; i++ )
				{
						LVQ_W25Q32_ReadByte(w25q32, i + block_number * w25q32.BlockSize, BufferCheck);
						if ( BufferCheck[0] != 0xFF )
								goto NOT_EMPTY;
				}
		}

		return 1;
NOT_EMPTY:
		return 0;
}
