#include "lcd.h"
#include "main.h"

static I2C_HandleTypeDef* lcd_i2c;
static uint8_t lcd_backlight = LCD_BL;

static void LCD_WriteNibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = nibble | lcd_backlight | rs;

    uint8_t buf[1];
    buf[0] = data | LCD_EN;
    HAL_I2C_Master_Transmit(lcd_i2c, LCD_ADDR << 1, buf, 1, 100);

    buf[0] = data & ~LCD_EN;
    HAL_I2C_Master_Transmit(lcd_i2c, LCD_ADDR << 1, buf, 1, 100);
}

static void LCD_WriteByte(uint8_t byte, uint8_t rs) {
    uint8_t high = byte & 0xF0;
    uint8_t low = (byte << 4) & 0xF0;

    LCD_WriteNibble(high, rs);
    LCD_WriteNibble(low, rs);
}

void LCD_Init(I2C_HandleTypeDef* hi2c) {
    lcd_i2c = hi2c;

    HAL_Delay(50);

    LCD_WriteNibble(0x30, 0);
    HAL_Delay(5);
    LCD_WriteNibble(0x30, 0);
    HAL_Delay(1);
    LCD_WriteNibble(0x30, 0);
    HAL_Delay(1);
    LCD_WriteNibble(0x20, 0);
    HAL_Delay(1);

    LCD_WriteByte(0x28, 0);
    HAL_Delay(1);
    LCD_WriteByte(0x08, 0);
    HAL_Delay(1);
    LCD_WriteByte(0x01, 0);
    HAL_Delay(2);
    LCD_WriteByte(0x06, 0);
    HAL_Delay(1);
    LCD_WriteByte(0x0C, 0);
    HAL_Delay(1);
}

void LCD_Command(uint8_t cmd) {
    LCD_WriteByte(cmd, 0);
}

void LCD_WriteChar(uint8_t data) {
    LCD_WriteByte(data, LCD_RS);
}

void LCD_Print(const char* str) {
    while (*str) {
        LCD_WriteChar(*str++);
    }
}

void LCD_Clear(void) {
    LCD_Command(0x01);
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x80, 0xC0};
    LCD_Command(row_offsets[row] | col);
}
