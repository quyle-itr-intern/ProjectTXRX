/**
  ******************************************************************************

  I2C DMA For STM32F446RE
  Author:   LVQ
  Updated:  22st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_i2c_dma.h"

/* Private structure */
typedef struct {
    uint32_t DMA_Channel;
    DMA_Stream_TypeDef* DMA_Stream;
} LVQ_I2C_DMA_INT_t;

/* Create variables if necessary */
#ifdef USE_I2C1
static LVQ_I2C_DMA_INT_t I2C1_DMA_TX_INT = {I2C1_DMA_TX_CHANNEL, I2C1_DMA_TX_STREAM};
static LVQ_I2C_DMA_INT_t I2C1_DMA_RX_INT = {I2C1_DMA_RX_CHANNEL, I2C1_DMA_RX_STREAM};
#endif
#ifdef USE_I2C2
static LVQ_I2C_DMA_INT_t I2C2_DMA_TX_INT = {I2C2_DMA_TX_CHANNEL, I2C2_DMA_TX_STREAM};
static LVQ_I2C_DMA_INT_t I2C2_DMA_RX_INT = {I2C2_DMA_RX_CHANNEL, I2C2_DMA_RX_STREAM};
#endif
#ifdef USE_I2C3
static LVQ_I2C_DMA_INT_t I2C3_DMA_TX_INT = {I2C3_DMA_TX_CHANNEL, I2C3_DMA_TX_STREAM};
static LVQ_I2C_DMA_INT_t I2C3_DMA_RX_INT = {I2C3_DMA_RX_CHANNEL, I2C3_DMA_RX_STREAM};
#endif

/* Private DMA structure */
static uint32_t LVQ_I2C_Timeout;

static uint8_t TX_OR_RX;

/* Private functions */
static LVQ_I2C_DMA_INT_t* LVQ_I2C_DMA_TX_INT_GetSettings(I2C_TypeDef* I2Cx);
static LVQ_I2C_DMA_INT_t* LVQ_I2C_DMA_RX_INT_GetSettings(I2C_TypeDef* I2Cx);

void LVQ_I2C_DMA_TX_Init(I2C_TypeDef* I2Cx) 
{
    /* Init DMA TX mode */
    /* Assuming I2C is already initialized and clock is enabled */

    /* Get I2C settings */
    LVQ_I2C_DMA_INT_t* I2C_Settings = LVQ_I2C_DMA_TX_INT_GetSettings(I2Cx);

    /* Enable DMA clock */
    if (I2C_Settings->DMA_Stream >= DMA2_Stream0) 
		{
        /* Enable DMA2 clock */
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    } else {
        /* Enable DMA1 clock */
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    }
		
		/* Enable I2C TX/RX DMA */
    I2Cx->CR2 |= I2C_CR2_DMAEN;

    /* Clear flags */
    LVQ_DMA_ClearFlags(I2C_Settings->DMA_Stream);

    /* Set DMA options */
		/* Bits 7:6 DIR[1:0]: data transfer direction */
		I2C_Settings->DMA_Stream->CR |= DMA_DIR_MemoryToPeripheral;
		/* Bit 9 PINC: peripheral increment mode */
		I2C_Settings->DMA_Stream->CR |= DMA_PeripheralInc_Disable;
		/* Bit 10 MINC: memory increment mode */
		I2C_Settings->DMA_Stream->CR |= DMA_MemoryInc_Enable;
		/* Bits 12:11 PSIZE[1:0]: peripheral data size */
		I2C_Settings->DMA_Stream->CR |= DMA_PeripheralDataSize_Byte;
		/* Bits 14:13 MSIZE[1:0]: memory data size */
		I2C_Settings->DMA_Stream->CR |= DMA_MemoryDataSize_Byte;
		/* Bit 8 CIRC: circular mode */
		I2C_Settings->DMA_Stream->CR |= DMA_Mode_Normal;
		/* Bits 17:16 PL[1:0]: priority level */ 
		I2C_Settings->DMA_Stream->CR |= DMA_Priority_Medium;
		/* Bits 22:21 PBURST[1:0]: peripheral burst transfer configuration */
		I2C_Settings->DMA_Stream->CR |= DMA_PeripheralBurst_Single;
		/* Bits 24:23 MBURST[1:0]: memory burst transfer configuration */
		I2C_Settings->DMA_Stream->CR |= DMA_MemoryBurst_Single;
		
		/* Bits 1:0 FTH[1:0]: FIFO threshold selection */
		I2C_Settings->DMA_Stream->FCR |= DMA_FIFOThreshold_Full;
		/* Bit 2 DMDIS: direct mode disable */
		I2C_Settings->DMA_Stream->FCR |= DMA_FIFOMode_Disable;
		
		/* Init first DMA */
		
		/* Enable interrupt DMA */
		LVQ_DMA_EnableInterrupts(I2C_Settings->DMA_Stream);
		
		free(I2C_Settings);
}

