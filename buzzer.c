#include "buzzer.h"
#include "main.h"
#include "delay.h"

void Buzzer_Init(void) {
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = BUZZER_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUZZER_PORT, &gpio);
    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

void Buzzer_Beep(uint32_t freq_hz, uint32_t duration_ms) {
    uint32_t half_period_us = 500000 / freq_hz;
    uint32_t total_us = duration_ms * 1000;
    uint32_t cycles = total_us / (2 * half_period_us);

    for (uint32_t i = 0; i < cycles; i++) {
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
        delay_us(half_period_us);
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
        delay_us(half_period_us);
    }
}

void Buzzer_BoxA_Alert(void) {
    for (int i = 0; i < 3; i++) {
        Buzzer_Beep(2000, 200);
        HAL_Delay(200);
    }
}

void Buzzer_BoxB_Alert(void) {
    for (int i = 0; i < 2; i++) {
        Buzzer_Beep(1000, 500);
        HAL_Delay(300);
    }
}
