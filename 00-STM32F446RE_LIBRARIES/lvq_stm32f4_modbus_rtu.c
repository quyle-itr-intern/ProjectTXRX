/**
  ******************************************************************************

  MODBUS RTU For STM32F446RE
  Author:   LVQ
  Updated:  5 January 2023
	Algorithm: ...

  ******************************************************************************
*/

#include "lvq_stm32f4_modbus_rtu.h"

uint16_t LVQ_CRC16(uint8_t* buffer, uint16_t buffer_length);

void LVQ_MODBUS_RTU_Init(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info)
{
		LVQ_USART_Init(USART3, LVQ_USART_PinsPack_2, MODBUS_RTU_Info.Baudrate_MODBUS_RTU); 
		LVQ_GPIO_Init(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN, LVQ_GPIO_Mode_OUT, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_NOPULL, LVQ_GPIO_Speed_High);
		LVQ_GPIO_SetPinLow(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
}

/*Private Function */
void Function_Read_Holding_Register(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info);
void Function_Read_Input_Register(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info);
void Function_Read_Coils(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info);
void Function_Read_Input_Coils(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info);
void Function_Write_Single_Coil(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info);
void Function_Write_Single_Register(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info);
void Function_Write_Coils(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info);
void Function_Write_Holding_Registers(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info);


void LVQ_MODBUS_RTU_Write(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info)
{
	  Function_Write_Holding_Registers(MODBUS_RTU_Info);

}

void Function_Read_Holding_Register(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info)
{
		uint8_t TXData[8];
	
		TXData[0] = 0x05;   /* Slave Address */
		TXData[1] = 0x03;   /* Functon code */
	
		/*
		* The function code 0x03 means we are reading Holding Registers
		* The Register address ranges from 40001 - 50000
		* The register address we input can range from 0-9999 (0x00-0x270F)
		* Here 0 corresponds to the Address 40001 and 9999 corresponds to 50000
		* Although we can only read 125 registers sequentially at once
		*/
	
		TXData[2] = 0;      /* Offset = 40001 */
	  TXData[3] = 0x04;   /* 0000000 0000100 = Offset + 4 = 40005 */
	
		TXData[4] = 0;
	  TXData[5] = 0x05;   /* 000000000 00000101 = 5 Register = 10 Bytes */
	
		uint16_t u16CRC = LVQ_CRC16(TXData, 6);
	
		TXData[6] = u16CRC & 0xFF ;            /* CRC LOW */
	  TXData[7] = ( u16CRC >> 8) & 0xFF ;    /* CRC HIGH */
	
		LVQ_GPIO_SetPinHigh(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
	
		LVQ_USART_Send(MODBUS_RTU_Info.USART_MODBUS_RTU, TXData, 8);
	
		Delay_us_systick(120);
	
		LVQ_GPIO_SetPinLow(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
}

void Function_Read_Input_Register(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info)
{
		uint8_t TXData[8];
	
		TXData[0] = 0x05;   /* Slave Address */
		TXData[1] = 0x04;   /* Functon code */
	
		/*
		* The function code 0x03 means we are reading Holding Registers
		* The Register address ranges from 40001 - 50000
		* The register address we input can range from 0-9999 (0x00-0x270F)
		* Here 0 corresponds to the Address 40001 and 9999 corresponds to 50000
		* Although we can only read 125 registers sequentially at once
		*/
	
		TXData[2] = 0;      /* Offset = 30001 */
	  TXData[3] = 0x01;   /* 0000000 00000001 = Offset + 1 = 30002 */
	
		TXData[4] = 0;
	  TXData[5] = 0x05;   /* 000000000 00000101 = 5 Register = 10 Bytes */
	
		uint16_t u16CRC = LVQ_CRC16(TXData, 6);
	
		TXData[6] = u16CRC & 0xFF ;            /* CRC LOW */
	  TXData[7] = ( u16CRC >> 8) & 0xFF ;    /* CRC HIGH */
	
		LVQ_GPIO_SetPinHigh(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
	
		LVQ_USART_Send(MODBUS_RTU_Info.USART_MODBUS_RTU, TXData, 8);
	
		Delay_us_systick(120);
	
		LVQ_GPIO_SetPinLow(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
}

void Function_Read_Coils(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info)
{
		uint8_t TXData[8];
	
		TXData[0] = 0x05;   /* Slave Address */
		TXData[1] = 0x01;   /* Functon code */
	
		TXData[2] = 0;      /* Offset = 1 */
	  TXData[3] = 0x00;   /* 0000000 00000000 = Offset + 1 = 1 */
	
		TXData[4] = 0;
	  TXData[5] = 0x08;   /* 000000000 00001000 = 8 Register = 1 Bytes */
	
		uint16_t u16CRC = LVQ_CRC16(TXData, 6);
	
		TXData[6] = u16CRC & 0xFF ;            /* CRC LOW */
	  TXData[7] = ( u16CRC >> 8) & 0xFF ;    /* CRC HIGH */
	
		LVQ_GPIO_SetPinHigh(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
	
		LVQ_USART_Send(MODBUS_RTU_Info.USART_MODBUS_RTU, TXData, 8);
	
		Delay_us_systick(120);
	
		LVQ_GPIO_SetPinLow(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
}

void Function_Read_Input_Coils(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info)
{
		uint8_t TXData[8];
	
		TXData[0] = 0x05;   /* Slave Address */
		TXData[1] = 0x02;   /* Functon code */
	
		TXData[2] = 0;      /* Offset = 10001 */
	  TXData[3] = 0x00;   /* 0000000 00000000 = Offset + 1 = 10001 */
	
		TXData[4] = 0;
	  TXData[5] = 0x0A;   /* 000000000 00001010 = 10 coils = 2 Bytes */
	
		uint16_t u16CRC = LVQ_CRC16(TXData, 6);
	
		TXData[6] = u16CRC & 0xFF ;            /* CRC LOW */
	  TXData[7] = ( u16CRC >> 8) & 0xFF ;    /* CRC HIGH */
	
		LVQ_GPIO_SetPinHigh(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
	
		LVQ_USART_Send(MODBUS_RTU_Info.USART_MODBUS_RTU, TXData, 8);
	
		Delay_us_systick(120);
	
		LVQ_GPIO_SetPinLow(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
}

void Function_Write_Single_Coil(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info)
{
		uint8_t TXData[8];
	
		TXData[0] = 0x05;   /* Slave Address */
		TXData[1] = 0x05;   /* Functon code */
	
		TXData[2] = 0;      /* Offset = 10001 */
	  TXData[3] = 0x00;   /* 0000000 00000000 = Offset + 1 = 10001 */
	
		TXData[4] = 0x00;
		TXData[5] = 0x00;   /* ON : FF 00 , OFF : 00 00 */
	
		uint16_t u16CRC = LVQ_CRC16(TXData, 6);
	
		TXData[6] = u16CRC & 0xFF ;            /* CRC LOW */
	  TXData[7] = ( u16CRC >> 8) & 0xFF ;    /* CRC HIGH */
	
		LVQ_GPIO_SetPinHigh(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
	
		LVQ_USART_Send(MODBUS_RTU_Info.USART_MODBUS_RTU, TXData, 8);
	
		Delay_us_systick(120);
	
		LVQ_GPIO_SetPinLow(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
}

void Function_Write_Single_Register(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info)
{
		uint8_t TXData[8];
	
		TXData[0] = 0x05;   /* Slave Address */
		TXData[1] = 0x06;   /* Functon code */
	
		TXData[2] = 0;      /*  */
	  TXData[3] = 0x05;   /* 40001 + TXData[3] = ... */
	
		TXData[4] = 0x12;
		TXData[5] = 0x34;   /* ON : FF 00 , OFF : 00 00 */
	
		uint16_t u16CRC = LVQ_CRC16(TXData, 6);
	
		TXData[6] = u16CRC & 0xFF ;            /* CRC LOW */
	  TXData[7] = ( u16CRC >> 8) & 0xFF ;    /* CRC HIGH */
	
		LVQ_GPIO_SetPinHigh(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
	
		LVQ_USART_Send(MODBUS_RTU_Info.USART_MODBUS_RTU, TXData, 8);
	
		Delay_us_systick(120);
	
		LVQ_GPIO_SetPinLow(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
}

void Function_Write_Coils(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info)
{
		uint8_t TXData[11];
	
		TXData[0] = 0x05;   /* Slave Address */
		TXData[1] = 0x0F;   /* Functon code : Colis */
	
		TXData[2] = 0;      /*  */
	  TXData[3] = 0x03;   /* 1 + TXData[3] = ... */
	
		TXData[4] = 0x00;
		TXData[5] = 0x0F;   /* 15 Coils */
	
		TXData[6] = 0x02;   /* 2 Bytes */
	
		TXData[7] = 0xAA;   /* 1 - 8 Coil */
		TXData[8] = 0x03;   /* 9 - 16 Coil */

		uint16_t u16CRC = LVQ_CRC16(TXData, 9);
	
		TXData[9] = u16CRC & 0xFF ;            /* CRC LOW */
	  TXData[10] = ( u16CRC >> 8) & 0xFF ;    /* CRC HIGH */
	
		LVQ_GPIO_SetPinHigh(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
	
		LVQ_USART_Send(MODBUS_RTU_Info.USART_MODBUS_RTU, TXData, 11);
	
		Delay_us_systick(120);
	
		LVQ_GPIO_SetPinLow(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
}

void Function_Write_Holding_Registers(LVQ_MODBUS_RTU_Info_t MODBUS_RTU_Info)
{
		uint8_t TXData[15];
	
		TXData[0] = 0x05;   /* Slave Address */
		TXData[1] = 16;   /* Functon code : Colis */
	
		TXData[2] = 0;      /*  */
	  TXData[3] = 0x00;   /* 40001 + TXData[3] = ... */
	
		TXData[4] = 0x00;
		TXData[5] = 0x03;   /*  */
	
		TXData[6] = 0x06;   /* 6 Bytes */
	
		TXData[7] = 0x12;   
		TXData[8] = 0x34;  
	
		TXData[9] = 0x56;   
		TXData[10] = 0x78;  
		
		TXData[11] = 0x90;   
		TXData[12] = 0xAB;   

		uint16_t u16CRC = LVQ_CRC16(TXData, 13);
	
		TXData[13] = u16CRC & 0xFF ;            /* CRC LOW */
	  TXData[14] = ( u16CRC >> 8) & 0xFF ;    /* CRC HIGH */
	
		LVQ_GPIO_SetPinHigh(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
	
		LVQ_USART_Send(MODBUS_RTU_Info.USART_MODBUS_RTU, TXData, 15);
	
		Delay_us_systick(120);
	
		LVQ_GPIO_SetPinLow(MODBUS_RTU_Info.GPIO_Port_MODBUS_RTU_EN, MODBUS_RTU_Info.GPIO_Pin_MODBUS_RTU_EN);
}

uint16_t LVQ_CRC16(uint8_t* buffer, uint16_t buffer_length)
{
		uint8_t u8CRCHi = 0xFF ;                  /* high byte of CRC initialized */
		uint8_t u8CRCLo = 0xFF ;                  /* low byte of CRC initialized */
		unsigned int uIndex ;                     /* will index into CRC lookup table */
	
		while (buffer_length--)                   /* pass through message buffer */
		{
				uIndex = u8CRCLo ^ *buffer++ ;        /* calculate the CRC */
				u8CRCLo = u8CRCHi ^ auchCRCHi[uIndex] ;
				u8CRCHi = auchCRCLo[uIndex] ;
		}
		return (u8CRCHi << 8 | u8CRCLo) ;
}
