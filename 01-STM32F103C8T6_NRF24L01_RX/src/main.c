#include "lvq_stm32f1_rccConfig.h"
#include "lvq_stm32f1_gpio.h"
#include "lvq_stm32f1_nrf24.h"
#include "lvq_stm32f1_soft_spi.h"
#include "stdio.h"
#include "string.h"
#include "delay_systick.h"
#include "usart.h"
#include "pwm.h"
#include "flash.h"

void SystemInitDefine(void);
void Delay_1_Ms(void);
void Delay_Ms(uint32_t u32Delay);
void LVQ_UpdatePWM(void);

#define MAX_PULSE 	2500
#define MID_PULSE	 	1700
#define MIN_PULSE 	900
#define MAX_TIMEOUT 2000

static LVQ_NRF24_Config_t nrf24 = {
			.NRF24_SPI = SPI1,
			.NRF24_SPI_PIN = LVQ_SPI_PinsPack_1,
			.NRF24_CE_PORT = GPIOB,
			.NRF24_CE_PIN = GPIO_PIN_13,
			.NRF24_CSN_PORT = GPIOB,
			.NRF24_CSN_PIN = GPIO_PIN_12,
			.NRF24_Channel = 10,
			.NRF24_TX_Power = LVQ_NRF24_TXPWR_18dBm,
			.NRF24_DataRate = LVQ_NRF24_DR_250kbps,
			.NRF24_SizePayLoad = 32,
			.NRF24_AddressWidth = 5,
	
			.NRF24_RX_Address[0] = 0x7E,
			.NRF24_RX_Address[1] = 0x2C,
			.NRF24_RX_Address[2] = 0xE3,
			.NRF24_RX_Address[3] = 0xE3,
			.NRF24_RX_Address[4] = 0xE3,
	
			.NRF24_TX_Address[0] = 0x7F,
			.NRF24_TX_Address[1] = 0x3C,
			.NRF24_TX_Address[2] = 0xE3,
			.NRF24_TX_Address[3] = 0xE3,
			.NRF24_TX_Address[4] = 0xE3
};

static uint8_t LVQ_NRF24_payload[32];
static uint8_t LVQ_NRF24_TXIF[6] = { 0 };
static uint16_t u16OutPWM[16] = { [0 ... 15] = 1500};
static uint8_t payload_length;
static uint8_t pipe = 0;
static uint8_t u8Channel = 0xFF;

#define PIN_TEST GPIO_PIN_13
#define PORT_TEST GPIOC

void PWMTimer1ForControlMotor(void);

