#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <stdint.h>
#include "stm32f1xx_hal.h"

#define BOX_1 1
#define BOX_2 2

#define BOX_A_LED_PIN GPIO_PIN_10
#define BOX_A_LED_PORT GPIOB
#define BOX_B_LED_PIN GPIO_PIN_11
#define BOX_B_LED_PORT GPIOB

void Box_LED_On(uint8_t box_id);
void Box_LED_Off(uint8_t box_id);

#endif
