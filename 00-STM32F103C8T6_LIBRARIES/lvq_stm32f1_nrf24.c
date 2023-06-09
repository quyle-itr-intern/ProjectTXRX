/**
  ******************************************************************************

  NRF24 For STM32F446RE
  Author:   LVQ
  Updated:  21 April 2023

  ******************************************************************************
*/

#include "lvq_stm32f1_nrf24.h"

#ifdef USE_SOFT_SPI
extern LVQ_SSPI_t sspi;
#endif

static inline void LVQ_NRF24_CE_L(LVQ_NRF24_Config_t nrf24) 
{
    LVQ_GPIO_SetPinLow(nrf24.NRF24_CE_PORT, nrf24.NRF24_CE_PIN);
}

static inline void LVQ_NRF24_CE_H(LVQ_NRF24_Config_t nrf24)	
{
    LVQ_GPIO_SetPinHigh(nrf24.NRF24_CE_PORT, nrf24.NRF24_CE_PIN);
}

static inline void LVQ_NRF24_CSN_L(LVQ_NRF24_Config_t nrf24) 
{
    LVQ_GPIO_SetPinLow(nrf24.NRF24_CSN_PORT, nrf24.NRF24_CSN_PIN);
}

static inline void LVQ_NRF24_CSN_H(LVQ_NRF24_Config_t nrf24) 
{
    LVQ_GPIO_SetPinHigh(nrf24.NRF24_CSN_PORT, nrf24.NRF24_CSN_PIN);
}

static inline uint8_t LVQ_NRF24_RW(LVQ_NRF24_Config_t nrf24, uint8_t data) 
{
    /* Wait until TX buffer is empty */
    uint8_t result;
		#ifdef USE_SOFT_SPI
			result = ( uint8_t ) LVQ_SSPI_SendByte(sspi, data);
		#else
			result = ( uint8_t ) LVQ_SPI_Send(nrf24.NRF24_SPI, data);
		#endif
    return result;
}

static uint8_t LVQ_NRF24_ReadReg(LVQ_NRF24_Config_t nrf24, uint8_t reg) 
{
		uint8_t value;

		LVQ_NRF24_CSN_L(nrf24);
		LVQ_NRF24_RW(nrf24, reg & LVQ_NRF24_MASK_REG_MAP);
		value = LVQ_NRF24_RW(nrf24, LVQ_NRF24_CMD_NOP);
		LVQ_NRF24_CSN_H(nrf24);
		
		return value;
}

static void LVQ_NRF24_WriteReg(LVQ_NRF24_Config_t nrf24, uint8_t reg, uint8_t value) 
{
		LVQ_NRF24_CSN_L(nrf24);
		if (reg < LVQ_NRF24_CMD_W_REGISTER) 
		{
			/* This is a register access */
			LVQ_NRF24_RW(nrf24, LVQ_NRF24_CMD_W_REGISTER | (reg & LVQ_NRF24_MASK_REG_MAP));
			LVQ_NRF24_RW(nrf24, value);
		} else {
			/* This is a single byte command or future command/register */
			LVQ_NRF24_RW(nrf24, reg);
			if ((reg != LVQ_NRF24_CMD_FLUSH_TX) && (reg != LVQ_NRF24_CMD_FLUSH_RX) && \
					(reg != LVQ_NRF24_CMD_REUSE_TX_PL) && (reg != LVQ_NRF24_CMD_NOP)) {
				/* Send register value */
				LVQ_NRF24_RW(nrf24, value);
			}
		}
		LVQ_NRF24_CSN_H(nrf24);
}

/* Read a multi-byte register 												*/
/* input:																							*/
/*   nrf24 - information config nrf24 							  */
/*   reg - number of register to read 								*/
/*   pBuf - pointer to the buffer for register data	  */
/*   count - number of bytes to read 									*/
static void LVQ_NRF24_ReadMBReg(LVQ_NRF24_Config_t nrf24, uint8_t reg, uint8_t *pBuf, uint8_t count) 
{
		LVQ_NRF24_CSN_L(nrf24);
		LVQ_NRF24_RW(nrf24, reg);
		while (count--) {
			*pBuf++ = LVQ_NRF24_RW(nrf24, LVQ_NRF24_CMD_NOP);
		}
		LVQ_NRF24_CSN_H(nrf24);
}

/* Write a multi-byte register 												*/
/* input: 																						*/
/*   nrf24 - information config nrf24									*/
/*   reg - number of register to write								*/
/*   pBuf - pointer to the buffer with data to write 	*/
/*   count - number of bytes to write 								*/
static void LVQ_NRF24_WriteMBReg(LVQ_NRF24_Config_t nrf24, uint8_t reg, uint8_t *pBuf, uint8_t count) 
{
		LVQ_NRF24_CSN_L(nrf24);
		LVQ_NRF24_RW(nrf24, reg);
		while (count--) 
		{
			LVQ_NRF24_RW(nrf24, *pBuf++);
		}
		LVQ_NRF24_CSN_H(nrf24);
}
	
/* Set transceiver to it's initial state 					*/
/* note: RX/TX pipe addresses remains untouched 	*/
void LVQ_NRF24_Init(LVQ_NRF24_Config_t nrf24) 
{
		/* CNS pin */
		LVQ_GPIO_Init(nrf24.NRF24_CSN_PORT, nrf24.NRF24_CSN_PIN, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Fast);

		/* CE pin */
		LVQ_GPIO_Init(nrf24.NRF24_CE_PORT, nrf24.NRF24_CE_PIN, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Fast);

		/* Initialize SPI */
		LVQ_SPI_Init(nrf24.NRF24_SPI, nrf24.NRF24_SPI_PIN);
			
		/* Write to registers their initial values */
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_CONFIG,     0x08); // 0x38
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_EN_AA,      0x3F);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_EN_RXADDR,  0x3F);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_SETUP_AW,   0x03);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_SETUP_RETR, 0x03);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_RF_CH,      0x02);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_RF_SETUP,   0x0E);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_STATUS,     0x00);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_RX_PW_P0,   0x00);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_RX_PW_P1,   0x00);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_RX_PW_P2,   0x00);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_RX_PW_P3,   0x00);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_RX_PW_P4,   0x00);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_RX_PW_P5,   0x00);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_DYNPD,      0x00);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_FEATURE,    0x00);

		/* Clear the FIFO's */
		LVQ_NRF24_FlushRX(nrf24);
		LVQ_NRF24_FlushTX(nrf24);

		/* Clear any pending interrupt flags */
		LVQ_NRF24_ClearIRQFlags(nrf24);

		/* Disable CSN pin (chip release) */
		LVQ_NRF24_CSN_H(nrf24);
		
		/* Disable the CE pin */
		LVQ_NRF24_CE_L(nrf24);
}