void LVQ_I2C_DMA_RX_Init(I2C_TypeDef* I2Cx) 
{
    /* Init DMA TX mode */
    /* Assuming I2C is already initialized and clock is enabled */

    /* Get I2C settings */
    LVQ_I2C_DMA_INT_t* I2C_Settings = LVQ_I2C_DMA_RX_INT_GetSettings(I2Cx);

    /* Enable DMA clock */
    if (I2C_Settings->DMA_Stream >= DMA2_Stream0) 
		{
        /* Enable DMA2 clock */
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    } else {
        /* Enable DMA1 clock */
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    }
		
		/* Enable I2C TX/RX DMA */
    I2Cx->CR2 |= I2C_CR2_DMAEN;

    /* Clear flags */
    LVQ_DMA_ClearFlags(I2C_Settings->DMA_Stream);

    /* Set DMA options */
		/* Bits 7:6 DIR[1:0]: data transfer direction */
		I2C_Settings->DMA_Stream->CR |= DMA_DIR_PeripheralToMemory;
		/* Bit 9 PINC: peripheral increment mode */
		I2C_Settings->DMA_Stream->CR |= DMA_PeripheralInc_Disable;
		/* Bit 10 MINC: memory increment mode */
		I2C_Settings->DMA_Stream->CR |= DMA_MemoryInc_Enable;
		/* Bits 12:11 PSIZE[1:0]: peripheral data size */
		I2C_Settings->DMA_Stream->CR |= DMA_PeripheralDataSize_Byte;
		/* Bits 14:13 MSIZE[1:0]: memory data size */
		I2C_Settings->DMA_Stream->CR |= DMA_MemoryDataSize_Byte;
		/* Bit 8 CIRC: circular mode */
		I2C_Settings->DMA_Stream->CR |= DMA_Mode_Normal;
		/* Bits 17:16 PL[1:0]: priority level */ 
		I2C_Settings->DMA_Stream->CR |= DMA_Priority_Medium;
		/* Bits 22:21 PBURST[1:0]: peripheral burst transfer configuration */
		I2C_Settings->DMA_Stream->CR |= DMA_PeripheralBurst_Single;
		/* Bits 24:23 MBURST[1:0]: memory burst transfer configuration */
		I2C_Settings->DMA_Stream->CR |= DMA_MemoryBurst_Single;
		
		/* Bits 1:0 FTH[1:0]: FIFO threshold selection */
		I2C_Settings->DMA_Stream->FCR |= DMA_FIFOThreshold_Full;
		/* Bit 2 DMDIS: direct mode disable */
		I2C_Settings->DMA_Stream->FCR |= DMA_FIFOMode_Disable;
		
		/* Init first DMA */
		DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
		
		/* Enable interrupt DMA */
		LVQ_DMA_EnableInterrupts(I2C_Settings->DMA_Stream);
		
		free(I2C_Settings);
}

void LVQ_I2C_DMA_TX_InitWithStreamAndChannel(I2C_TypeDef* I2Cx, DMA_Stream_TypeDef* DMA_Stream, uint32_t DMA_Channel) 
{
    /* Get I2C settings */
    LVQ_I2C_DMA_INT_t* Settings = LVQ_I2C_DMA_TX_INT_GetSettings(I2Cx);

    /* Set DMA stream and channel */
    Settings->DMA_Stream = DMA_Stream;
    Settings->DMA_Channel = DMA_Channel;

    /* Init DMA TX */
    LVQ_I2C_DMA_TX_Init(I2Cx);
	
		free(Settings);
}

