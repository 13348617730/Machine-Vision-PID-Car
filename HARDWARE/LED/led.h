#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define BEEP_ON		GPIO_SetBits(GPIOC,GPIO_Pin_5);	//��������
#define BEEP_OFF	GPIO_ResetBits(GPIOC,GPIO_Pin_5);//�ط�����


#define KEY0   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)//��ȡ����0
#define KEY1   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)//��ȡ����1
#define KEY2   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)//��ȡ����2 
#define KEY3   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)//��ȡ����2 
//#define LED  PAout(8)

///* ����SPI���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�������뼴������ı���Ӧ������*/
//#define LED_GPIO_PORT	 GPIOA			            /* GPIO�˿� */
//#define LED_RCC 	     RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
//#define LED_PIN		     GPIO_Pin_8	

void LED_GPIO_Init(void);
u8 KEY_Scan(u8 mode);
#endif

