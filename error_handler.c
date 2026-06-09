#include "main.h"
#include "lcd.h"

void error_handler(const char* msg) {
    // Turn on LED and halt
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

    // Display error message on LCD
    LCD_Clear();
    LCD_Print(msg);

    while(1);
}