#include "stm32f10x.h"
#include "delay.h"
#include "uart.h"

uint8_t TestBuffer[50] = {0};

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    delay_init();
	usart1_init();
    while(1)
    {
		read_data_frame(&q_UsartBuff, TestBuffer);
		
		if( DealFlag == 1 )
		{
			usart1_send_buff(TestBuffer);
			DealFlag = 0;
		}
		
        // delay_us(20);
    }
}

