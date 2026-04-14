#ifndef _usart33_h
#define _usart33_h

#include "sys.h"
#include "usart.h"
#include "stdarg.h"	 	 
#include "stdio.h"
#include "string.h"

#define BUFFSIZE 128

void uart3_init(u32 bound);
void u3_printf(char* str);
 extern u32 DS;
 extern   u32 BYTE_H,BYTE_M,BYTE_L;  	
#endif 
