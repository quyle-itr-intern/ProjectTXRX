/**
  ******************************************************************************

  W25Q32 For STM32F446RE
  Author:   LVQ
  Updated:  3st October 2022

  ******************************************************************************
*/

#ifndef __W25Q32_H
#define __W25Q32_H 

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_spi.h"
#include "lvq_stm32f4_delay.h"

/* Instruction Set Table 1 (Standard/Dual/Quad SPI Instructions) */
#define Write_Enable 									0x06
#define Volatile_SR_Write_Enable 			0x50
#define Write_Disable 								0x04
#define Read_Status_Register_1				0x05
#define Write_Status_Register_1				0x01 
#define Read_Status_Register_2 				0x35 
#define Write_Status_Register_2 			0x31
#define Read_Status_Register_3 				0x15 
#define Write_Status_Register_3 			0x11 
#define Chip_Erase										0x60
#define Erase_Program_Suspend 				0x75
#define Erase_Program_Resume 					0x7A
#define Power_down 										0xB9
#define Set_Read_Parameters 					0xC0
#define Release_Power_down_ID 				0xAB 
#define Manufacturer_Device_ID 				0x90 
#define JEDEC_ID 											0x9F 
#define Global_Block_Lock 						0x7E
#define Global_Block_Unlock 					0x98
#define Exit_QPI_Mode 								0xFF
#define Enter_QPI_Mode 								0x38
#define Enable_Reset 									0x66
#define Reset_Device 									0x99

/* Instruction Set Table 2 (Standard/Dual/Quad SPI Instructions) */
#define Read_Unique_ID 								0x4B 
#define Page_Program 									0x02 
#define Quad_Page_Program 						0x32 
#define Sector_Erase_4KB 							0x20 
#define Block_Erase_32KB 							0x52 
#define Block_Erase_64KB							0xD8
#define Read_Data 										0x03
#define Fast_Read 										0x0B 
#define Fast_Read_Dual_Output 				0x3B 
#define Fast_Read_Quad_Output 				0x6B 
#define Read_SFDP_Register 						0x5A 
#define Erase_Security_Register 			0x44 
#define Program_Security_Register 		0x42 
#define Read_Security_Register 				0x48
#define Individual_Block_Lock 				0x36 
#define Individual_Block_Unlock 			0x39
#define Read_Block_Lock 							0x3D

#define Fast_Read_Dual_IO 						0xBB
#define Mftr_Device_ID_Dual_IO 				0x92

#define Set_Burst_with_Wrap 					0x77 
#define Fast_Read_Quad_IO 						0xEB 
#define Word_Read_Quad_IO							0xE7 
#define Octal_Word_Read_Quad_IO				0xE3
#define Mftr_Device_ID_Quad_IO 				0x94

#define Dummy_Byte										0xA5

typedef struct {
		SPI_TypeDef* SPIx;
		LVQ_SPI_PinsPack_t SPI_Pinspack;
		LVQ_SPI_BaudRatePrescaler_t SPI_BaudRatePrescaler;
		GPIO_TypeDef* GPIO_Port_CS;
		uint16_t GPIO_Pin_CS;
	
		/* Info W25Q32 */
		uint8_t DeviID[2];
		uint8_t UniqID[8];
		uint8_t JEDECID[3];
		uint16_t PageSize;
		uint32_t PageCount;
		uint32_t SectorSize;
		uint32_t SectorCount;
		uint32_t BlockSize;
		uint32_t BlockCount;
}	LVQ_W25Q32_Info_t;

typedef enum {
		LVQ_W25Q32_Erase_4KB = 0,
		LVQ_W25Q32_Erase_32KB = 1,
		LVQ_W25Q32_Erase_64KB = 2
} LVQ_Erase_Type_t;

typedef enum {
		LVQ_Read_Status_Register_1 = 0,
		LVQ_Read_Status_Register_2 = 1,
		LVQ_Read_Status_Register_3 = 2
} LVQ_Read_Status_Register_t;

typedef enum {
		LVQ_Write_Status_Register_1 = 0,
		LVQ_Write_Status_Register_2 = 1,
		LVQ_Write_Status_Register_3 = 2
} LVQ_Write_Status_Register_t;

void LVQ_W25Q32_Init(LVQ_W25Q32_Info_t* w25q32);
void LVQ_W25Q32_WriteData(LVQ_W25Q32_Info_t w25q32, uint32_t address, uint8_t* DataOut, uint16_t count);
void LVQ_W25Q32_ReadData(LVQ_W25Q32_Info_t w25q32, uint32_t address, uint8_t* DataIn, uint32_t count);
void LVQ_W25Q32_EraseData(LVQ_W25Q32_Info_t w25q32, uint32_t address, LVQ_Erase_Type_t Erase_Type);
void LVQ_W25Q32_Chip_Erase(LVQ_W25Q32_Info_t w25q32);
void LVQ_W25Q32_DeviceID(LVQ_W25Q32_Info_t* w25q32);
void LVQ_W25Q32_JEDEC_ID(LVQ_W25Q32_Info_t* w25q32);
void LVQ_W25Q32_UniqueID(LVQ_W25Q32_Info_t* w25q32);

void LVQ_W25Q32_WritePage(LVQ_W25Q32_Info_t w25q32, uint32_t page_number, uint8_t offset_bytes, uint8_t* DataOut, uint16_t CountDataWriteBytes);
void LVQ_W25Q32_WriteSector(LVQ_W25Q32_Info_t w25q32, uint32_t sector_number, uint16_t offset_bytes, uint8_t* DataOut, uint16_t CountDataWriteBytes);
void LVQ_W25Q32_WriteBlock(LVQ_W25Q32_Info_t w25q32, uint32_t block_number, uint32_t offset_bytes, uint8_t* DataOut, uint32_t CountDataWriteBytes);
void LVQ_W25Q32_ReadPage(LVQ_W25Q32_Info_t w25q32, uint32_t page_number, uint8_t offset_bytes, uint8_t* DataIn, uint16_t CountDataReadBytes);
void LVQ_W25Q32_ReadSector(LVQ_W25Q32_Info_t w25q32, uint32_t sector_number, uint16_t offset_bytes, uint8_t* DataIn, uint16_t CountDataReadBytes);
void LVQ_W25Q32_ReadBlock(LVQ_W25Q32_Info_t w25q32, uint32_t block_number, uint32_t offset_bytes, uint8_t* DataIn, uint32_t CountDataReadBytes);

uint8_t LVQ_W25Q32_IsEmptyPage(LVQ_W25Q32_Info_t w25q32, uint32_t page_number, uint8_t offset_bytes, uint16_t CountDataCheckBytes );
uint8_t LVQ_W25Q32_IsEmptySector(LVQ_W25Q32_Info_t w25q32, uint32_t sector_number, uint8_t offset_bytes, uint16_t CountDataCheckBytes );
uint8_t LVQ_W25Q32_IsEmptyBlock(LVQ_W25Q32_Info_t w25q32, uint32_t block_number, uint32_t offset_bytes, uint32_t CountDataCheckBytes );

#endif
