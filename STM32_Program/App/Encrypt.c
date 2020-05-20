#include "Encrypt.h"
#include "USART1.h"
#include "string.h"
void UART_SendString(char *str,unsigned char num)
{
    while(num--)
    {
        Send_Buf(USART1,*str++);
    }
}
void Send_Package(Package *Pac)
{
	unsigned char cnt=0,Temp_Buf=0x00;
	unsigned char Transmit_Temp[12];
	memset(Transmit_Temp,0x00,sizeof(Transmit_Temp));
	Transmit_Temp[0] =  0xff;
	Temp_Buf = 0;
	for(cnt = 0;cnt<8;cnt++)
	{
		if(Pac->Data[cnt] == 0xff)
		{
			Transmit_Temp[(cnt + 2)] = 0x00;
			Temp_Buf = Temp_Buf | (1 << cnt);
		}
		else
		{
			Transmit_Temp[(cnt + 2)] = Pac->Data[cnt];
		}
	}
	Transmit_Temp[10] = Temp_Buf;
	Transmit_Temp[1] = (((Pac->PID) << 4) | ((~(Pac->PID)) & 0x0f));
	Transmit_Temp[11] = 0xff;
	UART_SendString((char *)&Transmit_Temp,12);
}
