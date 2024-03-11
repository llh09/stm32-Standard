#include "Dio.h"

InputSignal_Type filterInputSignal, realInputSignal, risingInputSignal, fallingInputSignal;
uint8_t timeCount[16];
uint16_t inputBuf[16];

#define OUT_NUM 9

const IOCfg_Type out[OUT_NUM] = 
{
    // LED
	{GPIOB, GPIO_Pin_5,  GPIO_Mode_Out_PP, GPIO_Speed_50MHz},  /* PB.5  LED1 */
	{GPIOB, GPIO_Pin_1,  GPIO_Mode_Out_PP, GPIO_Speed_50MHz},  /* PB.1  LED2 */
	{GPIOB, GPIO_Pin_0,  GPIO_Mode_Out_PP, GPIO_Speed_50MHz},  /* PB.0  LED3 */

    // LCD
	{GPIOA, GPIO_Pin_4,  GPIO_Mode_Out_PP, GPIO_Speed_50MHz},  /* PA.4  LCD CS     */
	{GPIOE, GPIO_Pin_5,  GPIO_Mode_Out_PP, GPIO_Speed_50MHz},  /* PE.5  LCD RES    */
	{GPIOE, GPIO_Pin_6,  GPIO_Mode_Out_PP, GPIO_Speed_50MHz},  /* PE.6  LCD DC     */
    {GPIOC, GPIO_Pin_13, GPIO_Mode_Out_PP, GPIO_Speed_50MHz},  /* PC.13 LCD BL     */
    {GPIOA, GPIO_Pin_5,  GPIO_Mode_AF_PP,  GPIO_Speed_50MHz},  /* PA.5  SPI1 SCK   */
    {GPIOA, GPIO_Pin_7,  GPIO_Mode_AF_PP,  GPIO_Speed_50MHz},  /* PA.5  SPI1 MOSI  */
};

const IOCfg_Type in[2] = 
{
    // KEY
	{GPIOC, GPIO_Pin_13, GPIO_Mode_IN_FLOATING},/* PC.13 KEY2 */
	{GPIOA, GPIO_Pin_0, GPIO_Mode_IN_FLOATING}, /* PA.0  KEY1 */
};

void Dio_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | \
                           RCC_APB2Periph_GPIOA | \
                           RCC_APB2Periph_GPIOC | \
                           RCC_APB2Periph_GPIOE, ENABLE);

    for( uint8_t i = 0; i < OUT_NUM; i++ )
    {
        GPIO_InitStructure.GPIO_Pin = out[i].pin;
        GPIO_InitStructure.GPIO_Mode = out[i].mode;
        GPIO_InitStructure.GPIO_Speed = out[i].speed;
        GPIO_Init(out[i].port, &GPIO_InitStructure);
    }

    for( uint8_t i = 0; i < 2; i++ )
    {
        GPIO_InitStructure.GPIO_Pin = in[i].pin;
        GPIO_InitStructure.GPIO_Mode = in[i].mode;
        GPIO_Init(in[i].port, &GPIO_InitStructure);
    }
}
uint8_t ia;
void InputCheck(void)
{
    uint8_t i = 0;

	realInputSignal.bits.in1 = (GPIO_ReadInputDataBit(in[0].port, in[0].pin))  == 1;
	realInputSignal.bits.in2 = (GPIO_ReadInputDataBit(in[1].port, in[1].pin))  == 1;
	
	for(i = 0; i < 16; i++)
	{
		if(realInputSignal.all & (0x0001u << i))
		{
			inputBuf[i] |= 0x0001u;
			timeCount[i]++;
			if(timeCount[i] >= 5)
			{
				filterInputSignal.all |= (0x0001u << i);
				timeCount[i] = 5u;
			}
		}
		else
		{
			timeCount[i] = 0;
			filterInputSignal.all &= ~(0x0001u << i);
			inputBuf[i] &= 0xFFFEu;
		}
		if((inputBuf[i] & 0x0003u) == 0x0001u)
			risingInputSignal.all |= (0x0001u << i);
		if((inputBuf[i] & 0x0003u) == 0x0002u)
			fallingInputSignal.all |= (0x0001u << i);
		inputBuf[i] <<= 1u;
	}
}


