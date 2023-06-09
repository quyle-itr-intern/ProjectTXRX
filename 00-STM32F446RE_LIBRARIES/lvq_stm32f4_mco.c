/**
  ******************************************************************************

  MCO For STM32F446RE
  Author:   LVQ
  Updated:  16st March 2023

  ******************************************************************************
*/

#include "lvq_stm32f4_mco.h"

void LVQ_MCO_InitMCO1(void) {
    /* Enable PA8 */
    LVQ_GPIO_Init(GPIOA, GPIO_PIN_8, LVQ_GPIO_Mode_AF, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High);
}

void LVQ_MCO_InitMCO2(void) {
    /* Enable PC9 */
    LVQ_GPIO_Init(GPIOC, GPIO_PIN_9, LVQ_GPIO_Mode_AF, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High);
}

void LVQ_MCO_SetOutput1(LVQ_MCO1_Source_t Source, LVQ_MCO_Prescaler_t Prescaler) 
{
    uint32_t presc = 0, src = 0;

    /* Get prescaler */
    switch (Prescaler) {
        case LVQ_MCO_Prescaler_1:
            presc = RCC_MCO1Div_1;
            break;
        case LVQ_MCO_Prescaler_2:
            presc = RCC_MCO1Div_2;
            break;
        case LVQ_MCO_Prescaler_3:
            presc = RCC_MCO1Div_3;
            break;
        case LVQ_MCO_Prescaler_4:
            presc = RCC_MCO1Div_4;
            break;
        case LVQ_MCO_Prescaler_5:
            presc = RCC_MCO1Div_5;
            break;
        default:
						break;
    }

    /* Get source */
    switch (Source) {
        case LVQ_MCO1_Source_HSE:
            src = RCC_MCO1Source_HSE;

            /* Enable RCC HSE oscillator */
            RCC_HSEConfig(RCC_HSE_ON);
            break;
        case LVQ_MCO1_Source_HSI:
            src = RCC_MCO1Source_HSI;
            break;
        case LVQ_MCO1_Source_PLLCLK:
            src = RCC_MCO1Source_PLLCLK;
            break;
        case LVQ_MCO1_Source_LSE:
            src = RCC_MCO1Source_LSE;

            /* Start LSE clock if available */
            RCC_LSEConfig(RCC_LSE_ON);
            break;
        default:
            break;
		}
    /* Set source and prescaler  */
    RCC_MCO1Config(src, presc);
}

void LVQ_MCO_SetOutput2(LVQ_MCO2_Source_t Source, LVQ_MCO_Prescaler_t Prescaler) 
{
    uint32_t presc, src;

    /* Get prescaler */
    switch (Prescaler) {
        case LVQ_MCO_Prescaler_1:
            presc = RCC_MCO2Div_1;
            break;
        case LVQ_MCO_Prescaler_2:
            presc = RCC_MCO2Div_2;
            break;
        case LVQ_MCO_Prescaler_3:
            presc = RCC_MCO2Div_3;
            break;
        case LVQ_MCO_Prescaler_4:
            presc = RCC_MCO2Div_4;
            break;
        case LVQ_MCO_Prescaler_5:
            presc = RCC_MCO2Div_5;
            break;
        default:
            break;
    }

    /* Get clock source */
    switch (Source) {
        case LVQ_MCO2_Source_SYSCLK:
            src = RCC_MCO2Source_SYSCLK;
            break;
        case LVQ_MCO2_Source_PLLI2SCLK:
            src = RCC_MCO2Source_PLLI2SCLK;
            break;
        case LVQ_MCO2_Source_HSE:
            src = RCC_MCO2Source_HSE;
            break;
        case LVQ_MCO2_Source_PLLCLK:
            src = RCC_MCO2Source_PLLCLK;
            break;
        default:
            break;
    }

    /* Set source and prescaler  */
    RCC_MCO2Config(src, presc);
}
