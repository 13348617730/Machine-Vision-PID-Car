#include "motor.h"
#include "pwm.h"
/*
PA0 -- END  -- PB0		L		��Ч��ƽ
PA1 -- MS1  -- PB1		H
PC2 -- MS2  -- PC4		H
PC3 -- MS3  -- PA4		H		1/16��
PA6 -- STEP -- PA7		PWM
PC1 -- DIR  -- PA5		����
*/

void MOTOR_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	
	//Motor��ʼ��
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;//|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	// ��ʼ��GPIOA
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	// ��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);	// ��ʼ��GPIOC
	
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);			
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
//	GPIO_ResetBits(GPIOA,GPIO_Pin_6);			//��ʼ��GPIOB_6����͵�ƽ
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);			//��ʼ��GPIOB_7����͵�ƽ
//	GPIO_ResetBits(GPIOA,GPIO_Pin_7);			//��ʼ��GPIOB_8����͵�ƽ
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);			//��ʼ��GPIOB_9����͵�ƽ

//	GPIO_SetBits(GPIOC,GPIO_Pin_1);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}

//	TIM_SetCompare1(TIM3,500);
//		  TIM_SetCompare2(TIM3,500);
void motor(unsigned int motor1_dir, unsigned int motor2_dir)
{
	
		switch(motor1_dir)
		{
			case 0 : GPIO_SetBits(GPIOA,GPIO_Pin_0);			
						   GPIO_SetBits(GPIOB,GPIO_Pin_0);
							 TIM_SetCompare1(TIM3,0);
							 TIM_SetCompare2(TIM3,0);			
							 break; 
			case 1 : GPIO_ResetBits(GPIOA,GPIO_Pin_0);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_0);
							 TIM_SetCompare1(TIM3,500);
							 TIM_SetCompare2(TIM3,500);		
							 break; 
			default : break; 
		}
		switch(motor2_dir)
		{
			case 0 : GPIO_SetBits(GPIOC,GPIO_Pin_1);
							 GPIO_ResetBits(GPIOA,GPIO_Pin_5);
							 break; 
			case 1 : GPIO_ResetBits(GPIOC,GPIO_Pin_1); 
							 GPIO_SetBits(GPIOA,GPIO_Pin_5);
							 break; 
			case 2 : GPIO_ResetBits(GPIOC,GPIO_Pin_1); 
							 GPIO_ResetBits(GPIOA,GPIO_Pin_5);
							 break; 
			case 3 : GPIO_SetBits(GPIOC,GPIO_Pin_1); 
							 GPIO_SetBits(GPIOA,GPIO_Pin_5);
							 break; 			
			default : break; 
		}
	
}


//void motor(unsigned int motor1_dir, unsigned int motor1_step, unsigned int motor2_dir, unsigned int motor2_step)
//{
//    unsigned int i;

//		switch(motor1_dir)
//		{
//			case 0 : GPIO_SetBits(GPIOC,GPIO_Pin_1); break; 
//			case 1 : GPIO_ResetBits(GPIOC,GPIO_Pin_1); break; 
//			default : break; 
//		}
//		switch(motor2_dir)
//		{
//			case 0 : GPIO_SetBits(GPIOA,GPIO_Pin_5); break; 
//			case 1 : GPIO_ResetBits(GPIOA,GPIO_Pin_5); break; 
//			default : break; 
//		}
//		/*
//		GPIO_SetBits(Motor_GPIO,Motor1_STEP);
//		GPIO_SetBits(Motor_GPIO,Motor2_STEP);
//		delay_ms(2);									//����1.3ms
//		GPIO_ResetBits(Motor_GPIO,Motor1_STEP);
//		GPIO_ResetBits(Motor_GPIO,Motor2_STEP);
//		delay_ms(2);
//		*/
//		for(i = 0;i < motor1_step || i < motor2_step; i++)
//		{
//			if(i<motor1_step)
//			{
//				GPIO_SetBits(GPIOA,GPIO_Pin_6);
//				delay_ms(1);									//����1.3ms
//				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
//				delay_ms(1);
//			} 
//			if(i<motor2_step)
//			{
//				GPIO_SetBits(GPIOA,GPIO_Pin_7);
//				delay_ms(1);									//����1.3ms
//				GPIO_ResetBits(GPIOA,GPIO_Pin_7);
//				delay_ms(1);
//			}
//		}

//  }