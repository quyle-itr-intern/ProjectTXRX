/**
  ******************************************************************************

  KEYPAD For SLVQ32F446RE
  Author:   LVQ
  Updated:  20st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_keypad.h"

/* Pins configuration, columns are outputs */
#define KEYPAD_COLUMN_1_HIGH        LVQ_GPIO_SetPinHigh(KEYPAD_COLUMN_1_PORT, KEYPAD_COLUMN_1_PIN)
#define KEYPAD_COLUMN_1_LOW         LVQ_GPIO_SetPinLow(KEYPAD_COLUMN_1_PORT, KEYPAD_COLUMN_1_PIN)
#define KEYPAD_COLUMN_2_HIGH        LVQ_GPIO_SetPinHigh(KEYPAD_COLUMN_2_PORT, KEYPAD_COLUMN_2_PIN)
#define KEYPAD_COLUMN_2_LOW         LVQ_GPIO_SetPinLow(KEYPAD_COLUMN_2_PORT, KEYPAD_COLUMN_2_PIN)
#define KEYPAD_COLUMN_3_HIGH        LVQ_GPIO_SetPinHigh(KEYPAD_COLUMN_3_PORT, KEYPAD_COLUMN_3_PIN)
#define KEYPAD_COLUMN_3_LOW         LVQ_GPIO_SetPinLow(KEYPAD_COLUMN_3_PORT, KEYPAD_COLUMN_3_PIN)
#define KEYPAD_COLUMN_4_HIGH        LVQ_GPIO_SetPinHigh(KEYPAD_COLUMN_4_PORT, KEYPAD_COLUMN_4_PIN)
#define KEYPAD_COLUMN_4_LOW         LVQ_GPIO_SetPinLow(KEYPAD_COLUMN_4_PORT, KEYPAD_COLUMN_4_PIN)

/* Read input pins */
#define KEYPAD_ROW_1_CHECK          (!LVQ_GPIO_GetInputPinValue(KEYPAD_ROW_1_PORT, KEYPAD_ROW_1_PIN))
#define KEYPAD_ROW_2_CHECK          (!LVQ_GPIO_GetInputPinValue(KEYPAD_ROW_2_PORT, KEYPAD_ROW_2_PIN))
#define KEYPAD_ROW_3_CHECK          (!LVQ_GPIO_GetInputPinValue(KEYPAD_ROW_3_PORT, KEYPAD_ROW_3_PIN))
#define KEYPAD_ROW_4_CHECK          (!LVQ_GPIO_GetInputPinValue(KEYPAD_ROW_4_PORT, KEYPAD_ROW_4_PIN))

uint8_t KEYPAD_INT_Buttons[4][4] = {
    {0x01, 0x02, 0x03, 0x0C},
    {0x04, 0x05, 0x06, 0x0D},
    {0x07, 0x08, 0x09, 0x0E},
    {0x0A, 0x00, 0x0B, 0x0F},
};

/* Private functions */
void LVQ_KEYPAD_INT_SetColumn(uint8_t column);
uint8_t LVQ_KEYPAD_INT_CheckRow(uint8_t column);
uint8_t LVQ_KEYPAD_INT_Read(void);

/* Private variables */
LVQ_KEYPAD_Type_t LVQ_KEYPAD_INT_KeypadType;
static LVQ_KEYPAD_Button_t KeypadStatus = LVQ_KEYPAD_Button_NOPRESSED;

LVQ_KEYPAD_Result_t LVQ_KEYPAD_Init(LVQ_KEYPAD_Type_t type) 
{
		if((type != LVQ_KEYPAD_Type_Large) && (type != LVQ_KEYPAD_Type_Small)) 
			return LVQ_KEYPAD_Result_Error;
		
    /* Set keyboard type */
    LVQ_KEYPAD_INT_KeypadType = type;

    /* Columns are output */
    /* Column 1 */
    LVQ_GPIO_Init(KEYPAD_COLUMN_1_PORT, KEYPAD_COLUMN_1_PIN, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium);
    /* Column 2 */
    LVQ_GPIO_Init(KEYPAD_COLUMN_2_PORT, KEYPAD_COLUMN_2_PIN, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium);
    /* Column 3 */
    LVQ_GPIO_Init(KEYPAD_COLUMN_3_PORT, KEYPAD_COLUMN_3_PIN, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium);
    /* Column 4 */
    if (LVQ_KEYPAD_INT_KeypadType == LVQ_KEYPAD_Type_Large) 
		{
        LVQ_GPIO_Init(KEYPAD_COLUMN_4_PORT, KEYPAD_COLUMN_4_PIN, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium);
    }

    /* Rows are inputs */
    /* Row 1 */
    LVQ_GPIO_Init(KEYPAD_ROW_1_PORT, KEYPAD_ROW_1_PIN, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium);
    /* Row 2 */
    LVQ_GPIO_Init(KEYPAD_ROW_2_PORT, KEYPAD_ROW_2_PIN, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium);
    /* Row 3 */
    LVQ_GPIO_Init(KEYPAD_ROW_3_PORT, KEYPAD_ROW_3_PIN, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium);
    /* Row 4 */
    LVQ_GPIO_Init(KEYPAD_ROW_4_PORT, KEYPAD_ROW_4_PIN, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium);

    /* All columns high */
    LVQ_KEYPAD_INT_SetColumn(0);
		
		return LVQ_KEYPAD_Result_Ok;
}

