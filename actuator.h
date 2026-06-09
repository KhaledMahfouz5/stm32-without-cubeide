#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <stdint.h>

#define BOX_1 1
#define BOX_2 2

void Box_Open(uint8_t box_id);
void Box_Close(uint8_t box_id);
void StatusLED_On(void);
void StatusLED_Off(void);

#endif