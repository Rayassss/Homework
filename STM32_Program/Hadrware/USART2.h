/**
  ******************************************************************************
  * @file    USART2.h
  * @author  Acme-LaoZhu
  * @version V1.0.0
  * @date    25-March-2019
  * @brief  This file is used to Config USART2 Periph
  * @License:GNU General Public License v3.0         
  ******************************************************************************
**/
#ifndef _USART2_H_
#define _USART2_H_
void USART2_Init(unsigned long baud);
__weak void USART2_Receive(unsigned char BUFFER);
void USART2_IRQHandler(void);
#endif
