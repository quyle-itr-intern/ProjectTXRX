/**
  ******************************************************************************

  EXTI For STM32F446RE
  Author:   LVQ
  Updated:  17st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_exti.h"

LVQ_EXTI_Result_t LVQ_EXTI_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Line, LVQ_EXTI_Trigger_t trigger) 
{
    LVQ_GPIO_PuPd_t PuPd;
    uint8_t pinsource, portsource, irqchannel;

    /* Check if user wants to init more than one gpio pin for exti at a time */
    if (!(GPIO_Line && !(GPIO_Line & (GPIO_Line - 1)))) 
		{
        uint8_t i;
        /* Check all pins */
        for (i = 0; i < 0x10; i++) 
				{
            if (GPIO_Line & (1 << i)) 
						{
                /* Attach one pin at a time */
                if (LVQ_EXTI_Init(GPIOx, 1 << i, trigger) != LVQ_EXTI_Result_Ok) 
								{
                    /* If one failed, return error */
                    return LVQ_EXTI_Result_Error;
                }
            }
        }

        /* Return OK, all succedded */
        return LVQ_EXTI_Result_Ok;
    }

    /* Check if line is already in use */
    if (
        (EXTI->IMR & GPIO_Line) || /*!< Interrupt already attached */
        (EXTI->EMR & GPIO_Line)    /*!< Event already attached */
    ) {
        /* Return error */
        return LVQ_EXTI_Result_Error;
    }

    /* Get IRQ channel */
    switch (GPIO_Line) {
        case GPIO_PIN_0:
            irqchannel = EXTI0_IRQn;
            break;
        case GPIO_PIN_1:
            irqchannel = EXTI1_IRQn;
            break;
        case GPIO_PIN_2:
            irqchannel = EXTI2_IRQn;
            break;
        case GPIO_PIN_3:
            irqchannel = EXTI3_IRQn;
            break;
        case GPIO_Pin_4:
            irqchannel = EXTI4_IRQn;
            break;
        case GPIO_PIN_5:
        case GPIO_PIN_6:
        case GPIO_PIN_7:
        case GPIO_PIN_8:
        case GPIO_PIN_9:
            irqchannel = EXTI9_5_IRQn;
            break;
        case GPIO_PIN_10:
        case GPIO_PIN_11:
        case GPIO_PIN_12:
        case GPIO_PIN_13:
        case GPIO_PIN_14:
        case GPIO_PIN_15:
            irqchannel = EXTI15_10_IRQn;
            break;
        default:
            return LVQ_EXTI_Result_Error;
    }

    /* Check pull settings */
    if (trigger == LVQ_EXTI_Trigger_Falling) 
		{
        PuPd = LVQ_GPIO_PuPd_UP;
    } 
		else if (trigger == LVQ_EXTI_Trigger_Rising) 
		{
        PuPd = LVQ_GPIO_PuPd_DOWN;
    } 
		else 
		{
        PuPd = LVQ_GPIO_PuPd_NOPULL;
    }

    /* Init GPIO pin */
    LVQ_GPIO_Init(GPIOx, GPIO_Line, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, PuPd, LVQ_GPIO_Speed_Low);

    /* Calculate pinsource */
    pinsource = LVQ_GPIO_GetPinSource(GPIO_Line);

    /* Calculate portsource */
    portsource = LVQ_GPIO_GetPortSource(GPIOx);

    /* Enable SYSCFG clock */
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* Connect proper GPIO to SYSCFG */
    SYSCFG->EXTICR[pinsource >> 2] &= ~(0x0F << (0x04 * (pinsource & 0x03)));
    SYSCFG->EXTICR[pinsource >> 2] |= (portsource << (0x04 * (pinsource & 0x03)));

    /* Clear first */
    EXTI->IMR &= ~GPIO_Line;
    EXTI->EMR &= ~GPIO_Line;

    /* Select interrupt mode */
    EXTI->IMR |= GPIO_Line;

    /* Clear first */
    EXTI->FTSR &= ~GPIO_Line;
    EXTI->RTSR &= ~GPIO_Line;
		
    /* Select edge */
    if (trigger == LVQ_EXTI_Trigger_Falling) 
		{
        /* Write to falling edge register */
        EXTI->FTSR |= GPIO_Line;
    } 
		else if (trigger == LVQ_EXTI_Trigger_Rising) 
	  {
        /* Write to rising edge register */
        EXTI->RTSR |= GPIO_Line;
    } else {
        /* Write to rising and falling edge registers */
        EXTI->FTSR |= GPIO_Line;
        EXTI->RTSR |= GPIO_Line;
    }
		
		/* Fill NVIC settings */
		/* Compute the Corresponding IRQ Priority --------------------------------*/
		uint8_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

		/*
		*     @arg NVIC_PriorityGroup_0: 0 bits for pre-emption priority : 0x700
		*                                4 bits for subpriority
		*     @arg NVIC_PriorityGroup_1: 1 bits for pre-emption priority : 0x600
		*                                3 bits for subpriority
		*     @arg NVIC_PriorityGroup_2: 2 bits for pre-emption priority : 0x500
		*                                2 bits for subpriority 
		*     @arg NVIC_PriorityGroup_3: 3 bits for pre-emption priority : 0x400
		*                                1 bits for subpriority
		*     @arg NVIC_PriorityGroup_4: 4 bits for pre-emption priority : 0x300
		*                                0 bits for subpriority
		*/
	
		SCB->AIRCR |= 0x500;	
		
		/* Add to NVIC */
		tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
		tmppre = (0x04 - tmppriority);
		tmpsub = tmpsub >> tmppriority;

    tmppriority = EXTI_NVIC_PRIORITY << tmppre;
    tmppriority |=  (uint8_t) (pinsource & tmpsub);
		
		tmppriority = tmppriority << 0x04;

		/* Enable the Selected IRQ Channels --------------------------------------*/
		NVIC->ISER[irqchannel >> 0x05] = (uint32_t)0x01 << (irqchannel & (uint8_t)0x1F);
	 
		/* IRQ Priority */
		NVIC->IP[irqchannel] = tmppriority;

    /* Return OK */
    return LVQ_EXTI_Result_Ok;
}

