#include "time.h"
#include "usart.h"
#include "pid.h"
#include "pwm.h"
extern PID_TypDef PHXC; 
extern u8 Openmv_X,
			    Openmv_Y;
extern int start;
int pwm1=145,pwm2=150;
void TIM1_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 

  TIM_TimeBaseStructure.TIM_Period = arr; 
  TIM_TimeBaseStructure.TIM_Prescaler =psc;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);

  TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	
  //?????
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //优先级组别
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(TIM1, ENABLE);  
}
void TIM1_UP_IRQHandler(void)
{
	static uint16_t led_cnt = 0;
				
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) 
	{
		if(start==1)
		{
			led_cnt++;
		}
		if(led_cnt>100)
		{
		  Lock_target();
			led_cnt=0;
		}	
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  
	}
}
void Lock_target(void)
{
	pwm2=150-PID_run(&PHXC,Openmv_X,80);
	pwm1=120+Openmv_Y/4;
	OLED_ShowNum(0,2,pwm2,3,16);
	if(pwm2>200)
	{
		pwm2=200;
		TIM_SetCompare2(TIM4,pwm2);
	}
	else if(pwm2<100)
	{
		pwm2=100;
		TIM_SetCompare2(TIM4,pwm2);
	}
	else
	{
		TIM_SetCompare2(TIM4,pwm2);
	}
	 if(pwm1>150)
	{
		pwm1=150;
		TIM_SetCompare1(TIM4,pwm1);
	}
	else if(pwm1<120)
	{
		pwm1=120;
		TIM_SetCompare1(TIM4,pwm1);
	}
	else
	{
		TIM_SetCompare1(TIM4,pwm1);
	}
	
}
