/**
  ******************************************************************************

  MODBUS RTU SLAVE For STM32F446RE
  Author:   LVQ
  Updated:  8 January 2023
	Algorithm: ...

  ******************************************************************************
*/

#include "lvq_stm32f4_modbus_rtu_slave.h"

static uint16_t Holding_Registers_Database[50] = {
		0000,  1111,  2222,  3333,  4444,  5555,  6666,  7777,  8888,  9999,   // 0-9   40001-40010
		12345, 15432, 15535, 10234, 19876, 13579, 10293, 19827, 13456, 14567,  // 10-19 40011-40020
		21345, 22345, 24567, 25678, 26789, 24680, 20394, 29384, 26937, 27654,  // 20-29 40021-40030
		31245, 31456, 34567, 35678, 36789, 37890, 30948, 34958, 35867, 36092,  // 30-39 40031-40040
		45678, 46789, 47890, 41235, 42356, 43567, 40596, 49586, 48765, 41029,  // 40-49 40041-40050
};

static uint16_t Input_Registers_Database[50] = {
		0000,  1111,  2222,  3333,  4444,  5555,  6666,  7777,  8888,  9999,   // 0-9   30001-30010
		12345, 15432, 15535, 10234, 19876, 13579, 10293, 19827, 13456, 14567,  // 10-19 30011-30020
		21345, 22345, 24567, 25678, 26789, 24680, 20394, 29384, 26937, 27654,  // 20-29 30021-30030
		31245, 31456, 34567, 35678, 36789, 37890, 30948, 34958, 35867, 36092,  // 30-39 30031-30040
		45678, 46789, 47890, 41235, 42356, 43567, 40596, 49586, 48765, 41029,  // 40-49 30041-30050
};

static uint8_t Coils_Database[25] = {
	 /* 0b01001001, 0b10011100, 0b10101010, 0b01010101, 0b11001100 */
		0x49        , 0x9C      , 0xAA      , 0x55      , 0xCC    ,    // 0-39    1-40
		0x49        , 0x9C      , 0xAA      , 0x55      , 0xCC    ,    // 40-79   41-80
		0x49        , 0x9C      , 0xAA      , 0x55      , 0xCC    ,    // 80-119  81-120
		0x49        , 0x9C      , 0xAA      , 0x55      , 0xCC    ,    // 120-159 121-160
		0x49        , 0x9C      , 0xAA      , 0x55      , 0xCC    ,    // 160-199 161-200
};

static const uint8_t Inputs_Database[25] = {
	 /* 0b01000110, 0b10011010, 0b01010101, 0b10101010, 0b01101001 */
		0x46        , 0x9A      , 0x55      , 0xAA      , 0x69    ,    // 0-39    10001-10040
		0x46        , 0x9A      , 0x55      , 0xAA      , 0x69    ,    // 40-79   10041-10080
		0x46        , 0x9A      , 0x55      , 0xAA      , 0x69    ,    // 80-119  10081-10120
		0x46        , 0x9A      , 0x55      , 0xAA      , 0x69    ,    // 120-159 10121-10160
		0x46        , 0x9A      , 0x55      , 0xAA      , 0x69    ,    // 160-199 10161-10200
};

extern uint8_t u8RXData[256];
extern uint8_t u8TXData[256];
extern LVQ_USART_Data_RXTX_t LVQ_Usart3;

void LVQ_MODBUS_RTU_Sendata(uint8_t *data, int size)
{
		/* we will calculate the CRC in this function itself */
		uint16_t u16CRC = LVQ_CRC16(data, size);
		data[size] = u16CRC & 0xFF;             /* CRC LOW */
		data[size+1] = ( u16CRC >> 8 ) & 0xFF;  /* CRC HIGH */

		LVQ_USART_Send(LVQ_Usart3.Instance, data, size + 2);
}