/* Check if the LVQ_NRF24L01 present								 	*/
/* return: 																						*/
/*   nrf24 - information config nrf24 							  */
/*   1 - LVQ_NRF24L01 is online and responding 				*/
/*   0 - received sequence differs from original 			*/
uint8_t LVQ_NRF24_Check(LVQ_NRF24_Config_t nrf24) 
{
		uint8_t rxbuf[5];
		uint8_t i;
		uint8_t *ptr = (uint8_t *)LVQ_NRF24_TEST_ADDR;

		/* Write test TX address and read TX_ADDR register */
		LVQ_NRF24_WriteMBReg( nrf24, LVQ_NRF24_CMD_W_REGISTER | LVQ_NRF24_REG_TX_ADDR, ptr, 5);
		LVQ_NRF24_ReadMBReg( nrf24, LVQ_NRF24_CMD_R_REGISTER | LVQ_NRF24_REG_TX_ADDR, rxbuf, 5);

		/* Compare buffers, return error on first mismatch */
		for (i = 0; i < 5; i++) {
			if (rxbuf[i] != *ptr++) return 0;
		}

		return 1;
}

/* Control transceiver power mode 																	*/
/* input: 																													*/
/*   nrf24 - information config nrf24 														  */
/*   mode - new state of power mode, one of LVQ_NRF24_PWR_xx values */
void LVQ_NRF24_SetPowerMode(LVQ_NRF24_Config_t nrf24, uint8_t mode) 
{
		uint8_t reg;

		reg = LVQ_NRF24_ReadReg( nrf24, LVQ_NRF24_REG_CONFIG);
		if (mode == LVQ_NRF24_PWR_UP) {
			/* Set the PWR_UP bit of CONFIG register to wake the transceiver  */
			/* It goes into Stanby-I mode with consumption about 26uA         */
			reg |= LVQ_NRF24_CONFIG_PWR_UP;
		} else {
			/* Clear the PWR_UP bit of CONFIG register to put the transceiver */
			/* into power down mode with consumption about 900nA              */
			reg &= ~LVQ_NRF24_CONFIG_PWR_UP;
		}
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_CONFIG, reg);
}

/* Set transceiver operational mode 															*/
/* input: 																												*/
/*   nrf24 - information config nrf24 											  		*/
/*   mode - operational mode, one of LVQ_NRF24_MODE_xx values 		*/
void LVQ_NRF24_SetOperationalMode(LVQ_NRF24_Config_t nrf24, uint8_t mode) 
{
		uint8_t reg;

		// Configure PRIM_RX bit of the CONFIG register
		reg  = LVQ_NRF24_ReadReg( nrf24, LVQ_NRF24_REG_CONFIG);
		reg &= ~LVQ_NRF24_CONFIG_PRIM_RX;
		reg |= (mode & LVQ_NRF24_CONFIG_PRIM_RX);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_CONFIG, reg);
}

/* Set transceiver DynamicPayloadLength feature for all the pipes */
/* input: 																												*/
/*   nrf24 - information config nrf24 							  						*/
/*   mode - status, one of LVQ_NRF24_DPL_xx values 								*/
void LVQ_NRF24_SetDynamicPayloadLength(LVQ_NRF24_Config_t nrf24, uint8_t mode) 
{
		uint8_t reg;
		reg  = LVQ_NRF24_ReadReg( nrf24, LVQ_NRF24_REG_FEATURE);
		if(mode) 
		{
			LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_FEATURE, reg | LVQ_NRF24_FEATURE_EN_DPL);
			LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_DYNPD, 0x1F);
		} 
		else 
		{
			LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_FEATURE, reg &~ LVQ_NRF24_FEATURE_EN_DPL);
			LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_DYNPD, 0x0);
		}
}

/* Enables Payload With Ack. NB Refer to the datasheet for proper retransmit timing. 	*/
/* input: 																																						*/
/*   nrf24 - information config nrf24 							  																*/
/*   mode - status, 1 or 0 																														*/
void LVQ_NRF24_SetPayloadWithAck(LVQ_NRF24_Config_t nrf24, uint8_t mode) 
{
		uint8_t reg;
		reg  = LVQ_NRF24_ReadReg( nrf24, LVQ_NRF24_REG_FEATURE);
		if(mode) {
			LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_FEATURE, reg | LVQ_NRF24_FEATURE_EN_ACK_PAY);
		} else {
			LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_FEATURE, reg &~ LVQ_NRF24_FEATURE_EN_ACK_PAY);
		}
}

/* Configure transceiver CRC scheme 																								*/
/* input: 																																					*/
/*   nrf24 - information config nrf24 							  															*/
/*   scheme - CRC scheme, one of LVQ_NRF24_CRC_xx values 														*/
/* note: transceiver will forcibly turn on the CRC in case if auto acknowledgment 	*/
/*       enabled for at least one RX pipe 																					*/
void LVQ_NRF24_SetCRCScheme(LVQ_NRF24_Config_t nrf24, uint8_t scheme) 
{
		uint8_t reg;

		/* Configure EN_CRC[3] and CRCO[2] bits of the CONFIG register */
		reg  = LVQ_NRF24_ReadReg( nrf24, LVQ_NRF24_REG_CONFIG);
		reg &= ~LVQ_NRF24_MASK_CRC;
		reg |= (scheme & LVQ_NRF24_MASK_CRC);
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_CONFIG, reg);
}

/* Set frequency channel 																														*/
/* input: 																																					*/
/*   nrf24 - information config nrf24 							  															*/
/*   channel - radio frequency channel, value from 0 to 127 												*/
/* note: frequency will be (2400 + channel)MHz 																			*/
/* note: PLOS_CNT[7:4] bits of the OBSERVER_TX register will be reset 							*/
void LVQ_NRF24_SetRFChannel(LVQ_NRF24_Config_t nrf24, uint8_t channel) 
{
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_RF_CH, channel);
}

