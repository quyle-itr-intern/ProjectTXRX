#include "lvq_stm32f4_uitx.h"
#include "lvq_stm32f4_oledmenu.h"

extern uint16_t AdcValues[];

enum {
	LVQ_VOL1 = 1,
	LVQ_VOL2 = 2,
	LVQ_VOL3 = 3,
	LVQ_VOL4 = 4,
	LVQ_VOL5 = 5,
	LVQ_VOL6 = 6,
	LVQ_VOL7 = 7,
	LVQ_VOL8 = 8
};

#define MAX_VOL_1 	3470
#define MID_VOL_1 	1810
#define MIN_VOL_1 	450

#define MAX_VOL_2 	3470
#define MID_VOL_2 	1810
#define MIN_VOL_2 	450

#define MAX_VOL_3 	3470
#define MID_VOL_3 	1810
#define MIN_VOL_3 	450

#define MAX_VOL_4 	3470
#define MID_VOL_4 	1810
#define MIN_VOL_4 	450

#define MAX_VOL_5 	3470
#define MID_VOL_5 	1810
#define MIN_VOL_5 	450

#define MAX_VOL_6 	3470
#define MID_VOL_6 	1810
#define MIN_VOL_6 	450

void LVQ_UITX_Loading(void)
{
			LVQ_SSD1306_Clear();
			LVQ_SSD1306_UpdateScreen(); //display
		
			LVQ_SSD1306_DrawBitmap(0, 0, LVQ_UITX_HomeScreen, 128, 64, 1);
		
			for(uint8_t i = 0; i < 101; i++)
			{
					LVQ_SSD1306_DrawFilledRectangle(13, 53, i, 6, 1);
					LVQ_SSD1306_UpdateScreen(); //display
					Delay_ms_systick(50);
			}
			LVQ_SSD1306_Clear();
			LVQ_SSD1306_UpdateScreen(); //display
}

void LVQ_Caculator_Volume(uint8_t x, uint8_t y, uint8_t u8value)
{
			LVQ_SSD1306_DrawBitmap(x, y, LVQ_UITX_ICON_SLIDER, 104, 10, 1);
			x += 1;
			y += 2;
			if( u8value > 50 )
			{			
					LVQ_SSD1306_DrawFilledRectangle(x + 50, y, u8value - 50, 5, 1);
					LVQ_SSD1306_DrawFilledRectangle(x + u8value+  1, y, 100 - u8value, 5, 0);
					LVQ_SSD1306_DrawFilledRectangle(x, y, 49, 5, 0);
			}
			else if( u8value < 50 )	
			{			
					LVQ_SSD1306_DrawFilledRectangle(x + u8value, y, 51 - u8value, 5, 1);
					LVQ_SSD1306_DrawFilledRectangle(x, y, u8value, 5, 0);
					LVQ_SSD1306_DrawFilledRectangle(x + 51, y, 50 , 5, 0);
			}		
			else
			{
					LVQ_SSD1306_DrawFilledRectangle(x + 50, y, 1, 5, 1);
					LVQ_SSD1306_DrawFilledRectangle(x, y, 49, 5, 0);
					LVQ_SSD1306_DrawFilledRectangle(x + 51, y, 49, 5, 0);
			}
}

