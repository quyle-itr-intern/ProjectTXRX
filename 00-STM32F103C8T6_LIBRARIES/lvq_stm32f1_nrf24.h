/**
  ******************************************************************************

  NRF24 For STM32F446RE
  Author:   LVQ
  Updated:  21 April 2023
	Reference : https://github.com/elmot/nrf24l01-lib

  ******************************************************************************
*/

#ifndef __NRF24_H
#define __NRF24_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */

#include "lvq_stm32f1_rccConfig.h"
#include "lvq_stm32f1_spi.h"
#include "lvq_stm32f1_gpio.h"
#include "delay_systick.h"
#include "lvq_stm32f1_soft_spi.h"
#include "stdio.h"
#include "string.h"
#include "main.h"

typedef struct {
		SPI_TypeDef* 				NRF24_SPI;
		LVQ_SPI_PinsPack_t 	NRF24_SPI_PIN;
		GPIO_TypeDef* 			NRF24_CE_PORT;
		uint16_t      			NRF24_CE_PIN;
		GPIO_TypeDef* 			NRF24_CSN_PORT;
		uint16_t      			NRF24_CSN_PIN;
		uint8_t							NRF24_Channel;
		uint8_t							NRF24_DataRate;
		uint8_t							NRF24_SizePayLoad;
		uint8_t             NRF24_TX_Power;
		uint8_t							NRF24_AddressWidth;
		uint8_t		          NRF24_TX_Address[5];
		uint8_t		          NRF24_RX_Address[5];
		uint8_t             NRF24_Initialize;
} LVQ_NRF24_Config_t;

/* nRF24L0 instruction definitions */
#define LVQ_NRF24_CMD_R_REGISTER       (uint8_t)0x00 // Register read 
#define LVQ_NRF24_CMD_W_REGISTER       (uint8_t)0x20 // Register write
#define LVQ_NRF24_CMD_ACTIVATE         (uint8_t)0x50 // (De)Activates R_RX_PL_WID, W_ACK_PAYLOAD, W_TX_PAYLOAD_NOACK features
#define LVQ_NRF24_CMD_R_RX_PL_WID	  	 (uint8_t)0x60 // Read RX-payload width for the top R_RX_PAYLOAD in the RX FIFO.
#define LVQ_NRF24_CMD_R_RX_PAYLOAD     (uint8_t)0x61 // Read RX payload
#define LVQ_NRF24_CMD_W_TX_PAYLOAD     (uint8_t)0xA0 // Write TX payload
#define LVQ_NRF24_CMD_W_ACK_PAYLOAD    (uint8_t)0xA8 // Write ACK payload 
#define LVQ_NRF24_CMD_W_TX_PAYLOAD_NOACK (uint8_t) 0xB0//Write TX payload and disable AUTOACK
#define LVQ_NRF24_CMD_FLUSH_TX         (uint8_t)0xE1 // Flush TX FIFO
#define LVQ_NRF24_CMD_FLUSH_RX         (uint8_t)0xE2 // Flush RX FIFO
#define LVQ_NRF24_CMD_REUSE_TX_PL      (uint8_t)0xE3 // Reuse TX payload
#define LVQ_NRF24_CMD_LOCK_UNLOCK      (uint8_t)0x50 // Lock/unlock exclusive features
#define LVQ_NRF24_CMD_NOP              (uint8_t)0xFF // No operation (used for reading status register)

