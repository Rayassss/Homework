#ifndef _GUI_H_
#define _GUI_H_
unsigned int TranColor(unsigned int color);
void GUI_Clear(unsigned int color);
void UGUI_Init(void);
void Draw_Icon(void);
void GUI_Point(unsigned char x, unsigned int y, unsigned int color);
void GUI_box(unsigned char sx,unsigned int sy,unsigned char ex,unsigned int ey,unsigned int color);
void GUI_Printf(unsigned char line,unsigned int dcolor,unsigned int bgcolor,const char *fmt,...);
void DrawLine(int x1,int y1,int x2,int y2,int color);
#endif
