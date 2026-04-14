#ifndef __MQTT_H_
#define __MQTT_H_

#include "sys.h"
#include "stdio.h"


//WIFI配置
#define WIFI_NAME	            "DDDAAA"           		//wifi名称 
#define WIFI_PASSWD             "sdesda7788"        //wifi密码



//阿里云服务器的登陆配置 STM32
#define MQTT_BROKERADDRESS		"a19mET0bQ9o.iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define MQTT_CLIENTID 			"FESA234FBDS24|securemode=3,signmethod=hmacsha1,timestamp=789|"
#define MQTT_USARNAME 			"STM32&a19mET0bQ9o"
#define MQTT_PASSWD 			"afd2b94328490ea090af09a76fe4e071bbea18d4"


#define	MQTT_PUBLISH_TOPIC 		"/sys/a19mET0bQ9o/STM32/thing/event/property/post"  //在头文件中添加订阅和发布的话题
#define MQTT_SUBSCRIBE_TOPIC	"/sys/a19mET0bQ9o/STM32/thing/service/property/set"

//////阿里云服务器的登陆配置 appdate
//#define MQTT_BROKERADDRESS		"a1ku3iLsSpD.iot-as-mqtt.cn-shanghai.aliyuncs.com"
//#define MQTT_CLIENTID 			"a1ku3iLsSpD.appdata|securemode=2,signmethod=hmacsha256,timestamp=1698815513084|"
//#define MQTT_USARNAME 			"appdata&a1ku3iLsSpD"
//#define MQTT_PASSWD 			"bc5b73d12efa09da9ad3cc97a40c32775441a648849e15e2433717ab6e95c35b"


//#define	MQTT_PUBLISH_TOPIC 		"/sys/a1ku3iLsSpD/appdata/thing/event/property/post"  //在头文件中添加订阅和发布的话题
//#define MQTT_SUBSCRIBE_TOPIC	"/sys/a1ku3iLsSpD/appdata/thing/service/property/set"

void MQTT_Init(void);

typedef struct
{
	u8 *rxbuf;u16 rxlen;
	u8 *txbuf;u16 txlen;
	void (*Init)(u8 *prx,u16 rxlen,u8 *ptx,u16 txlen);
	u8 (*Connect)(char *ClientID,char *Username,char *Password);
	u8 (*SubscribeTopic)(char *topic,u8 qos,u8 whether);
	u8 (*PublishData)(char *topic, char *message, u8 qos);
	void (*SendHeart)(void);
	void (*Disconnect)(void);
}_typdef_mqtt;

extern _typdef_mqtt _mqtt;

#endif

