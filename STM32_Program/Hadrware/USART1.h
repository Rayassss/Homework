/**
  ******************************************************************************
  * @file    USART1.h
  * @author  Acme-LaoZhu
  * @version V1.0.0
  * @date    25-March-2019
  * @brief  This file is used to Config USART1 Periph
  * @License:GNU General Public License v3.0         
  ******************************************************************************
**/
#ifndef _USART1_H_
#define _USART1_H_
#include "System_Config.h"
__weak void USART1_Receive(unsigned char BUFFER);
void USART1_IRQHandler(void);
void USART1_Init(unsigned long baud);
void Send_Buf(USART_TypeDef *USARTT,unsigned char data);
#endif
