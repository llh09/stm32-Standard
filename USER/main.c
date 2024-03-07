#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"

int main(void)
{
	delay_init();
	uart_init(115200);
	
	printf("ok \r\n");
	while(1)
	{
		// to do
	}
}

