#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <stdint.h>

#define BOX_1 1
#define BOX_2 2

void Box_LED_On(uint8_t box_id);
void Box_LED_Off(uint8_t box_id);

#endif