void LVQ_MODBUS_RTU_Exception(uint8_t u8Exceptioncode)
{
		/* | SLAVE_ID | FUNCTION_CODE | Exception code | CRC     | */
		/* | 1 BYTE   |    1 BYTE     |     1 BYTE     | 2 BYTES | */

		u8TXData[0] = u8RXData[0];            /* Slave ID */
		u8TXData[1] = u8RXData[1]|0x80;       /* Adding 1 to the MSB of the function code */
		u8TXData[2] = u8Exceptioncode;        /* Load the Exception code */
		LVQ_MODBUS_RTU_Sendata(u8TXData, 3);  /* send Data... CRC will be calculated in the function */
}

uint8_t Function_Slave_Read_Holding_Registers(void)
{
		uint16_t u16startAddr = ( ( u8RXData[2] << 8 ) | u8RXData[3] );  /* start Register Address */
		uint16_t u16numRegs = ( (u8RXData[4] << 8 ) | u8RXData[5] );     /* number to registers master has requested */
		 
		if ( (u16numRegs <1 ) || ( u16numRegs > 125) )                   /* maximum no. of Registers as per the PDF */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_VALUE);                  /* send an exception */
			return 0;
		}

		uint16_t u16endAddr = u16startAddr + u16numRegs - 1;                /* end Register */
		if ( u16endAddr > 49 )                                              /* end Register can not be more than 49 as we only have record of 50 Registers in total */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_ADDRESS);                /* send an exception */
			return 0;
		}

		/* Prepare u8TXData buffer */
 
		/*| SLAVE_ID | FUNCTION_CODE | BYTE COUNT | DATA      | CRC     | */
		/*| 1 BYTE   |  1 BYTE       |  1 BYTE    | N*2 BYTES | 2 BYTES | */

		u8TXData[0] = SLAVE_ID;                                          /* slave ID */
		u8TXData[1] = u8RXData[1];                                       /* function code */
		u8TXData[2] = u16numRegs*2;                                      /* Byte count */
		uint8_t u8Index = 3;                                             /* we need to keep track of how many bytes has been stored in u8TXData Buffer */

		for ( int i = 0; i < u16numRegs; i++)                            /* Load the actual data into u8TXData buffer */
		{
			u8TXData[u8Index++] = ( Holding_Registers_Database[u16startAddr] >> 8 ) & 0xFF;  /* extract the higher byte */
			u8TXData[u8Index++] = ( Holding_Registers_Database[u16startAddr] ) & 0xFF;       /* extract the lower byte */
			u16startAddr++;                                                /* increment the register address */
		}

		LVQ_MODBUS_RTU_Sendata(u8TXData, u8Index);                        /* send data... CRC will be calculated in the function itself */
		return 1;                                                         /* success */
}

uint8_t Function_Slave_Read_Input_Registers(void)
{
		uint16_t u16startAddr = ( ( u8RXData[2] << 8 ) | u8RXData[3] );  /*  start Register Address */
		uint16_t u16numRegs = ( ( u8RXData[4] << 8) | u8RXData[5] );     /*  number to registers master has requested */
		
		if ( ( u16numRegs < 1) || ( u16numRegs > 125 ) )                 /*  maximum no. of Registers as per the PDF */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_VALUE);                  /*  send an exception */
			return 0;
		}

		uint16_t u16endAddr = u16startAddr + u16numRegs - 1;                /*  end Register */
		if ( u16endAddr > 49 )                                                /*  end Register can not be more than 49 as we only have record of 50 Registers in total */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_ADDRESS);                /*  send an exception */
			return 0;
		}

		/*  Prepare u8TXData buffer */

		/* | SLAVE_ID | FUNCTION_CODE | BYTE COUNT | DATA      | CRC     | */
		/* | 1 BYTE   |  1 BYTE       |  1 BYTE    | N*2 BYTES | 2 BYTES | */

		u8TXData[0] = SLAVE_ID;                                          /* slave ID */
		u8TXData[1] = u8RXData[1];                                       /* function code */
		u8TXData[2] = u16numRegs*2;                                      /* Byte count */
		uint8_t u8Index = 3;                                             /* we need to keep track of how many bytes has been stored in u8TXData Buffer */

		for ( int i = 0; i < u16numRegs; i++ )                           /* Load the actual data into u8TXData buffer */
		{
			u8TXData[u8Index++] = ( Input_Registers_Database[u16startAddr] >> 8 ) & 0xFF;  /* extract the higher byte */
			u8TXData[u8Index++] = ( Input_Registers_Database[u16startAddr] ) & 0xFF;       /* extract the lower byte */
			u16startAddr++;                                                /* increment the register address */
		}

		LVQ_MODBUS_RTU_Sendata(u8TXData, u8Index);                       /* send data... CRC will be calculated in the function itself */
		return 1;                                                        /* success */
}

