#include "usart33.h"
#include "led.h"
#include "delay.h"
#include "usart22.h"

u8 u3_flag=0;
u8 u3_count,Counter,xindex;
u8 RxBuffer[BUFFSIZE];
u8 TxBuffer[BUFFSIZE];


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
//	USART_ITConfig(USART3, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//使能或者失能指定的USART中断 接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	
	USART_ClearFlag(USART3,USART_FLAG_TC);//清除USARTx的待处理标志位
 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 	   //打开USART3的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	 //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 			//响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			 //使能
	NVIC_Init(&NVIC_InitStructure);

}

//串口3发送字符串函数
void UART3_SendString(char* s)
{
	while(*s)//检测字符串结束符
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); 
		USART_SendData(USART3 ,*s++);//发送当前字符
	}
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
}

//串口3发送字符串函数
//void UART3_SendString(char *pstr)
//{
//	char *p = pstr;
//	while(*p!='\0')
//	{
//		USART_SendData(USART3, *p);
//		while( USART_GetFlagStatus(USART3, USART_FLAG_TXE)==RESET);//等待发送完成
//		USART_ClearFlag(USART3,USART_FLAG_TXE);//清空标志位、
//		p++;  //指针偏移
//	}
//}

void USART3_IRQHandler(void)                	//串口2中断服务程序
	{

		
}

void u3_printf(char* str)
{
	u3_count = strlen(str);
	strcpy((char* )TxBuffer,str);
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}
