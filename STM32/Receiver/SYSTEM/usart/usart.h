#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"


#define USART_DEBUG		USART3		//调试打印所使用的串口组
#define BUFFSIZE 128
#define RX_BUF_SIZE 128
#define RX2_BUF_SIZE 128
#define RX3_BUF_SIZE 128

void Usart1_Init(unsigned int baud);

void Usart2_Init(unsigned int baud);

void Usart3_Init(unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef* USARTx, const char* format, ...);

#endif
