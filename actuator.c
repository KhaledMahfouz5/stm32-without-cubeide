#include "actuator.h"
#include "main.h"

void Box_LED_On(uint8_t box_id) {
    if (box_id == BOX_1) {
        HAL_GPIO_WritePin(BOX_A_LED_PORT, BOX_A_LED_PIN, GPIO_PIN_SET);
    } else if (box_id == BOX_2) {
        HAL_GPIO_WritePin(BOX_B_LED_PORT, BOX_B_LED_PIN, GPIO_PIN_SET);
    }
}

void Box_LED_Off(uint8_t box_id) {
    if (box_id == BOX_1) {
        HAL_GPIO_WritePin(BOX_A_LED_PORT, BOX_A_LED_PIN, GPIO_PIN_RESET);
    } else if (box_id == BOX_2) {
        HAL_GPIO_WritePin(BOX_B_LED_PORT, BOX_B_LED_PIN, GPIO_PIN_RESET);
    }
}
