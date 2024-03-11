#include "led.h"


void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 		 //�����趨������ʼ��GPIOA.4|5
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);	//�ɿع裬		//���߹ض�//���Ϳ���

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.0
	
	
	
//	GPIO_InitTypeDef  GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);	 //ʹ�ܶ˿�ʱ��

//	GPIO_InitStructure.GPIO_Pin = LED_PIN;	 //�������
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
//	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);	 //��ʼ��GPIO
}

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(KEY2==0)return 3; 
		else if(KEY3==0)return 4; 
	}else if(KEY0==1&&KEY1==1&&KEY2==1||KEY3==1)key_up=1; 	     
	return 0;// �ް�������
}