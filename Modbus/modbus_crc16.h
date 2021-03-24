#ifndef _MODBUS_CRC16_H_
#define _MODBUS_CRC16_H_

#include "stdint.h"

uint16_t modbus_crc16(uint8_t *puchMsg, uint16_t usDataLen );

#endif

