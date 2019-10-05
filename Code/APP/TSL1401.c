#include "TSL1401.h"

void TSL1401_Init(void) {
	//Init GPIO
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//SI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//CLK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//AO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//analog input
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	//��ʼ��ADC1
	ADCx_Init(ADC1);
}

u8 ADV[128] = { 0 };
u8 Tint2 = 50;//�����ع�ʱ��(���ݻ�������) ��λ��ms
/*
���õ����ع� Ԥ���ع�ʱ����Tint=Tint1+Tint2=Tint2+4.884 ms 
CLK=(5,2000)KHz
������ʹ�� Tclk(H)=11us Tclk(L)=11us
����Ϊ22us��ʱ��(45.454KHz)
*/
void TSL1401_Read(void) {
	
	u8 i = 0;

	//����Ϊ�����֮ǰ�������� �����ع�
	TSL_CLK = 1;
	TSL_SI = 0;
	delay_us(11);//��ʼ״̬

	TSL_CLK = 0;
	delay_us(11);
	TSL_SI = 1;//-------SI�ߵ�ƽ��������һ��ʱ������
	delay_us(11);

	TSL_CLK = 1;
	delay_us(11);
	TSL_SI = 0;//-------SI�ߵ�ƽ��������һ��ʱ������
	delay_us(11);//TSL ������ʼ����

	for (i = 0; i < 128; ++i) {
		TSL_CLK = 0;
		ADV[i] = (u8)(Get_ADCx_Value(ADC1,ADC_Channel_10)>>4);//Ԥ��22us
		//��12λӳ�䵽8λ
		
		TSL_CLK = 1;
		delay_us(22);
	}
	TSL_CLK = 0;
	delay_us(22);//��129��ʱ���������

	//�����ع�ʱ��Ϊ Tint1=(128-18)*44+44=4884us
	delay_ms(Tint2);//�����ع�ʱ�� Tint2=7000us

	//�ع����  �ۼ��ع�ʱ��Tint=Tint1+Tint2=4884+7000=13.884ms 
	//���вɼ�
	TSL_SI = 1;//-------SI�ߵ�ƽ��������һ��ʱ������
	delay_us(11);

	TSL_CLK = 1;
	delay_us(11);
	TSL_SI = 0;//-------SI�ߵ�ƽ��������һ��ʱ������
	delay_us(11);//TSL ������ʼ����

	for (i = 0; i < 128; ++i) {
		TSL_CLK = 0;
		ADV[i] = (u8)(Get_ADCx_Value(ADC1,ADC_Channel_10) >> 4);//Ԥ��22us
		//��12λӳ�䵽8λ

		TSL_CLK = 1;
		delay_us(22);
	}
	TSL_CLK = 0;
}