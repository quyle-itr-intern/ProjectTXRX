#include "flash.h"

void Flash_Unclock(void)
{
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
}

void Flash_Erase(volatile uint32_t u32StartAddr)
{
	 /* Check that no Flash memory operation is ongoing by checking the BSY bit in the FLASH CR register */
	 while ( (FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);
		/* Wating for Bsy bit */
	 /* Check unlock sequences */
	 if ((FLASH->CR & FLASH_CR_LOCK) == FLASH_CR_LOCK )
	 {
		 Flash_Unclock();
	 }
	 /* Set the PER bit in the FLASH_CR register */
	 FLASH->CR |= FLASH_CR_PER;
	 /* Program the FLASH_AR register to select a page to erase */
	 FLASH->AR = (uint32_t)u32StartAddr;
	 /* Set the STRT bit in the FLASH CR register */
	 FLASH->CR |= FLASH_CR_STRT;
	 /* Wait for the BSY bit to be reset */
	 while ( (FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY)
	 {
		 /* Wating for Bsy bit */
	 }
		/* Check FLASH End of Operation flag  */
		if ((FLASH->SR & FLASH_FLAG_EOP) != RESET)
		{
			/* Clear FLASH End of Operation pending bit */
			FLASH->SR &= ~FLASH_FLAG_EOP;
		}
	 /* Clear PER bit in the FLASH_CR register */
	 FLASH->CR &= FLASH_CR_PER;
	 /* Clear STRT bit in the FLASH CR register */
	 FLASH->CR &= FLASH_CR_STRT;
	 
	 /* Set the LOCK Bit to lock the FLASH Registers access */
	 FLASH->CR |= FLASH_CR_LOCK;
}

FlashStatus Flash_Write(volatile uint32_t u32StartAddr, uint8_t* u8BufferWrite, uint32_t u32Length)
{
  	uint32_t u32Count = 0u;
   /* Check input paras */
   if ((u8BufferWrite == 0u) || (u32Length == 0u) || u32Length%2U != 0u)
   {
     return FLASH_ERRORS;
   }
   /* Check thet no Flash memory ope ration is ongoing by checking the BSY bit in the FLASH_CR register*/
   while ( (FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY)
   {
		/* Wating for Bsy bit */
   }  
   /* Check unlock sequences */
   if((FLASH->CR&FLASH_CR_LOCK) == FLASH_CR_LOCK )
	 {
			Flash_Unclock();
	 }
   /* Write FLASH CR_PG to 1 */
   FLASH->CR |= FLASH_CR_PG;
   /* Perform half-word write at the desired address */
   for (u32Count = 0; u32Count < (u32Length/2); u32Count++ )
	 {
			*(__IO uint16_t*) (u32StartAddr + u32Count*2u) = *( uint16_t* ) (u8BufferWrite + u32Count*2u);
   }
	 /* Wait for the BSY bit to be reset */
   while ( (FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY)
   {    
		 /* Wating for Bsy bit */
	 }
   /* Clear PG bit in the FLASH CR register */
   FLASH->CR &= FLASH_CR_PG;
	 
	 FLASH->CR |= FLASH_CR_LOCK;
   
	 return FLASH_NO_ERRORS;
}

FlashStatus Flash_Read(volatile uint32_t u32startAddr, uint8_t* u8BuffeRead, uint32_t u32Length)
{
	uint32_t u32Count = 0u;
	/* Check input paras */
	if ((u8BuffeRead == 0u) || (u32Length == 0u))
	{
		return FLASH_ERRORS;
	}
	/* Read data buffer */
	for (u32Count = 0u; u32Count < u32Length; u32Count++ )
	{
		*(uint8_t*) (u8BuffeRead + u32Count) = *(uint8_t*) (u32startAddr + u32Count);
	}
	return FLASH_NO_ERRORS;
}

