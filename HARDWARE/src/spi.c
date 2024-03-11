#include "spi.h"

void LcdSpiInit(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1, ENABLE );	

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 0;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t  LcdSpiSendReceiveByte(uint8_t data )
{
	while(SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_TXE);
	SPI_I2S_SendData(SPI1, data);

	while(SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_RXNE)==RESET);//   
	SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_RXNE);
	return SPI_I2S_ReceiveData(SPI1);
}
