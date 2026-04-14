#include "stm32f10x.h"
#include "usart22.h"	  
#include "string.h"
#include "LED.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	  

//初始化IO 串口2 
//bound:波特率
void uart2_init(u32 bound){
    //GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2
	
		USART_DeInit(USART2);  //复位串口1
	 //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10


   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口 
	 
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 
   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);                    //使能串口 

}

//	/*-------------------------------------------------*/
///*函数名：串口2 printf函数                         */
///*参  数：char* fmt,...  格式化输出字符串和参数    */
///*返回值：无                                       */
///*-------------------------------------------------*/

__align(8) char USART2_TxBuff[USART2_TXBUFF_SIZE];  

void u2_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART2_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)USART2_TxBuff);		
	while((USART2->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART2->DR = USART2_TxBuff[i];
		while((USART2->SR&0X40)==0);	
	}	
}	

void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志
				
			USART2_RX_BUF[USART2_RX_STA] = USART_ReceiveData(USART2);     //接收串口1数据到buff缓冲区
			USART2_RX_STA++; 

				if(USART2_RX_BUF[USART2_RX_STA-1] == 0xFF || USART2_RX_STA == USART2_REC_LEN)    //如果接收到尾标识是换行符（或者等于最大接受数就清空重新接收）
				{
				 if(USART2_RX_BUF[0] == 0xFE&&USART2_RX_BUF[2] == 0xAA)                      //检测到头标识是我们需要的 
				 {
					 LED=USART2_RX_BUF[6];
					 USART2_RX_STA=0;                                   
				 }
				else 	if(USART2_RX_BUF[0] == 0xFE&&USART2_RX_BUF[2] == 0xBB &&LED==0)                      //检测到头标识是我们需要的 
				 {
					FAN=USART2_RX_BUF[6];
					USART2_RX_STA=0;                                   
				 }
				 else 	if(USART2_RX_BUF[0] == 0xFE&&USART2_RX_BUF[2] == 0xCC &&LED==0)                      //检测到头标识是我们需要的 
				 {
					RELAY=USART2_RX_BUF[6];
					USART2_RX_STA=0;                                   
				 }
				 else 	if(USART2_RX_BUF[0] == 0xFE&&USART2_RX_BUF[2] == 0xDD &&LED==0)                      //检测到头标识是我们需要的 
				 {
					JS=USART2_RX_BUF[6];
					USART2_RX_STA=0;                                   
				 }
				 else
				 {
					USART2_RX_STA=0;                                   //不是我们需要的数据或者达到最大接收数则开始重新接收
				 }
				}
		}
}

