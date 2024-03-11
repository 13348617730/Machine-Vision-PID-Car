/*
1.项目名称：绿深旗舰店-STM32综合测试板SPI接口1.44吋LCD测试程序
2.显示模块：1.44吋LCD屏
3.配套APP：无
4.配套上位机：无
5.项目组成：1.44吋LCD、绿深STM32综合测试板
6.项目功能：1.44吋LCD显示中英文、数字、图片。具体操作请看演示视频
7.主要原理：具体参考1.44吋LCD数据手册
8.购买地址：https://lssz.tmall.com 或淘宝上搜索“绿深旗舰店”
9.演示视频：宝贝详情页面或购买后像客服索要！
10.版权声明：绿深旗舰店所有程序都申请软件著作权。均与本店产品配套出售，请不要传播，以免追究其法律责任！
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();
	LED_GPIO_Init();
	DCP_Init();
	PID_Init(&PHXC);
	//PID_Set(&PHXC,0.28,0.002,0.0001);
	PID_Set(&PHXC,0.2,0.04,-0.002);
	uart1_init(9600);
	//uart3_init(9600);
	OLED_Init();			//初始化OLED  
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
			TIM_SetCompare2(TIM4,200);//左右
			delay_ms(700);
			TIM_SetCompare2(TIM4,100);//左右
			delay_ms(1500);
			TIM_SetCompare2(TIM4,150);//左右
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
			TIM_SetCompare1(TIM4,130);//上下
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
//		  TIM_SetCompare1(TIM4,130);//110-150//x轴
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
//	LCD_DrawFont_GBK16(24, 60, BLUE, WHITE, "绿深旗舰店");
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


