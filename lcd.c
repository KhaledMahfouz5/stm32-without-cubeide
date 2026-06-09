#include "lcd.h"
#include "main.h"

// LCD command set
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORON 0x02
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_FUNCTIONSET 0x20
#define LCD_8BITMODE 0x10
#define LCD_2LINE 0x08

static I2C_HandleTypeDef* lcd_i2c;

void LCD_Init(I2C_HandleTypeDef* hi2c) {
    lcd_i2c = hi2c;

    // Initialize I2C peripheral
    HAL_I2C_Init(hi2c);

    // LCD initialization sequence
    LCD_Command(LCD_CLEARDISPLAY);
    HAL_Delay(2);

    LCD_Command(LCD_FUNCTIONSET | LCD_8BITMODE | LCD_2LINE);
    HAL_Delay(5);

    LCD_Command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON);
    HAL_Delay(5);

    LCD_Command(LCD_CLEARDISPLAY);
    HAL_Delay(2);

    LCD_Command(LCD_ENTRYMODESET); // Increment cursor
}

void LCD_Command(uint8_t cmd) {
    uint8_t buffer[2] = {0x80, cmd}; // 0x80 = instruction mode
    HAL_I2C_Master_Transmit(lcd_i2c, LCD_ADDR, buffer, 2, HAL_MAX_DELAY);
}

void LCD_WriteChar(uint8_t data) {
    uint8_t buffer[2] = {0x40, data}; // 0x40 = data mode
    HAL_I2C_Master_Transmit(lcd_i2c, LCD_ADDR, buffer, 2, HAL_MAX_DELAY);
}

void LCD_Print(const char* str) {
    while (*str) {
        LCD_WriteChar(*str++);
    }
}

void LCD_Clear(void) {
    LCD_Command(LCD_CLEARDISPLAY);
}

void LCD_SetCursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x80, 0xC0};
    LCD_Command(row_offsets[row] | col);
}