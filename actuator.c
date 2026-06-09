#include "actuator.h"
#include "main.h"

static const uint16_t box_relay_pins[] = {0, GPIO_PIN_8, GPIO_PIN_9};

void Box_Open(uint8_t box_id) {
    if (box_id < 1 || box_id > 2) return;
    HAL_GPIO_WritePin(GPIOA, box_relay_pins[box_id], GPIO_PIN_SET);
}

void Box_Close(uint8_t box_id) {
    if (box_id < 1 || box_id > 2) return;
    HAL_GPIO_WritePin(GPIOA, box_relay_pins[box_id], GPIO_PIN_RESET);
}

void StatusLED_On(void) {
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

void StatusLED_Off(void) {
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}