/**
  ******************************************************************************

  WS2812 For SLVQ32F446RE
  Author:   LVQ
  Updated:  9 July 2022

  ******************************************************************************
*/

#ifndef __WS2812_H
#define __WS2812_H 

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_pwm_dma.h"
#include "lvq_stm32f4_pwm.h"
#include "lvq_stm32f4_delay.h"
#include "math.h"

#define MAX_LED 180
#define USE_BRIGHTNESS 0

#define PI 3.14159265

void Light_Up(uint8_t red, uint8_t green, uint8_t blue, uint32_t speed);
void Fade(uint8_t red_begin, uint8_t green_begin,uint8_t blue_begin, uint8_t red_end, uint8_t green_end,uint8_t blue_end, uint32_t speed);
uint8_t rainbow_effect_left(void);
void Led_Single_Run(void);
void Rain(uint8_t red, uint8_t green, uint8_t blue, uint32_t speed);
void Rain_2(uint8_t red, uint8_t green, uint8_t blue, uint32_t speed);


#endif
