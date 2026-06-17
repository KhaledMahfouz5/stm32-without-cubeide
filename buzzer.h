#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

#define BUZZER_PIN  GPIO_PIN_2
#define BUZZER_PORT GPIOA

void Buzzer_Init(void);
void Buzzer_Beep(uint32_t freq_hz, uint32_t duration_ms);
void Buzzer_BoxA_Alert(void);
void Buzzer_BoxB_Alert(void);

#endif
