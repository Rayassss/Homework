/**
  ******************************************************************************
  * @file    USER.c
  * @author  Acme-LaoZhu
  * @version V1.0.0
  * @date    25-March-2019
  * @brief  This file is used to Finish User Function
  * @License:GNU General Public License v3.0         
  ******************************************************************************
**/
#include "Include.h"
void User_Task(void *pvParameters)
{
	while(1)
	{
		Package Report_Package;
		unsigned char Buffer[5];
		unsigned int CPU_Temp = Get_Temp();
		DTH11_ReadData(Buffer);
		Report_Package.PID = 0;
		Report_Package.Data[0] = (((Buffer[0] + Buffer[1]) & 0xff00) >> 8);
		Report_Package.Data[1] = ((Buffer[0] + Buffer[1]) & 0xff00);
		Report_Package.Data[2] = ((CPU_Temp & 0xff00) >> 8);
		Report_Package.Data[3] = (CPU_Temp & 0xff00);
		Report_Package.Data[4] = (((Buffer[2] + Buffer[3]) & 0xff00) >> 8);
		Report_Package.Data[5] = ((Buffer[2] + Buffer[3]) & 0xff00);
		Send_Package(&Report_Package);
		Delayms(1000);
	}
}
