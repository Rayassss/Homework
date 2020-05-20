/**
  ******************************************************************************
  * @file    Var.h
  * @author  Acme-LaoZhu
  * @version V1.0.0
  * @date    25-March-2019
  * @brief  This file is used to Config Parameter
  * @License:GNU General Public License v3.0         
  ******************************************************************************
**/
#ifndef _VAR_H_
#define _VAR_H_
#define User_Task_Depth 128				//User函数堆栈大小
#define LED_Frequency 1           //LED指示灯闪烁频率
#define USART2_Priority 1					//串口2中断优先级
#define USART2_Baudurate 115200		//串口2波特率
#define USART1_Baudurate 115200   //串口1波特率
#define RTOS_Memory 8*1024  			//RTOS申请的内存大小
#endif
