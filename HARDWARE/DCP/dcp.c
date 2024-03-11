#include "dcp.h"
#include "led.h"
void DCP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;	 //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 		 //�����趨������ʼ��GPIOA.4|5
	GPIO_SetBits(GPIOB,GPIO_Pin_14);	//�ɿع裬		//���߹ض�//���Ϳ���
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);//�̵���			//���߿���//���͹ض�
}

void shoot(void)		
{	
	RELAY_OPEN; //�򿪼̵���
	delay_ms(1500);
	delay_ms(1500);
	delay_ms(1500);
	delay_ms(1500);
	RELAY_CLOSE;			//�رռ̵���
	BEEP_ON;					//��ǹʾ��;
	delay_ms(500); 
	SCR_OPEN;					//��ʼ����
	delay_ms(500);
	SCR_CLOSE;				//����
	BEEP_OFF;					//�ر�ʾ��;
	delay_ms(100);
}
void fd_shoot(void)		
{	
	
	BEEP_ON;					//��ǹʾ��;
	delay_ms(500); 
	SCR_OPEN;					//��ʼ����
	delay_ms(500);
	SCR_CLOSE;				//����
	BEEP_OFF;					//�ر�ʾ��;
	delay_ms(100);
}