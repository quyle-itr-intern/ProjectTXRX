/**
  ******************************************************************************

  CRC For SLVQ32F446RE
  Author:   LVQ
  Updated:  26st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_crc.h"

void LVQ_CRC_Init(void) 
{
    /* Enable CRC clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
}

void LVQ_CRC_DeInit(void) 
{
    /* Disable CRC clock */
    RCC->AHB1ENR &= ~RCC_AHB1ENR_CRCEN;
}

uint32_t LVQ_CRC_Calculate8(uint8_t* arr, uint32_t count, uint8_t reset) 
{
    /* Reset CRC data register if necessary */
    if (reset) 
		{
        /* Reset generator */
        CRC->CR = CRC_CR_RESET;
    }
    /* Calculate CRC */
    while (count--) 
		{
        /* Set new value */
        CRC->DR = *arr++;
    }
    /* Return data */
    return CRC->DR;
}

uint32_t LVQ_CRC_Calculate16(uint16_t* arr, uint32_t count, uint8_t reset) 
{
    /* Reset CRC data register if necessary */
    if (reset) 
		{
        /* Reset generator */
        CRC->CR = CRC_CR_RESET;
    }
    /* Calculate CRC */
    while (count--) 
		{
        /* Set new value */
        CRC->DR = *arr++;
    }
    /* Return data */
    return CRC->DR;
}

uint32_t LVQ_CRC_Calculate32(uint32_t* arr, uint32_t count, uint8_t reset) 
{
    /* Reset CRC data register if necessary */
    if (reset) 
		{
        /* Reset generator */
        CRC->CR = CRC_CR_RESET;
    }
    /* Calculate CRC */
    while (count--) 
		{
        /* Set new value */
        CRC->DR = *arr++;
    }
    /* Return data */
    return CRC->DR;
}