/* Set automatic retransmission parameters 																					*/
/* input: 																																					*/
/*   nrf24 - information config nrf24 							  															*/
/*   ard - auto retransmit delay, one of LVQ_NRF24_ARD_xx values 										*/
/*   arc - count of auto retransmits, value form 0 to 15 														*/
/* note: zero arc value means that the automatic retransmission disabled 						*/
void LVQ_NRF24_SetAutoRetr(LVQ_NRF24_Config_t nrf24, uint8_t ard, uint8_t arc) 
{
		/* Set auto retransmit settings (SETUP_RETR register) */
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_SETUP_RETR, (uint8_t)((ard << 4) | (arc & LVQ_NRF24_MASK_RETR_ARC)));
}

/* Set of address widths 																														*/
/* input: 																																					*/
/*   nrf24 - information config nrf24 							  															*/
/*   addr_width - RX/TX address field width, value from 3 to 5 											*/
/* note: this setting is common for all pipes 																			*/
void LVQ_NRF24_SetAddressWidth(LVQ_NRF24_Config_t nrf24, uint8_t addr_width) 
{
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_SETUP_AW, addr_width - 2);
}

/* Set static RX address for a specified pipe																				*/
/* input:																																						*/
/*   pipe - pipe to configure address, one of LVQ_NRF24_PIPEx values								*/
/*   addr - pointer to the buffer with address																			*/
/* note: pipe can be a number from 0 to 5 (RX pipes) and 6 (TX pipe)								*/
/* note: buffer length must be equal to current address width of transceiver				*/
/* note: for pipes[2..5] only first byte of address will be written because					*/
/*       other bytes of address equals to pipe1																			*/
/* note: for pipes[2..5] only first byte of address will be written because					*/
/*       pipes 1-5 share the four most significant address bytes										*/
void LVQ_NRF24_SetAddress(LVQ_NRF24_Config_t nrf24, uint8_t pipe, const uint8_t *addr) 
{
		uint8_t addr_width;

		/* RX_ADDR_Px register */
		switch (pipe) {
			case LVQ_NRF24_PIPETX:
			case LVQ_NRF24_PIPE0:
			case LVQ_NRF24_PIPE1:
				/* Get address width */
				addr_width = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_SETUP_AW) + 1;
				/* Write address in reverse order (LSByte first) */
				addr += addr_width;
				LVQ_NRF24_CSN_L(nrf24);
				LVQ_NRF24_RW(nrf24, LVQ_NRF24_CMD_W_REGISTER | LVQ_NRF24_ADDR_REGS[pipe]);
				do {
					LVQ_NRF24_RW(nrf24, *addr--);
				} while (addr_width--);
				LVQ_NRF24_CSN_H(nrf24);
				break;
			case LVQ_NRF24_PIPE2:
			case LVQ_NRF24_PIPE3:
			case LVQ_NRF24_PIPE4:
			case LVQ_NRF24_PIPE5:
				/* Write address LSBbyte (only first byte from the addr buffer) */
				LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_ADDR_REGS[pipe], *addr);
				break;
			default:
				/* Incorrect pipe number -> do nothing */
				break;
		}
}

/* Configure RF output power in TX mode 															*/
/* input: 																														*/
/*   nrf24 - information config nrf24 							  								*/
/*   tx_pwr - RF output power, one of LVQ_NRF24_TXPWR_xx values 			*/
void LVQ_NRF24_SetTXPower(LVQ_NRF24_Config_t nrf24, uint8_t tx_pwr) 
{
		uint8_t reg;

		/* Configure RF_PWR[2:1] bits of the RF_SETUP register */
		reg  = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_RF_SETUP);
		reg &= ~LVQ_NRF24_MASK_RF_PWR;
		reg |= tx_pwr;
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_RF_SETUP, reg);
}

/* Configure transceiver data rate 																		*/
/* input: 																														*/
/*   nrf24 - information config nrf24 							  								*/
/*   data_rate - data rate, one of LVQ_NRF24_DR_xx values 						*/
void LVQ_NRF24_SetDataRate(LVQ_NRF24_Config_t nrf24, uint8_t data_rate) 
{
		uint8_t reg;

		/* Configure RF_DR_LOW[5] and RF_DR_HIGH[3] bits of the RF_SETUP register */
		reg  = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_RF_SETUP);
		reg &= ~LVQ_NRF24_MASK_DATARATE;
		reg |= data_rate;
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_RF_SETUP, reg);
}

/* Configure a specified RX pipe 																						*/
/* input: 																																	*/
/*   nrf24 - information config nrf24 							  											*/
/*   pipe - number of the RX pipe, value from 0 to 5 												*/
/*   aa_state - state of auto acknowledgment, one of LVQ_NRF24_AA_xx values */
/*   payload_len - payload length in bytes */
void LVQ_NRF24_SetRXPipe(LVQ_NRF24_Config_t nrf24, uint8_t pipe, uint8_t aa_state, uint8_t payload_len) 
{
		uint8_t reg;

		/* Enable the specified pipe (EN_RXADDR register) */
		reg = (LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_EN_RXADDR) | (1 << pipe)) & LVQ_NRF24_MASK_EN_RX;
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_EN_RXADDR, reg);

		/* Set RX payload length (RX_PW_Px register) */
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_RX_PW_PIPE[pipe], payload_len & LVQ_NRF24_MASK_RX_PW);

		/* Set auto acknowledgment for a specified pipe (EN_AA register) */
		reg = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_EN_AA);
		if (aa_state == LVQ_NRF24_AA_ON) {
			reg |=  (1 << pipe);
		} else {
			reg &= ~(1 << pipe);
		}
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_EN_AA, reg);
}

/* Disable specified RX pipe 																								*/
/* input: 																																	*/
/*   nrf24 - information config nrf24 							  											*/
/*   PIPE - number of RX pipe, value from 0 to 5 														*/
void LVQ_NRF24_ClosePipe(LVQ_NRF24_Config_t nrf24, uint8_t pipe) 
{
		uint8_t reg;

		reg  = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_EN_RXADDR);
		reg &= ~(1 << pipe);
		reg &= LVQ_NRF24_MASK_EN_RX;
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_EN_RXADDR, reg);
}

