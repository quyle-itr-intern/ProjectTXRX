/**
  ******************************************************************************

  OTA For STM32F446RE
  Author:   LVQ
  Updated:  24January 2023
	Algorithm: 

  ******************************************************************************
*/

#ifndef __OTA_H
#define __OTA_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_defines.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_delay.h"
#include "lvq_stm32f4_usart.h"
#include "lvq_stm32f4_flash.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

#define TRUE     1
#define FALSE    0

#define OTA_RECIEVE_TIMEOUT_S   5

/* To change these values you must change it too in NodeMCU code*/
#define OTA_DATA_START_CHAR     'X'
#define OTA_LINE_BREAK_CHAR     'Y'
#define OTA_DATA_END_CHAR       'Z'
#define OTA_READ_CONFIRM_CHAR   'R'

void LVQ_OTA_Init(void);
void LVQ_OTA_Run(void);


static void LVQ_OTA_WriteFlashMemory(void);
static void LVQ_OTA_SetHighAddress(void);

void LVQ_OTA_CharReceived(uint8_t rec);



#endif
