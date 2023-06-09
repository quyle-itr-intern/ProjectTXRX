/**
  ******************************************************************************

  FLASH For STM32F446RE
  Author:   LVQ
  Updated:  23st August 2022

  ******************************************************************************
*/

#ifndef __FLASH_H
#define __FLASH_H

#include "lvq_stm32f4_rccConfig.h"

#define LVQ_FLASH_TIMEOUT 20000

/* Define Flash Sector 0 */
#ifndef LVQ_FLASH_SECTOR_0
#define LVQ_FLASH_SECTOR_0    0
#endif

/* Define Flash Sector 1 */
#ifndef LVQ_FLASH_SECTOR_1
#define LVQ_FLASH_SECTOR_1    1
#endif

/* Define Flash Sector 2 */
#ifndef LVQ_FLASH_SECTOR_2
#define LVQ_FLASH_SECTOR_2    2
#endif

/* Define Flash Sector 3 */
#ifndef LVQ_FLASH_SECTOR_3
#define LVQ_FLASH_SECTOR_3    3
#endif

/* Define Flash Sector 4 */
#ifndef LVQ_FLASH_SECTOR_4
#define LVQ_FLASH_SECTOR_4    4
#endif

/* Define Flash Sector 5 */
#ifndef LVQ_FLASH_SECTOR_5
#define LVQ_FLASH_SECTOR_5    5
#endif

/* Define Flash Sector 6 */
#ifndef LVQ_FLASH_SECTOR_6
#define LVQ_FLASH_SECTOR_6    6
#endif

/* Define Flash Sector 7 */
#ifndef LVQ_FLASH_SECTOR_7
#define LVQ_FLASH_SECTOR_7    7
#endif

/* Public enum */
typedef enum 
{
		LVQ_Flash_Status_Errors = 0,
	  LVQ_Flash_Status_Ok,
} LVQ_Flash_Status_t;

#pragma pack(1)
typedef struct {
		uint8_t no;
		uint8_t ssid[25];
		uint8_t pass[25];
		uint8_t id;
} wifi_info_t;
#pragma pack()

void LVQ_Flash_Unlock(void);

LVQ_Flash_Status_t LVQ_Flash_Erase(volatile uint32_t u32StartAddr);
LVQ_Flash_Status_t LVQ_Flash_Write(volatile uint32_t u32StartAddr, uint32_t* u32BufferWrite, uint32_t u32Length);
LVQ_Flash_Status_t LVQ_Flash_Read(volatile uint32_t u32startAddr, uint32_t* u32BuffeRead, uint32_t u32Length);
LVQ_Flash_Status_t LVQ_Flash_WriteAndErase(volatile uint32_t u32StartAddr, uint32_t* u32BufferWrite, uint32_t u32Length);

LVQ_Flash_Status_t LVQ_Flash_Write_Float(volatile uint32_t u32StartAddr, float fBufferWrite);
LVQ_Flash_Status_t LVQ_Flash_Read_Float(volatile uint32_t u32StartAddr, float* fBufferWrite);

LVQ_Flash_Status_t LVQ_Flash_Write_MultiFloat(volatile uint32_t u32StartAddr, float* fBufferWrite, uint32_t u32Length);
LVQ_Flash_Status_t LVQ_Flash_Read_MultiFloat(volatile uint32_t u32StartAddr, float* fBufferWrite, uint32_t u32Length);

LVQ_Flash_Status_t LVQ_Flash_Write_Struct(volatile uint32_t u32StartAddr, wifi_info_t wifi_data);
LVQ_Flash_Status_t LVQ_Flash_Read_Struct(volatile uint32_t u32StartAddr, wifi_info_t* wifi_data);

#endif
