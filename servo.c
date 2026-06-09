#include "servo.h"

#define SERVO_MIN_PULSE 500
#define SERVO_MAX_PULSE 2500

static TIM_HandleTypeDef* servo_htim;

static uint16_t map_angle_to_pulse(uint16_t angle) {
    return (angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180) + SERVO_MIN_PULSE;
}

void Servo_Init(TIM_HandleTypeDef* htim) {
    servo_htim = htim;

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1500;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(servo_htim, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(servo_htim, &sConfigOC, TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(servo_htim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(servo_htim, TIM_CHANNEL_2);
}

void Servo_Write(uint8_t channel, uint16_t angle) {
    uint32_t pulse_width = map_angle_to_pulse(angle);
    uint32_t timer_counts = (pulse_width * servo_htim->Init.Period) / 20000;

    if (channel == SERVO_CHANNEL_1) {
        __HAL_TIM_SET_COMPARE(servo_htim, TIM_CHANNEL_1, timer_counts);
    } else if (channel == SERVO_CHANNEL_2) {
        __HAL_TIM_SET_COMPARE(servo_htim, TIM_CHANNEL_2, timer_counts);
    }
}