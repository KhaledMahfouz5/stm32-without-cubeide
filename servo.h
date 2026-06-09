#ifndef SERVO_H
#define SERVO_H

#include "stm32f1xx_hal.h"

#define SERVO_CHANNEL_1 1
#define SERVO_CHANNEL_2 2

void Servo_Init(TIM_HandleTypeDef* htim);
void Servo_Write(uint8_t channel, uint16_t angle);

#endif