int main(void)
{
	SystemInitDefine();
  Delay_Init();		
	
	LVQ_GPIO_Init(PORT_TEST, PIN_TEST, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_Fast);
	LVQ_GPIO_Init(GPIOA, GPIO_PIN_4, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Fast);
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);	
	PWM_Init(TIM1, PWM_Channel_1, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	PWM_Init(TIM1, PWM_Channel_2, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	PWM_Init(TIM1, PWM_Channel_3, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	PWM_Init(TIM1, PWM_Channel_4, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	
	PWM_Init(TIM2, PWM_Channel_1, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	PWM_Init(TIM2, PWM_Channel_2, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	PWM_Init(TIM2, PWM_Channel_3, PWM_PinsPack_2, 20000 - 1, 72 - 1);
	PWM_Init(TIM2, PWM_Channel_4, PWM_PinsPack_2, 20000 - 1, 72 - 1);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	PWM_Init(TIM3, PWM_Channel_1, PWM_PinsPack_2, 20000 - 1, 72 - 1);
	PWM_Init(TIM3, PWM_Channel_2, PWM_PinsPack_2, 20000 - 1, 72 - 1);
	PWM_Init(TIM3, PWM_Channel_3, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	PWM_Init(TIM3, PWM_Channel_4, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	
	PWM_Init(TIM4, PWM_Channel_1, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	PWM_Init(TIM4, PWM_Channel_2, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	PWM_Init(TIM4, PWM_Channel_3, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	PWM_Init(TIM4, PWM_Channel_4, PWM_PinsPack_1, 20000 - 1, 72 - 1);
	
	PWM_Set(TIM1, PWM_Channel_1, 1000);
	PWM_Set(TIM1, PWM_Channel_2, 1400);
	PWM_Set(TIM1, PWM_Channel_3, 1800);
	PWM_Set(TIM1, PWM_Channel_4, 2000);

	PWM_Set(TIM2, PWM_Channel_1, 1000);
	PWM_Set(TIM2, PWM_Channel_2, 1400);
	PWM_Set(TIM2, PWM_Channel_3, 1800);
	PWM_Set(TIM2, PWM_Channel_4, 2000);
	
	PWM_Set(TIM3, PWM_Channel_1, 1000);
	PWM_Set(TIM3, PWM_Channel_2, 1400);
	PWM_Set(TIM3, PWM_Channel_3, 1800);
	PWM_Set(TIM3, PWM_Channel_4, 2000);
	
	PWM_Set(TIM4, PWM_Channel_1, 1000);
	PWM_Set(TIM4, PWM_Channel_2, 1400);
	PWM_Set(TIM4, PWM_Channel_3, 1800);
	PWM_Set(TIM4, PWM_Channel_4, 2000);
	
	Delay_ms(2000);
	
	LVQ_NRF24_Init(nrf24);
	
	if( !LVQ_NRF24_Check(nrf24) )
	{
			while(1)
			{
					LVQ_GPIO_SetPinHigh(PORT_TEST, PIN_TEST);
					Delay_ms(1000);
					LVQ_GPIO_SetPinLow(PORT_TEST, PIN_TEST);
					Delay_ms(1000);
			}
	}
	
	Flash_Read(0x08007C00, LVQ_NRF24_TXIF, 6);
	nrf24.NRF24_Channel = LVQ_NRF24_TXIF[0];
	
	LVQ_NRF24_StartRXMode(&nrf24);
	
	if( ( LVQ_GPIO_GetInputPinValue(GPIOA, GPIO_PIN_4) ) == 0 )
	{
			LVQ_GPIO_SetPinHigh(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
			LVQ_GPIO_SetPinLow(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
			LVQ_GPIO_SetPinHigh(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
			LVQ_GPIO_SetPinLow(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
			LVQ_GPIO_SetPinHigh(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
			LVQ_GPIO_SetPinLow(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
		
			while( u8Channel == 0xFF )
			{
				u8Channel = LVQ_NRF24_TXSearchChannel(nrf24);
				LVQ_GPIO_SetPinHigh(GPIOC, GPIO_Pin_13);
				Delay_ms(100);
				LVQ_GPIO_SetPinLow(GPIOC, GPIO_Pin_13);
				Delay_ms(100);
			}
			LVQ_NRF24_TXIF[0] = u8Channel;
			Flash_Erase(0x08007C00);
			Flash_Write(0x08007C00, LVQ_NRF24_TXIF, 6);
			
			LVQ_GPIO_SetPinHigh(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
			LVQ_GPIO_SetPinLow(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
			LVQ_GPIO_SetPinHigh(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
			LVQ_GPIO_SetPinLow(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
			LVQ_GPIO_SetPinHigh(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
			LVQ_GPIO_SetPinLow(GPIOC, GPIO_Pin_13);
			Delay_ms(100);
	}
	
	Delay_ms(1000);
	
	Timer_reset();

  while(1) 
	{
			if (LVQ_NRF24_GetStatus_RXFIFO(nrf24) != LVQ_NRF24_STATUS_RXFIFO_EMPTY) 
			{
					/* Get a payload from the transceiver */
					pipe = LVQ_NRF24_ReadPayload(nrf24, LVQ_NRF24_payload, &payload_length);

					/* Clear pending IRQ flags */
					LVQ_NRF24_ClearIRQFlags(nrf24);
				
					LVQ_NRF24_HandleData(LVQ_NRF24_payload, u16OutPWM);
					LVQ_UpdatePWM();
					
					Timer_reset();
			}
			if( Timer_read_ms() > 500  )
			{
					LVQ_NRF24_ResetPLOS(nrf24);
					LVQ_GPIO_SetPinHigh(PORT_TEST, PIN_TEST);
					Delay_ms(50);
					LVQ_GPIO_SetPinLow(PORT_TEST, PIN_TEST);
					Delay_ms(50);
			}
  }
}

void LVQ_UpdatePWM(void)
{
		PWM_Set(TIM2, PWM_Channel_1, u16OutPWM[0]);
		PWM_Set(TIM2, PWM_Channel_2, u16OutPWM[1]);
		PWM_Set(TIM4, PWM_Channel_4, u16OutPWM[2]);
		PWM_Set(TIM4, PWM_Channel_3, u16OutPWM[3]);
		PWM_Set(TIM4, PWM_Channel_2, u16OutPWM[4]);
		PWM_Set(TIM4, PWM_Channel_1, u16OutPWM[5]);
		PWM_Set(TIM3, PWM_Channel_2, u16OutPWM[6]);
		PWM_Set(TIM3, PWM_Channel_1, u16OutPWM[7]);
		PWM_Set(TIM1, PWM_Channel_4, u16OutPWM[8]);
		PWM_Set(TIM1, PWM_Channel_3, u16OutPWM[9]);
		PWM_Set(TIM1, PWM_Channel_2, u16OutPWM[10]);
		PWM_Set(TIM1, PWM_Channel_1, u16OutPWM[11]);
		PWM_Set(TIM2, PWM_Channel_4, u16OutPWM[12]);
		PWM_Set(TIM2, PWM_Channel_3, u16OutPWM[13]);
		PWM_Set(TIM3, PWM_Channel_4, u16OutPWM[14]);
		PWM_Set(TIM3, PWM_Channel_3, u16OutPWM[15]);
}

void SystemInitDefine(void)
{
	RCC->CR |= (1<<16);
	while (!(RCC->CR & (1<<17)));
	
	RCC->APB1ENR |= (1<<28);
	
	FLASH->ACR |= (1<<5) | 2;
	
	RCC->CFGR |= (1<<16);
	RCC->CFGR |= (7<<18);
	
	RCC->CFGR &= ~(uint32_t) (1<<13);
	RCC->CFGR |= (4<<8);
	RCC->CFGR &= ~(uint32_t) (1<<7);
	
	RCC->CR |= (1<<24);
	while (!(RCC->CR & (1<<25)));
	
	RCC->CFGR |= (2<<0);
	while (!(RCC->CFGR & (2<<2)));
}

void Delay_1_Ms(void)
{
	SysTick->LOAD = 72000 - 1;
	SysTick->CTRL = 5;
	while(!(SysTick->CTRL & (1<<16)));
}

void Delay_Ms(uint32_t u32Delay)
{
	while(u32Delay)
	{
		Delay_1_Ms();
		--u32Delay;
	}
}