void LVQ_MapVolume(uint8_t x, uint8_t y, uint8_t u8NoValue)
{
			uint8_t u8value;
		
			switch(u8NoValue)
			{
					case LVQ_VOL1:
							if( AdcValues[0] > MID_VOL_1 )
								u8value = map(AdcValues[0], MID_VOL_1, MAX_VOL_1, 50, 100);
							if( AdcValues[0] < MID_VOL_1 )
								u8value = map(AdcValues[0], MIN_VOL_1, MID_VOL_1, 0, 50);
							LVQ_Caculator_Volume(x, y, u8value);
							break;
					case LVQ_VOL2:
							if( AdcValues[1] > MID_VOL_2 )
								u8value = map(AdcValues[1], MID_VOL_2, MAX_VOL_2, 50, 100);
							if( AdcValues[1] < MID_VOL_2 )
								u8value = map(AdcValues[1], MIN_VOL_2, MID_VOL_2, 0, 50);
							LVQ_Caculator_Volume(x, y, u8value);
							break;
					case LVQ_VOL3:
							if( AdcValues[2] > MID_VOL_3 )
								u8value = map(AdcValues[2], MID_VOL_3, MAX_VOL_3, 50, 100);
							if( AdcValues[2] < MID_VOL_3 )
								u8value = map(AdcValues[2], MIN_VOL_3, MID_VOL_3, 0, 50);
							LVQ_Caculator_Volume(x, y, u8value);
							break;
					case LVQ_VOL4:		
							if( AdcValues[3] > MID_VOL_4 )
								u8value = map(AdcValues[3], MID_VOL_4, MAX_VOL_4, 50, 100);
							if( AdcValues[3] < MID_VOL_4 )
								u8value = map(AdcValues[3], MIN_VOL_4, MID_VOL_4, 0, 50);
							LVQ_Caculator_Volume(x, y, u8value);
							break;
					case LVQ_VOL5:		
							if( AdcValues[4] > MID_VOL_5 )
								u8value = map(AdcValues[4], MID_VOL_5, MAX_VOL_5, 50, 100);
							if( AdcValues[4] < MID_VOL_5 )
								u8value = map(AdcValues[4], MIN_VOL_5, MID_VOL_5, 0, 50);
							LVQ_Caculator_Volume(x, y, u8value);
							break;	
					case LVQ_VOL6:		
							if( AdcValues[5] > MID_VOL_6 )
								u8value = map(AdcValues[5], MID_VOL_6, MAX_VOL_6, 50, 100);
							if( AdcValues[5] < MID_VOL_6 )
								u8value = map(AdcValues[5], MIN_VOL_6, MID_VOL_6, 0, 50);
							LVQ_Caculator_Volume(x, y, u8value);
							break;				
			}	
}

void LVQ_UITX_Volume(void)
{
			int8_t select = 0;
			int8_t button_right_clicked = 0;
			int8_t button_left_clicked = 0;
			
			start:;
		
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_12) )
						return;
		
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_15) && button_right_clicked == 0)
			{
					button_right_clicked = 1;
					select = 0;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_15) && button_right_clicked == 1)
					button_right_clicked = 0;
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_14) && button_left_clicked == 0)
			{
					button_left_clicked = 1;
					select = 1;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_14) && button_left_clicked == 1)
					button_left_clicked = 0;
		
			if( !select )
			{
						LVQ_SSD1306_Fill(0);
						LVQ_SSD1306_DrawRectangle(0, 0, 128, 64, 1);
				
						LVQ_SSD1306_GotoXY(12, 1);
						LVQ_SSD1306_Puts("-     CH1     +", &Font_7x10, 1);
						LVQ_MapVolume(12, 10, 1);
				
						LVQ_SSD1306_GotoXY(12, 22);
						LVQ_SSD1306_Puts("-     CH2     +", &Font_7x10, 1);
						LVQ_MapVolume(12, 31, 2);
				
						LVQ_SSD1306_GotoXY(12, 43);
						LVQ_SSD1306_Puts("-     CH3     +", &Font_7x10, 1);
						LVQ_MapVolume(12, 52, 3);
				
						LVQ_SSD1306_UpdateScreen(); //display
			}
			else
			{
						LVQ_SSD1306_Fill(0);
						LVQ_SSD1306_DrawRectangle(0, 0, 128, 64, 1);
				
						LVQ_SSD1306_GotoXY(12, 1);
						LVQ_SSD1306_Puts("-     CH4     +", &Font_7x10, 1);
						LVQ_MapVolume(12, 10, 4);
				
						LVQ_SSD1306_GotoXY(12, 22);
						LVQ_SSD1306_Puts("-     CH5     +", &Font_7x10, 1);
						LVQ_MapVolume(12, 31, 5);
				
						LVQ_SSD1306_GotoXY(12, 43);
						LVQ_SSD1306_Puts("-     CH6     +", &Font_7x10, 1);
						LVQ_MapVolume(12, 52, 6);
				
						LVQ_SSD1306_UpdateScreen(); //display
			}		
			
			goto start;
}