void LVQ_I2C_DMA_RX_InitWithStreamAndChannel(I2C_TypeDef* I2Cx, DMA_Stream_TypeDef* DMA_Stream, uint32_t DMA_Channel) 
{
    /* Get I2C settings */
    LVQ_I2C_DMA_INT_t* Settings = LVQ_I2C_DMA_RX_INT_GetSettings(I2Cx);

    /* Set DMA stream and channel */
    Settings->DMA_Stream = DMA_Stream;
    Settings->DMA_Channel = DMA_Channel;

    /* Init DMA TX */
    LVQ_I2C_DMA_RX_Init(I2Cx);
	
		free(Settings);
}

DMA_Stream_TypeDef* LVQ_I2C_DMA_GetStream(I2C_TypeDef* I2Cx) 
{
    /* Get I2C settings */
    return LVQ_I2C_DMA_TX_INT_GetSettings(I2Cx)->DMA_Stream;
}

void LVQ_I2C_DMA_Deinit(I2C_TypeDef* I2Cx) 
{
    /* Get I2C settings */
    LVQ_I2C_DMA_INT_t* Settings = LVQ_I2C_DMA_TX_INT_GetSettings(I2Cx);

    /* Deinit DMA Stream */
    DMA_DeInit(Settings->DMA_Stream);
}

uint8_t LVQ_I2C_DMA_Send(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* DataArray, uint16_t count) 
{
    /* Get I2C settings */
    LVQ_I2C_DMA_INT_t* Settings = LVQ_I2C_DMA_TX_INT_GetSettings(I2Cx);
	
		/* Check status */
		if(TX_OR_RX == 2)
		{
				return 1;
		}
		/* Mode TX */
		TX_OR_RX = 1;

    /* Check if DMA is working now */
    if (Settings->DMA_Stream->NDTR) 
		{
        /* DMA works right now */
        return 0;
    }
		/* Disable DMA Stream */
    Settings->DMA_Stream->CR &= ~DMA_SxCR_EN;	
		
		/* Set current data number again to count for MPu6050, only possible after disabling the DMA channel */
		DMA_SetCurrDataCounter(Settings->DMA_Stream, count);
		
    /* Set DMA options */
		/* Bits 27:25 CHSEL[2:0]: channel selection */
		Settings->DMA_Stream->CR |= Settings->DMA_Channel;
		
		Settings->DMA_Stream->M0AR = (uint32_t) &DataArray[0];
		
		Settings->DMA_Stream->PAR = (uint32_t) &I2Cx->DR;
		
		Settings->DMA_Stream->NDTR = count + 1;

    /* Deinit first, clear all flags */
    LVQ_DMA_ClearFlags(Settings->DMA_Stream);
		
		/* Enable DMA NACK automatic generation */
		I2C_DMALastTransferCmd(I2Cx, ENABLE);                    //Note this one, very important
		
		LVQ_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
		
		/* Read SR2 */
		(void)I2C1->SR2;
		
		/* Enable I2C DMA */
    I2Cx->CR2 |= I2C_CR2_DMAEN;

    /* Enable DMA Stream */
    Settings->DMA_Stream->CR |= DMA_SxCR_EN;
		
    /* DMA has started */
		free(Settings);
		
    return 1;
}

uint8_t LVQ_I2C_DMA_Puts(I2C_TypeDef* I2Cx, uint8_t address, char* DataArray) 
{
    /* Call DMA Send function */
    return LVQ_I2C_DMA_Send(I2Cx, address, (uint8_t*)DataArray, strlen(DataArray));
}