uint8_t Function_Slave_Read_Coils(void)
{
		uint16_t u16startAddr = ( ( u8RXData[2] << 8 ) | u8RXData[3] );  /* start Coil Address */
		uint16_t u16numCoils = ( ( u8RXData[4] << 8 ) | u8RXData[5] );   /* number to coils master has requested */
		
		if ( ( u16numCoils < 1 ) || ( u16numCoils > 2000 ) )             /* maximum no. of coils as per the PDF */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_VALUE);                  /* send an exception */
			return 0;
		}

		uint16_t u16endAddr = u16startAddr + u16numCoils - 1;            /* Last coils address */
		if ( u16endAddr > 199)                                           /* end coil can not be more than 199 as we only have record of 200 (0-199) coils in total */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_ADDRESS);                /* send an exception */
			return 0;
		}

		/* reset u8TXData buffer */
		memset (u8TXData, '\0', 256);

		/* Prepare u8TXData buffer */

		/* | SLAVE_ID | FUNCTION_CODE | BYTE COUNT | DATA      | CRC     | */
		/* | 1 BYTE   |  1 BYTE       |  1 BYTE    | N*2 BYTES | 2 BYTES | */

		u8TXData[0] = SLAVE_ID;                                                   /* slave ID */
		u8TXData[1] = u8RXData[1];                                                /* function code */
		u8TXData[2] = ( u16numCoils / 8 ) + ( ( u16numCoils % 8 ) > 0 ? 1 : 0 );  /* Byte count */
		uint8_t u8Index = 3;                                                      /* we need to keep track of how many bytes has been stored in u8TXData Buffer */

		/* The approach is simple. We will read 1 bit at a time and store them in the u8TXData buffer.
		 * First find the offset in the first byte we read from, for eg- if the start coil is 13,
		 * we will read from database[1] with an offset of 5. This bit will be stored in the u8TXData[0] at 0th position.
		 * Then we will keep shifting the database[1] to the right and read the bits.
		 * Once the u16bitPosition has crossed the value 7, we will increment the u16startByte
		 * When the u8IndexPosition exceeds 7, we increment the u8Index variable, so to copy into the next byte of the u8TXData
		 * This keeps going until the number of coils required have been copied
		 */
		 
		uint16_t u16startByte = u16startAddr / 8;                        /* which byte we have to start extracting the data from */
		uint16_t u16bitPosition = u16startAddr % 8;                      /* The shift position in the first byte */
		uint8_t u8IndexPosition = 0;                                     /* The shift position in the current u8Index of the u8TXData buffer */

		/* Load the actual data into u8TXData buffer */
		for (uint8_t i = 0; i < u16numCoils; i++)
		{
			u8TXData[u8Index] |= ((Coils_Database[u16startByte] >> u16bitPosition) & 0x01) << u8IndexPosition;
			u8IndexPosition++; 
			u16bitPosition++;
			if ( u8IndexPosition > 7 )                                     /* if the u8IndexPosition exceeds 7, we have to copy the data into the next byte position */
			{
				u8IndexPosition = 0;
				u8Index++;
			}
			if ( u16bitPosition > 7 )                                      /* if the u16bitPosition exceeds 7, we have to increment the u16startByte */
			{
				u16bitPosition = 0;
				u16startByte++; 
			}
		}

		if ( u16numCoils % 8 != 0) u8Index++;                            /* increment the u8Index variable, only if the u16numCoils is not a multiple of 8 */
		LVQ_MODBUS_RTU_Sendata(u8TXData, u8Index);                       /* send data... CRC will be calculated in the function itself */
		return 1;                                                        /* success */
}

