#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H
#include "stm32f10x.h"
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
#endif