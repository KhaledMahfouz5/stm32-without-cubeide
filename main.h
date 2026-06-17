#ifndef MAIN_H
#define MAIN_H

#include "stm32f1xx_hal.h"

#define BOX_A_LED_PIN GPIO_PIN_10
#define BOX_A_LED_PORT GPIOB
#define BOX_B_LED_PIN GPIO_PIN_11
#define BOX_B_LED_PORT GPIOB

#define KEYPAD_ROW_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9)
#define KEYPAD_COL_PINS (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)

extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;

void state_machine_init(void);
void state_machine_run(void);

#endif