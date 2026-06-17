#include "main.h"
#include "lcd.h"

void error_handler(const char* msg) {
    LCD_Clear();
    LCD_Print(msg);

    while(1);
}