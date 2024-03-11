#include "usart.h"
#include "sys.h"
#include "oled.h"
//u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//u16 USART3_RX_STA = 0;       //����״̬���	 
  u16 Distance = 0, Strength = 0;
//	uint8_t RX_Flag = 0;
	u16 USART_RX_STA=0;       //����״̬���	  
  u8 Res=80,Cx=80,Cy=60; 
	u8 Openmv_X =80,
	   Openmv_Y = 60;
//	uint8_t TFmini_low,TFmini_high = 0;
//	u8 flag_REC = 0;
//	u8 count_REC = 0;
void uart1_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}
void USART1_IRQHandler(void)			 
{
		int com_data1; 
		if( USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  	   //�����ж�  
		{
				//����жϱ�־
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
		if(RxState==0&&com_data==0xDC)  //0x2c֡ͷ����ֹ���ܲ���Ҫ�����֣�����128������Ϊ1~8
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;
				//	OLED_Refresh();
				}
		
				else if(RxState==1&&com_data==0xF3)  //0x12֡ͷ
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=com_data;
					
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1>=10||com_data == 0xE8)       //RxBuffer1��������,�������ݽ���
					{
						RxState=3;
						RxFlag1=1;
						Cx=RxBuffer1[RxCounter1-3];
						Cy=RxBuffer1[RxCounter1-2];           
					}
				}
		
				else if(RxState==3)		//����Ƿ���ܵ�������־
				{
						if(RxBuffer1[RxCounter1-1] == 0xE8)
						{
									USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//�ر�DTSABLE�ж�
									if(RxFlag1)
									{
//									OLED_Refresh();
					
									}
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
						}
						else   //���մ���
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //�����������������
									}
						}
				} 
	
				else   //�����쳣
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<10;i++)
						{
								RxBuffer1[i]=0x00;      //�����������������
						}
				}
}
//void USART1_IRQHandler(void)                	//����1�жϷ������
//	{

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
//		{
//			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		  Res=USART_ReceiveData(USART1);	//��ȡ���յ�������
//			OLED_ShowNum(0,0,Res,3,16);
//			USART_SendData(USART1,Res);//�ش����յ�������			
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ��������
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






//void USART1_IRQHandler(void)                	//����1�жϷ������
//	{
//  uint16_t distance;
//	uint8_t temp_data = 0;
//	

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
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
//	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
//  
//	//USART1_TX   GPIOA.9
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10
//	
//	  //USART1_RX	  GPIOA.10��ʼ��
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11  

//	//  //Usart1 NVIC ����
////  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;//��ռ���ȼ�1
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
////	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
// 
//	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
//	
// 
//  USART_Init(USART3, &USART_InitStructure); 			//��ʼ������3
//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//�������ڽ����ж�
//  USART_Cmd(USART3, ENABLE);                    	//ʹ�ܴ���3
//}








//void USART3_IRQHandler(void)
//{
//	uint16_t distance;
//	uint8_t TFmini_low,TFmini_high = 0;
//  uint8_t temp_data = 0;
//	
//	if(USART_GetITStatus(USART3, USART_IT_RXNE))//�����ж�
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


//void Uart2_Init(void)/*����1��ʼ��RXD 2.2 TXD 2.3 *//*����1��ʼ��RXD 3.2 TXD 3.3 */
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
//    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);//�������ڽ�������ж�		
//    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);//��������A3�ж�
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
//	if(USART_GetITStatus(USART3, USART_IT_RXNE))//�����ж�
//	{				
//		if(USART_ReceiveData(USART3) == Data_Head)
//			flag_data = 1;
//		
//		if(flag_data)
//		{
//			USART3_RX_BUF[index ++] = USART_ReceiveData(USART3);
//			
//			//������Ͻ�������У��
//			if(index == Data_Length)
//			{
//				//�����һλ�͵ڶ�λ��0x59
//				if((USART3_RX_BUF[0] == Data_Head) && (USART3_RX_BUF[1] == Data_Head))
//				{
//					//�����յ��������ۼ�
//					for(i = 0; i < (Data_Length - 1); i ++)
//					{
//						CheckSum += USART3_RX_BUF[i];
//					}
//					//����CheckSumУ��
//					if((CheckSum & 0x00ff) == USART3_RX_BUF[8])
//					{
//						//�������
//						Distance = USART3_RX_BUF[2] + USART3_RX_BUF[3] * 256;
//						//�����ź�ǿ��
//						Strength = USART3_RX_BUF[4] + USART3_RX_BUF[5] * 256;
//						//������ɱ�־
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
//	if(USART_GetITStatus(USART3, USART_IT_RXNE))//�����ж�
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