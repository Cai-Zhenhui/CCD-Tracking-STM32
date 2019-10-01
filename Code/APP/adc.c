#include "adc.h"
/*
u16 get_Adc(u8 ch) {
	ADC1->SQR3 &= 0xffffffe0;//��������1 
	ADC1->SQR3 |= ch; //ͨ��ch
	ADC1->CR2 |= 1 << 22;//��������ת��ͨ��
	while (!(ADC1->SR & 1 << 1)) {
		;//�ȴ�ת������
	}
	return ADC1->DR;//����digitalֵ
}

void adc_init(void) {
	RCC->APB2ENR |= 1 << 9;//ADC1 ʱ��ʹ��
	RCC->APB2RSTR |= 1 << 9;//ADC1 ��λ
	RCC->APB2RSTR &= ~(1 << 9);//��λ����
	RCC->CFGR &= (3 << 14);//��Ƶ��������

	RCC->CFGR |= 2 << 14;
	ADC1->CR1 &= 0XF0FFFF;//����ģʽ����
	ADC1->CR1 |= 0 << 16;//��������ģʽ
	ADC1->CR1 &= ~(1 << 8);//��ɨ��ģʽ
	ADC1->CR2 &= ~(1 << 1);//����ת��ģʽ
	ADC1->CR2 &= ~(7 << 17);
	ADC1->CR2 |= 7 << 17;//�������ת��
	ADC1->CR2 |= 1 << 20;//ʹ���ⲿ����(SWSTART) ����ʹ��һ���¼�������
	ADC1->CR2 &= ~(1 << 11);//�Ҷ���
	ADC1->SQR1 &= ~(0XF << 20);
	ADC1->SQR1 &= 0 << 20;//1��ת���ڹ��������� Ҳ����ֻת����������1
	
	//����ch 7 �Ĳ���ʱ��
	ADC1->SMPR2 = 0x3FFFFFFF;//����ͨ���Ĳ������ڶ�Ϊ239.5
	ADC1->SMPR2 |= 7 << 24;//ͨ��8 239.5����
	ADC1->CR2 |= 1 << 0;//ʹ�� ADC1
	ADC1->CR2 |= 1 << 3;//ʹ�� ��λ У׼
	while (ADC1->CR2 & 1 << 3) {
		;//�ȴ�У׼����
	}
	
	ADC1->CR2 |= 1 << 2;//ʹ�� AD У׼
	while (ADC1->CR2 & 1 << 2) {
		;//�ȴ�У׼����
	}
	delay_ms(1);
}*/
void Adc_Init(void) {
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
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);//����ADC
	
	ADC_ResetCalibration(ADC1);//����ADC1��У׼�Ĵ���
	while (ADC_GetResetCalibrationStatus(ADC1)) {
		;//�ȴ� ����ADC1��У׼�Ĵ��� �����
	}

	ADC_StartCalibration(ADC1);//У׼ADC1
	while (ADC_GetCalibrationStatus(ADC1)){
		;//�ȴ� У׼ADC1 �����
	}

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ADC1�������������

}

/*
Descrition: ��ȡͨ�� ch��ת��ֵ
Note:�ú���������Ҫ21us
*/
u16 Get_ADC_Value(u8 ch) {
	//CLKadc=72MHz/6=12MHz
	//��������=239.5*CLKadc
	//ת��ʱ��=��������+12.5)*CLKadc
	//       =239.5+12.5)*CLKadc
	//       =21us
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);//����ADC1 chͨ�� 239.5����������
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){
		;//��������21us
	}
	return ADC_GetConversionValue(ADC1);
}

