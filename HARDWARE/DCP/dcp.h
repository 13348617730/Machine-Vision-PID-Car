#ifndef __DCP_H
#define __DCP_H			
#include "delay.h"
#define  RELAY_OPEN           GPIO_SetBits(GPIOB, GPIO_Pin_15);		//¼ÌµçÆ÷
#define  RELAY_CLOSE					GPIO_ResetBits(GPIOB, GPIO_Pin_15);

#define  SCR_OPEN           	GPIO_ResetBits(GPIOB, GPIO_Pin_14);//·¢Éä
#define  SCR_CLOSE						GPIO_SetBits(GPIOB, GPIO_Pin_14);

void DCP_Init(void);
void shoot(void);
void fd_shoot(void);
#endif