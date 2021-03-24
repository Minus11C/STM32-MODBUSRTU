#include "modbus.h"

MODBUS modbus;


/*
因为波特率 9600
1位数据的时间为 1000000us/9600bit/s=104us
一个字节为    104us*10位  =1040us
所以 MODBUS确定一个数据帧完成的时间为   1040us*3.5=3.64ms  ->10ms
*/
void Modbus_Init(void)
{
    modbus.myadd=4;             //本从设备的地址
	modbus.timrun=0;            //MODbus定时器停止计时
    RS485_Init();
}


void Modbus_Event()
{
    uint16_t crc_cal_val;
    uint16_t crc_rec_val;
    
    if (modbus.reflag == 0) //没有收到MODBUS的数据包
    {
        return ;
    }
    
    crc_cal_val = modbus_crc16(&modbus.rcbuf[0], modbus.recount - 2);
    crc_rec_val =  modbus.rcbuf[modbus.recount -2]  << 8 | modbus.rcbuf[modbus.recount - 1];
    
    if(crc_cal_val == crc_rec_val)      //数据包符合规则
    {
        if(modbus.rcbuf[0] == modbus.myadd)     //数据是发给我的
        {
            switch(modbus.rcbuf[1])
            {
                case 0: modbus_fun0(); break;
                case 1: modbus_fun1(); break;
                case 2: modbus_fun2();break;
                case 3: modbus_fun3();break;
                case 4: modbus_fun4();break;
                case 5: modbus_fun5();break;
                case 6: modbus_fun6();break;
            }
        }
        else if (modbus.rcbuf[0] == 0)          //广播地址
        {
                
        }
    }
    
    modbus.recount = 0;     
    modbus.reflag = 0;      //置0 防止重复处理
}
