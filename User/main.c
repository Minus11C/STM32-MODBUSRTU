#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_timer.h"
#include "modbus.h"

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
    
    Timer2_Init();
    Modbus_Init();
    
    while(1)
    {
        Modbus_Event();
    }
}

