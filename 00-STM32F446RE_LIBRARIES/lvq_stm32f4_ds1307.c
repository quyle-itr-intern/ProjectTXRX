/**
  ******************************************************************************

  I2C DS1307 For SLVQ32F446RE
  Author:   LVQ
  Updated:  17st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_ds1307.h"

uint8_t LVQ_DS1307_Months[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

LVQ_DS1307_Result_t LVQ_DS1307_Init(void) 
{
    /* Initialize clock */
    LVQ_I2C_Init(DS1307_I2C, DS1307_I2C_PINSPACK, DS1307_I2C_CLOCK);

    /* Check if device is connected */
    if (!LVQ_I2C_IsDeviceConnected(DS1307_I2C, DS1307_I2C_ADDR)) 
		{
        return LVQ_DS1307_Result_DeviceNotConnected;
    }

    /* Return OK */
    return LVQ_DS1307_Result_Ok;
}

void LVQ_DS1307_GetDateTime(LVQ_DS1307_Time_t* time) 
{
    uint8_t data[7];

    /* Read multi bytes */
    LVQ_I2C_ReadMulti(DS1307_I2C, DS1307_I2C_ADDR, DS1307_SECONDS, data, 7);

    /* Fill data */
    time->seconds = LVQ_DS1307_Bcd2Bin(data[DS1307_SECONDS]);
    time->minutes = LVQ_DS1307_Bcd2Bin(data[DS1307_MINUTES]);
    time->hours   = LVQ_DS1307_Bcd2Bin(data[DS1307_HOURS]);
    time->day     = LVQ_DS1307_Bcd2Bin(data[DS1307_DAY]);
    time->date    = LVQ_DS1307_Bcd2Bin(data[DS1307_DATE]);
    time->month   = LVQ_DS1307_Bcd2Bin(data[DS1307_MONTH]);
    time->year    = LVQ_DS1307_Bcd2Bin(data[DS1307_YEAR]);
}

void LVQ_DS1307_SetDateTime(LVQ_DS1307_Time_t* time) 
{
    uint8_t data[7];

    /* Format data */
    data[DS1307_SECONDS] = LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(time->seconds, 0, 59));
    data[DS1307_MINUTES] = LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(time->minutes, 0, 59));
    data[DS1307_HOURS]   = LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(time->hours, 0, 23));
    data[DS1307_DAY]     = LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(time->day, 1, 7));
    data[DS1307_DATE]    = LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(time->date, 1, 31));
    data[DS1307_MONTH]   = LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(time->month, 1, 12));
    data[DS1307_YEAR]    = LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(time->year, 0, 99));

    /* Write to device */
    LVQ_I2C_WriteMulti(DS1307_I2C, DS1307_I2C_ADDR, DS1307_SECONDS, data, 7);
}

uint8_t LVQ_DS1307_GetSeconds(void) 
{
    return LVQ_DS1307_Bcd2Bin(LVQ_I2C_Read(DS1307_I2C, DS1307_I2C_ADDR, DS1307_SECONDS));
}

uint8_t LVQ_DS1307_GetMinutes(void) 
{
    return LVQ_DS1307_Bcd2Bin(LVQ_I2C_Read(DS1307_I2C, DS1307_I2C_ADDR, DS1307_MINUTES));
}

uint8_t LVQ_DS1307_GetHours(void) 
{
    return LVQ_DS1307_Bcd2Bin(LVQ_I2C_Read(DS1307_I2C, DS1307_I2C_ADDR, DS1307_HOURS));
}

uint8_t LVQ_DS1307_GetDay(void) 
{
    return LVQ_DS1307_Bcd2Bin(LVQ_I2C_Read(DS1307_I2C, DS1307_I2C_ADDR, DS1307_DAY));
}

uint8_t LVQ_DS1307_GetDate(void) 
{
    return LVQ_DS1307_Bcd2Bin(LVQ_I2C_Read(DS1307_I2C, DS1307_I2C_ADDR, DS1307_DATE));
}

uint8_t LVQ_DS1307_GetMonth(void) 
{
    return LVQ_DS1307_Bcd2Bin(LVQ_I2C_Read(DS1307_I2C, DS1307_I2C_ADDR, DS1307_MONTH));
}