LVQ_EXTI_Result_t LVQ_EXTI_Detach(uint16_t GPIO_Line) 
{
    /* Disable EXTI for specific GPIO line */
    EXTI->IMR &= ~GPIO_Line;
    EXTI->EMR &= ~GPIO_Line;

    /* Clear trigger edges */
    EXTI->FTSR &= ~GPIO_Line;
    EXTI->RTSR &= ~GPIO_Line;

    /* Return OK */
    return LVQ_EXTI_Result_Ok;
}

void LVQ_EXTI_DeInit(void) 
{
    /* CLear EXTERNAL lines only = GPIO pins */
    EXTI->IMR &= 0xFFFF0000;
    EXTI->EMR &= 0xFFFF0000;
    EXTI->FTSR &= 0xFFFF0000;
    EXTI->RTSR &= 0xFFFF0000;
    EXTI->PR &= 0xFFFF0000;
}

LVQ_EXTI_Handler_CallBack_t LVQ_EXTI_Handler_CallBack = NULL;

void LVQ_EXTI_Handler_Set_CallBack(void* cb)
{
		LVQ_EXTI_Handler_CallBack = cb;
}

#ifndef LVQ_EXTI_DISABLE_DEFAULT_HANDLER_0
void EXTI0_IRQHandler(void) 
{
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR0)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR0;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_0);
    }
}
#endif

#ifndef LVQ_EXTI_DISABLE_DEFAULT_HANDLER_1
void EXTI1_IRQHandler(void) 
{
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR1)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR1;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_1);
    }
}
#endif

#ifndef LVQ_EXTI_DISABLE_DEFAULT_HANDLER_2
void EXTI2_IRQHandler(void) 
{
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR2)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR2;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_2);
    }
}
#endif

#ifndef LVQ_EXTI_DISABLE_DEFAULT_HANDLER_3
void EXTI3_IRQHandler(void) 
{
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR3)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR3;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_3);
    }
}
#endif

#ifndef LVQ_EXTI_DISABLE_DEFAULT_HANDLER_4
void EXTI4_IRQHandler(void) 
{
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR4)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR4;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_4);
    }
}
#endif

#ifndef LVQ_EXTI_DISABLE_DEFAULT_HANDLER_9_5
void EXTI9_5_IRQHandler(void) 
{
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR5)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR5;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_5);
    }
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR6)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR6;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_6);
    }
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR7)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR7;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_7);
    }
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR8)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR8;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_8);
    }
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR9)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR9;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_9);
    }
}
#endif

#ifndef LVQ_EXTI_DISABLE_DEFAULT_HANDLER_15_10
void EXTI15_10_IRQHandler(void) 
{
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR10)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR10;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_10);
    }
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR11)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR11;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_11);
    }
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR12)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR12;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_12);
    }
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR13)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR13;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_13);	
    }
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR14)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR14;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_14);
    }
    /* Check status */
    if (EXTI->PR & (EXTI_PR_PR15)) 
		{
        /* Clear bit */
        EXTI->PR = EXTI_PR_PR15;
        /* Call global function */
        LVQ_EXTI_Handler_CallBack(GPIO_PIN_15);
    }
}
#endif
