#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

void DWT_Init(void);
void delay_us(uint32_t us);

#endif
