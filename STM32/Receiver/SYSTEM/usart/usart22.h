#ifndef __USART2_H
#define __USART2_H	
#include "stm32f10x.h"
#include "stdio.h"      //包含需要的头文件
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"     //包含需要的头文件
//////////////////////////////////////////////////////////////////////////////////	 

////////////////////////////////////////////////////////////////////////////////// 	
#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	
#define USART2_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节


//如果想串口中断接收，请不要注释以下宏定义
void uart2_init(u32 bound);
void u2_printf(char*,...) ;   
#endif


