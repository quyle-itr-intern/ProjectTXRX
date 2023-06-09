/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2013          /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED_SD
#define _DISKIO_DEFINED_SD

#define _USE_WRITE	1	/* 1: Enable disk_write function */
#define _USE_IOCTL	1	/* 1: Enable disk_ioctl fucntion */

#include "diskio.h"
#include "integer.h"

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"
#include "defines.h"

#include "lvq_stm32f4_spi.h"
#include "lvq_stm32f4_delay.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_fatfs.h"

#ifndef FATFS_SPI
#define FATFS_SPI							SPI1
#define FATFS_SPI_PINSPACK		LVQ_SPI_PinsPack_1
#endif

#ifndef FATFS_CS_PIN		
#define FATFS_CS_PORT						GPIOB
#define FATFS_CS_PIN						GPIO_PIN_0
#endif

#ifndef FATFS_USE_DETECT_PIN
#define FATFS_USE_DETECT_PIN				0
#endif

#ifndef FATFS_USE_WRITEPROTECT_PIN
#define FATFS_USE_WRITEPROTECT_PIN			0
#endif

#if FATFS_USE_DETECT_PIN > 0
#ifndef FATFS_USE_DETECT_PIN_PIN			
#define FATFS_USE_DETECT_PIN_PORT			GPIOB
#define FATFS_USE_DETECT_PIN_PIN			GPIO_PIN_6
#endif
#endif

#if FATFS_USE_WRITEPROTECT_PIN > 0
#ifndef FATFS_USE_WRITEPROTECT_PIN_PIN
#define FATFS_USE_WRITEPROTECT_PIN_RCC		RCC_AHB1Periph_GPIOB			
#define FATFS_USE_WRITEPROTECT_PIN_PORT		GPIOB
#define FATFS_USE_WRITEPROTECT_PIN_PIN		GPIO_Pin_7
#endif
#endif

#define FATFS_CS_LOW						LVQ_GPIO_SetPinLow(FATFS_CS_PORT, FATFS_CS_PIN)
#define FATFS_CS_HIGH						LVQ_GPIO_SetPinHigh(FATFS_CS_PORT, FATFS_CS_PIN)

#endif

