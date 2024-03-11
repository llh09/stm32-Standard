#include "uart.h"

Queue_t q_UsartBuff;

static void usart1_gpio_config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStructure;
	
    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX	  GPIOA.10
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void usart1_mode_config(void)
{
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitStructure.USART_BaudRate 				= 115200;
    USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
    USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
    USART_InitStructure.USART_Parity 				= USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

static void usart1_nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
    NVIC_InitStructure.NVIC_IRQChannel 					  = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 		  = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd 				  = ENABLE;
	
    NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler(void)
{
	uint8_t temp;
	
    if( USART_GetITStatus(USART1, USART_IT_RXNE) != RESET )
    {
		temp = USART_ReceiveData(USART1);
		queue_write_byte(&q_UsartBuff, temp);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

void usart1_send_buff(uint8_t * buf)
{
    uint8_t length = ((buf[2] << 8) | buf[3]) + 6;

    for( uint8_t i = 0; i < length; i++ )
    {
        USART_SendData(USART1, buf[i]);
        while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
}

void usart1_init(void)
{
	usart1_gpio_config();
	usart1_mode_config();
	usart1_nvic_config();
	
	queue_init(&q_UsartBuff);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void queue_init(Queue_t * q)
{
	q->head = 0;
	q->tail = 0;
	q->capacity = QUEUE_BUFF_LENGTH;
}

uint8_t queue_full(const Queue_t * q)
{
	return (((q->tail+1)%q->capacity) == (q->head));
}

uint8_t queue_empty(const Queue_t * q)
{
	return (q->tail == q->head);
}

void queue_write_byte(Queue_t * q, uint8_t data)
{
	q->data[q->tail] = data;
	q->tail = (q->tail + 1) % q->capacity;
}

void queue_read_byte(Queue_t * q, uint8_t * buff)
{
	*buff = q->data[q->head];
	q->head = (q->head + 1) % q->capacity;
}

uint8_t frame_h;
uint8_t frame_t;
uint8_t temp1, temp2;
uint16_t temp3, temp4;
uint8_t receive_frame(Queue_t * q)
{
    temp1 = q->data[(q->head+2) % q->capacity];       // 数据长度高位
    temp2 = q->data[(q->head+3) % q->capacity];       // 数据长度地位
    temp3 = q->head % q->capacity;                      // 帧头地址
    temp4 = ((temp1 << 8) | temp2);                     // 数据长度
	temp4 = (temp4 + temp3 + 5) % q->capacity;      // 帧尾地址
	
    frame_h = q->data[temp3];
    frame_t = q->data[temp4];

    if( frame_h == 0x02 && frame_t == 0x03 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t DealFlag = 0;
uint16_t length;	// 数据帧长度
uint8_t temp[50];		// 读一字节数据缓存
uint8_t a;
void read_data_frame(Queue_t * q, uint8_t * buf)
{
    uint8_t index1 = 0;
    a = receive_frame(q);
    if( queue_empty(q) != 1 && receive_frame(q) == 1 )
    {
        queue_read_byte(q, &temp[index1]);

        if( temp[0] == 0x02 )
        {
            for( index1 = 1; index1 < 4; index1++ )
            {
                queue_read_byte(q, &temp[index1]);
            }

            length = (temp[2] << 8) | temp[3];

            for( index1 = 4; index1 < (length+6); index1++ )
            {
                queue_read_byte(q, &temp[index1]);
            }

            for( index1 = 0; index1 < 50; index1++ )
            {
                buf[index1] = temp[index1];
            }

            if( (buf[0] == 0x02) && (buf[length+5] == 0x03) )
            {
                DealFlag = 1;
            }
            for( index1 = 0; index1 < 50; index1++ )
            {
                temp[index1] = 0x00;
            }    
        }
    }
}
