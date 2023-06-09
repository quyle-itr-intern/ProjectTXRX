/**
  ******************************************************************************

  I2C Setup For SLVQ32F446RE
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

void LVQ_I2C_Init(I2C_TypeDef* I2Cx, LVQ_I2C_PinsPack_t pinspack, uint32_t clockSpeed) 
{
    I2C_InitTypeDef I2C_InitStruct;

    if (I2Cx == I2C1) 
		{
        /* Enable clock */
				RCC_APB1PeriphClockCmd(RCC_APB1ENR_I2C1EN, ENABLE);

        /* Enable pins */
        LVQ_I2C1_INT_InitPins(pinspack);

        /* Check clock, set the lowest clock your devices support on the same I2C but */
        if (clockSpeed < LVQ_I2C_INT_Clocks[0]) {
            LVQ_I2C_INT_Clocks[0] = clockSpeed;
        }

        /* Set values */
        I2C_InitStruct.I2C_ClockSpeed = LVQ_I2C_INT_Clocks[0];
        I2C_InitStruct.I2C_AcknowledgedAddress = LVQ_I2C1_ACKNOWLEDGED_ADDRESS;
        I2C_InitStruct.I2C_Mode = LVQ_I2C1_MODE;
        I2C_InitStruct.I2C_OwnAddress1 = LVQ_I2C1_OWN_ADDRESS;
        I2C_InitStruct.I2C_Ack = LVQ_I2C1_ACK;
        I2C_InitStruct.I2C_DutyCycle = LVQ_I2C1_DUTY_CYCLE;
    } 
		else if (I2Cx == I2C2) 
		{
        /* Enable clock */
				RCC_APB1PeriphClockCmd(RCC_APB1ENR_I2C2EN, ENABLE);

        /* Enable pins */
        LVQ_I2C2_INT_InitPins(pinspack);

        /* Check clock, set the lowest clock your devices support on the same I2C but */
        if (clockSpeed < LVQ_I2C_INT_Clocks[1]) {
            LVQ_I2C_INT_Clocks[1] = clockSpeed;
        }

        /* Set values */
        I2C_InitStruct.I2C_ClockSpeed = LVQ_I2C_INT_Clocks[1];
        I2C_InitStruct.I2C_AcknowledgedAddress = LVQ_I2C2_ACKNOWLEDGED_ADDRESS;
        I2C_InitStruct.I2C_Mode = LVQ_I2C2_MODE;
        I2C_InitStruct.I2C_OwnAddress1 = LVQ_I2C2_OWN_ADDRESS;
        I2C_InitStruct.I2C_Ack = LVQ_I2C2_ACK;
        I2C_InitStruct.I2C_DutyCycle = LVQ_I2C2_DUTY_CYCLE;
    } 
		else if (I2Cx == I2C3) 
		{
        /* Enable clock */
        RCC_APB1PeriphClockCmd(RCC_APB1ENR_I2C3EN, ENABLE);

        /* Enable pins */
        LVQ_I2C3_INT_InitPins(pinspack);

        /* Check clock, set the lowest clock your devices support on the same I2C but */
        if (clockSpeed < LVQ_I2C_INT_Clocks[2]) {
            LVQ_I2C_INT_Clocks[2] = clockSpeed;
        }

        /* Set values */
        I2C_InitStruct.I2C_ClockSpeed = LVQ_I2C_INT_Clocks[2];
        I2C_InitStruct.I2C_AcknowledgedAddress = LVQ_I2C3_ACKNOWLEDGED_ADDRESS;
        I2C_InitStruct.I2C_Mode = LVQ_I2C3_MODE;
        I2C_InitStruct.I2C_OwnAddress1 = LVQ_I2C3_OWN_ADDRESS;
        I2C_InitStruct.I2C_Ack = LVQ_I2C3_ACK;
        I2C_InitStruct.I2C_DutyCycle = LVQ_I2C3_DUTY_CYCLE;
    }
		
    /* Disable I2C first */
    I2C_Cmd(I2Cx, DISABLE);

    /* Initialize I2C */
    I2C_Init(I2Cx, &I2C_InitStruct);

    /* Enable I2C */
    I2C_Cmd(I2Cx, ENABLE);
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
    I2C_GenerateSTART(I2Cx, ENABLE);                                   /* I2Cx->CR1 |= I2C_CR1_START; */

    /* Wait till I2C is busy */
    LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
    while ((!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) && LVQ_I2C_Timeout)) 
	 {                                                                    /* while (!(I2Cx->SR1 & I2C_SR1_SB)) */
        if (--LVQ_I2C_Timeout == 0x00) {
            return 1;
        }
    }

    /* Enable ack if we select it */
		if(ack) 
		{
			I2C_AcknowledgeConfig(I2Cx, ENABLE);              /* I2Cx->CR1 |= I2C_CR1_ACK; */
		}
		else
		{
			I2C_AcknowledgeConfig(I2Cx, DISABLE);             /* I2Cx->CR1 &= ~I2C_CR1_ACK; */ 
		}

		I2C_Send7bitAddress(I2Cx, address, direction);
		
		if (direction == I2C_TRANSMITTER_MODE) 
		{
			/* Wait till finished */
			LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
			while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) && LVQ_I2C_Timeout) 
			{
				if (--LVQ_I2C_Timeout == 0x00) 
				{
					return 1;
				}
			}
		} 
		else if (direction == I2C_RECEIVER_MODE) 
		{
			/* Wait till finished */
			LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
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
		while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) && LVQ_I2C_Timeout) 
		{
			LVQ_I2C_Timeout--;
		}

    /* Send I2C data */
    I2C_SendData(I2Cx, data);
}

uint8_t LVQ_I2C_ReadAck(I2C_TypeDef* I2Cx) 
{
    uint8_t data;

    /* Enable ACK */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);

    /* Wait till not received */
    LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) && LVQ_I2C_Timeout) 
		{
			LVQ_I2C_Timeout--;
		}

    /* Read data */
    data = I2C_ReceiveData(I2Cx);

    /* Return data */
    return data;
}

uint8_t LVQ_I2C_ReadNack(I2C_TypeDef* I2Cx) 
{
    uint8_t data;

    /* Disable ACK */
    I2C_AcknowledgeConfig(I2Cx, DISABLE);

    /* Generate stop */
    I2C_GenerateSTOP(I2Cx, ENABLE);

    /* Wait till received */
    LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) && LVQ_I2C_Timeout) 
		{
				LVQ_I2C_Timeout--;
		}

    /* Read data */
    data = I2C_ReceiveData(I2Cx);

    /* Return data */
    return data;
}

uint8_t LVQ_I2C_Stop(I2C_TypeDef* I2Cx) 
{
    /* Wait till transmitter not empty */
    LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
    while (((!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF))) && LVQ_I2C_Timeout) 
		{
			if (--LVQ_I2C_Timeout == 0x00) {
				return 1;
			}
		}

    /* Generate stop */
    I2C_GenerateSTOP(I2Cx, ENABLE);

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
