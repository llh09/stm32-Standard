#ifndef _UART_H
#define _UART_H

#include "stm32f10x.h"

#define QUEUE_BUFF_LENGTH	512

typedef struct
{
	uint16_t head;							// 队列头指针
	uint16_t tail;							// 队列尾指针
	uint16_t capacity;						// 队列长度
	uint8_t data[QUEUE_BUFF_LENGTH];		// 队列缓存
}Queue_t;

extern Queue_t q_UsartBuff;
extern uint8_t DealFlag;


void usart1_init(void);
void usart1_send_buff(uint8_t * buf);

void queue_init(Queue_t * q);
uint8_t queue_full(const Queue_t * q);
uint8_t queue_empty(const Queue_t * q);
void queue_write_byte(Queue_t * q, uint8_t data);
void queue_read_byte(Queue_t * q, uint8_t * buff);
void read_data_frame(Queue_t * q, uint8_t * buf);

#endif
