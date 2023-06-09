/**
  ******************************************************************************

  DELAY SYSTICK For STM32F446RE
  Author:   LVQ
  Updated:  21st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_delay.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO static uint32_t Counter_us = 0;		// Counter for function read timer
__IO static uint16_t Delay_1ms = 0;     // Counter for function handler interrupt 1ms

__IO uint32_t LVQ_Time = 0;
__IO uint32_t LVQ_Time2 = 0;

uint8_t LVQ_DELAY_Initialized = 0;

/* Private structure */
typedef struct {
    uint8_t Count;
    LVQ_DELAY_Timer_t* Timers[DELAY_MAX_CUSTOM_TIMERS];
} LVQ_DELAY_Timers_t;

/* Custom timers structure */
static LVQ_DELAY_Timers_t CustomTimers;

#if defined(LVQ_DELAY_TIM)
void LVQ_DELAY_INT_InitTIM(void);
#endif

/**
  * @brief  This function decrement timing variable
  *	@with __weak parameter to prevent errors
  * @param  None
  * @retval None
  */

#if defined(LVQ_DELAY_TIM)
void LVQ_DELAY_TIM_IRQ_HANDLER(void) 
{
    uint8_t i;
		TIM_ClearFlag(LVQ_DELAY_TIM, TIM_IT_Update);
#elif defined(KEIL_IDE)
void TimingDelay_Increasement(void) 
{
    uint8_t i;
#else
void SysTick_Handler(void) 
{
    uint8_t i;
#endif
	
	  LVQ_Time++;
    if (LVQ_Time2 != 0x00) {
        LVQ_Time2--;
    }
	
	/* Call user function */
  LVQ_DELAY_1msHandler();
	
	/* Check custom timers */
  for (i = 0; i < CustomTimers.Count; i++) 
	{
      /* Check if timer is enabled */
      if(
						CustomTimers.Timers[i] &&          /*!< Pointer exists */
            CustomTimers.Timers[i]->Enabled && /*!< Timer is enabled */
            CustomTimers.Timers[i]->CNT > 0    /*!< Counter is not NULL */
      )
			{
          /* Decrease counter */
          CustomTimers.Timers[i]->CNT--;

          /* Check if count is zero */
          if (CustomTimers.Timers[i]->CNT == 0) 
					{
              /* Call user callback function */
              CustomTimers.Timers[i]->Callback(CustomTimers.Timers[i]->UserParameters);

              /* Set new counter value */
              CustomTimers.Timers[i]->CNT = CustomTimers.Timers[i]->ARR;

              /* Disable timer if auto reload feature is not used */
              if (!CustomTimers.Timers[i]->AutoReload) 
							{
                    /* Disable counter */
                    CustomTimers.Timers[i]->Enabled = 0;
              }
          }
      }
   }
}

void LVQ_DELAY_1msHandler()
{
	
}

LVQ_DELAY_Timer_t* LVQ_DELAY_TimerCreate(uint32_t ReloadValue, uint8_t AutoReload, uint8_t StartTimer, void (*LVQ_DELAY_CustomTimerCallback)(void*), void* UserParameters) 
{
    LVQ_DELAY_Timer_t* tmp;

    /* Check if available */
    if (CustomTimers.Count >= DELAY_MAX_CUSTOM_TIMERS) 
		{
        return NULL;
    }

    /* Try to allocate memory for timer structure */
    tmp = (LVQ_DELAY_Timer_t*) malloc(sizeof(LVQ_DELAY_Timer_t));

    /* Check if allocated */
    if (tmp == NULL) {
        return NULL;
    }

    /* Fill settings */
    tmp->ARR = ReloadValue;
    tmp->CNT = tmp->ARR;
    tmp->AutoReload = AutoReload;
    tmp->Enabled = StartTimer;
    tmp->Callback = LVQ_DELAY_CustomTimerCallback;
    tmp->UserParameters = UserParameters;

    /* Increase number of timers in memory */
    CustomTimers.Timers[CustomTimers.Count++] = tmp;

    /* Return pointer to user */
    return tmp;
}

void LVQ_DELAY_TimerDelete(LVQ_DELAY_Timer_t* Timer) 
{
    uint8_t i;
    uint32_t irq;
    LVQ_DELAY_Timer_t* tmp;

    /* Get location in array of pointers */
    for (i = 0; i < CustomTimers.Count; i++) 
		{
        if (Timer == CustomTimers.Timers[i]) {
            break;
        }
    }

    /* Check for valid input */
    if (i == CustomTimers.Count) {
			 /* No timer in timers */
			 free(tmp);
       return;
    }

    /* Save pointer to timer */
    tmp = CustomTimers.Timers[i];

    /* Get interrupt status */
    irq = __get_PRIMASK();

    /* Disable interrupts */
    __disable_irq();

    /* Shift array up */
    for (; i < (CustomTimers.Count - 1); i++) {
        /* Shift data to the left */
        CustomTimers.Timers[i] = CustomTimers.Timers[i + 1];
    }

    /* Decrease count */
    CustomTimers.Count--;

    /* Enable IRQ if necessary */
    if (!irq) {
        __enable_irq();
    }

    /* Free timer */
    free(tmp);
}

LVQ_DELAY_Timer_t* LVQ_DELAY_TimerStop(LVQ_DELAY_Timer_t* Timer) 
{
    /* Disable timer */
    Timer->Enabled = 0;

    /* Return pointer */
    return Timer;
}

LVQ_DELAY_Timer_t* LVQ_DELAY_TimerStart(LVQ_DELAY_Timer_t* Timer) 
{
    /* Enable timer */
    Timer->Enabled = 1;

    /* Return pointer */
    return Timer;
}

LVQ_DELAY_Timer_t* LVQ_DELAY_TimerReset(LVQ_DELAY_Timer_t* Timer) 
{
    /* Reset timer */
    Timer->CNT = Timer->ARR;

    /* Return pointer */
    return Timer;
}

LVQ_DELAY_Timer_t* LVQ_DELAY_TimerAutoReload(LVQ_DELAY_Timer_t* Timer, uint8_t AutoReload) 
{
    /* Reset timer */
    Timer->AutoReload = AutoReload;

    /* Return pointer */
    return Timer;
}

LVQ_DELAY_Timer_t* LVQ_DELAY_TimerAutoReloadValue(LVQ_DELAY_Timer_t* Timer, uint32_t AutoReloadValue) 
{
    /* Reset timer */
    Timer->ARR = AutoReloadValue;

    /* Return pointer */
    return Timer;
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	Counter_us++;			// Tang gia tri dem len;
	Delay_1ms++;
	if(Delay_1ms == 1000)
	{
		#if defined(KEIL_IDE)
			TimingDelay_Increasement();
		#endif
		Delay_1ms = 0;
	}
}

/* Setup SysTick Timer for 1 msec interrupts.
------------------------------------------
	1. The SysTick_Config() function is a CMSIS function which configure:
	- The SysTick Reload register with value passed as function parameter.
	- Configure the SysTick IRQ priority to the lowest value (0x0F).
	- Reset the SysTick Counter register.
	- Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
	- Enable the SysTick Interrupt.
	- Start the SysTick Counter.
    
	2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
	SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
	inside the misc.c file.

	3. You can change the SysTick IRQ priority by calling the
	NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
	call. The NVIC_SetPriority() is defined inside the core_cm3.h file.

	4. To adjust the SysTick time base, use the following formula:
	
	Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
	
	- Reload Value is the parameter to be passed for SysTick_Config() function
	- Reload Value should not exceed 0xFFFFFF
*/

void Delay_Init(void)
{
	#if defined(LVQ_DELAY_TIM)
    LVQ_DELAY_INT_InitTIM();
	#else
  if (SysTick_Config(SystemCoreClock / 1000000))
  { 
    /* Capture error */ 
    while (1);
  }
	/* Configure the SysTick handler priority */
	NVIC_SetPriority(SysTick_IRQn, 0x00);
	#endif
	/* Set initialized flag */
  LVQ_DELAY_Initialized = 1;
}

void LVQ_DELAY_EnableDelayTimer(void) 
{
    /* Check if library is even initialized */
    if (!LVQ_DELAY_Initialized) {
        return;
    }
#if defined(LVQ_DELAY_TIM)
    /* Enable TIMER for delay, useful when you wakeup from sleep mode */
    LVQ_DELAY_TIM->CR1 |= TIM_CR1_CEN;
#else
    /* Enable systick interrupts, useful when you wakeup from sleep mode */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
#endif
		
}

void LVQ_DELAY_DisableDelayTimer(void) 
{
#if defined(LVQ_DELAY_TIM)
    /* Disable TIMER for delay, useful when you go to sleep mode */
    LVQ_DELAY_TIM->CR1 &= ~TIM_CR1_CEN;
#else
    /* Disable systick, useful when you go to sleep mode */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
#endif
}

/* Internal functions */
#if defined(LVQ_DELAY_TIM)
void LVQ_DELAY_INT_InitTIM(void) 
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    LVQ_TIMER_PROPERTIES_t TIM_Data;

    /* Get timer properties */
    LVQ_TIMER_GetTimerProperties(LVQ_DELAY_TIM, &TIM_Data);

    /* Generate timer properties, 1us ticks */
    LVQ_TIMER_CreateDataForPWMFrequency(&TIM_Data, 1000000);

    /* Enable clock for TIMx */
    LVQ_TIMER_EnableClock(LVQ_DELAY_TIM);

    /* Set timer settings */
    TIM_TimeBaseStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruct.TIM_Period = 999; /* 1 millisecond */
    TIM_TimeBaseStruct.TIM_Prescaler = SystemCoreClock / (1000000 * (SystemCoreClock / TIM_Data.TimerFrequency)) - 1; /* With prescaler for 1 microsecond tick */
    TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;

    /* Initialize timer */
    TIM_TimeBaseInit(LVQ_DELAY_TIM, &TIM_TimeBaseStruct);

    /* Enable interrupt each update cycle */
		TIM_ITConfig(LVQ_DELAY_TIM, TIM_IT_Update, ENABLE);

    /* Set NVIC parameters */
    NVIC_InitStruct.NVIC_IRQChannel = LVQ_DELAY_TIM_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);

    /* Start timer */
		TIM_Cmd(LVQ_DELAY_TIM, ENABLE);
}
#endif