uint8_t LVQ_DS1307_GetYear(void) 
{
    return LVQ_DS1307_Bcd2Bin(LVQ_I2C_Read(DS1307_I2C, DS1307_I2C_ADDR, DS1307_YEAR));
}

void LVQ_DS1307_SetSeconds(uint8_t seconds) 
{
    LVQ_I2C_Write(DS1307_I2C, DS1307_I2C_ADDR, DS1307_SECONDS, LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(seconds, 0, 59)));
}

void LVQ_DS1307_SetMinutes(uint8_t minutes) 
{
    LVQ_I2C_Write(DS1307_I2C, DS1307_I2C_ADDR, DS1307_MINUTES, LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(minutes, 0, 59)));
}
void LVQ_DS1307_SetHours(uint8_t hours) 
{
    LVQ_I2C_Write(DS1307_I2C, DS1307_I2C_ADDR, DS1307_HOURS, LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(hours, 0, 23)));
}

void LVQ_DS1307_SetDay(uint8_t day) 
{
    LVQ_I2C_Write(DS1307_I2C, DS1307_I2C_ADDR, DS1307_DAY, LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(day, 1, 7)));
}

void LVQ_DS1307_SetDate(uint8_t date) 
{
    LVQ_I2C_Write(DS1307_I2C, DS1307_I2C_ADDR, DS1307_DATE, LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(date, 1, 31)));
}

void LVQ_DS1307_SetMonth(uint8_t month) 
{
    LVQ_I2C_Write(DS1307_I2C, DS1307_I2C_ADDR, DS1307_MONTH, LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(month, 1, 12)));
}

void LVQ_DS1307_SetYear(uint8_t year) 
{
    LVQ_I2C_Write(DS1307_I2C, DS1307_I2C_ADDR, DS1307_YEAR, LVQ_DS1307_Bin2Bcd(LVQ_DS1307_CheckMinMax(year, 0, 99)));
}

void LVQ_DS1307_EnableOutputPin(LVQ_DS1307_OutputFrequency_t frequency) 
{
    uint8_t temp;
    if (frequency == LVQ_DS1307_OutputFrequency_1Hz) {
        temp = 1 << DS1307_CONTROL_OUT | 1 << DS1307_CONTROL_SQWE;
    } else if (frequency == LVQ_DS1307_OutputFrequency_4096Hz) {
        temp = 1 << DS1307_CONTROL_OUT | 1 << DS1307_CONTROL_SQWE | 1 << DS1307_CONTROL_RS0;
    } else if (frequency == LVQ_DS1307_OutputFrequency_8192Hz) {
        temp =  1 << DS1307_CONTROL_OUT |  1 << DS1307_CONTROL_SQWE | 1 << DS1307_CONTROL_RS1;
    } else if (frequency == LVQ_DS1307_OutputFrequency_32768Hz) {
        temp = 1 << DS1307_CONTROL_OUT |  1 << DS1307_CONTROL_SQWE | 1 << DS1307_CONTROL_RS1 | 1 << DS1307_CONTROL_RS0;
    } else if (frequency == LVQ_DS1307_OutputFrequency_High) {
        temp = 1 << DS1307_CONTROL_OUT;
    } else if (frequency == LVQ_DS1307_OutputFrequency_Low) {
        temp = 0;
    }

    /* Write to register */
    LVQ_I2C_Write(DS1307_I2C, DS1307_I2C_ADDR, DS1307_CONTROL, temp);
}

void LVQ_DS1307_DisableOutputPin(void) 
{
    /* Set output pin to high */
    LVQ_DS1307_EnableOutputPin(LVQ_DS1307_OutputFrequency_High);
}

uint8_t LVQ_DS1307_Bcd2Bin(uint8_t bcd) 
{
    uint8_t dec = 10 * (bcd >> 4);
    dec += bcd & 0x0F;
    return dec;
}

uint8_t LVQ_DS1307_Bin2Bcd(uint8_t bin) 
{
    uint8_t low = 0;
    uint8_t high = 0;

    /* High nibble */
    high = bin / 10;
    /* Low nibble */
    low = bin - (high * 10);

    /* Return */
    return high << 4 | low;
}

uint8_t LVQ_DS1307_CheckMinMax(uint8_t val, uint8_t min, uint8_t max) 
{
    if (val < min) 
		{
        return min;
    } 
		else if (val > max) 
	  {
        return max;
    }
    return val;
}
