#ifndef __UITX_H
#define __UITX_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_delay.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_ssd1306.h"
#include "lvq_stm32f4_nrf24.h"

void LVQ_UITX_Volume(void);
void LVQ_UITX_Loading(void);
void LVQ_UITX_Display(void);
void LVQ_UITX_Reverse(void);
void LVQ_UITX_EndPoint(void);
void LVQ_UITX_Battery(void);

#endif
