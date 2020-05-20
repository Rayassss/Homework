/**
  ******************************************************************************
  * @file    main.c
  * @author  Acme-LaoZhu
  * @version V1.0.0
  * @date    25-March-2019
  * @brief  This file is used to Init All Periph
  * @License:GNU General Public License v3.0         
  ******************************************************************************
**/
#include "Include.h"
TaskHandle_t Initial_Task_Handler;
TaskHandle_t User_Task_Handler;
void Initial(void *pvParameters)
{
	taskENTER_CRITICAL();
	USART2_Init(USART2_Baudurate);
	LED_Init();
	USART1_Init(USART1_Baudurate);
	Delay_Init();
	Timer_Init(1000);
	Temp_Init();
	LCD_Init();
	GUI_Clear(0xffff);
	Graph_Init();
	xTaskCreate((TaskFunction_t)User_Task,
							(const char *)"User_Task",
							(uint16_t)User_Task_Depth,
							(void *)NULL,
							(UBaseType_t)1,
							(TaskHandle_t *)User_Task_Handler);
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
}
int main(void)
{
	Stm32_Clock_Init();
	xTaskCreate((TaskFunction_t)Initial,
							(const char *)"Initial_Task",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)1,
							(TaskHandle_t *)Initial_Task_Handler);
	vTaskStartScheduler();	
	while(1);
}