uint8_t Function_Slave_Read_Inputs(void)
{
		uint16_t u16startAddr = ( ( u8RXData[2] << 8 ) | u8RXData[3] );  /* start Register Address */
		uint16_t u16numCoils = ( ( u8RXData[4] << 8 ) | u8RXData[5] );   /* number to coils master has requested */
		
		if ( ( u16numCoils < 1 ) || ( u16numCoils > 2000 ))              /* maximum no. of coils as per the PDF */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_VALUE);                 /* send an exception */
			return 0; 
		}

		uint16_t u16endAddr = u16startAddr + u16numCoils - 1;            /* Last coils address */
		if ( u16endAddr > 199 )                                          /* end coil can not be more than 199 as we only have record of 200 (0-199) coils in total */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_ADDRESS);                /* send an exception */
			return 0;
		}

		/* reset u8TXData buffer */
		memset (u8TXData, '\0', 256);

		/* Prepare u8TXData buffer */

		/* | SLAVE_ID | FUNCTION_CODE | BYTE COUNT | DATA      | CRC     | */
		/* | 1 BYTE   |  1 BYTE       |  1 BYTE    | N*2 BYTES | 2 BYTES | */

		u8TXData[0] = SLAVE_ID;                                                  /* slave ID */
		u8TXData[1] = u8RXData[1];                                               /* function code */
		u8TXData[2] = ( u16numCoils / 8 ) + ( ( u16numCoils % 8 ) > 0 ? 1 : 0);  /* Byte count */
		uint8_t u8Index = 3;                                                     /* we need to keep track of how many bytes has been stored in u8TXData Buffer */

		/* The approach is simple. We will read 1 bit at a time and store them in the u8TXData buffer.
		 * First find the offset in the first byte we read from, for eg- if the start coil is 13,
		 * we will read from database[1] with an offset of 5. This bit will be stored in the u8TXData[0] at 0th position.
		 * Then we will keep shifting the database[1] to the right and read the bits.
		 * Once the u16bitPosition has crossed the value 7, we will increment the u16startByte
		 * When the u8IndexPosition exceeds 7, we increment the u8Index variable, so to copy into the next byte of the u8TXData
		 * This keeps going until the number of coils required have been copied
		 */
		uint16_t u16startByte = u16startAddr / 8;                        /* which byte we have to start extracting the data from */
		uint16_t u16bitPosition = u16startAddr % 8;                      /* The shift position in the first byte */
		uint8_t u8IndexPosition = 0;                                     /* The shift position in the current u8Index of the u8TXData buffer */

		/* Load the actual data into u8TXData buffer */
		for (uint8_t i = 0; i < u16numCoils; i++)
		{
			u8TXData[u8Index] |= ( ( Inputs_Database[u16startByte] >> u16bitPosition) & 0x01 ) << u8IndexPosition;
			u8IndexPosition++; 
			u16bitPosition++;
			if ( u8IndexPosition > 7 )                                     /* if the u8IndexPosition exceeds 7, we have to copy the data into the next byte position */
			{
				u8IndexPosition = 0;
				u8Index++;
			}
			if ( u16bitPosition > 7 )                                      /* if the u16bitPosition exceeds 7, we have to increment the u16startByte */
			{
				u16bitPosition=0;
				u16startByte++;
			}
		}

		if ( u16numCoils % 8 != 0) u8Index++;                            /* increment the u8Index variable, only if the u16numCoils is not a multiple of 8 */
		LVQ_MODBUS_RTU_Sendata(u8TXData, u8Index);                       /* send data... CRC will be calculated in the function itself */
		return 1;                                                        /* success */
}

