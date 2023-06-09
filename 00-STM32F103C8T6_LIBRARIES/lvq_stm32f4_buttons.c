/**
  ******************************************************************************

  BUTTONS For SLVQ32F446RE
  Author:   LVQ
  Updated:  26st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_buttons.h"

/* Button states */
#define BUTTON_STATE_START        0
#define BUTTON_STATE_PRESSED      1
#define BUTTON_STATE_WAITRELEASE  2

/* Internal structure */
typedef struct {
    LVQ_BUTTON_t* Buttons[BUTTON_MAX_BUTTONS];
    uint16_t ButtonsCount;
} LVQ_BUTTON_INT_t;
static LVQ_BUTTON_INT_t Buttons;

/* Internal functions */
void LVQ_BUTTON_INT_CheckButton(LVQ_BUTTON_t* ButtonStruct);

LVQ_BUTTON_t* LVQ_BUTTON_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t ButtonState, void (*ButtonHandler)(LVQ_BUTTON_PressType_t)) 
{
    LVQ_BUTTON_t* ButtonStruct;
    LVQ_GPIO_PuPd_t P;

    /* Init delay function */
    Delay_Init();

    /* Check if available */
    if (Buttons.ButtonsCount >= BUTTON_MAX_BUTTONS) 
		{
        return NULL;
    }

    /* Allocate memory for button */
    ButtonStruct = (LVQ_BUTTON_t*) malloc(sizeof(LVQ_BUTTON_t));

    /* Check if allocated */
    if (ButtonStruct == NULL) 
		{
        return NULL;
    }

    /* Save settings */
    ButtonStruct->GPIOx = GPIOx;
    ButtonStruct->GPIO_Pin = GPIO_Pin;
    ButtonStruct->GPIO_State = ButtonState ? 1 : 0;
    ButtonStruct->ButtonHandler = ButtonHandler;
    ButtonStruct->State = BUTTON_STATE_START;

    /* Set default values */
    ButtonStruct->PressNormalTime = BUTTON_NORMAL_PRESS_TIME;
    ButtonStruct->PressLongTime = BUTTON_LONG_PRESS_TIME;

    /* Init pin with pull resistor */
    if (ButtonStruct->GPIO_State) {
        /* Pulldown */
        P = LVQ_GPIO_PuPd_DOWN;
    } else {
        /* Pullup */
        P = LVQ_GPIO_PuPd_UP;
    }

    /* Init GPIO pin as input with proper pull resistor */
    LVQ_GPIO_Init(GPIOx, GPIO_Pin, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, P, LVQ_GPIO_Speed_Low);

    /* Save button */
    Buttons.Buttons[Buttons.ButtonsCount++] = ButtonStruct;

    /* Return button pointer */
    return ButtonStruct;
}

LVQ_BUTTON_t* LVQ_BUTTON_SetPressTime(LVQ_BUTTON_t* ButtonStruct, uint16_t Normal, uint16_t Long) 
{
    /* Set values */
    ButtonStruct->PressNormalTime = Normal;
    ButtonStruct->PressLongTime = Long;

    /* Return pointer */
    return ButtonStruct;
}

void LVQ_BUTTON_Update(void) 
{
    uint16_t i;

    /* Go through all buttons */
    for (i = 0; i < Buttons.ButtonsCount; i++) {
        LVQ_BUTTON_INT_CheckButton(Buttons.Buttons[i]);
    }
}

/* Internal functions */
void LVQ_BUTTON_INT_CheckButton(LVQ_BUTTON_t* ButtonStruct) 
{
    uint8_t status = LVQ_GPIO_GetInputPinValue(ButtonStruct->GPIOx, ButtonStruct->GPIO_Pin);
    uint32_t now = Timer_read_ms();

    /* First stage */
    if (ButtonStruct->State == BUTTON_STATE_START) 
		{
        /* Check if pressed */
        if (status == ButtonStruct->GPIO_State) 
				{
            /* Button pressed, go to stage BUTTON_STATE_START */
            ButtonStruct->State = BUTTON_STATE_PRESSED;

            /* Save pressed time */
            ButtonStruct->StartTime = now;

            /* Button pressed OK, call function */
            if (ButtonStruct->ButtonHandler) 
						{
                /* Call function callback */
                ButtonStruct->ButtonHandler(LVQ_BUTTON_PressType_OnPressed);
            }
        }
    }

    if (ButtonStruct->State == BUTTON_STATE_PRESSED) 
		{
        /* Button still pressed */
			  if (status != ButtonStruct->GPIO_State) 
				{
            /* Not pressed */
            if (now > (ButtonStruct->StartTime + ButtonStruct->PressNormalTime) && now < (ButtonStruct->StartTime + ButtonStruct->PressLongTime)) 
						{
                /* Button pressed OK, call function */
                if (ButtonStruct->ButtonHandler) 
								{
                    /* Call function callback */
                    ButtonStruct->ButtonHandler(LVQ_BUTTON_PressType_Normal);
                }

                /* Go to stage BUTTON_STATE_WAITRELEASE */
                ButtonStruct->State = BUTTON_STATE_WAITRELEASE;
            } 
						else if (now > (ButtonStruct->StartTime + ButtonStruct->PressLongTime))
						{
                /* Button pressed OK, call function */
                if (ButtonStruct->ButtonHandler) 
								{
                    /* Call function callback */
                    ButtonStruct->ButtonHandler(LVQ_BUTTON_PressType_Long);
                }

                /* Go to stage BUTTON_STATE_WAITRELEASE */
                ButtonStruct->State = BUTTON_STATE_WAITRELEASE;
            } 
						else
						{
							/* Go to state BUTTON_STATE_START */
							ButtonStruct->State = BUTTON_STATE_WAITRELEASE;
						}
        } else {
            /* Go to state BUTTON_STATE_START */
            ButtonStruct->State = BUTTON_STATE_PRESSED;
        }
    }

    if (ButtonStruct->State == BUTTON_STATE_WAITRELEASE) 
		{
        /* Wait till button released */
        if (status != ButtonStruct->GPIO_State) 
				{
            /* Go to stage 0 again */
            ButtonStruct->State = BUTTON_STATE_START;
        }
    }

    /* Save current status */
    ButtonStruct->LastStatus = status;
}