uint8_t LVQ_I2C_DMA_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* DataArray, uint16_t count) 
{
    /* Get I2C settings */
    LVQ_I2C_DMA_INT_t* Settings = LVQ_I2C_DMA_RX_INT_GetSettings(I2Cx);
	
		/* Check status */
		if(TX_OR_RX == 1)
		{
				return 1;
		}
		
		/* Mode RX */
		TX_OR_RX = 2;
		
    /* Check if DMA is working now */
    if (Settings->DMA_Stream->NDTR) 
		{
        /* DMA works right now */
        return 0;
    }
		
		/* Disable DMA Stream */
    Settings->DMA_Stream->CR &= ~DMA_SxCR_EN;	
		
		/* Set current data number again to 14 for MPu6050, only possible after disabling the DMA channel */
		DMA_SetCurrDataCounter(DMA1_Stream5, count);
		
    /* Set DMA options */
		/* Bits 27:25 CHSEL[2:0]: channel selection */
		Settings->DMA_Stream->CR |= Settings->DMA_Channel;
		
		Settings->DMA_Stream->M0AR = (uint32_t) &DataArray[0];
		
		Settings->DMA_Stream->PAR = (uint32_t) &I2Cx->DR;
		
		Settings->DMA_Stream->NDTR = count;

    /* Deinit first, clear all flags */
    LVQ_DMA_ClearFlags(Settings->DMA_Stream);
		
		/* Enable DMA NACK automatic generation */
		I2C_DMALastTransferCmd(I2Cx, ENABLE);                    //Note this one, very important
		
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
		
		/* Disable ack */
		I2Cx->CR1 &= ~I2C_CR1_ACK;
		
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
		
		/* Read status register to clear ADDR flag */
    I2Cx->SR2;
		
		/* Enable the selected I2C peripheral */
    I2Cx->CR1 |= I2C_CR1_PE;
		
		/* Wait till I2C is not busy anymore */
    LVQ_I2C_Timeout = LVQ_I2C_TIMEOUT;
		while (!(I2Cx->SR1 & I2C_SR1_TXE) && LVQ_I2C_Timeout) 
		{
			LVQ_I2C_Timeout--;
		}

    /* Send I2C data */
    I2Cx->DR = reg;
		
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
		
		/* Enable ack */
		I2Cx->CR1 |= I2C_CR1_ACK;
		
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
			
		/* Read status register to clear ADDR flag */
    I2Cx->SR2;
		
		/* Start DMA to receive data from I2C */
		Settings->DMA_Stream->CR |= DMA_SxCR_EN;
		
		/* Enable I2C TX/RX DMA */
    I2Cx->CR2 |= I2C_CR2_DMAEN;

    /* DMA has started */
		free(Settings);
    return 1;
}

uint16_t LVQ_I2C_DMA_Sending(I2C_TypeDef* I2Cx) 
{
    /* Get I2C settings */
    LVQ_I2C_DMA_INT_t* Settings = LVQ_I2C_DMA_TX_INT_GetSettings(I2Cx);

    /* DMA has work to do still */
    if (Settings->DMA_Stream->NDTR) 
		{
				free(Settings);
        return 1;
    }
		
		free(Settings);
    /* Check DMA Stream register of remaining data bytes */
    return 0;/* !I2C_TXEMPTY(I2Cx); */
}

void LVQ_I2C_DMA_EnableInterrupts(I2C_TypeDef* I2Cx) 
{
    /* Get I2C settings */
    LVQ_I2C_DMA_INT_t* Settings = LVQ_I2C_DMA_TX_INT_GetSettings(I2Cx);

    /* Enable DMA interrupts */
    LVQ_DMA_EnableInterrupts(Settings->DMA_Stream);
	
		/* Get I2C settings */
		Settings = LVQ_I2C_DMA_RX_INT_GetSettings(I2Cx);
	
		/* Enable DMA interrupts */
    LVQ_DMA_EnableInterrupts(Settings->DMA_Stream);
	
		free(Settings);
}

void LVQ_I2C_DMA_DisableInterrupts(I2C_TypeDef* I2Cx) 
{
    /* Get I2C settings */
    LVQ_I2C_DMA_INT_t* Settings = LVQ_I2C_DMA_TX_INT_GetSettings(I2Cx);

    /* Disable DMA interrupts */
    LVQ_DMA_DisableInterrupts(Settings->DMA_Stream);
	
		/* Get I2C settings */
		Settings = LVQ_I2C_DMA_RX_INT_GetSettings(I2Cx);
	
		/* Enable DMA interrupts */
    LVQ_DMA_DisableInterrupts(Settings->DMA_Stream);
	
		free(Settings);
}

