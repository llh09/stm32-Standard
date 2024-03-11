#include "lcd.h"
#include "spi.h"
#include "delay.h"

void LcdInit(void)
{
	LcdSpiInit();
	
	LCD_RES_LOW;
	delay_ms(1500);
	LCD_RES_HIGH;
	delay_ms(1500);
	
	LcdWriteCommand(0x28);
	LcdWriteCommand(0x11);
	delay_ms(1200);
	
	LcdWriteCommand(0xB1);
	LcdWriteData(0x01);
	LcdWriteData(0x2C);
	LcdWriteData(0x2D);
	
	LcdWriteCommand(0xB2);
	LcdWriteData(0x01);
	LcdWriteData(0x2C);
	LcdWriteData(0x2D);
	
	LcdWriteCommand(0xB3);
	LcdWriteData(0x01);
	LcdWriteData(0x2C);
	LcdWriteData(0x2D);
	LcdWriteData(0x01);
	LcdWriteData(0x2C);
	LcdWriteData(0x2D);
	
	LcdWriteCommand(0xB4);
	LcdWriteData(0x07);
	
	LcdWriteCommand(0xC0); 
	LcdWriteData(0xA2); 
	LcdWriteData(0x02); 
	LcdWriteData(0x84); 
	LcdWriteCommand(0xC1); 
	LcdWriteData(0xC5); 

	LcdWriteCommand(0xC2); 
	LcdWriteData(0x0A); 
	LcdWriteData(0x00); 

	LcdWriteCommand(0xC3); 
	LcdWriteData(0x8A); 
	LcdWriteData(0x2A); 
	LcdWriteCommand(0xC4); 
	LcdWriteData(0x8A); 
	LcdWriteData(0xEE); 
	
	LcdWriteCommand(0xC5);//VCOM 
	LcdWriteData(0x0E); 
	
	LcdWriteCommand(0x36);//MX,MY,RGB mode 
//	LcdWriteData(0xA8);
	switch (DISPLAY_DIRECTION)
	{
		case	0: LcdWriteData(0xC8);break;
		case	1: LcdWriteData(0xA8);break;
		case  2: LcdWriteData(0x08);break;
		default	 : LcdWriteData(0x68);break;
	}

	//ST7735R Gamma Sequence
	LcdWriteCommand(0xE0); 
	LcdWriteData(0x0F); 
	LcdWriteData(0x1A); 
	LcdWriteData(0x0F); 
	LcdWriteData(0x18); 
	LcdWriteData(0x2F); 
	LcdWriteData(0x28); 
	LcdWriteData(0x20); 
	LcdWriteData(0x22); 
	LcdWriteData(0x1F); 
	LcdWriteData(0x1B); 
	LcdWriteData(0x23); 
	LcdWriteData(0x37); 
	LcdWriteData(0x00); 	
	LcdWriteData(0x07); 
	LcdWriteData(0x02); 
	LcdWriteData(0x10); 

	LcdWriteCommand(0xE1); 
	LcdWriteData(0x0F); 
	LcdWriteData(0x1B); 
	LcdWriteData(0x0F); 
	LcdWriteData(0x17); 
	LcdWriteData(0x33); 
	LcdWriteData(0x2C); 
	LcdWriteData(0x29); 
	LcdWriteData(0x2E); 
	LcdWriteData(0x30); 
	LcdWriteData(0x30); 
	LcdWriteData(0x39); 
	LcdWriteData(0x3F); 
	LcdWriteData(0x00); 
	LcdWriteData(0x07); 
	LcdWriteData(0x03); 
	LcdWriteData(0x10);  

	LcdWriteCommand(0x2A); //65k mode
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteData(0x7F);

	LcdWriteCommand(0x2B); //65k mode
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteData(0x9F);
	
	LcdWriteCommand(0xF0);
	LcdWriteData(0x01); 
	LcdWriteCommand(0xF6);
	LcdWriteData(0x00); 
	
	LcdWriteCommand(0x3A);//65k mode 
	LcdWriteData(0x05); 
	LcdWriteCommand(0x29);
	
	LcdSetReg(0,0,128,128);
	
	LcdClear(WHITE);//
	LCD_BL_HIGH;
}

void LcdWriteData(uint8_t data)
{
    LCD_CS_LOW;
    LCD_DATA;
    LcdSpiSendReceiveByte(data);
    LCD_CS_HIGH;
}

void LcdWrite16BitData(uint16_t data)
{
    uint8_t tempbuff[2];
    tempbuff[0] = data>>8;
    tempbuff[1] = data;

    LCD_CS_LOW;
    LCD_DATA;
    LcdSpiSendReceiveByte(tempbuff[0]);
    LcdSpiSendReceiveByte(tempbuff[1]);

    LCD_CS_HIGH;
}

void LcdWriteCommand(uint8_t command)
{
    LCD_CS_LOW;
    LCD_COMMAND;
    LcdSpiSendReceiveByte(command);
    LCD_CS_HIGH;
}

void LcdSetReg(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end)
{
    LcdWriteCommand(0x2a);
    LcdWriteData(0x00);
    LcdWriteData(x_start+2);
    LcdWriteData(0x00);
    LcdWriteData(x_end+2);

    LcdWriteCommand(0x2b);
    LcdWriteData(0x00);
    LcdWriteData(y_start+2);
    LcdWriteData(0x00);
    LcdWriteData(y_end+2);

    LcdWriteCommand(0x2c);
}

void LcdClear(uint16_t color)
{
	uint16_t i,j;
	LcdSetReg(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
//	LcdWriteCommand(0x2c);
	
	for(i = 0; i < X_MAX_PIXEL; i++)
	{
		for(j = 0; j < Y_MAX_PIXEL; j++)
		{
			LcdWrite16BitData(color);
		}
	}
}

void LcdDrawPoint(uint8_t x, uint8_t y, uint16_t color)
{
	LcdWriteCommand(0x2a);
	LcdWriteData(x);
	LcdWriteData(x);
	
	LcdWriteCommand(0x2b);
	LcdWriteData(y);
	LcdWriteData(y);

	LcdWriteCommand(0x2c);
	LcdWrite16BitData(color);
	
//	LcdSetReg(x,x,y,y);
//	LcdWrite16BitData(color);
}

void LcdDrawLine(uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end, uint16_t color)
{
	uint8_t i,j;
	for(i = x_start; i <= x_end; i++)//��
	{
		for(j = y_start; j <= y_end; j++)//��
		{
				LcdDrawPoint(i, j, color);
		}
	}
}

void LcdShowChinese16x16(uint8_t x, uint8_t y, uint16_t charcolor,uint16_t backcolor, uint8_t *CharCH)
{
	uint8_t i,j;
	uint32_t temp;
	
	LcdWriteCommand(0x2a);
	LcdWriteData(x);
	LcdWriteData(x+15);

	LcdWriteCommand(0x2b);
	LcdWriteData(y);
	LcdWriteData(y+15);

	LcdWriteCommand(0x2c);
	
	for(i=0;i<16;i++)
	{
		temp=CharCH[2*i]<<8|CharCH[2*i+1];
		for(j=0;j<16;j++)
		{
			if(temp &(1<<(15-j)) )
		  {
			  LcdWriteData(charcolor);
		  }
		  else
			{
				 LcdWriteData(backcolor);
			}	
		}
	}
}
















