/* Enable the auto retransmit (a.k.a. enhanced ShockBurst) for the specified RX pipe 	*/
/* input: 																																						*/
/*   nrf24 - information config nrf24 							  																*/
/*   pipe - number of the RX pipe, value from 0 to 5 																	*/
void LVQ_NRF24_EnableAA(LVQ_NRF24_Config_t nrf24, uint8_t pipe) 
{
		uint8_t reg;

		/* Set bit in EN_AA register */
		reg  = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_EN_AA);
		reg |= (1 << pipe);
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_EN_AA, reg);
}

/* Disable the auto retransmit (a.k.a. enhanced ShockBurst) for one or all RX pipes 										*/
/* input: 																																															*/
/*   nrf24 - information config nrf24 							  																									*/
/*   pipe - number of the RX pipe, value from 0 to 5, any other value will disable AA for all RX pipes 	*/
void LVQ_NRF24_DisableAA(LVQ_NRF24_Config_t nrf24, uint8_t pipe) 
{
		uint8_t reg;

		if (pipe > 5) {
			/* Disable Auto-ACK for ALL pipes */
			LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_EN_AA, 0x00);
		} else {
			/* Clear bit in the EN_AA register */
			reg  = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_EN_AA);
			reg &= ~(1 << pipe);
			LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_EN_AA, reg);
		}
}

/* Get value of the STATUS register */
/* return: value of STATUS register */
uint8_t LVQ_NRF24_GetStatus(LVQ_NRF24_Config_t nrf24) 
{
		return LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_STATUS);
}

/* Get pending IRQ flags                                                         */
/* input:                                                                        */
/*   nrf24 - information config nrf24 							                             */
/* return: current status of RX_DR, TX_DS and MAX_RT bits of the STATUS register */
uint8_t LVQ_NRF24_GetIRQFlags(LVQ_NRF24_Config_t nrf24) 
{
		return (LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_STATUS) & LVQ_NRF24_MASK_STATUS_IRQ);
}

/* Get status of the RX FIFO 																										 */
/* input:                                                                        */
/*   nrf24 - information config nrf24 							                             */
/* return: one of the LVQ_NRF24_STATUS_RXFIFO_xx values 												 */
uint8_t LVQ_NRF24_GetStatus_RXFIFO(LVQ_NRF24_Config_t nrf24) 
{
		return (LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_FIFO_STATUS) & LVQ_NRF24_MASK_RXFIFO);
}

/* Get status of the TX FIFO 																										 */
/* input:                                                                        */
/*   nrf24 - information config nrf24 							                             */
/* return: one of the LVQ_NRF24_STATUS_TXFIFO_xx values 												 */
/* note: the TX_REUSE bit ignored 																							 */
uint8_t LVQ_NRF24_GetStatus_TXFIFO(LVQ_NRF24_Config_t nrf24) 
{
		return ((LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_FIFO_STATUS) & LVQ_NRF24_MASK_TXFIFO) >> 4);
}

/* Get pipe number for the payload available for reading from RX FIFO 					 */
/* input:                                                                        */
/*   nrf24 - information config nrf24 							                             */
/* return: pipe number or 0x07 if the RX FIFO is empty 													 */
uint8_t LVQ_NRF24_GetRXSource(LVQ_NRF24_Config_t nrf24) 
{
		return ((LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_STATUS) & LVQ_NRF24_MASK_RX_P_NO) >> 1);
}

/* Get auto retransmit statistic 																															*/
/* input:                                                                        							*/
/*   nrf24 - information config nrf24 							                             							*/
/* return: value of OBSERVE_TX register which contains two counters encoded in nibbles:   		*/
/*   high - lost packets count (max value 15, can be reseted by write to RF_CH register)     	*/
/*   low  - retransmitted packets count (max value 15, reseted when new transmission starts) 	*/
uint8_t LVQ_NRF24_GetRetransmitCounters(LVQ_NRF24_Config_t nrf24) 
{
		return (LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_OBSERVE_TX));
}

/* Reset packet lost counter (PLOS_CNT bits in OBSERVER_TX register) */
void LVQ_NRF24_ResetPLOS(LVQ_NRF24_Config_t nrf24) 
{
		uint8_t reg;

		// The PLOS counter is reset after write to RF_CH register
		reg = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_RF_CH);
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_RF_CH, reg);
}

/* Flush the TX FIFO */
void LVQ_NRF24_FlushTX(LVQ_NRF24_Config_t nrf24) 
{
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_CMD_FLUSH_TX, LVQ_NRF24_CMD_NOP);
}

/* Flush the RX FIFO */
void LVQ_NRF24_FlushRX(LVQ_NRF24_Config_t nrf24)
{
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_CMD_FLUSH_RX, LVQ_NRF24_CMD_NOP);
}

/* Clear any pending IRQ flags */
void LVQ_NRF24_ClearIRQFlags(LVQ_NRF24_Config_t nrf24) 
{
		uint8_t reg;

		/* Clear RX_DR, TX_DS and MAX_RT bits of the STATUS register */
		reg  = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_STATUS);
		reg |= LVQ_NRF24_MASK_STATUS_IRQ;
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_STATUS, reg);
}

/* Write TX payload 																					*/
/* input: 																										*/
/*   nrf24 - information config nrf24 		      							*/
/*   pBuf - pointer to the buffer with payload data 					*/
/*   length - payload length in bytes  												*/
void LVQ_NRF24_WritePayload(LVQ_NRF24_Config_t nrf24, uint8_t *pBuf, uint8_t length) 
{
		LVQ_NRF24_WriteMBReg(nrf24, LVQ_NRF24_CMD_W_TX_PAYLOAD, pBuf, length);
}

static uint8_t LVQ_NRF24_GetRxDplPayloadWidth(LVQ_NRF24_Config_t nrf24) 
{
		uint8_t value;

		LVQ_NRF24_CSN_L(nrf24);
		LVQ_NRF24_RW(nrf24, LVQ_NRF24_CMD_R_RX_PL_WID);
		value = LVQ_NRF24_RW(nrf24, LVQ_NRF24_CMD_NOP);
		LVQ_NRF24_CSN_H(nrf24);

		return value;
}

