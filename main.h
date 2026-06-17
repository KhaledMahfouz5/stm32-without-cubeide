#ifndef MAIN_H
#define MAIN_H

#include "stm32f1xx_hal.h"
#include "dht11.h"

extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;
extern DHT11_HandleTypeDef dht11;

void state_machine_init(void);
void state_machine_run(void);

#endif
