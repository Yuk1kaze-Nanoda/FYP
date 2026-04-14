#include "LED.h"

void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	 //设置输出引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz	
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 //初始化GPIO	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |GPIO_Pin_5;	 //设置输出引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	 //初始化GPIO

	LED=1;
}

