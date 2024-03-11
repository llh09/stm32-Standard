#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"

#define LED1_GPIO_PORT    	GPIOB
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB
#define LED1_GPIO_PIN		GPIO_Pin_5

#define LED2_GPIO_PORT    	GPIOB
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOB
#define LED2_GPIO_PIN		GPIO_Pin_0

#define LED3_GPIO_PORT    	GPIOB
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOB
#define LED3_GPIO_PIN		GPIO_Pin_1

#define ON  0
#define OFF 1

#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)

#define	digitalHi(p,i)      {p->BSRR=i;}
#define digitalLo(p,i)      {p->BRR=i;}
#define digitalToggle(p,i)  {p->ODR ^=i;}


#define LED1_TOGGLE     digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF        digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON         digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE     digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF        digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON         digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE     digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF        digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON         digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)

#define LED_RED  \
					LED1_ON;\
					LED2_OFF\
					LED3_OFF


#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON\
					LED3_OFF


#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF\
					LED3_ON

					
				
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON\
					LED3_OFF

#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF\
					LED3_ON


#define LED_CYAN \
					LED1_OFF;\
					LED2_ON\
					LED3_ON
					

#define LED_WHITE	\
					LED1_ON;\
					LED2_ON\
					LED3_ON
					

#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF\
					LED3_OFF




typedef struct
{
	GPIO_TypeDef* port;
	uint16_t pin;
	GPIOMode_TypeDef mode;
    GPIOSpeed_TypeDef speed;
}IOCfg_Type;

typedef union
{
    uint16_t all;
    struct
    {
        uint16_t in1	:1;
        uint16_t in2	:1;
        uint16_t in3 	:1;
        
        uint16_t rcv			:13;
    }bits;
}InputSignal_Type;

extern InputSignal_Type filterInputSignal, realInputSignal, risingInputSignal, fallingInputSignal;

void Dio_Init(void);
void InputCheck(void);

#endif /* __LED_H */
