/**
  ******************************************************************************

  RTOS For STM32F446RE
  Author:   LVQ
  Updated:  28st August 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_rtos.h"

LVQ_RTOS_TaskInfor_t user_tasks[MAX_TASKS];

uint8_t current_task = 1; /* task1 is running */
uint32_t g_tick_count = 0;

uint8_t TaskCreate = 0;

__asm void LVQ_RTOS_ScheduleStart(uint32_t schedule_tack);

void LVQ_Unblock_Tasks(void);
void LVQ_Update_Global_Tick_Count(void);
void LVQ_Schedule_Task(void);
void SysTick_Interrupt(void);

LVQ_RTOS_Status_t LVQ_RTOS_CreateTask(LVQ_RTOS_TaskHandle_t LVQ_TaskHandler, char * LVQ_TaskName, uint32_t LVQ_TaskSize, uint32_t LVQ_TaskPriority)
{
		if(current_task == 0)
		{
			/* Enable processor faults */
			SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
			SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
			SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;	
		}
		if(TaskCreate > MAX_TASKS)
				return LVQ_RTOS_Status_Error;
		
		/* Init Process Stack Value */
		user_tasks[TaskCreate].PSP_Value = LVQ_STACK_START(TaskCreate);
		/* Init Current Task */
		user_tasks[TaskCreate].Current_State = TASK_READY_STATE;
		/* Init Task Handler */
		user_tasks[TaskCreate].Task_Handler = LVQ_TaskHandler;
		
		uint32_t *pPSP;

		pPSP = (uint32_t*) user_tasks[TaskCreate].PSP_Value;

		pPSP--;
		*pPSP = DUMMY_XPSR; /* 0x00100000 */

		pPSP--; /* PC */
		*pPSP = (uint32_t) user_tasks[TaskCreate].Task_Handler;

		pPSP--; /* LR */ 
		*pPSP = 0xFFFFFFFD;

		/* R0 -> R12 */
		for(int j = 0 ; j < 13 ; j++)
		{
			 pPSP--;
			*pPSP = 0;
		}
		user_tasks[TaskCreate].PSP_Value = (uint32_t)pPSP;
	
		TaskCreate++;
		
		return LVQ_RTOS_Status_Ok;
}

void LVQ_RTOS_Delay(uint32_t tick_count)
{
	__disable_irq();
	
	if(current_task)
	{
	   user_tasks[current_task].Block_Count = g_tick_count + tick_count;
	   user_tasks[current_task].Current_State = TASK_BLOCKED_STATE;
	   LVQ_Schedule_Task();
	}
  __enable_irq();
}

__asm void LVQ_RTOS_ScheduleStart(uint32_t schedule_tack)
{
		/* Push main stack */
		MSR MSP,R0
	
		IMPORT LVQ_Get_PSP_Value
	
		PRESERVE8 {TRUE}
		PUSH {LR}
		BL LVQ_Get_PSP_Value
		MSR PSP,R0
		POP {LR}

		MOV R0,#0x02
		MSR CONTROL,R0
		BX LR
}

uint32_t LVQ_Get_PSP_Value(void)
{
		return user_tasks[current_task].PSP_Value;
}

void LVQ_Save_PSP_Value(uint32_t Current_PSP_Value)
{
		user_tasks[current_task].PSP_Value = Current_PSP_Value;
}

void LVQ_Update_Next_Task(void)
{
		uint8_t state = TASK_BLOCKED_STATE;

		for(uint8_t i = 0; i < (MAX_TASKS); i++)
		{
			current_task++;
			current_task %= MAX_TASKS;
			state = user_tasks[current_task].Current_State;
			if( (state == TASK_READY_STATE) && (current_task != 0) )
				break;
		}

		if(state != TASK_READY_STATE)
			current_task = 0;
}

void LVQ_Schedule_Task(void)
{
	/* Pend The Pendsv Exception */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void SysTick_Interrupt(void)
{
		/* Dealy 1ms */
		SysTick->LOAD = 180U * 1000U - 1U;
		SysTick->VAL = 0U;
		SysTick->CTRL = 7U;
}
void SysTick_Handler(void)
{
		LVQ_Update_Global_Tick_Count();

		LVQ_Unblock_Tasks();

		/* Set PendSV_Handler */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}
__asm void PendSV_Handler(void)
{
		CPSID    I

		IMPORT LVQ_Save_PSP_Value
		IMPORT LVQ_Update_Next_Task
		IMPORT LVQ_Get_PSP_Value

		MRS R0,PSP         /* Save PSP to R0*/
		STMDB R0!,{R4-R11} /* Save R4-R11 in R0 and decrement R0*/
		PUSH {LR}
		BL LVQ_Save_PSP_Value
		BL LVQ_Update_Next_Task
		BL LVQ_Get_PSP_Value
		LDMIA R0!,{R4-R11}
		MSR PSP,R0
		POP {LR}

		CPSIE    I

		BX LR
}

void LVQ_Update_Global_Tick_Count(void)
{
		g_tick_count++;
}

void LVQ_Unblock_Tasks(void)
{
		for(uint8_t i = 1; i < MAX_TASKS; i++)
		{
			if(user_tasks[i].Current_State != TASK_READY_STATE)
			{
				if(user_tasks[i].Block_Count == g_tick_count)
				{
					user_tasks[i].Current_State = TASK_READY_STATE;
				}
			}
		}
}
