#ifndef MAIN_H
#define MAIN_H

#include "stm32f1xx_hal.h"

#define LED_PIN GPIO_PIN_13
#define LED_PORT GPIOC

#define KEYPAD_ROW_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
#define KEYPAD_COL_PINS (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)

extern UART_HandleTypeDef uart2;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;

void state_machine_init(void);
void state_machine_run(void);

#endif