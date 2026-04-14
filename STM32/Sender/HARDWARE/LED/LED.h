#ifndef __LED_H
#define __LED_H
#include "sys.h"


#define LED  PCout(13)
//#define LED1  PBout(5)
#define LED2  PAout(1)
#define BEEP  PAout(12)
#define FAN  PBout(7)

void LED_GPIO_Init(void);
#endif

