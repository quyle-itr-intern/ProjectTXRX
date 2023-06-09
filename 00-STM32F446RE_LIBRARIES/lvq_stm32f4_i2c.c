/**
  ******************************************************************************

  I2C Setup For STM32F446RE
  Author:   LVQ
  Updated:  17st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_i2c.h"

/* Private variables */
static uint32_t LVQ_I2C_Timeout;
static uint32_t LVQ_I2C_INT_Clocks[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

/* Private functions */
static void LVQ_I2C1_INT_InitPins(LVQ_I2C_PinsPack_t pinspack);
static void LVQ_I2C2_INT_InitPins(LVQ_I2C_PinsPack_t pinspack);
static void LVQ_I2C3_INT_InitPins(LVQ_I2C_PinsPack_t pinspack);
static void LVQ_I2C_ConfigClock(I2C_TypeDef* I2Cx, uint32_t clockSpeed);

void LVQ_I2C_Init(I2C_TypeDef* I2Cx, LVQ_I2C_PinsPack_t pinspack, uint32_t clockSpeed) 
{
		/* --- BEGIN -- */
	
    if (I2Cx == I2C1) 
		{
        /* Enable clock */
				RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

        /* Enable pins */
        LVQ_I2C1_INT_InitPins(pinspack);

				/* Disable I2C first */
				I2Cx->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_PE);
			
				/* Config clock I2C */
				LVQ_I2C_ConfigClock(I2Cx, clockSpeed);
    } 
		else if (I2Cx == I2C2) 
		{
        /* Enable clock */
				RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

        /* Enable pins */
        LVQ_I2C2_INT_InitPins(pinspack);
			
				/* Disable I2C first */
				I2Cx->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_PE);

        /* Config clock I2C */
				LVQ_I2C_ConfigClock(I2Cx, clockSpeed);
    } 
		else if (I2Cx == I2C3) 
		{
        /* Enable clock */
				RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;

        /* Enable pins */
        LVQ_I2C3_INT_InitPins(pinspack);
			
				/* Disable I2C first */
				I2Cx->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_PE);

        /* Config clock I2C */
				LVQ_I2C_ConfigClock(I2Cx, clockSpeed);
    }
    /* Initialize I2C */

    /* Enable I2C */
    I2Cx->CR1 |= I2C_CR1_PE;
		
		/* --- END -- */
}

void LVQ_I2C_ConfigClock(I2C_TypeDef* I2Cx, uint32_t clockSpeed)
{
		/* --- BEGIN --- */
	
		uint8_t x = 0;
		/* Check clock, set the lowest clock your devices support on the same I2C but */
		if (I2Cx == I2C1) x = 0;
		else if (I2Cx == I2C2) x = 1;
		else if (I2Cx == I2C3) x = 2;

		if (clockSpeed < LVQ_I2C_INT_Clocks[x]) 
		{
				LVQ_I2C_INT_Clocks[x] = clockSpeed;
		}
		else return;

    /* Set values */
		/* I2Cx CR2 Configuration */
				
		/* Clear frequency FREQ[5:0] bits */
		I2Cx->CR2 &= (uint16_t)~((uint16_t)I2C_CR2_FREQ);
		/* Bits 5:0 FREQ[5:0]: Peripheral clock frequency */
		I2Cx->CR2 |= LVQ_I2C_CLOCK_FREQUENCY/1000000;
				
		/* I2Cx CCR Configuration */
				
		/* Disable the selected I2C peripheral to configure TRISE */
		I2Cx->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_PE);
				
		/* Configure speed in standard mode */
		if(LVQ_I2C_INT_Clocks[x] == LVQ_I2C_CLOCK_STANDARD) 
		{
				I2Cx->CCR |= LVQ_I2C_CLOCK_FREQUENCY/(LVQ_I2C_INT_Clocks[x]*2);
				/* Set Maximum Rise Time for standard mode */
				/* Bits 5:0 TRISE[5:0]: Maximum rise time in Fm/Sm mode (Master mode) */
				I2Cx->TRISE = LVQ_I2C_CLOCK_FREQUENCY/1000000 + 1; 
		}
		/* Configure speed in fast mode */
		/* To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency (I2C peripheral input clock) must be a multiple of 10 MHz */
		if(LVQ_I2C_INT_Clocks[x] == LVQ_I2C_CLOCK_FAST_MODE || LVQ_I2C_INT_Clocks[x] == LVQ_I2C_CLOCK_FAST_MODE_PLUS || LVQ_I2C_INT_Clocks[x] == LVQ_I2C_CLOCK_HIGH_SPEED)
		{
				uint16_t result;
				if(LVQ_I2C1_DUTY_CYCLE == I2C_DutyCycle_2)
				{
						result = LVQ_I2C_CLOCK_FREQUENCY/(LVQ_I2C_INT_Clocks[x]*3);
						/* Set DUTY bit */
			      /* ------------ */
				}
				else if(LVQ_I2C1_DUTY_CYCLE == I2C_DutyCycle_16_9)
				{
						result = LVQ_I2C_CLOCK_FREQUENCY/(LVQ_I2C_INT_Clocks[x]*25);
						/* Set DUTY bit */
						result |= I2C_DutyCycle_16_9;
				}
				/* Test if CCR value is under 0x1*/
				if ((result & I2C_CCR_CCR) == 0)
				{
					/* Set minimum allowed value */
					result |= (uint16_t)0x0001;  
				}
				/* Set speed value and set F/S bit for fast mode */
				I2Cx->CCR = (uint16_t)(result | I2C_CCR_FS);
				/* Set Maximum Rise Time for standard mode */
				/* Bits 5:0 TRISE[5:0]: Maximum rise time in Fm/Sm mode (Master mode) */
				I2Cx->TRISE = (LVQ_I2C_CLOCK_FREQUENCY * 0.3 )/10000000 + 1;
		}
		
		/* Enable the selected I2C peripheral */
		I2Cx->CR1 |= I2C_CR1_PE;
				
		/* I2Cx CR1 Configuration */
				
		/* Clear ACK, SMBTYPE and  SMBUS bits */
		I2Cx->CR1 &= 0xFBF5;
		/* Bit 1 SMBUS: SMBus mode */
		I2Cx->CR1 |= LVQ_I2C1_MODE;
		/* Bit 10 ACK: Acknowledge enable */
		I2Cx->CR1 |= LVQ_I2C1_ACK;
				
		/* I2Cx OAR1 Configuration */
		
		/* Bit 15 ADDMODE Addressing mode (slave mode) */
		I2Cx->OAR1 |= LVQ_I2C1_ACKNOWLEDGED_ADDRESS;
		/* Bits 7:1 ADD[7:1]: Interface address */
		I2Cx->OAR1 |= LVQ_I2C1_OWN_ADDRESS;
		
		/* --- END --- */
}

