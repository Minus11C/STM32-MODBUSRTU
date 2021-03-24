#ifndef __MODBUS_UART_H_
#define __MODBUS_UART_H_

#include "stm32f10x_conf.h"
#include "modbus.h"

#define RS485_RT_1 GPIO_SetBits(GPIOA, GPIO_Pin_5)     //485·¢ËÍ×´Ì¬
#define RS485_RT_0 GPIO_ResetBits(GPIOA, GPIO_Pin_5)   //485ÖÃ½ÓÊÕ×´Ì¬

void RS485_Init(void);
void RS485_send_byte(uint8_t d);

#endif
