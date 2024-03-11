#include "dcp.h"
#include "led.h"
void DCP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;	 //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 		 //根据设定参数初始化GPIOA.4|5
	GPIO_SetBits(GPIOB,GPIO_Pin_14);	//可控硅，		//拉高关断//拉低开起
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);//继电器			//拉高开起//拉低关断
}

void shoot(void)		
{	
	RELAY_OPEN; //打开继电器
	delay_ms(1500);
	delay_ms(1500);
	delay_ms(1500);
	delay_ms(1500);
	RELAY_CLOSE;			//关闭继电器
	BEEP_ON;					//开枪示警;
	delay_ms(500); 
	SCR_OPEN;					//开始发射
	delay_ms(500);
	SCR_CLOSE;				//结束
	BEEP_OFF;					//关闭示警;
	delay_ms(100);
}
void fd_shoot(void)		
{	
	
	BEEP_ON;					//开枪示警;
	delay_ms(500); 
	SCR_OPEN;					//开始发射
	delay_ms(500);
	SCR_CLOSE;				//结束
	BEEP_OFF;					//关闭示警;
	delay_ms(100);
}