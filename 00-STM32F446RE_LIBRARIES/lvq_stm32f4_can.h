/**
  ******************************************************************************

  CAN For SLVQ32F446RE
  Author:   LVQ
  Updated:  12st August 2022

  ******************************************************************************
*/

#ifndef __CAN_H
#define __CAN_H

/**
 * @addtogroup LVQ_STM32F4xx_Libraries
 * @{
 */
 
#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_delay.h"
#include "lvq_stm32f4_gpio.h"
#include "stm32f4xx_can.h"

/**
 * @defgroup LVQ_CAN_Macros
 * @brief    Library macros
 * @{
 */

void CANx_Init(void);

void CANxWriteData(uint16_t ID);


#endif
