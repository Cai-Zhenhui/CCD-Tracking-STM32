#include "adc.h"

void ADCx_Init(ADC_TypeDef* ADCx) {
	//GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADC�ķ�Ƶ���� CLKadc=72MHz/6=12MHz

	/*
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PA1 ADC1 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//analog input
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������*/

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//disable scan conversion mode
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//disable continue conversion mode
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//disable external (use sofeware )
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;//1һ��ת���ڹ��������� Ҳ����ֻת����������1
	ADC_Init(ADCx, &ADC_InitStructure);

	ADC_Cmd(ADCx, ENABLE);//����ADC
	
	ADC_ResetCalibration(ADCx);//����ADC1��У׼�Ĵ���
	while (ADC_GetResetCalibrationStatus(ADCx)) {
		;//�ȴ� ����ADC1��У׼�Ĵ��� �����
	}

	ADC_StartCalibration(ADCx);//У׼ADCx
	while (ADC_GetCalibrationStatus(ADCx)){
		;//�ȴ� У׼ADC1 �����
	}

	ADC_SoftwareStartConvCmd(ADCx, ENABLE);//ʹ��ADC1�������������

}

/*
Descrition: ��ȡͨ�� ch��ת��ֵ
Note:�ú���������Ҫ21us
*/
u16 Get_ADCx_Value(ADC_TypeDef* ADCx,u8 ch) {
	//CLKadc=72MHz/6=12MHz
	//��������=239.5*CLKadc
	//ת��ʱ��=��������+12.5)*CLKadc
	//       =239.5+12.5)*CLKadc
	//       =21us
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_239Cycles5);//����ADC1 chͨ�� 239.5����������
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
	while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC)){
		;//��������21us
	}
	return ADC_GetConversionValue(ADCx);
}