static LVQ_NRF24_RXResult_t LVQ_NRF24_ReadPayloadGeneric(LVQ_NRF24_Config_t nrf24, uint8_t *pBuf, uint8_t *length, uint8_t dpl) 
{
		uint8_t pipe;

		/* Extract a payload pipe number from the STATUS register */
		pipe = (LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_STATUS) & LVQ_NRF24_MASK_RX_P_NO) >> 1;

		/* RX FIFO empty ? */
		if (pipe < 6) {
			/* Get payload length */
			if(dpl) 
			{
					*length = LVQ_NRF24_GetRxDplPayloadWidth(nrf24);
					if(*length > 32) 
					{ /* broken packet */
							*length = 0;
							LVQ_NRF24_FlushRX(nrf24);
					}
			} 
			else 
			{
				*length = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_RX_PW_PIPE[pipe]);
			}

			/* Read a payload from the RX FIFO */
			if (*length) 
			{
				LVQ_NRF24_ReadMBReg(nrf24, LVQ_NRF24_CMD_R_RX_PAYLOAD, pBuf, *length);
			}

			return ((LVQ_NRF24_RXResult_t) pipe);
		}

		/* The RX FIFO is empty */
		*length = 0;

		return LVQ_NRF24_RX_EMPTY;
}

/* Read top level payload available in the RX FIFO														*/
/* input:																																			*/
/*   nrf24 - information config nrf24 		      															*/
/*   pBuf - pointer to the buffer to store a payload data											*/
/*   length - pointer to variable to store a payload length										*/
/* return: one of LVQ_NRF24_RX_xx values																			*/
/*   LVQ_NRF24_RX_PIPEX - packet has been received from the pipe number X			*/
/*   LVQ_NRF24_RX_EMPTY - the RX FIFO is empty 																*/
LVQ_NRF24_RXResult_t LVQ_NRF24_ReadPayload(LVQ_NRF24_Config_t nrf24, uint8_t *pBuf, uint8_t *length) 
{
		return LVQ_NRF24_ReadPayloadGeneric(nrf24, pBuf, length,0);
}

LVQ_NRF24_RXResult_t LVQ_NRF24_ReadPayloadDpl(LVQ_NRF24_Config_t nrf24, uint8_t *pBuf, uint8_t *length) 
{
		return LVQ_NRF24_ReadPayloadGeneric(nrf24, pBuf, length,1);
}

uint8_t LVQ_NRF24_GetFeatures(LVQ_NRF24_Config_t nrf24) 
{
    return LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_FEATURE);
}

void LVQ_NRF24_ActivateFeatures(LVQ_NRF24_Config_t nrf24) 
{
    LVQ_NRF24_CSN_L(nrf24);
    LVQ_NRF24_RW(nrf24, LVQ_NRF24_CMD_ACTIVATE);
    LVQ_NRF24_RW(nrf24, 0x73);
    LVQ_NRF24_CSN_H(nrf24);
}
void LVQ_NRF24_WriteAckPayload(LVQ_NRF24_Config_t nrf24, LVQ_NRF24_RXResult_t pipe, char *payload, uint8_t length) 
{
		LVQ_NRF24_CSN_L(nrf24);
		LVQ_NRF24_RW(nrf24, LVQ_NRF24_CMD_W_ACK_PAYLOAD | pipe);
		while (length--) {
			LVQ_NRF24_RW(nrf24, (uint8_t) *payload++);
		}
		LVQ_NRF24_CSN_H(nrf24);
}

/* =============================== MODE TX/RX =============================*/

/* Set NRF24 in Mode RTX */
/*

Setting default:
		- LVQ_NRF24_CRC_2byte;
		- 
*/
void LVQ_NRF24_StartRTXMode(LVQ_NRF24_Config_t* __nrf24);
void LVQ_NRF24_StartRTXMode(LVQ_NRF24_Config_t* __nrf24)
{
		LVQ_NRF24_Config_t nrf24 = *__nrf24;
	
		if( nrf24.NRF24_Initialize )
				return;
		/* Disable ShockBurst for all RX pipes */
		LVQ_NRF24_DisableAA(nrf24, 0xFF);

		/* Set RF channel */
		LVQ_NRF24_SetRFChannel(nrf24, nrf24.NRF24_Channel);

		/* Set data rate */
		LVQ_NRF24_SetDataRate(nrf24, nrf24.NRF24_DataRate);

		/* Set CRC scheme */
		LVQ_NRF24_SetCRCScheme(nrf24, LVQ_NRF24_CRC_2byte);

		/* Set address width, its common for all pipes (RX and TX) */
		LVQ_NRF24_SetAddressWidth(nrf24, nrf24.NRF24_AddressWidth);

		/* Configure TX PIPE */
		LVQ_NRF24_SetAddress(nrf24, LVQ_NRF24_PIPETX, nrf24.NRF24_TX_Address); /* program TX address */
		LVQ_NRF24_SetAddress(nrf24, LVQ_NRF24_PIPE0, nrf24.NRF24_TX_Address); /* program TX address */
		
		/* Configure RX PIPE */
		LVQ_NRF24_SetAddress(nrf24, LVQ_NRF24_PIPE1, nrf24.NRF24_RX_Address); /* program RX address */
		LVQ_NRF24_SetRXPipe(nrf24, LVQ_NRF24_PIPE1, LVQ_NRF24_AA_OFF, nrf24.NRF24_SizePayLoad); 

		/* Set TX power (maximum) */
		LVQ_NRF24_SetTXPower(nrf24, nrf24.NRF24_TX_Power);

		/* Clear any pending IRQ flags */
		LVQ_NRF24_ClearIRQFlags(nrf24);

		/* Wake the transceiver */
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_UP);
		
		/* Initialize done*/
		__nrf24->NRF24_Initialize = 1;
}

/* Set NRF24 in Mode TX */
/*

Setting default:
		- LVQ_NRF24_CRC_2byte;
		- 
*/

