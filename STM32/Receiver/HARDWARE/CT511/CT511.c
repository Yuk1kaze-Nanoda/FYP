/*
 * @Author: your name
 * @Date: 2020-04-06 02:16:49
 * @LastEditTime: 2020-04-06 02:18:40
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \undefinedc:\Users\Administrator\Desktop\sunjianping\HARDWARE\BEEP\beep.c
 */

#include "CT511.h"
 #include "usart.h"
#include "delay.h"
#include "usart.h"
#include "usart22.h"
#include "usart33.h"
#include "oled.h"
#include "led.h"

char AtStrBuf[255];   								//打印缓存器
#define BUFLEN 255
unsigned short U3_RX_CntPre = 0;

//BEEP IO初始化
void CT511_Init(void)
{
	//1)AT指令入网、模块测试	
	while(CT511_SendCmd("AT\r\n", "OK", 200))          //测试指令，若返回OK，则模块可正常通信
		delay_ms(500);
	while(CT511_SendCmd("AT+CPIN?\r\n", "READY", 200))  //检查SIM卡是否在位
		delay_ms(500);
	while(CT511_SendCmd("AT+CSQ\r\n", "+CSQ", 200))     //查询信号质量
		delay_ms(500);
	while(CT511_SendCmd("AT+CREG?\r\n", "0,1", 200))    //查询入网状态
		delay_ms(500);
}

//////////************************ CT511N模块**********************///////////////////////
void CT511_CONNECT_MQTT_SERVER(char *PRODUCTKEY,char *DEVICENAME,char *DEVICESECRET)
{

	while(CT511_SendCmd("AT+MDISCONNECT\r\n", "OK", 200))
		delay_ms(500);
		while(CT511_SendCmd("ATE0\r\n", "OK", 200))
		delay_ms(500);
		while(CT511_SendCmd("AT+NETOPEN\r\n", "NETOPEN:SUCCESS", 200))
		delay_ms(500);
	
			//配置阿里云上的设备信息
	memset(AtStrBuf,0,255);
    sprintf(AtStrBuf,"AT+MCONFIG=\"%s\",\"%s\",\"%s\"\r\n",PRODUCTKEY,DEVICENAME,DEVICESECRET);
	while(CT511_SendCmd(AtStrBuf,"OK",200))	
		delay_ms(500);

	
			//打开一个MQTT客户端网络
	while(CT511_SendCmd("AT+MIPSTART=\"a19mET0bQ9o.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,4\r\n", "MIPSTART: SUCCESS", 300))
		delay_ms(500);
	
	while(CT511_SendCmd("AT+MCONNECT=1,60\r\n", "MCONNECT: SUCCESS", 200))
		delay_ms(500);
	//订阅服务器发布的主题
	memset(AtStrBuf,0,255);
	sprintf(AtStrBuf,"AT+MSUB=\"/sys/%s/%s/thing/service/property/set\",0\r\n","a19mET0bQ9o","STM32");
//	  sprintf(AtStrBuf,"AT+MSUB=\"/%s/%s/user/sub\",0\r\n","a19mET0bQ9o","STM32");
	while(CT511_SendCmd(AtStrBuf,"MSUB: SUCCESS",200)) 
		delay_ms(500);
	//printf("设备已经连接到阿里云,准备发送数据 [..]\r\n");
}




void CT511_MQTT_SEND_DATA(char *PRODUCTKEY,char *DEVICENAME,char *DATA)
{
    memset(AtStrBuf,0,BUFLEN);
   sprintf(AtStrBuf,"AT+MPUB=\"/sys/%s/%s/thing/event/property/post\",0,0,\"%s\"\r\n","a19mET0bQ9o","STM32",DATA);
//	  sprintf(AtStrBuf,"AT+MPUB=\"/%s/%s/user/sub\",0,0,\"%s\"\r\n","a19mET0bQ9o","STM32",DATA);
	CT511_SendCmd(AtStrBuf,"MPUB: SUCCESS",200);   
	USART_SendData(USART3, 0x1A); //数据结束符，需采用HEX形式发送
	//printf("用户数据发送成功  [OK]\r\n"); 
}








//==========================================================
//	函数名称：	CT511_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool CT511_WaitRecive(void)
{
	if(U3_RX_Cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
	if(U3_RX_Cnt == U3_RX_CntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		U3_RX_Cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	U3_RX_CntPre = U3_RX_Cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志
}


//==========================================================
//	函数名称：	CT511_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void CT511_Clear(void)
{
	memset(U3_RX_BUF, 0, sizeof(U3_RX_BUF));
	U3_RX_Cnt = 0;

}


//==========================================================
//	函数名称：	CT511_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool CT511_SendCmd(char *cmd, char *res, u16 time)
{	
	UART3_SendString(cmd);
	delay_ms(10);
	while(time--)
	{	
		if(CT511_WaitRecive() == REV_OK)							//如果收到数据
		{	
			if(strstr((const char *)U3_RX_BUF, res) != NULL)		//如果检索到关键词
			{
				CT511_Clear();									//清空缓存
				return 0;
			}
		}
		delay_ms(10);
	}
	return 1;
}

