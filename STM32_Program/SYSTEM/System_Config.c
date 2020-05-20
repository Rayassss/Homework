/**
  ******************************************************************************
  * @file    System_Config.c
  * @author  Acme-LaoZhu
  * @version V1.0.0
  * @date    25-March-2019
  * @brief  This file is used to Config System Parameters
  * @License:GNU General Public License v3.0         
  ******************************************************************************
**/
#include "System_Config.h"
#include "FreeRTOS.h"
#include "task.h"
static u32 Facus = 0;							//us延时倍乘数
static u16 Facms = 0;							//ms延时倍乘数,在os下,代表每个节拍的ms数
void Pre_Sleep(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,DISABLE);
}
void After_Sleep(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
}
void Delay_Init(void)
{
	u16 SYSCLK = 72;
	u32 Reload;
	Facus = SYSCLK;						    											//不论是否使用OS,fac_us都需要使用
	Reload = SYSCLK;					       										//每秒钟的计数次数 单位为K	   
	Reload *= 1000000 / configTICK_RATE_HZ;							//根据configTICK_RATE_HZ设定溢出时间
	Facms = 1000 / configTICK_RATE_HZ;									//代表OS可以延时的最少单位	   
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;					//开启SYSTICK中断
	SysTick->LOAD = Reload; 														//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 					//开启SYSTICK
}
void Delayms(u16 nms)
{	
	if(xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)	//如果OS已经在运行    
	{		 
		if(nms >= Facms)																		//延时的时间大于OS的最少时间周期 
		{ 
   			vTaskDelay(nms / Facms);												//OS延时
		}
		nms %= Facms;																				//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	else
		Delayus((u32)(nms*1000));															//普通方式延时
}
void Delayus(u32 nus)
{		
	u32 Ticks;
	u32 Time_Old , Time_Now , Time_Count=0;
	u32 Reload = SysTick->LOAD;								//LOAD的值	    	 
	Ticks = nus * Facus; 											//需要的节拍数 
	Time_Old = SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		Time_Now = SysTick->VAL;	
		if(Time_Now != Time_Old)
		{
			if(Time_Now < Time_Old)
				Time_Count += Time_Old - Time_Now;
			else
				Time_Count += Reload - Time_Now + Time_Old;	    
			Time_Old = Time_Now;
			if(Time_Count >= Ticks)
				break;															//时间超过/等于要延迟的时间,则退出.
		}
	}									    
}
void Stm32_Clock_Init(void)
{
	unsigned char temp=0;
	u8 timeout=0;
	RCC_DeInit();
	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
	timeout=0;
	while(!(RCC->CR>>17)&&timeout<200)timeout++;//等待外部时钟就绪	   
	FLASH->ACR|=0x32;//FLASH 2个延时周期
	RCC->CFGR|=0X001D2400;//APB1/2=DIV2;AHB=DIV1;PLL=9*CLK;HSE作为PLL时钟源
	RCC->CR|=0x01000000;  //PLLON
	timeout=0;
	while(!(RCC->CR>>25)&&timeout<200)timeout++;//等待PLL锁定
	RCC->CFGR|=0x00000002;//PLL作为系统时钟
	while(temp!=0x02&&timeout<200)     //等待PLL作为系统时钟设置成功
	{   
		temp=RCC->CFGR>>2;
		timeout++;
		temp&=0x03;
	}  
}
