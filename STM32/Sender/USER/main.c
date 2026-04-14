#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "oled.h"
#include "usart.h"
#include "stdio.h"
#include "LED.h"
#include "dht11.h"
#include "adc.h"
#include "Motor.h"


/*
起点科技
*/

u8 i,j;
u8 temp,humi,mq2,light;
u8 warn_flag,dj_flag;

u8 buffdata1[9]={0xFE,0x05,0xA0,0xA0,0x99,0x8A,0,0xFF,0xF1}; //发送温度
u8 buffdata2[9]={0xFE,0x05,0xA1,0xA1,0x99,0x8A,0,0xFF,0xF1}; //发送湿度
u8 buffdata3[9]={0xFE,0x05,0xA2,0xA2,0x99,0x8A,0,0xFF,0xF1}; //发送mq2
u8 buffdata4[9]={0xFE,0x05,0xA3,0xA3,0x99,0x8A,0,0xFF,0xF1}; //发送亮度

int main(void)
{	
	delay_init();
	delay_ms(50);
//	Usart1_Init(115200);	//WIFI
	Usart2_Init(115200);	//WIFI
	LED_GPIO_Init();
	Adc_Init();
	Motor_Init();
//	OLED_Init();
//	OLED_Clear();
//	OLED_ShowCH(0,0,"温度");
	DHT11_Init();
//	while(1)
//	{
//		LED=0;
//		LED2=0;
//		for(j=0;j<100;j++)
//		{
//			Motorcw();
//		}
//		GPIO_SetBits(GPIOA,GPIO_Pin_7);//依次高电平输出
//		GPIO_SetBits(GPIOA,GPIO_Pin_4);//依次高电平输出
//		GPIO_SetBits(GPIOA,GPIO_Pin_5);//依次高电平输出
//		GPIO_SetBits(GPIOA,GPIO_Pin_6);//依次高电平输出
//		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
//	}
	while(1)
	{
		DHT11_Read_Data();		//温湿度计时变量 	
		temp=DHT11Data[2];
		humi=DHT11Data[0];
		mq2=(Get_Adc_Average(8,10))/41;
		light=100-(Get_Adc_Average(0,10))/41;
		buffdata1[6]=temp;
		buffdata2[6]=humi;
		buffdata3[6]=mq2;
		buffdata4[6]=light;
//		OLED_ShowNum(65, 0, temp, 4,1);

		
		if(PAin(11)==0 ||mq2>60)
		{
			BEEP=1;
		}
		else
		{
			BEEP=0;
		}
		
		if(LED)
		{
			if(light<20)
				LED2=0;
			else
				LED2=1;
			if(temp>32 ||humi>80)
				FAN=1;
				else
					FAN=0;
		}
		
						
		while(buffdata1[i]!=0xF1)  // 每个字符串结尾 都是以   结尾的
			{
				USART_SendData(USART2,buffdata1[i]);      //通过库函数  发送数据
				while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
				//等待发送完成。  检测 USART_FLAG_TC 是否置1；   //见库函数 P359 介绍
				i++;              //i++一次
			}
			i=0;
			delay_ms(100);
			
		while(buffdata2[i]!=0xF1)  // 每个字符串结尾 都是以1111111111111111111111111111111111111   结尾的
			{
				USART_SendData(USART2,buffdata2[i]);      //通过库函数  发送数据
				while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
				//等待发送完成。  检测 USART_FLAG_TC 是否置1；   //见库函数 P359 介绍
				i++;              //i++一次
			}
			i=0;
			delay_ms(100);
			
		while(buffdata3[i]!=0xF1)  // 每个字符串结尾 都是以   结尾的
			{
				USART_SendData(USART2,buffdata3[i]);      //通过库函数  发送数据
				while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
				//等待发送完成。  检测 USART_FLAG_TC 是否置1；   //见库函数 P359 介绍
				i++;              //i++一次
			}
			i=0;
			delay_ms(100);
			
		while(buffdata4[i]!=0xF1)  // 每个字符串结尾 都是以   结尾的
			{
				USART_SendData(USART2,buffdata4[i]);      //通过库函数  发送数据
				while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
				//等待发送完成。  检测 USART_FLAG_TC 是否置1；   //见库函数 P359 介绍
				i++;              //i++一次
			}
			i=0;
			delay_ms(1000);
	}
}


