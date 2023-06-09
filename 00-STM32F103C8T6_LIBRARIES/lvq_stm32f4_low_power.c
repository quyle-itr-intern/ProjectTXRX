/**
  ******************************************************************************

  LOW POWER For SLVQ32F446RE
  Author:   LVQ
  Updated:  13st August 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_low_power.h"

void LVQ_LOWPOWER_SleepUntilInterrupt(uint8_t delay_timer) 
{
    /* Disable systick */
    if (delay_timer) 
		{
        LVQ_DELAY_DisableDelayTimer();
    }

    /* Wait for interrupt command */
    __WFI();

    /* At this stage, MCU will be in sleep mode, and when interrupt wake it up, */
    /* it will begin here */

    /* Enable delay timer back */
    if (delay_timer) 
		{
        LVQ_DELAY_EnableDelayTimer();
    }
}

void LVQ_LOWPOWER_SleepUntilEvent(void) 
{
    /* We don't need delay timer disable, because delay timer does not make an event */

    /* Wait for event */
    __WFE();
}

void LVQ_LOWPOWER_Standby(void) 
{
    /* Clear Standby and wakeup flag */
    PWR_ClearFlag(PWR_FLAG_SB | PWR_FLAG_WU);

    /* Go to standby mode */
    PWR_EnterSTANDBYMode();
}

uint8_t LVQ_LOWPOWER_StandbyReset(void) 
{
    /* Check Standby Flag */
    if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET) 
		{
        /* Clear Standby and wakeup flag */
        PWR_ClearFlag(PWR_FLAG_SB | PWR_FLAG_WU);

        /* Reset was from wakeup from standy */
        return 1;
    }

    /* Reset was not from standby */
    return 0;
}

void LVQ_LOWPOWER_StopUntilInterrupt(void) 
{
    /* Go to STOP mode */
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

    /* After wakeup, call system init to enable PLL as clock source */
    SystemInit();
}

void LVQ_LOWPOWER_StopUntilEvent(void) 
{
    /* Go to STOP mode */
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);

    /* After wakeup, call system init to enable PLL as clock source */
    SystemInit();
}

void LVQ_LOWPOWER_EnableWakeUpPin(void) 
{
    /* Enable Wakeup pin, PA0 */
#if defined(STM32F446xx)
    PWR_WakeUpPinCmd(PWR_WakeUp_Pin1, ENABLE);
#else
    PWR_WakeUpPinCmd(ENABLE);
#endif
}

void LVQ_LOWPOWER_DisableWakeUpPin(void) 
{
    /* Disable Wakeup pin, PA0 */
#if defined(STM32F446xx)
    PWR_WakeUpPinCmd(PWR_WakeUp_Pin1, DISABLE);
#else
    PWR_WakeUpPinCmd(DISABLE);
#endif
}
