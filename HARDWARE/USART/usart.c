#include "usart.h"
#include "sys.h"
#include "oled.h"
//u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//u16 USART3_RX_STA = 0;       //接收状态标记	 
  u16 Distance = 0, Strength = 0;
//	uint8_t RX_Flag = 0;
	u16 USART_RX_STA=0;       //接收状态标记	  
  u8 Res=80,Cx=80,Cy=60; 
	u8 Openmv_X =80,
	   Openmv_Y = 60;
//	uint8_t TFmini_low,TFmini_high = 0;
//	u8 flag_REC = 0;
//	u8 count_REC = 0;
void uart1_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}
void USART1_IRQHandler(void)			 
{
		int com_data1; 
		if( USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  	   //接收中断  
		{
				//清除中断标志
				com_data1 = USART_ReceiveData(USART1);
				Open_Mv(com_data1);
				Openmv_data();							
			  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		}
		
}
void Openmv_data(void)
{
	Openmv_X = Cx;
	Openmv_Y = Cy;
}
void Open_Mv(int com_data)
{ 
		u8 i;
		static u8 RxCounter1=0;
		static u16 RxBuffer1[10]={0};
		static u8 RxState = 0;	
		static u8 RxFlag1 = 0;
		if(RxState==0&&com_data==0xDC)  //0x2c帧头，防止接受不合要求数字，例如128，正常为1~8
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;
				//	OLED_Refresh();
				}
		
				else if(RxState==1&&com_data==0xF3)  //0x12帧头
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=com_data;
					
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1>=10||com_data == 0xE8)       //RxBuffer1接受满了,接收数据结束
					{
						RxState=3;
						RxFlag1=1;
						Cx=RxBuffer1[RxCounter1-3];
						Cy=RxBuffer1[RxCounter1-2];           
					}
				}
		
				else if(RxState==3)		//检测是否接受到结束标志
				{
						if(RxBuffer1[RxCounter1-1] == 0xE8)
						{
									USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//关闭DTSABLE中断
									if(RxFlag1)
									{
//									OLED_Refresh();
					
									}
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
						}
						else   //接收错误
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //将存放数据数组清零
									}
						}
				} 
	
				else   //接收异常
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<10;i++)
						{
								RxBuffer1[i]=0x00;      //将存放数据数组清零
						}
				}
}
//void USART1_IRQHandler(void)                	//串口1中断服务程序
//	{

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
//		{
//			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		  Res=USART_ReceiveData(USART1);	//读取接收到的数据
//			OLED_ShowNum(0,0,Res,3,16);
//			USART_SendData(USART1,Res);//回传接收到的数据			
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送完毕
//			delay_ms(1);
//			USART_RX_STA=0;
//     }
//} 
	
void uart3_init(u32 bound)
{  
  GPIO_InitTypeDef 	GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef 	NVIC_InitStructure;
  DMA_InitTypeDef		DMA_InitStructure;
	 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 
  //USART3_RX	  GPIOB.11		(using)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
 
  //USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  USART_InitStructure.USART_BaudRate =bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
  USART_Init(USART3, &USART_InitStructure);
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART3, ENABLE); 
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

uint8_t temp_data = 0;
uint8_t TFmini_low,TFmini_high = 0;
uint8_t flag = 0;
uint16_t distance;
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
	{
		temp_data = USART_ReceiveData(USART3);
		
		if((temp_data == 0x59) && (flag == 0))
		{
			flag = 1;
		}
		else if((temp_data == 0x59) && (flag == 1))
		{
			flag = 2;
		}
		else if(flag == 2)
		{
			TFmini_low = temp_data;
			
			flag = 3;
		}
		else if(flag == 3)
		{
			TFmini_high = temp_data;
			
			distance = ((uint16_t)TFmini_high<<8) | ((uint16_t)TFmini_low);
			Distance=(u16)distance;
			OLED_ShowNum(0,4,Distance,3,16);
			flag = 0;
		}
	}
}






//void USART1_IRQHandler(void)                	//串口1中断服务程序
//	{
//  uint16_t distance;
//	uint8_t temp_data = 0;
//	

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
//		{
//		temp_data=USART_ReceiveData(USART1);
//		if((temp_data == 0x59) && (RX_Flag == 0))
//		{
//			RX_Flag = 1;
//		}
//		else if((temp_data == 0x59) && (RX_Flag == 1))
//		{
//			RX_Flag = 2;
//		}
//		else if(RX_Flag == 2)
//		{
//			TFmini_low = temp_data;
//			
//			RX_Flag = 3;
//		}
//		else if(RX_Flag == 3)
//		{
//			TFmini_high = temp_data;
//			
//			distance = ((uint16_t)TFmini_high<<8) | ((uint16_t)TFmini_low);
//			Distance = (u16)distance;
//			RX_Flag = 0;
//		}
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	}
//}


//void uart3_init(u32 bound)
//{
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
////	NVIC_InitTypeDef NVIC_InitStructure;
//	 
//	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOA时钟
//  
//	//USART1_TX   GPIOA.9
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10
//	
//	  //USART1_RX	  GPIOA.10初始化
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11  

//	//  //Usart1 NVIC 配置
////  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;//抢占优先级1
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
////	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
// 
//	USART_InitStructure.USART_BaudRate = bound;//串口波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//	
// 
//  USART_Init(USART3, &USART_InitStructure); 			//初始化串口3
//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//开启串口接收中断
//  USART_Cmd(USART3, ENABLE);                    	//使能串口3
//}








