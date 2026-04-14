
#ifndef __CT511_H
#define __CT511_H	 
#include "sys.h"


	
#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

void CT511_Init(void);//初始化
void CT511_CONNECT_MQTT_SERVER(char *PRODUCTKEY,char *DEVICENAME,char *DEVICESECRET);
void CT511_MQTT_SEND_DATA(char *PRODUCTKEY,char *DEVICENAME,char *DATA);	

_Bool CT511_SendCmd(char *cmd, char *res, u16 time);
void CT511_SendMsg(char *phone,char *txt);
void CT511_CallPhone(char *phone);
#endif
