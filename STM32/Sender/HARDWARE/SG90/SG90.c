#include "SG90.h"
#include "delay.h"	

//==========================舵机=====================================

void SG90_Init(void)	//舵机接口初始化
	{ 
		GPIO_InitTypeDef  GPIO_InitStructure; 	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能端口时钟
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);  
//		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		
		GPIO_InitStructure.GPIO_Pin = SE_OUT; //选择端口号（0~15或all）                        
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
		GPIO_Init(SE_PORT, &GPIO_InitStructure);
		GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(0)); //接口输出
		SG90_angle(0);
	}

void SG90_angle(u8 a)	//舵机角度控制设置（参数值0～180）对应角度0～180度
	{ 
		u8 b=100;//角度校正偏移量
		GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(1)); //接口输出
		delay_us(500+a*10+b); //延时
		GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(0)); //接口输出
		delay_us(19500-a*10-b); //延时
		GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(1)); //接口输出
		delay_us(500+a*10+b); //延时
		GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(0)); //接口输出
		delay_us(19500-a*10-b); //延时
	} 