uint8_t Function_Slave_Write_Single_Register(void)
{
		uint16_t u16startAddr = ( ( u8RXData[2] << 8 ) | u8RXData[3] );  /* start Register Address */

		if ( u16startAddr > 49 )                                         /* The Register Address can not be more than 49 as we only have record of 50 Registers in total */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_ADDRESS);                /* send an exception */
			return 0;
		}

		Holding_Registers_Database[u16startAddr] = ( u8RXData[4] << 8 ) | u8RXData[5];

		/* Prepare Response */

		/* | SLAVE_ID | FUNCTION_CODE | Start Addr |      Data      | CRC     | */
		/* | 1 BYTE   |  1 BYTE       |  2 BYTE    | 2 BYTES        | 2 BYTES | */
		
		u8TXData[0] = SLAVE_ID;                /* slave ID */
		u8TXData[1] = u8RXData[1];             /* function code */
		u8TXData[2] = u8RXData[2];             /* Start Addr HIGH Byte */
		u8TXData[3] = u8RXData[3];             /* Start Addr LOW Byte */
		u8TXData[4] = u8RXData[4];             /* Reg Data HIGH Byte */
		u8TXData[5] = u8RXData[5];             /* Reg Data LOW  Byte */
						
		LVQ_MODBUS_RTU_Sendata(u8TXData, 6);   /* send data... CRC will be calculated in the function itself */
		return 1;                              /* success */
}

uint8_t Function_Slave_Write_Holding_Registers(void)
{
		uint16_t u16startAddr = ( ( u8RXData[2] << 8 ) | u8RXData[3] );  /* start Register Address */
		uint8_t u8numRegs = ( ( u8RXData[4] << 8 ) | u8RXData[5] );      /* number to registers master has requested */
		
		if ( ( u8numRegs < 1) || ( u8numRegs > 123 ) )                   /* maximum no. of Registers as per the PDF */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_VALUE);                  /* send an exception */
			return 0;
		}

		uint16_t u16endAddr = u16startAddr + u8numRegs - 1;              /* end Register */
		if ( u16endAddr > 49 )                                           /* end Register can not be more than 49 as we only have record of 50 Registers in total */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_ADDRESS);                /* send an exception */
			return 0;
		}

		uint8_t u8Index = 7;                                             /* we need to keep track of index in u8RXData */
		for (uint8_t i = 0; i < u8numRegs; i++)
		{
			 Holding_Registers_Database[u16startAddr] = ( u8RXData[u8Index] << 8) | u8RXData[u8Index + 1];
			 u16startAddr++;
			 u8Index += 2;
		}

		/* Prepare Response */

		/* | SLAVE_ID | FUNCTION_CODE | Start Addr | num of Regs    | CRC     | */
		/* | 1 BYTE   |  1 BYTE       |  2 BYTE    | 2 BYTES        | 2 BYTES | */

		u8TXData[0] = SLAVE_ID;                /* slave ID */
		u8TXData[1] = u8RXData[1];             /* function code */
		u8TXData[2] = u8RXData[2];             /* Start Addr HIGH Byte */
		u8TXData[3] = u8RXData[3];             /* Start Addr LOW Byte */
		u8TXData[4] = u8RXData[4];             /* num of Regs HIGH Byte */
		u8TXData[5] = u8RXData[5];             /* num of Regs LOW Byte */

		LVQ_MODBUS_RTU_Sendata(u8TXData, 6);   /* send data... CRC will be calculated in the function itself */
		return 1;                              /* success */
}

uint8_t Function_Slave_Write_Single_Coil(void)
{
		uint16_t u16startAddr = ( ( u8RXData[2] << 8 ) | u8RXData[3] );  /* start Register Address */

		if ( u16startAddr > 199 )                                        /* The Coil Address can not be more than 199 as we only have record of 200 Coils in total */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_VALUE);                  /* send an exception */
			return 0;
		}

		/* Calculation for the bit in the database, where the modification will be done */
		uint16_t u16startByte = u16startAddr / 8;                        /* which byte we have to start extracting the data from */
		uint16_t u16bitPosition = u16startAddr % 8;                      /* The shift position in the first byte */
		
		/* The next 2 bytes in the RxData determines the state of the coil
		 * A value of FF 00 hex requests the coil to be ON.
		 * A value of 00 00 requests it to be OFF.
		 * All other values are illegal and will not affect the coil.
		 */
		
		if ( ( u8RXData[4] == 0xFF ) && ( u8RXData[5] == 0x00 ) )
		{
			  Coils_Database[u16startByte] |= 1 << u16bitPosition;         /* Replace that bit with 1 */
		}

		else if ( ( u8RXData[4] == 0x00) && ( u8RXData[5] == 0x00 ) )
		{
				Coils_Database[u16startByte] &= ~( 1 << u16bitPosition );    /* Replace that bit with 0 */
		}
		
		/* Prepare Response */

		/* | SLAVE_ID | FUNCTION_CODE | Start Addr | Coil Data      | CRC     | */
		/* | 1 BYTE   |  1 BYTE       |  2 BYTE    | 2 BYTES        | 2 BYTES | */

		u8TXData[0] = SLAVE_ID;                /* slave ID */
		u8TXData[1] = u8RXData[1];             /* function code */
		u8TXData[2] = u8RXData[2];             /* Start Addr HIGH Byte */
		u8TXData[3] = u8RXData[3];             /* Start Addr LOW Byte */
		u8TXData[4] = u8RXData[4];             /* Coil Data HIGH Byte */
		u8TXData[5] = u8RXData[5];             /* Coil Data LOW  Byte */

		LVQ_MODBUS_RTU_Sendata(u8TXData, 6);   /* send data... CRC will be calculated in the function itself */
		return 1;                              /* success */
}

