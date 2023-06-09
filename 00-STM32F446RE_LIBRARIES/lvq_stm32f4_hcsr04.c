/**
  ******************************************************************************

  HCSR04 For SLVQ32F446RE
  Author:   LVQ
  Updated:  11st August 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_hcsr04.h"

uint8_t LVQ_HCSR04_Init(LVQ_HCSR04_t* HCSR04, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_GPIO_Pin, GPIO_TypeDef* TRIGGER_GPIOx, uint16_t TRIGGER_GPIO_Pin) 
{
    /* Init Delay functions */
    Delay_Init();

    /* Save everything */
    HCSR04->ECHO_GPIOx = ECHO_GPIOx;
    HCSR04->ECHO_GPIO_Pin = ECHO_GPIO_Pin;
    HCSR04->TRIGGER_GPIOx = TRIGGER_GPIOx;
    HCSR04->TRIGGER_GPIO_Pin = TRIGGER_GPIO_Pin;

    /* Initialize pins */
    /* Trigger pin */
    LVQ_GPIO_Init(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_DOWN, LVQ_GPIO_Speed_Medium);

    /* Echo pin */
    LVQ_GPIO_Init(HCSR04->ECHO_GPIOx, HCSR04->ECHO_GPIO_Pin, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_DOWN, LVQ_GPIO_Speed_Medium);

    /* Trigger set to low */
    LVQ_GPIO_SetPinLow(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin);

    /* Start measurement, check if sensor is working */
    if (LVQ_HCSR04_Read(HCSR04) >= 0) 
		{
        /* Sensor OK */
        return 1;
    }

    /* Sensor error */
    return 0;
}

float LVQ_HCSR04_Read(LVQ_HCSR04_t* HCSR04) 
{
    uint32_t time, timeout;
    /* Trigger low */
    LVQ_GPIO_SetPinLow(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin);
    /* Delay 2 us */
    Delay_us_systick(2);
    /* Trigger high for 10us */
    LVQ_GPIO_SetPinHigh(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin);
    /* Delay 10 us */
    Delay_us_systick(10);
    /* Trigger low */
    LVQ_GPIO_SetPinLow(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin);

    /* Give some time for response */
    timeout = HCSR04_TIMEOUT;
    while (!LVQ_GPIO_GetInputPinValue(HCSR04->ECHO_GPIOx, HCSR04->ECHO_GPIO_Pin)) 
		{
        if (timeout-- == 0x00) 
				{
            return -1;
        }
    }

    /* Start time */
    time = 0;
    /* Wait till signal is low */
    while (LVQ_GPIO_GetInputPinValue(HCSR04->ECHO_GPIOx, HCSR04->ECHO_GPIO_Pin)) {
        /* Increase time */
        time++;
        /* Delay 1us */
        Delay_us_systick(1);
    }

    /* Convert us to cm */
    HCSR04->Distance =  (float)time * HCSR04_NUMBER;

    /* Return distance */
    return HCSR04->Distance;
}
