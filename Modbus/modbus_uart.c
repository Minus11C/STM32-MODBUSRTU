#include "modbus_uart.h"

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	
	NVIC_Init(&NVIC_InitStructure);
}

void RS485_Init()
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_AF_OD;//
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
     RS485_RT_0; //使MAX485芯片处于接收状态
	
    //USART1_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_AF_OD;//
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;//
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    //Usart1   
    USART_InitStructure.USART_BaudRate = 9600;//波特率9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
    USART_DeInit(USART2);
    USART_Init(USART2, &USART_InitStructure);
    NVIC_Configuration();
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); 
    USART_Cmd(USART2, ENABLE);
    USART_ClearFlag(USART2,USART_FLAG_TC );

}

void USART2_IRQHandler() //MODBUS字节接收中断
{
    uint8_t st,sbuf;
    st = USART_GetITStatus(USART2, USART_IT_RXNE);
    if(st == SET)  
    {   		 
        sbuf=USART2->DR;
        if( modbus.reflag==1)  //有数据包正在处理
		{
            return ;
		}			 
		modbus.rcbuf[modbus.recount++] = sbuf;
        modbus.timout=0;  
        if(modbus.recount==1)  //收到主机发来的一帧数据的第一字节
		{
			modbus.timrun=1;  //开始启动定时
		}
    } 
}

void RS485_send_byte(uint8_t d)  //485发送一个字节
{
    USART_SendData(USART2, d);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
    USART_ClearFlag(USART2,USART_FLAG_TC ); 
}

