/**
  ******************************************************************************

  ENCODER For SLVQ32F446RE
  Author:   LVQ
  Updated:  23st August 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_encoder.h"

/* Return with status macro */
#define RETURN_WITH_STATUS(p, s)    (p)->Rotation = s; return s

void LVQ_ENCODER_Init(LVQ_ENCODER_t* data, GPIO_TypeDef* GPIO_A_Port, uint16_t GPIO_A_Pin, GPIO_TypeDef* GPIO_B_Port, uint16_t GPIO_B_Pin) 
{
    /* Save parameters */
    data->GPIO_A = GPIO_A_Port;
    data->GPIO_B = GPIO_B_Port;
    data->GPIO_PIN_A = GPIO_A_Pin;
    data->GPIO_PIN_B = GPIO_B_Pin;

		
    /* Set pin A as exti interrupt */
    LVQ_EXTI_Init(data->GPIO_A, data->GPIO_PIN_A, LVQ_EXTI_Trigger_Rising_Falling);

		#if defined(ENCODER_MODE_X1) || defined(ENCODER_MODE_X2)
				/* Set pin B as input */
				LVQ_GPIO_Init(data->GPIO_B, data->GPIO_PIN_B, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Low);
		#elif defined(ENCODER_MODE_X4)
				/* Set pin B as exti interrupt */
				LVQ_EXTI_Init(data->GPIO_B, data->GPIO_PIN_B, LVQ_EXTI_Trigger_Rising_Falling);
		#endif
	
    /* Set default mode */
    data->Mode = LVQ_ENCODER_Mode_Zero;

    /* Set default */
    data->RE_Count = 0;
    data->Diff = 0;
    data->Absolute = 0;
    data->LastA = 1;
}

LVQ_ENCODER_Rotate_t LVQ_ENCODER_Get(LVQ_ENCODER_t* data) 
{
    /* Calculate everything */
    data->Diff = data->RE_Count - data->Absolute;
    data->Absolute = data->RE_Count;

    /* Check */
    if (data->RE_Count < 0) 
		{
        RETURN_WITH_STATUS(data, LVQ_ENCODER_Rotate_Decrement);
    } 
		else if (data->RE_Count > 0) 
		{
        RETURN_WITH_STATUS(data, LVQ_ENCODER_Rotate_Increment);
    }

    RETURN_WITH_STATUS(data, LVQ_ENCODER_Rotate_Nothing);
}

void LVQ_ENCODER_SetMode(LVQ_ENCODER_t* data, LVQ_ENCODER_Mode_t mode) 
{
    /* Set mode */
    data->Mode = mode;
}

void LVQ_ENCODER_Process(LVQ_ENCODER_t* data) 
{
    uint8_t now_a;
    uint8_t now_b;

    /* Read inputs */
    now_a = LVQ_GPIO_GetInputPinValue(data->GPIO_A, data->GPIO_PIN_A);
    now_b = LVQ_GPIO_GetInputPinValue(data->GPIO_B, data->GPIO_PIN_B);

		#if defined(ENCODER_MODE_X1) || defined(ENCODER_MODE_X2)
    /* Check difference */
    if (now_a != data->LastA) 
		{
        data->LastA = now_a;
				
				#if defined(ENCODER_MODE_X1)
						if (data->LastA == 0) 
						{
								if (data->Mode == LVQ_ENCODER_Mode_Zero) 
								{
										if (now_b == 1) 
										{
												data->RE_Count--;
										} 
										else 
										{
												data->RE_Count++;
										}
								} 
								else if (data->Mode == LVQ_ENCODER_Mode_One)
								{
										if (now_b == 1) 
										{
												data->RE_Count++;
										} 
										else 
										{
												data->RE_Count--;
										}
								}
						}
				#elif defined(ENCODER_MODE_X2)
						if(now_a == 0)
						{
								if (now_b == 1) 
								{
										data->RE_Count++;
								} 
								else 
								{
										data->RE_Count--;
								}
						}
						else if(now_a == 1)
						{ 
								if (now_b == 0) 
								{
										data->RE_Count++;
								} 
								else 
								{
										data->RE_Count--;
								}
						}
				#endif
    }
		#elif defined(ENCODER_MODE_X4)
				if(data->LastA == now_b)
						data->RE_Count++;
				else
						data->RE_Count--;
				data->LastA = now_a;
				data->LastB = now_b;
		#endif
}

