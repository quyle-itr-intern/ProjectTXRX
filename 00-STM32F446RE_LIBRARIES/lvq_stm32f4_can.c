/**
  ******************************************************************************

  CAN For STM32F446RE
  Author:   LVQ
  Updated:  12st August 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_can.h"

uint16_t CAN1_ID;
uint8_t CAN1_DATA0,CAN1_DATA1,CAN1_DATA2,CAN1_DATA3,CAN1_DATA4,CAN1_DATA5,CAN1_DATA6,CAN1_DATA7;
CanTxMsg TxMessage;
uint8_t CanFlag = DISABLE;

void CANx_Init(void)
{
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	/* CAN GPIOs configuration **************************************************/
		/* Configure CAN RX and TX pins */
	LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_11, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Fast, GPIO_AF_CAN1);
	LVQ_GPIO_InitAlternate(GPIOA, GPIO_PIN_12, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Fast, GPIO_AF_CAN1);

	/* CAN configuration ********************************************************/
	
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	/* CAN Baudrate = 500KBps (CAN clocked at 45 MHz) */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler = 18;

	CAN_Init(CAN1, &CAN_InitStructure);

	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x00005;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Transmit Structure preparation */
	TxMessage.StdId = 0x446;
	TxMessage.ExtId = 0x000;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 2;
	
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void CANxWriteData(uint16_t ID)
{
	CanTxMsg TxMessage;

	CAN1_DATA0 = 50; CAN1_DATA1 = 50;
	CAN1_DATA2 = rand()%0xff; CAN1_DATA3=rand()%0xff;
	CAN1_DATA4 = rand()%0xff; CAN1_DATA5=rand()%0xff;
	CAN1_DATA6 = rand()%0xff; CAN1_DATA7=rand()%0xff;
		
	/* transmit */
	TxMessage.StdId = ID;
	TxMessage.ExtId = 0x000;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 2;
	TxMessage.Data[0] = CAN1_DATA0;
	TxMessage.Data[1] = CAN1_DATA1;
	TxMessage.Data[2] = CAN1_DATA2;
	TxMessage.Data[3] = CAN1_DATA3;
	TxMessage.Data[4] = CAN1_DATA4;
	TxMessage.Data[5] = CAN1_DATA5;
	TxMessage.Data[6] = CAN1_DATA6;
	TxMessage.Data[7] = CAN1_DATA7;
	CAN_Transmit(CAN1,&TxMessage);
	CAN_CancelTransmit(CAN1, 0);
}

/**
* @brief This function handles CAN1 RX0 request.
* @param None
* @retval None
*/
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
	CAN1_ID=RxMessage.StdId;
	CAN1_DATA0=RxMessage.Data[0];
	CAN1_DATA1=RxMessage.Data[1];
	CAN1_DATA2=RxMessage.Data[2];
	CAN1_DATA3=RxMessage.Data[3];
	CAN1_DATA4=RxMessage.Data[4];
	CAN1_DATA5=RxMessage.Data[5];
	CAN1_DATA6=RxMessage.Data[6];
	CAN1_DATA7=RxMessage.Data[7];
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	CanFlag = ENABLE;
}

/**
* @brief This function handles CAN1 RX0 request.
* @param None
* @retval None
*/
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	CAN_Receive(CAN2,CAN_FIFO0, &RxMessage);
	CAN1_ID=RxMessage.StdId;
	CAN1_DATA0=RxMessage.Data[0];
	CAN1_DATA1=RxMessage.Data[1];
	CAN1_DATA2=RxMessage.Data[2];
	CAN1_DATA3=RxMessage.Data[3];
	CAN1_DATA4=RxMessage.Data[4];
	CAN1_DATA5=RxMessage.Data[5];
	CAN1_DATA6=RxMessage.Data[6];
	CAN1_DATA7=RxMessage.Data[7];
	CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);
}