/* nRF24L0 register definitions */
#define LVQ_NRF24_REG_CONFIG           (uint8_t)0x00 // Configuration register
#define LVQ_NRF24_REG_EN_AA            (uint8_t)0x01 // Enable "Auto acknowledgment"
#define LVQ_NRF24_REG_EN_RXADDR        (uint8_t)0x02 // Enable RX addresses
#define LVQ_NRF24_REG_SETUP_AW         (uint8_t)0x03 // Setup of address widths
#define LVQ_NRF24_REG_SETUP_RETR       (uint8_t)0x04 // Setup of automatic retransmit
#define LVQ_NRF24_REG_RF_CH            (uint8_t)0x05 // RF channel
#define LVQ_NRF24_REG_RF_SETUP         (uint8_t)0x06 // RF setup register
#define LVQ_NRF24_REG_STATUS           (uint8_t)0x07 // Status register
#define LVQ_NRF24_REG_OBSERVE_TX       (uint8_t)0x08 // Transmit observe register
#define LVQ_NRF24_REG_RPD              (uint8_t)0x09 // Received power detector
#define LVQ_NRF24_REG_RX_ADDR_P0       (uint8_t)0x0A // Receive address data pipe 0
#define LVQ_NRF24_REG_RX_ADDR_P1       (uint8_t)0x0B // Receive address data pipe 1
#define LVQ_NRF24_REG_RX_ADDR_P2       (uint8_t)0x0C // Receive address data pipe 2
#define LVQ_NRF24_REG_RX_ADDR_P3       (uint8_t)0x0D // Receive address data pipe 3
#define LVQ_NRF24_REG_RX_ADDR_P4       (uint8_t)0x0E // Receive address data pipe 4
#define LVQ_NRF24_REG_RX_ADDR_P5       (uint8_t)0x0F // Receive address data pipe 5
#define LVQ_NRF24_REG_TX_ADDR          (uint8_t)0x10 // Transmit address
#define LVQ_NRF24_REG_RX_PW_P0         (uint8_t)0x11 // Number of bytes in RX payload in data pipe 0
#define LVQ_NRF24_REG_RX_PW_P1         (uint8_t)0x12 // Number of bytes in RX payload in data pipe 1
#define LVQ_NRF24_REG_RX_PW_P2         (uint8_t)0x13 // Number of bytes in RX payload in data pipe 2
#define LVQ_NRF24_REG_RX_PW_P3         (uint8_t)0x14 // Number of bytes in RX payload in data pipe 3
#define LVQ_NRF24_REG_RX_PW_P4         (uint8_t)0x15 // Number of bytes in RX payload in data pipe 4
#define LVQ_NRF24_REG_RX_PW_P5         (uint8_t)0x16 // Number of bytes in RX payload in data pipe 5
#define LVQ_NRF24_REG_FIFO_STATUS      (uint8_t)0x17 // FIFO status register
#define LVQ_NRF24_REG_DYNPD            (uint8_t)0x1C // Enable dynamic payload length
#define LVQ_NRF24_REG_FEATURE          (uint8_t)0x1D // Feature register

// Register bits definitions
#define LVQ_NRF24_CONFIG_PRIM_RX       (uint8_t)0x01 // PRIM_RX bit in CONFIG register
#define LVQ_NRF24_CONFIG_PWR_UP        (uint8_t)0x02 // PWR_UP bit in CONFIG register
#define LVQ_NRF24_FEATURE_EN_DYN_ACK   (uint8_t)0x01 // EN_DYN_ACK bit in FEATURE register
#define LVQ_NRF24_FEATURE_EN_ACK_PAY   (uint8_t)0x02 // EN_ACK_PAY bit in FEATURE register
#define LVQ_NRF24_FEATURE_EN_DPL       (uint8_t)0x04 // EN_DPL bit in FEATURE register
#define LVQ_NRF24_FLAG_RX_DR           (uint8_t)0x40 // RX_DR bit (data ready RX FIFO interrupt)
#define LVQ_NRF24_FLAG_TX_DS           (uint8_t)0x20 // TX_DS bit (data sent TX FIFO interrupt)
#define LVQ_NRF24_FLAG_MAX_RT          (uint8_t)0x10 // MAX_RT bit (maximum number of TX retransmits interrupt)

