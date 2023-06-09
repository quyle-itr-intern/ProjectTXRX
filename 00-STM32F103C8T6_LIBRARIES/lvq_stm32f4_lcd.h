/**
  ******************************************************************************

  I2C LCD For STM32F446RE
  Author:   LVQ
  Updated:  17st June 2022

  ******************************************************************************
*/

#ifndef __I2C_LCD_H
#define __I2C_LCD_H

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_i2c.h"
#include "lvq_stm32f4_delay.h"
#include <stdint.h>

/* I2C settings for LCD library */
#ifndef LCD_I2C
#define LCD_I2C                  I2C1
#define LCD_I2C_PINSPACK         LVQ_I2C_PinsPack_2
#endif

#define I2C_LCD_ADDR 0x4E

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2
#define LCD_BL 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

void LVQ_LCD_ConfigI2C(void);
void LVQ_I2C_LCD_Init(void);
void LVQ_I2C_LCD_Write_Char(char c);
void LVQ_I2C_LCD_Write_String(char *str);
void LVQ_I2C_LCD_GotoCursor(unsigned char u8row, unsigned char u8col);
void LVQ_I2C_LCD_CursorChar(unsigned char u8row, unsigned char u8col, char c);
void LVQ_I2C_LCD_CursorString(unsigned char u8row, unsigned char u8col, char *str);
void LVQ_I2C_LCD_BackLight(unsigned char u8BackLight);

#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_ENTRYMODESET 0x04

/* flags for display/cursor shift */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

/* flags for display entry mode */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/* flags for display on/off control */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

void LVQ_LCD_Display(void);
void LVQ_LCD_noDisplay(void);
void LVQ_LCD_noCursor(void);
void LVQ_LCD_cursor(void);
void LVQ_LCD_noBlink(void);
void LVQ_LCD_blink(void);
void LVQ_LCD_scrollDisplayLeft(void);
void LVQ_LCD_scrollDisplayRight(void);
void LVQ_LCD_leftToRight(void);
void LVQ_LCD_rightToLeft(void);
void LVQ_LCD_autoscroll(void);
void LVQ_LCD_noAutoscroll(void);

#endif
