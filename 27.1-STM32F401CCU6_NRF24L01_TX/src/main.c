#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_delay.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_usart.h"
#include "lvq_stm32f4_spi.h"
#include "lvq_stm32f4_adc.h"
#include "lvq_stm32f4_adc_dma.h"
#include "lvq_stm32f4_i2c.h"
#include "lvq_stm32f4_ssd1306.h"
#include "lvq_stm32f4_nrf24.h"
#include "lvq_stm32f4_uitx.h"
#include "lvq_stm32f4_74hc165.h"
#include "String.h"

#define ADDRESS_DATAUITX   0x08010000

uitx_info_t UITX_REData = {
			.LVQ_UITX_RE = 0x0000,															
};

uint16_t uitx_volminmax[7][3] = { { 461, 1808, 3460 },
																	{ 400, 2125, 3640 },
																	{ 520, 1950, 3630 },
																	{ 470, 1900, 3480 },
																	{ 0, 	 2048, 4095 },
																	{ 0, 	 2048, 4095 },
																	{ 0, 	 2048, 4095 },															
};

void Buzzer(uint8_t beep, uint16_t delay);
void SetFrequencyLoopSend(uint16_t __frequency);
void PackDataSend(void);
void CheckHome(void);

#define BUZZER_PORT GPIOB
#define BUZZER_PIN  GPIO_PIN_10

LVQ_74HC165_Config_t dataConfig = {
		.__74HC165_PL_PORT = GPIOA,
		.__74HC165_PL_PIN = GPIO_PIN_15,
		.__74HC165_CP_PORT = GPIOA,
		.__74HC165_CP_PIN = GPIO_PIN_12,
		.__74HC165_DA_PORT = GPIOA,
		.__74HC165_DA_PIN = GPIO_PIN_11
};

LVQ_74HC165_Config_t dataTrim = {
		.__74HC165_PL_PORT = GPIOB,
		.__74HC165_PL_PIN = GPIO_PIN_14,
		.__74HC165_CP_PORT = GPIOB,
		.__74HC165_CP_PIN = GPIO_PIN_13,
		.__74HC165_DA_PORT = GPIOB,
		.__74HC165_DA_PIN = GPIO_PIN_12
};

LVQ_74HC165_Config_t dataChannel = {
		.__74HC165_PL_PORT = GPIOB,
		.__74HC165_PL_PIN = GPIO_PIN_15,
		.__74HC165_CP_PORT = GPIOA,
		.__74HC165_CP_PIN = GPIO_PIN_8,
		.__74HC165_DA_PORT = GPIOB,
		.__74HC165_DA_PIN = GPIO_PIN_6
};

#define ADC_CHANNEL 8

volatile uint16_t AdcValues[ADC_CHANNEL];

LVQ_NRF24_Config_t nrf24 = {
			.NRF24_SPI = SPI1,
			.NRF24_SPI_PIN = LVQ_SPI_PinsPack_2,
			.NRF24_CE_PORT = GPIOB,
			.NRF24_CE_PIN = GPIO_PIN_0,
			.NRF24_CSN_PORT = GPIOB,
			.NRF24_CSN_PIN = GPIO_PIN_1,
			.NRF24_Channel = 10,
			.NRF24_TX_Power = LVQ_NRF24_TXPWR_0dBm,
			.NRF24_DataRate = LVQ_NRF24_DR_250kbps,
			.NRF24_SizePayLoad = 32,
			.NRF24_AddressWidth = 5,
	
			.NRF24_TX_Address[0] = 0x7E,
			.NRF24_TX_Address[1] = 0x2C,
			.NRF24_TX_Address[2] = 0xE3,
			.NRF24_TX_Address[3] = 0xE3,
			.NRF24_TX_Address[4] = 0xE3,
	
			.NRF24_RX_Address[0] = 0x7F,
			.NRF24_RX_Address[1] = 0x3C,
			.NRF24_RX_Address[2] = 0xE3,
			.NRF24_RX_Address[3] = 0xE3,
			.NRF24_RX_Address[4] = 0xE3
};

uint8_t LVQ_NRF24_payload[32];

LVQ_NRF24_TXResult_t tx_res;

uint8_t payload_length = 32;

uint16_t TX_PayLoad[16];

uint32_t ADC_AVR[6];

volatile uint8_t value = 0;

LVQ_PWM_TIM_t TIM2_Data;

int main(void)
 {
		Delay_Init();
	
		LVQ_GPIO_Init(GPIOC, GPIO_PIN_13, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High);	
	
		LVQ_GPIO_Init(BUZZER_PORT, BUZZER_PIN, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High);	
		
		LVQ_ADC_DMA_Init(ADC1, GPIOA, GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, ADC_CHANNEL, (uint32_t ) &ADC1->DR, (uint32_t) AdcValues);
		
		LVQ_74HC165_Init(dataConfig);
		LVQ_74HC165_Init(dataTrim);
		LVQ_74HC165_Init(dataChannel);
	 
		LVQ_UITX_ReadData(0x08010000, &UITX_REData);
	 
		LVQ_SSD1306_Init();
		CheckHome();
	
		LVQ_NRF24_Init(nrf24);
	
		if( !LVQ_NRF24_Check(nrf24) )
		{
				while(1)
				{
						LVQ_GPIO_SetPinHigh(GPIOC, GPIO_PIN_13);
						Delay_ms_systick(20);
						LVQ_GPIO_SetPinLow(GPIOC, GPIO_PIN_13);
						Delay_ms_systick(20);
				}
		}
	
		LVQ_NRF24_StartTXMode(&nrf24);
		LVQ_NRF24_PowerUpTx(nrf24);
		
		if( LVQ_74HC165_Read(dataConfig) & 0x08 )
		{
				LVQ_NRF24_RXSearchChannel(nrf24);
				Buzzer(3, 100);	
		}
		
		SetFrequencyLoopSend(20);
		
		Buzzer(2, 200);
		LVQ_UITX_Loading();
		LVQ_UITX_Display();
		LVQ_UITX_EndPoint();
						
		while(1)
		{

		}
}

