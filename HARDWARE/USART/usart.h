#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART3_REC_LEN  	200  	//定义最大接收字节数 200
#define EN_USART3_RX 		1		//使能（1）/禁止（0）串口3接收
 
#define Data_Head         0X59   
#define Data_Length       9  
 
//#define Data_Head         0X59   
//#define Data_Length       9  
void uart1_init(u32 bound);
void uart3_init(u32 bound);
void Openmv_data(void);
void Open_Mv(int com_data);
#endif