void LVQ_UITX_Display(void)
{
			LVQ_SSD1306_Clear();
	
			const int8_t NUM_ITEMS = 10;
			int8_t padding = 10;
	
			int8_t item_selected = 1; 
			int8_t item_sel_previous; 
			int8_t item_sel_next;
			int8_t lastselect = 0;
			int8_t button_right_clicked = 0;
			int8_t button_left_clicked = 0;
			int8_t button_ok_clicked = 0;
	
			start:;
	
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_13) && button_ok_clicked == 0)
			{
					button_ok_clicked = 1;
					if( item_selected == 1 )
							LVQ_UITX_Reverse();
					else if ( item_selected == 2 )
							LVQ_UITX_Battery();
					else if ( item_selected == 9 )
							LVQ_UITX_Volume();
					else if ( item_selected == 4 )
							LVQ_UITX_EndPoint();
					lastselect = 10;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_13) && button_ok_clicked == 1)
			{
					Delay_ms_systick(100);
					button_ok_clicked = 0;
			}
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_15) && button_right_clicked == 0)
			{
					button_right_clicked = 1;
					item_selected++;
					if( item_selected == NUM_ITEMS)
							item_selected = 0;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_15) && button_right_clicked == 1)
			{
					Delay_ms_systick(100);
					button_right_clicked = 0;
			}
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_14) && button_left_clicked == 0)
			{
					button_left_clicked = 1;
					item_selected--;
					if( item_selected < 0 )
							item_selected = NUM_ITEMS - 1;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_14) && button_left_clicked == 1)
			{
					Delay_ms_systick(100);
					button_left_clicked = 0;
			}
			
			if( item_selected != lastselect )
					lastselect = item_selected;
			else
					goto start;
	
			item_sel_previous = item_selected - 1;
			if (item_sel_previous < 0) {item_sel_previous = NUM_ITEMS - 1;} 
			item_sel_next = item_selected + 1;  
			if (item_sel_next >= NUM_ITEMS) {item_sel_next = 0;} 
	
			LVQ_SSD1306_Fill(0);
			
			LVQ_SSD1306_DrawBitmap(2 + padding, 4, LVQ_UITX_MENUS[item_sel_previous], 16, 16, 1);
			LVQ_SSD1306_GotoXY(25 + padding, 9);
			LVQ_SSD1306_Puts(LVQ_UITX_MENU_ITEMS[item_sel_previous], &Font_7x10, 1);
		
			LVQ_SSD1306_DrawBitmap(2 + padding, 24, LVQ_UITX_MENUS[item_selected], 16, 16, 1);
			LVQ_SSD1306_GotoXY(25 + padding, 29);
			LVQ_SSD1306_Puts(LVQ_UITX_MENU_ITEMS[item_selected], &Font_7x10, 1);
		
			LVQ_SSD1306_DrawBitmap(2 + padding, 44, LVQ_UITX_MENUS[item_sel_next], 16, 16, 1);
			LVQ_SSD1306_GotoXY(25 + padding, 49);
			LVQ_SSD1306_Puts(LVQ_UITX_MENU_ITEMS[item_sel_next], &Font_7x10, 1);
			
			LVQ_SSD1306_DrawBitmap(0, 26, LVQ_UITX_Arrow_Horizontial, 9, 13, 1);
			LVQ_SSD1306_DrawBitmap(120, 0, LVQ_UITX_Scrollbar_Background, 8, 64, 1);
			
			LVQ_SSD1306_DrawFilledRectangle(125, 64/NUM_ITEMS * item_selected, 3, 64/NUM_ITEMS, 1);
			
			LVQ_SSD1306_UpdateScreen(); //display
			
			goto start;	
}

