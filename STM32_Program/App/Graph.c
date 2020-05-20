#include "Graph.h"
#include "GUI.h"
#include "FreeRTOS.h"
#include "string.h"

static unsigned char Node_Counter[List_Num] = {0,0,0};
static Curve_List_t DataList[List_Num];
static Curve_List_Node_t FreeNode[Node_Length * List_Num];
static const float STEP = (END_X - START_X) / Node_Length;
static const float SCALE[3] = { (END_Y - START_Y) / CURVE1_MAX,(END_Y - START_Y) / CURVE2_MAX,(END_Y - START_Y) / CURVE3_MAX};

void Free_Node(Curve_List_Node_t *NodeNum);
Curve_List_Node_t *Malloc_Node(void);

void Draw_AllList(unsigned int color[3])
{
	unsigned int counter1,counter2;
	for(counter1 = 0 ; counter1 < List_Num ; counter1++)
	{
		Curve_List_Node_t *Present_Node, *Next_Node;
		for(counter2 = 0 ; counter2 < Node_Length ; counter2++)
		{
			if(counter2 == 0)
				Present_Node = DataList[counter1].HeadNode;
			else
				Present_Node = Next_Node;
			if(Present_Node->pNext == (unsigned int *)0xFFFFFFFFUL)
				break;
			Next_Node = (Curve_List_Node_t *)Present_Node->pNext;
			DrawLine(	START_X + (unsigned int)(counter2 * STEP) , 
								END_Y - (unsigned int)(Present_Node->Content * SCALE[counter1]),
								START_X + (unsigned int)((counter2 + 1) * STEP),
								END_Y - (unsigned int)(Next_Node->Content * SCALE[counter1]),
								color[counter1]);
		}
	}
}
void List_Append(unsigned char List_Index,unsigned int data)
{
	Curve_List_Node_t *Temp_Node;
	unsigned int color[3] = {BG_COLOR,BG_COLOR,BG_COLOR};
	Draw_AllList(color);
	if(Node_Counter[List_Index] >= MOVE_INDEX)
	{
		Temp_Node = (Curve_List_Node_t *)DataList[List_Index].HeadNode->pNext;
		Free_Node(DataList[List_Index].HeadNode);
		DataList[List_Index].HeadNode = Temp_Node;
	}
	Temp_Node = Malloc_Node();
	DataList[List_Index].TailNode->pNext = (unsigned int *)Temp_Node;
	DataList[List_Index].TailNode = Temp_Node;
	Temp_Node->pNext = (unsigned int *)0xFFFFFFFFUL;
	Temp_Node->Content = data;
	color[0] = CURVE1_COLOR;
	color[1] = CURVE2_COLOR;
	color[2] = CURVE3_COLOR;
	Draw_AllList(color);
}
void Graph_Init(void)
{
	unsigned char counter;
	memset(Node_Counter,0x00,sizeof(Node_Counter));
	memset(FreeNode,0xFFFFFFFFUL,sizeof(FreeNode));
	for(counter = 0 ; counter < List_Num ; counter++)
	{
		DataList[counter].Length = 1;
		DataList[counter].HeadNode = Malloc_Node();
		DataList[counter].TailNode = DataList[counter].HeadNode;
	}
	GUI_box(START_X,START_Y,END_X,END_Y,BG_COLOR);
	
}
void Free_Node(Curve_List_Node_t *NodeNum)
{
	NodeNum->Content = 0xFFFFFFFFUL;
	NodeNum->pNext = (unsigned int *)0xFFFFFFFF;
}
Curve_List_Node_t *Malloc_Node(void)
{
	unsigned int counter;
	for(counter = 0 ; counter < Node_Length * List_Num ; counter++)
	{
		if( FreeNode[counter].Content == 0xFFFFFFFFUL )
			return &FreeNode[counter];
	}
	return 0x00000000UL;
}
