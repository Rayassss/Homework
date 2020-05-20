#include "DTH11.h"

void Set_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //∏°ø’ ‰»Î
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void Set_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Õ∆ÕÏ ‰≥ˆ
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void DTH11_RST(void)
{
    Set_Output();
    DHT11_OUT_L;
    Delayus(19000);
    DHT11_OUT_H;
    Delayus(30);
    Set_Input();
}

u16 DTH11_Scan(void)
{
    return DHT11_IN;
}

u16 dht11_read_bit(void)
{
    while (DHT11_IN == RESET);
    Delayus(40);
    if (DHT11_IN == SET)
    {
        while (DHT11_IN == SET);
        return 1;
    }
    else
    {
        return 0;
    }
}

u16 DTH11_ReadByte(void)
{
    u16 i;
    u16 data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= dht11_read_bit();
    }
    return data;
}

u16 DTH11_ReadData(u8 *buffer)
{
    u16 i = 0;
		u8 checksum;
    DTH11_RST();
    if (DTH11_Scan() == RESET)
    {
        while (DTH11_Scan() == RESET);
        while (DTH11_Scan() == SET);
        for (i = 0; i < 5; i++)
            buffer[i] = DTH11_ReadByte();
        while (DTH11_Scan() == RESET);
        Set_Output();
        DHT11_OUT_H;
        checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
        if (checksum != buffer[4])
					return 1;
    }
    return 0;
}