// Register masks definitions
#define LVQ_NRF24_MASK_REG_MAP         (uint8_t)0x1F // Mask bits[4:0] for CMD_RREG and CMD_WREG commands
#define LVQ_NRF24_MASK_CRC             (uint8_t)0x0C // Mask for CRC bits [3:2] in CONFIG register
#define LVQ_NRF24_MASK_STATUS_IRQ      (uint8_t)0x70 // Mask for all IRQ bits in STATUS register
#define LVQ_NRF24_MASK_RF_PWR          (uint8_t)0x06 // Mask RF_PWR[2:1] bits in RF_SETUP register
#define LVQ_NRF24_MASK_RX_P_NO         (uint8_t)0x0E // Mask RX_P_NO[3:1] bits in STATUS register
#define LVQ_NRF24_MASK_DATARATE        (uint8_t)0x28 // Mask RD_DR_[5,3] bits in RF_SETUP register
#define LVQ_NRF24_MASK_EN_RX           (uint8_t)0x3F // Mask ERX_P[5:0] bits in EN_RXADDR register
#define LVQ_NRF24_MASK_RX_PW           (uint8_t)0x3F // Mask [5:0] bits in RX_PW_Px register
#define LVQ_NRF24_MASK_RETR_ARD        (uint8_t)0xF0 // Mask for ARD[7:4] bits in SETUP_RETR register
#define LVQ_NRF24_MASK_RETR_ARC        (uint8_t)0x0F // Mask for ARC[3:0] bits in SETUP_RETR register
#define LVQ_NRF24_MASK_RXFIFO          (uint8_t)0x03 // Mask for RX FIFO status bits [1:0] in FIFO_STATUS register
#define LVQ_NRF24_MASK_TXFIFO          (uint8_t)0x30 // Mask for TX FIFO status bits [5:4] in FIFO_STATUS register
#define LVQ_NRF24_MASK_PLOS_CNT        (uint8_t)0xF0 // Mask for PLOS_CNT[7:4] bits in OBSERVE_TX register
#define LVQ_NRF24_MASK_ARC_CNT         (uint8_t)0x0F // Mask for ARC_CNT[3:0] bits in OBSERVE_TX register

/* Fake address to test transceiver presence (5 bytes long) */
#define LVQ_NRF24_TEST_ADDR            "nRF24"
#define LVQ_NRF24_WAIT_TIMEOUT         (uint32_t)0x000FFFF
#define LVQ_NRF24_TIMEOUTSEARCH        50 /* 25 */

/* Retransmit delay */
enum {
	LVQ_NRF24_ARD_NONE   = (uint8_t)0x00, // Dummy value for case when retransmission is not used
	LVQ_NRF24_ARD_250us  = (uint8_t)0x00,
	LVQ_NRF24_ARD_500us  = (uint8_t)0x01,
	LVQ_NRF24_ARD_750us  = (uint8_t)0x02,
	LVQ_NRF24_ARD_1000us = (uint8_t)0x03,
	LVQ_NRF24_ARD_1250us = (uint8_t)0x04,
	LVQ_NRF24_ARD_1500us = (uint8_t)0x05,
	LVQ_NRF24_ARD_1750us = (uint8_t)0x06,
	LVQ_NRF24_ARD_2000us = (uint8_t)0x07,
	LVQ_NRF24_ARD_2250us = (uint8_t)0x08,
	LVQ_NRF24_ARD_2500us = (uint8_t)0x09,
	LVQ_NRF24_ARD_2750us = (uint8_t)0x0A,
	LVQ_NRF24_ARD_3000us = (uint8_t)0x0B,
	LVQ_NRF24_ARD_3250us = (uint8_t)0x0C,
	LVQ_NRF24_ARD_3500us = (uint8_t)0x0D,
	LVQ_NRF24_ARD_3750us = (uint8_t)0x0E,
	LVQ_NRF24_ARD_4000us = (uint8_t)0x0F
};

/* Data rate */
enum {
	LVQ_NRF24_DR_250kbps = (uint8_t)0x20, /* 250kbps data rate */
	LVQ_NRF24_DR_1Mbps   = (uint8_t)0x00, /* 1Mbps data rate */
	LVQ_NRF24_DR_2Mbps   = (uint8_t)0x08  /* 2Mbps data rate */
};

