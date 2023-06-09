/**
  ******************************************************************************

  I2C LCD For STM32F446RE
  Author:   LVQ
  Updated:  17st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_lcd.h"

void LVQ_LCD_ConfigI2C(void)
{
		LVQ_I2C_Init(LCD_I2C, LCD_I2C_PINSPACK, 100000);
}

void I2C_WriteLCD(unsigned char u8Address, unsigned char *pu8Data, unsigned char u8Length)
{
		unsigned char i;

		LVQ_I2C_Start(LCD_I2C, u8Address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
		for (i = 0; i < u8Length; ++i) 
		{
			LVQ_I2C_WriteData(LCD_I2C, pu8Data[i]);
		}
		LVQ_I2C_Stop(LCD_I2C);
}

unsigned char u8LCD_Buff[8];
unsigned char u8LcdTmp;
unsigned char displaycontrol, displaymode;

void I2C_LCD_FlushVal(void)
{
		unsigned char i;
		
		for (i = 0; i < 8; ++i) 
		{
			u8LcdTmp >>= 1;
			if(u8LCD_Buff[i]) 
			{
				u8LcdTmp |= 0x80;
			}
		}
		I2C_WriteLCD(I2C_LCD_ADDR, &u8LcdTmp, 1);
}

void I2C_LCD_WriteData(unsigned char u8Data)
{
		if(u8Data & 0x80) 
		{
			u8LCD_Buff[LCD_D7] = 1;
		} else {
			u8LCD_Buff[LCD_D7] = 0;
		}
		if(u8Data & 0x40) 
		{
			u8LCD_Buff[LCD_D6] = 1;
		} else {
			u8LCD_Buff[LCD_D6] = 0;
		}
		if(u8Data & 0x20) 
		{
			u8LCD_Buff[LCD_D5] = 1;
		} else {
			u8LCD_Buff[LCD_D5] = 0;
		}
		if(u8Data & 0x10) 
		{
			u8LCD_Buff[LCD_D4] = 1;
		} else {
			u8LCD_Buff[LCD_D4] = 0;
		}
			
		u8LCD_Buff[LCD_EN] = 1;
		I2C_LCD_FlushVal();
			
		u8LCD_Buff[LCD_EN] = 0;
		I2C_LCD_FlushVal();
		Delay_ms_systick(1);
}

void I2C_LCD_WriteCmd(unsigned char u8Cmd)
{
		u8LCD_Buff[LCD_RS] = 0;
		I2C_LCD_FlushVal();
		
		u8LCD_Buff[LCD_RW] = 0;
		I2C_LCD_FlushVal();

		I2C_LCD_WriteData(u8Cmd);
		I2C_LCD_WriteData(u8Cmd << 4);
}

void LVQ_I2C_LCD_Init(void)
{
    LVQ_LCD_ConfigI2C();
    Delay_ms_systick(5);
    unsigned char i;
    
    for (i = 0; i < 8; ++i) 
		{
			u8LCD_Buff[i] = 0;
	  }
    I2C_LCD_FlushVal();
    
    u8LCD_Buff[LCD_RS] = 0;
		I2C_LCD_FlushVal();
	
		u8LCD_Buff[LCD_RW] = 0;
		I2C_LCD_FlushVal();
    
    I2C_LCD_WriteData(0x30);
    Delay_ms_systick(5);
    I2C_LCD_WriteData(0x30);
    Delay_ms_systick(1);
    I2C_LCD_WriteData(0x30);
    Delay_ms_systick(1);
    
    I2C_LCD_WriteData(0x28);
    Delay_ms_systick(1);
    
    I2C_LCD_WriteCmd(0x28);
    
    I2C_LCD_WriteCmd(0x01);
    
    I2C_LCD_WriteCmd(0x0E);
		displaycontrol = 0x0E;
}

void LVQ_I2C_LCD_Write_Char(char c)
{
		u8LCD_Buff[LCD_RS] = 1;
		I2C_LCD_FlushVal();
		u8LCD_Buff[LCD_RW] = 0;
		I2C_LCD_FlushVal();
			
		I2C_LCD_WriteData(c);
		I2C_LCD_WriteData(c << 4);
}

void LVQ_I2C_LCD_Write_String(char *str)
{
		while(*str)
		{
			 LVQ_I2C_LCD_Write_Char(*str++);
		}
}

void LVQ_I2C_LCD_GotoCursor(unsigned char u8row, unsigned char u8col)
{
    unsigned char cmd;
		cmd = (u8row==1?0x80:0xC0) + u8col - 1;
    I2C_LCD_WriteCmd(cmd);
}

void LVQ_I2C_LCD_CursorChar(unsigned char u8row, unsigned char u8col, char c)
{
    unsigned char cmd;
		cmd = (u8row==1?0x80:0xC0) + u8col - 1;
    I2C_LCD_WriteCmd(cmd);
    
    LVQ_I2C_LCD_Write_Char(c);
}

void LVQ_I2C_LCD_CursorString(unsigned char u8row, unsigned char u8col, char *str)
{
    unsigned char cmd;
		cmd = (u8row==1?0x80:0xC0) + u8col - 1;
    I2C_LCD_WriteCmd(cmd);
    
    LVQ_I2C_LCD_Write_String(str);
}

void LVQ_I2C_LCD_BackLight(unsigned char u8BackLight)
{
	
		if(u8BackLight) 
		{
			u8LCD_Buff[LCD_BL] = 1;
		} 
		else 
		{
			u8LCD_Buff[LCD_BL] = 0;
		}
		I2C_LCD_FlushVal();
}

void LVQ_LCD_Display() 
{
		displaycontrol |= LCD_DISPLAYON;
		I2C_LCD_WriteCmd(LCD_DISPLAYCONTROL | displaycontrol);
}

void LVQ_LCD_noDisplay(void) 
{
		displaycontrol &= ~LCD_DISPLAYON;
		I2C_LCD_WriteCmd(LCD_DISPLAYCONTROL | displaycontrol);
}

void LVQ_LCD_noCursor(void) 
{
		displaycontrol &= ~LCD_CURSORON;
		I2C_LCD_WriteCmd(LCD_DISPLAYCONTROL | displaycontrol);
}

void LVQ_LCD_cursor(void) 
{
		displaycontrol |= LCD_CURSORON;
		I2C_LCD_WriteCmd(LCD_DISPLAYCONTROL | displaycontrol);
}

void LVQ_LCD_noBlink(void) 
{
		displaycontrol &= ~LCD_BLINKON;
		I2C_LCD_WriteCmd(LCD_DISPLAYCONTROL | displaycontrol);
}
void LVQ_LCD_blink(void)
{
		displaycontrol |= LCD_BLINKON;
		I2C_LCD_WriteCmd(LCD_DISPLAYCONTROL | displaycontrol);
}

void LVQ_LCD_scrollDisplayLeft(void) 
{
		I2C_LCD_WriteCmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LVQ_LCD_scrollDisplayRight(void) 
{
		I2C_LCD_WriteCmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LVQ_LCD_leftToRight(void) 
{
		displaymode |= LCD_ENTRYLEFT;
		I2C_LCD_WriteCmd(LCD_ENTRYMODESET | displaymode);
}

void LVQ_LCD_rightToLeft(void) 
{
		displaymode &= ~LCD_ENTRYLEFT;
		I2C_LCD_WriteCmd(LCD_ENTRYMODESET | displaymode);
}

void LVQ_LCD_autoscroll(void) 
{
		displaymode |= LCD_ENTRYSHIFTINCREMENT;
		I2C_LCD_WriteCmd(LCD_ENTRYMODESET | displaymode);
}

void LVQ_LCD_noAutoscroll(void) 
{
		displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
		I2C_LCD_WriteCmd(LCD_ENTRYMODESET | displaymode);
}
