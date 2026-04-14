#ifndef STM_KEY_H
#define STM_KEY_H 			   
#include "sys.h" 
#include "delay.h"

#define key_1 PBin(12)
#define key_2 PBin(13)
#define key_3 PBin(14)
#define key_4 PBin(15)

void KEY_Init(void);
u8 KEY_Scan(void);
#endif