/* RF output power in TX mode */
enum {
	LVQ_NRF24_TXPWR_18dBm = (uint8_t)0x00, /* -18dBm */
	LVQ_NRF24_TXPWR_12dBm = (uint8_t)0x02, /* -12dBm */
	LVQ_NRF24_TXPWR_6dBm  = (uint8_t)0x04, /* -6dBm */
	LVQ_NRF24_TXPWR_0dBm  = (uint8_t)0x06  /*  0dBm */
};

/* CRC encoding scheme */
enum {
	LVQ_NRF24_CRC_Off   = (uint8_t)0x00, /* CRC disabled */
	LVQ_NRF24_CRC_1byte = (uint8_t)0x08, /* 1-byte CRC */
	LVQ_NRF24_CRC_2byte = (uint8_t)0x0c  /* 2-byte CRC */
};

/* nRF24L01 power control */
enum {
	LVQ_NRF24_PWR_UP   = (uint8_t)0x02, /* Power up */
	LVQ_NRF24_PWR_DOWN = (uint8_t)0x00  /* Power down */
};

/* Transceiver mode */
enum {
	LVQ_NRF24_MODE_RX = (uint8_t)0x01, /* PRX */
	LVQ_NRF24_MODE_TX = (uint8_t)0x00  /* PTX */
};

enum {
	LVQ_NRF24_DPL_ON = (uint8_t)0x01,  /* PRX */
	LVQ_NRF24_DPL_OFF = (uint8_t)0x00  /* PTX */
} ;

/* Enumeration of RX pipe addresses and TX address */
enum {
	LVQ_NRF24_PIPE0  = (uint8_t)0x00, /* pipe0 */
	LVQ_NRF24_PIPE1  = (uint8_t)0x01, /* pipe1 */
	LVQ_NRF24_PIPE2  = (uint8_t)0x02, /* pipe2 */
	LVQ_NRF24_PIPE3  = (uint8_t)0x03, /* pipe3 */
	LVQ_NRF24_PIPE4  = (uint8_t)0x04, /* pipe4 */
	LVQ_NRF24_PIPE5  = (uint8_t)0x05, /* pipe5 */
	LVQ_NRF24_PIPETX = (uint8_t)0x06  /* TX address (not a pipe in fact) */
};

/* State of auto acknowledgment for specified pipe */
enum {
	LVQ_NRF24_AA_OFF = (uint8_t)0x00,
	LVQ_NRF24_AA_ON  = (uint8_t)0x01
};

/* Status of the RX FIFO */
enum {
	LVQ_NRF24_STATUS_RXFIFO_DATA  = (uint8_t)0x00, /* The RX FIFO contains data and available locations */
	LVQ_NRF24_STATUS_RXFIFO_EMPTY = (uint8_t)0x01, /* The RX FIFO is empty */
	LVQ_NRF24_STATUS_RXFIFO_FULL  = (uint8_t)0x02, /* The RX FIFO is full */
	LVQ_NRF24_STATUS_RXFIFO_ERROR = (uint8_t)0x03  /* Impossible state: RX FIFO cannot be empty and full at the same time */
};

/* Status of the TX FIFO */
enum {
	LVQ_NRF24_STATUS_TXFIFO_DATA  = (uint8_t)0x00, /* The TX FIFO contains data and available locations */
	LVQ_NRF24_STATUS_TXFIFO_EMPTY = (uint8_t)0x01, /* The TX FIFO is empty */
	LVQ_NRF24_STATUS_TXFIFO_FULL  = (uint8_t)0x02, /* The TX FIFO is full */
	LVQ_NRF24_STATUS_TXFIFO_ERROR = (uint8_t)0x03  /* Impossible state: TX FIFO cannot be empty and full at the same time */
};

