/**
  ******************************************************************************
  * @file    LED.c
  * @author  Acme-LaoZhu
  * @version V1.0.0
  * @date    25-March-2019
  * @brief  This file is used to Config LED 
  * @License:GNU General Public License v3.0         
  ******************************************************************************
**/
#include "System_Config.h"
#include "LED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Var.h"
TaskHandle_t LED_Flash_Task_Handler;
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  PWR_BackupAccessCmd( ENABLE );
  RCC_LSEConfig( RCC_LSE_OFF );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	PWR_BackupAccessCmd(DISABLE);
	
	PCout(14) = 1;
	xTaskCreate((TaskFunction_t)LED_Flash,
							(const char *)"LED_Flash",
							(uint16_t)64,
							(void *)NULL,
							(UBaseType_t)4,
							(TaskHandle_t *)LED_Flash_Task_Handler);
}
void LED_Flash(void *pvParameters)
{
	while(1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
		vTaskDelay(500/LED_Frequency);
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		vTaskDelay(500/LED_Frequency);
	}
}