void LVQ_NRF24_StartTXMode(LVQ_NRF24_Config_t* __nrf24)
{
		LVQ_NRF24_Config_t nrf24 = *__nrf24;
	
		if( nrf24.NRF24_Initialize )
				return;
		/* Disable ShockBurst for all RX pipes */
		LVQ_NRF24_DisableAA(nrf24, 0xFF);

		/* Set RF channel */
		LVQ_NRF24_SetRFChannel(nrf24, nrf24.NRF24_Channel);

		/* Set data rate */
		LVQ_NRF24_SetDataRate(nrf24, nrf24.NRF24_DataRate);

		/* Set CRC scheme */
		LVQ_NRF24_SetCRCScheme(nrf24, LVQ_NRF24_CRC_2byte);

		/* Set address width, its common for all pipes (RX and TX) */
		LVQ_NRF24_SetAddressWidth(nrf24, nrf24.NRF24_AddressWidth);

		/* Configure TX PIPE */
		LVQ_NRF24_SetAddress(nrf24, LVQ_NRF24_PIPETX, nrf24.NRF24_TX_Address); /* program TX address */
		LVQ_NRF24_SetAddress(nrf24, LVQ_NRF24_PIPE0, nrf24.NRF24_TX_Address); /* program TX address */
		
		/* Configure RX PIPE */
		LVQ_NRF24_SetAddress(nrf24, LVQ_NRF24_PIPE1, nrf24.NRF24_RX_Address); /* program RX address */
		LVQ_NRF24_SetRXPipe(nrf24, LVQ_NRF24_PIPE1, LVQ_NRF24_AA_OFF, nrf24.NRF24_SizePayLoad); 

		/* Set TX power (maximum) */
		LVQ_NRF24_SetTXPower(nrf24, nrf24.NRF24_TX_Power);

		/* Set operational mode (PTX == transmitter) */
		LVQ_NRF24_SetOperationalMode(nrf24, LVQ_NRF24_MODE_TX);

		/* Clear any pending IRQ flags */
		LVQ_NRF24_ClearIRQFlags(nrf24);

		/* Wake the transceiver */
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_UP);
		
		/* Initialize done*/
		__nrf24->NRF24_Initialize = 1;
}

/* Set NRF24 in Mode RX */
/*

Setting default:
		- LVQ_NRF24_CRC_2byte;
		- 
*/

void LVQ_NRF24_StartRXMode(LVQ_NRF24_Config_t* __nrf24)
{
		LVQ_NRF24_Config_t nrf24 = *__nrf24;
	
		if( nrf24.NRF24_Initialize )
				return;
		/* Disable ShockBurst for all RX pipes */
    LVQ_NRF24_DisableAA(nrf24, 0xFF);

    /* Set RF channel */
    LVQ_NRF24_SetRFChannel(nrf24, nrf24.NRF24_Channel);

    /* Set data rate */
    LVQ_NRF24_SetDataRate(nrf24, nrf24.NRF24_DataRate);

    /* Set CRC scheme */
    LVQ_NRF24_SetCRCScheme(nrf24, LVQ_NRF24_CRC_2byte);

    /* Set address width, its common for all pipes (RX and TX) */
    LVQ_NRF24_SetAddressWidth(nrf24, nrf24.NRF24_AddressWidth);
		
		/* Configure TX PIPE */
		LVQ_NRF24_SetAddress(nrf24, LVQ_NRF24_PIPETX, nrf24.NRF24_TX_Address); 									/* program TX address */
		LVQ_NRF24_SetAddress(nrf24, LVQ_NRF24_PIPE0, nrf24.NRF24_TX_Address); /* program TX address */
		
    /* Configure RX PIPE */
    LVQ_NRF24_SetAddress(nrf24, LVQ_NRF24_PIPE1, nrf24.NRF24_RX_Address);                   /* program address for RX pipe #1 */
    LVQ_NRF24_SetRXPipe(nrf24, LVQ_NRF24_PIPE1, LVQ_NRF24_AA_OFF, nrf24.NRF24_SizePayLoad); /* Auto-ACK: disabled, payload length: x bytes */

		/* Set TX power (maximum) when mode TX */
		LVQ_NRF24_SetTXPower(nrf24, nrf24.NRF24_TX_Power);
		
    /* Set operational mode (PRX == receiver) */
    LVQ_NRF24_SetOperationalMode(nrf24, LVQ_NRF24_MODE_RX);

    /* Wake the transceiver */
    LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_UP);
		
		Delay_ms(50);

    /* Put the transceiver to the RX mode */
    LVQ_NRF24_CE_H(nrf24);
		
		/* Initialize done*/
		__nrf24->NRF24_Initialize = 1;
}

/* Function to transmit data packet																						*/
/* input:																																			*/
/*   nrf24 - information config nrf24 		      															*/
/*   pBuf - pointer to the buffer with data to transmit												*/
/*   length - length of the data buffer in bytes															*/
/* return: one of LVQ_NRF24_TX_xx values                                     	*/
LVQ_NRF24_TXResult_t LVQ_NRF24_TransmitPacket(LVQ_NRF24_Config_t nrf24, uint8_t *pBuf, uint8_t length) 
{
		volatile uint32_t wait = LVQ_NRF24_WAIT_TIMEOUT;
		uint8_t status;
		
		/* Disable the CE pin */
		LVQ_NRF24_CE_L(nrf24);

		/* Transfer a data from the specified buffer to the TX FIFO */
		LVQ_NRF24_WritePayload(nrf24, pBuf, length);

		/* Start a transmission by asserting CE pin (must be held at least 10us) */
		LVQ_NRF24_CE_H(nrf24);

		/* Poll the transceiver status register until one of the following flags will be set:   			*/	
		/*   TX_DS  - means the packet has been transmitted																						*/
		/*   MAX_RT - means the maximum number of TX retransmits happened															*/
		/* note: this solution is far from perfect, better to use IRQ instead of polling the status   */
		do {
				status = LVQ_NRF24_GetStatus(nrf24);
				if (status & (LVQ_NRF24_FLAG_TX_DS | LVQ_NRF24_FLAG_MAX_RT)) 
				{
						break;
				}
		} while (wait--);

		/* Disable the CE pin */
		LVQ_NRF24_CE_L(nrf24);

		if (!wait) 
		{
			/* Timeout */
			return LVQ_NRF24_TX_TIMEOUT;
		}

		/* Check the flags in STATUS register */
		/* printf("[%d]", status); */

		/* Clear pending IRQ flags */
		LVQ_NRF24_ClearIRQFlags(nrf24);

		if (status & LVQ_NRF24_FLAG_MAX_RT) 
		{
				/* Auto retransmit counter exceeds the programmed maximum limit (FIFO is not removed) */
				return LVQ_NRF24_TX_MAXRT;
		}

		if (status & LVQ_NRF24_FLAG_TX_DS) 
		{
				/* Successful transmission */
				return LVQ_NRF24_TX_SUCCESS;
		}

		/* Some banana happens, a payload remains in the TX FIFO, flush it */
		LVQ_NRF24_FlushTX(nrf24);

		return LVQ_NRF24_TX_ERROR;
}

