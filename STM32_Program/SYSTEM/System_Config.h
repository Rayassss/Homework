/**
  ******************************************************************************
  * @file    System_Config.h
  * @author  Acme-LaoZhu
  * @version V1.0.0
  * @date    25-March-2019
  * @brief  This file is used to Config System Parameters
  * @License:GNU General Public License v3.0         
  ******************************************************************************
**/
#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_
#include "stm32f10x.h"
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输?
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 ?
void Pre_Sleep(void);
void After_Sleep(void);
void Delayms(u16 xms);
void Delay_Init(void);
void Stm32_Clock_Init(void);
void Delayus(u32 xus);
#endif
