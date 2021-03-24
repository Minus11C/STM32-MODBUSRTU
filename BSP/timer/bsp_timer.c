#include "bsp_timer.h"

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;	
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	
	NVIC_Init(&NVIC_InitStructure);
}

void Timer2_Init()    //1ms产生1次更新事件
{
	TIM_TimeBaseInitTypeDef timer;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);
	timer.TIM_Period=1000-1;//   1ms
	timer.TIM_Prescaler=72-1;// 72M/72=1MHZ-->1us
	timer.TIM_ClockDivision=TIM_CKD_DIV1;
	timer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&timer);
	
	TIM_Cmd(TIM2,ENABLE);
	NVIC_Configuration();
	TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
}

void TIM2_IRQHandler()//定时器2的中断服务子函数  1ms一次中断
{
    uint8_t st;
    st = TIM_GetFlagStatus(TIM2, TIM_FLAG_Update);	
	if(st == SET)
	{
	    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
        if(modbus.timrun != 0)
        {
            modbus.timout++;
            if(modbus.timout >= 4)                  //间隔达到了4个ms 
            {
                modbus.timrun = 0;                  //停止定时
                modbus.reflag = 1;                  //收到一帧数据
            }
        }
	}	
}


