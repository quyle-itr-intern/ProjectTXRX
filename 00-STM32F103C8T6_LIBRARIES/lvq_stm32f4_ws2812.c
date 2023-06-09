/**
  ******************************************************************************

  WS2812 For SLVQ32F446RE
  Author:   LVQ
  Updated:  9 July 2022

  ******************************************************************************
*/
#include "lvq_stm32f4_ws2812.h"

extern LVQ_PWM_TIM_t TIM4_Data;

uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4];  // for brightness

void Set_LED(int LEDnum, int Red, int Green, int Blue)
{
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = Green;
	LED_Data[LEDnum][2] = Red;
	LED_Data[LEDnum][3] = Blue;
}

void Set_Brightness (int brightness)  // 0-45
{
#if USE_BRIGHTNESS

	if (brightness > 45) brightness = 45;
	for (int i=0; i<MAX_LED; i++)
	{
		LED_Mod[i][0] = LED_Data[i][0];
		for (int j=1; j<4; j++)
		{
			float angle = 90-brightness;  // in degrees
			angle = angle*PI / 180;  // in rad
			LED_Mod[i][j] = (LED_Data[i][j])/(tan(angle));
		}
	}

#endif
}

void Set_Brightness_SingleLed(uint16_t i, int brightness)  
{
	if (brightness > 45) brightness = 45;
	
	for (uint8_t j = 1; j < 4; j++)
	{
			float angle = 90-brightness;  // in degrees
			angle = angle*PI / 180;  // in rad
			LED_Data[i][j] = (LED_Data[i][j])/(tan(angle));
	}
}

uint8_t datasentflag = 0;

uint16_t pwmData[(24*MAX_LED)+50];

void WS2812_Send(void)
{
	// Set_Brightness(10);
	
	uint32_t indx=0;
	uint32_t color;

	for (int i= 0; i<MAX_LED; i++)
	{
#if USE_BRIGHTNESS
		color = ((LED_Mod[i][1]<<16) | (LED_Mod[i][2]<<8) | (LED_Mod[i][3]));
#else
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));
#endif

		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				pwmData[indx] = 75;  // 2/3 of 90
			}

			else pwmData[indx] = 38;  // 1/3 of 90

			indx++;
		}

	}

	for (int i=0; i<50; i++)
	{
		pwmData[indx] = 0;
		indx++;
	}

	LVQ_PWM_DMA_SetDuty(pwmData);
	while (!datasentflag){};
	datasentflag = 0;
}

uint8_t effStep = 0;

