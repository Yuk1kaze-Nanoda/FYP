//硬件驱动
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "led.h"
#include "Motor.h"

//C库
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/*
************************************************************
*	函数名称：	Usart1_Init
*
*	函数功能：	串口1初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PA9		RX-PA10
************************************************************
*/
void Usart1_Init(unsigned int baud)
{

	GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//PA9	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_9;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	//PA10	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_10;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	usart_initstruct.USART_BaudRate = baud;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
	usart_initstruct.USART_Parity = USART_Parity_No;									//无校验
	usart_initstruct.USART_StopBits = USART_StopBits_1;								//1位停止位
	usart_initstruct.USART_WordLength = USART_WordLength_8b;							//8位数据位
	USART_Init(USART1, &usart_initstruct);
	
	USART_Cmd(USART1, ENABLE);														//使能串口
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//使能接收中断
	
	nvic_initstruct.NVIC_IRQChannel = USART1_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic_initstruct);

}

/*
************************************************************
*	函数名称：	Usart2_Init
*
*	函数功能：	串口2初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PA2		RX-PA3
************************************************************
*/
void Usart2_Init(unsigned int baud)
{

	GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//PA2	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_2;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	//PA3	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_3;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	usart_initstruct.USART_BaudRate = baud;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
	usart_initstruct.USART_Parity = USART_Parity_No;									//无校验
	usart_initstruct.USART_StopBits = USART_StopBits_1;								//1位停止位
	usart_initstruct.USART_WordLength = USART_WordLength_8b;							//8位数据位
	USART_Init(USART2, &usart_initstruct);
	
	USART_Cmd(USART2, ENABLE);														//使能串口
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//使能接收中断
	
	nvic_initstruct.NVIC_IRQChannel = USART2_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_initstruct);

}

void Usart3_Init(unsigned int baud)
{
	GPIO_InitTypeDef gpio_initstruct;	//声明一个结构体变量，用来初始化GPIO
	USART_InitTypeDef usart_initstruct;	  //串口结构体定义
	NVIC_InitTypeDef nvic_initstruct;//中断结构体定义
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //打开时钟
	
	/*  配置GPIO的模式和IO口 */ //PB10	TXD
	gpio_initstruct.GPIO_Pin=GPIO_Pin_10;//TX			   //串口输出
	gpio_initstruct.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_initstruct.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOB,&gpio_initstruct);  /* 初始化串口输入IO */
	
	gpio_initstruct.GPIO_Pin=GPIO_Pin_11;//RX			 //串口输入
	gpio_initstruct.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_initstruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOB,&gpio_initstruct); /* 初始化GPIO */
	
	usart_initstruct.USART_BaudRate=baud;   //波特率设置为9600	//波特率
	usart_initstruct.USART_WordLength=USART_WordLength_8b;		//数据长8位
	usart_initstruct.USART_StopBits=USART_StopBits_1;			//1位停止位
	usart_initstruct.USART_Parity=USART_Parity_No;				//无效验
	usart_initstruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //失能硬件流
	usart_initstruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	 //开启发送和接受模式
	USART_Init(USART3,&usart_initstruct);	/* 初始化USART1 */
	
	
	USART_Cmd(USART3, ENABLE);		   /* 使能USART1 */
	USART_ITConfig(USART3, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//使能或者失能指定的USART中断 接收中断
	USART_ClearFlag(USART3,USART_FLAG_TC);//清除USARTx的待处理标志位
 
	nvic_initstruct.NVIC_IRQChannel = USART3_IRQn; 	   //打开USART3的全局中断
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0; 	 //抢占优先级为0
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0; 			//响应优先级为1
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE; 			 //使能
	NVIC_Init(&nvic_initstruct);

}

