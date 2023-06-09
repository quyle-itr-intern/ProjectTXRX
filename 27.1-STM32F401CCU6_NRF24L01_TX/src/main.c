#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_delay.h"
#include "lvq_stm32f4_gpio.h"
#include "lvq_stm32f4_usart.h"
#include "lvq_stm32f4_spi.h"
#include "lvq_stm32f4_adc.h"
#include "lvq_stm32f4_adc_dma.h"
#include "lvq_stm32f4_dac.h"
#include "lvq_stm32f4_i2c.h"
#include "lvq_stm32f4_lcd.h"
#include "lvq_stm32f4_ds1307.h"
#include "lvq_stm32f4_exti.h"
#include "lvq_stm32f4_rtc.h"
#include "lvq_stm32f4_pwm.h"
#include "lvq_stm32f4_keypad.h"
#include "lvq_stm32f4_usart_dma.h"
#include "lvq_stm32f4_mpu6050.h"
#include "lvq_stm32f4_pwm_dma.h"
#include "lvq_stm32f4_buttons.h"
#include "lvq_stm32f4_crc.h"
#include "lvq_stm32f4_gps.h"
#include "lvq_stm32f4_ssd1306.h"
#include "lvq_stm32f4_nrf24.h"
#include "lvq_stm32f4_uitx.h"
#include "String.h"

#define ADC_CHANNEL 7

volatile uint16_t AdcValues[ADC_CHANNEL];

LVQ_NRF24_Config_t nrf24 = {
			.NRF24_SPI = SPI1,
			.NRF24_SPI_PIN = LVQ_SPI_PinsPack_2,
			.NRF24_CE_PORT = GPIOB,
			.NRF24_CE_PIN = GPIO_PIN_0,
			.NRF24_CSN_PORT = GPIOB,
			.NRF24_CSN_PIN = GPIO_PIN_1,
			.NRF24_Channel = 10,
			.NRF24_TX_Power = LVQ_NRF24_TXPWR_18dBm,
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

uint8_t payload_length;

#define MODE_RX 1

uint16_t TX_PayLoad[16];

uint32_t ADC_AVR[6];

volatile uint8_t value = 0;

int main(void)
{
		Delay_Init();
	
		LVQ_GPIO_Init(GPIOC, GPIO_PIN_13, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High);	
	
		LVQ_GPIO_Init(GPIOB, GPIO_PIN_15, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High);	
		LVQ_GPIO_Init(GPIOB, GPIO_PIN_14, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High);	
		LVQ_GPIO_Init(GPIOB, GPIO_PIN_13, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High);	
		LVQ_GPIO_Init(GPIOB, GPIO_PIN_12, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_High);	
	
		LVQ_ADC_DMA_Init(ADC1, GPIOA, GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, ADC_CHANNEL, (uint32_t ) &ADC1->DR, (uint32_t) AdcValues);
	
		LVQ_SSD1306_Init();
	
		LVQ_UITX_Loading();
		
		LVQ_UITX_Display();
	
		LVQ_UITX_EndPoint();
	
		LVQ_NRF24_Init(nrf24);
	
		if( !LVQ_NRF24_Check(nrf24) )
		{
			while(1)
			{
					LVQ_GPIO_SetPinHigh(GPIOC, GPIO_PIN_13);
					Delay_ms_systick(100);
					LVQ_GPIO_SetPinLow(GPIOC, GPIO_PIN_13);
					Delay_ms_systick(100);
			}
		}
	
		LVQ_NRF24_StartTXMode(&nrf24);
		
		payload_length = 32;
	
		#ifdef MODE_RX
				LVQ_NRF24_PowerUpTx(nrf24);
		#endif

		//LVQ_NRF24_RXSearchChannel(nrf24);
						
		while (1) 
		{
				for( uint8_t i = 0; i < 16; i++)
				{
						if( i == 2 )
								TX_PayLoad[2] = (uint16_t) map(AdcValues[2], 390, 3630, 900, 2500);
						else
								TX_PayLoad[i] = 2500;
				}

				LVQ_NRF24_TransmitPacket(nrf24, (uint8_t *) TX_PayLoad, payload_length);

				LVQ_GPIO_SetPinHigh(GPIOC, GPIO_PIN_13);
				Delay_ms_systick(20);
				LVQ_GPIO_SetPinLow(GPIOC, GPIO_PIN_13);
				Delay_ms_systick(20);
		}
}

/*
				ADC_AVR[0] = ADC_AVR[1] = ADC_AVR[2] = ADC_AVR[3] = ADC_AVR[4] = ADC_AVR[5] = 0;
				for( uint8_t i = 0; i < 5; i++)
				{
							for( uint8_t j = 0; j < 6; j++)
							{
									ADC_AVR[j] += AdcValues[j];
									Delay_us_systick(20);
							}
				}
				ADC_AVR[0] /= 5;
				ADC_AVR[1] /= 5;
				ADC_AVR[2] /= 5;
				ADC_AVR[3] /= 5;
				ADC_AVR[4] /= 5;
				ADC_AVR[5] /= 5;
			
				TX_PayLoad[0] = (uint8_t) map(ADC_AVR[0], 390, 3630, 0, 100);
				TX_PayLoad[1] =	(uint8_t) map(ADC_AVR[1], 410, 3090, 0, 100);
				TX_PayLoad[2] =	(uint8_t) map(ADC_AVR[2], 410, 3580, 0, 100);
				TX_PayLoad[3] =	(uint8_t) map(ADC_AVR[3], 370, 3590, 0, 100);
				TX_PayLoad[4] =	(uint8_t) map(ADC_AVR[4], 370, 3590, 0, 100);
				TX_PayLoad[5] =	(uint8_t) map(ADC_AVR[5], 370, 3590, 0, 100);
				
				for( uint8_t i = 0; i < 16; i++)
						TX_PayLoad[i] = 900;

				LVQ_NRF24_TransmitPacket(nrf24, (uint8_t *) TX_PayLoad, payload_length);

				Delay_ms_systick(1000);
				LVQ_GPIO_SetPinHigh(GPIOC, GPIO_PIN_13);
				Delay_ms_systick(25);
				LVQ_GPIO_SetPinLow(GPIOC, GPIO_PIN_13);
				Delay_ms_systick(25);

				for( uint8_t i = 0; i < 16; i++)
						TX_PayLoad[i] = 1700;

				LVQ_NRF24_TransmitPacket(nrf24, (uint8_t *) TX_PayLoad, payload_length);

				Delay_ms_systick(1000);
				LVQ_GPIO_SetPinHigh(GPIOC, GPIO_PIN_13);
				Delay_ms_systick(25);
				LVQ_GPIO_SetPinLow(GPIOC, GPIO_PIN_13);
				Delay_ms_systick(25);

				for( uint8_t i = 0; i < 16; i++)
						TX_PayLoad[i] = 2500;

				LVQ_NRF24_TransmitPacket(nrf24, (uint8_t *) TX_PayLoad, payload_length);

				Delay_ms_systick(1000);
				LVQ_GPIO_SetPinHigh(GPIOC, GPIO_PIN_13);
				Delay_ms_systick(25);
				LVQ_GPIO_SetPinLow(GPIOC, GPIO_PIN_13);
				Delay_ms_systick(25);
*/
