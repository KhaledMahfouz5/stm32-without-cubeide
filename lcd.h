#ifndef LCD_H
#define LCD_H

#include "stm32f1xx_hal.h"

#define LCD_ADDR 0x27

#define LCD_BL 0x08
#define LCD_EN 0x04
#define LCD_RW 0x02
#define LCD_RS 0x01

void LCD_Init(I2C_HandleTypeDef* hi2c);
void LCD_Command(uint8_t cmd);
void LCD_WriteChar(uint8_t data);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t col, uint8_t row);
void LCD_Print(const char* str);

#endif