void LVQ_UITX_Reverse(void)
{
			Delay_ms_systick(100);
			LVQ_SSD1306_Clear();
			LVQ_SSD1306_UpdateScreen(); //display
			
			uint8_t configReverse = 0x00;
			
			int8_t select = 1;
			int8_t lastselect = 0;
			int8_t button_right_clicked = 0;
			int8_t button_left_clicked = 0;
			int8_t button_ok_clicked = 0;
			
			start:;
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_12) )
						return;
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_13) && button_ok_clicked == 0)
			{
					button_ok_clicked = 1;
					
					if( configReverse & ( 1 << (select - 1) ) )
								configReverse &= ~( 1 << (select - 1) );
					else
								configReverse |= ( 1 << (select - 1) );
					goto display;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_13) && button_ok_clicked == 1)
			{
					Delay_ms_systick(100);
					button_ok_clicked = 0;
			}
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_15) && button_right_clicked == 0)
			{
					button_right_clicked = 1;
					select++;
					if (select == 9)
							select = 1;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_15) && button_right_clicked == 1)
			{
					Delay_ms_systick(100);
					button_right_clicked = 0;
			}
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_14) && button_left_clicked == 0)
			{
					button_left_clicked = 1;
					select--;
					if (select == 0)
							select = 8;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_14) && button_left_clicked == 1)
			{
					Delay_ms_systick(100);
					button_left_clicked = 0;
			}
			
			if( select != lastselect )
					lastselect = select;
			else
					goto start;
			
			display:
			LVQ_SSD1306_Fill(0);
			LVQ_SSD1306_GotoXY(40, 3);
			LVQ_SSD1306_Puts("Reverse", &Font_7x10, 1);
			LVQ_SSD1306_DrawRectangle(0, 0, 128, 64, 1);
			
			LVQ_SSD1306_GotoXY(9, 	45);
			LVQ_SSD1306_Putc('1', &Font_7x10, 1);
			LVQ_SSD1306_GotoXY(24, 	45);
			LVQ_SSD1306_Putc('2', &Font_7x10, 1);
			LVQ_SSD1306_GotoXY(39, 	45);
			LVQ_SSD1306_Putc('3', &Font_7x10, 1);
			LVQ_SSD1306_GotoXY(54, 	45);
			LVQ_SSD1306_Putc('4', &Font_7x10, 1);
			LVQ_SSD1306_GotoXY(69, 	45);
			LVQ_SSD1306_Putc('5', &Font_7x10, 1);
			LVQ_SSD1306_GotoXY(84, 	45);
			LVQ_SSD1306_Putc('6', &Font_7x10, 1);
			LVQ_SSD1306_GotoXY(99, 	45);
			LVQ_SSD1306_Putc('7', &Font_7x10, 1);
			LVQ_SSD1306_GotoXY(114, 45);
			LVQ_SSD1306_Putc('8', &Font_7x10, 1);
			
			if( configReverse & ( 1 << 0 ) )
					LVQ_SSD1306_DrawBitmap(7,  	25, LVQ_UITX_RE_1, 10, 16, 1);
			else
					LVQ_SSD1306_DrawBitmap(7,  	25, LVQ_UITX_RE_2, 10, 16, 1);
			
			if( configReverse & ( 1 << 1 ) )
					LVQ_SSD1306_DrawBitmap(22, 	25, LVQ_UITX_RE_1, 10, 16, 1);
			else
					LVQ_SSD1306_DrawBitmap(22, 	25, LVQ_UITX_RE_2, 10, 16, 1);
			
			if( configReverse & ( 1 << 2 ) )
					LVQ_SSD1306_DrawBitmap(37, 	25, LVQ_UITX_RE_1, 10, 16, 1);
			else
					LVQ_SSD1306_DrawBitmap(37, 	25, LVQ_UITX_RE_2, 10, 16, 1);
			
			if( configReverse & ( 1 << 3 ) )
					LVQ_SSD1306_DrawBitmap(52, 	25, LVQ_UITX_RE_1, 10, 16, 1);
			else
					LVQ_SSD1306_DrawBitmap(52, 	25, LVQ_UITX_RE_2, 10, 16, 1);
			
			if( configReverse & ( 1 << 4 ) )
					LVQ_SSD1306_DrawBitmap(67, 	25, LVQ_UITX_RE_1, 10, 16, 1);
			else
					LVQ_SSD1306_DrawBitmap(67, 	25, LVQ_UITX_RE_2, 10, 16, 1);
			
			if( configReverse & ( 1 << 5 ) )
					LVQ_SSD1306_DrawBitmap(82, 	25, LVQ_UITX_RE_1, 10, 16, 1);
			else
					LVQ_SSD1306_DrawBitmap(82, 	25, LVQ_UITX_RE_2, 10, 16, 1);
			
			if( configReverse & ( 1 << 6 ) )
					LVQ_SSD1306_DrawBitmap(97, 	25, LVQ_UITX_RE_1, 10, 16, 1);
			else
					LVQ_SSD1306_DrawBitmap(97, 	25, LVQ_UITX_RE_2, 10, 16, 1);
			
			if( configReverse & ( 1 << 7 ) )
					LVQ_SSD1306_DrawBitmap(112, 25, LVQ_UITX_RE_1, 10, 16, 1);
			else
					LVQ_SSD1306_DrawBitmap(112, 25, LVQ_UITX_RE_2, 10, 16, 1);
			
			switch(select)
			{
				case 1:
						LVQ_SSD1306_DrawFilledRectangle(5, 12, 120, 9, 0);
						LVQ_SSD1306_UpdateScreen(); //display
						LVQ_SSD1306_DrawBitmap(5, 12, LVQ_UITX_Arrow_Verticial, 13, 9, 1);
						LVQ_SSD1306_UpdateScreen(); //display
						break;
				case 2:
						LVQ_SSD1306_DrawFilledRectangle(5, 12, 120, 9, 0);
						LVQ_SSD1306_UpdateScreen(); //display
						LVQ_SSD1306_DrawBitmap(20, 12, LVQ_UITX_Arrow_Verticial, 13, 9, 1);
						LVQ_SSD1306_UpdateScreen(); //display
						break;
				case 3:
						LVQ_SSD1306_DrawFilledRectangle(5, 12, 120, 9, 0);
						LVQ_SSD1306_UpdateScreen(); //display
						LVQ_SSD1306_DrawBitmap(35, 12, LVQ_UITX_Arrow_Verticial, 13, 9, 1);
						LVQ_SSD1306_UpdateScreen(); //display
						break;
				case 4:
						LVQ_SSD1306_DrawFilledRectangle(5, 12, 120, 9, 0);
						LVQ_SSD1306_UpdateScreen(); //display
						LVQ_SSD1306_DrawBitmap(50, 12, LVQ_UITX_Arrow_Verticial, 13, 9, 1);
						LVQ_SSD1306_UpdateScreen(); //display
						break;
				case 5:
						LVQ_SSD1306_DrawFilledRectangle(5, 12, 120, 9, 0);
						LVQ_SSD1306_UpdateScreen(); //display
						LVQ_SSD1306_DrawBitmap(65, 12, LVQ_UITX_Arrow_Verticial, 13, 9, 1);
						LVQ_SSD1306_UpdateScreen(); //display
						break;
				case 6:
						LVQ_SSD1306_DrawFilledRectangle(5, 12, 120, 9, 0);
						LVQ_SSD1306_UpdateScreen(); //display
						LVQ_SSD1306_DrawBitmap(80, 12, LVQ_UITX_Arrow_Verticial, 13, 9, 1);
						LVQ_SSD1306_UpdateScreen(); //display
						break;
				case 7:
						LVQ_SSD1306_DrawFilledRectangle(5, 12, 120, 9, 0);
						LVQ_SSD1306_UpdateScreen(); //display
						LVQ_SSD1306_DrawBitmap(95, 12, LVQ_UITX_Arrow_Verticial, 13, 9, 1);
						LVQ_SSD1306_UpdateScreen(); //display
						break;
				case 8:
						LVQ_SSD1306_DrawFilledRectangle(5, 12, 120, 9, 0);
						LVQ_SSD1306_UpdateScreen(); //display
						LVQ_SSD1306_DrawBitmap(110, 12, LVQ_UITX_Arrow_Verticial, 13, 9, 1);
						LVQ_SSD1306_UpdateScreen(); //display
						break;
			}			
			goto start;
}

