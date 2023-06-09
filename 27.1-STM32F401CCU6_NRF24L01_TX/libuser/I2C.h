/**
  ******************************************************************************

  I2C Setup For STM32F446RE
  Author:   ControllersTech
  Updated:  31st Jan 2020

  ******************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ******************************************************************************
*/

#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>

void I2C_Config(void);

void I2C_Start(void);

void I2C_Write(uint8_t data);

void I2C_Address(uint8_t Address);

void I2C_Stop(void);

void I2C_WriteMulti(uint8_t *data, uint8_t size);

void I2C_Read(uint8_t Address, uint8_t *buffer, uint8_t size);

#define I2C_LCD_ADDR 0x4E
#define LCD_EN 2
#define LCD_RW 1
#define LCD_RS 0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BL 3

void I2C_LCD_Init(void);
void I2C_LCD_Write_Char(char c);
void I2C_LCD_Write_String(char *str);
void I2C_LCD_GotoCursor(unsigned char u8row, unsigned char u8col);
void I2C_LCD_CursorChar(unsigned char u8row, unsigned char u8col, char c);
void I2C_LCD_CursorString(unsigned char u8row, unsigned char u8col, char *str);
void I2C_LCD_BackLight(unsigned char u8BackLight);

#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_ENTRYMODESET 0x04

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

void LCD_Display(void);
void LCD_noDisplay(void);
void LCD_noCursor(void);
void LCD_cursor(void);
void LCD_noBlink(void);
void LCD_blink(void);
void LCD_scrollDisplayLeft(void);
void LCD_scrollDisplayRight(void);
void LCD_leftToRight(void);
void LCD_rightToLeft(void);
void LCD_autoscroll(void);
void LCD_noAutoscroll(void);

#endif
