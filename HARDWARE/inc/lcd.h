#ifndef _LCD_H
#define _LCD_H

#include "stm32f10x.h"

/********************************************************************/
#define LCD_CS_LOW    GPIO_ResetBits(GPIOA,GPIO_Pin_4);
#define LCD_CS_HIGH   GPIO_SetBits(GPIOA,GPIO_Pin_4);

#define LCD_COMMAND   GPIO_ResetBits(GPIOE,GPIO_Pin_6);
#define LCD_DATA      GPIO_SetBits(GPIOE,GPIO_Pin_6);

#define LCD_RES_LOW   GPIO_ResetBits(GPIOE,GPIO_Pin_5);
#define LCD_RES_HIGH  GPIO_SetBits(GPIOE,GPIO_Pin_5);

#define LCD_BL_LOW    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
#define LCD_BL_HIGH   GPIO_SetBits(GPIOC,GPIO_Pin_13);
/********************************************************************/
#define DISPLAY_DIRECTION 0

#define X_MAX_PIXEL 128
#define Y_MAX_PIXEL 128

#define RED  	  0xf800
#define GREEN	  0x07e0
#define BLUE 	  0x001f
#define WHITE	  0xffff
#define BLACK	  0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D
#define GRAY1   0x8410
#define GRAY2   0x4208

#define ST7735_COLUMN_ADDRESS_SET 0x24
#define ST7735_ROW_ADDRESS_SET    0x2B
#define ST7735_DISPLAY_ON         0x29
#define ST7735_SLEEP_OUT          0x11

/********************************************************************/
void LcdInit(void);
void LcdWriteData(uint8_t data);
void LcdWrite16BitData(uint16_t data);
void LcdWriteCommand(uint8_t command);
void LcdSetReg(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end);
void LcdClear(uint16_t color);
uint8_t  LcdSpiSendReceiveByte(uint8_t data );
void LcdDrawPoint(uint8_t x, uint8_t y, uint16_t color);
void LcdDrawLine(uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end, uint16_t color);
void LcdShowChinese16x16(uint8_t x, uint8_t y, uint16_t charcolor,uint16_t backcolor, uint8_t *CharCH);

#endif