//void USART3_IRQHandler(void)
//{
//	uint16_t distance;
//	uint8_t TFmini_low,TFmini_high = 0;
//  uint8_t temp_data = 0;
//	
//	if(USART_GetITStatus(USART3, USART_IT_RXNE))//接收中断
//	{				
//		temp_data=USART_ReceiveData(USART3);
//		if((temp_data == 0x59) && (RX_Flag == 0))
//		{
//			RX_Flag = 1;
//		}
//		else if((temp_data == 0x59) && (RX_Flag == 1))
//		{
//			RX_Flag = 2;
//		}
//		else if(RX_Flag == 2)
//		{
//			TFmini_low = temp_data;
//			
//			RX_Flag = 3;
//		}
//		else if(RX_Flag == 3)
//		{
//			TFmini_high = temp_data;
//			
//			distance = ((uint16_t)TFmini_high<<8) | ((uint16_t)TFmini_low);
//			Distance = (u16)distance;
//			OLED_ShowNum(0,4,Distance,3,16);
//			RX_Flag = 0;
//		}
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//	}
//}


//void Uart2_Init(void)/*串口1初始化RXD 2.2 TXD 2.3 *//*串口1初始化RXD 3.2 TXD 3.3 */
//{
//    eUSCI_UART_Config uartConfig =
//    {
//        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
//        312,                                     // BRDIV = 312
//        8,                                       // UCxBRF = 8
//        0,                                       // UCxBRS = 0
//        EUSCI_A_UART_NO_PARITY,                  // No Parity
//        EUSCI_A_UART_LSB_FIRST,                  // LSB First
//        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
//        EUSCI_A_UART_MODE,                       // UART mode
//        1,  // Oversampling
//        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
//    };
//	
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

//    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
//    MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig);

//    /* Enable UART module */
//    MAP_UART_enableModule(EUSCI_A2_BASE);
//    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);//开启串口接收相关中断		
//    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);//开启串口A3中断
//    /* Enabling interrupts */
//}



//uint16_t distance;
//uint8_t temp_data = 0;
//uint8_t TFmini_low,TFmini_high = 0;
//uint8_t RX_Flag = 0;
//void EUSCIA2_IRQHandler(void)
//{
//    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
//    MAP_UART_clearInterruptFlag(EUSCI_A2_BASE,status);
//    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
//    {
//        temp_data = MAP_UART_receiveData(EUSCI_A2_BASE);
//		if((temp_data == 0x59) && (RX_Flag == 0))
//		{
//			RX_Flag = 1;
//		}
//		else if((temp_data == 0x59) && (RX_Flag == 1))
//		{
//			RX_Flag = 2;
//		}
//		else if(RX_Flag == 2)
//		{
//			TFmini_low = temp_data;
//			
//			RX_Flag = 3;
//		}
//		else if(RX_Flag == 3)
//		{
//			TFmini_high = temp_data;
//			
//			distance = ((uint16_t)TFmini_high<<8) | ((uint16_t)TFmini_low);
//			RX_Flag = 0;
//		}
//    }
//       
//}



//void USART3_IRQHandler(void)
//{
//	static u8 flag_data = 0;
//	static u8 index = 0;
//	u16 CheckSum = 0;
//	u8 i;
//	
//	if(USART_GetITStatus(USART3, USART_IT_RXNE))//接收中断
//	{				
//		if(USART_ReceiveData(USART3) == Data_Head)
//			flag_data = 1;
//		
//		if(flag_data)
//		{
//			USART3_RX_BUF[index ++] = USART_ReceiveData(USART3);
//			
//			//接收完毕进行数据校验
//			if(index == Data_Length)
//			{
//				//如果第一位和第二位是0x59
//				if((USART3_RX_BUF[0] == Data_Head) && (USART3_RX_BUF[1] == Data_Head))
//				{
//					//将接收到的数据累加
//					for(i = 0; i < (Data_Length - 1); i ++)
//					{
//						CheckSum += USART3_RX_BUF[i];
//					}
//					//进行CheckSum校验
//					if((CheckSum & 0x00ff) == USART3_RX_BUF[8])
//					{
//						//计算距离
//						Distance = USART3_RX_BUF[2] + USART3_RX_BUF[3] * 256;
//						//计算信号强度
//						Strength = USART3_RX_BUF[4] + USART3_RX_BUF[5] * 256;
//						//接收完成标志
//						flag_REC = 1;
//						count_REC ++;
//					}
//					index = 0;
//					flag_data = 0;
//				}
//			}
//		}
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//	}
//}


//void USART3_IRQHandler(void)
//{
//	uint16_t distance;
//	uint8_t temp_data = 0;
//	uint8_t TFmini_low,TFmini_high = 0;
//	uint8_t RX_Flag = 0;
//	
//	if(USART_GetITStatus(USART3, USART_IT_RXNE))//接收中断
//	{				
//		temp_data=USART_ReceiveData(USART3);
//		
//		if((temp_data == 0x59) && (RX_Flag == 0))
//		{
//			RX_Flag = 1;
//		}
//		else if((temp_data == 0x59) && (RX_Flag == 1))
//		{
//			RX_Flag = 2;
//		}
//		else if(RX_Flag == 2)
//		{
//			TFmini_low = temp_data;
//			
//			RX_Flag = 3;
//		}
//		else if(RX_Flag == 3)
//		{
//			TFmini_high = temp_data;
//			
//			distance = ((uint16_t)TFmini_high<<8) | ((uint16_t)TFmini_low);
//			RX_Flag = 0;
//		}
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//	}
//}