/* Result of RX FIFO reading */
typedef enum {
	LVQ_NRF24_RX_PIPE0  = (uint8_t)0x00, /* Packet received from the PIPE#0 */
	LVQ_NRF24_RX_PIPE1  = (uint8_t)0x01, /* Packet received from the PIPE#1 */
	LVQ_NRF24_RX_PIPE2  = (uint8_t)0x02, /* Packet received from the PIPE#2 */
	LVQ_NRF24_RX_PIPE3  = (uint8_t)0x03, /* Packet received from the PIPE#3 */
	LVQ_NRF24_RX_PIPE4  = (uint8_t)0x04, /* Packet received from the PIPE#4 */
	LVQ_NRF24_RX_PIPE5  = (uint8_t)0x05, /* Packet received from the PIPE#5 */
	LVQ_NRF24_RX_EMPTY  = (uint8_t)0xff  /* The RX FIFO is empty */
} LVQ_NRF24_RXResult_t;


/* Addresses of the RX_PW_P# registers */
static const uint8_t LVQ_NRF24_RX_PW_PIPE[6] = {
		LVQ_NRF24_REG_RX_PW_P0,
		LVQ_NRF24_REG_RX_PW_P1,
		LVQ_NRF24_REG_RX_PW_P2,
		LVQ_NRF24_REG_RX_PW_P3,
		LVQ_NRF24_REG_RX_PW_P4,
		LVQ_NRF24_REG_RX_PW_P5
};

/* Addresses of the address registers */
static const uint8_t LVQ_NRF24_ADDR_REGS[7] = {
		LVQ_NRF24_REG_RX_ADDR_P0,
		LVQ_NRF24_REG_RX_ADDR_P1,
		LVQ_NRF24_REG_RX_ADDR_P2,
		LVQ_NRF24_REG_RX_ADDR_P3,
		LVQ_NRF24_REG_RX_ADDR_P4,
		LVQ_NRF24_REG_RX_ADDR_P5,
		LVQ_NRF24_REG_TX_ADDR
};

/* Result of packet transmission */
typedef enum {
		LVQ_NRF24_TX_ERROR  = (uint8_t)0x00, /* Unknown error */
		LVQ_NRF24_TX_SUCCESS,                /* Packet has been transmitted successfully */
		LVQ_NRF24_TX_TIMEOUT,                /* It was timeout during packet transmit */
		LVQ_NRF24_TX_MAXRT                   /* Transmit failed with maximum auto retransmit count */
} LVQ_NRF24_TXResult_t;

/* Function prototypes */
void LVQ_NRF24_Init(LVQ_NRF24_Config_t nrf24);
uint8_t LVQ_NRF24_Check(LVQ_NRF24_Config_t nrf24);

void LVQ_NRF24_StartTXMode(LVQ_NRF24_Config_t* __nrf24);
void LVQ_NRF24_StartRXMode(LVQ_NRF24_Config_t* __nrf24);

void LVQ_NRF24_SetPowerMode(LVQ_NRF24_Config_t nrf24, uint8_t mode);
void LVQ_NRF24_SetOperationalMode(LVQ_NRF24_Config_t nrf24, uint8_t mode);
void LVQ_NRF24_SetRFChannel(LVQ_NRF24_Config_t nrf24, uint8_t channel);
void LVQ_NRF24_SetAutoRetr(LVQ_NRF24_Config_t nrf24, uint8_t ard, uint8_t arc);
void LVQ_NRF24_SetAddressWidth(LVQ_NRF24_Config_t nrf24, uint8_t addr_width);
void LVQ_NRF24_SetAddress(LVQ_NRF24_Config_t nrf24, uint8_t pipe, const uint8_t *addr);
void LVQ_NRF24_SetTXPower(LVQ_NRF24_Config_t nrf24, uint8_t tx_pwr);
void LVQ_NRF24_SetDataRate(LVQ_NRF24_Config_t nrf24, uint8_t data_rate);
void LVQ_NRF24_SetCRCScheme(LVQ_NRF24_Config_t nrf24, uint8_t scheme);
void LVQ_NRF24_SetRXPipe(LVQ_NRF24_Config_t nrf24, uint8_t pipe, uint8_t aa_state, uint8_t payload_len);
void LVQ_NRF24_ClosePipe(LVQ_NRF24_Config_t nrf24, uint8_t pipe);
void LVQ_NRF24_EnableAA(LVQ_NRF24_Config_t nrf24, uint8_t pipe);
void LVQ_NRF24_DisableAA(LVQ_NRF24_Config_t nrf24, uint8_t pipe);
void LVQ_NRF24_SetDynamicPayloadLength(LVQ_NRF24_Config_t nrf24, uint8_t mode);
void LVQ_NRF24_SetPayloadWithAck(LVQ_NRF24_Config_t nrf24, uint8_t mode);

