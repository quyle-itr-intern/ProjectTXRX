#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm32f10x.h"

typedef enum {
	FLASH_ERRORS = 0,
	FLASH_NO_ERRORS,
} FlashStatus;

void Flash_Erase (volatile uint32_t u32StartAddr);
void Flash_Unclock(void);
FlashStatus Flash_Write(volatile uint32_t u32StartAddr, uint8_t* u8BufferWrite, uint32_t u32Length);
FlashStatus Flash_Read(volatile uint32_t u32startAddr, uint8_t* u8BuffeRead, uint32_t u32Length);


#endif

