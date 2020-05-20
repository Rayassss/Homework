#ifndef _LCD_H_
#define _LCD_H_

#define RES PAout(8)
#define A0	PAout(6)

#define SDA PAout(7)
#define SCL PAout(5)
#define CSB PAout(4)

void LCD_Init(void);
void LCD_Scan(unsigned char mode);
void Lcd_Write_Data(unsigned int dat16);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
#endif