void LVQ_NRF24_DisableIRQ(LVQ_NRF24_Config_t nrf24, uint8_t Flag_IRQ)
{
		uint8_t reg;

		/* Configure IRQ bit of the CONFIG register */
		reg  = LVQ_NRF24_ReadReg( nrf24, LVQ_NRF24_REG_CONFIG);
		reg &= ~Flag_IRQ;
		reg |= Flag_IRQ;
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_CONFIG, reg);
}

void LVQ_NRF24_EnableIRQ(LVQ_NRF24_Config_t nrf24, uint8_t Flag_IRQ);
void LVQ_NRF24_EnableIRQ(LVQ_NRF24_Config_t nrf24, uint8_t Flag_IRQ)
{
		uint8_t reg;

		/* Configure IRQ bit of the CONFIG register */
		reg  = LVQ_NRF24_ReadReg( nrf24, LVQ_NRF24_REG_CONFIG);
		reg &= ~Flag_IRQ;
		LVQ_NRF24_WriteReg( nrf24, LVQ_NRF24_REG_CONFIG, reg);
}

/* Clear any pending IRQ flags */
void LVQ_NRF24_ClearIRQFlag(LVQ_NRF24_Config_t nrf24, uint8_t Flag_IRQ) 
{
		uint8_t reg;

		/* Clear RX_DR, TX_DS and MAX_RT bits of the STATUS register */
		reg  = LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_STATUS);
		reg |= Flag_IRQ;
		LVQ_NRF24_WriteReg(nrf24, LVQ_NRF24_REG_STATUS, reg);
}

void LVQ_NRF24_PowerUpTx(LVQ_NRF24_Config_t nrf24) 
{
		/* Disable RX/TX mode */
    LVQ_NRF24_CE_L(nrf24);
		/* Clear TX buffer */
    LVQ_NRF24_FlushTX(nrf24);
		/* Clear pending IRQ flags */
		LVQ_NRF24_ClearIRQFlags(nrf24);
		/* Setup TX mode */
		LVQ_NRF24_SetOperationalMode(nrf24, LVQ_NRF24_MODE_TX);
		/* Stop listening */
}

void LVQ_NRF24_PowerUpRx(LVQ_NRF24_Config_t nrf24) 
{
    /* Disable RX/TX mode */
    LVQ_NRF24_CE_L(nrf24);
    /* Clear RX buffer */
    LVQ_NRF24_FlushRX(nrf24);
    /* Clear pending IRQ flags */
		LVQ_NRF24_ClearIRQFlags(nrf24);
    /* Setup RX mode */
    LVQ_NRF24_SetOperationalMode(nrf24, LVQ_NRF24_MODE_RX);
    /* Start listening */
    LVQ_NRF24_CE_H(nrf24);
}

void LVQ_NRF24_OpeningPIPERead(LVQ_NRF24_Config_t nrf24, uint8_t pipe, uint8_t* addr);
void LVQ_NRF24_OpeningPIPERead(LVQ_NRF24_Config_t nrf24, uint8_t pipe, uint8_t* addr)
{
		LVQ_NRF24_SetAddress(nrf24, pipe, addr);  
		LVQ_NRF24_SetRXPipe(nrf24, pipe, LVQ_NRF24_AA_OFF, nrf24.NRF24_SizePayLoad); 
}

uint8_t LVQ_NRF24_TXSearchChannel(LVQ_NRF24_Config_t nrf24)
{
		uint8_t u8DataCheck[5] = { 0xAA, 0x01, 0x02, 0x03, 0x04};
		uint8_t u8SizePayLoad = 5;
		uint8_t u8Channel = 0;
		uint8_t pipe = 0;
		
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_DOWN);
		LVQ_NRF24_SetRXPipe(nrf24, LVQ_NRF24_PIPE1, LVQ_NRF24_AA_OFF, 5);
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_UP);
	
		start:
		
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_DOWN);
		Delay_ms(1);
		LVQ_NRF24_SetRFChannel(nrf24, u8Channel);
		Delay_ms(1);
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_UP);
		
		/* printf("Channel: %d\r\n", LVQ_NRF24_ReadReg(nrf24, LVQ_NRF24_REG_RF_CH)); */
		
		Delay_ms(1);
		LVQ_NRF24_PowerUpTx(nrf24);
		Delay_ms(1);
		
		/*
		printf("Status: [%d]\r\n", LVQ_NRF24_TransmitPacket(nrf24, u8DataCheck, u8SizePayLoad));
		printf("Status: [%d]\r\n", LVQ_NRF24_TransmitPacket(nrf24, u8DataCheck, u8SizePayLoad));
		*/
		
		LVQ_NRF24_TransmitPacket(nrf24, u8DataCheck, u8SizePayLoad);
		LVQ_NRF24_TransmitPacket(nrf24, u8DataCheck, u8SizePayLoad);
		
		LVQ_NRF24_PowerUpRx(nrf24);
		
		uint16_t u16TimeOut = LVQ_NRF24_TIMEOUTSEARCH;
		while ( u16TimeOut--) 
		{
				if (LVQ_NRF24_GetStatus_RXFIFO(nrf24) != LVQ_NRF24_STATUS_RXFIFO_EMPTY) 
				{
						/* Get a payload from the transceiver */
						pipe = LVQ_NRF24_ReadPayload(nrf24, u8DataCheck, &u8SizePayLoad);

						/* Clear pending IRQ flags */
						LVQ_NRF24_ClearIRQFlags(nrf24);

						if ( u8DataCheck[0] == 0xF0 )
						{
								/*
								printf("0x%d -0x%d -0x%d -0x%d - 0x%d\r\n", u8DataCheck[0], u8DataCheck[1], u8DataCheck[2], u8DataCheck[3], u8DataCheck[4]);
								printf("TimeOut: %d\r\n", u16TimeOut);
								*/
								goto end;
						}
				}
				Delay_ms(1);
		}
		if( u8Channel == 125 )
				return 0xFF;
		u8Channel++;
		goto start;
		
		end:
		LVQ_NRF24_PowerUpRx(nrf24);
		
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_DOWN);
		LVQ_NRF24_SetRXPipe(nrf24, LVQ_NRF24_PIPE1, LVQ_NRF24_AA_OFF, nrf24.NRF24_SizePayLoad);
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_UP);
		
		return u8Channel;
}

