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
static u32 Facus = 0;							//us��ʱ������
static u16 Facms = 0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��
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
	Facus = SYSCLK;						    											//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	Reload = SYSCLK;					       										//ÿ���ӵļ������� ��λΪK	   
	Reload *= 1000000 / configTICK_RATE_HZ;							//����configTICK_RATE_HZ�趨���ʱ��
	Facms = 1000 / configTICK_RATE_HZ;									//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;					//����SYSTICK�ж�
	SysTick->LOAD = Reload; 														//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 					//����SYSTICK
}
void Delayms(u16 nms)
{	
	if(xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)	//���OS�Ѿ�������    
	{		 
		if(nms >= Facms)																		//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			vTaskDelay(nms / Facms);												//OS��ʱ
		}
		nms %= Facms;																				//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	else
		Delayus((u32)(nms*1000));															//��ͨ��ʽ��ʱ
}
void Delayus(u32 nus)
{		
	u32 Ticks;
	u32 Time_Old , Time_Now , Time_Count=0;
	u32 Reload = SysTick->LOAD;								//LOAD��ֵ	    	 
	Ticks = nus * Facus; 											//��Ҫ�Ľ����� 
	Time_Old = SysTick->VAL;        					//�ս���ʱ�ļ�����ֵ
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
				break;															//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	}									    
}
void Stm32_Clock_Init(void)
{
	unsigned char temp=0;
	u8 timeout=0;
	RCC_DeInit();
	RCC->CR|=0x00010000;  //�ⲿ����ʱ��ʹ��HSEON
	timeout=0;
	while(!(RCC->CR>>17)&&timeout<200)timeout++;//�ȴ��ⲿʱ�Ӿ���	   
	FLASH->ACR|=0x32;//FLASH 2����ʱ����
	RCC->CFGR|=0X001D2400;//APB1/2=DIV2;AHB=DIV1;PLL=9*CLK;HSE��ΪPLLʱ��Դ
	RCC->CR|=0x01000000;  //PLLON
	timeout=0;
	while(!(RCC->CR>>25)&&timeout<200)timeout++;//�ȴ�PLL����
	RCC->CFGR|=0x00000002;//PLL��Ϊϵͳʱ��
	while(temp!=0x02&&timeout<200)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
	{   
		temp=RCC->CFGR>>2;
		timeout++;
		temp&=0x03;
	}  
}
