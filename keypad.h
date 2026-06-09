#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f1xx_hal.h"

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

extern GPIO_TypeDef* Keypad_Row_Port[];
extern uint16_t Keypad_Row_Pin[];
extern GPIO_TypeDef* Keypad_Col_Port[];
extern uint16_t Keypad_Col_Pin[];

void Keypad_Init(void);
char Keypad_GetKey(void);

#endif