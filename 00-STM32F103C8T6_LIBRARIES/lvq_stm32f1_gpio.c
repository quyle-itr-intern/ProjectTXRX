#include "lvq_stm32f1_gpio.h"

/* Private function */
static uint16_t GPIO_UsedPins[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Private functions */
uint8_t LVQ_GPIO_EnableClock(GPIO_TypeDef* GPIOx);
uint8_t LVQ_GPIO_DisableClock(GPIO_TypeDef* GPIOx);
void LVQ_GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx);
void LVQ_GPIO_INT_DisableClock(GPIO_TypeDef* GPIOx);
void LVQ_GPIO_Config(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, LVQ_GPIO_Mode_t GPIO_Mode, LVQ_GPIO_OType_t GPIO_OType, LVQ_GPIO_PuPd_t GPIO_PuPd, LVQ_GPIO_Speed_t GPIO_Speed);

void LVQ_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, LVQ_GPIO_Mode_t GPIO_Mode, LVQ_GPIO_OType_t GPIO_OType, LVQ_GPIO_PuPd_t GPIO_PuPd, LVQ_GPIO_Speed_t GPIO_Speed) 
{
  /* Check input */
  /* Enable clock for GPIO */
	if(LVQ_GPIO_EnableClock(GPIOx) == 0) return;

  /* Do initialization */
  LVQ_GPIO_Config(GPIOx, GPIO_Pin, GPIO_Mode, GPIO_OType, GPIO_PuPd, GPIO_Speed);
}

void LVQ_GPIO_InitAlternate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, LVQ_GPIO_OType_t GPIO_OType, LVQ_GPIO_PuPd_t GPIO_PuPd, LVQ_GPIO_Speed_t GPIO_Speed, uint8_t Alternate) 
{

}

uint8_t LVQ_GPIO_EnableClock(GPIO_TypeDef* GPIOx)
{
	if(GPIOA == GPIOx) 			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if(GPIOB == GPIOx) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(GPIOC == GPIOx) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(GPIOD == GPIOx) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(GPIOE == GPIOx) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(GPIOF == GPIOx) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(GPIOG == GPIOx) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	else return 0;
	return 1;
}

void LVQ_GPIO_Config(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, LVQ_GPIO_Mode_t GPIO_Mode, LVQ_GPIO_OType_t GPIO_OType, LVQ_GPIO_PuPd_t GPIO_PuPd, LVQ_GPIO_Speed_t GPIO_Speed)
{
	GPIO_InitTypeDef GPIO;
	/* GPIO Pin*/
	GPIO.GPIO_Pin = GPIO_Pin;
	
	/* GPIO Mode */
	if(GPIO_Mode == LVQ_GPIO_Mode_IN)
	{ 
			if( GPIO_PuPd == LVQ_GPIO_PuPd_UP )
				GPIO.GPIO_Mode = GPIO_Mode_IPU;
			else if( GPIO_PuPd == LVQ_GPIO_PuPd_DOWN )
				GPIO.GPIO_Mode = GPIO_Mode_IPD;
			else if( GPIO_PuPd == LVQ_GPIO_PuPd_NOPULL )
				GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}
	else if(GPIO_Mode == LVQ_GPIO_Mode_OUT) 
	{
			/* GPIO Otype */
			if(GPIO_OType == LVQ_GPIO_OType_PP) GPIO.GPIO_Mode = GPIO_Mode_Out_PP;
			else if(GPIO_OType == LVQ_GPIO_OType_OD) GPIO.GPIO_Mode = GPIO_Mode_Out_OD;
			
			/* GPIO Speed*/
			if(GPIO_Speed == LVQ_GPIO_Speed_Fast) GPIO.GPIO_Speed = GPIO_Speed_50MHz;
			else if(GPIO_Speed == LVQ_GPIO_Speed_Medium) GPIO.GPIO_Speed = GPIO_Speed_10MHz;
			else if(GPIO_Speed == LVQ_GPIO_Speed_Low) GPIO.GPIO_Speed = GPIO_Speed_2MHz;
	}
	else if(GPIO_Mode == LVQ_GPIO_Mode_AF) 
	{
			/* GPIO Otype */
			if(GPIO_OType == LVQ_GPIO_OType_PP) GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
			else if(GPIO_OType == LVQ_GPIO_OType_OD) GPIO.GPIO_Mode = GPIO_Mode_AF_OD;
		
			/* GPIO Speed*/
			if(GPIO_Speed == LVQ_GPIO_Speed_Fast) GPIO.GPIO_Speed = GPIO_Speed_50MHz;
			else if(GPIO_Speed == LVQ_GPIO_Speed_Medium) GPIO.GPIO_Speed = GPIO_Speed_10MHz;
			else if(GPIO_Speed == LVQ_GPIO_Speed_Low) GPIO.GPIO_Speed = GPIO_Speed_2MHz;
	}
	else if(GPIO_Mode == LVQ_GPIO_Mode_AN) GPIO.GPIO_Mode = GPIO_Mode_AIN;

	GPIO_Init(GPIOx, &GPIO);
}

void LVQ_GPIO_SetPinAsInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)       // 00: Input(reset state)
{

}

void LVQ_GPIO_SetPinAsOutput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)      // 01: General purpose output mode
{

}

void LVQ_GPIO_SetPinAsAnalog(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)      // 11: Analog mode
{

}

void LVQ_GPIO_SetPinAsAlternate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)   // 10: Alternate function mode
{

}

/**
 * @note
 *   00: No pull-up, pull-down
 *	 01: Pull-up
 *	 10: Pull-down
 *	 11: Reserved
 */
void LVQ_GPIO_SetPullResistor(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, LVQ_GPIO_PuPd_t GPIO_PuPd) 
{

}

void LVQ_GPIO_Lock(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    uint32_t d;

    /* Set GPIO pin with 16th bit set to 1 */
    d = 0x00010000 | GPIO_Pin;

    /* Write to LCKR register */
    GPIOx->LCKR = d;
    GPIOx->LCKR = GPIO_Pin;
    GPIOx->LCKR = d;

    /* Read twice */
    (void)GPIOx->LCKR;
    (void)GPIOx->LCKR;
}

uint16_t LVQ_GPIO_GetPinSource(uint16_t GPIO_Pin) {
    uint16_t pinsource = 0;

    /* Get pinsource */
    while (GPIO_Pin > 1) {
        /* Increase pinsource */
        pinsource++;
        /* Shift right */
        GPIO_Pin >>= 1;
    }

    /* Return source */
    return pinsource;
}

uint16_t LVQ_GPIO_GetPortSource(GPIO_TypeDef* GPIOx) {
    /* Get port source number */
    /* Offset from GPIOA                       Difference between 2 GPIO addresses */
    return (uint16_t)  (GPIOx - (GPIOA_BASE)) / ( (GPIOB_BASE) - (GPIOA_BASE) );
}

/* Private functions */
void LVQ_GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx) {
    /* Set bit according to the 1 << portsourcenumber */
    RCC->APB2ENR |= (1 << LVQ_GPIO_GetPortSource(GPIOx));
}

void LVQ_GPIO_INT_DisableClock(GPIO_TypeDef* GPIOx) {
    /* Clear bit according to the 1 << portsourcenumber */
    RCC->APB2ENR &= ~(1 << LVQ_GPIO_GetPortSource(GPIOx));
}

uint16_t LVQ_GPIO_GetUsedPins(GPIO_TypeDef* GPIOx) {
    /* Return used */
    return GPIO_UsedPins[LVQ_GPIO_GetPortSource(GPIOx)];
}

uint16_t LVQ_GPIO_GetFreePins(GPIO_TypeDef* GPIOx) {
    /* Return free pins */
    return ~GPIO_UsedPins[LVQ_GPIO_GetPortSource(GPIOx)];
}

