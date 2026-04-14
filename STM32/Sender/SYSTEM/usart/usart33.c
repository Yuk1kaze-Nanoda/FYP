#include "usart33.h"
#include "led.h"

u8 u3_flag=0;
u8 u3_count,Counter,xindex;
u8 RxBuffer[BUFFSIZE];
u8 TxBuffer[BUFFSIZE];
 u32 BYTE_H,BYTE_M,BYTE_L;
u32 DS;
void uart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//声明一个结构体变量，用来初始化GPIO
	USART_InitTypeDef  USART_InitStructure;	  //串口结构体定义
	NVIC_InitTypeDef NVIC_InitStructure;//中断结构体定义
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //打开时钟
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//TX			   //串口输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);  /* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RX			 //串口输入
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOB,&GPIO_InitStructure); /* 初始化GPIO */

	USART_InitStructure.USART_BaudRate=bound;   //波特率设置为9600	//波特率
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;		//数据长8位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1位停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;				//无效验
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //失能硬件流
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	 //开启发送和接受模式
	USART_Init(USART3,&USART_InitStructure);	/* 初始化USART1 */
	USART_Cmd(USART3, ENABLE);		   /* 使能USART1 */
	USART_ITConfig(USART3, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//使能或者失能指定的USART中断 接收中断
	USART_ClearFlag(USART3,USART_FLAG_TC);//清除USARTx的待处理标志位
 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 	   //打开USART3的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	 //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 			//响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			 //使能
	NVIC_Init(&NVIC_InitStructure);

}

void USART3_IRQHandler(void)                	//串口2中断服务程序
	{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
	//	Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
			USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清除中断标志
		//Uart1_Rx=0;
				RxBuffer[Counter] = USART_ReceiveData(USART3);     //接收串口1数据到buff缓冲区
				Counter++; 
		//	DS++;
			BYTE_H=RxBuffer[0];
			BYTE_M=RxBuffer[1];
			BYTE_L=RxBuffer[2];
			DS=((RxBuffer[0]<<16)+(RxBuffer[1]<<8)+RxBuffer[2])/10000;
			if(Counter>2)Counter=0;
     } 
}

void u3_printf(char* str)
{
	u3_count = strlen(str);
	strcpy((char* )TxBuffer,str);
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}
