#include "modbus_function.h"

/*模拟寄存器的值的数组 ,实际根据传感器得到值*/
uint16_t reg[11] = {0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x00a0};

void modbus_fun0(void)
{
}

void modbus_fun1(void)
{
}

void modbus_fun2(void)
{
}

/*主机要读取本 从机的寄存器*/
void modbus_fun3(void)
{
    uint16_t reg_add;   //要从那个地址开始读
    uint16_t reg_cont;  //要读多少个
    uint16_t i,j = 0;
    uint16_t byte;      //要返回的数据字节数
    uint16_t crc_send_val;
    
    reg_add = modbus.rcbuf[2] << 8 | modbus.rcbuf[3];
    reg_cont = modbus.rcbuf[4] << 8 | modbus.rcbuf[5];
    
    modbus.Sendbuf[i++] = modbus.myadd;
    modbus.Sendbuf[i++] = 0x03;
    byte = reg_cont * 2;
    modbus.Sendbuf[i++] = byte >> 8;  //要返回数据字节数的高8位
    modbus.Sendbuf[i++] = byte & 0x00ff;
    
    for(j = 0; j < reg_cont; j++)
    {
        modbus.Sendbuf[i++] = reg[j+reg_add] >> 8;
        modbus.Sendbuf[i++] = reg[j+reg_add] & 0x00ff;
    }
    
    crc_send_val = modbus_crc16(&modbus.Sendbuf[0],i);
    
    modbus.Sendbuf[i++] = crc_send_val >> 8;
    modbus.Sendbuf[i++] = crc_send_val & 0x00ff;
    
    RS485_RT_1;
    for(j = 0; j < i; j++)
    {
        RS485_send_byte(modbus.Sendbuf[j]);
    }
    RS485_RT_0;
    
}
void modbus_fun4(void)
{
    
}
    
void modbus_fun5(void)
{
}
    
void modbus_fun6(void)
{
    uint16_t Regadd;
	uint16_t val;
	uint16_t i,crc,j;
	i=0;
    Regadd=modbus.rcbuf[2] << 8 | modbus.rcbuf[3];  //得到要修改的地址 
	val=modbus.rcbuf[4] << 8 | modbus.rcbuf[5];     //修改后的值
	reg[Regadd] = val;  //修改本设备相应的寄存器
	
	//以下为回应主机
	
	modbus.Sendbuf[i++]=modbus.myadd;//本设备地址
    modbus.Sendbuf[i++]=0x06;        //功能码 
    modbus.Sendbuf[i++]=Regadd >> 8;
	modbus.Sendbuf[i++]=Regadd & 0x00ff;
	modbus.Sendbuf[i++]=val >> 8;
	modbus.Sendbuf[i++]=val & 0x00ff;
	crc=modbus_crc16(modbus.Sendbuf,i);
	modbus.Sendbuf[i++]=crc >> 8;  
	modbus.Sendbuf[i++]=crc & 0x00ff;
	
	RS485_RT_1;  //
	
	for(j=0;j<i;j++)
	{
        RS485_send_byte(modbus.Sendbuf[j]);
	}
	
	RS485_RT_0;  //
}
