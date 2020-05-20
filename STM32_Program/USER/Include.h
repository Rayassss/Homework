/**
  ******************************************************************************
  * @file    Include.h
  * @author  Acme-LaoZhu
  * @version V1.0.0
  * @date    25-March-2019
  * @brief  This file is used to Include Head File
  * @License:GNU General Public License v3.0         
  ******************************************************************************
**/
#ifndef _INCLUDE_H_
#define _INCLUDE_H_

///////////// System //////////////
#include "System_Config.h"
#include "stdio.h"
#include "Var.h"

///////////// FreeRTOS //////////////
#include "FreeRTOS.h"
#include "task.h"

///////////// HardWare //////////////
#include "USART2.h"
#include "USART1.h"
#include "LED.h"
#include "Timer.h"
#include "LCD.h"
#include "ADC.h"

///////////// APP //////////////
#include "DTH11.h"
#include "Graph.h"
#include "Encrypt.h"
#include "GUI.h"

void User_Task(void *pvParameters);
#endif
