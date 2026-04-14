#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"


#define BEEP  PCout(14)

/* 定义SPI连接的GPIO端口, 用户只需要修改下面代码即可任意改变相应的引脚*/
#define BEEP_GPIO_PORT	 GPIOC			            /* GPIO端口 */
#define BEEP_RCC 	     RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define BEEP_PIN		     GPIO_Pin_14

void BEEP_GPIO_Init(void);

#endif