uint8_t LVQ_I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg) 
{
    uint8_t received_data;
    LVQ_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    LVQ_I2C_WriteData(I2Cx, reg);
    LVQ_I2C_Stop(I2Cx);
    LVQ_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
    received_data = LVQ_I2C_ReadNack(I2Cx);
    return received_data;
}

void LVQ_I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) 
{
    LVQ_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    LVQ_I2C_WriteData(I2Cx, reg);
    LVQ_I2C_Stop(I2Cx);
    LVQ_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
    while (count--) {
        if (!count) {
            /* Last byte */
            *data++ = LVQ_I2C_ReadNack(I2Cx);
        } else {
            *data++ = LVQ_I2C_ReadAck(I2Cx);
        }
    }
}

void LVQ_I2C_ReadMultiReg16(I2C_TypeDef* I2Cx, uint8_t address, uint16_t reg, uint8_t* data, uint16_t count) 
{
    LVQ_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    LVQ_I2C_WriteData(I2Cx, ( reg >> 8 ) & 0xFF );
		LVQ_I2C_WriteData(I2Cx, ( reg >> 0 ) & 0xFF );
    LVQ_I2C_Stop(I2Cx);
    LVQ_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
    while (count--) {
        if (!count) {
            /* Last byte */
            *data++ = LVQ_I2C_ReadNack(I2Cx);
        } else {
            *data++ = LVQ_I2C_ReadAck(I2Cx);
        }
    }
}

uint8_t LVQ_I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t address) 
{
    uint8_t data;
    LVQ_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
    /* Also stop condition happens */
    data = LVQ_I2C_ReadNack(I2Cx);
    return data;
}

void LVQ_I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count) 
{
    LVQ_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
    while (count--)
		{
        if (!count) {
            /* Last byte */
            *data++ = LVQ_I2C_ReadNack(I2Cx);
        } else {
            *data++ = LVQ_I2C_ReadAck(I2Cx);
        }
    }
}

void LVQ_I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data) 
{
    LVQ_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    LVQ_I2C_WriteData(I2Cx, reg);
    LVQ_I2C_WriteData(I2Cx, data);
    LVQ_I2C_Stop(I2Cx);
}

void LVQ_I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) 
{
    LVQ_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    LVQ_I2C_WriteData(I2Cx, reg);
    while (count--) {
        LVQ_I2C_WriteData(I2Cx, *data++);
    }
    LVQ_I2C_Stop(I2Cx);
}

