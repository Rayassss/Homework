#include "LCD.h"
#include "GUI.h"
#include "Font.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "System_Config.h"

void LCD_SendChar(unsigned char x,unsigned char line,unsigned char value,unsigned int dcolor,unsigned int bgcolor)	
{  
	unsigned char i,j;
	unsigned char *temp = (unsigned char *)&Font_8_16[0];
	temp+=(value-32)*16;
	Address_set(x,line * 16 + 5,x+7,line * 16 + 20);
		for(j=0;j<16;j++)
		{
			for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<(7-i)))!=0)
				  Lcd_Write_Data(dcolor);
				 
				else				
				  Lcd_Write_Data(bgcolor);
			}
			temp++;
		}
}
void LCD_ShowString(unsigned char line,unsigned int dcolor,unsigned int bgcolor,uint8_t *str)
{
	uint8_t n = 0;
	uint8_t row = 5;
	while (str[n] != '\0')
  {
		LCD_SendChar(row,line,str[n],dcolor,bgcolor);
		row+=8;
		n++;
	}
}
void GUI_Printf(unsigned char line,unsigned int dcolor,unsigned int bgcolor,const char *fmt,...)
{
    uint8_t LCD_BUF[128] = {0};
    va_list ap;
		memset(LCD_BUF,'\0',sizeof(LCD_BUF));
    va_start(ap, fmt);
    vsprintf((char *)LCD_BUF, fmt, ap);
    va_end(ap);
    LCD_ShowString(line,dcolor,bgcolor,LCD_BUF);
}
void GUI_Clear(unsigned int color)
{
	 unsigned int i;
	 unsigned int j;
	 Address_set(0,0,127,159);
   for(i=0;i<128;i++)
	 {
	    for (j=0;j<160;j++)
        	 Lcd_Write_Data(color);
		}
}
void GUI_Point(unsigned char x, unsigned int y, unsigned int color)
{  
	unsigned int Temp_Color = TranColor(color);
	Address_set(x,y,x,y);
	Lcd_Write_Data(Temp_Color);
}
unsigned int TranColor(unsigned int color)
{
	unsigned int Result;
	unsigned int Temp_Red,Temp_Blue,Temp_Green;
	Temp_Red = color >> 11;
	Temp_Blue = color & 0x001f;
	Temp_Green = (color & 0x07ff) >> 5;
	Result = Temp_Red;
	Result |= Temp_Green << 5;
	Result |= Temp_Blue << 11;
	return Result;
}
void GUI_box(unsigned char sx,unsigned int sy,unsigned char ex,unsigned int ey,unsigned int color)
{ 
	unsigned int temp,temp1,m,n;   
	unsigned int Temp_Color = TranColor(color);
	Address_set(sx,sy,ex,ey); 
  n=ex-sx+1;
	m=ey-sy+1;
	for(temp=0;temp<n;temp++)
	{	 	
		for(temp1=0;temp1<m;temp1++)
	 	{	
			Lcd_Write_Data(Temp_Color);
		}
	}
}
void DrawLine(int x1,int y1,int x2,int y2,int color)
{
    int dx,dy,e;
    dx=x2-x1; 
    dy=y2-y1;
    if(dx>=0)
    {
        if(dy >= 0)
        {
            if(dx>=dy)
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    GUI_Point(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}   
                    x1+=1;
                    e+=dy;
                }
            }
            else
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    GUI_Point(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}   
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else
        {
            dy=-dy;
            if(dx>=dy)
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    GUI_Point(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}   
                    x1+=1;
                    e+=dy;
                }
            }
            else
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    GUI_Point(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}   
                    y1-=1;
                    e+=dx;
                }
            }
        }   
    }
    else
    {
        dx=-dx;
        if(dy >= 0)
        {
            if(dx>=dy)
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    GUI_Point(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}   
                    x1-=1;
                    e+=dy;
                }
            }
            else
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    GUI_Point(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}   
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else
        {
            dy=-dy;
            if(dx>=dy)
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    GUI_Point(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}   
                    x1-=1;
                    e+=dy;
                }
            }
            else
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    GUI_Point(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}   
                    y1-=1;
                    e+=dx;
                }
            }
        }   
    }
}
