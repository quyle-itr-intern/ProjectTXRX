/**
  ******************************************************************************

  FLASH For STM32F446RE
  Author:   LVQ
  Updated:  23st August 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_flash.h"

#define KEY1 				(uint32_t) 0x45670123
#define KEY2        (uint32_t) 0xCDEF89AB

uint8_t LVQ_GetSectorFromAddress(volatile uint32_t u32StartAddr)
{
		uint8_t Sector = 0;
		if( (u32StartAddr >= 0x08000000) && (u32StartAddr < 0x08003FFF) )
				Sector = LVQ_FLASH_SECTOR_0;
		else if( (u32StartAddr >= 0x08004000) && (u32StartAddr < 0x08007FFF) )
				Sector = LVQ_FLASH_SECTOR_1;
		else if( (u32StartAddr >= 0x08008000) && (u32StartAddr < 0x0800BFFF) )
				Sector = LVQ_FLASH_SECTOR_2;
		else if( (u32StartAddr >= 0x0800C000) && (u32StartAddr < 0x0800FFFF) )
				Sector = LVQ_FLASH_SECTOR_3;
		else if( (u32StartAddr >= 0x08010000) && (u32StartAddr < 0x0801FFFF) )
				Sector = LVQ_FLASH_SECTOR_4;
		else if( (u32StartAddr >= 0x08020000) && (u32StartAddr < 0x0803FFFF) )
				Sector = LVQ_FLASH_SECTOR_5;
		else if( (u32StartAddr >= 0x08040000) && (u32StartAddr < 0x0805FFFF) )
				Sector = LVQ_FLASH_SECTOR_6;
		else if( (u32StartAddr >= 0x08060000) && (u32StartAddr < 0x0807FFFF) )
				Sector = LVQ_FLASH_SECTOR_7;
		return Sector;
}

uint32_t LVQ_GetAddressFromSector(uint8_t u8Sector)
{
		uint32_t Address = 0;
		if( u8Sector == 0 )
				Address = 0x08000000;
		else if( u8Sector == 1 )
				Address = 0x08004000;
		else if( u8Sector == 2 )
				Address = 0x08008000;
		else if( u8Sector == 3 )
				Address = 0x0800C000;
		else if( u8Sector == 4 )
				Address = 0x08010000;
		else if( u8Sector == 5 )
				Address = 0x08020000;
		else if( u8Sector == 6 )
				Address = 0x08040000;
		else if( u8Sector == 7 )
				Address = 0x08060000;
		return Address;
}

void LVQ_Flash_Unlock(void)
{
		FLASH->KEYR = KEY1;
		FLASH->KEYR = KEY2;
}

LVQ_Flash_Status_t LVQ_Flash_Erase(volatile uint32_t u32StartAddr)
{
		uint32_t Timeout = LVQ_FLASH_TIMEOUT;
		/* Check that no Flash memory operation is ongoing by checking the BSY bit in the FLASH CR register */
		while ( ((FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY) && Timeout )
		{
				if (--Timeout == 0x00) {
            return LVQ_Flash_Status_Errors;
        }
		}
		/* Wating for Bsy bit */
		/* Check unlock sequences */
		if( (FLASH->CR & FLASH_CR_LOCK) ==  FLASH_CR_LOCK)
		{
				LVQ_Flash_Unlock();
		}

		FLASH->CR |= FLASH_PSIZE_WORD;
		/* Set the SER bit in the FLASH_CR register */
		FLASH->CR |= FLASH_CR_SER;
		/* Program the FLASH_CR register to select a sector to erase */
		FLASH->CR |= LVQ_GetSectorFromAddress(u32StartAddr) << 3;
		/* Set the STRT bit in the FLASH CR register */
		FLASH->CR |= FLASH_CR_STRT;
		/* Wait for the BSY bit to be reset */
		Timeout = LVQ_FLASH_TIMEOUT;
		while ( ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY) && Timeout)
		{
				/* Wating for Bsy bit */
				if (--Timeout == 0x00) {
            return LVQ_Flash_Status_Errors;
        }
		}
	 
		/* Check FLASH End of Operation flag  */
		if ((FLASH->SR & FLASH_FLAG_EOP) != RESET)
		{
				/* Clear FLASH End of Operation pending bit */
				FLASH->SR &= ~FLASH_FLAG_EOP;
		}
		/* Clear SER bit in the FLASH_CR register */
		FLASH->CR &= FLASH_CR_SER;
		/* Clear STRT bit in the FLASH CR register */
		FLASH->CR &= FLASH_CR_STRT;
		
		/* Set the LOCK Bit to lock the FLASH Registers access */
		FLASH->CR |= FLASH_CR_LOCK;
		
		return LVQ_Flash_Status_Ok;
}