/**
  * @brief  Initialize Delay
  * @note   Su dung chuc nang systick cua vdk lam delay
  * @param  None
  * @retval None
  */
void Delay_ms_systick(uint32_t ms)
{
	Delay_us_systick(ms*1000);
}

/**
  * @brief  Initialize Delay
  * @note   Su dung chuc nang systick cua vdk lam delay
  * @param  None
  * @retval None
  */
void Delay_us_systick(uint32_t us)
{
	#if defined(LVQ_DELAY_TIM)
    volatile uint32_t timer = LVQ_DELAY_TIM->CNT;

    do {
        /* Count timer ticks */
        while ((LVQ_DELAY_TIM->CNT - timer) == 0);

        /* Increase timer */
        timer = LVQ_DELAY_TIM->CNT;

        /* Decrease microseconds */
    } while (--us);
	#else
	/* Called from interrupt */
  while(us) 
	{
     if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) 
          us--;
  }
	#endif
}

/**
  * @brief  Initialize Delay
  * @note   Su dung chuc nang systick cua vdk lam delay
  * @param  None
  * @retval None
  */
void Timer_reset(void)
{
	Counter_us = 0;				// Dat gia tri bat dau dem len
}


/**
  * @brief  Read_Timer
  * @note   Su dung ham nay de doc ra thoi gian he thong bat dau tinh tu khi goi ham Set_Timer
  * @param  None
	* @retval thoi gian ms tu khi goi ham
  */
 uint32_t Timer_read_ms(void)
{
	return Counter_us/1000;
}

/**
  * @brief  Read_Timer
  * @note   Su dung ham nay de doc ra thoi gian he thong bat dau tinh tu khi goi ham Set_Timer
  * @param  None
	* @retval thoi gian ms tu khi goi ham Set_Timer: Thoi gian = Read_Timer() - Read_Timer(0)
  */
 uint32_t Timer_read_us(void)
{
	return Counter_us;
}

uint16_t LVQ_DELAY_Time2(void)
{ 
	return (LVQ_Time2); 
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
