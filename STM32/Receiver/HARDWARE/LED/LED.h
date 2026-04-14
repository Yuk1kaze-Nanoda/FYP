#ifndef __LED_H
#define __LED_H
#include "sys.h"


#define LED  PCout(13)
#define FAN   PBout(7)
#define LED1  PBout(5)

void LED_GPIO_Init(void);
#endif

