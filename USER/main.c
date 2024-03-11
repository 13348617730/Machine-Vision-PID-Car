/*
1.��Ŀ���ƣ������콢��-STM32�ۺϲ��԰�SPI�ӿ�1.44��LCD���Գ���
2.��ʾģ�飺1.44��LCD��
3.����APP����
4.������λ������
5.��Ŀ��ɣ�1.44��LCD������STM32�ۺϲ��԰�
6.��Ŀ���ܣ�1.44��LCD��ʾ��Ӣ�ġ����֡�ͼƬ����������뿴��ʾ��Ƶ
7.��Ҫԭ������ο�1.44��LCD�����ֲ�
8.�����ַ��https://lssz.tmall.com ���Ա��������������콢�ꡱ
9.��ʾ��Ƶ����������ҳ��������ͷ���Ҫ��
10.��Ȩ�����������콢�����г��������������Ȩ�����뱾���Ʒ���׳��ۣ��벻Ҫ����������׷���䷨�����Σ�
*/
#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "stdio.h"
#include "Picture.h"
#include "led.h"
#include "oled.h"
#include "motor.h"
#include "pwm.h"
#include "usart.h"
#include "pid.h"
#include "time.h"
#include "dcp.h"
PID_TypDef PHXC; 
extern  u8 Res;
extern  u16 Distance;
int stfs =1,start=0;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	LED_GPIO_Init();
	DCP_Init();
	PID_Init(&PHXC);
	//PID_Set(&PHXC,0.28,0.002,0.0001);
	PID_Set(&PHXC,0.2,0.04,-0.002);
	uart1_init(9600);
	//uart3_init(9600);
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear();
  delay_ms(200);
	TIM1_Init(720-1,100-1);		//1ms
	MOTOR_Init();
	TIM3_PWM_Init(7200-1,10-1);
	TIM4_PWM_Init(2000-1,720-1);//50hz		100--1ms
	TIM_SetCompare1(TIM4,145);
	TIM_SetCompare2(TIM4,150);
	delay_ms(200);
	while(1)
	{
		if(KEY_Scan(1)==1&&stfs==1)
		{	
			delay_ms(500);
			TIM_SetCompare2(TIM4,200);//����
			delay_ms(700);
			TIM_SetCompare2(TIM4,100);//����
			delay_ms(1500);
			TIM_SetCompare2(TIM4,150);//����
			delay_ms(500);
			motor(1,3);
			delay_ms(1500);
			delay_ms(600);
			motor(1,0);
			delay_ms(1500);
			delay_ms(1500);
			delay_ms(1500);
			delay_ms(1500);
			motor(0,0);
			TIM_SetCompare1(TIM4,130);//����
			delay_ms(1500);
			OLED_ShowString(50,4,"ON",16);
			shoot();
			stfs=0;
			OLED_ShowString(50,4,"OFF",16);
		}
		if(KEY_Scan(1)==2&&stfs==1)
		{ 
			TIM_SetCompare1(TIM4,130);
			TIM_SetCompare2(TIM4,150);
			delay_ms(1500);
			OLED_ShowString(50,4,"ON",16);
			shoot();
			stfs=0;
			OLED_ShowString(50,4,"OFF",16);
		}
		if(KEY_Scan(1)==3)
		{
			start=1;
		}			
		if(KEY_Scan(1)==4)
		{
			fd_shoot();	
		}			
//			TIM_SetCompare1(TIM3,500);
//		  TIM_SetCompare2(TIM3,500);
//		  TIM_SetCompare1(TIM4,130);//110-150//x��
//			motor(1,0);
//		  delay_ms(500);
//			motor(1,1);
//		  delay_ms(500);
//			motor(1,2);
//		  delay_ms(500);
//			motor(1,3);
//		  delay_ms(500);
//			motor(0,0);
			//OLED_ShowNum(10,2,Distance,3,16);
		  delay_ms(100);
//		
	}
//	LCD_DrawFont_GBK16(24, 60, BLUE, WHITE, "�����콢��");
//	LCD_DrawFont_GBK16(28, 80, RED, WHITE, "1.44 LCD");
//	LCD_DrawFont_GBK16(8, 100, BLACK, WHITE, "lssz.tmall.com");
//	LED=0;
//	
//	while(1)
//	{
//		delay_ms(500);
//		LED=!LED;
//	}
}


