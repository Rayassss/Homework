#include "LCD.h"
#include "System_Config.h"
//发送命令
void  LCD_WriteReg(unsigned int Com)
{
     int i;
     A0=0;
     CSB=0;
     for(i=0;i<8;i++)
     {
			 if(Com & 0x80)
					SDA=1;
			 else 
					SDA=0;
			 SCL=0;
			 SCL=1;
			 Com <<=1;
     }
     CSB=1;	
}
void LCD_WriteData(unsigned int dat)
{
      int i;
      A0=1;
      CSB=0;
      for(i=0;i<8;i++)
      {
				if(dat&0x80)
					SDA=1;
				else 
					SDA=0;
				SCL=0;
				SCL=1;
				dat<<=1;
      } 
      CSB=1;
}
//发送数据
void Lcd_Write_Data(unsigned int dat16)
{
  LCD_WriteData(dat16>>8);
  LCD_WriteData(dat16);
}

//设置坐标范围		
//x1 y1  初始位置
//x2 y2  结束位置
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
		LCD_WriteReg(0X2A); 
		LCD_WriteData(x1>>8); 
		LCD_WriteData(x1&0XFF);
		LCD_WriteData(x2>>8); 
		LCD_WriteData(x2&0XFF);
		LCD_WriteReg(0X2B); 
		LCD_WriteData(y1>>8); 
		LCD_WriteData(y1&0XFF);
		LCD_WriteData(y2>>8); 
		LCD_WriteData(y2&0XFF);
	  LCD_WriteReg(0X2C);      //开始写入GRAM
}
/*
9341扫描特点 如果扫描方向改变后 会直接改变x y起始原点
所以在横屏显示的时候 就不用考虑 x y的变化后的结果 因为原点也发生改变
就是可以一种代码通用
*/
//tft屏设置扫描方式函数
//mode 1 -8  一共8种扫描方式
//     1   左>右  上>下 
//     2   左>右  下>上
//     3   右>左  上>下
//     4   右>左  下>上 
//     5   上>下  左>右  
//     6   上>下  右>左 
//     7   下>上  左>右 
//     8   下>上  右>左 
void LCD_Scan(unsigned char mode)
{
 switch(mode)
 {
   case 1:
		LCD_WriteReg(0x36);    // Memory Access Control 	   saomiao
		LCD_WriteData(0x08); 	
    LCD_WriteReg(0X2C);      //开始写入GRAM			 
	break;
   case 2:
		LCD_WriteReg(0x36);    // Memory Access Control 	   saomiao
		LCD_WriteData(0x88); 
		LCD_WriteReg(0X2C);      //开始写入GRAM					 
	break;
   case 3:
		LCD_WriteReg(0x36);    // Memory Access Control 	   saomiao
		LCD_WriteData(0x48); 	
		LCD_WriteReg(0X2C);      //开始写入GRAM				
	break;
   case 4:
		LCD_WriteReg(0x36);    // Memory Access Control 	   saomiao
		LCD_WriteData(0xc8); 	
		LCD_WriteReg(0X2C);      //开始写入GRAM				 
	break;
   case 5:
		LCD_WriteReg(0x36);    // Memory Access Control 	   saomiao
		LCD_WriteData(0x28); 
		LCD_WriteReg(0X2C);      //开始写入GRAM					 
	break;
   case 6:
		LCD_WriteReg(0x36);    // Memory Access Control 	   saomiao
		LCD_WriteData(0x68); 	
		LCD_WriteReg(0X2C);      //开始写入GRAM				 
	break;
   case 7:
		LCD_WriteReg(0x36);    // Memory Access Control 	   saomiao
		LCD_WriteData(0xa8); 	
		LCD_WriteReg(0X2C);      //开始写入GRAM				 
	break;
   case 8:
		LCD_WriteReg(0x36);    // Memory Access Control 	   saomiao
		LCD_WriteData(0xe8); 
		LCD_WriteReg(0X2C);      //开始写入GRAM					 
	break;
 }
}
//液晶初始化
void LCD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RES =1; 
	Delayus(1000);                                                              //Delay 1ms 
	RES =0; 
	Delayus(1000);                                                              //Delay 1ms 
	RES =1; 
	Delayus(100000);                                                              //Delay 100ms 
	LCD_WriteReg(0x11); //Sleep out 
	Delayus(100000);              //Delay 100ms 
	LCD_WriteReg(0xB1); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteReg(0xB2); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteReg(0xB3); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteReg(0xB4); //Dot inversion 
	LCD_WriteData(0x03); 
	LCD_WriteReg(0xC0); 
	LCD_WriteData(0x28); 
	LCD_WriteData(0x08); 
	LCD_WriteData(0x04); 
	LCD_WriteReg(0xC1); 
	LCD_WriteData(0XC0); 
	LCD_WriteReg(0xC2); 
	LCD_WriteData(0x0D); 
	LCD_WriteData(0x00); 
	LCD_WriteReg(0xC3); 
	LCD_WriteData(0x8D); 
	LCD_WriteData(0x2A); 
	LCD_WriteReg(0xC4); 
	LCD_WriteData(0x8D); 
	LCD_WriteData(0xEE); 
	LCD_WriteReg(0xC5); //VCOM 
	LCD_WriteData(0x1A); 
	LCD_WriteReg(0x36); //MX, MY, RGB mode 
	LCD_WriteData(0xC0); 
	LCD_WriteReg(0xE0); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x22); 
	LCD_WriteData(0x07); 
	LCD_WriteData(0x0A); 
	LCD_WriteData(0x2E); 
	LCD_WriteData(0x30); 
	LCD_WriteData(0x25); 
	LCD_WriteData(0x2A); 
	LCD_WriteData(0x28); 
	LCD_WriteData(0x26); 
	LCD_WriteData(0x2E); 
	LCD_WriteData(0x3A); 
	LCD_WriteData(0x00); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x03); 
	LCD_WriteData(0x13); 
	LCD_WriteReg(0xE1); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x16); 
	LCD_WriteData(0x06); 
	LCD_WriteData(0x0D); 
	LCD_WriteData(0x2D); 
	LCD_WriteData(0x26); 
	LCD_WriteData(0x23); 
	LCD_WriteData(0x27); 
	LCD_WriteData(0x27); 
	LCD_WriteData(0x25); 
	LCD_WriteData(0x2D); 
	LCD_WriteData(0x3B); 
	LCD_WriteData(0x00); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x13); 
	LCD_WriteReg(0x3A); //65k mode 
	LCD_WriteData(0x05); 
	LCD_WriteReg(0x29); //Display on 
}	   
