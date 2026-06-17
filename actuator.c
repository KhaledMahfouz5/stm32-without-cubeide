#include "actuator.h"
#include "main.h"

static const struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} box_leds[] = {
    {BOX_A_LED_PORT, BOX_A_LED_PIN},
    {BOX_B_LED_PORT, BOX_B_LED_PIN}
};

void Box_LED_On(uint8_t box_id) {
    if (box_id >= 1 && box_id <= 2) {
        HAL_GPIO_WritePin(box_leds[box_id - 1].port, box_leds[box_id - 1].pin, GPIO_PIN_SET);
    }
}

void Box_LED_Off(uint8_t box_id) {
    if (box_id >= 1 && box_id <= 2) {
        HAL_GPIO_WritePin(box_leds[box_id - 1].port, box_leds[box_id - 1].pin, GPIO_PIN_RESET);
    }
}