void LVQ_I2C_WriteMultiReg16(I2C_TypeDef* I2Cx, uint8_t address, uint16_t reg, uint8_t* data, uint16_t count) 
{
    LVQ_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    LVQ_I2C_WriteData(I2Cx, ( reg >> 8 ) & 0xFF );
		LVQ_I2C_WriteData(I2Cx, ( reg >> 0 ) & 0xFF );
    while (count--) {
        LVQ_I2C_WriteData(I2Cx, *data++);
    }
    LVQ_I2C_Stop(I2Cx);
}

void LVQ_I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data) 
{
    LVQ_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    LVQ_I2C_WriteData(I2Cx, data);
    LVQ_I2C_Stop(I2Cx);
}

void LVQ_I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count) 
{
    LVQ_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    while (count--) {
        LVQ_I2C_WriteData(I2Cx, *data++);
    }
    LVQ_I2C_Stop(I2Cx);
}

uint8_t LVQ_I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t address) 
{
    uint8_t connected = 0;
    /* Try to start, function will return 0 in case device will send ACK */
    if (!LVQ_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE)) {
        connected = 1;
    }

    /* STOP I2C */
    LVQ_I2C_Stop(I2Cx);

    /* Return status */
    return connected;
}

__weak void LVQ_I2C_InitCustomPinsCallback(I2C_TypeDef* I2Cx, uint16_t AlternateFunction) {
    /* Custom user function. */
    /* In case user needs functionality for custom pins, this function should be declared outside this library */
}

/* Custom pinout initialization callback */
void TM_I2C_InitCustomPinsCallback(I2C_TypeDef* I2Cx, uint16_t AlternateFunction) {
	/* Check I2C */
	if (I2Cx == I2C1) {
		/* Init SCL and SDA pins */
		/* SCL = PB6, SDA = PB9 */
		LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_6 | GPIO_PIN_9, LVQ_GPIO_OType_OD, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Low, AlternateFunction);
	}
}

/* Private functions */
int16_t LVQ_I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack) 
{
    /* Generate I2C start pulse */
    I2Cx->CR1 |= I2C_CR1_START;

    /* Wait till I2C is busy */
    LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
    while ((!(I2Cx->SR1 & I2C_SR1_SB) && LVQ_I2C_Timeout)) 
	 {                                                               
        if (--LVQ_I2C_Timeout == 0x00) {
            return 1;
        }
    }

    /* Enable ack if we select it */
		if(ack) 
		{
			I2Cx->CR1 |= I2C_CR1_ACK;
		}
		else
		{
			I2Cx->CR1 &= ~I2C_CR1_ACK;
		}
		
		/* Send write/read bit */
		if (direction == I2C_TRANSMITTER_MODE) 
		{
			/* Send address with zero last bit */
      I2Cx->DR = address & ~I2C_OAR1_ADD0;
			
			/* Wait till finished */
			LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
			while (!(I2Cx->SR1 & I2C_SR1_ADDR) && LVQ_I2C_Timeout) 
			{
				if (--LVQ_I2C_Timeout == 0x00) 
				{
					return 1;
				}
			}
		} 
		else if (direction == I2C_RECEIVER_MODE) 
		{
			/* Send address with 1 last bit */
      I2Cx->DR = address | I2C_OAR1_ADD0;
			
			/* Wait till finished */
			LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
			
			/* Check */
			/* Bit 1 ADDR: Address sent (master mode)/matched (slave mode) == 1*/
			/* Bit 0 MSL: Master/slave == 1 */
			/* Bit 1 BUSY: Bus busy == 1*/
			while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && LVQ_I2C_Timeout) 
			{
				if (--LVQ_I2C_Timeout == 0x00)
				{
					return 1;
				}
			}
		}

    /* Read status register to clear ADDR flag */
    I2Cx->SR2;

    /* Return 0, everything ok */
    return 0;
}

void LVQ_I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data) 
{
    /* Wait till I2C is not busy anymore */
    LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
		while (!(I2Cx->SR1 & I2C_SR1_TXE) && LVQ_I2C_Timeout) 
		{
			LVQ_I2C_Timeout--;
		}

    /* Send I2C data */
    I2Cx->DR = data;
}

uint8_t LVQ_I2C_ReadAck(I2C_TypeDef* I2Cx) 
{
    uint8_t data;

    /* Enable ACK */
    I2Cx->CR1 |= I2C_CR1_ACK;

    /* Wait till not received */
    LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
		
	  /* Check */
		/* Bit 6 RxNE: Data register not empty (receivers) == 1*/
		/* Bit 0 MSL: Master/slave == 1 */
		/* Bit 1 BUSY: Bus busy == 1*/
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) && LVQ_I2C_Timeout)
		{
			LVQ_I2C_Timeout--;
		}

    /* Read data */
    data = I2Cx->DR;

    /* Return data */
    return data;
}