void LVQ_UITX_EndPoint(void)
{		
			Delay_ms_systick(200);
	
			char str[20];
				
			int8_t position_arrow[8][2] = { {36, 12}, 
																			{85, 12},
																			{36, 24},
																			{85, 24},
																			{36, 36},
																			{85, 36},
																			{36, 48},
																			{85, 48},
			};
			int8_t select_arrow = 0;
			int8_t channel = 0;
			int8_t rightpoint = 100, leftpoint = 100;
			int8_t button_right_clicked = 0;
			int8_t button_left_clicked = 0;
			int8_t button_ok_clicked = 0;
	
			start:;
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_12) )
						return;
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_13) && button_ok_clicked == 0)
			{
					button_ok_clicked = 1;
					channel = !channel;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_13) && button_ok_clicked == 1)
					button_ok_clicked = 0;
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_15) && button_right_clicked == 0)
			{
					button_right_clicked = 1;
					select_arrow++;
					if (select_arrow == 8)
							select_arrow = 0;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_15) && button_right_clicked == 1)
					button_right_clicked = 0;
			
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_14) && button_left_clicked == 0)
			{
					button_left_clicked = 1;
					select_arrow--;
					if (select_arrow < 0)
							select_arrow = 7;
			}
			
			if( LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_14) && button_left_clicked == 1)
					button_left_clicked = 0;
	
			LVQ_SSD1306_Fill(0);
			LVQ_SSD1306_GotoXY(35, 3);
			LVQ_SSD1306_Puts("End Point", &Font_7x10, 1);
		
			LVQ_SSD1306_GotoXY(4, 15);
			sprintf(str, "CH%d   %3d%%   %3d%%", channel*4 + 1, leftpoint, rightpoint);
			LVQ_SSD1306_Puts(str, &Font_7x10, 1);
	
			LVQ_SSD1306_GotoXY(4, 27);
			sprintf(str, "CH%d   %3d%%   %3d%%", channel*4 + 2, leftpoint, rightpoint);
			LVQ_SSD1306_Puts(str, &Font_7x10, 1);
	
			LVQ_SSD1306_GotoXY(4, 39);
			sprintf(str, "CH%d   %3d%%   %3d%%", channel*4 + 3, leftpoint, rightpoint);
			LVQ_SSD1306_Puts(str, &Font_7x10, 1);
	
			LVQ_SSD1306_GotoXY(4, 51);
			sprintf(str, "CH%d   %3d%%   %3d%%", channel*4 + 4, leftpoint, rightpoint);
			LVQ_SSD1306_Puts(str, &Font_7x10, 1);
	
			LVQ_SSD1306_DrawRectangle(0, 0, 128, 64, 1);
			LVQ_SSD1306_DrawBitmap(position_arrow[select_arrow][0], position_arrow[select_arrow][1], LVQ_UITX_Arrow_Small, 6, 9, 1);
			
			LVQ_SSD1306_UpdateScreen(); //display

			goto start;
}

void LVQ_UITX_Battery(void)
{
			char str[5];
			float fBattery = 0;
			LVQ_SSD1306_Fill(0);
			start:;
	
			if( !LVQ_GPIO_GetInputPinValue(GPIOB, GPIO_PIN_12) )
						return;
			
			LVQ_SSD1306_GotoXY(0, 20);
			LVQ_SSD1306_Puts("Volts Battery TX", &Font_7x10, 1);
			
			LVQ_SSD1306_GotoXY(0, 30);
			fBattery = ( 3.3 / 4095 )* AdcValues[6] * (28.0 / 11 );
			sprintf(str, "%.2f", fBattery);
			LVQ_SSD1306_Puts(str, &Font_7x10, 1);
			
			LVQ_SSD1306_UpdateScreen(); //display
			
			Delay_ms_systick(200);

			goto start;
}
