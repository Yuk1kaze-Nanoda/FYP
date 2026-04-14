#ifndef _usart33_h
#define _usart33_h

#include "sys.h"
#include "usart.h"
#include "stdarg.h"	 	 
#include "stdio.h"
#include "string.h"

#define BUFFSIZE 128


//外部引用串口接收数据完成的变量
extern u8 U3_RX_BUF[1024];  //接收缓冲区 
extern  u16  U3_RX_Cnt;    //接收计数
extern u8  U3_RX_Cmd ;      //接收命令到标志位 U3_RX_Cmd = 1
extern u8  U3_RX_Error;    // 接收到错误


void uart3_init(u32 bound);
void u3_printf(char* str);
 

void UART3_SendString(char* s);
#endif 