LVQ_Flash_Status_t LVQ_Flash_Write(volatile uint32_t u32StartAddr, uint32_t* u32BufferWrite, uint32_t u32Length)
{
		uint32_t u32Count = 0u;
		uint32_t Timeout = LVQ_FLASH_TIMEOUT;
		/* Check input parameters */
		if( (u32BufferWrite == 0u) || (u32Length == 0u) )
		{
			 return LVQ_Flash_Status_Errors;
		}
		
		/* Check thet no Flash memory ope ration is ongoing by checking the BSY bit in the FLASH_CR register*/
		while ( ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY) && Timeout)
		{
				/* Wating for Bsy bit */
				if (--Timeout == 0x00) {
            return LVQ_Flash_Status_Errors;
        }
		}
		/* Check unlock sequences */
		if( (FLASH->CR & FLASH_CR_LOCK) == FLASH_CR_LOCK )
		{
			LVQ_Flash_Unlock();
		}
		FLASH->CR |= FLASH_PSIZE_WORD;
		/* Write FLASH CR_PG to 1 */
		FLASH->CR |= FLASH_CR_PG;
		/* Perform word write at the desired address */
		for(u32Count = 0; u32Count < u32Length; u32Count++ )
		{
				*(__IO uint32_t*) (u32StartAddr + u32Count*4u) = *(uint32_t*) (u32BufferWrite + u32Count);
		}
		/* Wait for the BSY bit to be reset */
		Timeout = LVQ_FLASH_TIMEOUT;
		while ( ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY) && Timeout)
		{
				/* Wating for Bsy bit */
				if (--Timeout == 0x00) {
            return LVQ_Flash_Status_Errors;
        }
		}
		/* Clear PG bit in the FLASH CR register */
		FLASH->CR &= FLASH_CR_PG;
		
		/* Set the LOCK Bit to lock the FLASH Registers access */
		FLASH->CR |= FLASH_CR_LOCK;
  
		return LVQ_Flash_Status_Ok;
}

LVQ_Flash_Status_t LVQ_Flash_Read(volatile uint32_t u32startAddr, uint32_t* u32BuffeRead, uint32_t u32Length)
{
	uint32_t u32Count = 0u;
	/* Check input paras */
	if((u32BuffeRead == 0u) || (u32Length == 0u))
	{
		return LVQ_Flash_Status_Errors;
	}
	/* Read data buffer */
	for(u32Count = 0u; u32Count < u32Length; u32Count++ )
	{
		*(uint32_t*) (u32BuffeRead + u32Count) = *(uint32_t*) (u32startAddr + u32Count*4u);
	}
	return LVQ_Flash_Status_Ok;
}

LVQ_Flash_Status_t LVQ_Flash_WriteAndErase(volatile uint32_t u32StartAddr, uint32_t* u32BufferWrite, uint32_t u32Length)
{
		uint16_t u32Count = 0u;
		uint16_t Timeout = LVQ_FLASH_TIMEOUT;
		/* Check input parameters */
		if( (u32BufferWrite == 0u) || (u32Length == 0u) )
		{
			 return LVQ_Flash_Status_Errors;
		}
		
		/* --- BEGIN ERASE SECTOR WIRTE DATA TO FLASH MEMORY  --- */
		
		/* Get the number of sector to erase */
		uint8_t StartSector = LVQ_GetSectorFromAddress(u32StartAddr);
	  uint8_t EndSector = LVQ_GetSectorFromAddress(u32StartAddr + u32Length*4);
		uint8_t NumberSector = EndSector - StartSector + 1;
		
		/* Erase sector for write data */
		for(uint8_t i = 0; i < NumberSector; i++)
		{
				LVQ_Flash_Erase(LVQ_GetAddressFromSector(StartSector));
				StartSector++;
		}
		
		/* --- END ERASE SECTOR WIRTE DATA TO FLASH MEMORY  --- */
		
		/* --- BEGIN WRITE DATA TO FLASH MEMORY --- */
		
		/* Check thet no Flash memory ope ration is ongoing by checking the BSY bit in the FLASH_CR register*/
		while ( ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY) && Timeout)
		{
				/* Wating for Bsy bit */
				if (--Timeout == 0x00) {
            return LVQ_Flash_Status_Errors;
        }
		}
		/* Check unlock sequences */
		if( (FLASH->CR & FLASH_CR_LOCK) == FLASH_CR_LOCK )
		{
			LVQ_Flash_Unlock();
		}
		FLASH->CR |= FLASH_PSIZE_WORD;
		/* Write FLASH CR_PG to 1 */
		FLASH->CR |= FLASH_CR_PG;
		/* Perform word write at the desired address */
		for(u32Count = 0; u32Count < u32Length; u32Count++ )
		{
				*(__IO uint32_t*) (u32StartAddr + u32Count*4u) = *(uint32_t*) (u32BufferWrite + u32Count);
		}
		/* Wait for the BSY bit to be reset */
		Timeout = LVQ_FLASH_TIMEOUT;
		while ( ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY) && Timeout)
		{
				/* Wating for Bsy bit */
				if (--Timeout == 0x00) {
            return LVQ_Flash_Status_Errors;
        }
		}
		/* Clear PG bit in the FLASH CR register */
		FLASH->CR &= FLASH_CR_PG;
		
		/* Set the LOCK Bit to lock the FLASH Registers access */
		FLASH->CR |= FLASH_CR_LOCK;
		
		/* --- END WRITE DATA TO FLASH MEMORY --- */
  
		return LVQ_Flash_Status_Ok;
}

