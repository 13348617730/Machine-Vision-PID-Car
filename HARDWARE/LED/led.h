#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define BEEP_ON		GPIO_SetBits(GPIOC,GPIO_Pin_5);	//开蜂鸣器
#define BEEP_OFF	GPIO_ResetBits(GPIOC,GPIO_Pin_5);//关蜂鸣器


#define KEY0   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)//读取按键0
#define KEY1   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)//读取按键1
#define KEY2   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)//读取按键2 
#define KEY3   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)//读取按键2 
//#define LED  PAout(8)

///* 定义SPI连接的GPIO端口, 用户只需要修改下面代码即可任意改变相应的引脚*/
//#define LED_GPIO_PORT	 GPIOA			            /* GPIO端口 */
//#define LED_RCC 	     RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
//#define LED_PIN		     GPIO_Pin_8	

void LED_GPIO_Init(void);
u8 KEY_Scan(u8 mode);
#endif

