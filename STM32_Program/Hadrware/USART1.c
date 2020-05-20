/**
  ******************************************************************************
  * @file    USART1.c
  * @author  Acme-LaoZhu
  * @version V1.0.0
  * @date    25-March-2019
  * @brief  This file is used to Config USART1 Periph
  * @License:GNU General Public License v3.0         
  ******************************************************************************
**/
#include "USART1.h"
#include "stdio.h"
#include "Var.h"
void USART1_IRQHandler(void)
{
	unsigned char Buffer;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Buffer = USART_ReceiveData(USART1);
		USART1_Receive(Buffer);
		USART_ClearFlag(USART1, USART_IT_RXNE);
  }
}
__weak void USART1_Receive(unsigned char BUFFER)
{
	
}
void USART1_Init(unsigned long baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);
}
void Send_Buf(USART_TypeDef *USARTT,unsigned char data)
{
	while((USARTT->SR&0X40)==0){}
    USARTT->DR = (u8) data;
}
#pragma import(__use_no_semihosting)
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;         
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){}
    USART1->DR = (u8) ch;
	return ch;
}