LVQ_Flash_Status_t LVQ_Flash_Write_Float(volatile uint32_t u32StartAddr, float fBufferWrite)
{
		uint8_t DaTa[4];
		*(float *) DaTa = fBufferWrite;
		if( LVQ_Flash_Write(u32StartAddr, (uint32_t*) DaTa, 1) != LVQ_Flash_Status_Ok )
				return LVQ_Flash_Status_Errors;
		return LVQ_Flash_Status_Ok;
}

LVQ_Flash_Status_t LVQ_Flash_Read_Float(volatile uint32_t u32StartAddr, float* fBufferWrite)
{
		uint32_t DaTa;
		if( LVQ_Flash_Read(u32StartAddr, &DaTa, 1) != LVQ_Flash_Status_Ok )
				return LVQ_Flash_Status_Errors;
		*fBufferWrite = *(float *) &DaTa;
		return LVQ_Flash_Status_Ok;
}

LVQ_Flash_Status_t LVQ_Flash_Write_MultiFloat(volatile uint32_t u32StartAddr, float* fBufferWrite, uint32_t u32Length)
{
		uint8_t DaTa[4];
		uint32_t DaTa32[u32Length];
		for(uint16_t i = 0; i < u32Length; i++)
		{
					*(float *) DaTa = *(fBufferWrite + i);
					DaTa32[i] = *(uint32_t *) DaTa;
		}
		if( LVQ_Flash_Write(u32StartAddr, DaTa32, u32Length) != LVQ_Flash_Status_Ok )
				return LVQ_Flash_Status_Errors;
		return LVQ_Flash_Status_Ok;
}

LVQ_Flash_Status_t LVQ_Flash_Read_MultiFloat(volatile uint32_t u32StartAddr, float* fBufferWrite, uint32_t u32Length)
{
		uint32_t DaTa[u32Length];
		if( LVQ_Flash_Read(u32StartAddr, DaTa, u32Length) != LVQ_Flash_Status_Ok )
				return LVQ_Flash_Status_Errors;
		for(uint16_t i = 0; i < u32Length; i++)
		{
				*(fBufferWrite + i) = *(float *) (DaTa + i);
		}
		return LVQ_Flash_Status_Ok;
}

LVQ_Flash_Status_t LVQ_Flash_Write_Struct(volatile uint32_t u32StartAddr, wifi_info_t wifi_data)
{
		if( LVQ_Flash_Write(u32StartAddr, (uint32_t *) &wifi_data, sizeof(wifi_info_t)/4 + (sizeof(wifi_info_t)%4 != 0)) != LVQ_Flash_Status_Ok )
				return LVQ_Flash_Status_Errors;
		return LVQ_Flash_Status_Ok;
}

LVQ_Flash_Status_t LVQ_Flash_Read_Struct(volatile uint32_t u32StartAddr, wifi_info_t* wifi_data)
{
		if( LVQ_Flash_Read(u32StartAddr, (uint32_t *) wifi_data, sizeof(wifi_info_t)/4 + (sizeof(wifi_info_t)%4 != 0)) != LVQ_Flash_Status_Ok )
				return LVQ_Flash_Status_Errors;
		return LVQ_Flash_Status_Ok;
}