uint8_t LVQ_NRF24_GetStatus(LVQ_NRF24_Config_t nrf24);
uint8_t LVQ_NRF24_GetIRQFlags(LVQ_NRF24_Config_t nrf24);
uint8_t LVQ_NRF24_GetStatus_RXFIFO(LVQ_NRF24_Config_t nrf24);
uint8_t LVQ_NRF24_GetStatus_TXFIFO(LVQ_NRF24_Config_t nrf24);
uint8_t LVQ_NRF24_GetRXSource(LVQ_NRF24_Config_t nrf24);
uint8_t LVQ_NRF24_GetRetransmitCounters(LVQ_NRF24_Config_t nrf24);
uint8_t LVQ_NRF24_GetFeatures(LVQ_NRF24_Config_t nrf24);

void LVQ_NRF24_ResetPLOS(LVQ_NRF24_Config_t nrf24);
void LVQ_NRF24_FlushTX(LVQ_NRF24_Config_t nrf24);
void LVQ_NRF24_FlushRX(LVQ_NRF24_Config_t nrf24);
void LVQ_NRF24_ClearIRQFlags(LVQ_NRF24_Config_t nrf24);
void LVQ_NRF24_ClearIRQFlag(LVQ_NRF24_Config_t nrf24, uint8_t Flag_IRQ);
void LVQ_NRF24_ActivateFeatures(LVQ_NRF24_Config_t nrf24);
void LVQ_NRF24_WritePayload(LVQ_NRF24_Config_t nrf24, uint8_t *pBuf, uint8_t length);
void LVQ_NRF24_WriteAckPayload(LVQ_NRF24_Config_t nrf24, LVQ_NRF24_RXResult_t pipe, char *payload, uint8_t length);
LVQ_NRF24_RXResult_t LVQ_NRF24_ReadPayload(LVQ_NRF24_Config_t nrf24, uint8_t *pBuf, uint8_t *length);
LVQ_NRF24_RXResult_t LVQ_NRF24_ReadPayloadDpl(LVQ_NRF24_Config_t nrf24, uint8_t *pBuf, uint8_t *length);

LVQ_NRF24_TXResult_t LVQ_NRF24_TransmitPacket(LVQ_NRF24_Config_t nrf24, uint8_t *pBuf, uint8_t length);

void LVQ_NRF24_PowerUpTx(LVQ_NRF24_Config_t nrf24);
void LVQ_NRF24_PowerUpRx(LVQ_NRF24_Config_t nrf24);

void LVQ_NRF24_DisableIRQ(LVQ_NRF24_Config_t nrf24, uint8_t Flag_IRQ);

uint8_t LVQ_NRF24_TXSearchChannel(LVQ_NRF24_Config_t nrf24);
void LVQ_NRF24_RXSearchChannel(LVQ_NRF24_Config_t nrf24);

#define LVQ_NRF24_RX_ON(nrf24)   LVQ_NRF24_CE_H(nrf24);
#define LVQ_NRF24_RX_OFF(nrf24)  LVQ_NRF24_CE_L(nrf24);

uint8_t CharToNo(uint8_t c);
long map(long x, long in_min, long in_max, long out_min, long out_max);
void LVQ_NRF24_HandleData(uint8_t* pString, uint16_t* pOutput);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