/* Private functions */
static LVQ_I2C_DMA_INT_t* LVQ_I2C_DMA_TX_INT_GetSettings(I2C_TypeDef* I2Cx) 
{
    LVQ_I2C_DMA_INT_t* result;
#ifdef USE_I2C1
    if (I2Cx == I2C1) 
		{
        result = &I2C1_DMA_TX_INT;
    }
#endif
#ifdef USE_I2C2
    if (I2Cx == I2C2) 
		{
        result = &I2C2_DMA_TX_INT;
    }
#endif
#ifdef USE_I2C3
    if (I2Cx == I2C3) 
		{
        result = &I2C3_DMA_TX_INT;
    }
#endif
    /* Return */
    return result;
}

/* Private functions */
static LVQ_I2C_DMA_INT_t* LVQ_I2C_DMA_RX_INT_GetSettings(I2C_TypeDef* I2Cx) 
{
    LVQ_I2C_DMA_INT_t* result;
#ifdef USE_I2C1
    if (I2Cx == I2C1) 
		{
        result = &I2C1_DMA_RX_INT;
    }
#endif
#ifdef USE_I2C2
    if (I2Cx == I2C2) 
		{
        result = &I2C2_DMA_RX_INT;
    }
#endif
#ifdef USE_I2C3
    if (I2Cx == I2C3) 
		{
        result = &I2C3_DMA_RX_INT;
    }
#endif
    /* Return */
    return result;
}

void LVQ_DMA_TransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream)
{
#ifdef USE_I2C1
		/* TX I2C1 */
		if(DMA_Stream == I2C1_DMA_TX_STREAM)
		{
				/* Disable I2C DMA */
				I2C1->CR2 &= ~I2C_CR2_DMAEN;
				/* Send I2C1 STOP Condition */
				I2C1->CR1 |= I2C_CR1_STOP;
				/* Disable DMA stream */
				DMA_Stream->CR &= ~DMA_SxCR_EN;
				/* Reset TX */
				TX_OR_RX = 0;
		}
		/* RX I2C1 */
		if(DMA_Stream == I2C1_DMA_RX_STREAM)
		{
				/* Disable I2C DMA */
				I2C1->CR2 &= ~I2C_CR2_DMAEN;
				/* Send I2C1 STOP Condition */
				I2C1->CR1 |= I2C_CR1_STOP;
				/* Disable DMA stream */
				DMA_Stream->CR &= ~DMA_SxCR_EN;
				/* Reset RX */
				TX_OR_RX = 0;
		}
#endif
#ifdef USE_I2C2
    /* TX I2C2 */
		if(DMA_Stream == I2C2_DMA_TX_STREAM)
		{
				/* Disable I2C DMA */
				I2C2->CR2 &= ~I2C_CR2_DMAEN;
				/* Send I2C2 STOP Condition */
				I2C2->CR1 |= I2C_CR1_STOP;
				/* Disable DMA stream */
				DMA_Stream->CR &= ~DMA_SxCR_EN;
				/* Reset TX */
				TX_OR_RX = 0;
		}
		/* RX I2C2 */
		if(DMA_Stream == I2C2_DMA_RX_STREAM)
		{
				/* Disable I2C DMA */
				I2C2->CR2 &= ~I2C_CR2_DMAEN;
				/* Send I2C2 STOP Condition */
				I2C2->CR1 |= I2C_CR1_STOP;
				/* Disable DMA stream */
				DMA_Stream->CR &= ~DMA_SxCR_EN;
				/* Reset RX */
				TX_OR_RX = 0;
		}
#endif
#ifdef USE_I2C3
    /* TX I2C3 */
		if(DMA_Stream == I2C3_DMA_TX_STREAM)
		{
				/* Disable I2C DMA */
				I2C3->CR2 &= ~I2C_CR2_DMAEN;
				/* Send I2C3 STOP Condition */
				I2C3->CR1 |= I2C_CR1_STOP;
				/* Disable DMA stream */
				DMA_Stream->CR &= ~DMA_SxCR_EN;
				/* Reset TX */
				TX_OR_RX = 0;
		}
		/* RX I2C3 */
		if(DMA_Stream == I2C3_DMA_RX_STREAM)
		{
				/* Disable I2C DMA */
				I2C3->CR2 &= ~I2C_CR2_DMAEN;
				/* Send I2C3 STOP Condition */
				I2C3->CR1 |= I2C_CR1_STOP;
				/* Disable DMA stream */
				DMA_Stream->CR &= ~DMA_SxCR_EN;
				/* Reset RX */
				TX_OR_RX = 0;
		}
#endif
}
