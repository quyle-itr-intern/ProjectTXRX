/**
  ******************************************************************************

  RTOS For STM32F446RE
  Author:   LVQ
  Updated:  28st August 2022

  ******************************************************************************
*/

#ifndef __RTOS_H
#define __RTOS_H

/**
 * @defgroup LVQ_RTOS_Typedefs
 * @brief    Library Typedefs
 * @{
 */
 
#include "lvq_stm32f4_rccConfig.h"

#define DUMMY_XPSR  						 0x01000000U
#define MAX_TASKS                3U
#define TASK_READY_STATE         0x00U
#define TASK_BLOCKED_STATE       0xFFU
#define SRAM_START               0x20000000U
#define SIZE_SRAM                ((128U) * (1024))             /* 128 Kbytes of SRAM */
#define SRAM_END                 ((SRAM_START) + (SIZE_SRAM) )

#define LVQ_STACK_START(x)       ( (SRAM_END) - (x * SIZE_TASK_STACK) ) 
#define SCHEDULED_START        ( (SRAM_END) - (MAX_TASKS * SIZE_TASK_STACK ) )

/* Some stack memory calculations */
#define SIZE_TASK_STACK          1024U
#define SIZE_SCHED_STACK         1024U

typedef struct
{
	uint32_t PSP_Value;
	uint32_t Block_Count;
	uint8_t  Current_State;
	void (*Task_Handler) (void);
} LVQ_RTOS_TaskInfor_t;

typedef void * LVQ_RTOS_TaskHandle_t;

typedef enum
{	
		LVQ_RTOS_Status_Error,
		LVQ_RTOS_Status_Ok,
}	LVQ_RTOS_Status_t;

LVQ_RTOS_Status_t LVQ_RTOS_CreateTask(LVQ_RTOS_TaskHandle_t LVQ_TaskHandler, char * LVQ_TaskName, uint32_t LVQ_TaskSize, uint32_t LVQ_TaskPriority);

void LVQ_RTOS_Delay(uint32_t tick_count);
void SysTick_Interrupt(void);

void LVQ_RTOS_ScheduleStart(uint32_t schedule_tack);


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif
