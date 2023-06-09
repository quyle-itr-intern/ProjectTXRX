#include "lvq_stm32f4_gpio.h"

/* Private function */
static uint16_t GPIO_UsedPins[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Private functions */
uint8_t LVQ_GPIO_EnableClock(GPIO_TypeDef* GPIOx);
uint8_t LVQ_GPIO_DisableClock(GPIO_TypeDef* GPIOx);
void LVQ_GPIO_Config(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, LVQ_GPIO_Mode_t GPIO_Mode, LVQ_GPIO_OType_t GPIO_OType, LVQ_GPIO_PuPd_t GPIO_PuPd, LVQ_GPIO_Speed_t GPIO_Speed);
void LVQ_GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx);

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
  /* Check input */
  /* Enable clock for GPIO */
	if(LVQ_GPIO_EnableClock(GPIOx) == 0) return;

	uint32_t pinpos;
	for (pinpos = 0; pinpos < 0x10; pinpos++) 
	{
        /* Check pin */
        if ((GPIO_Pin & (1 << pinpos)) == 0) {
            continue;
        }
        /* Set alternate function */
        GPIOx->AFR[pinpos >> 0x03] = (GPIOx->AFR[pinpos >> 0x03] & ~(0x0F << (4 * (pinpos & 0x07)))) | (Alternate << (4 * (pinpos & 0x07)));
  }
  /* Do initialization */
	LVQ_GPIO_Config(GPIOx, GPIO_Pin, LVQ_GPIO_Mode_AF, GPIO_OType, GPIO_PuPd, GPIO_Speed);
}

uint8_t LVQ_GPIO_EnableClock(GPIO_TypeDef* GPIOx)
{
	if(GPIOA == GPIOx) LVQ_GPIO_INT_EnableClock(GPIOA);
	else if(GPIOB == GPIOx) LVQ_GPIO_INT_EnableClock(GPIOB);
	else if(GPIOC == GPIOx) LVQ_GPIO_INT_EnableClock(GPIOC);
	else if(GPIOD == GPIOx) LVQ_GPIO_INT_EnableClock(GPIOD);
	else if(GPIOE == GPIOx) LVQ_GPIO_INT_EnableClock(GPIOE);
	else if(GPIOF == GPIOx) LVQ_GPIO_INT_EnableClock(GPIOF);
	else if(GPIOG == GPIOx) LVQ_GPIO_INT_EnableClock(GPIOG);
	else if(GPIOH == GPIOx) LVQ_GPIO_INT_EnableClock(GPIOH);
	else return 0;
	return 1;
}

void LVQ_GPIO_Config(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, LVQ_GPIO_Mode_t GPIO_Mode, LVQ_GPIO_OType_t GPIO_OType, LVQ_GPIO_PuPd_t GPIO_PuPd, LVQ_GPIO_Speed_t GPIO_Speed)
{
	uint8_t pinpos;
  uint8_t ptr = LVQ_GPIO_GetPortSource(GPIOx);

  /* Go through all pins */
  for (pinpos = 0; pinpos < 0x10; pinpos++) 
	{
      /* Check if pin available */
      if ((GPIO_Pin & (1 << pinpos)) == 0) {
            continue;
      }

      /* Pin is used */
      GPIO_UsedPins[ptr] |= 1 << pinpos;

      /* Set GPIO PUPD register */
      GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(GPIO_PuPd << (2 * pinpos)));

      /* Set GPIO MODE register */
      GPIOx->MODER = (GPIOx->MODER & ~((uint32_t)(0x03 << (2 * pinpos)))) | ((uint32_t)(GPIO_Mode << (2 * pinpos)));

      /* Set only if output or alternate functions */
      if (GPIO_Mode == LVQ_GPIO_Mode_OUT || GPIO_Mode == LVQ_GPIO_Mode_AF) 
			{
          /* Set GPIO OTYPE register */
          GPIOx->OTYPER = (GPIOx->OTYPER & ~(uint16_t)(0x01 << pinpos)) | ((uint16_t)(GPIO_OType << pinpos));

          /* Set GPIO OSPEED register */
          GPIOx->OSPEEDR = (GPIOx->OSPEEDR & ~((uint32_t)(0x03 << (2 * pinpos)))) | ((uint32_t)(GPIO_Speed << (2 * pinpos)));
       }
   }
}

