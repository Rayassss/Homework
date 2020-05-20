#ifndef _GRAPH_H_
#define _GRAPH_H_

#define HUMIDITY 0
#define CPU_TEMP 1
#define HUM_TEMP 2

#define Node_Length 64
#define List_Num 3

#define START_X 0								//图表的原点的绝对位置（单位：像素）
#define START_Y 0								//图表的原点的绝对位置（单位：像素）
#define END_X 100								//图表的终点的绝对位置（单位：像素）
#define END_Y 100								//图表的终点的绝对位置（单位：像素）

#define CURVE1_MAX 1000					//曲线1的最大值
#define CURVE2_MAX 400					//曲线2的最大值
#define CURVE3_MAX 400					//曲线3的最大值

#define MOVE_INDEX 48						//图表开始移动的位置

#define BG_COLOR 0x0000					//图表背景颜色
#define CURVE1_COLOR 0xffff			//曲线1的颜色
#define CURVE2_COLOR 0xffff			//曲线2的颜色
#define CURVE3_COLOR 0xffff			//曲线3的颜色

typedef struct
{
	unsigned int *pNext;
	unsigned int Content;
}Curve_List_Node_t;
typedef struct
{
	Curve_List_Node_t *HeadNode;
	Curve_List_Node_t *TailNode;
	unsigned int Length;
}Curve_List_t;

void List_Append(unsigned char List_Index,unsigned int data);
void Graph_Init(void);

#endif
