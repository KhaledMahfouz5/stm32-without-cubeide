#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f1xx_hal.h"

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

#define KEYPAD_ROW_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9)
#define KEYPAD_COL_PINS (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)

extern GPIO_TypeDef* Keypad_Row_Port[];
extern uint16_t Keypad_Row_Pin[];
extern GPIO_TypeDef* Keypad_Col_Port[];
extern uint16_t Keypad_Col_Pin[];

void Keypad_Init(void);
char Keypad_GetKey(void);

#endif