#ifndef __MODBUS_H_
#define __MODBUS_H_

#include <stdint.h>
#include "modbus_uart.h"
#include "modbus_crc16.h"
#include "modbus_function.h"

typedef struct
{
    uint8_t myadd;             //本设备的地址
    uint8_t rcbuf[100];        //MODBUS接收缓冲区
    uint8_t timout;            //MODbus的数据断续时间 判断是否超过3.5个字符时间(波特率为9600的时候 3.5个字符的时间为3.64ms),从而判断是否接收完成	
    uint8_t recount;           //MODbus端口已经收到的数据个数
    uint8_t timrun;            //MODbus定时器是否计时的标志
    uint8_t  reflag;           //收到一帧数据的标志
    uint8_t Sendbuf[100];      //MODbus发送缓冲区	

}MODBUS;

extern MODBUS modbus;

void Modbus_Init(void);
void Modbus_Event(void);


#endif