/*
************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}

}

/*
************************************************************
*	函数名称：	UsartPrintf
*
*	函数功能：	格式化打印
*
*	入口参数：	USARTx：串口组
*				fmt：不定长参
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void UsartPrintf(USART_TypeDef* USARTx, const char* format, ...) {
    char buffer[296];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args); // 格式化字符串
    va_end(args);

    for (int i = 0; buffer[i] != '\0'; i++) {
        while (!(USARTx->SR & USART_SR_TXE)); // 等待发送缓冲区为空
        USARTx->DR = (buffer[i] & 0xFF);    // 发送字符
    }
}

/*
************************************************************
*	函数名称：	USART1_IRQHandler
*
*	函数功能：	串口1收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
u8 usart1_rx_buf[RX_BUF_SIZE];
u16 usart1_rx_index = 0;
extern u8 warn_flag;
extern u8 dj_flag;
extern u8 j;

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志
			usart1_rx_buf[usart1_rx_index] = USART_ReceiveData(USART1);     //接收串口1数据到buff缓冲区
			usart1_rx_index++; 
			if(usart1_rx_buf[usart1_rx_index-1] == 0xFF || usart1_rx_index == RX_BUF_SIZE)    //如果接收到尾标识是换行符（或者等于最大接受数就清空重新接收）
			{
				if(usart1_rx_buf[0] == 0xFE&&usart1_rx_buf[2] == 0xEE)                      //检测到头标识是我们需要的 
				 {
					 LED=!LED;
					 usart1_rx_index=0;                                   
				 }
				
				else if(usart1_rx_buf[0] == 0xFE&&usart1_rx_buf[2] == 0xAA&&LED==0)                      //检测到头标识是我们需要的 
				 {
//					 LED1=!LED1;
					 usart1_rx_index=0;                                   
				 }
				 else if(usart1_rx_buf[0] == 0xFE&&usart1_rx_buf[2] == 0xBB &&LED==0)                      //检测到头标识是我们需要的 
				 {
					 FAN=!FAN;
					 usart1_rx_index=0;                                   
				 }
				 else if(usart1_rx_buf[0] == 0xFE&&usart1_rx_buf[2] == 0xCC &&LED==0)                      //检测到头标识是我们需要的 
				 {
					 for(j=0;j<100;j++)
					{
						Motorcw();
					}
					 usart1_rx_index=0;                                   
				 }
				 else if(usart1_rx_buf[0] == 0xFE&&usart1_rx_buf[2] == 0xDD &&LED==0)                      //检测到头标识是我们需要的 
				 {
					  for(j=0;j<100;j++)
					{
						Motorfw();
					}
					 usart1_rx_index=0;                                   
				 }
				 else
				 {					 
					usart1_rx_index=0;                                   //不是我们需要的数据或者达到最大接收数则开始重新接收
				 }
			 }
		}
}

u8 usart2_rx_buf[RX2_BUF_SIZE];
u16 usart2_rx_index = 0;
extern u8 temp,humi,mq2,light,mq2th;

void USART2_IRQHandler(void)                	//串口2中断服务程序
	{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志
			usart2_rx_buf[usart2_rx_index] = USART_ReceiveData(USART2);     //接收串口1数据到buff缓冲区
			usart2_rx_index++; 
			if(usart2_rx_buf[usart2_rx_index-1] == 0xFF || usart2_rx_index == RX2_BUF_SIZE)    //如果接收到尾标识是换行符（或者等于最大接受数就清空重新接收）
			{
				 if(usart2_rx_buf[0] == 0xFE&&usart2_rx_buf[2] == 0xEE)                      //检测到头标识是我们需要的 
				 {
					 LED=!LED;
					 usart2_rx_index=0;                                   
				 }
				 else if(usart2_rx_buf[0] == 0xFE&&usart2_rx_buf[2] == 0xAA&&LED==0)                      //检测到头标识是我们需要的 
				 {
					 LED2=!LED2;
					 usart2_rx_index=0;                                   
				 }
				else if(usart2_rx_buf[0] == 0xFE&&usart2_rx_buf[2] == 0xBB&&LED==0)                      //检测到头标识是我们需要的 
				 {
					 FAN=!FAN;
					 usart2_rx_index=0;                                   
				 }
				 else if(usart2_rx_buf[0] == 0xFE&&usart2_rx_buf[2] == 0xCC&&LED==0)                      //检测到头标识是我们需要的 
				 {
					  for(j=0;j<100;j++)
					{
						Motorfw();
					}
					 usart2_rx_index=0;                                   
				 }
				 else if(usart2_rx_buf[0] == 0xFE&&usart2_rx_buf[2] == 0xDD&&LED==0)                      //检测到头标识是我们需要的 
				 {
					  for(j=0;j<100;j++)
					{
						Motorcw();
					}
					 usart2_rx_index=0;                                   
				 }
				 else
				 { 
					usart2_rx_index=0;                                   //不是我们需要的数据或者达到最大接收数则开始重新接收
				 }
			 }
		}
	}
	

u8 usart3_rx_buf[RX3_BUF_SIZE];
u16 usart3_rx_index = 0;

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收中断
	{
		u8 ch = USART_ReceiveData(USART3);
		usart3_rx_buf[0]=ch;
		if(usart3_rx_buf[0]=='A')
			{
				usart3_rx_buf[0]=0;
			}
		else if(usart3_rx_buf[0]=='B')
			{
				usart3_rx_buf[0]=0;
			}
		else if(usart3_rx_buf[0]=='C')
			{
				usart3_rx_buf[0]=0;
			}
		else if(usart3_rx_buf[0]=='D')
			{
				usart3_rx_buf[0]=0;
			}			
			else
				usart3_rx_buf[0]=0;
		}
}
	



