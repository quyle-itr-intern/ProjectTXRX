/*
 * mytasks.h
 */

#ifndef __MYTASKS_H
#define __MYTASKS_H

#include "lvq_stm32f4_rccConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "main.h"
#define DEBOUNCECOUNTS 10

extern xQueueHandle RxQueue, TxQueue;
void vLEDFlashTask( void *pvParameters );
void vButtonCheckTask( void *pvParameters );
void vButtonLEDsTask( void *pvParameters );
void vLCDTask( void *pvParameters );
void vUSARTTask( void *pvParameters );

#endif /* MYTASKS_H_ */