void PackDataSend(void)
{
		uint8_t u8IndexChannel = 0;
		uint8_t u8DA = LVQ_74HC165_Read(dataChannel);
		for( uint8_t i = 0; i < 16; i++)
		{
				if ( i < 7 )
				{
						if( UITX_REData.LVQ_UITX_RE.REG & ( 1 << i ) )
						{
								if( AdcValues[i] <= uitx_volminmax[i][1] )
										TX_PayLoad[i] = (uint16_t) map(AdcValues[i], uitx_volminmax[i][0], uitx_volminmax[i][1], UITX_REData.LVQ_UITX_EP[i][0], 1700 												 	);
								else
										TX_PayLoad[i] = (uint16_t) map(AdcValues[i], uitx_volminmax[i][1], uitx_volminmax[i][2], 1700													, UITX_REData.LVQ_UITX_EP[i][1]	);
						}
						else
						{
								if( AdcValues[i] <= uitx_volminmax[i][1] )
										TX_PayLoad[i] = (uint16_t) map(AdcValues[i], uitx_volminmax[i][0], uitx_volminmax[i][1], UITX_REData.LVQ_UITX_EP[i][1], 1700													);
								else
										TX_PayLoad[i] = (uint16_t) map(AdcValues[i], uitx_volminmax[i][1], uitx_volminmax[i][2], 1700 												, UITX_REData.LVQ_UITX_EP[i][0]	);
						}
				}
				else if( i < 14)
				{ 
						if( UITX_REData.LVQ_UITX_RE.REG & ( 1 << i ) )
						{
								if( u8DA & ( 1 << u8IndexChannel ) )
										TX_PayLoad[i] = UITX_REData.LVQ_UITX_EP[i][0];
								else
										TX_PayLoad[i] = UITX_REData.LVQ_UITX_EP[i][1];
								u8IndexChannel++;
						}
						else
						{
								if( u8DA & ( 1 << u8IndexChannel ) )
										TX_PayLoad[i] = UITX_REData.LVQ_UITX_EP[i][1];
								else
										TX_PayLoad[i] = UITX_REData.LVQ_UITX_EP[i][0];
								u8IndexChannel++;
						}
				}
				else
						TX_PayLoad[i] = 1700;
		}
		LVQ_NRF24_TransmitPacket(nrf24, (uint8_t *) TX_PayLoad, payload_length);
}

void TIM2_IRQHandler(void)
{
		TIM_ClearFlag(TIM2, TIM_IT_Update);
		PackDataSend();
		LVQ_GPIO_TogglePinValue(GPIOC, GPIO_PIN_13);
}

void Buzzer(uint8_t beep, uint16_t delay)
{
		for(uint8_t i = 0; i < beep; i++)
		{
				LVQ_GPIO_SetPinHigh(BUZZER_PORT, BUZZER_PIN);
				Delay_ms_systick(delay);
				LVQ_GPIO_SetPinLow(BUZZER_PORT, BUZZER_PIN);
				Delay_ms_systick(delay);
		}
}

void SetFrequencyLoopSend(uint16_t __frequency)
{
		LVQ_PWM_InitTimer(TIM2, &TIM2_Data, __frequency );
		
		NVIC_InitTypeDef NVIC_InitStruct;
		/* Start timer */
		TIM_Cmd(TIM2, DISABLE);
		/* Enable interrupt each update cycle */
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		/* Set NVIC parameters */
		NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0xF;
		NVIC_Init(&NVIC_InitStruct);
		/* Start timer */
		TIM_Cmd(TIM2, ENABLE);
}

void CheckHome(void)
{
		if( AdcValues[1] > 600 )
		{
				LVQ_SSD1306_Fill(0);
				LVQ_SSD1306_DrawRectangle(0, 0, 128, 64, 1);
				LVQ_SSD1306_GotoXY(13, 20);
				LVQ_SSD1306_Puts("Return Joystick", &Font_7x10, 1);
				LVQ_SSD1306_GotoXY(40, 35);
				LVQ_SSD1306_Puts("To Home", &Font_7x10, 1);
				LVQ_SSD1306_GotoXY(40, 45);
				LVQ_SSD1306_Puts(" . . . ", &Font_7x10, 1);
				LVQ_SSD1306_UpdateScreen(); //display
				while(1)
				{
						Buzzer(2, 100);
						Delay_ms_systick(5000);
				}
		}
}
