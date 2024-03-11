#ifndef __MOTOR_H
#define __MOTOR_H	
#include "delay.h"

//#define Motor_GPIO GPIOA				//PF
//#define Motor_RCC RCC_APB2Periph_GPIOA
////第一个步进电机A4988的接线
//#define Motor1_STEP	GPIO_Pin_6	//STEP - PF1
//#define Motor1_DIR	GPIO_Pin_1	//DIR  - PF2
////第二个步进电机A4988的接线	//PB
//#define Motor2_STEP	GPIO_Pin_7	//STEP - PF3
//#define Motor2_DIR	GPIO_Pin_5	//DIR  - PF4

void MOTOR_Init();
void motor(unsigned int motor1_dir, unsigned int motor2_dir);
#endif  