uint8_t LVQ_I2C_ReadNack(I2C_TypeDef* I2Cx) 
{
    uint8_t data;

    /* Disable ACK */
    I2Cx->CR1 &= ~I2C_CR1_ACK;

    /* Generate stop */
    I2Cx->CR1 |= I2C_CR1_STOP;

    /* Wait till received */
    LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
	
		/* Check */
		/* Bit 6 RxNE: Data register not empty (receivers) == 1*/
		/* Bit 0 MSL: Master/slave == 1 */
		/* Bit 1 BUSY: Bus busy == 1*/
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) && LVQ_I2C_Timeout) 
		{
				LVQ_I2C_Timeout--;
		}

    /* Read data */
    data = I2Cx->DR;

    /* Return data */
    return data;
}

uint8_t LVQ_I2C_Stop(I2C_TypeDef* I2Cx) 
{
    /* Wait till transmitter not empty */
    LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
    while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF))) && LVQ_I2C_Timeout) 
		{
			if (--LVQ_I2C_Timeout == 0x00) {
				return 1;
			}
		}

    /* Generate stop */
    I2Cx->CR1 |= I2C_CR1_STOP;

    /* Return 0, everything ok */
    return 0;
}

/* Private functions */
static void LVQ_I2C1_INT_InitPins(LVQ_I2C_PinsPack_t pinspack) 
{
    /* Init pins */
#if defined(GPIOB)
    if (pinspack == LVQ_I2C_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_6 | GPIO_PIN_7, LVQ_GPIO_OType_OD, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium, GPIO_AF_I2C1);
    }
#endif
#if defined(GPIOB)
    if (pinspack == LVQ_I2C_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, LVQ_GPIO_OType_OD, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium, GPIO_AF_I2C1);
    }
#endif
#if defined(GPIOB)
    if (pinspack == LVQ_I2C_PinsPack_3) {
        LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_6 | GPIO_PIN_9, LVQ_GPIO_OType_OD, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium, GPIO_AF_I2C1);
    }
#endif
    if (pinspack == LVQ_I2C_PinsPack_Custom) {
        /* Init custom pins, callback function */
        LVQ_I2C_InitCustomPinsCallback(I2C1, GPIO_AF_I2C1);
    }
}

static void LVQ_I2C2_INT_InitPins(LVQ_I2C_PinsPack_t pinspack) 
{
    /* Init pins */
#if defined(GPIOB)
    if (pinspack == LVQ_I2C_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOB, GPIO_PIN_10 | GPIO_PIN_11, LVQ_GPIO_OType_OD, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium, GPIO_AF_I2C2);
    }
#endif
#if defined(GPIOF)
    if (pinspack == LVQ_I2C_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOF, GPIO_PIN_0 | GPIO_PIN_1, LVQ_GPIO_OType_OD, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium, GPIO_AF_I2C2);
    }
#endif
#if defined(GPIOH)
    if (pinspack == LVQ_I2C_PinsPack_3) {
        LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_4 | GPIO_PIN_5, LVQ_GPIO_OType_OD, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium, GPIO_AF_I2C2);
    }
#endif
    if (pinspack == LVQ_I2C_PinsPack_Custom) {
        /* Init custom pins, callback function */
        LVQ_I2C_InitCustomPinsCallback(I2C2, GPIO_AF_I2C2);
    }
}

static void LVQ_I2C3_INT_InitPins(LVQ_I2C_PinsPack_t pinspack) 
{
    /* Init pins */
#if defined(GPIOA) && defined(GPIOC)
    if (pinspack == LVQ_I2C_PinsPack_1) {
        LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_8, LVQ_GPIO_OType_OD, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium, GPIO_AF_I2C3);
        LVQ_GPIO_InitAlternate(GPIOC, GPIO_PIN_9, LVQ_GPIO_OType_OD, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium, GPIO_AF_I2C3);
    }
#endif
#if defined(GPIOH)
    if (pinspack == LVQ_I2C_PinsPack_2) {
        LVQ_GPIO_InitAlternate(GPIOH, GPIO_PIN_7 | GPIO_PIN_8, LVQ_GPIO_OType_OD, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium, GPIO_AF_I2C3);
    }
#endif
    if (pinspack == LVQ_I2C_PinsPack_Custom) {
        /* Init custom pins, callback function */
        LVQ_I2C_InitCustomPinsCallback(I2C3, GPIO_AF_I2C3);
    }
}