void LVQ_NRF24_RXSearchChannel(LVQ_NRF24_Config_t nrf24)
{
		uint8_t u8DataCheck[5] = { 0 };
		uint8_t u8SizePayLoad = 5;
		uint8_t pipe = 0;
		
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_DOWN);
		LVQ_NRF24_SetRXPipe(nrf24, LVQ_NRF24_PIPE1, LVQ_NRF24_AA_OFF, 5);
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_UP);
		
		while(1)
		{
				if (LVQ_NRF24_GetStatus_RXFIFO(nrf24) != LVQ_NRF24_STATUS_RXFIFO_EMPTY) 
				{
						/* Get a payload from the transceiver */
						pipe = LVQ_NRF24_ReadPayload(nrf24, u8DataCheck, &u8SizePayLoad);

						/* Clear pending IRQ flags */
						LVQ_NRF24_ClearIRQFlags(nrf24);

						if( u8DataCheck[0] == 0xAA)
						{
								LVQ_NRF24_PowerUpTx(nrf24);
								u8DataCheck[0] = 0xF0;
								LVQ_NRF24_TransmitPacket(nrf24, u8DataCheck, u8SizePayLoad);
								LVQ_NRF24_TransmitPacket(nrf24, u8DataCheck, u8SizePayLoad);
								LVQ_NRF24_PowerUpTx(nrf24);
								break;
						}
				}
		}
		
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_DOWN);
		LVQ_NRF24_SetRXPipe(nrf24, LVQ_NRF24_PIPE1, LVQ_NRF24_AA_OFF, nrf24.NRF24_SizePayLoad);
		LVQ_NRF24_SetPowerMode(nrf24, LVQ_NRF24_PWR_UP);
}

uint8_t CharToNo(uint8_t c)
{
		if( c < '0' || c > '9')
					return 0;
		return c - 48;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
		long lvalue = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		if ( lvalue > out_max )
				lvalue = out_max;
		if( lvalue < out_min )
				lvalue = out_min;
		return lvalue; 
}

void LVQ_NRF24_HandleData(uint8_t* pString, uint16_t* pOutput)
{
		/*X:100Y:100Z:100F:100G:100H:100S:100*/
		
		union {
				struct {
						uint16_t dataA;
						uint16_t dataB;
						uint16_t dataC;
						uint16_t dataD;
						uint16_t dataE;
						uint16_t dataF;
						uint16_t dataG;
						uint16_t dataH;
						uint16_t dataI;
						uint16_t dataJ;
						uint16_t dataK;
						uint16_t dataL;
						uint16_t dataM;
						uint16_t dataN;
						uint16_t dataO;
						uint16_t dataP;
				} Data;
				uint8_t datapayload[32];
		} DataFrame;
	
		strcpy((char*) DataFrame.datapayload, (char*) pString);
		
		pOutput[0] = DataFrame.Data.dataA;
		pOutput[1] = DataFrame.Data.dataB;
		pOutput[2] = DataFrame.Data.dataC;
		pOutput[3] = DataFrame.Data.dataD;
		pOutput[4] = DataFrame.Data.dataE;
		pOutput[5] = DataFrame.Data.dataF;
		pOutput[6] = DataFrame.Data.dataG;
		pOutput[7] = DataFrame.Data.dataH;
		pOutput[8] = DataFrame.Data.dataI;
		pOutput[9] = DataFrame.Data.dataJ;
		pOutput[10] = DataFrame.Data.dataK;
		pOutput[11] = DataFrame.Data.dataL;
		pOutput[12] = DataFrame.Data.dataM;
		pOutput[13] = DataFrame.Data.dataN;
		pOutput[14] = DataFrame.Data.dataO;
		pOutput[15] = DataFrame.Data.dataP;
	
		/*
			uint8_t PercentPWM;
		
			PercentPWM = CharToNo(pString[2]) * 100 + CharToNo(pString[3]) * 10 + CharToNo(pString[4]);
		
			if( pString[0] == 'X' && pOutput[0] != PercentPWM )
						pOutput[0] = PercentPWM;
			
			PercentPWM = CharToNo(pString[7]) * 100 + CharToNo(pString[8]) * 10 + CharToNo(pString[9]);
		
			if( pString[5] == 'Y' && pOutput[1] != PercentPWM )
						pOutput[1] = PercentPWM;
		
			PercentPWM = CharToNo(pString[12]) * 100 + CharToNo(pString[13]) * 10 + CharToNo(pString[14]);
		
			if( pString[10] == 'Z' && pOutput[2] != PercentPWM )
						pOutput[2] = PercentPWM;
			
			PercentPWM = CharToNo(pString[17]) * 100 + CharToNo(pString[18]) * 10 + CharToNo(pString[19]);
		
			if( pString[15] == 'F' && pOutput[3] != PercentPWM )
						pOutput[3] = PercentPWM;

			PercentPWM = CharToNo(pString[22]) * 100 + CharToNo(pString[23]) * 10 + CharToNo(pString[24]);
		
			if( pString[20] == 'G' && pOutput[4] != PercentPWM )
						pOutput[4] = PercentPWM;
			
			PercentPWM = CharToNo(pString[27]) * 100 + CharToNo(pString[28]) * 10 + CharToNo(pString[29]);
		
			if( pString[25] == 'H' && pOutput[5] != PercentPWM )
						pOutput[5] = PercentPWM;
			
			PercentPWM = CharToNo(pString[31]);
		
			if( pString[30] == 'S' && pOutput[6] != PercentPWM )
						pOutput[6] = PercentPWM;
		*/
}
