#include "System_Config.h"
#include "ADC.h"
void Temp_Init()
{
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//72/6 = 12M
	ADC_DeInit(ADC1);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 1; 
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,1,ADC_SampleTime_239Cycles5);
	ADC_TempSensorVrefintCmd(ENABLE);//���ڲ��¶ȴ�����ʹ��
	ADC_Cmd(ADC1,ENABLE);	
	ADC_ResetCalibration(ADC1);//����ָ����ADC��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));//��ȡADC����У׼�Ĵ�����״̬
	ADC_StartCalibration(ADC1);//��ʼָ��ADC��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));//��ȡָ��ADC��У׼����
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ�ܻ���ʧ��ָ����ADC�����ת����������
}
unsigned int Get_Temp()		//����10��
{
	char i = 0;
	float value = 0;
	for(i=0;i<50;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
		value += ADC_GetConversionValue(ADC1);
	}
	value /= 50.0f;
	value = (1.43-value*3.3/4096)/0.0043+25;
	value = value * 10;
	return (unsigned int)(value / 1);
}