uint8_t Function_Slave_Write_MultiCoils(void)
{
		uint16_t u16startAddr = ( ( u8RXData[2] << 8 ) | u8RXData[3] );  /* start Register Address */
		uint16_t u16numCoils  = ( ( u8RXData[4] << 8 ) | u8RXData[5] );  /* number to registers master has requested */
		
		if ( ( u16numCoils < 1) || ( u16numCoils > 1968 ) )              /* maximum no. of Registers as per the PDF */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_VALUE);                  /* send an exception */
			return 0;
		}

		uint16_t u16endAddr = u16startAddr + u16numCoils - 1;            /* end Register */
		if ( u16endAddr > 119 )                                          /* end Register can not be more than 49 as we only have record of 50 Registers in total */
		{
			LVQ_MODBUS_RTU_Exception(ILLEGAL_DATA_ADDRESS);                /* send an exception */
			return 0;
		}
		
		uint8_t u8Index = 7;
		uint16_t u16startByte = u16startAddr / 8;                        /* which byte we have to start extracting the data from */
		uint16_t u16bitPosition = u16startAddr % 8;                      /* The shift position in the first byte */
		uint8_t u8IndexPosition = 0;                                     /* The shift position in the current u8Index of the u8TXData buffer */

		/* Load the actual data into u8TXData buffer */
		for (uint8_t i = 0; i < u16numCoils; i++)
		{
				if ( ( ( u8RXData[u8Index] >> u8IndexPosition) & 0x01 ) == 1)
				{
						Coils_Database[u16startByte] |= 1 << u16bitPosition;     /* replace that bit with 1 */
				}
				else
				{
						Coils_Database[u16startByte] &= ~(1 << u16bitPosition);  /* replace that bit with 0 */
				}
				u8IndexPosition++; 
				u16bitPosition++;
				if ( u8IndexPosition > 7 )                                   /* if the u8IndexPosition exceeds 7, we have to copy the data into the next byte position */
				{
					u8IndexPosition = 0;
					u8Index++;
				}
				if ( u16bitPosition > 7 )                                    /* if the u16bitPosition exceeds 7, we have to increment the u16startByte */
				{
					u16bitPosition = 0;
					u16startByte++;
				}
		}
		
		/* Prepare Response */

		/* | SLAVE_ID | FUNCTION_CODE | Start Addr | Num Coil       | CRC     | */
		/* | 1 BYTE   |  1 BYTE       |  2 BYTE    | 2 BYTES        | 2 BYTES | */

		u8TXData[0] = SLAVE_ID;                /* slave ID */
		u8TXData[1] = u8RXData[1];             /* function code */
		u8TXData[2] = u8RXData[2];             /* Start Addr HIGH Byte */
		u8TXData[3] = u8RXData[3];             /* Start Addr LOW Byte */
		u8TXData[4] = u8RXData[4];             /* Num of coils HIGH Byte */
		u8TXData[5] = u8RXData[5];             /* Num of coils LOW  Byte */

		LVQ_MODBUS_RTU_Sendata(u8TXData, 6);   /* send data... CRC will be calculated in the function itself */
		return 1;                              /* success */
}
