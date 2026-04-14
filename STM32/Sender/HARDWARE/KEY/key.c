#include "key.h"
#include "BEEP.h"
#include "oled.h"
//按键为PA 8/11/12/15/1.
//按键上拉

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOA时钟
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//PB12/13/14/15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
} 

u8 KEY_Scan(void)
{
	if(!key_1)//run键按下
		{	
			while(key_1==0);//等待按键松开
			delay_ms(5);//延时消抖	
			return 1;
		}
	if(!key_2)//shift键按下
		{	
			while(key_2==0);//等待按键松开
			delay_ms(5);//延时消抖	
			return 2;
		}
	if(!key_3)//beep键按下
		{	
			while(key_3==0);//等待按键松开
			delay_ms(5);//延时消抖	
			return 3;
		}
	if(!key_4)//up键按下
		{	
			while(key_4==0);//等待按键松开
			delay_ms(5);//延时消抖	
			return 4;
		}
		return 0;
}


