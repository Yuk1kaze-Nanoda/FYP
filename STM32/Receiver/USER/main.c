#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "oled.h"
#include "usart.h"
#include "stdio.h"
#include "LED.h"
#include "key.h"

/*
起点科技
*/

u8 temp,humi,mq2,light;
u8 mq2th=30;
extern u8 i;
u8 j;

extern u8 buffdata1[9]; //发送状态


int main(void)
{	
	delay_init();
	delay_ms(50);
	Usart1_Init(115200);	//WIFI
	Usart2_Init(115200);	//WIFI
	LED_GPIO_Init();
	KEY_Init(); 			//按键初始化
	OLED_Init();
	OLED_Clear();
	OLED_ShowCH(0,0,"温度");
	OLED_ShowCH(64,0,"湿度");
	OLED_ShowCH(0,2,"空气");
	OLED_ShowCH(64,2,"亮度");
//	OLED_ShowCH(0,4,"阈值");
	while(1)
	{
		j++;
		if(j==100)
		{
			j=0;
			UsartPrintf(USART1,"cmd=2&uid=4d9ec352e0376f2110a0c601a2857225&topic=temp&msg=#%d#%d#%d#%d#\r\n",temp,humi,mq2,light);

		}

		delay_ms(10);
	}
}