uint8_t rainbow_effect_left() 
{
  float factor1, factor2;
  uint16_t ind;
  for(uint16_t j=0;j<MAX_LED;j++)
	{
    ind = effStep + j * 1.9565217391304348;
    switch((int)((ind % 45) / 15)) {
      case 0: factor1 = 1.0 - ((float)(ind % 45 - 0 * 15) / 15);
              factor2 = (float)((int)(ind - 0) % 45) / 15;
              Set_LED(j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
							WS2812_Send();
              break;
      case 1: factor1 = 1.0 - ((float)(ind % 45 - 1 * 15) / 15);
              factor2 = (float)((int)(ind - 15) % 45) / 15;
              Set_LED(j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
              WS2812_Send();
              break;
      case 2: factor1 = 1.0 - ((float)(ind % 45 - 2 * 15) / 15);
              factor2 = (float)((int)(ind - 30) % 45) / 15;
              Set_LED(j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
							WS2812_Send();
              break;
    }
  }
  if(effStep >= 45) {effStep = 0; return 0x03; }
  else effStep++;
  return 0x01;
}


void Led_Single_Run(void)
{
	for(uint8_t i = 0; i < MAX_LED; i++)
	{
		 Set_LED(i, 0, 255, 0);
	}
	
	for(uint8_t i = 0; i < MAX_LED; i++)
	{
			Set_LED(i, 255, 0, 0);
			Set_LED(i - 1 , 0, 255, 0);
			WS2812_Send();
			Delay_ms_systick(50);
	}
}

void Light_Up(uint8_t red, uint8_t green, uint8_t blue, uint32_t speed)
{
		for(int16_t i = 0; i < MAX_LED; i++)
		{
				Set_LED(i, red, green, blue);
				WS2812_Send();
				Delay_ms_systick(speed);
		}
		for(int16_t i = MAX_LED - 1; i >= 0; i--)
		{
				Set_LED(i, 0, 0, 0);
				WS2812_Send();
				Delay_ms_systick(speed);
		}
}

void LVQ_DMA_TransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream) 
{
    /* NOTE: This function should not be modified, when the callback is needed,
            the LVQ_DMA_TransferCompleteHandler could be implemented in the user file
    */
	/* Enable DMA */
	DMA_Cmd(DMA1_Stream0, DISABLE);
	TIM_Cmd(TIM4, DISABLE);
	
	datasentflag = 1;
}

void Fade(uint8_t red_begin, uint8_t green_begin,uint8_t blue_begin, uint8_t red_end, uint8_t green_end,uint8_t blue_end, uint32_t speed) 
{
  uint8_t r,g,b;
  double e;
  e = (effStep * 9) / (double)423;
  r = ( e ) * red_end + red_begin * ( 1.0 - e );
  g = ( e ) * green_end + green_begin * ( 1.0 - e );
  b = ( e ) * blue_end + blue_begin * ( 1.0 - e );
  for(uint16_t j = 0; j < MAX_LED; j++) 
	{
    if((j % 1) == 0)
		{
      Set_LED(j, r, g, b);
		}
    else
		{
      Set_LED(j, 0, 0, 0);
		}
  }
	WS2812_Send();
	Delay_ms_systick(speed);
	
  if(effStep >= 47) 
	{
		effStep = 0; 
	}
  else effStep++;
}

void Rain(uint8_t red, uint8_t green, uint8_t blue, uint32_t speed)
{
		uint8_t Brightness = 45;
		for(uint8_t i = 0; i < 45/5; i++)
		{
				Set_LED(i, red, green, blue);
				Set_Brightness_SingleLed(i, 5*i);
		}
		WS2812_Send();
		uint8_t NumberStart = 1, NumberEnd = 9;
		for(uint8_t i = 0; i < 60; i++)
		{
			 for(int8_t j = NumberEnd; j >= NumberStart; j--)
			 {
					for (uint8_t k = 1; k < 4; k++)
					{
							LED_Data[j][k] = LED_Data[j-1][k];
					}
			 }
			 Set_LED(NumberStart, 0, 0, 0);
			 WS2812_Send();
			 Delay_ms_systick(speed);
			 NumberStart++;
			 NumberEnd++;
		}
}

void Rain_2(uint8_t red, uint8_t green, uint8_t blue, uint32_t speed)
{
		uint8_t Brightness = 45;
		for(uint8_t i = 0; i < 45/5; i++)
		{
				Set_LED(i, red, green, blue);
				Set_Brightness_SingleLed(i, 5*i);
		}
		
		for(uint8_t i = 12; i < 21; i++)
		{
				Set_LED(i, red, green, blue);
				Set_Brightness_SingleLed(i, 5*(i - 12));
		}
		
		WS2812_Send();
		uint8_t NumberStart = 1, NumberEnd = 22;
		for(uint8_t i = 0; i < 59; i++)
		{
			 for(int8_t j = NumberEnd; j >= NumberStart; j--)
			 {
					for (uint8_t k = 1; k < 4; k++)
					{
							LED_Data[j][k] = LED_Data[j-1][k];
					}
			 }
			 Set_LED(NumberStart, 0, 0, 0);
			 Set_LED(NumberStart + 12, 0, 0, 0);
			 WS2812_Send();
			 Delay_ms_systick(speed);
			 NumberStart++;
			 NumberEnd++;
		}
}
