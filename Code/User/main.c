#include "SysTick.h"
#include "Motor.h"
//#include "usart.h"
#include "TSL1401.h"
#include "SteeringGear.h"

void InitALL()
{
	//Initialize all the configurations here
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	

	//USART1_Init(38400);

	TSL1401_Init();

	Motor_Init();

	SteeringGearInit();

}	

void delay_s(int a){
	int i=0;
	for(i=0;i<a;++i){
		delay_ms(1000);
	}
}

void Track() {
	/*
	MidIndex to angle to TIM
	1			L		70
	64			M		120
	127			R		170
	So TIM=0.794*MidIndex+69
	*/

	int i = 0;
	u8 max,min;
	int Threshold;//��ֵ�� ��ֵ
	int fact = -30;//�˲�ϵ�� -30��˼��ƽ�����µ�30
	u8 IndexL = 0;
	u8 IndexR = 0;
	u8 IndexM = 0;

	TSL1401_Read();//��128��������Ϣ ����
	ADV[127] = ADV[126];//����128��������Ϣ����

	//ȡ��ֵ
	for (min = ADV[0], max = ADV[0], i = 0; i < 128; ++i) {
		if (ADV[i] < min) {
			min = ADV[i];
		}
		else {
			if (ADV[i] > max) {
				max = ADV[i];
			}
		}
	}
	Threshold = ((min + max) >> 1) + fact;//������ֵ

	//��ֵ��
	for (i = 0; i < 128; ++i) {
		if (ADV[i] > Threshold) {
			ADV[i] = 0xff;
		}
		else {
			ADV[i] = 0x00;
		}
	}

	IndexL = 1;
	for (i = 1; i < 128; ++i) {
		if (ADV[i] == 0x00) {
			IndexL = i;
			break;
		}
	}

	IndexR = 126;
	for (i = 126; i >0; --i) {
		if (ADV[i] == 0x00) {
			IndexR = i;
			break;
		}
	}

	IndexM = (IndexL + IndexR) >> 1;//��������
	/*if ((IndexR - IndexL) >= 55) {//test
		//���߿��ռ�� �ӽ�һ�� ����ֱ����
		if (IndexM > 64) {
			//TurnRigh
			TurnRight(60, 0);
		}
		else {
			//TurnLeft
			TurnLeft(60, 0);
		}
		delay_s(1);
	}
	else {
		Go(50);
	}*/
	Go(70);
	//������λ�û���� ����Ƕ��� �ٻ���� PWMռ�ձ�
	TIM2->CCR3 = (u16)(0.8 * IndexM + 72);//MidIndex to angle to TIM
	
}
int main()
{
	
	InitALL();

	TIM2->CCR3 = 128;//�����λ
	delay_s(5);

	//Go(50);
	while (1) {
		
		Track();
		//delay_ms(200);
	}
	
	/*
	int i = 0;
	int max;
	int min;
	int Threshold;
	int fact = -30;

	InitALL();
	TIM2->CCR3 = 128;//�����λ
	
	//r = 40;
	delay_s(13);

	
	while (1) {
		TSL1401_Read();//��128��������Ϣ ����
		ADV[127] = ADV[126];//����128��������Ϣ����

		//ȡ��ֵ
		for (min=ADV[0],max=ADV[0],i=0; i < 128; ++i) {
			if (ADV[i] < min) {
				min = ADV[i];
			}
			else {
				if (ADV[i] > max) {
					max = ADV[i];
				}
			}
		}
		Threshold = ((min + max) >> 1) + fact;



		for (i = 0; i < 128; ++i) {
			
			
			//��ֵ��
			if (ADV[i] > Threshold) {
				ADV[i] = 0xff;
			}
			else {
				ADV[i] = 0x00;
			}

			USART_SendData(USART1, ADV[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET) {
				;//waiting 
			}
			USART_ClearFlag(USART1, USART_FLAG_TC);
		}
		//Tint2 = r;
		//delay_ms(100);
	}*/
	return 0;
}