void LVQ_GPIO_DeInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) 
{
    uint8_t i;
    uint8_t ptr = LVQ_GPIO_GetPortSource(GPIOx);

    /* Go through all pins */
    for (i = 0x00; i < 0x10; i++) {
        /* Pin is set */
        if (GPIO_Pin & (1 << i)) {
            /* Set 11 bits combination for analog mode */
            GPIOx->MODER |= (0x03 << (2 * i));

            /* Pin is not used */
            GPIO_UsedPins[ptr] &= ~(1 << i);
        }
    }
}

void LVQ_GPIO_SetPinAsInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)       // 00: Input(reset state)
{
    uint8_t i;
    /* Go through all pins */
    for (i = 0x00; i < 0x10; i++) {
        /* Pin is set */
        if (GPIO_Pin & (1 << i)) {
            /* Set 00 bits combination for input */
            GPIOx->MODER &= ~(0x03 << (2 * i));
        }
    }
}

void LVQ_GPIO_SetPinAsOutput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)      // 01: General purpose output mode
{
    uint8_t i;
    /* Go through all pins */
    for (i = 0x00; i < 0x10; i++) {
        /* Pin is set */
        if (GPIO_Pin & (1 << i)) {
            /* Set 01 bits combination for output */
            GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x01 << (2 * i));
        }
    }
}

void LVQ_GPIO_SetPinAsAnalog(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)      // 11: Analog mode
{
    uint8_t i;
    /* Go through all pins */
    for (i = 0x00; i < 0x10; i++) {
        /* Pin is set */
        if (GPIO_Pin & (1 << i)) {
            /* Set 11 bits combination for analog mode */
            GPIOx->MODER |= (0x03 << (2 * i));
        }
    }
}

void LVQ_GPIO_SetPinAsAlternate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)   // 10: Alternate function mode
{
    uint8_t i;
    /* Set alternate functions for all pins */
    for (i = 0; i < 0x10; i++) {
        /* Check pin */
        if ((GPIO_Pin & (1 << i)) == 0) {
            continue;
        }
        /* Set alternate mode */
        GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x02 << (2 * i));
    }
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
    uint8_t pinpos;

    /* Go through all pins */
    for (pinpos = 0; pinpos < 0x10; pinpos++) {
        /* Check if pin available */
        if ((GPIO_Pin & (1 << pinpos)) == 0) {
            continue;
        }
        /* Set GPIO PUPD register */
        GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(GPIO_PuPd << (2 * pinpos)));
    }
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
    return ((uint32_t)GPIOx - (GPIOA_BASE)) / ((GPIOB_BASE) - (GPIOA_BASE));
}

/* Private functions */
void LVQ_GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx) {
    /* Set bit according to the 1 << portsourcenumber */
    RCC->AHB1ENR |= (1 << LVQ_GPIO_GetPortSource(GPIOx));
}

void LVQ_GPIO_INT_DisableClock(GPIO_TypeDef* GPIOx) {
    /* Clear bit according to the 1 << portsourcenumber */
    RCC->AHB1ENR &= ~(1 << LVQ_GPIO_GetPortSource(GPIOx));
}

uint16_t LVQ_GPIO_GetUsedPins(GPIO_TypeDef* GPIOx) {
    /* Return used */
    return GPIO_UsedPins[LVQ_GPIO_GetPortSource(GPIOx)];
}

uint16_t LVQ_GPIO_GetFreePins(GPIO_TypeDef* GPIOx) {
    /* Return free pins */
    return ~GPIO_UsedPins[LVQ_GPIO_GetPortSource(GPIOx)];
}