LVQ_KEYPAD_Button_t LVQ_KEYPAD_Read(void)
{
    LVQ_KEYPAD_Button_t temp;

    /* Get keypad status */
    temp = KeypadStatus;

    /* Reset keypad status */
    KeypadStatus = LVQ_KEYPAD_Button_NOPRESSED;

    return temp;
}

/* Private */
void LVQ_KEYPAD_INT_SetColumn(uint8_t column) 
{
    /* Set rows high */
    KEYPAD_COLUMN_1_HIGH;
    KEYPAD_COLUMN_2_HIGH;
    KEYPAD_COLUMN_3_HIGH;
    if (LVQ_KEYPAD_INT_KeypadType == LVQ_KEYPAD_Type_Large) 
		{
        KEYPAD_COLUMN_4_HIGH;
    }

    /* Set column low */
    if (column == 1) {
        KEYPAD_COLUMN_1_LOW;
    }
    if (column == 2) {
        KEYPAD_COLUMN_2_LOW;
    }
    if (column == 3) {
        KEYPAD_COLUMN_3_LOW;
    }
    if (column == 4) {
        KEYPAD_COLUMN_4_LOW;
    }
}

uint8_t LVQ_KEYPAD_INT_CheckRow(uint8_t column) 
{
    /* Read rows */

    /* Scan row 1 */
    if (KEYPAD_ROW_1_CHECK) {
        return KEYPAD_INT_Buttons[0][column - 1];
    }
    /* Scan row 2 */
    if (KEYPAD_ROW_2_CHECK) {
        return KEYPAD_INT_Buttons[1][column - 1];
    }
    /* Scan row 3 */
    if (KEYPAD_ROW_3_CHECK) {
        return KEYPAD_INT_Buttons[2][column - 1];
    }
    /* Scan row 4 */
    if (LVQ_KEYPAD_INT_KeypadType == LVQ_KEYPAD_Type_Large && KEYPAD_ROW_4_CHECK) 
		{
        return KEYPAD_INT_Buttons[3][column - 1];
    }

    /* Not pressed */
    return KEYPAD_NO_PRESSED;
}

uint8_t LVQ_KEYPAD_INT_Read(void) 
{
    uint8_t check;
    /* Set row 1 to LOW */
    LVQ_KEYPAD_INT_SetColumn(1);
    /* Check rows */
    check = LVQ_KEYPAD_INT_CheckRow(1);
    if (check != KEYPAD_NO_PRESSED) 
		{
        return check;
    }

    /* Set row 2 to LOW */
    LVQ_KEYPAD_INT_SetColumn(2);
    /* Check columns */
    check = LVQ_KEYPAD_INT_CheckRow(2);
    if (check != KEYPAD_NO_PRESSED) 
		{
        return check;
    }

    /* Set row 3 to LOW */
    LVQ_KEYPAD_INT_SetColumn(3);
    /* Check columns */
    check = LVQ_KEYPAD_INT_CheckRow(3);
    if (check != KEYPAD_NO_PRESSED) 
		{
        return check;
    }

    if (LVQ_KEYPAD_INT_KeypadType == LVQ_KEYPAD_Type_Large) 
		{
        /* Set column 4 to LOW */
        LVQ_KEYPAD_INT_SetColumn(4);
        /* Check rows */
        check = LVQ_KEYPAD_INT_CheckRow(4);
        if (check != KEYPAD_NO_PRESSED) 
				{
            return check;
        }
    }

    /* Not pressed */
    return KEYPAD_NO_PRESSED;
}

void LVQ_KEYPAD_Update(void) 
{
    static uint16_t millis = 0;

    /* Every X ms read */
    if (++millis >= KEYPAD_READ_INTERVAL && KeypadStatus == LVQ_KEYPAD_Button_NOPRESSED) {
        /* Reset */
        millis = 0;

        /* Read keyboard */
        KeypadStatus = (LVQ_KEYPAD_Button_t) LVQ_KEYPAD_INT_Read();
    }
}
