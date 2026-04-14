#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "BEEP.h"
#include "key.h"
#include "DS1302.h"
#include "oled.h"
#include "SG90.h"
#include "usart.h"
#include "usart22.h" 
#include "usart33.h" 
#include "DHT11.h"
#include "adc.h"
#include "stdio.h"
#include "INIT.h"
#include "Motor.h"
#include "LED.h"
#include "LD3320.h"
#include "HX711.h"


u8 i;
u16 buffdx[1]={0x1A};      //发完短信后，必须发一个0x1a才能发送出去

	//如下是智能药盒药量初始化
	extern u8 flag_hw;
	u8 flag_moshi0 = 0;	//显示模式
	u8 flag_moshi1 = 0;	//调节阈值
	u8 flag_moshi2 = 0;	//控制模式
	u8 flag_moshi3 = 0; 
	u8 flag_LED1 = 0; 
	u8 flag_FAN = 0; 
	u8 flag_MOTOR = 0; 
	u8 Light_yuzhi = 30;
	u8 MQ2_yuzhi = 80;
	u8 Humi_yuzhi = 70;
	u8 Temp_yuzhi = 38;
	//--------------------------
	u8 Temp,Humi;
	int  adcdata[3],MQ2=0,Light=0,MQ315=0;

void Init_all(void)
{		
	delay_init();
	delay_ms(50);
	DHT11_Init();
	uart_init(115200);
	uart2_init(9600);
	BEEP_GPIO_Init();
	LED_GPIO_Init();
	FAN_GPIO_Init();
	Motor_Init();
	Adc_Init();
	KEY_Init(); 			//按键初始化
	OLED_Init();
	OLED_Clear();
	LED1=1;
	LED2=1;
	LED=1;
}

void DHT11_func(void)
{
	DHT11_Read_Data();		//读取温度湿度
	Temp=DHT11Data[2];
	Humi=DHT11Data[0];
	
	if (flag_moshi0 == 0 && flag_moshi1 == 0 && flag_moshi2 == 0 && flag_moshi3 == 0 )
	{
		OLED_ShowCH(0,4,"湿度:");		//显示中文：距离
		OLED_ShowCH(80,4,"%");		//测试显示中文
		OLED_ShowNum(52,4,Humi,3,12);//显示ASCII字符的码值 
		
		OLED_ShowCH(0,6,"温度:");		//显示中文：温度		
		OLED_ShowCH(80,6,"C");		//显示：℃			
		OLED_ShowNum(52,6,Temp,3,12);//显示ASCII字符的码值 
	}
}

void MQ2_func(void)
{
//	adcdata[0] = (Get_Adc_Average(8,10));
//	light= 300-adcdata[0]/10;

	adcdata[0] = (Get_Adc_Average(8,10));	
	MQ2= adcdata[0];
	
	if(flag_moshi0 == 0 && flag_moshi1 == 0 && flag_moshi2 == 0 && flag_moshi3 == 0)
	{
		OLED_ShowCH(0,2,"MQ2:");		//显示中文：距离
		OLED_ShowCH(80,2,"PPM");		//测试显示中文
		OLED_ShowNum(52,2,MQ2,3,12);//显示ASCII字符的码值
	}
	
	
}

	void Light_func(void)	//空气质量AQI
	{
		adcdata[1] = (Get_Adc_Average(0,10));	
		Light= 100-adcdata[1]/41;
		
		if(flag_moshi0 == 0 && flag_moshi1 == 0 && flag_moshi2 == 0 && flag_moshi3 == 0)
		{
			OLED_ShowCH(0,0,"Light:");		//显示中文：距离
			OLED_ShowCH(80,0,"Lux");		//测试显示中文
			OLED_ShowNum(52,0,Light,3,0);//显示ASCII字符的码值
			u2_printf("%d",Light);
		}
	}
	
